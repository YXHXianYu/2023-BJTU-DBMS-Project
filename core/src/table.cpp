#include "table.h"
#include "database.h"
#include <algorithm>
#include <cassert>

#include "index/fhqtreapindex.h"

Table::Table(std::string &table_name) {
    this->table_name = table_name;
    index_ptr = nullptr;
}

Table::Table(const std::string& table_name,
             const std::vector<std::pair<std::string, std::string>>& fields,
             const std::vector<Constraint*>& constraints_) {
    this->table_name = table_name;
    this->fields = fields;
    this->constraints = constraints_;
    for(auto field:fields) {
        field_map[field.first] = field.second;
    }
    index_ptr = nullptr;
}

Table::Table(const std::string& table_name,
             const std::vector<std::pair<std::string, std::string>>& fields,
             const std::vector<Constraint*>& constraints_,
             const std::vector<std::unordered_map<std::string, std::any>>& records) {
    this->table_name = table_name;
    this->fields = fields;
    this->constraints = constraints_;
    this->records = records;
    for(auto field:fields) {
        field_map[field.first] = field.second;
    }
    index_ptr = nullptr;
}


Table::~Table() {                           // 根据零三五法则，需要同时定义析构、拷贝构造、拷贝赋值函数
    // delete index_ptr;
    index_ptr = nullptr;
}

Table::Table(const Table& p): table_name(p.table_name), fields(p.fields), constraints(p.constraints), records(p.records) {
    for(auto& field: fields) {
        field_map[field.first] = field.second;
    }
    index_ptr = p.index_ptr;
}

Table& Table::operator = (Table other) {   // unique_ptr 需要定义 拷贝赋值函数
    swap(*this, other);
    return *this;
}

void swap(Table& s1, Table& s2) {
    using std::swap;
    swap(s1.table_name, s2.table_name);
    swap(s1.fields, s2.fields);
    swap(s1.field_map, s2.field_map);
    swap(s1.records, s2.records);
    swap(s1.constraints, s2.constraints);
    swap(s1.index_ptr, s2.index_ptr);
}



const std::string& Table::GetTableName() const {
    return table_name;
}

int Table::FindField(std::string field_name) const {
    if(field_map.count(field_name)) return kSuccess;
    return kFieldNotFound;
}

const std::vector<std::pair<std::string, std::string>>& Table::GetFields() const {
    return fields;
}

const std::unordered_map<std::string, std::string> Table::GetFieldMap() const {
    return field_map;
}

const std::vector<std::unordered_map<std::string, std::any> >& Table::GetRecords() const {
    return records;
}

const std::vector<Constraint*>& Table::GetConstraints() const {
    return constraints;
}

int Table::GetIndex(std::vector<std::string>& result_key) const {
    if(index_ptr == nullptr || index_ptr->getState() != 0) return kFailedIndexNotBuild;
    result_key = index_ptr->getCompareKey();
    return kSuccess;
}

int Table::DropForeignReferedConstraint(std::string table_name) {
    for(auto it = constraints.begin(); it != constraints.end();) {
        if(dynamic_cast<const ForeignReferedConstraint *>(*it) == nullptr) {
            if(dynamic_cast<const ForeignReferedConstraint *>(*it)->GetReferenceTableName() == table_name) {
                delete *it;
                constraints.erase(it);
            }
            else it++;
        }
        else it++;
    }
    return kSuccess;
}

int Table::DropForeignReferedConstraint(std::string table_name, std::string field_name) {
    for(auto it = constraints.begin(); it != constraints.end();) {
        if(dynamic_cast<const ForeignReferedConstraint *>(*it) == nullptr) {
            if(dynamic_cast<const ForeignReferedConstraint *>(*it)->GetReferenceTableName() == table_name && dynamic_cast<const ForeignReferedConstraint *>(*it)->GetReferenceFieldName() == field_name) {
                delete *it;
                constraints.erase(it);
            }
            else it++;
        }
        else it++;
    }
    return kSuccess;
}

