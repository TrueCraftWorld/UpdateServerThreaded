#include "updatesocket.h"
#include "protocolcommand.h"

UpdateSocket::UpdateSocket(int socket,int ID,QObject *parent)
    : QTcpSocket{parent}, socketID(ID)
{
    data.payloadSize = 1024 * 64;//64k
    clear();

    QObject::connect(this,&UpdateSocket::readyRead,this,&UpdateSocket::receiveData);
}

UpdateSocket::~UpdateSocket()
{

}

void UpdateSocket::sendFile(QString path)
{
    data.bytesWritten = 0;
    data.fileName = path;
    data.localFile = new QFile(path);
    if(!data.localFile->open(QFile::ReadOnly))
    {
        qDebug()<<tr("open file error!")<<endl;
        return;
    }
    data.totalBytes  = data.localFile->size();

    QDataStream sendOut(&data.dataBlock,QIODevice::WriteOnly);

    sendOut.setVersion(QDataStream::Qt_5_0);

    QString currentFilename = data.fileName.right(data.fileName.size()
                                                          -data.fileName.lastIndexOf('/')-1);


    sendOut << qint64(0) << qint64(0) << qint64(0)<< currentFilename;
    data.totalBytes += data.dataBlock.size();

    sendOut.device()->seek(0);

    sendOut << data.totalBytes<<_TRANSFER_FILE_
            <<qint64((data.dataBlock.size()-(sizeof(qint64)*3)));

    qint64 sum = write(data.dataBlock);
    waitForBytesWritten(2000);
    data.bytesToWrite = data.totalBytes - sum;


    if(data.bytesToWrite>0)
    {
        data.dataBlock = data.localFile->readAll();
        write(data.dataBlock);
        data.dataBlock.resize(0);
    }
    // waitForBytesWritten();
}

void UpdateSocket::sendMSG(QString msg, qint64 cmd)
{

}

void UpdateSocket::clear()
{
    data.totalBytes = 0;
    data.bytesReceived = 0;
    data.fileNameSize = 0;
    data.dataBlock.resize(0);
}

QString UpdateSocket::findDownloadFile(QString path, QString fileName)
{

}

void UpdateSocket::receiveData()
{
    int downflag = 0  ,  synfilelistflag = 0,   transferfileflag = 0;
    qint32 temp;

    if(bytesAvailable()<=0) {
        return;
    }

    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_0);

    if(data.bytesReceived <= sizeof(qint64)*3) {
        if(bytesAvailable()>=sizeof(qint64)*3
            &&(data.fileNameSize==0)) {
            in >> data.totalBytes >> data.command
                >> data.fileNameSize >> temp;
            data.bytesReceived += sizeof(qint64)*3;
        }
        if(bytesAvailable() >= data.fileNameSize
            && data.fileNameSize!=0) {
            in >> data.fileName;
            data.bytesReceived += data.fileNameSize;
        }
    }
    switch(data.command)
    {
    case _TRANSFER_FILE_ :
    {
        transferfileflag = 1;
        if(data.fileNameSize != 0)
        {
            QString tempfilename("./ReceiveFile/");
            tempfilename += data.fileName;
            data.localFile = new QFile(tempfilename);
            if(!data.localFile->open(QFile::WriteOnly)) {
                qDebug()<<"open local file error!";
                return;
            }
        }
    }
    break;
    case _TRANSFER_LIST_ :
    {
        // synfilelistflag = 1;
        // clear();
        // sendFile("./FileList/FILELIST.TXT");
    }
    break;
    case _DOWNLOAD_FILE_ :
    {
        // downflag = 1;
        // QString downloadFilePath;
        // clear();
        // downloadFilePath = this->findDownloadFile("./ServerFile/",data.fileName);
        // sendFile(downloadFilePath);
    }
    break;
    case _TRANSFER_ACK_ :
    {
        // qDebug()<<"Send file success!";
    }
    break;
    default:
        qDebug()<<"Receive command nulity!";
        break;
    }
    if(data.bytesReceived < data.totalBytes)
    {
        data.bytesReceived += bytesAvailable();
        data.dataBlock = readAll();
        data.localFile->write(data.dataBlock);
        data.dataBlock.resize(0);
    }
    if(data.bytesReceived == data.totalBytes)
    {
        clear();
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
    }
}

void UpdateSocket::clientDisconnectSlot()
{

}
