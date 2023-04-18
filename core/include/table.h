#ifndef TABLE_H
#define TABLE_H

#include<iostream>
#include<vector>
#include<any>

class Table {
private:
    std::string table_name;
    std::vector<std::pair<std::string, std::any>> fields;
    std::vector<std::vector<std::any>> records;
    

private:
    Table();
public:
    Table(std::string table_name);
};

#endif // TABLE_H
