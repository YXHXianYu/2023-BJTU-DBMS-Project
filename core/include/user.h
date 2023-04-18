#ifndef __USER_H__
#define __USER_H__

#include<cstring>
#include<iostream>
#include<vector>
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
    static constexpr int kIdentifiedSuccess = 0;
    static constexpr int kIdentifiedFail = 1;
};

#endif // __USER_H__
