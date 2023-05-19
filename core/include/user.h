/**
 * author: drj
 * 用户类。存储了用户名、权限等。
 * 提供了检查权限、授予权限、收回权限等权限相关的接口。
 * 用户登录时需在此类检查用户名和密码是否匹配。
*/
#ifndef __USER_H__
#define __USER_H__

#include<cstring>
#include<iostream>
#include<vector>
#include"constants.h"

enum authority_number {
    //表级权限
    SELECT,
    DELETE,
    INSERT,
    UPDATE,
    INDEX,
    ALTER,
    //数据库级权限
    CREATE,
    DROP,
};
struct priviledge {
    std::string database_name;
    std::string table_name;
    authority_number number;
    //数据库级权限，table_name = ""
};

class User {
public:
    
private:
    
    std::vector<priviledge> authorities;
    std::string user_name;
    std::string user_password;

public:
    User(std::string user_name, std::string user_password, std::vector<priviledge> authorities);
    User(std::string user_name, std::string user_password);
    //验证密码
    int Identify(std::string password);
    //获取用户名
    std::string GetUserName() const ;
    //获取密码
    std::string GetUserPassword() const ;
    //查询数据库级权限
    int CheckAuthority(std::string database_name, authority_number number) const;
    //查询表级权限
    int CheckAuthority(std::string database_name, std::string table_name, authority_number number) const;
    //添加数据库级权限
    int GrantAuthority(std::string database_name, authority_number number) ;
    //添加表级权限
    int GrantAuthority(std::string database_name, std::string table_name, authority_number number) ;
    //删除数据库级权限
    int RevokeAuthority(std::string database_name, authority_number number) ;
    //删除表级权限
    int RevokeAuthority(std::string database_name, std::string table_name, authority_number number) ;
    int GrantAllTableAuthorities(std::string database_name, std::string table_name);
    int GrantAllDatabaseAuthorities(std::string database_name);
    int RevokeAllTableAuthorities(std::string database_name, std::string table_name);
    int RevokeAllDatabaseAuthorities(std::string database_name);
    int RevokeAllDatabaseAndTableAuthorities(std::string database_name);
    int CheckDatabaseInAuthorities(std::string database_name);
    int CheckTableInAuthorities(std::string database_name, std::string table_name);
    //获取权限
    std::vector<priviledge> GetAuthorities() const ;

};

#endif // __USER_H__
