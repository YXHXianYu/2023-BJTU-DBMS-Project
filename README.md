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

* 读取SQL脚本文件，解析并批量执行
  * YXH

* 复杂的SQL查询（多表多条件）
  * DRJ

* 事务管理
* 索引管理
  * YXH

* 完整性管理（主键、外键、UNIQUE、NOT NULL、DEFAULT）
  * DRJ

* 安全性管理（用户、权限）
  * DRJ


***

### 2. SQL语法

#### 2.1 基本约定

* Co1aSQL 支持的SQL语法，均为 **大小写不敏感**。

#### 2.2 用户

* 登入数据库

  ```sql
  colasql 用户名 密码
  
  #管理员用户名admin，密码colasql
  ```
  
* 创建用户

  ```sql
  CREATE USER 用户名 密码;
  #所有用户默认拥有数据库的查询权、创建权，只对自己创建的数据库有删除权
  #创建用户不需要任何权限，但授予权限只能授予自己拥有的权限
  #其它说明见下
  ```
  
* 修改用户权限

  ```sql
  GRANT USER 用户名 数据库名 表名 权限名;
  #数据库名和表名都不能为"*"，对某个特定的数据库的表授予权限。
  
  GRANT USER 用户名 数据库名 权限名;
  
  #todo: admin用户如何创建，可否登录时指定或由初始admin用户指定。
  
  #数据库名为"*"，对所有数据库的所有表授予权限(当前用户拥有该数据库的权限,仅admin可用)。
  #数据库名不为"*"时，若为表级权限则授予该数据库里所有表的表级权限(当前用户需拥有授权的所有权限)，若为数据库级权限则授予该数据库的数据库级权限(当前用户需拥有该权限)。
  ```

  * 权限名

    ```sql
    #表级权限
    SELECT
    DELETE
    INSERT
    UPDATE
    INDEX
    ALTER
    
    #数据库级权限
    CREATE
    DROP
    
    #ALL表示以上所有权限,仅数据库管理员(admin)可用
    ALL
    
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

* 创建表(需要CREATE权限)

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
  
* 删除表(需要DROP权限)

  ```sql
  DROP TABLE 表名;
  ```

* 修改表(需要ALTER权限)

  ```sql
  ALTER TABLE 表名 ADD 列名 类型;
  ALTER TABLE 表名 DROP 列名;
  ALTER TABLE 表名 MODIFY 列名 类型;
  ALTER TABLE 表名 约束条件;
  ALTER TABLE 表名 DELETE 约束条件名;
  ```

  * 约束条件语法见上文
  * 约束条件名可以通过“查看表结构”语句来查看。

* 查看表结构(需要SELECT权限)

  ```sql
  DESC TABLE 表名;
  ```
  
* 显示所有表

  ```sql
  SHOW TABLES;
  ```

* 索引相关

  ```sql
  CREATE INDEX ON 表名 列名1 列名2 ...
  ```

  * 以上述列名为关键字建立索引，在Select时Where列需为此处列的前缀才可以触发索引。

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

#### 2.7 文件

* 储存 & 提交

  ```mysql
  SAVE;
  COMMIT;
  ```

* 回滚 & 读取

  ```mysql
  READ;
  ROLLBACK;
  ```

#### 2.8 关键字

* 以下内容为Co1aSQL关键字，若占用，则不一定能得到预期结果
  * ```
  NULL
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
  // 属性（字段）
  std::pair<std::string, std::string> field;
  std::unordered_map<std::string, std::any> records;
  
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
                  const std::vector<std::pair<std::string, std::any>>& fields,
                  const std::vector<Constraint*>& constraints);
  
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

***

### 4. TODO Lists

* DELETE全表后，不一定返回empty set的问题。
* add foreign key后rollback发生re
* 外键建完commit之后，下次打开gui无法显示
