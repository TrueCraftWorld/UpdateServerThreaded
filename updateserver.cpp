#include "updateserver.h"
#include <QDir>
#include <QThread>

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
}

void UpdateServer::incomingConnection(qintptr socketDescriptor)
{

    socketList.append(socketDescriptor);

    socketNum++; //kinda incremantal ID

    UpdateWorker *thread = new UpdateWorker(socketDescriptor,socketNum,nullptr);
    QThread* theThread = new QThread(nullptr);
    theThreadList.append(theThread);
    thread->setDirectory(m_dir);
    thread->moveToThread(theThread);
    theThread->start();
}

void UpdateServer::clientDisconnectSlot(int ID)
{

}
