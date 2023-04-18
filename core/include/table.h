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
    std::string GetTableName() const;
    Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint*> constraints);
    int Select(std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string,int>> conditions,        std::vector<std::vector<std::any>> &return_records);
    int Insert(std::vector<std::pair<std::string, std::string>> record_in);
    int Delete(std::vector<std::tuple<std::string, std::string, int>> conditions);
    int CheckCondition(const std::unordered_map<std::string, std::any>& record, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
};

#endif // TABLE_H
