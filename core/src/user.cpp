#include "user.h"

User::User(std::string user_name, std::string user_password, Authority authority) {
    this->user_name = user_name;
    this->user_password = user_password;
    this->authority = authority;
    return;
}

User::User(std::string user_name, std::string user_password): User(user_name, user_password, Authority::All) {
    return;
}

int User::Identify(std::string password) {
    if(user_password == password) return kSuccess;
    return kIdentifiedFail;
}

std::string User::GetUserName() const {
    return user_name;
}

std::string User::GetUserPassword() const  {
    return user_password;
}

Authority User::GetAuthority() const {
    return authority;
}