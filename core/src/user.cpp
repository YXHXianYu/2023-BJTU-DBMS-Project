#include "user.h"

User::User() {}

User::User(std::string user_name, std::string user_password) {
    this->user_name = user_name;
    this->user_password = user_password;
    return;
}

