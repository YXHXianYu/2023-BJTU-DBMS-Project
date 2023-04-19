#ifndef TABLE_H
#define TABLE_H

#include<iostream>
#include<vector>
#include<any>
#include<unordered_map>
#include"all_constraints.h"
#include"colasqltool.h"
#include "constants.h"
class Table {
private:
    std::string table_name;
    std::vector<std::pair<std::string, std::string>> fields; //pair<field_name, field_type>
    std::unordered_map<std::string, std::string> field_map;
    std::vector<std::unordered_map<std::string, std::any> > records;
    std::vector<Constraint*> constraints;

private:
    Table();
public:
    Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);
    //获取表名
    std::string GetTableName() const;
    //Select查询记录
    int Select(std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string,int>> conditions,        std::vector<std::vector<std::any>> &return_records);
    //Insert插入记录
    int Insert(std::vector<std::pair<std::string, std::string>> record_in);
    //Delete删除记录
    int Delete(std::vector<std::tuple<std::string, std::string, int>> conditions);
    //检查记录是否满足Where条件
    int CheckCondition(const std::unordered_map<std::string, std::any>& record, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
    int Update(const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
    //更新记录
};

#endif // TABLE_H
