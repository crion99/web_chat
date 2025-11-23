#ifndef CHATSERVIVE_H
#define CHATSERVIVE_H
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>

#include "json.hpp"
#include"usermodel.hpp"
#include"offlinemessagemodel.hpp"
#include"friendmodel.hpp"
#include"groupmodel.hpp"
#include"redis/redis.hpp"

#include<mutex>
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
   //一对一聊天业务
   void oneChat(const TcpConnectionPtr&conn,json&js,Timestamp time);
   //添加好友
   void addFriend(const TcpConnectionPtr&conn,json&js,Timestamp time);
   //创建群组
   void creatGroup(const TcpConnectionPtr&conn,json&js,Timestamp time);
   //加入群组
   void addGroup(const TcpConnectionPtr&conn,json&js,Timestamp time);
   //群组聊天
   void groupChat(const TcpConnectionPtr&conn,json&js,Timestamp time);
   //处理注销业务的
   void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    
   //获取消息对应的处理器
   MsgHandler getHandler(int msgid);

   // 处理redis订阅消息
   void handleRedisSubscribeMessage(int channel, string message);
 
   
   //处理客户端异常退出
   void clientCloseException(const TcpConnectionPtr &conn);
   //服务器异常后，业务重置方法
   void reset();
private:
   ChatService();
   // 储存消息id和其对应的业务处理方法
   unordered_map<int, MsgHandler> _msgHandlerMap;

    //储存在线用户的通信连接
   unordered_map<int,TcpConnectionPtr> _userConnMap;

   //定义互斥锁，保证——userConnMap的安全
   mutex _connMutex;


   //数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    //redis操作对象
     Redis _redis;
};
#endif