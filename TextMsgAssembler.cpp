#include "TextMsgAssembler.h"

TextMsgAssembler::TextMsgAssembler(QObject* parent) : QObject (parent)
{
    clear();
}
/* 将字节流放入自己的仓库 */
void TextMsgAssembler::prepare(const char* data, int len)
{
    for(int i=0; i<len; i++)
    {
         m_queue.push_back(data[i]);
    }
}
/* 容器里要有八个字节 */
bool TextMsgAssembler::makeTypeAndLength()
{
    bool ret = (m_queue.length() >= 8);
    if(ret)
    {
        QString len = "";
        m_type = QString(fetch(4));
        len = QString(fetch(4));
        m_length = len.trimmed().toInt(&ret,16);

        if(!ret)
        {
            clear();
        }
    }

    return ret;
}
/*
 * 1.计算需要的剩余数据量
 * 2.看看仓库里的数据够不够
*/
TextMessage* TextMsgAssembler::makeMessage()
{
    TextMessage* ret = nullptr;
    if(m_type != "")
    {
        int need = m_length - m_data.length();
        /* 如果仓库里的数据小于需要的数据，则仓库里有多少就要多少 */
        int n = (need <= m_queue.length()) ? need : m_queue.length();

        m_data.append(fetch(n));
        if(m_length == m_data.length())
        {
            ret = new TextMessage(m_type,QString(m_data));
        }
    }
    return ret;
}
QByteArray TextMsgAssembler::fetch(int n)
{
    QByteArray ret;
    for(int i=0; i<n; i++)
    {
        ret.append(m_queue.dequeue());
    }
    return ret;
}
/* 用智能指针
 * 1.判断m_type是否为空，为空则装配m_type
 * 2.不为空装配数据区
 * 3.有可能装配玩类型和长度之后，在装配数据时，发现缓冲区不够，因此无法创建协议对象，返回空
 *
 * assemble()函数的注意事项
 *   以m_type作为标志决定是否解析类型和长度
 *   m_length是接收后续数据的基础
 *   当m_data的长度与m_length相同时创建协议对象
 *   否则，返回NULL
*/
QSharedPointer<TextMessage> TextMsgAssembler::assemble()
{
    TextMessage* ret = nullptr;
    bool tryMakeMsg = false;
    if(m_type == "")
    {
        tryMakeMsg = makeTypeAndLength();
    }
    else
    {
        tryMakeMsg = true;
    }
    if(tryMakeMsg)
    {
        ret = makeMessage();
    }
    if(ret != nullptr)
    {
        clear();
    }
    return QSharedPointer<TextMessage>(ret);
}
QSharedPointer<TextMessage> TextMsgAssembler::assemble(const char* data, int len)
{
    prepare(data,len);
    return assemble();
}
void TextMsgAssembler::reset()
{
    clear();
    m_queue.clear();
}
void TextMsgAssembler::clear()
{
    m_type = "";
    m_length = 0;
    m_data.clear();
}
