#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include<vector>
#include<any>
#include"database.h"
#include"user.h"
#include"constants.h"

#include".\constraint\all_constraints.h"

// file
#include "filemanager.h"

class DataProcessor {
private:
    std::vector<Database> databases;
    std::vector<User> users;
    Database* current_database;
    User* current_user;
private:
    DataProcessor();
public:
    static DataProcessor& GetInstance();
//数据库
public:
    int CreateUser(std::string user_name, std::string user_password);           //创建用户
    int Login(std::string user_name, std::string user_password);                //登录用户
//数据库
public:
    int CreateDatabase(std::string database_name);                              //创建数据库
    int UseDatabase(std::string database_name);                                 //使用数据库
    int ShowDatabases(std::vector<std::string>& return_databases);      //显示所有数据库
//表
public:
    int ShowTables(std::vector<std::string>& return_tables);                                     //显示所有表
    int CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);                                                               // 创建表
    int DropTable(std::string table_name);                                      //删除表
    int DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>&    constraints);                                  //查看表结构

    int AlterTableAdd(std::string table_name, std::pair<std::string, std::string> field);//修改表结构,增加字段
    int AlterTableDrop(std::string table_name, std::string field_name);          //删除字段
    int AlterTableModify(std::string table_name, std::pair<std::string, std::string> field); //修改字段
    int AlterTableConstraint(std::string table_name, Constraint constraint);    //添加约束条件
    int AlterTableDeleteConstraint(std::string table_name, std::string constraint_name); ///删除约束

//记录
public:
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in); // 插入记录
    int Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string, int>> conditions, std::vector<std::vector<std::any>> &return_records); // 查询记录
    int Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions);                                                                                         // 删除记录
    int Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions);                      //更新记录
    //values: vector<>

//文件
public:
    int Read(bool debug = false);
    int Write();

};

#endif // DATAPROCESSOR_H
