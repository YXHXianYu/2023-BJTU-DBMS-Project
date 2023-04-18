#include <iostream>
#include <vector>
#include <cstring>

#include "all_constraints.h"

int main() {

    std::vector<Constraint*> constraints;

    constraints.push_back(new PrimaryKeyConstraint("FieldName"));
    constraints.push_back(new ForeignKeyConstraint("FieldName", "ReferenceTableName", "ReferenceFieldName"));
    constraints.push_back(new NotNullConstraint("FieldName"));
    constraints.push_back(new UniqueConstraint("FieldName"));
    constraints.push_back(new DefaultConstraint("FieldName", 1));
    constraints.push_back(new DefaultConstraint("FieldName", 1.0f));
    constraints.push_back(new DefaultConstraint("FieldName", std::string("value")));
    constraints.push_back(new DefaultConstraint("FieldName", std::string("1234567890qwertyuiopasdfghjklzxcvbnm")));

    for(auto &it: constraints) {
        if(dynamic_cast<PrimaryKeyConstraint*>(it) != nullptr) {
            PrimaryKeyConstraint* p = dynamic_cast<PrimaryKeyConstraint*>(it);

            std::cout << "Primary Key: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<ForeignKeyConstraint*>(it) != nullptr) {
            ForeignKeyConstraint* p = dynamic_cast<ForeignKeyConstraint*>(it);

            std::cout << "Foreign Key: " << p->GetFieldName() << " Reference " << p->GetRefenrenceTableName() << "." << p->GetReferenceFieldName() << std::endl;
        } else if(dynamic_cast<NotNullConstraint*>(it) != nullptr) {
            NotNullConstraint* p = dynamic_cast<NotNullConstraint*>(it);

            std::cout << "Not Null: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<UniqueConstraint*>(it) != nullptr) {
            UniqueConstraint* p = dynamic_cast<UniqueConstraint*>(it);

            std::cout << "Unique: " << p->GetFieldName() << std::endl;
        } else if(dynamic_cast<DefaultConstraint*>(it) != nullptr) {
            DefaultConstraint* p = dynamic_cast<DefaultConstraint*>(it);

            if(p->GetValue().type() == typeid(int))
                std::cout << "Default: " << p->GetFieldName() << " = " << std::any_cast<int>(p->GetValue()) << std::endl;
            else if(p->GetValue().type() == typeid(float))
                std::cout << "Default: " << p->GetFieldName() << " = " << std::any_cast<float>(p->GetValue()) << std::endl;
            else if(p->GetValue().type() == typeid(std::string))
                std::cout << "Default: " << p->GetFieldName() << " = " << std::any_cast<std::string>(p->GetValue()) << std::endl;
            else
                std::cout << "Default: " << p->GetFieldName() << "; Unknown Type " << p->GetValue().type().name() << std::endl;
        } else {
            std::cout << "Unknown Constraint." << std::endl;
        }
    }

    return 0;
}