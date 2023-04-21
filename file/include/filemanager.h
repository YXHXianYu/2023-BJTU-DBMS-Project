#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

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

    int WriteTablesFile(const std::string& databaseName, const std::vector<Table>& tables); 
    
    int WriteUsersFile(const std::vector<User>& users);

    int WriteLogFile(const std::string& databaseName, const std::string& log);

    // ----- Read -----

    int ReadDatabasesFile(std::vector<Database>& databases);

    int ReadTablesFile(const std::string& databaseName, std::vector<Table>& tables);

    int ReadUsersFile(std::vector<User>& users);

    int ReadLogFile(const std::string& databaseName, std::string& log);
};


#endif 