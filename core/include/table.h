/**
 * author: drj
 * 表类。存储了所有表里的记录，和字段信息、约束等。
 * 主要由database调用，正确时返回kSuccess。
 * 否则返回./constrants.h里的错误代码
 * 本模块中，记录的存储方式是std::vector<std::unordered_map<std::string, std::any> > records，一条记录的类型是std::unordered_map<std::string, std::any>，其中string为记录对应字段，any是记录在对应字段的值。
 * 单行单列交叉处的数据以STL中的any存储，在colasqltool类里有对any进行转换、修改、输出等操作的方法。
*/
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

    Index* index_ptr;

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

    int GetIndex(std::vector<std::string>& result_key) const;
    
    //查询记录
    int Select(std::vector<std::string> field_name, // field_name不为常引用的原因是，函数内对field_name进行了修改
               const std::vector<std::tuple<std::string, std::string,int>>& conditions,
               std::vector<std::vector<std::any>> &return_records,
               const std::vector<std::string>& orderby_key = std::vector<std::string>());
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
    //描述表（字段信息、约束等）
    int DescribeTable(std::vector<std::pair<std::string, std::string>>& fields,
                      std::vector<Constraint*>& constraints);
    
    int CheckDataType(std::string type, std::string value);
    //增加字段
    int AlterTableAdd(std::pair<std::string, std::string> new_field);
    int AlterTableDrop(std::string field_name, Database* db);
    int AlterTableModify(std::pair<std::string, std::string> field);
    int AlterTableConstraint(Constraint* constraint);
    // 建立索引
    int BuildIndex(const std::vector<std::string>& compare_key, int type = kFHQTreapIndex);
    //检查一条记录是否约束
    int CheckConstraint(std::unordered_map<std::string, std::any>& record, Database* db);
    int CheckConstraint(std::unordered_map<std::string, std::any>& record, Database* db, std::vector<std::unordered_map<std::string, std::any> > records, int current_record_order);
    //检查是否被其它表参考
    int CheckBeingRefered(std::unordered_map<std::string, std::any>& record, Database* db);
    //查找字段，找到返回kSuccess
    int FindField(std::string field_name) const;
    int FindField(std::string field_name, std::any value) const;
    //根据表名来删除外键约束
    int DropForeignReferedConstraint(std::string table_name);
    //根据表名和字段名来删除外键约束。
    int DropForeignReferedConstraint(std::string table_name, std::string field_name);

    int DeleteConstraint(std::string constraint_name, Database* db);
    int CheckUnique(std::string field_name) const;
};

#endif // TABLE_H
