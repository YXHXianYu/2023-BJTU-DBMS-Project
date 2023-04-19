#include"dataprocessor.h"

DataProcessor DataProcessor::GetInstance() {
    static DataProcessor instance;
    return instance;
}

DataProcessor::DataProcessor() {
    current_user = nullptr;
    current_database = nullptr;

    //to do: read files into data

}

int DataProcessor::CreateUser(std::string user_name, std::string user_password) {
    int user_name_exist = 0;
    for(auto user : users) {
        if(user.GetUserName() == user_name) {
            user_name_exist = 1;
        }
    }
    if(user_name_exist) {
        return kUserNameExisted;
    }
    users.push_back(User(user_name, user_password));
    return kSuccess;
}

int DataProcessor::Login(std::string user_name, std::string user_password) {
    for(auto& user : users) {
        if(user.GetUserName() == user_name) {
            if(user.Identify(user_password) == kSuccess) {
                current_user = &user;
                return kSuccess;
            }
            else {
                return kUserPasswordError;
            }
        }
    }
    return kUserNameNotFound;
}

int DataProcessor::CreateDatabase(std::string database_name) {
    if(current_user == nullptr) {
        return kUserNotLogin;
    }
    for(auto& database : databases) {
        if(database.GetDatabaseName() == database_name) {
            return kDatabaseExisted;
        }
    }
    databases.push_back(Database(database_name, current_user->GetUserName()));
    return kSuccess;
}

int DataProcessor::UseDatabase(std::string database_name) {
    if(current_user == nullptr) {
        return kUserNotLogin;
    }
    for(auto& database : databases) {
        if(database.GetDatabaseName() == database_name) {
            current_database = &database;
            return kSuccess;
        }
    }
    return kDatabaseNotFound;
}
int DataProcessor::CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints) {
    if(current_user == nullptr) {
        return kUserNotLogin;
    }
    if(current_database == nullptr) {
        return kDatabaseNotUse;
    }
    return current_database->CreateTable(table_name,fields, constraints);
}

int DataProcessor::DropTable(std::string table_name) {
    if(current_user == nullptr) {
        return kUserNotLogin;
    }
    if(current_database == nullptr) {
        return kDatabaseNotUse;
    }
    return current_database->DropTable(table_name);
}

int DataProcessor::DescribeTable(std::string table_name) {
    //todo
}

int DataProcessor::ShowTables(std::string table_name){
    //todo
}

int DataProcessor::Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in) {
    if (current_user == nullptr) {
        return kUserNotLogin;
    }
    if (current_database == nullptr) {
        return kDatabaseNotUse;
    }
    return current_database->Insert(table_name,record_in);
}
int DataProcessor::Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string, int>> conditions, std::vector<std::vector<std::any>> &return_records) {
    if (current_user == nullptr) {
        return kUserNotLogin;
    }
    if (current_database == nullptr) {
        return kDatabaseNotUse;
    }
    return current_database->Select(table_name,field_name,conditions,return_records);
}
int DataProcessor::Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions) {
    if (current_user == nullptr){
        return kUserNotLogin;
    }
    if (current_database == nullptr){
        return kDatabaseNotUse;
    }
    return current_database->Delete(table_name,conditions);
}
int DataProcessor::Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    if (current_user == nullptr){
        return kUserNotLogin;
    }
    if (current_database == nullptr){
        return kDatabaseNotUse;
    }
    return current_database->Update(table_name,values,conditions);
}