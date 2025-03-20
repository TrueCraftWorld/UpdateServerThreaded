#include "updatethread.h"
#include "package.h"

#include <QDir>


UpdateThread::UpdateThread(int socketDes, int ID, QObject *parent)
    :  QThread(parent), socketDescriptor(socketDes), socket(ID)
{
    // setParent(parent);
    // clientID = ID;
    socketDescriptor = socketDes;
}

void UpdateThread::run()
{
   // socket.reset( new UpdateSocket(clientID));

    if(!socket.setSocketDescriptor(socketDescriptor))
        return;

    // connect(socket,&UpdateSocket::disconnected,this,&UpdateThread::closeClientConnectSlot);
    // connect(this,&UpdateThread::sendFileSignal,socket,&UpdateSocket::sendFile);
    // connect(this,&UpdateThread::clientDisconnectSignal,socket,&UpdateSocket::clientDisconnectSlot);
    connect(&socket, &UpdateSocket::fileRequested, this, &UpdateThread::sendFileSlot);
    connect(&socket, &UpdateSocket::listRequested, this, &UpdateThread::sendFileList);

    exec();
}


void UpdateThread::sendFileSlot(QString filename)
{
    if (fileList.contains(filename)) {
        socket.sendFile(directory + '/' + filename);
    }
}

void UpdateThread::sendFileList( int fileType)
{
    // QString dir = directory;
    // QStringList fileList;
    QDir dir;
    switch (fileType) {
    case TransferHeader::DevelopmentFiles:
        dir.setPath(baseDirectory);
        break;
    case TransferHeader::FirmwareUpdate:
        dir.setPath(baseDirectory + '/' + "Firmware");
        break;
    case TransferHeader::SoftwareUpdate:
        dir.setPath(baseDirectory + '/' + "Software");
        break;
    case TransferHeader::MediaUpdate:
        dir.setPath(baseDirectory + '/' + "Media");
        break;
    case TransferHeader::RecommendationUpdate:
        dir.setPath(baseDirectory + '/' + "Recommendations");
        break;
    case TransferHeader::SettingsUpdate:
        dir.setPath(baseDirectory + '/' + "Settings");
        break;
    default:
        break;
    }

    if (dir.exists()) {
        directory = dir.absolutePath();
        fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Time);
        socket.sendFileList(fileList);
    }
}

QString UpdateThread::getDirectory() const
{
    return baseDirectory;
}

void UpdateThread::setDirectory(const QString &newDirectory)
{
    baseDirectory = newDirectory;
    directory = baseDirectory;
}

void UpdateThread::setFileList(const QStringList &newFileList)
{
    fileList = newFileList;
}
