#include "database.h"
#include<assert.h>
#include <functional>
#include<map>
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
int Database::FindTable(std::string table_name) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) return kSuccess;
    }
    return kTableNotFound;
}

Table& Database::FindTableReference(std::string table_name) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) return table;
    }
    assert(1);
    //impossible situation
        return tables[0];
}

int Database::FindTable(std::string table_name, Table& return_table) {
    for(const auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return_table = table;
            return kSuccess;
        }
    }
    return kTableNotFound;
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
int Database::AlterTableConstraint(std::string table_name, Constraint* constraint) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr) {
                ForeignReferedConstraint* refered_constraint = new ForeignReferedConstraint(dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceFieldName(),table_name,constraint->GetFieldName());
                int ret = AlterTableConstraint(dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName(),refered_constraint);
                if(ret!=kSuccess) return ret;
            }
            return table.AlterTableConstraint(constraint);
        }
    }
    
    return kTableNotFound;
}

int Database::DropTable(std::string table_name) {
    for(int i = 0; i < tables.size(); ++i) {
        if(tables[i].GetTableName() == table_name) {

            //check constraint
            auto constraints = tables[i].GetConstraints();
            for(const auto& constraint : constraints) {
                if(dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr){return kBeingRefered;}
            }
            
            //删除所有被此表参考的约束
            for(const auto& constraint : constraints) {
                if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr){
                    std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName();
                    std::string tmp;
                    FindTableReference(reference_table_name).DropForeignReferedConstraint(table_name);
                }
            }
            tables.erase(tables.begin() + i);
            return kSuccess;
        }
    }
    return kTableNotFound;
}

int Database::Insert(std::string table_name, std::vector<std::pair<std::string, std::string>> record_in) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Insert(record_in, this);
        }
    }
    return kTableNotFound;
}

int Database::Delete(std::string table_name, std::vector<std::tuple<std::string, std::string, int>> conditions) {
    for(auto& table : tables) {
        if(table.GetTableName() == table_name) {
            return table.Delete(conditions, this);
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
int Database::Select(std::vector<std::string> table_names, std::vector<std::string> field_names,std::vector<std::tuple<std::string, std::string, int>> conditions,std::vector<std::vector<std::any>>& return_records) {
    std::vector<std::vector<std::vector<std::any>>> td_records;
    std::vector<std::tuple<std::string, std::string, int>> empty_condition;
    std::vector<std::string> all_field_names;
    all_field_names.push_back("*");
    for(const auto& table_name: table_names) {
        for(auto & table: tables) {
            if(table.GetTableName() == table_name) {
                return_records.clear();
                table.Select(all_field_names, empty_condition, return_records);
                td_records.push_back(return_records);
            }
        }
    }
    int sz = td_records.size();
    //std::cout<<"sz = "<<sz<<std::endl;
    return_records.clear();

    all_field_names.clear();
    //return_records第一行全是字段名
    std::vector<std::any> tmp;
    if(field_names[0] == "*") {
        field_names.clear();
        std::map<std::string, int> mp;
        for(const auto& inner_records: td_records) {
            for(const auto& name : inner_records[0]) {
                if(!mp.count(ColasqlTool::AnyToString(name)))tmp.push_back(name);
                mp[ColasqlTool::AnyToString(name)] = 1;
            }
        }
        for(const auto& x : tmp) {
            field_names.push_back(ColasqlTool::AnyToString(x));
        }
    }
    else {
        std::map<std::string, int> mp;
        for(const auto& inner_records: td_records) {
            for(const auto& name : inner_records[0]) {
                mp[ColasqlTool::AnyToString(name)] = 1;
            }
        }
        for(const auto& name: field_names) {
            if(!mp.count(name)) return kFieldNotFound;
            tmp.push_back(std::any(name));
        }
    }
    return_records.push_back(tmp);
    const auto& get_return_records = [&]() {
        const auto& dfs = [&](auto&& self, int now, std::unordered_map<std::string, std::any> record) {
            //std::cout<<"check now and sz: "<<now<<" "<<sz<<std::endl;
            if(now == sz) {
                std::vector<std::any> return_record;
                for(const auto& field_name: field_names) {
                    //std::cout<<"check dfs: "<<field_name<<" ";
                    if(!record.count(field_name)) return_record.push_back(std::any(ColasqlNull()));
                    else return_record.push_back(record.at(field_name));
                }
                //std::cout<<std::endl;
                return_records.push_back(return_record);
                return;
            }
            const auto& inner_records = td_records[now];
            //std::cout<<"checksize: "<<now<<" "<<td_records[now].size()<<std::endl;
            std::unordered_map<std::string, std::any> new_record;
            for(int i = 1; i < inner_records.size(); ++i) {
                const auto& inner_record = inner_records[i];
                new_record = record;
                bool flag = 1;
                for(int j = 0; j < inner_record.size(); ++j){
                    if(new_record.count(ColasqlTool::AnyToString(inner_records[0][j])) && ColasqlTool::CompareAny(inner_record[j], new_record.at(ColasqlTool::AnyToString(inner_records[0][j])))!= kEqual){
                        flag = 0;
                        break;
                    }
                    new_record[ColasqlTool::AnyToString(inner_records[0][j])] = inner_record[j];
                }
                if(flag == 0) continue;
                //std::cout<<now<<" "<<i<<" "<<inner_records.size()<<std::endl;
                self(self, now + 1, new_record);
            }
        };
        std::unordered_map<std::string, std::any> initial_record;
        dfs(dfs, 0, initial_record);
    };

    get_return_records();
    return kSuccess;
    //dfs(0, sz, return_records);
}

int Database::Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    for(auto& table:tables) {
        if(table.GetTableName() == table_name) {
            return table.Update(values, conditions, this);
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
            int ret= table.AlterTableDrop(field_name, this);
            
            return ret;
        }
    }
    return kTableNotFound;
}

int Database::AlterTableModify(std::string table_name, std::pair<std::string, std::string> field){
    for(auto& table :tables) {
        if(table.GetTableName() == table_name) {
            int ret = table.AlterTableModify(field);
            //std::cout<<"ret is "<<ret<<std::endl;
            return ret;
        }
    }
    return kTableNotFound;
};

int Database::ShowTables(std::vector<std::string>& return_tables) {
    for(const auto& table:tables) {
        return_tables.push_back(table.GetTableName());
    }
    return kSuccess;
}

int Database::BuildIndex(std::string table_name, const std::vector<std::string>& compare_key) {
    for(auto& table: tables) {
        if(table.GetTableName() != table_name) continue;
        const auto& fmap = table.GetFieldMap();
        for(const auto& key: compare_key) {
            if(fmap.count(key) == 0) {
                return kFieldNotFound;
            }
        }
        return table.BuildIndex(compare_key);
    }
    return kTableNotFound;

}

