#include "colasqltool.h"

#include <iostream>

std::any ColasqlTool::GetAnyByType(const std::string& type) {
    std::any result;

    if (type == "int") result = 1;
    else if(type == "float") result = 1.0f;
    else if(type == "string") result = std::string("rua");
    else result = ColasqlException();

    return result;
}


std::any ColasqlTool::GetAnyByTypeAndValue(const std::string& type, const std::string& value) {
    std::any result;

    if(type == "int") result = std::stoi(value);
    else if(type == "float") result = std::stof(value);
    else if(type == "string") result = value;
    else result = ColasqlException();

    return result;
}

std::any ColasqlTool::GetAnyByTypeAndValue(const std::any& type, const std::string& value) {
    std::any result;

    if(type.type() == typeid(int)) result = std::stoi(value);
    else if(type.type() == typeid(float)) result = std::stof(value);
    else if(type.type() == typeid(std::string)) result = value;
    else result = ColasqlException();

    return result;
}

std::string ColasqlTool::AnyToString(const std::any& any) {

    if(any.type() == typeid(int)) return std::to_string(std::any_cast<int>(any));
    else if(any.type() == typeid(float)) return std::to_string(std::any_cast<float>(any));
    else if(any.type() == typeid(std::string)) return std::any_cast<std::string>(any);
    else if(any.type() == typeid(ColasqlNull)) return "NULL";
    else return "[not found this type]";

}

void ColasqlTool::OutputFields(const std::vector<std::pair<std::string, std::string>>& fields) {
    std::cout << "Fields:" << std::endl;
    for(auto [name, value]: fields) {
        std::cout << "  " << name << ": " << value << std::endl;
    }
    std::cout << std::endl;
}

int ColasqlTool::CompareAny(const std::any& any1, const std::any& any2) {
    if(any1.type() != any2.type()) {
        return kNotSameType;
    }
    if (any1.type() == typeid(int)) {
        int val1 = std::any_cast<int>(any1);
        int val2 = std::any_cast<int>(any2);
        if(val1 == val2) return kEqual;
        if(val1 < val2) return kLess;
        if(val1 > val2) return kLarger;
    }
    if (any1.type() == typeid(float)) {
        float val1 = std::any_cast<float>(any1);
        float val2 = std::any_cast<float>(any2);
        if(std::fabs(val1 - val2) <= 1e-6) return kEqual;
        if(val1 < val2) return kLess;
        if(val1 > val2) return kLarger;
    }

    if (any1.type() == typeid(std::string)) {

        std::string val1 = std::any_cast<std::string>(any1);
        std::string val2 = std::any_cast<std::string>(any2);
        if (val1 == val2) return kEqual;
        if(val1 < val2) return kLess;
        if(val1 > val2) return kLarger;
    }
    return kEqual;
}

void ColasqlTool::OutputConstraints(const std::vector<Constraint*> constraints) {
    std::cout << "Constraints: " << std::endl;
    for(auto &it: constraints) {
        if(dynamic_cast<PrimaryKeyConstraint*>(it) != nullptr) {
            PrimaryKeyConstraint* p = dynamic_cast<PrimaryKeyConstraint*>(it);

            std::cout << "  Primary Key: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<ForeignKeyConstraint*>(it) != nullptr) {
            ForeignKeyConstraint* p = dynamic_cast<ForeignKeyConstraint*>(it);

            std::cout << "  Foreign Key: " << p->GetFieldName() << " Reference " << p->GetRefenrenceTableName() << "." << p->GetReferenceFieldName() << std::endl;
        } else if(dynamic_cast<NotNullConstraint*>(it) != nullptr) {
            NotNullConstraint* p = dynamic_cast<NotNullConstraint*>(it);

            std::cout << "  Not Null: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<UniqueConstraint*>(it) != nullptr) {
            UniqueConstraint* p = dynamic_cast<UniqueConstraint*>(it);

            std::cout << "  Unique: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<DefaultConstraint*>(it) != nullptr) {
            DefaultConstraint* p = dynamic_cast<DefaultConstraint*>(it);

            if(p->GetValue().type() == typeid(int))
                std::cout << "  Default: " << p->GetFieldName() << " = " << std::any_cast<int>(p->GetValue()) << std::endl;
            else if(p->GetValue().type() == typeid(float))
                std::cout << "  Default: " << p->GetFieldName() << " = " << std::any_cast<float>(p->GetValue()) << std::endl;
            else if(p->GetValue().type() == typeid(std::string))
                std::cout << "  Default: " << p->GetFieldName() << " = " << std::any_cast<std::string>(p->GetValue()) << std::endl;
            else
                std::cout << "  Default: " << p->GetFieldName() << "; Unknown Type " << p->GetValue().type().name() << std::endl;
        } else {
            std::cout << "Unknown Constraint." << std::endl;
        }
    }
    std::cout << std::endl;
}

