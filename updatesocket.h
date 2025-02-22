#ifndef UPDATESOCKET_H
#define UPDATESOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "package.h"

class UpdateSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit UpdateSocket(int socket,int ID,QObject *parent = nullptr);
    ~UpdateSocket();

    void sendFile(QString path);
    void sendFileList(QStringList list);
    // void sendMSG(QString msg,qint64 cmd);
    void clear();
    // QString findDownloadFile(QString path, QString fileName);
signals:
    void fileRequested(const QString&);

public slots:
    void receiveData();
    void clientDisconnectSlot();

private:
    void clearNetworkData();

    TransferData data;
    int socketID;
    int socketDescriptor;
};

#endif // UPDATESOCKET_H
