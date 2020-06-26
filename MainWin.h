#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include "QLoginDialog.h"
#include "clientdemo.h"
#include "textmsghandler.h"
#include <QTcpSocket>
#include "TextMessage.h"
#include <QMap>
#include <QListWidget>
#include <QDateTime>

class MainWin : public QWidget, public TextMsgHandler
{
    Q_OBJECT
    typedef void(MainWin::*MSGHandler)(QTcpSocket&, TextMessage&);

    QVBoxLayout vMainLayout;
    /* 接受消息 */
    QGroupBox msgGrpBx;
    /* 输入消息 */
    QGroupBox inputGrpBx;
    QListWidget listwidget;
    QMenu listWidgetMenu;
    QPlainTextEdit msgEditor;
    QLineEdit inputEdit;
    QPushButton LoginoutBtn;
    QPushButton sendBtn;
    QLabel status;
    QString m_level;

    QLoginDialog loginDlg;

    ClientDemo m_client;

    QMap<QString, MSGHandler> m_handlerMap;

    /* 用来初始化与界面无关的成员 */
    void initMember();
    void initMsgGrpBx();
    void initInputGrpBx();
    void initListWidgetMenu();
    void connectSlots();

    void setCtrlEnabled(bool enabled);
    /* 用来得到用户勾选用户ID */
    QString getCheckedUserId();

    /* 发送用户名和密码用于登录与注册 */
    void SendIdAndPwd(QString head);

    void CONN_handler(QTcpSocket&, TextMessage&);
    void DSCN_handler(QTcpSocket&, TextMessage&);
    void LIOK_handler(QTcpSocket&, TextMessage&);
    void LIER_handler(QTcpSocket&, TextMessage&);
    void USER_handler(QTcpSocket&, TextMessage&);
    void MSGA_handler(QTcpSocket&, TextMessage&);
    void CTRL_handler(QTcpSocket&, TextMessage&);
    void INSE_handler(QTcpSocket&, TextMessage&);
private slots:
    void sendBtnClicked();
    void logInOutBtnClinked();
    void listWidgetMenuClicked();
    void listWidgetContextMenu(const QPoint&);
protected:
    bool eventFilter(QObject*, QEvent*);
public:
    MainWin(QWidget *parent = 0);
    void handle(QTcpSocket& obj, TextMessage& message);
    ~MainWin();
};

#endif // MAINWIN_H
