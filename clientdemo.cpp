#include "clientdemo.h"
#include <QDebug>
#include <QHostAddress>
ClientDemo::ClientDemo(QObject* parent) : QObject (parent) , m_handler(nullptr)
{
    connect(&m_client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&m_client, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    connect(&m_client, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
}
void ClientDemo::onConnected()
{
    if(m_handler != nullptr)
    {
        TextMessage conn("CONN",m_client.peerAddress().toString() + ":"+QString::number(m_client.peerPort()));

        m_handler->handle(m_client,conn);
    }
}
void ClientDemo::onDisconnected()
{
    m_assembler.reset();

    if(m_handler != nullptr)
    {
        TextMessage dscn("DSCN","");

        m_handler->handle(m_client,dscn);
    }
}
/* 读取数据 */
void ClientDemo::onDataReady()
{
    char buf[256] = {0};
    int len = 0;

    while((len = m_client.read(buf,sizeof(buf))) > 0)
    {
        QSharedPointer<TextMessage> ptm = nullptr;

        m_assembler.prepare(buf, len);

        /* 不停地装配，装配到不能装配为止 */
        while((ptm = m_assembler.assemble()) != nullptr)
        {
            /* 表示已经得到一个文本协议对象了，添加一个接口 */
            /* 如果两个指针都不为空，可以进行通知 */
            if(m_handler != nullptr)
            {
                m_handler->handle(m_client,*ptm);
            }
        }
    }
}
void ClientDemo::onBytesWritten(qint64 bytes)
{
    (void)bytes;
}

bool ClientDemo::connectTo(QString ip,quint16 port)
{
    bool ret = true;
    m_client.connectToHost(ip,port);
    ret = m_client.waitForConnected();
    return ret;
}
qint64 ClientDemo::send(TextMessage& message)
{
    QByteArray ba = message.serialize();

    return m_client.write(ba.data(),ba.length());
}
qint64 ClientDemo::avaliable()
{
    return m_client.bytesAvailable();
}
void ClientDemo::setHandler(TextMsgHandler* handler)
{
    m_handler = handler;
}
bool ClientDemo::isValid()
{
    return m_client.isValid();
}
void ClientDemo::close()
{
    m_client.close();
}
