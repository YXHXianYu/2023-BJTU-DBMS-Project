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
    
    const std::string& GetOwnerUserName() const;
    const std::string& GetDatabaseName() const;
    const std::vector<Table>& GetTables() const;
    void SetTables(const std::vector<Table>&);

    int ShowTables(std::vector<std::string>& return_tables);
    int CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);
    int DropTable(std::string table_name);
    int Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in);
    
    int Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string,std::string,int>> conditions, std::vector<std::vector<std::any>> &return_records);

    int Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions);

    int Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions);

    int DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>& constraints);
    int AlterTableAdd(std::string table_name, std::pair<std::string, std::string> field);
    int AlterTableDrop(std::string table_name, std::string field_name);
};


#endif // DATABASE_H