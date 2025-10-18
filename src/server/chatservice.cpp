#include "Chatservice.hpp"
// 获取单例对象的接口函数
ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}
// 注册消息以及对应的回调操作
ChatService::ChatService()
{
}
// 登录业务
void ChatService::login(const TcpConnection &conn, json &js, Timestamp time)
{
}
// 注册业务
void ChatService::reg(const TcpConnection &conn, json &js, Timestamp time)
{
}
