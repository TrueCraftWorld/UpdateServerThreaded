#ifndef PACKAGE_H
#define PACKAGE_H

#include <QtCore>
#include <QSharedPointer>

struct TransferData{
    QSharedPointer<QFile> localFile;           //
    qint64 totalBytes;          //
    QString fileName;           //
    QByteArray dataBlockOutput;    //
    QByteArray dataBlockInput;    //
    qint64 bytesWritten;        //
    qint64 bytesToWrite;        //
    qint64 payloadSize;         //
    int commandOutput;                //
    int commandInput;                //
    qint64 bytesReceived;       //
    qint64 fileNameSize;        //
    qint64 headerSize;
    qint64 lastOutputSize;
};

#endif // PACKAGE_H
