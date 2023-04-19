#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../core/include/database.h"
#include "../core/include/user.h"
#include "../core/include/constants.h"

class FileManager {
private:
    FileManager();
public :
    static FileManager& GetInstance();
    int ReadDatabases(std::vector<Database>& databases);
    int ReadUsers(std::vector<User>& users);
    int WriteDatabases(const std::vector<Database>& databases);
    int WriteUsers(const std::vector<User>& users);
};


#endif 