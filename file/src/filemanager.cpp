#include "filemanager.h"

#include <cassert>
#include <fstream>
#include <direct.h>

#include "colasqltool.h"

FileManager::FileManager() {

}

FileManager& FileManager::GetInstance() {
    static FileManager instance;
    return instance;
}

// ----- Write -----

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

// *: tables.txt, table.txt, tableRecord.txt, tableConstraint.txt, tableIndex.txt  
int FileManager::WriteTablesFile(const std::string& databaseName, const std::vector<Table>& tables) {

    std::string path = "./data/" + databaseName + "/";

    // mkdir
    mkdir(path.c_str());

    // tables.txt
    std::ofstream out(path + "tables.txt", std::ofstream::out | std::ofstream::trunc);
    if(!out.is_open()) assert(false);

    out << tables.size() << std::endl;
    for(const auto& table: tables) {
        out << table.GetTableName() << std::endl;
    }
    out.close();

    // each table
    for(const auto& table: tables) {

        // table.txt
        out.open(path + table.GetTableName() + ".txt", std::ofstream::out | std::ofstream::trunc);
        if(!out.is_open()) assert(false);

        const std::vector<std::pair<std::string, std::string>>& fields = table.GetFields();
        out << fields.size() << std::endl;
        for(const auto& [name, type]: fields) {
            out << name << " " << type << std::endl;
        }
        out.close();

        // tableRecords.txt
        out.open(path + table.GetTableName() + "Records.txt", std::ofstream::out | std::ofstream::trunc);
        if(!out.is_open()) assert(false);

        const std::vector<std::unordered_map<std::string, std::any>>& records = table.GetRecords();
        out << records.size() << std::endl;
        for(const auto& record: records) {
            out << record.size() << " ";
            for(const auto& [name, value]: record) {
                out << name << " " << ColasqlTool::AnyToString(value) << " ";
            }
            out << std::endl;
        }
        out.close();

        // tableConstraints.txt
        out.open(path + table.GetTableName() + "Constraints.txt", std::ofstream::out | std::ofstream::trunc);
        if(!out.is_open()) assert(false);

        // TODO
        out.close();


        // tableIndexes.txt
        out.open(path + table.GetTableName() + "Indexes.txt", std::ofstream::out | std::ofstream::trunc);
        if(!out.is_open()) assert(false);

        // TODO;
        out.close();

    }

    return 0;
}


int FileManager::WriteUsersFile(const std::vector<User>& users) {
    // 创建新文件夹
    mkdir("./data");

    // c++ ofstream 必须要目录存在才可以创建新文件夹
    std::ofstream out("./data/users.txt", std::ofstream::out | std::ofstream::trunc);

    // out
    if(out.is_open()) {
        out << users.size() << std::endl;
        for(const auto& user: users) {
            out << user.GetUserName() << " " << user.GetUserPassword() << " " << int(user.GetAuthority()) << std::endl;
        }
        out.close();
    }

    return 0;
}


int FileManager::WriteLogFile(const std::string& databaseName, const std::string& log) {
    return -1;
}

// ----- Read -----

int FileManager::ReadDatabasesFile(std::vector<Database>& databases) {
    // c++ ifstream
    std::ifstream in("./data/databases.txt", std::ifstream::in);

    // in
    databases.clear();

    if(!in.is_open()) return -1;

    int n;
    in >> n;
    for(int i = 1; i <= n; i++) {
        std::string databaseName;
        std::string ownerUser;
        in >> databaseName >> ownerUser;
        databases.push_back(Database(databaseName, ownerUser));
    }

    return 0;
}

int FileManager::ReadTablesFile(const std::string& databaseName, std::vector<Table>& tables) {

    std::string path = "./data/" + databaseName + "/";

    tables.clear();

    std::vector<std::string> tableNames;
    int n, m;

    // tables.txt
    std::ifstream in(path + "tables.txt", std::ifstream::in);
    if(!in.is_open()) assert(false);

    in >> n;
    for(int i = 1; i <= n; i++) {
        std::string tableName;
        in >> tableName;
        tableNames.push_back(tableName);
    }
    in.close();

    // each table
    for(const auto& tableName: tableNames) {
        std::vector<std::pair<std::string, std::string>> fields;
        std::unordered_map<std::string, std::string> field_map;
        std::vector<std::unordered_map<std::string, std::any>> records;
        std::vector<Constraint*> constraints;
        // std::vector .... indexes;

        // table.txt
        in.open(path + tableName + ".txt", std::ifstream::in);
        if(!in.is_open()) assert(false);

        in >> n;
        for(int i = 1; i <= n; i++) {
            std::string name;
            std::string type;
            in >> name >> type;
            fields.push_back({name, type});
            field_map[name] = type;
        }
        in.close();

        // tableRecords.txt
        in.open(path + tableName + "Records.txt", std::ifstream::in);
        if(!in.is_open()) assert(false);

        in >> n;
        for(int i = 1; i <= n; i++) {
            std::unordered_map<std::string, std::any> record;
            std::string name;
            std::string value;
            in >> m;
            for(int j = 1; j <= m; j++) {
                in >> name >> value;
                record[name] = ColasqlTool::GetAnyByTypeAndValue(field_map[name], value);
            }
            records.push_back(record);
        }
        in.close();

        // tableConstraints.txt
        in.open(path + tableName + "Records.txt", std::ifstream::in);
        if(!in.is_open()) assert(false);

        // TODO

        // tableIndexes.txt
        in.open(path + tableName + "Indexes.txt", std::ifstream::in);
        if(!in.is_open()) assert(false);
        
        // TODO

        // construct table
        Table table(tableName, fields, constraints, records);
        tables.push_back(table);
    }

    return 0;
}


int FileManager::ReadUsersFile(std::vector<User>& users) {
    // c++ ifstream
    std::ifstream in("./data/users.txt", std::ifstream::in);

    // in
    users.clear();

    if(!in.is_open()) return -1;

    int n;
    in >> n;
    for(int i = 1; i <= n; i++) {
        std::string name;
        std::string password;
        int x;
        in >> name >> password >> x;
        users.push_back(User(name, password, Authority(x)));
    }

    return 0;
}

int FileManager::ReadLogFile(const std::string& databaseName, std::string& log) {
    return -1;
}

