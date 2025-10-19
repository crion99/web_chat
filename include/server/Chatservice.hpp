#ifndef CHATSERVIVE_H
#define CHATSERVIVE_H
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include "json.hpp"
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;
// 处理消息的回调处理
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;
// 聊天服务器业务类
class ChatService
{
public:
   // 获取单例对象的接口函数
   static ChatService *instance();
   // 登录业务
   void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
   // 注册业务

   void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

   //获取消息对应的处理器
   MsgHandler getHandler(int msgid);
private:
   ChatService();
   // 储存消息id和其对应的业务处理方法
   unordered_map<int, MsgHandler> _msgHandlerMap;
};
#endif