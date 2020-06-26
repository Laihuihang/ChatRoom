#include "MainWin.h"
#include <QHBoxLayout>
#include <QGridLayout>

MainWin::MainWin(QWidget *parent)
    : QWidget(parent), loginDlg(this), m_level("")
{
    initMember();
    initMsgGrpBx();
    initInputGrpBx();
    initListWidgetMenu();
    connectSlots();

    vMainLayout.setSpacing(10);
    vMainLayout.addWidget(&msgGrpBx);
    vMainLayout.addWidget(&inputGrpBx);

    setWindowTitle("NULL的聊天室");
    setLayout(&vMainLayout);
    setMinimumSize(550,400);
    resize(550,400);
}
void MainWin::initMsgGrpBx()
{
    QHBoxLayout* hbl = new QHBoxLayout();

    hbl->setContentsMargins(2,5,2,2);
    hbl->addWidget(&msgEditor,7);
    hbl->addWidget(&listwidget,3);

    msgEditor.setReadOnly(true);
    msgEditor.setFocusPolicy(Qt::NoFocus);
    listwidget.setFocusPolicy(Qt::NoFocus);
    /* emit a signal */
    listwidget.setContextMenuPolicy(Qt::CustomContextMenu);


    msgGrpBx.setLayout(hbl);
    msgGrpBx.setTitle("聊天信息");
}
void MainWin::initInputGrpBx()
{
    QGridLayout* gl = new QGridLayout();
    gl->setSpacing(10);
    gl->addWidget(&inputEdit,0,0,1,5);
    gl->addWidget(&status,1,0,1,3);
    gl->addWidget(&LoginoutBtn,1,3);
    gl->addWidget(&sendBtn,1,4);

    inputEdit.setFixedHeight(23);
    /* 初始状态下组件都不可用 */
    inputEdit.setEnabled(false);
    /* 注册事件过滤器 */
    inputEdit.installEventFilter(this);
    status.setText("状态：未登录");
    LoginoutBtn.setFixedHeight(30);
    LoginoutBtn.setText("登陆");
    sendBtn.setFixedHeight(30);
    sendBtn.setText("发送");
    sendBtn.setEnabled(false);

    inputGrpBx.setFixedHeight(100);
    inputGrpBx.setLayout(gl);
    inputGrpBx.setTitle("用户名");
}
void MainWin::initListWidgetMenu()
{
    QAction* act = nullptr;

    act = listWidgetMenu.addAction("禁言", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("silent");

    act = listWidgetMenu.addAction("恢复", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("recover");

    listWidgetMenu.addSeparator();

    act = listWidgetMenu.addAction("封禁", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("kick");
}
void MainWin::connectSlots()
{
    connect(&sendBtn,SIGNAL(clicked(bool)),this,SLOT(sendBtnClicked()));
    connect(&LoginoutBtn,SIGNAL(clicked(bool)),this,SLOT(logInOutBtnClinked()));
    connect(&listwidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listWidgetContextMenu(QPoint)));
}
void MainWin::setCtrlEnabled(bool enabled)
{
    inputEdit.setEnabled(enabled);
    status.setText(enabled?"状态：连接成功":"状态：未登录");
    LoginoutBtn.setText(enabled?"退出":"登陆");
    sendBtn.setEnabled(enabled);

    if(enabled)
    {
        inputEdit.setFocus();
    }
    else
    {
        msgEditor.clear();
        listwidget.clear();
        inputEdit.clear();
    }
}

