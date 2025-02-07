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
    void setFileList(const QStringList &newFileList);

protected:
    virtual void run() override;
signals:
    void closeClientConnectSignal(int ID);
    void clientDisconnectSignal();
    // void sendFileListSignal(QString filelist);
    void sendFileSignal(QString filename);
public slots:
    void closeClientConnectSlot();
    void clientDisconnectSlot();
    void getFileListSlot(QString filelist);
    void sendFileSlot(QString filename);
    void sendFileList();
private:
    UpdateSocket *socket;
    int clientID;
    quintptr socketDescriptor;

private:
    TransferData data;
    QString directory;
    QStringList fileList;
};

#endif // UPDATETHREAD_H
