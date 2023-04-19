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
    //验证密码
    int Identify(std::string password);
    //获取用户名
    std::string GetUserName();

};

#endif // __USER_H__
