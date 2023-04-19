#include "my_parser.h"

#include <unordered_map>

namespace ColaSQLCommand {

Parser::Parser() {}

std::string Parser::Parse(const std::vector<std::string>& seq) {

    // std::string output = "";
    // for(auto str: seq) output.append(str + " ");
    // return output;

    // ----- size 0 -----
    if(seq.size() <= 0) return error + statementIncomplete;

    // ----- Save -----
    if(seq[0] == "save") return TestSave();

    // ----- size 1 -----
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
    if(seq.size() != 3) {
        return error + statementRedundant;
    }

    std::string databaseName = seq[2];

    int ret = DataProcessor::GetInstance().CreateDatabase(databaseName);

    if(ret != 0) {
        return error; // TODO: error information
    }

    return success;
}

std::string Parser::DeleteDatabase(const std::vector<std::string>& seq) {
    return "Warning: DeleteDatabase Under development";
}

std::string Parser::UseDatabase(const std::vector<std::string>& seq) {
    if(seq.size() != 2) {
        return error + statementRedundant;
    }

    std::string databaseName = seq[1];

    int ret = DataProcessor::GetInstance().UseDatabase(databaseName);

    if(ret != 0) {
        return error; // TODO: error information
    }

    return success;
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
    std::vector<std::pair<std::string, std::string>> fields;
    std::vector<Constraint*> constraints;

    const std::string constraint = "constraint";

    std::unordered_map<std::string, std::string> fieldMap; // 预处理，防止往core传入错误内容

    // 获取 fields
    int i;
    for(i = 3; i < seq.size() - 1; i += 2) {
        if(seq[i] == constraint) { // fields over
            break;
        }

        std::string fieldName = seq[i];
        // std::any value = ColasqlTool::GetAnyByType(seq[i + 1]);
        // if(value.type() == typeid(ColasqlException)) return error + statementError + "(type name is unknown) (maybe forgot to add \"constraint\")";

        if(fieldMap.count(fieldName) != 0) return error + statementError + "(field name is duplicate)";

        fields.push_back({fieldName, seq[i + 1]});

        fieldMap[fieldName] = seq[i + 1];
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

            std::any value = ColasqlTool::GetAnyByTypeAndValue(fieldMap[seq[i + 2]], seq[i + 3]);
            if(value.type() == typeid(ColasqlException)) return error + statementError + "(default value is incompatible)";

            constraints.push_back(new DefaultConstraint(seq[i + 2], value));

        } else {
            return error + statementError + "(not found this kind of constraint)";
        }
    }

    ColasqlTool::OutputFields(fields);
    ColasqlTool::OutputConstraints(constraints);

    // std::string tableName = seq[2];
    // std::vector<std::pair<std::string, std::any>> fields;
    // std::vector<Constraint*> constraints;

    int ret = DataProcessor::GetInstance().CreateTable(tableName, fields, constraints);

    if(ret != 0) {
        return error; // TODO: error information
    }

    return success;
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

    if(seq.size() < 6) return error + statementError;

    std::string tableName = seq[2];
    std::vector<std::pair<std::string, std::string>> values;

    int mid = 4;
    while(mid < seq.size() - 1 && seq[mid] != "values") mid++;

    int i = 3, j = mid + 1;
    if(mid - i != seq.size() - j) return error + statementError + "(parameters are not incompatible)";

    for(int i = 3, j = mid + 1; i < mid && j < seq.size(); i++, j++) {
        values.push_back({seq[i], seq[j]});
    }

    int ret = DataProcessor::GetInstance().Insert(tableName, values);

    if(ret != 0) {
        return error; // TODO: error information
    }

    return success;
}

std::string Parser::DeleteRecord(const std::vector<std::string>& seq) {

    return "Warning: DeleteRecord Under development";
}

