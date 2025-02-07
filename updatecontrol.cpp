#include "updatecontrol.h"

#include <QHostAddress>

UpdateControl::UpdateControl(QObject *parent)
    : QObject{parent}
{

}

void UpdateControl::setDirectory(const QString &dir)
{
    m_dir = dir;
    server.setDirectory(dir);
}

void UpdateControl::start()
{
    server.listen(QHostAddress::Any, 11111);
}
