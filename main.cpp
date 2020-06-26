#include "MainWin.h"
#include <QApplication>

/*
 * 一般情况下，聊天服务端只负责消息的转发
 * 客户端的连接与断开是两个关键操作，必须消息通知
 * 服务端能够随时对指定客户端发送消息
 *
 * 任何与网络相关的操作都需要处理连接成功与断开连接这讲个操作
 * 聊天消息与控制消息
 *
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWin w;
    w.show();

    return a.exec();
}