int Table::CheckConstraint(std::unordered_map<std::string, std::any>& record, Database* db) {
    int cnt = 0;
    std::cout<<"constraint number is "<<constraints.size()<<std::endl;
    for(auto constraint:constraints) {
        cnt++;
        std::cout<<"checking constraint "<<cnt<<std::endl;
        if (dynamic_cast<const DefaultConstraint *>(constraint) != nullptr){//默认
            if (!record.count(constraint->GetFieldName())) {
                record[constraint->GetFieldName()] = dynamic_cast<const DefaultConstraint *>(constraint)->GetValue();
            }
        }
        if(dynamic_cast<const NotNullConstraint *>(constraint) != nullptr) {//非空
            if(!record.count(constraint->GetFieldName())) {
                return kConstraintNotNullConflict;
            }
        }
        if (dynamic_cast<const UniqueConstraint *>(constraint) != nullptr){//唯一
            std::string field_name = constraint->GetFieldName();
            if(record.count(field_name)) {
                for(auto& other_record : records) {
                    if (other_record.count(field_name)){
                        if (ColasqlTool::CompareAny(record[field_name], other_record[field_name]) == kEqual){
                            return kConstraintUniqueConflict;
                        }
                    }
                }
            }
        }
        if (dynamic_cast<const PrimaryKeyConstraint *>(constraint) != nullptr) { //主键
            //std::cout<<"checking primary key constraint"<<std::endl;
            if(!record.count(constraint->GetFieldName())) {
                //std::cout<<"failed field name is "<<constraint->GetFieldName()<<std::endl;
                return kConstraintPrimaryKeyConflict;
            }
            std::string field_name = constraint->GetFieldName();
            for(auto& other_record : records) {
                if (other_record.count(field_name)){
                    if (ColasqlTool::CompareAny (record[field_name], other_record[field_name]) == kEqual){
                        //std::cout<<"failed record[field_name]"<< ColasqlTool::AnyToString(record[field_name])<<std::endl;
                        return kConstraintPrimaryKeyConflict;
                    }
                }
            }
        }
        if (dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr) { //检查外键约束
            if(!record.count(constraint->GetFieldName())) {
                continue;
            }
            std::string field_name = constraint->GetFieldName();
            std::any current_value = record[field_name];
            std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName();
            std::string reference_field_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceFieldName();
            std::string tmp;
            Table reference_table = Table(tmp);
            int ret = db->FindTable(reference_table_name, reference_table);
            //std::cout<<"Checking ForeignKeyConstraint 1: "<<reference_table_name<<" "<<reference_field_name<<std::endl;
            if(ret != kSuccess) return kImpossibleSituation;
            int success = 0;
            for(const auto& record : reference_table.GetRecords()) {
                if(!record.count(reference_field_name)) continue;
                //std::cout<<"Checking ForeignKeyConstraint 2: "<<ColasqlTool::AnyToString( record.at(reference_field_name))<<" "<<ColasqlTool::AnyToString(current_value)<<std::endl;
                if(ColasqlTool::CompareAny(current_value,record.at(reference_field_name)) == kEqual) {
                    success = 1;
                    break;
                }
            }
            if(success == 0)return kConstraintForeignKeyConflict;
        }
        if (dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr) { //检查反外键
            if(!record.count(constraint->GetFieldName())) {
                continue;
            }
            for(const auto& other_record: records) {
                if(other_record.count(constraint->GetFieldName()) && ColasqlTool::CompareAny(other_record.at(constraint->GetFieldName()), record.at(constraint->GetFieldName())) == kEqual) {
                    return kBeingRefered;
                }
            }
        }
    }
    return kSuccess;
}

