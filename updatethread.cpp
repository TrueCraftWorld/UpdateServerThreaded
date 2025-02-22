#include "updatethread.h"
#include "config.h"



UpdateThread::UpdateThread(int socketDes, int ID, QObject *parent)
    : QThread(parent), socketDescriptor(socketDes)
{
    clientID = ID;
    socketDescriptor = socketDes;
}

void UpdateThread::run()
{
    UpdateSocket *socket = new UpdateSocket(socketDescriptor,clientID,0);

    if(!socket->setSocketDescriptor(socketDescriptor))
        return;

    // QObject::connect(socket,&UpdateSocket::disconnected,this,&UpdateThread::closeClientConnectSlot);
    // QObject::connect(this,&UpdateThread::sendFileSignal,socket,&UpdateSocket::sendFile);
    // QObject::connect(this,&UpdateThread::clientDisconnectSignal,socket,&UpdateSocket::clientDisconnectSlot);
    connect(socket, &UpdateSocket::fileRequested, this, &UpdateThread::sendFileSlot);


    /*
     * тут пока сделаем Маня-мирок - отдаём один конкретный файл каждому абоненту
     *
     * Потом будет сначала обмен списком доступных файлов и запрос желаемого
     * */
    // socket->sendFile("/home/kikorik/Desktop/SmallFileForTest2.png");

    socket->sendFileList(fileList);

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
    if (fileList.contains(filename)) {
        socket->sendFile(directory + '/' + filename);
    }
}

void UpdateThread::sendFileList()
{

}

void UpdateThread::setFileList(const QStringList &newFileList)
{
    fileList = newFileList;
}
