#include "dataprocessor.h"

DataProcessor& DataProcessor::GetInstance() {
	static DataProcessor instance;
	return instance;
}



DataProcessor::DataProcessor() {
	current_user = nullptr;
	current_database = nullptr;

	// read files into data
	Read(false);
}

int DataProcessor::GetCurrentDatabase(std::string& database_name) {
	if(current_database == nullptr) return kDatabaseNotUse;
	database_name = current_database->GetDatabaseName();
	return kSuccess;
}

int DataProcessor::CreateUser(std::string user_name,
								std::string user_password) {
	for (auto& user : users) {
		if (user.GetUserName() == user_name) {
		return kUserNameExisted;
		}
	}
	if (current_user != nullptr) {
		users.push_back(User(user_name, user_password));
		for (auto& user : users) {
			if (user.GetUserName() == current_user_name) {
				current_user = &user;
				return kSuccess;
			}
		}
		return kErrorCurrentUser;
	}
	users.push_back(User(user_name, user_password));

	return kSuccess;
}

int DataProcessor::Login(std::string user_name, std::string user_password) {
	for (auto& user : users) {
		if (user.GetUserName() == user_name) {
		if (user.Identify(user_password) == kSuccess) {
			current_user = &user;
			current_user_name = user_name;
			return kSuccess;
		} else {
			return kUserPasswordError;
		}
		}
	}
	return kUserNameNotFound;
}

int DataProcessor::GrantAuthority(std::string user_name, std::string database_name, std::string table_name, std::string authority_name) {
	if(current_user == nullptr) return kUserNotLogin;
	//todo:
	authority_number number;
	if(authority_name == "select") number = authority_number::SELECT;
	if(authority_name == "delete") number = authority_number::DELETE;
	if(authority_name == "insert") number = authority_number::INSERT;
	if(authority_name == "update") number = authority_number::UPDATE;
	if(authority_name == "index") number = authority_number::INDEX;
	if(authority_name == "alter") number = authority_number::ALTER;


	if(current_user->CheckAuthority(database_name, table_name, number) != kSuccess) {
		return kInsufficientAuthority;
	}
	for(auto& user:users) {
		if(user.GetUserName() == user_name) {
			if(user.CheckAuthority(database_name, table_name, number) != kSuccess) user.GrantAuthority(database_name,table_name,number);
			return kSuccess;
		}
	}
	return kUserNameNotFound;
}
int DataProcessor::GrantAuthority(std::string user_name, std::string database_name, std::string authority_name) {
	GrantAuthority(database_name,"",authority_name);
}
int RevokeAuthority(std::string database_name, std::string table_name, std::string authority_name) {
	
}
int RevokeAuthority(std::string database_name, std::string authority_name) {

}

int DataProcessor::ShowDatabases(std::vector<std::string>& return_databases) {
	if(current_user == nullptr) {
	    return kUserNotLogin;
	}
	return_databases.clear();
	for (const auto& database : databases) {
		if(current_user->CheckDatabaseInAuthorities(database.GetDatabaseName()) == kSuccess){
			return_databases.push_back(database.GetDatabaseName());
		}
	}
	return kSuccess;
}

int DataProcessor::CreateDatabase(std::string database_name) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	for (auto& database : databases) {
		if (database.GetDatabaseName() == database_name) {
		return kDatabaseExisted;
		}
	}
	if (current_database != nullptr) {
		databases.push_back(Database(database_name, current_user->GetUserName()));
		for (auto& database : databases) {
			if (database.GetDatabaseName() == current_database_name) {
				current_database = &database;
				current_user->GrantAllDatabaseAuthorities(database_name);
				return kSuccess;
			}
		}
		return kErrorCurrentDatabase;
	}
	current_user->GrantAuthority(database_name,authority_number::CREATE);
	current_user->GrantAuthority(database_name,authority_number::DROP);
	databases.push_back(Database(database_name, current_user->GetUserName()));
	return kSuccess;
}