int Table::CheckConstraint(std::unordered_map<std::string, std::any>& record, Database* db, std::vector<std::unordered_map<std::string, std::any> > records, int current_record_order) {
    int cnt = 0;
    //std::cout<<"constraint number is "<<constraints.size()<<std::endl;
    for(auto constraint:constraints) {
        cnt++;
        //std::cout<<"checking constraint "<<cnt<<std::endl;
        if (dynamic_cast<const DefaultConstraint *>(constraint) != nullptr){//默认
            if (!record.count(constraint->GetFieldName())) {
                record[constraint->GetFieldName()] = dynamic_cast<const DefaultConstraint *>(constraint)->GetValue();
            }
        }
        if(dynamic_cast<const NotNullConstraint *>(constraint) != nullptr) {//非空
            if(!record.count(constraint->GetFieldName())) {
                return kConstraintNotNullConflict;
            }
        }
        if (dynamic_cast<const UniqueConstraint *>(constraint) != nullptr){//唯一
            std::string field_name = constraint->GetFieldName();
            if(record.count(field_name)) {
                for(int i = 0; i < records.size(); ++i){
                    if(i == current_record_order) continue;
                    auto& other_record = records[i];
                    if (other_record.count(field_name)){
                        if (ColasqlTool::CompareAny(record[field_name], other_record[field_name]) == kEqual){
                            return kConstraintUniqueConflict;
                        }
                    }
                }
            }
        }
        if (dynamic_cast<const PrimaryKeyConstraint *>(constraint) != nullptr) { //主键
            //std::cout<<"checking primary key constraint"<<std::endl;
            if(!record.count(constraint->GetFieldName())) {
                //std::cout<<"failed field name is "<<constraint->GetFieldName()<<std::endl;
                return kConstraintPrimaryKeyConflict;
            }
            std::string field_name = constraint->GetFieldName();
            
            for(int i = 0; i < records.size(); ++i){
                if(i == current_record_order) continue;
                auto& other_record = records[i];

                if (other_record.count(field_name)){
                    if (ColasqlTool::CompareAny (record[field_name], other_record[field_name]) == kEqual){
                        //std::cout<<"failed record[field_name]"<< ColasqlTool::AnyToString(record[field_name])<<std::endl;
                        return kConstraintPrimaryKeyConflict;
                    }
                }
            }
        }
        if (dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr) { //检查外键约束
            if(!record.count(constraint->GetFieldName())) {
                continue;
            }
            std::string field_name = constraint->GetFieldName();
            std::any current_value = record[field_name];
            std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName();
            std::string reference_field_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceFieldName();
            std::string tmp;
            Table reference_table = Table(tmp);
            int ret = db->FindTable(reference_table_name, reference_table);
            //std::cout<<"Checking ForeignKeyConstraint 1: "<<reference_table_name<<" "<<reference_field_name<<std::endl;
            if(ret != kSuccess) return kImpossibleSituation;
            int success = 0;
            for(const auto& record : reference_table.GetRecords()) {
                if(!record.count(reference_field_name)) continue;
                //std::cout<<"Checking ForeignKeyConstraint 2: "<<ColasqlTool::AnyToString( record.at(reference_field_name))<<" "<<ColasqlTool::AnyToString(current_value)<<std::endl;
                if(ColasqlTool::CompareAny(current_value,record.at(reference_field_name)) == kEqual) {
                    success = 1;
                    break;
                }
            }
            if(success == 0)return kConstraintForeignKeyConflict;
        }
        if (dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr) { //检查反外键
            if(!record.count(constraint->GetFieldName())) {
                continue;
            }
            for(int i = 0; i < records.size(); ++i){
                if(i == current_record_order) continue;
                auto& other_record = records[i];
                if(other_record.count(constraint->GetFieldName()) && ColasqlTool::CompareAny(other_record.at(constraint->GetFieldName()), record.at(constraint->GetFieldName())) == kEqual) {
                    return kBeingRefered;
                }
            }
        }
    }
    return kSuccess;
}


int Table::Insert(std::vector<std::pair<std::string, std::string>> record_in, Database* db) {
    std::unordered_map<std::string, std::any> record;
    if(record_in[0].first == "*") {
        if(fields.size() != record_in.size()) {
            return kSizeNotProper;
        }
        for(int i = 0; i < fields.size(); ++i) {
            record_in[i].first = fields[i].first;
        }
    }
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
    int ret = CheckConstraint(record, db);
    if(ret != kSuccess) return ret;
    
    records.push_back(record);
    return kSuccess;
}


int Table::Select(std::vector<std::string> field_name,
                  const std::vector<std::tuple<std::string, std::string,int>>& conditions,
                  std::vector<std::vector<std::any>> &return_records,
                  const std::vector<std::string>& orderby_key) {
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

    
    bool haveGetAnswer = false;
    
    // ===== 获取 selected_index =====
    std::vector<int> selected_index;
    
    // 可以由索引得到选中记录
    if(!haveGetAnswer && index_ptr && conditions.size() > 0) {
        int ret = index_ptr->query(conditions, selected_index);
        if(ret == 0) {
            std::cout << "Index speedup successfully." << std::endl;
            haveGetAnswer = true;
        }
    }
    
    // 默认使用暴力方法求选中记录
    if(!haveGetAnswer) {
        for(int i = 0; i < records.size(); i++) {
            // 检查条件
            if(CheckCondition(records[i], conditions) != kSuccess) continue;
            // 添加一条记录
            selected_index.push_back(i);
        }
    }

    // ===== Order By =====
    if(orderby_key.size() > 0) {
        std::sort(selected_index.begin(), selected_index.end(), [&](int x, int y) {
            for(const auto& key: orderby_key) {
                int ret = ColasqlTool::CompareAny(records[x].at(key), records[y].at(key));
                if(ret < 0) return true;
                else if(ret > 0) return false;
            }
            return false;
        });
    }

    // ===== 将记录索引转换成表格 =====
    
    // 这个lambda表达式用于往return_records中加入一条记录
    auto addRecord = [&](const auto& record) {
        std::vector<std::any> ret_record;
        for(const auto& name: field_name) {
            if(!record.count(name)) {
                ret_record.push_back(std::any(ColasqlNull()));
            } else {
                ret_record.push_back(record.at(name));
            }
        }
        return_records.push_back(ret_record);
    };

    for(const auto& idx: selected_index) {
        assert(idx < records.size());
        addRecord(records[idx]);
    }

    return kSuccess;
}

