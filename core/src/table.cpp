#include "table.h"
Table::Table(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint*> constraints) {
    this->table_name = table_name;
    this->fields = fields;
    this->constraints = constraints;
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
            for (auto x : field.second) {
                if ((x > '9' || x < '0') && x != '.')
                    return kDataTypeWrong;
            }
            record[field.first] = std::any(std::stof(field.second));
        }
        else if (field_map[field.first] == "string") {
            record[field.first] = field.second;
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

}