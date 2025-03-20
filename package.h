#ifndef PACKAGE_H
#define PACKAGE_H

#include <QSharedPointer>
#include <QFile>

constexpr int payloadSize = 1024*1024*4;
constexpr qint64 magicNum = 0x004AFFB2009CFF31;
constexpr int headerFieldCount = 5;
constexpr int headerSizeBytes = headerFieldCount * sizeof(qint64);

const QStringList names = {"Рарзработка"
                           , "Обновление микропрограмм"
                           , "Обновление ПО"
                           , "Обновление медиафайлов"
                           , "Обновление конфигурации"
                           , "Обновление рекомендаций"};

struct TransferHeader {

    /**
     * @brief типы файлов списки которых можно запрашивать
     * @details предполагается, что в папке для обновления для каждой категории создана подпапка
     * с некоторым заранее известным именем. девелоп файлы лежат в корне. возможно ограничение по расширениям файлов
     * в каждой папке
     */
    enum FileType : int {
        DevelopmentFiles = 0, /**< режим для разработки, просто файлы из папки */
        FirmwareUpdate, /**< файлы прошивок стм-ок */
        SoftwareUpdate, /**< файлы обновления ПО одноплатника */
        MediaUpdate, /**< файл с медиаконтентом - обучение, инструкции, реклама */
        SettingsUpdate,  /**< файлы настроек и конфигов */
        RecommendationUpdate /**< файлы установок встроенных программ, будь то архив с конфигами ил файлы базы данных */
    };
    static QStringList fileTypes() {
        return names;
    }
    qint64 magic; ///идентификатор нашего протокола
    qint64 command; //тип сообщения
    qint64 messageSize; //размер сообщения
    qint64 fileSize; //размер файла
    qint64 fileType; //типа запрашиваемого.передаваемого файла
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
    // qint
    QSharedPointer<QFile> localFile;
};

#endif // PACKAGE_H
