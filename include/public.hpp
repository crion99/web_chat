#ifndef PUBLIC_H
#define PUBLIC_H

/*
server和client的公共文件
*/
enum EnMsgTypr{
    LOGIN_MSG = 1,//登录消息
    REG_MSG,//注册消息
    REG_MSG_ACK  //注册响应
};
#endif