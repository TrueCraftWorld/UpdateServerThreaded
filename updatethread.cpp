#include "updatethread.h"
#include "package.h"

#include <QDir>


UpdateThread::UpdateThread(int socketDes, int ID, QObject *parent)
    : QThread(parent), socketDescriptor(socketDes)
{
    clientID = ID;
    socketDescriptor = socketDes;
}

void UpdateThread::run()
{
   socket.reset( new UpdateSocket(clientID,0));

    if(!socket->setSocketDescriptor(socketDescriptor))
        return;

    // QObject::connect(socket,&UpdateSocket::disconnected,this,&UpdateThread::closeClientConnectSlot);
    // QObject::connect(this,&UpdateThread::sendFileSignal,socket,&UpdateSocket::sendFile);
    // QObject::connect(this,&UpdateThread::clientDisconnectSignal,socket,&UpdateSocket::clientDisconnectSlot);
    connect(socket.data(), &UpdateSocket::fileRequested, this, &UpdateThread::sendFileSlot);
    connect(socket.data(), &UpdateSocket::listRequested, this, &UpdateThread::sendFileList);

    exec();
}

// void UpdateThread::closeClientConnectSlot()
// {

// }

// void UpdateThread::clientDisconnectSlot()
// {

// }


void UpdateThread::sendFileSlot(QString filename)
{
    if (socket.isNull())
        return;
    if (fileList.contains(filename)) {
        socket->sendFile(directory + '/' + filename);
    }
}

void UpdateThread::sendFileList( int fileType)
{
    // QString dir = directory;
    QStringList fileList;
    QDir dir;
    switch (fileType) {
    case TransferHeader::DevelopmentFiles:
        dir.setPath(directory);
        break;
    case TransferHeader::FirmwareUpdate:
        dir.setPath(directory + '/' + "Firmware");
        break;
    case TransferHeader::SoftwareUpdate:
        dir.setPath(directory + '/' + "Software");
        break;
    case TransferHeader::MediaUpdate:
        dir.setPath(directory + '/' + "Media");
        break;
    case TransferHeader::RecommendationUpdate:
        dir.setPath(directory + '/' + "Recommendations");
        break;
    case TransferHeader::SettingsUpdate:
        dir.setPath(directory + '/' + "Settings");
        break;
    default:
        break;
    }

    if (dir.exists()) {
        fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Time);
        socket->sendFileList(fileList);
    }
}

QString UpdateThread::getDirectory() const
{
    return directory;
}

void UpdateThread::setDirectory(const QString &newDirectory)
{
    directory = newDirectory;
}

void UpdateThread::setFileList(const QStringList &newFileList)
{
    fileList = newFileList;
}
