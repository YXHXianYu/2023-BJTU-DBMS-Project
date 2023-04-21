#ifndef __USER_H__
#define __USER_H__

#include<cstring>
#include<iostream>
#include<vector>
#include"constants.h"

enum Authority {
    Admin,
    All,
    Query,
};

class User {
private:
    std::string user_name;
    std::string user_password;
    Authority authority;

public:
    User(std::string user_name, std::string user_password);
    User(std::string user_name, std::string user_password, Authority authority);
    //验证密码
    int Identify(std::string password);
    //获取用户名
    std::string GetUserName() const ;
    //获取密码
    std::string GetUserPassword() const ;
    //获取权限
    Authority GetAuthority() const ;

};

#endif // __USER_H__
