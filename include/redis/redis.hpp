#ifndef REDIS_H
#define REDIS_H

#include<hiredis/hiredis.h>
#include<thread>
#include<functional>
using namespace std;

/*
redis作为集群服务器同学的基于发布-订阅消息队列时，会遇到两个最难搞的bug问题
1. redis客户端在订阅消息后，会阻塞在redisGetReply函数上，导致无法处理其他事情
2. redis客户端在订阅消息后，无法再执行其他的redis命令
*/
class Redis
{

    public:
    Redis();
    ~Redis();
    
    //连接redis服务器
    bool connect();

    //向redis指定的通道channel发布消息
    bool publish(int channel,string message);

    //向redis指定的通道subscribe订阅消息
    bool subscribe(int channel);

    //向redis指定的通道unsubscribe取消订阅消息
    bool unsubscribe(int channel);

    //在独立线程中接受订阅通道的消息
    void observer_channel_message();

    //初始化业务层上同胞消息的回调对象
    void init_notify_handler(function<void(int,string)> fn);

private:
    //hiredis同步上下文对象，负责public消息
    redisContext *_publish_context;

    //hiredis同步上下文对象，负责subscribe消息
    redisContext *_subcribe_context;

    //回调操作，收到订阅的消息，给servicxe层上报
    function <void(int,string)> _notify_message_handler;




};
#endif