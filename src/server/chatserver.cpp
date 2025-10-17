#include "server/ChatServer.hpp"
#include "json.hpp"
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json= nlohmann::json;

ChatServer::ChatServer(EventLoop *_loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(_loop, listenAddr, nameArg),
      _loop(_loop)
{
    // 注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    // 设置线程数量
    _server.setThreadNum(4); // 1个I/O线程，3个worker线程
}
// 启动服务
void ChatServer::start()
{
    _server.start();
}
// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &coon)
{
    if(!coon->connected())
    {
        coon->shutdown();

    }
}
// 上报读写事件相关的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp)
{
    string buf=buffer->retrieveAllAsString();
    //数据的反序列化
    json js=json::parse(buf);
    //达到目的：完全解读网络模块的代码和业务模块的代码
    //通过js读出的message id 获取业务处理器=》coon js time

}
