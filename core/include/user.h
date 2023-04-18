#ifndef __USER_H__
#define __USER_H__

#include<cstring>
#include<iostream>
#include<vector>
#include"constants.h"
class User {
private:
    std::string user_name;
    std::string user_password;


private:
    User();
public:
    User(std::string user_name, std::string user_password);
    int Identify(std::string password);
    std::string GetUserName();
//常量
public:
};

#endif // __USER_H__
