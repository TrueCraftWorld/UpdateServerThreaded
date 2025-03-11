#include "updateserver.h"
#include <QDir>

UpdateServer::UpdateServer(QObject *parent)
    : QTcpServer{parent}
{}

UpdateServer::~UpdateServer()
{

}

void UpdateServer::setDirectory(const QString &dir)
{
    m_dir = dir;
    QDir directory(m_dir);
    // updateFiles.clear();

    // updateFiles = directory.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Time);

}

void UpdateServer::incomingConnection(qintptr socketDescriptor)
{

    socketList.append(socketDescriptor);

    socketNum++; //kinda incremantal ID

    UpdateThread *thread = new UpdateThread(socketDescriptor,socketNum,nullptr);
    threadList.append(thread);
    // thread->setFileList(updateFiles);
    thread->setDirectory(m_dir);
    thread->start();
}

void UpdateServer::clientDisconnectSlot(int ID)
{

}
