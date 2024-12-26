#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QObject>
#include <QThread>

#include "package.h"
#include "updatesocket.h"



class UpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateThread(int socketDes,int ID,QObject *parent = nullptr);
protected:
    virtual void run() override;
signals:
    void closeClientConnectSignal(int ID);
    void clientDisconnectSignal();
    void getFileListSignal(QString filelist);
    void sendFileSignal(QString filename);
public slots:
    void closeClientConnectSlot();
    void clientDisconnectSlot();
    void getFileListSlot(QString filelist);
    void sendFileSlot(QString filename);
private:
    UpdateSocket *socket;
    int clientID;
    quintptr socketDescriptor;

private:
    TransferData data;
};

#endif // UPDATETHREAD_H
