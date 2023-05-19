#ifndef __MY_PARSER_H__
#define __MY_PARSER_H__

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <any>

// cmd
#include "all_constraints.h"
#include "colasqltool.h"

// core
#include "dataprocessor.h"
#include "constants.h"

namespace ColaSQLCommand {

/**
 * A Co1aSQL Parser
 * 读入一句Co1aSQL语句，将其解析成一段core模块的接口调用
*/
class Parser {
public:
    /**
    * 构造函数
    */
    Parser();

    /**
    * Parse一条SQL语句
    */
    std::string Parse(const std::vector<std::string>& seq);

    /**
    * Parser一条Select语句，并以vector的方法返回结果，而非打印进stdout
    */
    std::string ParseSelect(const std::vector<std::string>& seq, std::vector<std::vector<std::any>>& result);

    /**
    * DEBUG参数
    */
    const bool DEBUG = false;

    const std::string success = "Success!";
    const std::string error = "Error: ";
    const std::string statementIncomplete = "Statement is incomplete.";
    const std::string statementError = "Statement has errors.";
    const std::string statementRedundant = "Statment has redundant parts.";

private:

    std::string CreateUser(const std::vector<std::string>& seq);    // 创建用户
    std::string GrantUser(const std::vector<std::string>& seq);     // 授予用户权限
    std::string RevokeUser(const std::vector<std::string>& seq);    // 收回用户权限
    std::string DeleteUser(const std::vector<std::string>& seq);    // 删除用户
    std::string Login(const std::vector<std::string>& seq);         // 登陆(切换用户)


    std::string CreateDatabase(const std::vector<std::string>& seq);// 创建数据库
    std::string DeleteDatabase(const std::vector<std::string>& seq);// 删除数据库
    std::string UseDatabase(const std::vector<std::string>& seq);   // 使用数据库
    std::string ShowDatabases(const std::vector<std::string>& seq); // 显示所有数据库

    std::string CreateTable(const std::vector<std::string>& seq);   // 创建表
    std::string DeleteTable(const std::vector<std::string>& seq);   // 删除表
    std::string AlterTableAdd(const std::vector<std::string>& seq); // 修改表（添加一个字段）
    std::string AlterTableDrop(const std::vector<std::string>& seq);        // 修改表（删除一个字段）
    std::string AlterTableModify(const std::vector<std::string>& seq);      // 修改表（修改一个字段）
    std::string AlterTableConstraint(const std::vector<std::string>& seq);  // 修改表（增加一个约束）
    std::string AlterTableDeleteConstraint(const std::vector<std::string>& seq); // 修改表（删除一个约束）
    std::string QueryTable(const std::vector<std::string>& seq);    // 描述表结构
    std::string ShowTables(const std::vector<std::string>& seq);    // 显示所有表（当前数据库下的）

    std::string InsertRecord(const std::vector<std::string>& seq);  // 插入记录
    std::string DeleteRecord(const std::vector<std::string>& seq);  // 删除记录
    std::string SelectRecord(const std::vector<std::string>& seq);  // 选择记录
    std::string UpdateRecord(const std::vector<std::string>& seq);  // 更新记录
    std::string ReturnSelectRecord(const std::vector<std::string>& seq, std::vector<std::vector<std::any>>& result); // 以vector返回result

    std::string BuildIndex(const std::vector<std::string>& seq);    // 构建索引

    std::string ShowConstraints(const std::vector<std::string>& seq);   // 显示所有约束

    std::string Read(bool debug = false); // 读取(Rollback)
    std::string Save();                   // 存储(Commit)

    std::string GetErrorMessage(int errorCode); // 获取错误信息

    bool PushCondition(std::vector<std::tuple<std::string, std::string, int>>& conditions,
                       std::string fieldName, std::string sign, std::string value); // 往conditions中添加一个条件
};

} // ColaSQLCommand

#endif // __MY_PARSER_H__
