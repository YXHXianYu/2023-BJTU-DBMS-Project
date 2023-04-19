# Core
* 核心模块
* 使用C++实现 



## 1. 类

#### 1.1DataProcessor

​			DataProcessor主要用于与cmd交互，返回数据库操作结果。

* 接口
  ```C++
  //调用单例
  public:
      static DataProcessor GetInstance();
  
  //数据库
  public:
  	//创建用户
      int CreateUser(std::string user_name,
                     std::string user_password);         
  	//登录用户  
      int Login(std::string user_name,
                std::string user_password);                
  //数据库
  public:
   	//创建数据库
      int CreateDatabase(std::string database_name);     
  	//使用数据库                        
      int UseDatabase(std::string database_name);       
  	//显示所有数据库                          
      std::vector<std::string> ShowDatabases();
  
  //表
  public:
  	// 创建表
      int CreateTable(std::string table_name, std::vector<std::pair<std::string, std::string>> fields, std::vector<Constraint *> constraints);       int DropTable(std::string table_name);                                      //删除表
      int DescribeTable(std::string table_name,std::vector<std::pair<std::string, std::string>>&,std::vector<Constraint>&    constraints);                                  //查看表结构
      int ShowTables(std::string table_name);                                     //显示所有表
      int AlterTableAdd(std::string table_name, std::pair<std::string, std::string> field);//修改表结构,增加字段
      int AlterTableDrop(std::string table_name, std::string field_name);          //删除字段
      int AlterTableModify(std::string table_name, std::pair<std::string, std::string> field); //修改字段
      int AlterTableConstraint(std::string table_name, Constraint constraint);    //添加约束条件
      int AlterTableDeleteConstraint(std::string table_name, std::string constraint_name); ///删除约束                                                                                  
  //记录
  public:
  	// 插入记录
      int Insert(std::string table_name,
                 std::vector<std::pair<std::string, 				 std::string>> record_in); 
  	// 查询记录
      int Select(std::string table_name,
                 std::vector<std::string>           					field_name,
                 std::vector<std::tuple<std::string,
                 std::string, int>> conditions,
                 std::vector<std::vector<std::any>> 					&return_records); 
      //删除记录
  	int Delete(std::string table_name,
                 std::vector<std::tuple<std::string, 					std::string, int>> conditions);    
  ```
  
  

#### 1.2 用户(User)

* 构造函数
  ```C++
  private:
      User();
  public:
      User(std::string user_name, std::string user_password);
  ```

* 接口
  ```C++
  public:	
  	//验证密码
      int Identify(std::string password);
      //获取用户名
      std::string GetUserName();
  ```

  

#### 1.3 表

* 构造函数
  ```c++
  public:
  Table(std::string table_name,	//表名
        std::vector<std::pair<std::string,
        	std::string>> fields,	//字段
        std::vector<Constraint*> constraints);//约束
  ```

* 接口
  ```c++
  public:
  	//获取表名
      std::string GetTableName() const;
      //Select查询记录
      int Select(std::vector<std::string> field_name,
                 std::vector<std::tuple<std::string, std::string,int>> conditions,
                 std::vector<std::vector<std::any>> &return_records);
      //Insert插入记录
      int Insert(std::vector<std::pair<std::string, std::string>> record_in);
      //Delete删除记录
      int Delete(std::vector<std::tuple<std::string, std::string, int>> conditions);
      //检查记录是否满足Where条件
      int CheckCondition(const std::unordered_map<std::string, std::any>& record, const std::vector<std::tuple<std::string, std::string, int>>& conditions);
  ```

  
