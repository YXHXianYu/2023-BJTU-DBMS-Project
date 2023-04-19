#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "database.h"
#include "user.h"
#include "constants.h"

class FileManager {
private:
    FileManager();

public :
    static FileManager& GetInstance();

    int ReadDatabases(std::vector<Database>& databases);
    int ReadUsers(std::vector<User>& users);
    int WriteDatabases(const std::vector<Database>& databases);
    int WriteUsers(const std::vector<User>& users);

private:
    // ----- ./data/ -----
    int WriteDatabasesFile(const std::vector<Database>& databases);

    int ReadDatabasesFile(std::vector<Database>& databases);

    // ----- ./data/database/ -----
    int WriteTablesFile(const std::vector<Table>& tables);
    int WriteLogFile(const std::string& log);

    int ReadTablesFile(std::vector<Table>& tables);
    int ReadLogFile(std::string& log);

    // ----- ./data/database/ -----
    int WriteTableFile(const Table& table);
    int WriteRecordFile(const Table& table);
    int WriteConstraintFile(const Table& table);
    int WriteIndexFile(const Table& table);

    int ReadTableFile(Table& table);
    int ReadRecordFile(Table& table);
    int ReadConstraintFile(Table& table);
    int ReadIndexFile(Table& table);
};


#endif 