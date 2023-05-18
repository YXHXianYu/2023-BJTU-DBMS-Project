#include "user.h"

User::User(std::string user_name, std::string user_password, std::vector<priviledge> authorities) {
    this->user_name = user_name;
    this->user_password = user_password;
    this->authorities = authorities;
    return;
}

User::User(std::string user_name, std::string user_password) {
    this->user_name = user_name;
    this->user_password = user_password;
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
//查询数据库级权限
int User::CheckAuthority(std::string database_name, authority_number number) const {
    if(user_name == "admin") {
        return kSuccess;
    }
    for(const auto& authority : authorities) {
        if(authority.database_name == database_name && authority.number == number) return kSuccess;
    }
    return kInsufficientAuthority;
}
//查询表级权限
int User::CheckAuthority(std::string database_name, std::string table_name, authority_number number) const {
    if(user_name == "admin") {
        return kSuccess;
    }
    for(const auto& authority : authorities) {
        if(authority.database_name != database_name) continue;
        if(authority.table_name != table_name) continue;
        if(authority.number == number) return kSuccess;
    }
    return kInsufficientAuthority;
}
//添加数据库级权限
int User::GrantAuthority(std::string database_name, authority_number number) {
    if(CheckAuthority(database_name, number) == kSuccess) return kAuthorityExisted;
    priviledge authority;
    authority.database_name = database_name;
    authority.table_name = "";
    authority.number = number;
    authorities.push_back(authority);
    return kSuccess;
}
//添加表级权限
int User::GrantAuthority(std::string database_name, std::string table_name, authority_number number) {
    if(CheckAuthority(database_name, table_name, number) == kSuccess) return kAuthorityExisted;
    priviledge authority;
    authority.database_name = database_name;
    authority.table_name = table_name;
    authority.number = number;
    authorities.push_back(authority);
    return kSuccess;
}
//删除数据库级权限
int User::RevokeAuthority(std::string database_name, authority_number number) {
    if(CheckAuthority(database_name, number) != kSuccess) return kAuthorityNotFound;
    for(int i = 0; i < authorities.size(); ++i) {
        const auto& authority = authorities[i];
        if(authority.database_name == database_name && authority.table_name == "" && authority.number == number) {
            authorities.erase(authorities.begin() + i);
            return kSuccess;
        }
    }
    return kSuccess;
}
//删除表级权限
int User::RevokeAuthority(std::string database_name, std::string table_name, authority_number number) {
    if(CheckAuthority(database_name, table_name, number) != kSuccess) return kAuthorityNotFound;
    for(int i = 0; i < authorities.size(); ++i) {
        const auto& authority = authorities[i];
        if(authority.database_name == database_name && authority.table_name == table_name && authority.number == number) {
            authorities.erase(authorities.begin() + i);
            return kSuccess;
        }
    }
    return kSuccess;
}
//授予所有表级权限
int User::GrantAllTableAuthorities(std::string database_name, std::string table_name) {
    if(CheckAuthority(database_name, table_name, authority_number::SELECT) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::SELECT);
    }
    if(CheckAuthority(database_name, table_name, authority_number::DELETE) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::DELETE);
    }
    if(CheckAuthority(database_name, table_name, authority_number::INSERT) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::INSERT);
    }
    if(CheckAuthority(database_name, table_name, authority_number::UPDATE) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::UPDATE);
    }
    if(CheckAuthority(database_name, table_name, authority_number::INDEX) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::INDEX);
    }
    if(CheckAuthority(database_name, table_name, authority_number::ALTER) != kSuccess) {
        GrantAuthority(database_name, table_name, authority_number::ALTER);
    }
    return kSuccess;
}
//授予所有数据库级权限
int User::GrantAllDatabaseAuthorities(std::string database_name) {
    if(CheckAuthority(database_name,  authority_number::CREATE) != kSuccess) {
        GrantAuthority(database_name, authority_number::CREATE);
    }
    if(CheckAuthority(database_name, authority_number::DROP) != kSuccess) {
        GrantAuthority(database_name, authority_number::DROP);
    }
    return kSuccess;
}
//收回所有表级权限
int User::RevokeAllTableAuthorities(std::string database_name, std::string table_name) {
    for(auto it = authorities.begin(); it != authorities.end();) {
        if(it->database_name == database_name && it->table_name == table_name) {
            authorities.erase(it);
        }
        else it++;
    }
    return kSuccess;
}
//收回所有数据库级权限
int User::RevokeAllDatabaseAuthorities(std::string database_name) {
    for(auto it = authorities.begin(); it != authorities.end();) {
        if(it->database_name == database_name && it->table_name =="") {
            authorities.erase(it);
        }
        else it++;
    }
    return kSuccess;
}
//收回所有数据库级权限和表级权限，指定数据库名
int User::RevokeAllDatabaseAndTableAuthorities(std::string database_name) {
    for(auto it = authorities.begin(); it != authorities.end();) {
        if(it->database_name == database_name) {
            authorities.erase(it);
        }
        else it++;
    }
    return kSuccess;
}
int User::CheckDatabaseInAuthorities(std::string database_name) {
    if(user_name == "admin") {
        return kSuccess;
    }
    for(const auto& authority : authorities) {
        if(authority.database_name == database_name) return kSuccess;
    }
    return kDatabaseNotFound;
}
int User::CheckTableInAuthorities(std::string database_name, std::string table_name) {
    if(user_name == "admin") {
        return kSuccess;
    }
    for(const auto& authority : authorities) {
        if(authority.database_name == database_name && authority.table_name == table_name) return kSuccess;
    }
    return kDatabaseNotFound;
}
std::vector<priviledge>  User::GetAuthorities() const {
    return authorities;
}