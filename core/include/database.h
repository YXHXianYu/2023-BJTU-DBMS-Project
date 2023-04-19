#ifndef DATABASE_H
#define DATABASE_H

#include<iostream>
#include<vector>
#include"table.h"
#include"constants.h"
class Database {
private:
    std::string owner_user;
    std::string database_name;
    std::vector<Table> tables;
private:
    Database();
public:
    Database(std::string database_name, std::string owner_user);
    std::string GetOwnerUserName();
    std::string GetDatabaseName();
    int CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);
    int DropTable(std::string table_name);
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in);

    int Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string,std::string,int>> conditions, std::vector<std::vector<std::any>> &return_records);

    int Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions);

    int Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
};


#endif // DATABASE_H