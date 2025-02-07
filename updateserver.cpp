#include "updateserver.h"

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
    updateFiles.clear();

    updateFiles = directory.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Time);

}

void UpdateServer::incomingConnection(qintptr socketDescriptor)
{

    socketList.append(socketDescriptor);

    socketNum++; //kinda incremantal ID

    // if(socketNum > 3)
    //     socketNum = 1;// dunno some chinese stuff

    UpdateThread *thread = new UpdateThread(socketDescriptor,socketNum,nullptr);
    threadList.append(thread);
    // connect(thread,&UpdateThread::closeClientConnectSignal,widget,&Widget::closeClientConnectSlot); //need to reimplement here
    // connect(thread,&UpdateThread::addClientIPToGUISignal,widget,&Widget::addClientIPTGUISlot);//need to reimplement here
    // connect(widget,&Widget::getFileListSignal,thread,&UpdateThread::getFileListSlot);
    // connect(widget,&Widget::sendFileSignal,thread,&UpdateThread::sendFileSlot);
    // connect(widget,&Widget::clientDiconnectSignal,this,&Myserver::clientDisconnectSlot);
    thread->setFileList(updateFiles);
    thread->start();
}

void UpdateServer::clientDisconnectSlot(int ID)
{

}
