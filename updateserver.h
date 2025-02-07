#ifndef UPDATESERVER_H
#define UPDATESERVER_H

#include <QObject>
#include <QTcpServer>

#include <updatethread.h>

class UpdateServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit UpdateServer(QObject *parent = nullptr);
    ~UpdateServer();

    void setDirectory(const QString& dir);

    QList<int > socketList;
    QList<UpdateThread *>threadList;
    QList<QString> socketIP;
    int socketNum;

protected:
    virtual void incomingConnection(qintptr socketDescriptor) override;
signals:
    void clientDisconnectSignal();
private slots:
    void clientDisconnectSlot(int ID);
private:
    QString m_dir;
    QStringList updateFiles;
};
#endif // UPDATESERVER_H
