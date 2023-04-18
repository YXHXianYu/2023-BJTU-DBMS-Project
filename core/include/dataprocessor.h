#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include<vector>
#include<any>
#include"database.h"
#include"User.h"
class DataProcessor {
private:
    std::vector<Database> databases;
    std::vector<User> users;
    Database* current_database;
    User* current_user;
private:
    DataProcessor();
public:
    static DataProcessor GetInstance();
public:
    int CreateUser(std::string user_name, std::string user_password);//创建用户
    int Login(std::string user_name, std::string user_password);//登录用户

public:
    int CreateDatabase(std::string database_name);//创建数据库
    int UseDatabase(std::string database_name);//使用数据库
public:
    int CreateTable(std::string table_name);//创建表
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::any>>);//向表内插入数据
    std::vector<std::vector<std::any>> Select(std::string table_name, std::vector<std::string> field_name);//查询数据
public:
    static int constexpr kSuccess = 0;
    static int constexpr kUserNameExisted = 1;
    static int constexpr kUserNameNotFound = 2;
    static int constexpr kUserPasswordError = 3;
    static int constexpr kUserNotLogin = 4;
    static int constexpr kDatabaseNotFound = 5;
    static int constexpr kDatabaseExisted = 6;
    static int constexpr kTableNameExisted = 7;

};

#endif // DATAPROCESSOR_H