#include "my_parser.h"

namespace ColaSQLCommand {

Parser::Parser() {}

std::string Parser::Parse(const std::vector<std::string>& seq) {
    
    // std::string output = "";
    // for(auto str: seq) output.append(str + " ");
    // return output;

    if(seq.size() <= 1) return statementIncomplete;

    // ----- User -----
    if(seq[1] == "user") {
        if(seq[0] == "create") return CreateUser(seq);         // Create User
        else if(seq[0] == "drop") return DeleteUser(seq);      // Drop User
    } else if(seq[1] == "admin") {
        if(seq[0] == "activate") return ActivateAdmin(seq);    // Activate Admin
        else if(seq[0] == "deactivate") return ActivateAdmin(seq); // Deactivate Admin
    }

    // ----- Database -----
    if(seq[1] == "database") {
        if(seq[0] == "create") return CreateDatabase(seq);     // Create Database
        else if(seq[0] == "drop") return CreateDatabase(seq);  // Drop Database
    } else if(seq[0] == "use") return UseDatabase(seq);        // Use Database

    // ----- Table -----
    if(seq[1] == "table") {
        if(seq[0] == "create") return CreateTable(seq);        // Create Table
        else if(seq[0] == "drop") return DeleteTable(seq);     // Drop Table
        else if(seq[0] == "alter") {                           // Alter Table
            if(seq.size() <= 3) return statementIncomplete;
            if(seq[3] == "add") return AlterTableAdd(seq);
            else if(seq[3] == "drop") return AlterTableDrop(seq);
            else if(seq[3] == "modify") return AlterTableModify(seq);
            else if(seq[3] == "constraint") return AlterTableConstraint(seq);
            else if(seq[3] == "delete") return AlterTableDeleteConstraint(seq);
        } else if(seq[0] == "desc" || seq[0] == "describe") return QueryTable(seq); // Query Table
    }

    // ----- Record -----
    if(seq[0] == "insert" && seq[1] == "into") return InsertRecord(seq);
    if(seq[0] == "delete" && seq[1] == "from") return DeleteRecord(seq);
    if(seq[0] == "select") return SelectRecord(seq);
    if(seq[0] == "update") return UpdateRecord(seq);

    return statementError;
}

// ----- User -----

std::string Parser::CreateUser(const std::vector<std::string>& seq) {

}

std::string Parser::DeleteUser(const std::vector<std::string>& seq) {

}

std::string Parser::ActivateAdmin(const std::vector<std::string>& seq) {

}

std::string Parser::DeactivateAdmin(const std::vector<std::string>& seq) {

}

// ----- Database -----

std::string Parser::CreateDatabase(const std::vector<std::string>& seq) {

}

std::string Parser::DeleteDatabase(const std::vector<std::string>& seq) {

}

std::string Parser::UseDatabase(const std::vector<std::string>& seq) {

}

// ----- Table -----

std::string Parser::CreateTable(const std::vector<std::string>& seq) {

}

std::string Parser::DeleteTable(const std::vector<std::string>& seq) {

}

std::string Parser::AlterTableAdd(const std::vector<std::string>& seq) {

}

std::string Parser::AlterTableDrop(const std::vector<std::string>& seq) {

}

std::string Parser::AlterTableModify(const std::vector<std::string>& seq) {

}

std::string Parser::AlterTableConstraint(const std::vector<std::string>& seq) {

}

std::string Parser::AlterTableDeleteConstraint(const std::vector<std::string>& seq) {

}

std::string Parser::QueryTable(const std::vector<std::string>& seq) {

}

// ----- Record -----

std::string Parser::InsertRecord(const std::vector<std::string>& seq) {

}

std::string Parser::DeleteRecord(const std::vector<std::string>& seq) {

}

std::string Parser::SelectRecord(const std::vector<std::string>& seq) {

}

std::string Parser::UpdateRecord(const std::vector<std::string>& seq) {

}

} // ColaSQLCommand