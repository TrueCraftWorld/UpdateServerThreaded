#ifndef FILECHECKER_H
#define FILECHECKER_H

#include <QString>

class FileChecker
{
public:
    FileChecker() = default;
    static QString getCheckSum(const QString& filePath);
};

#endif // FILECHECKER_H