std::string Parser::SelectRecord(const std::vector<std::string>& seq) {

    if(seq.size() < 4) return error + statementError;

    int selectIdx = 0;
    int fromIdx = -1;
    int whereIdx = -1;
    int groupIdx = -1;  // TODO
    int havingIdx = -1; // TODO
    int orderIdx = -1;

    std::vector<int> idx;

    idx.push_back(0);
    for(int i = selectIdx + 1; i < seq.size(); i++) {
        if(seq[i] == "from") fromIdx = idx.size(), idx.push_back(i);
        else if(seq[i] == "where") whereIdx = idx.size(), idx.push_back(i);
        else if(seq[i] == "group") groupIdx = idx.size(), idx.push_back(i);
        else if(seq[i] == "having") havingIdx = idx.size(), idx.push_back(i);
        else if(seq[i] == "order") orderIdx = idx.size(), idx.push_back(i);
    }
    idx.push_back(seq.size());

    /**
     * SELECT 列名1 列名2 列名3 ...
     * FROM 表名1 [NATURAL JOIN 表名2 ...]
     * [WHERE 筛选条件1 筛选条件2 ...]
     * [ORDER BY 列名1 列名2 ...];
    */
    std::vector<std::string> fieldName;
    std::vector<std::string> tableName;
    std::vector<std::tuple<std::string, std::string, int>> conditions;
    std::vector<std::string> orderField;

    // select
    for(int i = idx[selectIdx] + 1; i < idx[selectIdx + 1]; i++) fieldName.push_back(seq[i]);

    // from
    for(int i = idx[fromIdx] + 1; i < idx[fromIdx + 1]; i += 3) {
        if(i != idx[fromIdx] + 1 && (seq[i - 2] != "natural" || seq[i - 1] != "join")) return error + statementError + "(missing \"NATURAL JOIN\")";

        tableName.push_back(seq[i]);
    }

    // from error
    if(tableName.size() <= 0) {
        return error + statementError + "(have no tables)";
    }

    // where
    if(whereIdx != -1) {
        if((idx[whereIdx + 1] - idx[whereIdx] - 1) % 3 != 0) return error + statementError + "(conditions statement is incompatible)";

        for(int i = idx[whereIdx] + 1; i < idx[whereIdx + 1]; i += 3) {
            if(seq[i + 1] != "=") return error + statementError + "(conditions statement is error)";

            // TODO: 缺少了其他关系条件
            conditions.push_back({seq[i], seq[i + 2], kEqualConditon});
        }
    }

    // TODO: group by
    // TODO: having

    // order by
    for(int i = idx[orderIdx] + 2; i < idx[orderIdx + 1]; i++) {
        orderField.push_back(seq[i]);
    }

    // output for testing
    std::cout << "  fieldName: ";
    for(auto str: fieldName) std::cout << str << " ";
    std::cout << std::endl;

    std::cout << "  tableName: ";
    for(auto str: tableName) std::cout << str << " ";
    std::cout << std::endl;

    std::cout << "  conditions: ";
    for(auto [str1, str2, i]: conditions) std::cout << "(" << str1 << ", " << str2 << ", " << i << ") ";
    std::cout << std::endl;

    std::cout << "  orderField: ";
    for(auto str: orderField) std::cout << str << " ";
    std::cout << std::endl;

    // std::vector<std::string> fieldName;
    // std::vector<std::string> tableName;
    // std::vector<std::tuple<std::string, std::string, int>> conditions;
    // std::vector<std::string> orderField;

    std::vector<std::vector<std::any>> result;

    int ret = DataProcessor::GetInstance().Select(tableName[0], fieldName, conditions, result);

    if(ret != 0) {
        return error; // TODO: error information
    }

    ColasqlTool::OutputSelectResult(result);

    return success;
}

std::string Parser::UpdateRecord(const std::vector<std::string>& seq) {
    return "Warning: UpdateRecord Under development";
}

std::string Parser::TestSave() {
    FileManager::GetInstance().WriteDatabases(DataProcessor::GetInstance().GetDatabases());
    return "Test";
}

} // ColaSQLCommand
