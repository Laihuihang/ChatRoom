#ifndef TEXTMSGASSEMBLER_H
#define TEXTMSGASSEMBLER_H

#include <QObject>
#include <QQueue>
#include <QSharedPointer>
#include "TextMessage.h"
/* 装配类的实现关键是如何处理字节数据不够的情况 */
/* 为了使装配类支持中文，m_data需要用utf-8编码，type以及length用ASCII编码 */
class TextMsgAssembler : public QObject
{
    QQueue<char> m_queue;
    QString m_type;
    int m_length;
    QByteArray m_data;

    bool makeTypeAndLength();
    /* 从仓库中获取n个字节的数据，返回一个字符串 */
    QByteArray fetch(int n);
    TextMessage* makeMessage();
public:
    TextMsgAssembler(QObject* parent = nullptr);
    /* 将字节流放入自己的仓库 */
    void prepare(const char* data, int len);
    /* 用智能指针 */
    QSharedPointer<TextMessage> assemble();
    QSharedPointer<TextMessage> assemble(const char* data, int len);
    void reset();
    void clear();
};

#endif // TEXTMSGASSEMBLER_H
