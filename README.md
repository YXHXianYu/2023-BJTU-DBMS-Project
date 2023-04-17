# 2023-BJTU-DBMS-Project
> 北京交通大学软件学院，2023年春季的实训课项目，数据库管理系统

### 1. SQL语法

#### 1.1 数据类型

* ```mysql
  int    # 32位整数，如 1, 14, 514
  float  # 32位浮点数，如 1.0, 5.1, 2
  string # 字符串，如 "dbms", "2002-10-12"
  ```

#### 1.2 数据库

* 创建数据库

  ```mysql
  CREATE DATABASE 数据库名;
  ```

* 删除数据库

  ```mysql
  DROP DATABASE 数据库名;
  ```

* 使用数据库

  ```mysql
  USE 数据库名;
  ```

#### 1.3 表

* 创建表

  ```mysql
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
  
    ```mysql
  CONSTRAINT PRIMARY KEY 列名;                # 指定主键
  CONSTRAINT FOREIGN KEY 列名 REFERENCES 表名2 列名2; # 指定外键，该列 参考 表2的列2
  CONSTRAINT UNIQUE 列名;                     # 指定该列值不可重复（未指定时，默认可重）
  CONSTRAINT NOT NULL 列名;                   # 指定该列非空（未指定时，默认可空）
  CONSTRAINT DEFAULT 列名 值;                  # 指定该列默认值（未指定时，默认值为NULL）
  
* 删除表

  ```mysql
  DROP TABLE 表名;
  ```

* 修改表

  ```mysql
  ALTER TABLE 表名 ADD 列名 类型;
  ALTER TABLE 表名 DROP 列名;
  ALTER TABLE 表名 MODIFY 列名 类型;
  ```

* 查看表结构

  ```mysql
  DESC TABLE 表名;
  ```

#### 1.4 记录

* 插入记录

  ```mysql
  INSERT INTO 表名 列名1 列名2 列名3 ... VALUES 值1 值2 值3 ...;
  ```

* 删除记录

  ```mysql
  DELETE FROM 表名 [WHERE 筛选条件1 筛选条件2 ...];
  ```
  
  * 筛选条件语法
  
    ```mysql
    列名 = 值
    ```
    
    * **筛选条件'='左右必须要包含空格**，否则会产生编译错误
  
* 选择记录

  ```mysql
  SELECT 列名1 列名2 列名3 ...
  FROM 表名1 [NATURAL JOIN 表名2 ...]
  [WHERE 筛选条件1 筛选条件2 ...];
  ```

* 更新记录

  ```mysql
  UPDATE 表名 SET 列名1 = 值1 [列名2 = 值2 ...]
  [WHERE 筛选条件1 筛选条件2 ...];
  ```

### 2. C++函数接口
