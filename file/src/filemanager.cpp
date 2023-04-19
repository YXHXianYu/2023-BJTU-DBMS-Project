#include "filemanager.h"

#include <fstream>
#include <direct.h>

FileManager::FileManager() {

}

FileManager& FileManager::GetInstance() {
    static FileManager instance;
    return instance;
}

int FileManager::ReadDatabases(std::vector<Database>& databases) {
    return -1;
}
int FileManager::ReadUsers(std::vector<User>& users) {
    return -1;
}
int FileManager::WriteDatabases(const std::vector<Database>& databases) {
    WriteDatabasesFile(databases);
    return -1;
}
int FileManager::WriteUsers(const std::vector<User>& users) {
    return -1;
}

// ----- ./data/ -----

int FileManager::WriteDatabasesFile(const std::vector<Database>& databases) {
    
    // 创建新文件夹
    mkdir("./data");

    // c++ ofstream 必须要目录存在才可以创建新文件夹
    std::ofstream out("./data/databases.txt", std::ofstream::out | std::ofstream::trunc);

    // out
    if(out.is_open()) {
        out << databases.size() << std::endl;
        for(const auto& database: databases) {
            out << database.GetDatabaseName() << " " << database.GetOwnerUserName() << std::endl;
        }
        out.close();
    }

    return 0;
}

int FileManager::ReadDatabasesFile(std::vector<Database>& databases) {

    // c++ ifstream
    std::ifstream in("./data/databases.txt", std::ifstream::in);

    // in
    databases.clear();
    if(in.is_open()) {
        int n;
        in >> n;
        for(int i = 1; i <= n; i++) {
            std::string databaseName;
            std::string ownerUser;
            in >> databaseName >> ownerUser;
            databases.push_back(Database(databaseName, ownerUser));
        }
    }

    return 0;
}

// ----- ./data/database/* -----
// *: tables.txt, table.txt, tableRecord.txt, tableConstraint.txt, tableIndex.txt
    
int FileManager::WriteTablesFile(const std::string& databaseName, const std::vector<Table>& tables) {

    std::string path = "./data/" + databaseName + "/";

    // mkdir
    mkdir(path.c_str());

    // tables.txt
    std::ofstream out(path + "tables.txt", std::ofstream::out | std::ofstream::trunc);

    if(out.is_open()) {
        out << tables.size() << std::endl;
        for(const auto& table: tables) {
            out << table.GetTableName() << std::endl;
        }
        out.close();
    }

    // each table
    for(const auto& table: tables) {
        out.open(path + table.GetTableName() + ".txt", std::ofstream::out | std::ofstream::trunc);

        


    }




    return 0;
}

int FileManager::ReadTablesFile(const std::string& databaseName, std::vector<Table>& tables) {

    // tables.txt
    std::ifstream in("./data/" + databaseName + "/tables.txt", std::ifstream::in);

    // in
    tables.clear();

    std::vector
    if(in.is_open()) {
        int n;
        in >> n;
        for(int i = 1; i <= n; i++) {
            std::string tableName;
            in >> tableName;
            tables.push_back((tableName))
        }
    }

    return -1;
}

// ----- ./data/database/log.txt -----

int FileManager::WriteLogFile(const std::string& databaseName, const std::string& log) {
    return -1;
}


int FileManager::ReadLogFile(const std::string& databaseName, std::string& log) {
    return -1;
}