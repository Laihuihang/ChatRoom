#ifndef CLIENTDEMO_H
#define CLIENTDEMO_H

#include <QObject>
#include <QTcpSocket>
#include "TextMessage.h"
#include "TextMsgAssembler.h"
#include "textmsghandler.h"

class ClientDemo : public QObject
{
    Q_OBJECT

    QTcpSocket m_client;
    TextMsgAssembler m_assembler;
    TextMsgHandler* m_handler;
protected slots:
    void onConnected();
    void onDisconnected();
    void onDataReady();
    void onBytesWritten(qint64 bytes);
public:
    ClientDemo(QObject* parent = nullptr);
    bool connectTo(QString ip, quint16 port);
    qint64 send(TextMessage& message);
    qint64 avaliable();
    void setHandler(TextMsgHandler* handler);
    /* 判断当前的客户端是否连接 */
    bool isValid();
    void close();
};

#endif // CLIENTDEMO_H
