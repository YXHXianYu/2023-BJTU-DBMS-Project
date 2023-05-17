#include "filemanager.h"

#include <cassert>
#include <fstream>

#ifdef __WIN32__
#include <direct.h>
void myMkdir(const std::string& path) { mkdir(path.c_str()); }
#elif __linux__
#include <sys/stat.h>
#include <sys/types.h>
void myMkdir(const std::string& path) {
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
}
#endif

#include "colasqltool.h"

FileManager::FileManager() {}

FileManager& FileManager::GetInstance() {
  static FileManager instance;
  return instance;
}

// ----- Write -----

int FileManager::WriteDatabasesFile(const std::vector<Database>& databases) {
  // 创建新文件夹
  myMkdir("./data");

  // c++ ofstream 必须要目录存在才可以创建新文件夹
  std::ofstream out("./data/databases.txt",
                    std::ofstream::out | std::ofstream::trunc);

  // out
  if (out.is_open()) {
    out << databases.size() << std::endl;
    for (const auto& database : databases) {
      out << database.GetDatabaseName() << " " << database.GetOwnerUserName()
          << std::endl;
    }
    out.close();
  }

  return 0;
}

// *: tables.txt, table.txt, tableRecord.txt, tableConstraint.txt,
// tableIndex.txt
int FileManager::WriteTablesFile(const std::string& databaseName,
                                 const std::vector<Table>& tables) {
  std::string path = "./data/" + databaseName + "/";

  // mkdir
  myMkdir(path.c_str());

  // tables.txt
  std::ofstream out(path + "tables.txt",
                    std::ofstream::out | std::ofstream::trunc);
  if (!out.is_open()) assert(false);

  out << tables.size() << std::endl;
  for (const auto& table : tables) {
    out << table.GetTableName() << std::endl;
  }
  out.close();

  // each table
  for (const auto& table : tables) {
    // table.txt
    out.open(path + table.GetTableName() + ".txt",
             std::ofstream::out | std::ofstream::trunc);
    if (!out.is_open()) assert(false);

    const std::vector<std::pair<std::string, std::string>>& fields =
        table.GetFields();
    out << fields.size() << std::endl;
    for (const auto& [name, type] : fields) {
      out << name << " " << type << std::endl;
    }
    out.close();

    // tableRecords.txt
    out.open(path + table.GetTableName() + "Records.txt",
             std::ofstream::out | std::ofstream::trunc);
    if (!out.is_open()) assert(false);

    const std::vector<std::unordered_map<std::string, std::any>>& records =
        table.GetRecords();
    out << records.size() << std::endl;
    for (const auto& record : records) {
      out << record.size() << " ";
      for (const auto& [name, value] : record) {
        out << name << " " << ColasqlTool::AnyToString(value) << " ";
      }
      out << std::endl;
    }
    out.close();

    // tableConstraints.txt
    out.open(path + table.GetTableName() + "Constraints.txt",
             std::ofstream::out | std::ofstream::trunc);
    if (!out.is_open()) assert(false);

    out << ColasqlTool::ConstraintsToString(table.GetConstraints()) << std::endl;
    out.close();

    // tableIndexes.txt
    out.open(path + table.GetTableName() + "Indexes.txt",
             std::ofstream::out | std::ofstream::trunc);
    if (!out.is_open()) assert(false); 

    // TODO;
    out.close();
  }

  return 0;
}

int FileManager::WriteUsersFile(const std::vector<User>& users) {
  // 创建新文件夹
  myMkdir("./data");

  // c++ ofstream 必须要目录存在才可以创建新文件夹
  std::ofstream out("./data/users.txt",
                    std::ofstream::out | std::ofstream::trunc);

    auto withNull = [&](const std::string& str) {
        if(str == "") return "NULL";
        else return str.c_str();
    };

  // out
  if (out.is_open()) {
    out << users.size() << std::endl;
    for (const auto& user : users) {
      out << user.GetUserName() << " " << user.GetUserPassword() << " ";
      
      int n = user.GetAuthorities().size();
      out << n << " ";
      for(const auto& privi: user.GetAuthorities()) {
        out << withNull(privi.database_name) << " " << withNull(privi.table_name) << " " << privi.number << " ";
      }
      out << std::endl;
    }
    out.close();
  }

  return 0;
}

int FileManager::WriteLogFile(const std::string& databaseName,
                              const std::string& log) {
  return -1;
}

// ----- Read -----

