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

    // ----- ./data/database/* -----
    // *: tables.txt, table.txt, tableRecord.txt, tableConstraint.txt, tableIndex.txt
    int WriteTablesFile(const std::string& databaseName, const std::vector<Table>& tables);
    int ReadTablesFile(const std::string& databaseName, std::vector<Table>& tables);

    // ----- ./data/database/log.txt -----
    int WriteLogFile(const std::string& databaseName, const std::string& log);
    int ReadLogFile(const std::string& databaseName, std::string& log);
};


#endif 