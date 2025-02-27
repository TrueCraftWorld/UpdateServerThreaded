#ifndef UPDATESOCKET_H
#define UPDATESOCKET_H

#include <QObject>
#include <QTcpSocket>

#include "package.h"

class UpdateSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit UpdateSocket(int ID,QObject *parent = nullptr);
    ~UpdateSocket();

    void sendFileList(QStringList list);
    void requestFile(const QString& name);
    void sendFile(const QString &path);

signals:
    void fileRequested(const QString&);
    void listRecieved(QStringList list);

private slots:

    void recieveFile(const QString &fileName);

    void sendFilePart();

    void readMessage();
    void recieveFile();

private:
    void clearOutput();
    void clearInput();

    FileInfo inputFile;
    FileInfo outputFile;
    TransferHeader outputHeader;
    TransferHeader inputHeader;
    int socketID;
};

#endif // UPDATESOCKET_H