int Table::CheckCondition(const std::unordered_map<std::string, std::any>& record, const std::vector<std::tuple<std::string, std::string, int>>& conditions) {
    for(const auto& condition : conditions) {
        std::string field_name = std::get<0>(condition);
        int expected_result = std::get<2>(condition);
        std::any to_any = ColasqlTool::GetAnyByTypeAndValue(field_map[field_name], std::get<1>(condition));
        
        if(!record.count(field_name)) {
            if(to_any.type() == typeid(ColasqlNull)) {
                continue;
            }
            return kConditionsNotSatisfied;
        }
        if(to_any.type() == typeid(ColasqlNull)) {
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
int Table::CheckBeingRefered(std::unordered_map<std::string, std::any>& record, Database* db) {
    //std::cout<<"Checking BeingRefered "<<constraints.size()<<std::endl;
    int cnt = 0;
    for(const auto& constraint:constraints) {
        //std::cout<<"checking "<<cnt<<" "<<(dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr)<<std::endl;
        cnt++;
        if (dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr){
            if(!record.count(constraint->GetFieldName())) continue;
            //std::cout<<"check point 1"<<std::endl;
            std::string field_name = constraint->GetFieldName();
            std::any current_value = record[field_name];
            std::string reference_table_name = dynamic_cast<const ForeignReferedConstraint *>(constraint)->GetReferenceTableName();
            std::string reference_field_name = dynamic_cast<const ForeignReferedConstraint *>(constraint)->GetReferenceFieldName();
            //std::cout<<"check point 2"<<std::endl;
            std::string tmp;
            Table reference_table = Table(tmp);
            int ret = db->FindTable(reference_table_name, reference_table);
            //std::cout<<"check point3"<<std::endl;
            if(ret != kSuccess) return kImpossibleSituation;
            for(const auto& record : reference_table.GetRecords()) {
                if(!record.count(reference_field_name)) continue;
                if(ColasqlTool::CompareAny(current_value,record.at(reference_field_name)) == kEqual) return kBeingRefered;
            }
           //std::cout<<"check point4"<<std::endl;
            
        }
    }
    //std::cout<<"check point5"<<std::endl;
    return kSuccess;
}
int Table::Delete(std::vector<std::tuple<std::string, std::string, int>> conditions, Database* db) {
    for (const auto &condition : conditions) {
        if(!field_map.count(std::get<0>(condition))) {
            return kFieldNotFound;
        }
    }
    std::vector<int> index_for_delete;
    for(int i = 0; i < records.size(); ++i) {
        std::unordered_map<std::string, std::any>& record = records[i];
        
        if(CheckCondition(record, conditions) == kSuccess) {
            if(CheckBeingRefered(record,db) == kBeingRefered) return kBeingRefered;
            index_for_delete.push_back(i);
        }
    }
    reverse(index_for_delete.begin(), index_for_delete.end());
    for(const auto& x : index_for_delete) {
        records.erase(records.begin() + x);
    }
    return kSuccess;
}

int Table::CheckDataType(std::string type, std::string value) {
    if(type == "int") {
        for(auto x : value) {
            if(x > '9' || x < '0') return kDataTypeWrong;
        }
    }
    else if(type == "float") {
        int sum_dot = 0;
        for (auto x : value) {
            if(x == '.') sum_dot++;
            if ((x > '9' || x < '0') && sum_dot>=2)
                return kDataTypeWrong;
        }
    }
    return kSuccess;
}
int Table::AlterTableConstraint(Constraint* constraint) {
    /*if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr) {
        std::string field_name = constraint->GetFieldName();
        std::any current_value = record[field_name];
        std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceTableName();
        std::string reference_field_name = dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetReferenceFieldName();
    }*/


    constraints.push_back(constraint);
    return kSuccess;
}
int Table::Update(const std::vector<std::pair<std::string,std::string>>& values, const std::vector<std::tuple<std::string, std::string, int>>& conditions, Database* db){
    for(const auto& change_field: values) {
        if(!field_map.count(change_field.first)) {
            return kFieldNotFound;
        }
    }
    for(const auto& field : values) {
        int ret = CheckDataType(field_map[field.first], field.second);
        if(ret != kSuccess) return ret;
    }
    auto test_records = records;
    int now_order = -1;
    for(auto& record: test_records) {
        now_order++;
        if(CheckCondition(record,conditions) != kSuccess) {
            continue;
        }
        for(const auto& field : values) {
            if(field.second == "") {
                if(!record.count(field.first)) continue;
                record.erase(record.find(field.first));
            }
            else if(field_map[field.first] == "int") {
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
        int ret = CheckConstraint(record, db, test_records, now_order);
        if(ret != kSuccess) return ret;
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

int Table::AlterTableDrop(std::string field_name, Database* db) {
    if(!field_map.count(field_name)) return kFieldNotFound;
    
    
    for(const auto& constraint : constraints) {
        if(dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr && dynamic_cast<const ForeignReferedConstraint *>(constraint)->GetFieldName()==field_name) 
            return kBeingRefered;
    }

    for(auto it = constraints.begin(); it != constraints.end();) {
        if((*it)->GetFieldName() != field_name) {it++;continue;}
        if(dynamic_cast<const ForeignKeyConstraint *>(*it) != nullptr) {
            //delete foreignreferedconstraint
            std::string reference_table_name = dynamic_cast<const ForeignKeyConstraint *>(*it)->GetReferenceTableName();
            db->FindTableReference(reference_table_name).DropForeignReferedConstraint(table_name, field_name);
        }
        delete (*it);
        constraints.erase(it);

    }


    for(auto& record : records) {
        for(auto it = record.begin(); it != record.end(); ++it) {
            if(it->first == field_name) {
                record.erase(it);
                break;
            }
        }
    }
    for(auto it = field_map.begin(); it != field_map.end(); ++it) {
        if(it->first == field_name) {
            field_map.erase(it);
            break;
        }
    }
    for(int i = 0; i < fields.size(); ++i) {
        if(fields[i].first == field_name) {
            fields.erase(fields.begin() + i);
            break;
        }
    }
    return kSuccess;
}

int Table::AlterTableModify(std::pair<std::string, std::string> field) {
    if(!field_map.count(field.first)) {
        return kFieldNotFound;
    }
    if(field_map[field.first] == field.second) return kSuccess;
    for(const auto& constraint : constraints) {
        if(dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr && dynamic_cast<const ForeignKeyConstraint *>(constraint)->GetFieldName()==field.first) 
            return kConstraintForeignKeyConflict;
        if(dynamic_cast<const ForeignReferedConstraint *>(constraint) != nullptr && dynamic_cast<const ForeignReferedConstraint *>(constraint)->GetFieldName()==field.first) 
            return kBeingRefered;
        if(dynamic_cast<const DefaultConstraint *>(constraint) != nullptr&& dynamic_cast<const DefaultConstraint *>(constraint)->GetFieldName()==field.first) return 
            kConstraintDefaultConflict;
    }
    for(auto& record: records) {
        if(!record.count(field.first)) {
            continue;
        }
        int ret = CheckDataType(field.second, ColasqlTool::AnyToString(record[field.first]));
        //std::cout<<"ret : "<<ret<<std::endl;
        if(ret != kSuccess) return ret;
    }
    field_map[field.first] = field.second;
    for(auto& old_field : fields) {
        if(old_field.first == field.first) {
            old_field = field;
        }
    }
    for(auto& record: records) {
        if(!record.count(field.first)) {
            continue;
        }
        std::string tmp = ColasqlTool::AnyToString(record[field.first]);
        record[field.first] = ColasqlTool::GetAnyByTypeAndValue(field.second, tmp);
    }
    return kSuccess;
}

// ===== Index =====

int Table::BuildIndex(const std::vector<std::string>& compare_key, int type) {
    if(type == kFHQTreapIndex) {
        // delete index_ptr;
        index_ptr = nullptr;
        index_ptr = new FHQTreapIndex(records, fields, field_map, compare_key);
        return kSuccess;
    } else {
        return kUnknownIndex;
    }
}
