#ifndef DEFINITION_H
#define DEFINITION_H

/* 数据库查询相关的返回错误 */
#define SQL_OK          (0x00000000)
#define SQL_NO_ACCOUNT  (0x10000000)
#define SQL_PWD_ERROR   (0x10000001)

/* 服务器返回登录错误类型 */
#define LIER_HASLOGIN   (0x20000001)
#define LIER_NO_ACCOUNT (0x20000002)
#define LIER_PWD_ERROR  (0x20000003)

/* 登录界面状态码 */
#define REGISTER        (0x30000001)
#define LOGIN           (0x30000002)
#define CANCLE          (0x30000003)

/* 数据库插入相关错误 */
#define INSERT_OK       (0x00000000)
#define INSERT_ERROR    (0x00000001)

#endif // DEFINITION_H
