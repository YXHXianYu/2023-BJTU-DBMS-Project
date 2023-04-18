#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include<vector>
#include<any>
#include"database.h"
#include"user.h"
#include"constants.h"
#include".\constraint\all_constraints.h"
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
//数据库
public:
    int CreateUser(std::string user_name, std::string user_password);           //创建用户
    int Login(std::string user_name, std::string user_password);                //登录用户
//数据库
public:
    int CreateDatabase(std::string database_name);                              //创建数据库
    int UseDatabase(std::string database_name);                                 //使用数据库
    std::vector<std::string> ShowDatabases();                                   //显示所有数据库
//表
public:
    int CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);                                                               // 创建表
    int DropTable(std::string table_name);                                      //删除表
    int DescribeTable(std::string table_name);                                  //查看表结构
    int ShowTables(std::string table_name);                                     //显示所有表
//记录
public:
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in); // 插入记录
    int Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string, int>> conditions, std::vector<std::vector<std::any>> &return_records); // 查询记录
    int Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions);                                                                                          // 删除记录

public:
    
    
};

#endif // DATAPROCESSOR_H