#ifndef TEXTMSGHANDLER_H
#define TEXTMSGHANDLER_H

#include "TextMessage.h"
#include <QTcpSocket>

class TextMsgHandler
{
public:
    virtual void handle(QTcpSocket&,TextMessage&) = 0;
};
#endif // TEXTMSGHANDLER_H
