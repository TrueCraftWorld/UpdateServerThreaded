#include "updatesocket.h"
#include "protocolcommand.h"

UpdateSocket::UpdateSocket(int ID, QObject *parent)
    : QTcpSocket{parent}, socketID(ID)
{
    data.payloadSize = 1024 * 64;//64k
    clear();
    connect(this,&UpdateSocket::readyRead,this,&UpdateSocket::receiveData);
    connect(this, &UpdateSocket::bytesWritten, this, &UpdateSocket::sendData);
}

UpdateSocket::~UpdateSocket()
{

}

void UpdateSocket::sendFile(const QString& path)
{
    data.bytesWritten = 0;
    data.fileName = path;
    data.localFile.reset(new QFile(path));
    if(!data.localFile->open(QFile::ReadOnly))
    {
        data.localFile.reset(nullptr);
        return;
    }
    data.totalBytes  = data.localFile->size();

    QDataStream sendOut(&data.dataBlockOutput,QIODevice::WriteOnly);

    sendOut.setVersion(QDataStream::Qt_5_15);

    QString currentFilename = data.fileName.right(data.fileName.size()
                                                          -data.fileName.lastIndexOf('/')-1);


    sendOut << qint64(0) << qint64(0) << qint64(0)<< currentFilename;
    data.totalBytes += data.dataBlockOutput.size();

    sendOut.device()->seek(0);

    sendOut << data.totalBytes<<_TRANSFER_FILE_
            <<qint64((data.dataBlockOutput.size()-(sizeof(qint64)*3)));
    data.commandOutput = _TRANSFER_FILE_;
    data.headerSize = data.dataBlockOutput.size();
    data.lastOutputSize = data.headerSize;
    write(data.dataBlockOutput.constData(), data.headerSize);
}

void UpdateSocket::sendFileList(QStringList list)
{
    QDataStream sendOut(&data.dataBlockOutput,QIODevice::WriteOnly);

    sendOut.setVersion(QDataStream::Qt_5_15);

    sendOut << qint64(0) << qint64(0) << qint64(0)<< list.join('%');
    data.totalBytes += data.dataBlockOutput.size();

    sendOut.device()->seek(0);

    sendOut << data.totalBytes<<_TRANSFER_LIST_
            <<qint64((data.dataBlockOutput.size()-(sizeof(qint64)*3)));
    data.commandOutput = _TRANSFER_LIST_;
    write(data.dataBlockOutput.constData(), data.dataBlockOutput.size());
}

void UpdateSocket::clear()
{
    data.totalBytes = 0;
    data.bytesReceived = 0;
    data.fileNameSize = 0;
    data.dataBlockOutput.resize(0);
    data.bytesWritten = 0;
    data.fileName = "";
    data.localFile.reset(nullptr);
    data.totalBytes = 0;
    data.commandInput = 0;
    data.commandOutput = 0;
}


void UpdateSocket::receiveData()
{
    ///TODO - переделать эти дурачкие флаги в переменную состояния
    int downflag = 0  ,  synfilelistflag = 0,   transferfileflag = 0;
    int selectflag = 0;
    qint32 temp;

    if(bytesAvailable() <= 0) {
        return;
    }

    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_15);

    if(data.bytesReceived <= sizeof(qint64)*3) {
        if(bytesAvailable()>=sizeof(qint64)*3
            &&(data.fileNameSize==0)) {
            in >> data.totalBytes >> data.commandInput
                >> data.fileNameSize >> temp;
            data.bytesReceived += sizeof(qint64)*3;
        }
        if(bytesAvailable() >= data.fileNameSize
            && data.fileNameSize!=0) {
            in >> data.fileName;
            data.bytesReceived += data.fileNameSize;
        }
    }

    switch(data.commandInput)
    {
        case _TRANSFER_FILE_ :
        {
            transferfileflag = 1;
            if(data.fileNameSize != 0)
            {//пока не принимаем данные на сервере
            }
        }
        break;
        case _TRANSFER_LIST_ :
        {
            synfilelistflag = 1;
            clear();
        }
        break;
        case _DOWNLOAD_FILE_ :
        {
        }
        break;
        case _TRANSFER_ACK_ :
        {
        }
        break;
        case _SELECT_FILE_:
        {
            selectflag = 1;
        }
        break;
        default:
            qDebug()<<"Receive command nulity!";
            break;
    }

    if(data.bytesReceived < data.totalBytes)
    {
        data.bytesReceived += bytesAvailable();
        data.dataBlockOutput = readAll();
        data.localFile->write(data.dataBlockOutput);
        data.dataBlockOutput.resize(0);
    }

    if(data.bytesReceived == data.totalBytes)
    {
        if(transferfileflag == 1)
        {
            transferfileflag = 0;
            data.localFile->close();
            qDebug()<<"Receive file success!";
        }
        else if(synfilelistflag == 1)
        {
            synfilelistflag = 0;
            qDebug()<<"Request file list success!";
        }
        else if(downflag == 1)
        {
            downflag = 0;
            qDebug()<<"Download file success!";
        }
        else if (selectflag == 1)
        {
            selectflag = 0;
            QString file = data.fileName;
            // clearNetworkData();
            clear();
            emit fileRequested(file);

        }
        // clearNetworkData();
        clear();
    }

}

void UpdateSocket::sendData(int alreadyWritten)
{
    data.bytesWritten += alreadyWritten;
    switch (data.commandOutput) {
    case _TRANSFER_FILE_:
        data.lastOutputSize -= alreadyWritten;
        if (data.bytesWritten >= data.headerSize && data.lastOutputSize == 0){
            data.dataBlockOutput.clear();
            data.dataBlockOutput.resize(data.payloadSize);
            data.localFile->seek(data.bytesWritten-data.headerSize);
            int sendSize = data.localFile->read(data.dataBlockOutput.data() ,data.payloadSize);
            // data.dataBlockOutput = data.localFile->read(data.payloadSize);
            write(data.dataBlockOutput.constData(), sendSize);
            data.lastOutputSize = sendSize;
        }
        if (data.bytesToWrite == data.bytesWritten)
            clear();
        break;
    default:
        if (data.bytesWritten == data.bytesToWrite) {
            //всё отправили можно зачищать буфер
            clear();
        } else if (data.bytesWritten < data.bytesToWrite) {
            write((data.dataBlockOutput.constData()+data.bytesWritten));
        }
        break;
    }

}

// void UpdateSocket::clientDisconnectSlot()
// {

// }


