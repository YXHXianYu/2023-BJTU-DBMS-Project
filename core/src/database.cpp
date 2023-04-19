#include "database.h"

Database::Database() {

}

Database::Database(std::string database_name, std::string owner_user) {
    this->database_name = database_name;
    this->owner_user = owner_user;
}

const std::string& Database::GetOwnerUserName() const {
    return owner_user;
}

const std::string& Database::GetDatabaseName() const {
    return database_name;
}

const std::vector<Table>& Database::GetTables() const {
    return tables;
}

void Database::SetTables(const std::vector<Table>& tables) {
    this->tables = tables;
}

int Database::CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return kTableNameExisted;
        }
    }
    tables.push_back(Table(table_name, fields, constraints));
    return kSuccess;
}

int Database::DropTable(std::string table_name) {
    for(int i = 0; i < tables.size(); ++i) {
        if(tables[i].GetTableName() == table_name) {
            tables.erase(tables.begin() + i);
            return kSuccess;
        }
    }
    return kTableNotFound;
}

int Database::Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Insert(record_in);
        }
    }
    return kTableNotFound;
}

int Database::Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Delete(conditions);
        }
    }
    return kTableNotFound;
}

int Database::Select(std::string table_name, std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string, int>> conditions, std::vector<std::vector<std::any>> &return_records) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Select(field_name, conditions, return_records);
        }
    }
    return kTableNotFound;
}
int Database::Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.Update(values, conditions);
        }
    }
    return kTableNotFound;
}

int Database::DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>& constraints) {
    for(auto& table: tables) {
        if(table.GetTableName() == table_name) {
            return table.DescribeTable(fields, constraints);

        }
    }
    return kTableNotFound;
}

int Database::AlterTableAdd(std::string table_name, std::pair<std::string, std::string> field) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.AlterTableAdd(field);
        }
    }
    return kTableNotFound;
}

int Database::AlterTableDrop(std::string table_name, std::string field_name) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.AlterTableDrop(field_name);
        }
    }
    return kTableNotFound;
}

int Database::ShowTables(std::vector<std::string>& return_tables) {
    for(const auto& table:tables) {
        return_tables.push_back(table.GetTableName());
    }
    return kSuccess;
}