std::vector<std::vector<std::any>> ColasqlTool::ChangeDescriptionToRecords(const std::vector<std::pair<std::string, std::string>>& fields,const std::vector<Constraint*>&constraints) {
    std::vector<std::vector<std::any>> ret;
    std::vector<std::any> inner;
    for(auto x : fields) {
        std::cout<<"Che: "<<x.first<<" "<<x.second<<std::endl;
    }
    inner.push_back(std::string("Field"));
    inner.push_back(std::string("Type"));
    inner.push_back(std::string("Null"));
    inner.push_back(std::string("Key"));
    inner.push_back(std::string("Default"));
    ret.push_back(inner);

    inner.clear();
    for(const auto& field : fields) {
        std::string field_name = field.first;
        //std::cout<<"testdes"<<" "<<field_name<<std::endl;
        std::string Type = field.second;
        std::string Null = "YES";
        std::string Key = "";
        std::string Default = "NULL";
        for(auto constraint: constraints) {
            if (constraint->GetFieldName() != field_name) {
                continue;
            }
            if (dynamic_cast<const DefaultConstraint *>(constraint) != nullptr){
                //todo
                Default = ColasqlTool::AnyToString(dynamic_cast<const DefaultConstraint *>(constraint)->GetValue());
            }
            if(dynamic_cast<const NotNullConstraint *>(constraint) != nullptr) {//非空
                Null = "NO";
            }
            if (dynamic_cast<const UniqueConstraint *>(constraint) != nullptr){
                Key = "UNI";
            }
            if (dynamic_cast<const ForeignKeyConstraint *>(constraint) != nullptr){
                Key = "FOR";
            }
            if (dynamic_cast<const PrimaryKeyConstraint *>(constraint) != nullptr){
                Key = "PRI";
            }

        }
        inner.clear();
        inner.push_back(field_name);
        inner.push_back(Type);
        inner.push_back(Null);
        inner.push_back(Key);
        inner.push_back(Default);
        ret.push_back(inner);
    }
    return ret;
}

std::vector<std::vector<std::any>> ColasqlTool::ChangeStringsToRecords(std::vector<std::string> strings, std::string head="") {
    std::vector<std::vector<std::any>> ret;
    if(head != "") {
        std::vector<std::any> inner;
        inner.push_back(head);
        ret.push_back(inner);
    }
    for(const auto x: strings) {
        std::vector<std::any> inner;
        inner.push_back(x);
        ret.push_back(inner);
    }
    return ret;
}

std::string ColasqlTool::OutputSelectResult(const std::vector<std::vector<std::any>> result) {
    
    std::string out_result;
    if(result.size() <= 1) {
        out_result = "Empty set";
        std::cout<<"Empty set";
        return out_result;
    }
    int count_field = 0;
    int count_record = 0;
    std::vector<int> len;
    
    for(auto field_name : result[0]) {
        count_record++;
        len.push_back(ColasqlTool::AnyToString(field_name).length());
    }
    for(auto record : result) {
        for(int i = 0; i < record.size(); ++i){
            len[i] = std::max(len[i], (int)ColasqlTool::AnyToString(record[i]).length());
        }
    }
    std::string tmp;
    tmp += "+-";
    for(int i = 0; i < count_record; ++i) {
        for(int j = 1; j <= len[i]; ++j) {
            tmp += "-";
        }
        if(i != count_record - 1)
            tmp += "-+-";
        else tmp += "-+";
    }
    out_result += tmp;
    out_result += '\n';
    for(const auto& record : result) {
        out_result += "| ";
        for(int i = 0; i < count_record; ++i){
            out_result += ColasqlTool::AnyToString(record[i]);
            int current_len = ColasqlTool::AnyToString(record[i]).length();
            for(int j = 1; j <= len[i] - current_len; ++j) {
                out_result += " ";
            }
            if(i != count_record - 1)
                out_result += " | ";
            else out_result += " |";
        }
        out_result += '\n';
        out_result += tmp;
        out_result += '\n';
    }
    std::cout<<out_result;
    return out_result;
}