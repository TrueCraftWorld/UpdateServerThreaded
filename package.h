#ifndef PACKAGE_H
#define PACKAGE_H

#include <QtCore>


struct TransferData{
    QFile *localFile;           //
    qint64 totalBytes;          //
    QString fileName;           //
    QByteArray dataBlock;    //
    qint64 bytesWritten;        //
    qint64 bytesToWrite;        //
    qint64 payloadSize;         //
    int command;                //
    qint64 bytesReceived;       //
    qint64 fileNameSize;        //
};

#endif // PACKAGE_H
