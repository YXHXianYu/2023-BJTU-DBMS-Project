#include "table.h"
#include <algorithm>
Table::Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint*> constraints) {
    this->table_name = table_name;
    this->fields = fields;
    this->constraints = constraints;
    for(auto field:fields) {
        field_map[field.first] = field.second;
    }
}

std::string Table::GetTableName() const{
    return table_name; 
}

int Table::Insert(std::vector<std::pair<std::string, std::string>> record_in) {
    std::unordered_map<std::string, std::any> record;
    for(const auto& field : record_in) {
        if(!field_map.count(field.first)) {
            return kFieldNotFound;
        }
        if(field_map[field.first] == "int") {
            for(auto x : field.second) {
                if(x > '9' || x < '0') return kDataTypeWrong;
            }
            record[field.first] = std::any(std::stoi(field.second));
        }
        else if(field_map[field.first] == "float") {
            int sum_dot = 0;
            for (auto x : field.second) {
                if(x == '.') sum_dot++;
                if ((x > '9' || x < '0') && sum_dot>=2)
                    return kDataTypeWrong;
            }
            record[field.first] = std::any(std::stof(field.second));
        }
        else if (field_map[field.first] == "string") {
            record[field.first] = std::any(field.second);
        }
    }
    /*todo: 约束条件
    for(auto constraint:constraints) {
        if (dynamic_cast<const DefaultConstraint *>(constraint) != nullptr){
            if (!record.count(constraint->GetFieldName())) {
                record[constraint->GetFieldName()] = getField
            }
        }
        if(dynamic_cast<const NotNullConstraint *>(constraint) != nullptr) {//非空
            if(!record.count(constraint->GetFieldName())) {
                return kConstraintConflict;
            }
        }
        if (dynamic_cast<const UniqueConstraint *>(constraint) != nullptr){//唯一
            std::string field_name = constraint->GetFieldName();
            if(record.count(field_name)) {
                for(auto other_record : records) {
                    if (other_record.count(field_name)){
                        if (record[field_name] == other_record[field_name]){
                            return kConstraintConflict;
                        }
                    }
                }
            }
        }

    }
    */
    records.push_back(record);
    return kSuccess;
}

int Table::Select(std::vector<std::string> field_name, std::vector<std::tuple<std::string, std::string,int>> conditions, std::vector<std::vector<std::any>>& return_records)
{
    if(field_name[0] == "*") {
        field_name.clear();
        for(auto field : fields) {
            field_name.push_back(field.first);
        }
    }
    for(const auto& name: field_name) {
        if(!field_map.count(name)) {
            return kFieldNotFound;
        }
    }
    //return_records第一行全是字段名
    std::vector<std::any> tmp;
    for(const auto& name: field_name) {
        tmp.push_back(std::any(name));
    }
    return_records.push_back(tmp);
    //ok

    for(const auto& record: records) {
        //where ... continue
        if(CheckCondition(record, conditions) != kSuccess) continue;
        
        std::vector<std::any> ret_record;
        for(const auto& name: field_name) {
            if(!record.count(name)) {
                ret_record.push_back(std::any(ColasqlNull()));
            }
            else {
                ret_record.push_back(record.at(name));
            }
        }
        return_records.push_back(ret_record);
    }
    // TODO
    return kSuccess;
}

int Table::CheckCondition(const std::unordered_map<std::string, std::any>& record, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    for(const auto& condition : conditions) {
        std::string field_name = std::get<0>(condition);
        int expected_result = std::get<2>(condition);
        std::any to_any = ColasqlTool::GetAnyByTypeAndValue(field_map[field_name], std::get<1>(condition));
        
        if(!record.count(field_name)) {
            return kConditionsNotSatisfied;
        }
        
        int compare_result = ColasqlTool::CompareAny(record.at(field_name), to_any);
        /*std::cout<<field_name<<" "<<compare_result<<" "<<std::get<0>(condition)<<" "<<std::get<1>(condition)<<std::endl;*/
        if(compare_result == kEqual) {
            if (expected_result != kEqualConditon && expected_result != kLessEqualConditon && expected_result != kLargerEqualCondition)
                return kConditionsNotSatisfied;
        }
        if(compare_result == kLarger) {
            if(expected_result != kLargerConditon && expected_result != kLargerEqualCondition && expected_result != kNotEqualConditon) {
                return kConditionsNotSatisfied;
            }
        }
        if(compare_result == kLess) {
            if(expected_result != kLessCondition && expected_result != kLessEqualConditon && expected_result != kNotEqualConditon) {
                return kConditionsNotSatisfied;
            }
        }
    }
    return kSuccess;
}

int Table::Delete(std::vector<std::tuple<std::string, std::string, int>> conditions) {
    for (const auto &condition : conditions) {
        if(!field_map.count(std::get<0>(condition))) {
            return kFieldNotFound;
        }
    }
    std::vector<int> index_for_delete;
    for(int i = 0; i < records.size(); ++i) {
        std::unordered_map<std::string, std::any>& record = records[i];
        if(CheckCondition(record, conditions) == kSuccess) {
            index_for_delete.push_back(i);
        }
    }
    reverse(index_for_delete.begin(), index_for_delete.end());
    for(const auto& x : index_for_delete) {
        records.erase(records.begin() + x);
    }
    return kSuccess;
}
int Table::Update(const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions){
    for(const auto& change_field: values) {
        if(!field_map.count(change_field.first)) {
            return kFieldNotFound;
        }
    }
    for(auto& record: records) {
        if(CheckCondition(record,conditions) != kSuccess) {
            continue;
        } 
        for(const auto& field : values) {
            if(field_map[field.first] == "int") {
                for(auto x : field.second) {
                    if(x > '9' || x < '0') return kDataTypeWrong;
                }
                record[field.first] = std::any(std::stoi(field.second));
            }
            else if(field_map[field.first] == "float") {
                int sum_dot = 0;
                for (auto x : field.second) {
                    if(x == '.') sum_dot++;
                    if ((x > '9' || x < '0') && sum_dot>=2)
                        return kDataTypeWrong;
                }
                record[field.first] = std::any(std::stof(field.second));
            }
            else if (field_map[field.first] == "string") {
                record[field.first] = std::any(field.second);
            }
        }
    }
    return kSuccess;
}

int Table::DescribeTable(std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>& constraints) {
    fields = this->fields;
    constraints = this->constraints;
    return kSuccess;
}
int Table::AlterTableAdd(std::pair<std::string, std::string> new_field) {
    if(field_map.count(new_field.first)) return kFieldExisted;
    fields.push_back(new_field);
    field_map[new_field.first] = new_field.second;
    return kSuccess; 
}