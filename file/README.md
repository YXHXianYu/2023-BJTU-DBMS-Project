# File

* 文件管理系统
* 单例
* 使用C++实现 
  



## 1. 接口

```C++
int CreateUser(const User& user);
int CreateDatabaseDirectory(const DataBase& database);
int CreateTableFile(const Table& table);

int ReadUser(User& user);
int ReadUsers(std::vector<User>& users);
int ReadDatabase(Database& database);
int ReadDatabases(std::vector<Database>& databases);

int WriteUser(User user);
int WriteUsers(std::vector<User> users);
int ReadDatabase(Database& database);
int ReadDatabases(std::vector<Database> databases);

```

