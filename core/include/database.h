#ifndef DATABASE_H
#define DATABASE_H

#include<iostream>
#include<vector>
#include"table.h"

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
    int CreateTable(std::string table_name);
    int DropTable(std::string table_name);
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::any>>);
    int Delete(std::string table_name);
    std::vector<std::vector<std::any>> Select(std::string table_name, std::vector<std::string>);
};


#endif // DATABASE_H