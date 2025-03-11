#include "updatesocket.h"
#include "protocolcommand.h"
#include <QDir>

UpdateSocket::UpdateSocket(int ID, QObject *parent)
    : QTcpSocket{parent}, socketID(ID)
{
    clearInput();
    clearOutput();
    connect(this,&UpdateSocket::readyRead,this,&UpdateSocket::readMessage);
}

UpdateSocket::~UpdateSocket()
{

}

void UpdateSocket::requestFileList(TransferHeader::FileType fileType)
{
    sendMessageOnly("", _REQUEST_LIST_, fileType);
}

void UpdateSocket::sendFile(const QString& path)
{
    clearOutput();
    outputHeader.messageSize = 0;
    outputHeader.fileSize = 0;
    outputHeader.command = _TRANSFER_FILE_;

    outputHeader.bytesReadOrWritten = 0;
    outputFile.localFile.reset(new QFile(path));

    if(!outputFile.localFile->open(QFile::ReadOnly))
    {
        outputFile.localFile.reset(nullptr);
        return;
    }
    outputHeader.fileSize  = outputFile.localFile->size();

    QDataStream outStream(&outputHeader.dataBlock,QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_15);

    QString currentFilename = path.right(path.size() - path.lastIndexOf('/')-1);
    outputHeader.message = currentFilename;

    outStream << qint64(0)
              << qint64(0)
              << qint64(0)
              << qint64(0)
              << qint64(0);
    if (!outputHeader.message.isEmpty())
        outStream << outputHeader.message;

    outputHeader.bytesToReadOrWrite += outputHeader.dataBlock.size();
    outputHeader.messageSize = outputHeader.dataBlock.size() - headerSizeBytes;
    outStream.device()->seek(0);

    outStream << outputHeader.magic
              << outputHeader.command
              << outputHeader.messageSize
              << outputHeader.fileSize
              << outputHeader.fileType;

    connect(this, &UpdateSocket::bytesWritten, this, &UpdateSocket::sendFilePart);
    write(outputHeader.dataBlock.constData(), outputHeader.bytesToReadOrWrite);
    waitForBytesWritten();

}

void UpdateSocket::sendMessageOnly(const QString &message, qint64 command, TransferHeader::FileType fileType)
{
    clearOutput();
    outputHeader.messageSize = 0;
    outputHeader.fileSize = 0;
    outputHeader.fileType = fileType;
    outputHeader.command = command;

    outputHeader.bytesReadOrWritten = 0;

    QDataStream outStream(&outputHeader.dataBlock,QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_15);

    outputHeader.message = message;

    outStream << qint64(0)
              << qint64(0)
              << qint64(0)
              << qint64(0)
              << qint64(0);
    if (!outputHeader.message.isEmpty())
        outStream << outputHeader.message;

    outputHeader.bytesToReadOrWrite += outputHeader.dataBlock.size();
    outputHeader.messageSize = outputHeader.dataBlock.size() - headerSizeBytes;
    outStream.device()->seek(0);

    outStream << outputHeader.magic
              << outputHeader.command
              << outputHeader.messageSize
              << qint64(0)
              << outputHeader.fileType;

    write(outputHeader.dataBlock.constData(), outputHeader.bytesToReadOrWrite);
    waitForBytesWritten();
}

void UpdateSocket::sendFileList(QStringList list)
{
    sendMessageOnly(list.join('%'), _TRANSFER_LIST_);
}

void UpdateSocket::requestFile(const QString &name)
{
    sendMessageOnly(name, _SELECT_FILE_);
}

void UpdateSocket::sendFilePart()
{
    outputHeader.dataBlock.clear();
    outputHeader.dataBlock.resize(payloadSize);

    if(!outputFile.localFile->atEnd()){
        qint64 in = outputFile.localFile->read(outputHeader.dataBlock.data(), payloadSize);
        write(outputHeader.dataBlock.constData(), in);

    } else {
        outputFile.localFile->close();
        outputFile.localFile.reset(nullptr);
        disconnect(this, &UpdateSocket::bytesWritten, this, &UpdateSocket::sendFilePart);
        clearOutput();
    }
}


