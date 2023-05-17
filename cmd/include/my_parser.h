#ifndef __MY_PARSER_H__
#define __MY_PARSER_H__

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <any>

// cmd
#include "all_constraints.h"
#include "colasqltool.h"

// core
#include "dataprocessor.h"
#include "constants.h"

namespace ColaSQLCommand {

/**
 * A Co1aSQL Parser
 * 读入一句Co1aSQL语句，将其解析成一段core模块的接口调用
*/
class Parser {
public:
    Parser();
    std::string Parse(const std::vector<std::string>& seq);
    std::string ParseSelect(const std::vector<std::string>& seq, std::vector<std::vector<std::any>>& result);

    const bool DEBUG = false;

    const std::string success = "Success!";
    const std::string error = "Error: ";
    const std::string statementIncomplete = "Statement is incomplete.";
    const std::string statementError = "Statement has errors.";
    const std::string statementRedundant = "Statment has redundant parts.";

private:

    std::string CreateUser(const std::vector<std::string>& seq); //
    std::string GrantUser(const std::vector<std::string>& seq); //
    std::string DeleteUser(const std::vector<std::string>& seq); //
    // std::string ActivateAdmin(const std::vector<std::string>& seq);
    // std::string DeactivateAdmin(const std::vector<std::string>& seq);

    std::string CreateDatabase(const std::vector<std::string>& seq); //
    std::string DeleteDatabase(const std::vector<std::string>& seq); //
    std::string UseDatabase(const std::vector<std::string>& seq);
    std::string ShowDatabases(const std::vector<std::string>& seq); //

    std::string CreateTable(const std::vector<std::string>& seq);
    std::string DeleteTable(const std::vector<std::string>& seq); //
    std::string AlterTableAdd(const std::vector<std::string>& seq); //
    std::string AlterTableDrop(const std::vector<std::string>& seq); //
    std::string AlterTableModify(const std::vector<std::string>& seq); //
    std::string AlterTableConstraint(const std::vector<std::string>& seq); //
    std::string AlterTableDeleteConstraint(const std::vector<std::string>& seq); //
    std::string QueryTable(const std::vector<std::string>& seq); //
    std::string ShowTables(const std::vector<std::string>& seq); //

    std::string InsertRecord(const std::vector<std::string>& seq);
    std::string DeleteRecord(const std::vector<std::string>& seq); //
    std::string SelectRecord(const std::vector<std::string>& seq);
    std::string UpdateRecord(const std::vector<std::string>& seq); //
    std::string ReturnSelectRecord(const std::vector<std::string>& seq, std::vector<std::vector<std::any>>& result);

    std::string BuildIndex(const std::vector<std::string>& seq);

    std::string Read(bool debug = false);
    std::string Save();

    std::string GetErrorMessage(int errorCode);

};

} // ColaSQLCommand

#endif // __MY_PARSER_H__
