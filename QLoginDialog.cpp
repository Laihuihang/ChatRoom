#include "QLoginDialog.h"

#include <QPainter>
#include <QTime>
//#include <QDebug>
#include <QMessageBox>

QLoginDialog::QLoginDialog(QWidget* parent) :
    QDialog(parent, Qt::WindowCloseButtonHint)
  , UserLabel(this)
  , PwdLabel(this)
  , CaptLabel(this)
  , UserEdit(this)
  , PwdEdit(this)
  , CaptEdit(this)
  , LoginBtn(this)
  , CancelBtn(this)
  , RegisterBtn(this)
  , m_vf(nullptr)
{
    UserLabel.setText("用户名:");
    UserLabel.move(20, 30);
    UserLabel.resize(60, 25);

    UserEdit.move(85, 30);
    UserEdit.resize(180, 25);

    PwdLabel.setText("密码:");
    PwdLabel.move(20, 65);
    PwdLabel.resize(60,25);

    PwdEdit.move(85, 65);
    PwdEdit.resize(180, 25);
    PwdEdit.setEchoMode(QLineEdit::Password);

    CaptLabel.setText("验证码:");
    CaptLabel.move(20, 100);
    CaptLabel.resize(60, 25);

    CaptEdit.move(85, 100);
    CaptEdit.resize(85, 25);

    CancelBtn.setText("取消");
    CancelBtn.move(40, 145);
    CancelBtn.resize(70, 30);

    RegisterBtn.setText("注册");
    RegisterBtn.move(120,145);
    RegisterBtn.resize(70,30);

    LoginBtn.setText("登陆");
    LoginBtn.move(195, 145);
    LoginBtn.resize(70, 30);

    m_timer.setParent(this);

    setWindowTitle("登陆");
    setFixedSize(285, 205);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(Timer_Timeout()));
    connect(&LoginBtn, SIGNAL(clicked()), this, SLOT(LoginBtn_Clicked()));
    connect(&CancelBtn, SIGNAL(clicked()), this, SLOT(CancelBtn_Clicked()));
    connect(&RegisterBtn, SIGNAL(clicked()), this, SLOT(RegisterBtn_Clicked()));

    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());//种下种子

    m_timer.start(100);
}

bool QLoginDialog::CheckIdAndPwd(int status)
{
    QString captcha = CaptEdit.text().replace(" ", "");

    if( m_captcha.toLower() == captcha.toLower() )//不区分大小写
    {
        m_user = UserEdit.text().trimmed();
        m_pwd = PwdEdit.text();

        if( m_user == "" )
        {
            QMessageBox::information(this, "消息", "用户名不能为空!");
        }
        else if( m_pwd == "" )
        {
            QMessageBox::information(this, "消息", "密码不能为空");
        }
        else if((m_vf != nullptr) && !(m_vf(m_user)))
        {
            QMessageBox::critical(this, "错误", "用户名不合法");
        }
        else
        {
            done(status);
        }
    }
    else
    {
        QMessageBox::critical(this, "错误", "验证码不匹配!");

        m_captcha = getCaptcha();

        CaptEdit.selectAll();
    }
}

void QLoginDialog::LoginBtn_Clicked()
{
    CheckIdAndPwd(LOGIN);
}

void QLoginDialog::RegisterBtn_Clicked()
{
    CheckIdAndPwd(REGISTER);
}

void QLoginDialog::CancelBtn_Clicked()
{
    done(CANCLE);
}

QString QLoginDialog::getUser()
{
    return m_user;
}

QString QLoginDialog::getPwd()
{
    return m_pwd;
}
void QLoginDialog::Timer_Timeout()
{
    m_colors = getColors();
    update(); //每100ms改变颜色
}
void QLoginDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(180, 100, 84, 24, Qt::white);//先画个白色矩形

    painter.setFont(QFont("Comic Sans MS", 12));

    for(int i=0; i<150; i++)
    {
        painter.setPen(m_colors[i%4]);
        painter.drawPoint(180 + qrand() % 84, 100 + qrand() % 24);//在矩形区域增加噪点
    }

    for(int i=0; i<4; i++)
    {
        painter.setPen(m_colors[i]);
        painter.drawText(180 + 20 * i, 100, 20, 24, Qt::AlignCenter, QString(m_captcha[i]));
    }

    QDialog::paintEvent(event);
}
QString QLoginDialog::getCaptcha()
{
    QString ret = "";

    for(int i=0; i<4; i++)
    {
        int c = (qrand() % 2) ? 'a' : 'A';

        ret += static_cast<QChar>(c + qrand() % 26);
    }

    return ret;
}
Qt::GlobalColor* QLoginDialog::getColors()
{
    static Qt::GlobalColor colors[4];

    for(int i=0; i<4; i++)
    {
        colors[i] = static_cast<Qt::GlobalColor>(2 + qrand() % 16);//颜色范围2，17
    }

    return colors;
}
void QLoginDialog::showEvent(QShowEvent * event)
{
    m_captcha = getCaptcha();
    m_colors = getColors();

    QDialog::showEvent(event);
}
QLoginDialog::~QLoginDialog()
{

}

void QLoginDialog::setValFunc(ValFunc vf)
{
    m_vf = vf;
}

