#include"server/ChatServer.hpp"
#include"Chatservice.hpp"
#include <iostream>
#include<signal.h>
using namespace std;
//处理服务器ctr+c结束后，重置user的状态信息
EventLoop* g_loop = nullptr;
void resetHandler(int)
{
    ChatService::instance()->reset();
        exit(0);

}
int main(int argc,char**argv){
    cout<<"server start"<<endl;
    if(argc<3)
    {
        cerr<<"command invalid! example: ./ChatServer 127.0.0.1 6000"<<endl;
        exit(-1);
    }
    char *ip=argv[1];
    uint16_t port=atoi(argv[2]);
    signal(SIGINT,resetHandler);

    EventLoop loop;
    InetAddress addr(ip,port);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();
    loop.loop();

    return 0;
}