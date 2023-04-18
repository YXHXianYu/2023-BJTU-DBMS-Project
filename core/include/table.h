#ifndef TABLE_H
#define TABLE_H

#include<iostream>
#include<vector>
#include<any>
#include<unordered_map>
class Table {
private:
    std::string table_name;
    std::vector<std::pair<std::string, std::string>> fields; //pair<field_name, field_type>
    std::vector< std::unordered_map<std::string, std::any> > records;
    

private:
    Table();
public:
    Table(std::string table_name);
    Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields);
    std::vector<std::vector<std::any>> Select(std::vector<std::string>);
    int Insert(std::vector<std::string, std::any>);
    
    
};

#endif // TABLE_H
