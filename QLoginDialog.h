#ifndef _QLOGINDIALOG_H_
#define _QLOGINDIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include "Definition.h"

class QLoginDialog : public QDialog
{
    Q_OBJECT
public:
    typedef bool (*ValFunc)(QString);
private:
    QLabel UserLabel;
    QLabel PwdLabel;
    QLabel CaptLabel;
    QLineEdit UserEdit;
    QLineEdit PwdEdit;
    QLineEdit CaptEdit;
    QPushButton LoginBtn;
    QPushButton CancelBtn;
    QPushButton RegisterBtn;
    QString m_user;
    QString m_pwd;
    QString m_captcha;
    Qt::GlobalColor* m_colors;
    QTimer m_timer;
    /* 指向外部成员的函数指针 */
    ValFunc m_vf;
private slots:
    void LoginBtn_Clicked();
    void CancelBtn_Clicked();
    void RegisterBtn_Clicked();
    void Timer_Timeout();
protected:
    void paintEvent(QPaintEvent *);
    QString getCaptcha();
    Qt::GlobalColor* getColors();
    void showEvent(QShowEvent *);
public:
    QLoginDialog(QWidget *parent = 0);
    QString getUser();
    QString getPwd();
    void setValFunc(ValFunc);
    ~QLoginDialog();
private:
    bool CheckIdAndPwd(int status);
};


#endif

