#ifndef PACKAGE_H
#define PACKAGE_H

#include <QtCore>


struct TransferData{
    QFile *localFile;           //存放本地文件
    qint64 totalBytes;          //存放总大小信息
    QString fileName;           //存放文件名
    QByteArray dataBlock;    //数据缓冲区
    qint64 bytesWritten;        //已经发送数据大小
    qint64 bytesToWrite;        //剩余数据的大小
    qint64 payloadSize;         //每次发送数据的大小
    int command;                //发送命令
    qint64 bytesReceived;       //已收到数据的大小
    qint64 fileNameSize;        //文件名的大小信息
};

#endif // PACKAGE_H
