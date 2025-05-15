#include "FileChecker.h"

// FileChecker::FileChecker() {}


#include <QCryptographicHash>
#include <QFile>
#include <QDebug>

QString FileChecker::getCheckSum(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << file.errorString();
        return QString();
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);

    // Read the file in chunks to handle large files efficiently
    const qint64 bufferSize = 65536; // 64KB
    char buffer[bufferSize];
    qint64 bytesRead;

    while (!file.atEnd()) {
        bytesRead = file.read(buffer, bufferSize);
        if (bytesRead > 0) {
            hash.addData(buffer, bytesRead);
        } else {
            qWarning() << "Error reading file:" << file.errorString();
            file.close();
            return QString();
        }
    }

    file.close();

    // Convert the hash result to a hex string
    return QString(hash.result().toHex());
}
