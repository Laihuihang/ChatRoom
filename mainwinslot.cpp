#include "MainWin.h"
#include "QLoginDialog.h"
#include <QMessageBox>
#include <QDebug>

/* 用来验证用户名是否正确 */
static bool ValidateUserID(QString id)
{
    bool ret = true;

    QString invalid = "~!@#$%^&*()_+-={}[];':,./<>?";

    for(int i=0; i<invalid.size(); i++)
    {
        if(id.contains(invalid[i]))
        {
            ret = false;
            break;
        }
    }
    return ret;
}

void MainWin::initMember()
{
    m_client.setHandler(this);
    /* 构造函数中建立字符串到函数指针的映射 */
    m_handlerMap.insert("CONN", &MainWin::CONN_handler);
    m_handlerMap.insert("DSCN", &MainWin::DSCN_handler);
    m_handlerMap.insert("LIOK", &MainWin::LIOK_handler);
    m_handlerMap.insert("LIER", &MainWin::LIER_handler);
    m_handlerMap.insert("MSGA", &MainWin::MSGA_handler);
    m_handlerMap.insert("USER", &MainWin::USER_handler);
    m_handlerMap.insert("CTRL", &MainWin::CTRL_handler);
    m_handlerMap.insert("INSE", &MainWin::INSE_handler);
}
/* 重写eventFilter函数，实现按下回车发送 */
bool MainWin::eventFilter(QObject* obj, QEvent* event)
{
    if((obj == &inputEdit) && (event->type() == QEvent::KeyPress))
    {
        QKeyEvent* ke = dynamic_cast<QKeyEvent*>(event);

        /* 处理按下回车这个事件 */
        if(ke->text() == "\r")
        {
            sendBtnClicked();

            return true;
        }
    }

    /* 返回主窗口进行处理 */
    return QWidget::eventFilter(obj,event);
}
void MainWin::sendBtnClicked()
{
    QString input = inputEdit.text().trimmed();

    if (input != "")
    {
       QDateTime current_date_time =QDateTime::currentDateTime();
       QString current_date =current_date_time.toString("hh:mm:ss");
       QString self = inputGrpBx.title();
       QString text = self + "\t" + current_date + ":\n" + "   " + input + "\n";
       QString uid = getCheckedUserId();
       qDebug() << "uid:" << uid;
       bool ok = true;

       /* 发送公聊消息 */
       if(uid == "")
       {
           TextMessage tm("MSGA",text);

           ok = m_client.send(tm);
       }
       /* 发送私聊消息 */
       else
       {
            /* 判断uid中有无自己的id，如没有则加上 */
            QString sid = (uid.indexOf(self) >= 0)? uid :  (uid + self + "\r");
            qDebug() << "sid:" << sid;
            /* 因为私聊模式的消息格式是前面为私聊模式的用户id，最后一个才为消息的内容 */
            TextMessage tm("MSGP", sid + text);

            ok = m_client.send(tm);
       }

       if(ok)
       {
           inputEdit.clear();
       }
    }
}

