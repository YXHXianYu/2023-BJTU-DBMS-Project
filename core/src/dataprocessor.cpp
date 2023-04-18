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
    users.push_back(user(user_name, user_password));
    return kSuccess;
}

int DataProcessor::Login(std::string user_name, std::string user_password) {
    for(auto& user : users) {
        if(user.GetUserName() == user_name) {
            if(user.Identify(user_password) == User::kIdentifiedSuccess) {
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
    databases.push_back(database(database_name, current_user->GetUserName()));
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
