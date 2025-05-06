#ifndef UPDATEWORKER_H
#define UPDATEWORKER_H

#include <QObject>

#include "updatesocket.h"



class UpdateWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateWorker(int socketDes,int ID,QObject *parent);
    void setFileList(const QStringList &newFileList);

    QString getDirectory() const;
    void setDirectory(const QString &newDirectory);

signals:
    void closeClientConnectSignal(int ID);
    void clientDisconnectSignal();
    void sendFileSignal(QString filename);
public slots:
    void sendFileSlot(QString filename);
    void sendFileList(int fileType);
private:
    UpdateSocket socket;
    quintptr socketDescriptor;

private:
    QString baseDirectory;
    QString directory;
    QStringList fileList;
};

#endif // UPDATEWORKER_H
