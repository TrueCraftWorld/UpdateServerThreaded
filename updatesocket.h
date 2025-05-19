#ifndef UPDATESOCKET_H
#define UPDATESOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>

#include "updateConfig.h"
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
    void signalListRecieved(QStringList list);
    void signalFileRecieved(const QString& path, int fileType);
    void signalFileRecievedError(const QString& path);
    void signalFilePartRecieved(double fileSuccesPercentage);

    void signalFileRequested(const QString&);
    void signalListRequested(int fileType);

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

    void recieveFile(const QString &fileName, const QString &destPath=DOWNLOAD_PATH);

    void sendFilePart(int lasrSendSize);

    void readMessage();
    void recieveFile();

private:
    void clearOutput();
    void clearInput();
    /**
     * @brief sendFileCheck отправка чексуммы SHA-256 файла
     * @param filePath путь к файлу
     *
     */
    void sendFileCheck(const QString& filePath);
    void completeSendFile(const QString& path);

    FileInfo inputFile;
    FileInfo outputFile;
    TransferHeader outputHeader;
    TransferHeader inputHeader;
    int socketID;

    int m_toNextPart = 0;


    QTimer m_updateTimer;
    void prepareFileInfo(const QString& checkSum);
};

#endif // UPDATESOCKET_H
