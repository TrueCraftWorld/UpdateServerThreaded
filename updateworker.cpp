#include "updateworker.h"
#include "package.h"

// #include <memory>

#include <QDir>


UpdateWorker::UpdateWorker(int socketDes, int ID, QObject */*parent*/)
    :  QObject(nullptr),
    socket(ID,this),
    socketDescriptor(socketDes)
{
    socketDescriptor = socketDes;
    if(!socket.setSocketDescriptor(socketDescriptor))
        return;

    // connect(socket,&UpdateSocket::disconnected,this,&UpdateThread::closeClientConnectSlot);
    // connect(this,&UpdateThread::sendFileSignal,socket,&UpdateSocket::sendFile);
    // connect(this,&UpdateThread::clientDisconnectSignal,socket,&UpdateSocket::clientDisconnectSlot);
    connect(&socket, &UpdateSocket::signalFileRequested, this, &UpdateWorker::sendFileSlot);
    connect(&socket, &UpdateSocket::signalListRequested, this, &UpdateWorker::sendFileList);
}


void UpdateWorker::sendFileSlot(QString filename)
{
    if (fileList.contains(filename)) {
        socket.sendFile(directory + '/' + filename);
    }
}

void UpdateWorker::sendFileList( int fileType)
{
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

QString UpdateWorker::getDirectory() const
{
    return baseDirectory;
}

void UpdateWorker::setDirectory(const QString &newDirectory)
{
    baseDirectory = newDirectory;
    directory = baseDirectory;
}

void UpdateWorker::setFileList(const QStringList &newFileList)
{
    fileList = newFileList;
}
