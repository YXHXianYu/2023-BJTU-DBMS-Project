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

    std::cout << "open state: " << out.is_open() << std::endl;

    out << databases.size() << std::endl;
    for(const auto& database: databases) {
        out << database.GetDatabaseName() << " " << database.GetOwnerUserName() << std::endl;
    }

    out.close();

    return 0;
}

int FileManager::ReadDatabasesFile(std::vector<Database>& databases) {
    return -1;
}

// ----- ./data/database/ -----
int FileManager::WriteTablesFile(const std::vector<Table>& tables) {
    return -1;
}
int FileManager::WriteLogFile(const std::string& log) {
    return -1;
}

int FileManager::ReadTablesFile(std::vector<Table>& tables) {
    return -1;
}
int FileManager::ReadLogFile(std::string& log) {
    return -1;
}

// ----- ./data/database/ -----
int FileManager::WriteTableFile(const Table& table) {
    return -1;
}
int FileManager::WriteRecordFile(const Table& table) {
    return -1;
}
int FileManager::WriteConstraintFile(const Table& table) {
    return -1;
}
int FileManager::WriteIndexFile(const Table& table) {
    return -1;
}

int FileManager::ReadTableFile(Table& table) {
    return -1;
}
int FileManager::ReadRecordFile(Table& table) {
    return -1;
}
int FileManager::ReadConstraintFile(Table& table) {
    return -1;
}
int FileManager::ReadIndexFile(Table& table) {
    return -1;
}