int DataProcessor::DeleteDatabase(std::string database_name) {
	if(current_user == nullptr) {
		return kUserNotLogin;
	}
	
	for(int i = 0; i < databases.size(); ++i) {
		auto& database = databases[i]; 
		if(database.GetDatabaseName() == database_name) {
			if(current_user->CheckAuthority(database_name, authority_number::DROP) == kSuccess) {
				if(current_database != nullptr) {
					if(current_database_name == database_name) {
						current_database = nullptr;
						current_database_name = "";
					}
				}
				current_database = nullptr;
				databases.erase(databases.begin() + i);
				int get_database_flag = 0;
				if(current_database_name == "") get_database_flag = 1;
				for (auto& database : databases) {
					if (database.GetDatabaseName() == current_database_name) {
						current_database = &database;
						get_database_flag = 1;
						break;
					}
				}
				if(get_database_flag ==  1) {
					for(auto& user:users) {
						user.RevokeAllDatabaseAndTableAuthorities(database_name);
					}
					return kSuccess;
				}
				return kErrorCurrentDatabase;
			}
			return kInsufficientAuthority;
		}
	}
	return kDatabaseNotFound;
}

int DataProcessor::UseDatabase(std::string database_name) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (database_name == "") {
		current_database = nullptr;
		current_database_name = "";
		return kSuccess;
	}
	for (auto& database : databases) {
		if (database.GetDatabaseName() == database_name) {
			if(current_user->CheckDatabaseInAuthorities(database.GetDatabaseName())) {
				current_database = &database;
				current_database_name = database_name;
				return kSuccess;
			}
			return kInsufficientAuthority;
		}
	}
	return kDatabaseNotFound;
}

int DataProcessor::CreateTable(
		std::string table_name,
		std::vector<std::pair<std::string, std::string>> fields,
		std::vector<Constraint*> constraints) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_user->CheckAuthority(current_database_name, authority_number::CREATE)) {
		int ret = current_database->CreateTable(table_name, fields, constraints);
		if(ret == kSuccess) {
			UpdatePointer();
			current_user->GrantAllTableAuthorities(current_database_name, table_name);
			UpdatePointer();
		}
		return ret;
	}
	return kInsufficientAuthority;
}

int DataProcessor::DropTable(std::string table_name) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_user->CheckAuthority(current_database_name, authority_number::DROP)) {
		int w = current_database->DropTable(table_name);
		if(w == kSuccess) {
			UpdatePointer();
			for(auto& user:users) {
				user.RevokeAllTableAuthorities(current_database_name,table_name);
			}
		}
		return w;
	}
	return kInsufficientAuthority;
}

int DataProcessor::DescribeTable(
		std::string table_name,
		std::vector<std::pair<std::string, std::string>>& fields,
		std::vector<Constraint*>& constraints) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_user->CheckTableInAuthorities(current_database_name, table_name) != kSuccess) return kInsufficientAuthority;
	fields.clear();
	constraints.clear();
	int ret = current_database->DescribeTable(table_name, fields, constraints);
	return ret;
}

int DataProcessor::ShowTables(std::vector<std::string>& return_tables) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	return_tables.clear();
	int ret = current_database->ShowTables(return_tables);
	if(ret == kSuccess) {
		for(auto it = return_tables.begin(); it != return_tables.end();) {
			if(current_user->CheckTableInAuthorities(current_database_name,(*it)) != kSuccess) {
				return_tables.erase(it);
			}
			else it++;
		}
	}
	return current_database->ShowTables(return_tables);
}

int DataProcessor::AlterTableAdd(std::string table_name,
									std::pair<std::string, std::string> field) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name, authority_number::ALTER) != kSuccess) return kInsufficientAuthority;
	int ret = current_database->AlterTableAdd(table_name, field);
	UpdatePointer();
	return ret;
}

int DataProcessor::AlterTableDrop(std::string table_name,
									std::string field_name) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name, authority_number::ALTER) != kSuccess) return kInsufficientAuthority;
	int ret = current_database->AlterTableDrop(table_name, field_name);
	UpdatePointer();
	return ret;
}

