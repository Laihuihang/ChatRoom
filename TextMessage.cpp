#include "TextMessage.h"

TextMessage::TextMessage(QObject* parent) : QObject (parent)
{
    m_type = "";
    m_data = "";
}
TextMessage::TextMessage(QString type, QString data, QObject* parent) : QObject (parent)
{
    /* 强制转换为4个字节 */
    m_type = type.trimmed();
    m_type.resize(4,' ');
    /* 取一个子串，该子串最多两个字节的长度 */
    m_data = data.mid(0,15000);
}


QString TextMessage::type()
{
    /* 去除前后的空格 */
    return m_type.trimmed();
}
int TextMessage::length()
{
    return m_data.length();
}
QString TextMessage::data()
{
    return m_data;
}
/* 组装三个部分 */
QByteArray TextMessage::serialize()
{
    QByteArray ret;
    /* data段用的utf-8编码 */
    QByteArray dba = m_data.toUtf8();
    QString len = QString::asprintf("%X",dba.length());
    len.resize(4,' ');

    ret.append(m_type.toStdString().c_str(),4);
    ret.append(len.toStdString().c_str(),4);
    ret.append(dba);

    return ret;
}
bool TextMessage::unserialize(QByteArray ba)
{
    bool ret = (ba.length() >= 8);
    if(ret)
    {
        QString type = QString(ba.mid(0,4));
        QString len = QString(ba.mid(4,4).trimmed());
        /* 16进制转变为整形，成功与否保存到ret中 */
        int l = len.toInt(&ret,16);

        ret = ret && (l == (ba.length() - 8));

        if(ret)
        {
            m_type = type;
            /* QString里面自己做了utf-8的解码 */
            m_data = QString(ba.mid(8));
        }
    }
    return ret;
}
