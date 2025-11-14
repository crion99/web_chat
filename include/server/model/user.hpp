#ifndef USER_H
#define USER_H
#include <string>
using namespace std;
//匹配user表的orm类
class User
{
public:
    User(int id = 1, string name = "", string pwd = "", string state = "offline")
    {
        this->id=id;
        this->name=name;
        this->password=pwd;
        this->state=state;
    }
    void setId(int id){this->id=id;}
    void setName(string name){this->name=name;}
    void setPwd(string password){this->password=password;}
    void setState(string state){this->state=state;}



    int getId(){return this->id;}
    string getName(){return this->name=name;}
    string getPwd(){return this->password=password;}
    string getState(){return this->state=state;}



private:
    int id;
    string name;
    string password;
    string state;
};

#endif