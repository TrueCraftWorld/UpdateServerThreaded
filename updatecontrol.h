#ifndef UPDATECONTROL_H
#define UPDATECONTROL_H

#include <QObject>

#include "updateserver.h"

class UpdateControl : public QObject
{
    Q_OBJECT
public:
    explicit UpdateControl(QObject *parent = nullptr);
    void setDirectory(const QString& dir);
    void start();

signals:
private:
    QString m_dir;
    UpdateServer server;
};

#endif // UPDATECONTROL_H
