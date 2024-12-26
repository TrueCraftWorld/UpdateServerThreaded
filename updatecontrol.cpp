#include "updatecontrol.h"

#include <QHostAddress>

UpdateControl::UpdateControl(QObject *parent)
    : QObject{parent}
{
    server.listen(QHostAddress::Any, 11111);
}