int DataProcessor::AlterTableModify(std::string table_name,
										std::pair<std::string, std::string> field) {
	// todo
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name, authority_number::ALTER) != kSuccess) return kInsufficientAuthority;
	int ret = current_database->AlterTableModify(table_name, field);
	UpdatePointer();
	return ret;
}

int DataProcessor::Insert(
		std::string table_name,
		std::vector<std::pair<std::string, std::string>> record_in) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name,authority_number::INSERT) != kSuccess) return kInsufficientAuthority;

	int ret =  current_database->Insert(table_name, record_in);
	UpdatePointer();
	return ret;
}

int DataProcessor::Select(
		std::string table_name, std::vector<std::string> field_name,
		std::vector<std::tuple<std::string, std::string, int>> conditions,
		std::vector<std::vector<std::any>>& return_records) {
	return_records.clear();
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name,authority_number::SELECT) != kSuccess) return kInsufficientAuthority;
	int ret = current_database->Select(table_name, field_name, conditions,
									return_records);
	UpdatePointer();
	return ret;
}
int DataProcessor::Delete(
		std::string table_name,
		std::vector<std::tuple<std::string, std::string, int>> conditions) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name,authority_number::DELETE) != kSuccess) return kInsufficientAuthority;
	int ret = current_database->Delete(table_name, conditions);
	UpdatePointer();
	return ret;
}

int DataProcessor::Update(
		std::string table_name,
		const std::vector<std::pair<std::string, std::string>>& values,
		const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
	if (current_user == nullptr) {
		return kUserNotLogin;
	}
	if (current_database == nullptr) {
		return kDatabaseNotUse;
	}
	if(current_database->FindTable(table_name) != kSuccess) {
		return kTableNotFound;
	} 
	if(current_user->CheckAuthority(current_database_name,table_name,authority_number::UPDATE) != kSuccess) return kInsufficientAuthority;
	
	int ret = current_database->Update(table_name, values, conditions);
	UpdatePointer();
	return ret;
}

int DataProcessor::Read(bool debug) {
	FileManager::GetInstance().ReadUsersFile(users);
	FileManager::GetInstance().ReadDatabasesFile(databases);

	if (debug) {
		std::cout << databases.size() << std::endl;
	}

	for (auto& database : databases) {
		std::vector<Table> tables;
		FileManager::GetInstance().ReadTablesFile(database.GetDatabaseName(),
												tables);
		database.SetTables(tables);

		if (debug) {
		std::cout << " - " << tables.size() << std::endl;
		for (const auto& table : tables) {
			std::cout << " - - " << table.GetTableName() << std::endl;

			std::cout << " - - - ";
			for (const auto& field : table.GetFields()) {
			std::cout << "(" << field.first << ", " << field.second << ") ";
			}
			std::cout << std::endl;

			std::cout << " - - - ";
			for (const auto& record : table.GetRecords()) {
			std::cout << "[";
			for (const auto& [name, value] : record) {
				std::cout << name << ": " << ColasqlTool::AnyToString(value) << " ";
			}
			std::cout << "]    ";
			}
			std::cout << std::endl;

			std::cout << std::endl;
		}
		}
	}

	return 0;
}

int DataProcessor::Write() {
	FileManager::GetInstance().WriteUsersFile(users);
	FileManager::GetInstance().WriteDatabasesFile(databases);
	for (const auto& database : databases) {
		FileManager::GetInstance().WriteTablesFile(database.GetDatabaseName(),
												database.GetTables());
	}

	return 0;
}

int DataProcessor::UpdatePointer() {
	for(auto& database : databases) {
		if(database.GetDatabaseName() == current_database_name) {
			current_database = &database;
			return kSuccess;
		}
	}
	std::cout<<"IMPOSSIBLE SITUATION!!!"<<std::endl;
	return kErrorCurrentDatabase;
}