void UpdateSocket::readMessage()
{
    if(bytesAvailable() <= 0) {
        return;
    }

    QDataStream inStream(this);
    inStream.setVersion(QDataStream::Qt_5_15);

    if(bytesAvailable() >= headerSizeBytes
        && (inputHeader.command == 0)) {

        inStream >> inputHeader.magic
            >> inputHeader.command
            >> inputHeader.messageSize
            >> inputHeader.fileSize
            >> inputHeader.fileType;

        inputHeader.bytesReadOrWritten += headerSizeBytes;
    }
    inputHeader.bytesToReadOrWrite = inputHeader.messageSize
                                     + inputHeader.fileSize
                                     + headerSizeBytes;

    if(bytesAvailable() >= inputHeader.messageSize
        && inputHeader.message.isEmpty()) {
        inStream >> inputHeader.message;
        inputHeader.bytesReadOrWritten += inputHeader.messageSize;
    }
    //тут мы закончили читать хэдер


    switch(inputHeader.command)
    {
    case _TRANSFER_FILE_ :
    {
        //тут нам теперь надо прочитать входящий файл
        if (bytesAvailable())
            inputHeader.dataBlock = readAll();
        recieveFile(inputHeader.message);
    }
    break;
    case _TRANSFER_LIST_ :
    {
        emit listRecieved(inputHeader.message.split('%'));
        clearInput();
    }
    break;
    case _SELECT_FILE_:
    {
        emit fileRequested(inputHeader.message);
        clearInput();
    }
    case _REQUEST_LIST_:
    {
        emit listRequested(inputHeader.fileType);
        clearInput();
    }
    break;
    default:
        qDebug()<<"Receive command nulity!";
        break;
    }
}



void UpdateSocket::recieveFile(const QString& fileName) {

    if (inputFile.localFile.isNull()) {
        QString savePath = "/home/kikorik/garbage/";

        QDir dir;

        dir.mkpath(savePath);
        inputFile.awaitedSize = inputHeader.fileSize;
        inputFile.localFile.reset( new QFile(savePath + fileName));
        inputFile.localFile->open(QIODevice::WriteOnly);
        inputFile.bytesRecived = 0;
    }

    recieveFile();

}



void UpdateSocket::recieveFile() {

    QDataStream in(this);

    if(!inputHeader.dataBlock.isEmpty()){

        inputFile.localFile->write(inputHeader.dataBlock);

        inputFile.bytesRecived += inputHeader.dataBlock.size();

        inputHeader.dataBlock.clear();
    }

    while(!in.atEnd()){
        inputHeader.dataBlock.resize(payloadSize);
        qint64 toFile = in.readRawData(inputHeader.dataBlock.data(), inputHeader.dataBlock.size());

        inputFile.bytesRecived += toFile;

        inputFile.localFile->write(inputHeader.dataBlock.constData(), toFile);
        inputHeader.dataBlock.clear();
    }

    if(inputFile.bytesRecived == inputFile.awaitedSize){

        inputFile.localFile->close();
        inputFile.localFile.reset(nullptr);

        inputFile.bytesRecived = 0;

        inputFile.awaitedSize = 0;
        emit fileRecieved(inputHeader.message);
        // clearInput();
    }
}

void UpdateSocket::clearOutput()
{
    outputHeader.bytesReadOrWritten = 0;
    outputHeader.bytesToReadOrWrite = 0;
    outputHeader.command = 0;
    outputHeader.dataBlock.clear();
    // outputHeader.dataBlock.resize(0)
    outputHeader.fileSize = 0;
    outputHeader.message.clear();
    outputHeader.messageSize = 0;

    outputFile.localFile.reset(nullptr);
    outputFile.awaitedSize = 0;
    outputFile.bytesRecived = 0;
}

void UpdateSocket::clearInput()
{
    inputHeader.bytesReadOrWritten = 0;
    inputHeader.bytesToReadOrWrite = 0;
    inputHeader.command = 0;
    inputHeader.dataBlock.clear();
    // outputHeader.dataBlock.resize(0)
    inputHeader.fileSize = 0;
    inputHeader.message.clear();
    inputHeader.messageSize = 0;

    inputFile.localFile.reset(nullptr);
    inputFile.awaitedSize = 0;
    inputFile.bytesRecived = 0;
}