int FileManager::ReadDatabasesFile(std::vector<Database>& databases) {
  // c++ ifstream
  std::ifstream in("./data/databases.txt", std::ifstream::in);

  // in
  databases.clear();

  if (!in.is_open()) return -1;

  int n;
  in >> n;
  for (int i = 1; i <= n; i++) {
    std::string databaseName;
    std::string ownerUser;
    in >> databaseName >> ownerUser;
    databases.push_back(Database(databaseName, ownerUser));
  }

  return 0;
}

int FileManager::ReadTablesFile(const std::string& databaseName,
                                std::vector<Table>& tables) {
  std::string path = "./data/" + databaseName + "/";

  tables.clear();

  std::vector<std::string> tableNames;
  int n, m;

  // tables.txt
  std::ifstream in(path + "tables.txt", std::ifstream::in);
  if (!in.is_open()) assert(false);

  in >> n;
  for (int i = 1; i <= n; i++) {
    std::string tableName;
    in >> tableName;
    tableNames.push_back(tableName);
  }
  in.close();

  // each table
  for (const auto& tableName : tableNames) {
    std::vector<std::pair<std::string, std::string>> fields;
    std::unordered_map<std::string, std::string> field_map;
    std::vector<std::unordered_map<std::string, std::any>> records;
    std::vector<Constraint*> constraints;
    // std::vector .... indexes;

    // table.txt
    in.open(path + tableName + ".txt", std::ifstream::in);
    if (!in.is_open()) assert(false);

    in >> n;
    for (int i = 1; i <= n; i++) {
      std::string name;
      std::string type;
      in >> name >> type;
      fields.push_back({name, type});
      field_map[name] = type;
    }
    in.close();

    // tableRecords.txt
    in.open(path + tableName + "Records.txt", std::ifstream::in);
    if (!in.is_open()) assert(false);

    in >> n;
    for (int i = 1; i <= n; i++) {
      std::unordered_map<std::string, std::any> record;
      std::string name;
      std::string value;
      in >> m;
      for (int j = 1; j <= m; j++) {
        in >> name >> value;
        record[name] =
            ColasqlTool::GetAnyByTypeAndValue(field_map[name], value);
      }
      records.push_back(record);
    }
    in.close();

    // tableConstraints.txt
    in.open(path + tableName + "Constraints.txt", std::ifstream::in);
    if (!in.is_open()) assert(false);

        in >> n;
        for(int i = 1; i <= n; i++) {
            int type;
            std::string fieldName;
            in >> type >> fieldName;
            if(type == kPrimaryKey) {
                constraints.push_back(new PrimaryKeyConstraint(fieldName));
            } else if(type == kForeignKey) {
                std::string s1, s2;
                in >> s1 >> s2;
                constraints.push_back(new ForeignKeyConstraint(fieldName, s1, s2));
            } else if(type == kForeignRefered) {
                std::string s1, s2;
                in >> s1 >> s2;
                constraints.push_back(new ForeignReferedConstraint(fieldName, s1, s2));
            } else if(type == kUnique) {
                constraints.push_back(new UniqueConstraint(fieldName));
            } else if(type == kNotNull) {
                constraints.push_back(new NotNullConstraint(fieldName));
            } else if(type == kDefault) {
                std::string value;
                in >> value;
                constraints.push_back(new DefaultConstraint(fieldName, ColasqlTool::GetAnyByTypeAndValue(field_map.at(fieldName), value)));
            } else {
                std::cerr << "Unknown Constraint Type" << std::endl;
            }
        }

        // ColasqlTool::OutputConstraints(constraints);


    // tableIndexes.txt
    in.open(path + tableName + "Indexes.txt", std::ifstream::in);
    if (!in.is_open()) assert(false);

    // TODO

    // construct table
    Table table(tableName, fields, constraints, records);
    tables.push_back(table);
  }

  return 0;
}

int FileManager::ReadUsersFile(std::vector<User>& users) {
  // c++ ifstream
  std::ifstream in("./data/users.txt", std::ifstream::in);

  // in
  users.clear();

  if (!in.is_open()) return -1;

    auto makeNull = [&](std::string& str) {
        if(str == "NULL") str = "";
    };

  int n;
  in >> n;
  for (int i = 1; i <= n; i++) {
    std::string name;
    std::string password;
    int m;
    in >> name >> password >> m;
    std::vector<priviledge> authorities;
    authorities.resize(m);
    for(int i = 0; i < m; i++) {
        priviledge privi;
        int x;
        in >> privi.database_name >> privi.table_name >> x;
        makeNull(privi.database_name);
        makeNull(privi.table_name);
        privi.number = authority_number(x);
        authorities[i] = privi;
    }
    users.push_back(User(name, password, authorities));
  }

  return 0;
}

int FileManager::ReadLogFile(const std::string& databaseName,
                             std::string& log) {
  return -1;
}
