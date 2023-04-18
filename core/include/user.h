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

};

#endif // __USER_H__
