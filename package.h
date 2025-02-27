#ifndef PACKAGE_H
#define PACKAGE_H

#include <QtCore>
#include <QSharedPointer>

constexpr int payloadSize = 1024*64;
constexpr qint64 magicNum = 0x004AFFB2009CFF31;


struct TransferHeader {
    qint64 magic; ///идентификатор нашего протокола
    qint64 command; //тип сообщения
    qint64 messageSize; //размер сообщения
    qint64 fileSize; //размер файла
    //выше - стабильная, обязательная, часть сообщения,
    //ниже - опционально. message - список фалойв, имя запрашиваемого файла, имя передаваемого файла
    QString message;
    //место для посылки
    QByteArray dataBlock;
    //ниже переменные для статистики при обработке
    qint64 bytesReadOrWritten;
    qint64 bytesToReadOrWrite;
};

struct FileInfo
{
    qint64 bytesRecived;
    qint64 awaitedSize;
    QSharedPointer<QFile> localFile;
};

#endif // PACKAGE_H
