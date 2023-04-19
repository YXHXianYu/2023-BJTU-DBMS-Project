#include <iostream>
#include "colasqltool.h"
#include "constants.h"
#include "dataprocessor.h"
#include "all_constraints.h"
int main() {
    std::cout<< DataProcessor::GetInstance().CreateUser(std::string("admin"),std::string("123456"))<<std::endl;
    std::cout<< DataProcessor::GetInstance().Login(std::string("admin"),std::string("123456"))<<std::endl;
    std::cout<< DataProcessor::GetInstance().CreateDatabase("test")<<std::endl;
    std::cout<< DataProcessor::GetInstance().CreateDatabase("test2")<<std::endl;
    
    std::cout<<DataProcessor::GetInstance().UseDatabase("test")<<std::endl;
    //测试建表
    std::vector<std::pair<std::string, std::string>> fields;
    fields.push_back({"Sno", "string"});
    fields.push_back({"Sname", "string"});
    fields.push_back({"Age", "int"});
    std::vector<Constraint*> constraints;
    std::cout<<DataProcessor::GetInstance().CreateTable("student",fields,constraints)<<std::endl;
    
    //测试insert
    std::vector<std::pair<std::string, std::string>> record_in;
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
    //测试select
    std::string table_name = "student";
    std::vector<std::string> field_name;
    field_name.push_back("Sno");
    field_name.push_back("Sname");
    field_name.push_back("Age");
    std::vector<std::tuple<std::string, std::string, int>> conditions;
    conditions.push_back(std::make_tuple("Age","50", kLargerConditon));
    conditions.push_back(std::make_tuple("Sname","hahaha", kEqualConditon));
    std::vector<std::vector<std::any>> return_records;
    std::cout<<DataProcessor::GetInstance().Select("student",field_name,conditions,return_records)<<std::endl;
    for(auto x : return_records) {
        for(auto mes : x){
            std::cout<<ColasqlTool::AnyToString(mes)<<" ";
        }
        std::cout<<" "<<std::endl;
    }


    std::vector<std::string> databases;
    std::cout<<DataProcessor::GetInstance().ShowDatabases(databases)<<std::endl;
    return 0;
}