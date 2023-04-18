# Co1aSQL
> 2023-BJTU-DBMS-Project
>
> 北京交通大学软件学院 2023年春季 实训课项目 数据库管理系统

### 1. 简介

#### 1.1 软件介绍

* 软件名称：**Co1aSQL** —— 数据库管理系统
* 我们需要实现一个数据库管理系统，支持数据库的创建、数据库表的管理、记录的添加与查询、完整性 约束的实现，索引的创建与实现，多用户等功能。
* 数据库管理系统是一种操纵和管理数据库的大型软件，用于建立、使用和维护数据库。它对数据库进行 统一的管理和控制，以保证数据库的安全性和完整性。用户通过数据库管理系统访问数据库中的数据， 数据库管理员也通过数据库管理系统进行数据库的维护工作。它可使多个应用程序和用户用不同的方法 在同时或不同时刻去建立，修改和询问数据库。
* 我们也支持用户使用SQL语句来控制数据库，包括DDL、DML、DCL，供用户定义数据库的模式结构与 权限约束，实现对数据的追加、删除等操作。

#### 1.2 基本功能

* 创建、删除数据库
* 创建、修改、删除表
* 插入、更新、查询、删除记录
* 三种数据类型：int, float, string
* 数据库文件

#### 1.3 拓展功能

* 索引管理
* 完整性管理（主键、外键、UNIQUE、NOT NULL、DEFAULT）
* 安全性管理（用户、权限）
* 数据库的备份与恢复：日志、备份功能、一个单独的恢复应用
* 跨表查询

***

### 2. SQL语法

#### 2.1 基本约定

* Co1aSQL 支持的SQL语法，均为 **大小写不敏感**。

#### 2.2 用户

* 登入数据库

  ```sql
  colasql 用户名 密码
  ```
  
* 创建用户（需要管理员权限）

  ```sql
  CREATE USER 用户名 密码 数据库名 权限名;
  # 数据库名: 用户拥有权限的数据库；若数据库名="ALL"，则会拥有所有数据库的权限
  ```

* 修改用户权限（需要管理员权限）

  ```sql
  GRANT 用户名 数据库名 权限名;
  ```

  * 权限名

    ```sql
    ALL   # 允许查询表和修改表
    QUERY # 只允许查询表
    ```

* 删除用户（需要管理员权限）

  ```sql
  DROP USER 用户名;
  ```

#### 2.3 数据类型

* ```sql
  int    # 32位整数，如 1, 14, 514
  float  # 32位浮点数，如 1.0, 5.1, 2
  string # 字符串，如 "dbms", "2002-10-12"
  ```

#### 2.4 数据库

* 创建数据库（需要管理员权限）

  ```sql
  CREATE DATABASE 数据库名;
  ```

* 删除数据库（需要管理员权限）

  ```sql
  DROP DATABASE 数据库名;
  ```

* 使用数据库

  ```sql
  USE 数据库名;
  ```
  
* 显示所有数据库

  ```sql
  SHOW DATABASES;
  ```

#### 2.5 表

* 创建表

  ```sql
  CREATE TABLE 表名
    列名1 类型1
    列名2 类型2
    ...
    
    约束条件1
    约束条件2
    ...
  ;
  ```

  * 约束条件语法
  
    ```sql
    CONSTRAINT PRIMARY KEY 列名;                # 指定主键
    CONSTRAINT FOREIGN KEY 列名 REFERENCES 表名2 列名2; # 指定外键，该列 参考 表2的列2
    CONSTRAINT UNIQUE 列名;                     # 指定该列值不可重复（未指定时，默认可重）
    CONSTRAINT NOT NULL 列名;                   # 指定该列非空（未指定时，默认可空）
    CONSTRAINT DEFAULT 列名 值;                  # 指定该列默认值（未指定时，默认值为NULL）
    ```
  
* 删除表

  ```sql
  DROP TABLE 表名;
  ```

* 修改表

  ```sql
  ALTER TABLE 表名 ADD 列名 类型;
  ALTER TABLE 表名 DROP 列名;
  ALTER TABLE 表名 MODIFY 列名 类型;
  ALTER TABLE 表名 约束条件;
  ALTER TABLE 表名 DELETE 约束条件名;
  ```

  * 约束条件语法见上文
  * 约束条件名可以通过“查看表结构”语句来查看。

* 查看表结构

  ```sql
  DESC TABLE 表名;
  ```
  
* 显示所有表

  ```sql
  SHOW TABLES;
  ```

#### 2.6 记录

* 插入记录

  ```sql
  INSERT INTO 表名 列名1 列名2 列名3 ... VALUES 值1 值2 值3 ...;
  ```

