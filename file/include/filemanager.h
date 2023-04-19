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

public :
    // ----- Write -----

    int WriteDatabasesFile(const std::vector<Database>& databases);

    // *: tables.txt, table.txt, tableRecords.txt, tableConstraints.txt, tableIndexes.txt
    int WriteTablesFile(const std::string& databaseName, const std::vector<Table>& tables); 
    
    int WriteLogFile(const std::string& databaseName, const std::string& log);

    // ----- Read -----

    int ReadDatabasesFile(std::vector<Database>& databases);

    int ReadTablesFile(const std::string& databaseName, std::vector<Table>& tables);

    int ReadLogFile(const std::string& databaseName, std::string& log);
};


#endif 