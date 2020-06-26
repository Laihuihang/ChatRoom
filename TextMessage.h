#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include <QByteArray>

class TextMessage : public QObject
{
    Q_OBJECT

    QString m_type;
    QString m_data;
public:
    TextMessage(QObject* parent = nullptr);
    TextMessage(QString type, QString data, QObject* parent = nullptr);

    QString type();
    int length();
    QString data();

    /* 增加对中文的实现，不应该用QString */
    QByteArray serialize();
    bool unserialize(QByteArray ba);
};

#endif // TEXTMESSAGE_H
