#include"redis.hpp"
#include<iostream>
using namespace std;

Redis::Redis()
    :_publish_context(nullptr),_subcribe_context(nullptr)
{
}

Redis::~Redis()
{
    if(_publish_context!=nullptr)
    {
        redisFree(_publish_context);
    }
    if(_subcribe_context!=nullptr)
    {
        redisFree(_subcribe_context);
    }
}

bool Redis::connect()
{
    //负责publish发布消息的上下文连接
    _publish_context=redisConnect("127.0.0.1",6000);
    if(nullptr==_publish_context)
    {
        cerr<<"connect redis failed!"<<endl;
        return false;
    }

    //负责subscribe订阅消息的上下文连接
    _subcribe_context=redisConnect("127.0.0.1",6000);
    if (nullptr==_subcribe_context)
    {
        cerr<<"connect redis failed!"<<endl;
        return false;
    }


    //在单独的线程中，监听通道上的事件，有消息给业务层上报
    thread t([&](){
        observer_channel_message();
    });
    t.detach();
    cout<<"connect redis-server success!"<<endl;
    return true;  

}

//向redis指定的通道channel发布消息
bool Redis::publish(int channel,string message)
{
    redisReply *reply=(redisReply*)redisCommand(_publish_context,"PUBLISH channel_%d %s",channel,message.c_str());
    if (nullptr==reply)
    {
        cerr<<"publish command failed!"<<endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

//向redis指定的通到subscribe订阅消息
bool Redis::subscribe(int channel)
{
    //SUBCRIBE命令本身会造成线程阻塞等待通道里面的消息，这里制作订阅通道，不接受通道纤细
    //通道消息的接受专门在observer——channel——message函数的独立线程里面
    //只负责发送命令，不足时接受redis server响应消息，否则和nitifymsg线程抢占通道
    if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"SUBCRIBE %d",channel))
    {
        cerr<<"subscribe command failed!"<<endl;
        return false;
    }
    //redisBuffer可以循环发送缓冲区，直至缓冲区数据发送完毕（done为0）
    int done=0;
    while(!done)
    {
        if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done))
        {
            cerr<<"subscribe command failed!"<<endl;
            return false;
        }
    }
    return true;
}

//向redis指定的通道unsubscribe取消订阅消息
bool Redis::unsubscribe(int channel)
{
    if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"UNSUBCRIBE %d",channel))
    {
        cerr<<"unsubscribe command failed!"<<endl;
        return false;
    }
    int done=0;
    while(!done)
    {
        if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done))
        {
            cerr<<"unsubscribe command failed!"<<endl;
            return false;
        }
    }
    return true;
} 
//在独立的线程中接收订阅通道的消息
void Redis::observer_channel_message()
{
    redisReply *reply=nullptr;
    while (REDIS_OK==redisGetReply(this->_subcribe_context,(void**)&reply))
    {
        //订阅收到的消息是一个带有3个元素的数组
        //元素1：message
        //元素2：channel
        //元素3：实际消息内容
      if(reply!=nullptr&&reply->element[2]!=nullptr&&reply->element[2]->str!=nullptr)
      {
        //给业务层上报
         _notify_message_handler(atoi(reply->element[1]->str),reply->element[2]->str);

      }
        freeReplyObject(reply);
    }   
    cerr<<"<<<<<<<<<<<<<<<<<<< observer_channel_message quit <<<<<<<<<<<<<<"<<endl;
}