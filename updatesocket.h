#ifndef UPDATESOCKET_H
#define UPDATESOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>

#include "package.h"

class UpdateSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit UpdateSocket(int ID,QObject *parent = nullptr);
    ~UpdateSocket();

    void requestFileList(TransferHeader::FileType fileType);
    void sendFileList(QStringList list);
    void requestFile(const QString& name,
                     TransferHeader::FileType fileType = TransferHeader::DevelopmentFiles);
    void sendFile(const QString &path);

signals:
    void listRecieved(QStringList list);
    void fileRecieved(const QString& path);

    void fileRequested(const QString&);
    void listRequested(int fileType);

private slots:
    /**
     * @brief отправка посылки, соотоящей только из команды и сообщения. без файлы
     * @param message - текст сообщения, может быть пуст
     * @param command - команда
     * @param fileType - тип файла, заполняется только если сообщение касается подготовки к обмену файлами
     */
    void sendMessageOnly(const QString& message,
                         qint64 command,
                         TransferHeader::FileType fileType = TransferHeader::DevelopmentFiles);

    void recieveFile(const QString &fileName);

    void sendFilePart(int lasrSendSize);

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

    int m_toNextPart = 0;


    QTimer m_updateTimer;
};

#endif // UPDATESOCKET_H