void MainWin::SendIdAndPwd(QString head)
{
    QString usr = loginDlg.getUser().trimmed();
    QString pwd = loginDlg.getPwd().trimmed();

    /* 连接成功 */
    if(m_client.connectTo("103.46.128.45",22003))
    {
        //setCtrlEnabled(true);
        TextMessage msg(head,usr + '\r' + pwd);
        m_client.send(msg);
    }
    /* 连接不成功 */
    else
    {
        QMessageBox::critical(this, "失败", "连接不到远程服务端");
    }
}
void MainWin::logInOutBtnClinked()
{
    /* 如果没有连接上就连接 */
    if(!m_client.isValid())
    {
        loginDlg.setValFunc(ValidateUserID);
        switch (loginDlg.exec())
        {
            case LOGIN:
                SendIdAndPwd("LGIN");
                break;
            case REGISTER:
                SendIdAndPwd("REGI");
                break;
        }
    }
    /* 否则就断开连接 */
    else
    {
        m_client.close();
    }
}
void MainWin::listWidgetMenuClicked()
{
    QAction* act = dynamic_cast<QAction*>(sender());

    if(act != nullptr)
    {
        /* 获取鼠标所指向的列表中的选项 */
        const QList<QListWidgetItem*>& sl = listwidget.selectedItems();

        if(sl.length() > 0)
        {
            QString user = sl.at(0)->text();

            if(QMessageBox::question(this, "提示",
                                     "确认对聊天成员 [ " + user + " ] 进行" + act->text() + "操作吗?",
                                     QMessageBox::Yes, QMessageBox::No)
                                     == QMessageBox::Yes)
            {
                QString data = act->objectName() + "\r" + user;
                TextMessage tm("ADMN",data);

                m_client.send(tm);
            }
        }
        else
        {
            QMessageBox::information(this, "提示", "请选择聊天成员");
        }
    }
}
void MainWin::listWidgetContextMenu(const QPoint&)
{
    if(m_level == "admin")
    {
        /* 在当前鼠标的位置弹出右键菜单 */
        listWidgetMenu.exec(QCursor::pos());
    }
}
QString MainWin::getCheckedUserId()
{
    QString ret = "";

    for (int i=0; i<listwidget.count(); i++)
    {
        QListWidgetItem* item = listwidget.item(i);

        if(item->checkState() == Qt::Checked)
        {
            ret += item->text() + "\r";
        }
    }

    return ret;
}
/* 重构思想，用字符串对应函数指针，可以用QMap来实现 */
void MainWin::handle(QTcpSocket& obj, TextMessage& message)
{
    /* 判断消息类型是否在表中 */
    if(m_handlerMap.contains(message.type()))
    {
        MSGHandler handler = m_handlerMap.value(message.type());
        (this->*handler)(obj, message);
    }
}
MainWin::~MainWin()
{
    m_client.close();
}
void MainWin::CONN_handler(QTcpSocket& obj, TextMessage& message)
{

}
void MainWin::DSCN_handler(QTcpSocket&, TextMessage&)
{
    setCtrlEnabled(false);
    inputGrpBx.setTitle("用户名");

    m_level = "";
}
void MainWin::LIOK_handler(QTcpSocket&, TextMessage& message)
{
    QStringList rl = message.data().split("\r",QString::SkipEmptyParts);
    QString id = rl[0];
    QString status = rl[1];

    m_level = rl[2];

    if(status == "silent")
    {
        /* 设置登陆状态 */
        setCtrlEnabled(true);
        /* 在界面上设置所使用的用户名 */
        inputGrpBx.setTitle(id);

        inputEdit.setEnabled(false);
        sendBtn.setEnabled(false);
    }
    else if(status == "kick")
    {
        m_client.close();
        QMessageBox::information(this, "提示", "账号 [ " + id + " ] 被禁止登陆聊天室");
    }
    else
    {
        /* 设置登陆状态 */
        setCtrlEnabled(true);
        /* 在界面上设置所使用的用户名 */
        inputGrpBx.setTitle(id);
    }
}
void MainWin::LIER_handler(QTcpSocket&, TextMessage&)
{
    QMessageBox::critical(this, "错误", "身份验证失败！");

    m_client.close();
}
void MainWin::USER_handler(QTcpSocket&, TextMessage& message)
{
    QStringList users = message.data().split("\r", QString::SkipEmptyParts);
    /* 得到用户勾选的ID列表 */
    QStringList checked = getCheckedUserId().split("\r",QString::SkipEmptyParts);

    listwidget.clear();

    for(int i=0; i<users.size(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem();

        if(item != nullptr)
        {
            item->setText(users[i]);
            item->setCheckState(Qt::Unchecked);

            listwidget.addItem(item);
        }
    }

    /* 将保存的私聊用户恢复 */
    for(int i=0; i<listwidget.count(); i++)
    {
        QListWidgetItem* item = listwidget.item(i);

        for(int j=0; j<checked.size(); j++)
        {
            if(checked[j] == item->text())
            {
                item->setCheckState(Qt::Checked);
            }
        }
    }
}
void MainWin::MSGA_handler(QTcpSocket&, TextMessage& message)
{
    /* 将收到的消息插入到接收区域 */
    msgEditor.appendPlainText(message.data());

    /* 让窗口闪烁 */
    activateWindow();
}
void MainWin::CTRL_handler(QTcpSocket&, TextMessage& message)
{
    if(message.data() == "silent")
    {
        QMessageBox::information(this, "提示", "你已经被管理员禁言！");

        inputEdit.clear();
        inputEdit.setEnabled(false);
        sendBtn.setEnabled(false);
    }
    else if(message.data() == "recover")
    {
        QMessageBox::information(this, "提示", "管理员恢复了你的聊天权限");

        inputEdit.setEnabled(true);
        sendBtn.setEnabled(true);
    }
    else if(message.data() == "kick")
    {
        QMessageBox::information(this, "提示", "账号 [ " + inputGrpBx.title() + " ] 被禁止登陆聊天室！");

        m_client.close();
    }

}
void MainWin::INSE_handler(QTcpSocket&, TextMessage& message)
{
    QStringList rl = message.data().split("\r");
    int status = rl[0].toInt();
    switch (status)
    {
        case INSERT_OK:
            m_client.close();
            QMessageBox::information(this, "提示", "注册成功");
            break;
        case INSERT_ERROR:
            m_client.close();
            QMessageBox::critical(this, "错误", "账号已被注册");
            break;
    }
}
