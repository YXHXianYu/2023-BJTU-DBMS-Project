#ifndef TABLE_H
#define TABLE_H

#include<iostream>
#include<vector>
#include<any>
#include<unordered_map>
#include"./constraint/all_constraints.h"
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
    
    Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint*> constraints);
    std::vector<std::vector<std::any>> Select(std::vector<std::string> field_name);
    int Insert(std::vector<std::pair<std::string, std::string>> record_in);

public:
    static int constexpr kSuccess = 0;
    static int constexpr kConstraintConflict = 1;
    static int constexpr kFieldNotFound = 2;
    static int constexpr kDataTypeWrong = 3;
};

#endif // TABLE_H