* 删除记录

  ```sql
  DELETE FROM 表名 [WHERE 筛选条件1 筛选条件2 ...];
  ```
  
  * 筛选条件语法
  
    ```sql
    列名 = 值
    ```
    
    * **筛选条件'='左右必须要包含空格**，否则会产生编译错误
  
* 选择记录

  ```sql
  SELECT 列名1 列名2 列名3 ...
  FROM 表名1 [NATURAL JOIN 表名2 ...]
  [WHERE 筛选条件1 筛选条件2 ...]
  [ORDER BY 列名1 列名2 ...];
  ```

* 更新记录

  ```sql
  UPDATE 表名 SET 列名1 = 值1 [列名2 = 值2 ...]
  [WHERE 筛选条件1 筛选条件2 ...];
  ```

***

### 3. C++函数接口

#### 3.1 基本约定

* 异常
  * 不使用异常，而是使用 函数返回值 来表示执行状态，用 assert断言 来处理一些重大异常。
  * 若函数返回值为0，则表示操作成功；若函数返回值不为0，则表示操作失败，函数返回值表示失败原因；
* 此处只规定接口的大致形式，细节不做规定，可 **自由发挥**。
  * 比如，可以把以下接口置于不同类中；
  * 比如，可以让 使用数据库(登录)方法 返回一个 数据库对象，通过该对象来操作数据库。

#### 3.2 数据载体

* ```c++
  // 旧版本
  // Struct Field {
  //     std::string name;
  //     std::any value; // any见https://en.cppreference.com/w/cpp/utility/any
  // };
  
  // 属性（字段）
  //  - 该类既可以表示属性的类型，也可以表示属性的值
  Class Field { std::string name; };
  Class IntField: public Field { int value; };
  Class FloatField: public Field { float value; };
  Class StringField: public Field { std::string value; };
  
  // 约束条件
  Class Constraint {};
  Class PrimaryKeyConstraint: public Constraint {};
  Class ForeignKeyConstraint: public Constraint {};
  Class UniqueConstraint: public Constraint {};
  Class NotNullConstraint: public Constraint {};
  Class DefaultConstraint: public Constraint {};
  ```

#### 3.3 用户

* ```c++
  // 创建用户（需要管理员权限）
  int CreateAccount(std::string accountName,
                    std::string password,
                    bool ownAllDatabases,
                    std::string databaseName,
                    int authority);
  
  // 删除用户（需要管理员权限）
  int DeleteAccount(std::string accountName);
  
  // 修改用户权限
  int GrantAccount(std::string accountName,
                   bool ownAllDatabases,
                   std::string databaseName,
                   int authority);
  ```

#### 3.4 数据库

* ```c++
  // 创建数据库（需要管理员权限）
  int CreateDatabase(std::string databaseName);
  
  // 删除数据库（需要管理员权限）
  int DeleteDatabase(std::string databaseName);
  
  // 使用数据库 —— 该函数会对其他函数产生影响
  int UseDatabase(std::string databaseName,
                  std::string accountName,
                  std::string password);
  
  // 显示所有数据库
  int ShowDatabase(std::string& output);
  ```

#### 3.5 表

* ```c++
  // 创建表
  int CreateTable(std::string tableName,
                  const std::vector<Field>& fields,
                  const std::vector<Constraint>& constraints);
  
  // 删除表
  int DeleteTable(std::string tableName);
  
  // 修改表
  int AlterTableAdd(std::string tableName, Field field);
  int AlterTableDrop(std::string tableName, std::string fieldName);
  int AlterTableModify(std::string tableName, std::string fieldName, Field field);
  int AlterTableConstraint(std::string tableName, Constraint constraint);
  int AlterTableDeleteConstraint(std::string tableName, std::string constraintName);
  
  // 查看表结构（通过&output来输出结果）
  int QueryTable(std::string tableName, std::string& output) const;
  
  // 显示当前数据库的所有表
  int ShowTable(std::string& output);
  ```

#### 3.6 记录

* ```c++
  // 插入记录
  int InsertRecord(std::string tableName,
                   const std::vector<Field>& values);
  
  // 删除记录（conditions储存筛选条件，根据筛选条件删除记录）
  int DeleteRecord(std::string tableName,
                   const std::vector<Field>& conditions);
  
  // SelectRecord的参数需要修改！！！目前不完善！！！
  // 选择(查询)记录（conditions储存筛选条件）
  int SelectRecord(std::string tableName,
                   const std::vector<Field>& conditions,
                   std::string& output) const;
  
  // 更新记录
  int UpdateRecord(std::string tableName,
                   const std::vector<Field>& values,
                   const std::vector<Field>& conditions);
  ```

