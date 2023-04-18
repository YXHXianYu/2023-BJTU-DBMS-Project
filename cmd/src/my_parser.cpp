#include "my_parser.h"

#include <unordered_map>

namespace ColaSQLCommand {

Parser::Parser() {}

std::string Parser::Parse(const std::vector<std::string>& seq) {
    
    // std::string output = "";
    // for(auto str: seq) output.append(str + " ");
    // return output;

    if(seq.size() <= 1) return error + statementIncomplete;

    // ----- User -----
    if(seq[1] == "user") {
        if(seq[0] == "create") return CreateUser(seq);         // Create User
        else if(seq[0] == "grant") return GrantUser(seq);
        else if(seq[0] == "drop") return DeleteUser(seq);      // Drop User
    }

    // ----- Database -----
    if(seq[1] == "database") {
        if(seq[0] == "create") return CreateDatabase(seq);     // Create Database
        else if(seq[0] == "drop") return CreateDatabase(seq);  // Drop Database
    } else if(seq[0] == "use") {
        return UseDatabase(seq);                               // Use Database
    } else if(seq[0] == "show" && seq[1] == "databases") {
        return ShowDatabases(seq);                             // Show Databases
    }

    // ----- Table -----
    if(seq[1] == "table") {
        if(seq[0] == "create") return CreateTable(seq);        // Create Table
        else if(seq[0] == "drop") return DeleteTable(seq);     // Drop Table
        else if(seq[0] == "alter") {                           // Alter Table
            if(seq.size() <= 3) return error + statementIncomplete;
            if(seq[3] == "add") return AlterTableAdd(seq);
            else if(seq[3] == "drop") return AlterTableDrop(seq);
            else if(seq[3] == "modify") return AlterTableModify(seq);
            else if(seq[3] == "constraint") return AlterTableConstraint(seq);
            else if(seq[3] == "delete") return AlterTableDeleteConstraint(seq);
        } else if(seq[0] == "desc" || seq[0] == "describe") return QueryTable(seq); // Query Table
    } else if(seq[0] == "show" && seq[1] == "tables") {
        return ShowTables(seq);                                // Show Tables
    }

    // ----- Record -----
    if(seq[0] == "insert" && seq[1] == "into") return InsertRecord(seq);
    if(seq[0] == "delete" && seq[1] == "from") return DeleteRecord(seq);
    if(seq[0] == "select") return SelectRecord(seq);
    if(seq[0] == "update") return UpdateRecord(seq);

    return error + statementError;
}

// ----- Tools -----

std::any Parser::GetAnyByType(const std::string& type) {
    std::any result;

    if (type == "int") result = 1;
    else if(type == "float") result = 1.0f;
    else if(type == "string") result = std::string("rua");
    else result = Exception();

    return result;
}


std::any Parser::GetAnyByTypeAndValue(const std::any& type, const std::string& value) {
    std::any result;

    if(type.type() == typeid(int)) result = std::stoi(value);
    else if(type.type() == typeid(float)) result = std::stof(value);
    else if(type.type() == typeid(std::string)) result = value;
    else result = Exception();

    return result;
}

std::string Parser::AnyToString(const std::any& any) {

    if(any.type() == typeid(int)) return std::to_string(std::any_cast<int>(any));
    else if(any.type() == typeid(float)) return std::to_string(std::any_cast<float>(any));
    else if(any.type() == typeid(std::string)) return std::any_cast<std::string>(any);
    else return "[not found this type]";

}

void Parser::OutputFields(const std::vector<std::pair<std::string, std::any>>& fields) {
    std::cout << "Fields:" << std::endl;
    for(auto [name, value]: fields) {
        std::cout << "  " << name << ": " << AnyToString(value) << std::endl;
    }
    std::cout << std::endl;
}

void Parser::OutputConstraints(const std::vector<Constraint*> constraints) {
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

// ----- User -----

std::string Parser::CreateUser(const std::vector<std::string>& seq) {
    return "Warning: Create User is under development";
}

std::string Parser::GrantUser(const std::vector<std::string>& seq) {
    return "Warning: Grant User is under development";
}

std::string Parser::DeleteUser(const std::vector<std::string>& seq) {
    return "Warning: Delete User is under development";
}

// ----- Database -----

std::string Parser::CreateDatabase(const std::vector<std::string>& seq) {
    return "Warning: CreateDatabase Under development";
}

std::string Parser::DeleteDatabase(const std::vector<std::string>& seq) {
    return "Warning: DeleteDatabase Under development";
}

std::string Parser::UseDatabase(const std::vector<std::string>& seq) {
    if(seq.size() >= 3) {
        return error + statementRedundant;
    }

    std::string databaseName = seq[1];

    // TODO: use database

    return "Warning: UseDatabase Under development";
}

std::string Parser::ShowDatabases(const std::vector<std::string>& seq) {
    return "Warning: ShowDatabases Under development";
}

// ----- Table -----

std::string Parser::CreateTable(const std::vector<std::string>& seq) {

    if (seq.size() < 5) {
        return error + statementIncomplete;
    }

    std::string tableName = seq[2];

    std::vector<std::pair<std::string, std::any>> fields;
    std::vector<Constraint*> constraints;

    // 通过 类型名 获取对应类型的 any值
    

    const std::string constraint = "constraint";

    std::unordered_map<std::string, std::any> fieldMap; // 预处理，防止往core传入错误内容

    // 获取 fields
    int i;
    for(i = 3; i < seq.size() - 1; i += 2) {
        if(seq[i] == constraint) { // fields over
            break;
        }

        std::string fieldName = seq[i];
        std::any value = GetAnyByType(seq[i + 1]);
        if(value.type() == typeid(Exception)) return error + statementError + "(type name is unknown) (maybe forgot to add \"constraint\")";

        if(fieldMap.count(fieldName) != 0) return error + statementError + "(field name is duplicate)";

        fields.push_back({fieldName, value});

        fieldMap[fieldName] = value;
    }

    // 获取 constraints
    int j;
    for(; i < seq.size(); i = j + 1) {
        j = i;
        // 如果下一个token不是constraint，那么就继续读取token
        while(j < seq.size() - 1 && seq[j + 1] != constraint) j++;

        // token读取完毕，seq[i...j]即为当前constraint的所有内容
        if(seq[i] != constraint) return error + statementError + "(error occurs when getting constraints in CreateTable)";
        if(j - i < 2) return error + statementError + "(constraint statement is too short)";

        if(seq[i + 1] == "primary" && seq[i + 2] == "key") {
            if(j - i != 3) return error + statementError + "(constraint statement is too short or too long)";
            if(fieldMap.count(seq[i + 3]) == 0) return error + statementError + "(field name is not found)";

            constraints.push_back(new PrimaryKeyConstraint(seq[i + 3]));

        } else if(seq[i + 1] == "foreign" && seq[i + 2] == "key") {
            if(j - i != 6) return error + statementError + "(constraint statement is too short or too long)";
            if(fieldMap.count(seq[i + 3]) == 0) return error + statementError + "(field name is not found)";
            if(seq[i + 4] != "reference") return error + statementError + "(not found \"reference\")";
            
            constraints.push_back(new ForeignKeyConstraint(seq[i + 3], seq[i + 5], seq[i + 6]));

        } else if(seq[i + 1] == "unique") {
            if(j - i != 2) return error + statementError + "(constraint statement is too short or too long)";
            if(fieldMap.count(seq[i + 2]) == 0) return error + statementError + "(field name is not found)";

            constraints.push_back(new UniqueConstraint(seq[i + 2]));

        } else if(seq[i + 1] == "not" && seq[i + 2] == "null") {
            if(j - i != 3) return error + statementError + "(constraint statement is too short or too long)";
            if(fieldMap.count(seq[i + 3]) == 0) return error + statementError + "(field name is not found)";

            constraints.push_back(new NotNullConstraint(seq[i + 3]));

        } else if(seq[i + 1] == "default") {
            if(j - i != 3) return error + statementError + "(constraint statement is too short or too long)";
            if(fieldMap.count(seq[i + 2]) == 0) return error + statementError + "(field name is not found)";

            std::any value = GetAnyByTypeAndValue(fieldMap[seq[i + 2]], seq[i + 3]);
            if(value.type() == typeid(Exception)) return error + statementError + "(default value is incompatible)";

            constraints.push_back(new DefaultConstraint(seq[i + 2], value));
        } else {

            return error + statementError + "(not found this kind of constraint)";
        }
    }

    OutputFields(fields);
    OutputConstraints(constraints);

    // TODO: call function

    return "Warning: CreateTable Under development";
}

std::string Parser::DeleteTable(const std::vector<std::string>& seq) {
    return "Warning: DeleteTable Under development";
}

std::string Parser::AlterTableAdd(const std::vector<std::string>& seq) {
    return "Warning: AlterTableAdd Under development";
}

std::string Parser::AlterTableDrop(const std::vector<std::string>& seq) {
    return "Warning: AlterTableDrop Under development";
}

std::string Parser::AlterTableModify(const std::vector<std::string>& seq) {
    return "Warning: AlterTableModify Under development";
}

std::string Parser::AlterTableConstraint(const std::vector<std::string>& seq) {
    return "Warning: AlterTableConstraint Under development";
}

std::string Parser::AlterTableDeleteConstraint(const std::vector<std::string>& seq) {
    return "Warning: AlterTableDeleteConstraint Under development";
}

std::string Parser::QueryTable(const std::vector<std::string>& seq) {
    return "Warning: QueryTable Under development";
}

std::string Parser::ShowTables(const std::vector<std::string>& seq) {
    return "Warning: ShowTables Under development";
}

// ----- Record -----

std::string Parser::InsertRecord(const std::vector<std::string>& seq) {
    return "Warning: InsertRecord Under development";
}

std::string Parser::DeleteRecord(const std::vector<std::string>& seq) {
    return "Warning: DeleteRecord Under development";
}

std::string Parser::SelectRecord(const std::vector<std::string>& seq) {
    return "Warning: SelectRecord Under development";
}

std::string Parser::UpdateRecord(const std::vector<std::string>& seq) {
    return "Warning: UpdateRecord Under development";
}

} // ColaSQLCommand