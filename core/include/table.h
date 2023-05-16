#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <vector>
#include <any>
#include <unordered_map>
#include <memory>

#include "all_constraints.h"
#include "colasqltool.h"
#include "constants.h"
#include "index/index.h"

class Database;

class Table {
private:
    std::string table_name;
    std::vector<std::pair<std::string, std::string>> fields; //pair<字段名, 类型>
    std::unordered_map<std::string, std::string> field_map;
    std::vector<std::unordered_map<std::string, std::any> > records;
    std::vector<Constraint*> constraints;

    std::unique_ptr<Index> index_ptr;

public:
    Table(std::string &table_name);
    Table(const std::string& table_name,
          const std::vector<std::pair<std::string, std::string>>& fields,
          const std::vector<Constraint*>& constraints);

    Table(const std::string& table_name,
          const std::vector<std::pair<std::string, std::string>>& fields,
          const std::vector<Constraint*>& constraints,
          const std::vector<std::unordered_map<std::string, std::any>>& records);
    
    ~Table();                           // 根据零三五法则，需要同时定义析构、拷贝构造、拷贝赋值函数
    Table(const Table&);                // 根据零三五法则，需要同时定义析构、拷贝构造、拷贝赋值函数
    Table& operator = (Table);   // unique_ptr 需要定义 拷贝赋值函数
    friend void swap(Table& s1, Table& s2); // 基于友元函数的copy-and-swap策略
    
    //获取表名
    const std::string& GetTableName() const;
    const std::vector<std::pair<std::string, std::string>>& GetFields() const;
    const std::unordered_map<std::string, std::string> GetFieldMap() const;
    const std::vector<std::unordered_map<std::string, std::any> >& GetRecords() const;
    const std::vector<Constraint*>& GetConstraints() const;
    
    //查询记录
    int Select(std::vector<std::string> field_name,
               std::vector<std::tuple<std::string, std::string,int>> conditions,
               std::vector<std::vector<std::any>> &return_records);
    //插入记录
    int Insert(std::vector<std::pair<std::string, std::string>> record_in, Database* db);
    //删除记录
    int Delete(std::vector<std::tuple<std::string, std::string, int>> conditions, Database* db);
    //检查记录是否满足Where条件
    int CheckCondition(const std::unordered_map<std::string, std::any>& record,
                       const std::vector<std::tuple<std::string, std::string, int>>& conditions);
    //更新记录
    int Update(const std::vector<std::pair<std::string,std::string>>& value,
               const std::vector<std::tuple<std::string, std::string, int>>& conditions, Database* db);

    int DescribeTable(std::vector<std::pair<std::string, std::string>>& fields,
                      std::vector<Constraint*>& constraints);
    
    int CheckDataType(std::string type, std::string value);
    //增加字段
    int AlterTableAdd(std::pair<std::string, std::string> new_field);
    int AlterTableDrop(std::string field_name);
    int AlterTableModify(std::pair<std::string, std::string> field);

    // 建立索引
    int BuildIndex(const std::vector<std::string>& compare_key, int type = kFHQTreapIndex);
    //检查一条记录是否约束
    int CheckConstraint(std::unordered_map<std::string, std::any>& record, Database* db);
    //检查是否被其它表参考
    int CheckBeingRefered(std::unordered_map<std::string, std::any>& record, Database* db);
};

#endif // TABLE_H
