#ifndef PROTOCOLCOMMAND_H
#define PROTOCOLCOMMAND_H

#include "qglobal.h"
constexpr qint64  _TRANSFER_ACK_       =    0x0F00;
constexpr qint64  _TRANSFER_FILE_      =    0x0F01;
constexpr qint64  _TRANSFER_LIST_      =    0x0F02;
constexpr qint64  _DOWNLOAD_FILE_      =    0x0F03;
constexpr qint64  _SELECT_FILE_        =    0x0F04;
constexpr qint64  _REQUEST_LIST_       =    0x0F05;
constexpr qint64  _I_AM_ALIVE_         =    0x0F06;
constexpr qint64  _CLOSE_CONNECTION    =    0x0F07;
constexpr qint64  _REQUEST_ACK_        =    0x0F08;
constexpr qint64  _FILE_CHECK_         =    0x0F09;

#endif // PROTOCOLCOMMAND_H
