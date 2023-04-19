#include "filemanager.h"

FileManager& FileManager::GetInstance() {
    static FileManager instance;
    return instance;
}

int FileManager::ReadDatabases(std::vector<Database>& databases) {
    
}
int FileManager::ReadUsers(std::vector<User>& users) {

}
int FileManager::WriteDatabases(const std::vector<Database>& databases) {

}
int FileManager::WriteUsers(const std::vector<User>& users) {

}