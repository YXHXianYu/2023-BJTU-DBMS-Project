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
    else return "[not found this type]";

}

void ColasqlTool::OutputFields(const std::vector<std::pair<std::string, std::any>>& fields) {
    std::cout << "Fields:" << std::endl;
    for(auto [name, value]: fields) {
        std::cout << "  " << name << ": " << AnyToString(value) << std::endl;
    }
    std::cout << std::endl;
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