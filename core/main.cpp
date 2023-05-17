#include <iostream>
#include "colasqltool.h"
#include "constants.h"
#include "dataprocessor.h"
#include "all_constraints.h"
int main() {
    std::cout<< DataProcessor::GetInstance().CreateUser(std::string("admin"),std::string("123456"))<<std::endl;
    std::cout<< DataProcessor::GetInstance().Login(std::string("admin"),std::string("123456"))<<std::endl;
    std::cout<< DataProcessor::GetInstance().CreateDatabase("test")<<std::endl;
    std::cout<< DataProcessor::GetInstance().UseDatabase("test")<<std::endl;
    std::cout<< DataProcessor::GetInstance().CreateDatabase("test2")<<std::endl;
    
    
    //测试建表
    std::vector<std::pair<std::string, std::string>> fields;
     fields.push_back({"Sno", "string"});
     fields.push_back({"Sname", "string"});
     fields.push_back({"Age", "int"});
    std::vector<Constraint*> constraints;
    std::cout<<DataProcessor::GetInstance().CreateTable("student",fields,constraints)<<std::endl;

    
    fields.clear();
    fields.push_back({"Cno", "string"});
    fields.push_back({"Cname", "string"});
    std::cout<<DataProcessor::GetInstance().CreateTable("course",fields,constraints)<<std::endl;

    fields.clear();
    fields.push_back({"Sno", "string"});
    fields.push_back({"Cno", "string"});
    fields.push_back({"Grade", "int"});
    std::cout<<DataProcessor::GetInstance().CreateTable("sc",fields,constraints)<<std::endl;

    

    //测试insert course
    std::vector<std::pair<std::string, std::string>> record_in;
    record_in.clear();
    record_in.push_back({"Cno", "001"});
    record_in.push_back({"Cname", "software"});
    std::cout<<DataProcessor::GetInstance().Insert("course",record_in)<<std::endl;
    
    //测试insert course
    record_in.clear();
    record_in.push_back({"Cno", "002"});
    record_in.push_back({"Cname", "sleep"});
    std::cout<<DataProcessor::GetInstance().Insert("course",record_in)<<std::endl;
    
    //测试insert student
    record_in.clear();
    record_in.push_back({"Sno", "21301032"});
    record_in.push_back({"Sname", "drj"});
    record_in.push_back({"Age", "20"});
    std::cout<<DataProcessor::GetInstance().Insert("student",record_in)<<std::endl;

    record_in.clear();
    record_in.push_back({"Sno", "21301000"});
    record_in.push_back({"Sname", "hahaha"});
    record_in.push_back({"Age", "1100"});
    std::cout<<DataProcessor::GetInstance().Insert("student",record_in)<<std::endl;
    
    record_in.clear();
    record_in.push_back({"Sno", "21301030"});
    record_in.push_back({"Sname", "third"});
    record_in.push_back({"Age", "60"});
    std::cout<<DataProcessor::GetInstance().Insert("student",record_in)<<std::endl;

    //测试insert sc
    record_in.clear();
    record_in.push_back({"Sno", "21301032"});
    record_in.push_back({"Cno", "001"});
    record_in.push_back({"Grade", "99"});
    std::cout<<DataProcessor::GetInstance().Insert("sc",record_in)<<std::endl;

    record_in.clear();
    record_in.push_back({"Sno", "21301032"});
    record_in.push_back({"Cno", "002"});
    record_in.push_back({"Grade", "97"});
    std::cout<<DataProcessor::GetInstance().Insert("sc",record_in)<<std::endl;

    record_in.clear();
    record_in.push_back({"Sno", "21301030"});
    record_in.push_back({"Cno", "002"});
    record_in.push_back({"Grade", "90"});
    std::cout<<DataProcessor::GetInstance().Insert("sc",record_in)<<std::endl;

    record_in.clear();
    record_in.push_back({"Sno", "21301000"});
    record_in.push_back({"Cno", "001"});
    record_in.push_back({"Grade", "50"});
    std::cout<<DataProcessor::GetInstance().Insert("sc",record_in)<<std::endl;

    //测试select student
    std::string table_name = "student";
    std::vector<std::string> field_name;
    field_name.push_back("*");
    std::vector<std::tuple<std::string, std::string, int>> conditions;
    //conditions.push_back(std::make_tuple("Age","50", kLargerConditon));
    //conditions.push_back(std::make_tuple("Sname","hahaha", kEqualConditon));
    std::vector<std::vector<std::any>> return_records;
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);
    
    //测试select course
    table_name = "course";
    field_name.clear();
    field_name.push_back("*");
    conditions.clear();
    return_records.clear();
    std::cout<<DataProcessor::GetInstance().Select(table_name,field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);

    //测试select sc
    table_name = "sc";
    field_name.clear();
    field_name.push_back("*");
    conditions.clear();
    return_records.clear();
    std::cout<<DataProcessor::GetInstance().Select(table_name,field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);

    //测试select student, course, sc
    std::vector<std::string> table_names;
    
    table_name = "student";
    table_names.push_back(table_name);
    table_name = "course";
    table_names.push_back(table_name);
    table_name = "sc";
    table_names.push_back(table_name);
    field_name.clear();
    
    field_name.push_back("Sname");
    field_name.push_back("Cname");
    field_name.push_back("Grade");
    return_records.clear();
    std::cout<<DataProcessor::GetInstance().Select(table_names,field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);    
    //测试select student, course
        table_names.clear();
        table_name = "student";
        table_names.push_back(table_name);
        table_name = "course";
        table_names.push_back(table_name);
        field_name.clear();
        field_name.push_back("*");
        return_records.clear();
        std::cout<<DataProcessor::GetInstance().Select(table_names,field_name,conditions,return_records)<<std::endl;
        /*for(const auto& record : return_records) {
            for(auto x : record) {
                std::cout<<ColasqlTool::AnyToString(x)<<" ";
            }
            std::cout<<std::endl;
        }
        std::cout<<"end"<<std::endl;*/
        ColasqlTool::OutputSelectResult(return_records);

    //测试update
    // int Update(std::string table_name, const std::vector<std::pair<std::string,std::string>>& value, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
    std::vector<std::pair<std::string,std::string>> values;
    conditions.clear();
    values.push_back({"Age","70"});
    conditions.push_back({"Sno","21301032",kEqualConditon});
    conditions.push_back({"Sname","drj",kNotEqualConditon});
    std::cout<<DataProcessor::GetInstance().Update("student",values,conditions)<<std::endl;
    conditions.clear();
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);

    //测试AlterTable Add
    std::pair<std::string, std::string> new_field = {"Ssex","string"};
    DataProcessor::GetInstance().AlterTableAdd("student", new_field);
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);
    
    //调试AlterTable Modify
    //int AlterTableModify(std::string table_name, std::pair<std::string, std::string> field);
    std::cout<<"here : "<<DataProcessor::GetInstance().AlterTableModify("student",{"Sname", "int"})<<std::endl;
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);
    
    std::cout<<"here : "<<DataProcessor::GetInstance().AlterTableModify("student",{"Age", "string"})<<std::endl;
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);

    //调试AlterTable Drop
    //int AlterTableDrop(std::string table_name, std::string field_name);
    DataProcessor::GetInstance().AlterTableDrop("student", "Age");
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    ColasqlTool::OutputSelectResult(return_records);


    //调试Show Databases
    std::vector<std::string> databases;
    std::cout<<DataProcessor::GetInstance().ShowDatabases(databases)<<std::endl;
    ColasqlTool::OutputSelectResult(ColasqlTool::ChangeStringsToRecords(databases, "DATABASES"));


    //调试Show Tables
    std::vector<std::string> tables;
    std::cout<<DataProcessor::GetInstance().ShowTables(tables)<<std::endl;
    ColasqlTool::OutputSelectResult(ColasqlTool::ChangeStringsToRecords(tables, "TABLES"));

    //调试Desc Tables
    // int DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>& fields,std::vector<Constraint*>&    constraints);
    std::vector<std::pair<std::string, std::string>> table_fields;
    
    std::vector<Constraint*> table_constraints;
    std::cout<<DataProcessor::GetInstance().DescribeTable("student",table_fields,table_constraints)<<std::endl;
    ColasqlTool::OutputSelectResult(ColasqlTool::ChangeDescriptionToRecords(table_fields,table_constraints));
    //调试delete database
    std::cout<<"delete database:"<<DataProcessor::GetInstance().DeleteDatabase("test")<<std::endl;

    return 0;
}