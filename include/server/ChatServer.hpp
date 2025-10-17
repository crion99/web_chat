#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <string>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;
// 聊天服务器类
class ChatServer {
public:
    ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg);
    void start();
private:
// 连接相关的回调函数
    void onConnection(const TcpConnectionPtr &);
// 消息相关的回调函数
    void onMessage(const TcpConnectionPtr &, Buffer *, Timestamp);
    EventLoop *_loop;// 事件循环
    TcpServer _server;// 服务器对象
};

#endif