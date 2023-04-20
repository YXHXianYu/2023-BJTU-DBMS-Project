#include "mainwindow.h"

#include "ui_login.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui_log = new ui_login;
  ui_log->show();

  setWindowTitle("ColaSql");

  addTreeItem();

  QTextBrowserStreamBuf *streamBuf = new QTextBrowserStreamBuf(ui->textBrowser);

  // 保存原始缓冲区，以便在以后恢复
  //  std::streambuf *originalCoutBuf = std::cout.rdbuf();

  // 重定向 std::cout 到 QTextBrowser
  std::cout.rdbuf(streamBuf);

  // 现在，你可以使用 std::cout 输出到 QTextBrowser
  std::cout << "Hello, I am Co1aSql!";

  connect(ui_log, SIGNAL(login()), this, SLOT(show()));
  connect(ui->action_database, SIGNAL(triggered()), this,
          SLOT(click_action_database()));
  connect(ui->action_table, SIGNAL(triggered()), this,
          SLOT(click_action_table()));
  connect(ui->action_column, SIGNAL(triggered()), this,
          SLOT(click_action_column()));
  connect(ui->action_row, SIGNAL(triggered()), this, SLOT(click_action_row()));
}

MainWindow::~MainWindow() { delete ui; }

int MainWindow::getLevel(QTreeWidgetItem *item) {
  int level = 0;
  QTreeWidgetItem *parentItem = item->parent();
  while (parentItem) {
    level++;
    item = parentItem;
    parentItem = item->parent();
  }
  return level;
}

void MainWindow::setConnectTreeItem(QTreeWidgetItem *item, QString Name) {
  item->setText(0, Name);
  item->setText(1, "");
  connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this,
          [=](QTreeWidgetItem *clickedItem, int column) {
            //            std::cout << "You clicked on item: "
            //                      << clickedItem->text(column).toStdString();
            if (getLevel(clickedItem) == 0) {
              for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
                QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
                // 将其他数据库的第二列设置为空
                if (item != clickedItem) {
                  item->setText(1, "");
                }
              }
            }
            if (clickedItem == item && getLevel(item) == 0) {
              // 如果点击此item则use this database
              qDebug() << "You clicked on item: " << clickedItem->text(column);
              clickedItem->setSelected(true);
              clickedItem->setText(1, "Selected");
              std::string dbName = clickedItem->text(0).toStdString();
              int ret = DataProcessor::GetInstance().UseDatabase(dbName);
              qDebug() << "use" + clickedItem->text(0) << ret << endl;
            }
            if (clickedItem == item && getLevel(item) == 1) {
              // 点击table
              qDebug() << "You clicked on item: " << clickedItem->text(column)
                       << column;
              clickedItem->setSelected(true);
              QTreeWidgetItem *parentItem = item->parent();
              parentItem->setText(1, "Selected");
              for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
                QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
                // 将其他数据库的第二列设置为空
                if (item != parentItem) {
                  item->setText(1, "");
                }
              }
              std::string dbName = parentItem->text(0).toStdString();
              int use_db_ret = DataProcessor::GetInstance().UseDatabase(dbName);
              qDebug() << "use" + parentItem->text(0) << use_db_ret << endl;
              std::vector<std::string> fields;
              fields.push_back("*");
              std::string tbName = clickedItem->text(0).toStdString();
              std::vector<std::tuple<std::string, std::string, int>> conditions;
              std::vector<std::vector<std::any>> return_records;
              DataProcessor::GetInstance().Select(tbName, fields, conditions,
                                                  return_records);
              //              qDebug() << "im here1" << endl;
              std::string ret = ColasqlTool::OutputSelectResult(return_records);
              //              qDebug() << "im here2" << endl;
              //              ui->textBrowser->append(QString::fromStdString(ret));
              //              ui->textBrowser->setText(QString::fromStdString(ret));
              //              qDebug() << "im here3" << endl;
            }
          });
}

void MainWindow::addTreeItem() {
  // 将databases中的数据添加到树形控件中
  DataProcessor::GetInstance().ShowDatabases(databases);
  ui->treeWidget->clear();
  for (const auto &database : databases) {
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    setConnectTreeItem(item, QString::fromStdString(database));
  }
}

void MainWindow::click_action_database() {
  //    新建数据库
  qDebug() << "add database\n";
  QString dbName =
      QInputDialog::getText(this, "输入", "数据库名:", QLineEdit::Normal);
  if (dbName.isEmpty()) {
    return;
  } else {
    qDebug() << dbName;
    int ret = DataProcessor::GetInstance().CreateDatabase(dbName.toStdString());
    if (!ret) {
      // 将数据库添加到treeWidget中
      QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
      setConnectTreeItem(item, dbName);
      QMessageBox::information(this, "通知", "数据库已创建");
    } else if (ret == kDatabaseExisted) {
      QMessageBox::warning(this, "错误", "数据库已存在\n");
      return;
    }
  }
}

void MainWindow::click_action_table() {
  //    新建表
  qDebug() << "add table\n";
  QString tbName =
      QInputDialog::getText(this, "输入", "表名:", QLineEdit::Normal);
  if (tbName.isEmpty()) {
    return;
  } else {
    qDebug() << tbName;
    std::vector<std::pair<std::string, std::string>> fields;
    std::vector<Constraint *> constraints;
    int ret = DataProcessor::GetInstance().CreateTable(tbName.toStdString(),
                                                       fields, constraints);
    qDebug() << ret << endl;
    if (!ret) {
      QTreeWidgetItem *faterItem;
      for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        if (item->text(1) == "Selected") {
          faterItem = item;
          break;
        }
      }
      QTreeWidgetItem *childItem = new QTreeWidgetItem(faterItem);
      setConnectTreeItem(childItem, tbName);

    } else if (ret == kDatabaseNotUse) {
      QMessageBox::warning(this, "错误", "未选用数据库\n");
      return;
    } else {
      qDebug() << ret << endl;
    }
  }
}

void MainWindow::click_action_column() {
  qDebug() << "add column\n";
  ColumnDialog dialog;
  if (dialog.exec() == QDialog::Accepted) {
    QString tbName = dialog.lineEdit1->text();
    QString columnName = dialog.lineEdit2->text();
    QString columnType = dialog.lineEdit3->text();
    columnType = columnType.toLower();
    qDebug() << QString("Info1: %1, Info2: %2, Info3: %3")
                    .arg(tbName)
                    .arg(columnName)
                    .arg(columnType);
    if (columnType != "int" && columnType != "float" &&
        columnType != "string") {
      QMessageBox::warning(this, "错误", "类型只能为int、float或string\n");
      return;
    }
    std::pair<std::string, std::string> field;
    field = std::make_pair(columnName.toStdString(), columnType.toStdString());
    int ret =
        DataProcessor::GetInstance().AlterTableAdd(tbName.toStdString(), field);
    if (!ret) {
      QMessageBox::information(this, "通知", "新建列成功\n");
    } else if (ret == kDatabaseNotUse) {
      QMessageBox::warning(this, "错误", "未选用数据库\n");
    } else if (ret == kTableNotFound) {
      QMessageBox::warning(this, "错误", "未找到该表\n");
    }
  }
}

void MainWindow::click_action_row() {
  qDebug() << "add row\n";
  QString tbName =
      QInputDialog::getText(this, "输入", "表名:", QLineEdit::Normal);
  QStringList fieldName =
      QInputDialog::getText(this, "输入",
                            "请输入要插入的字段名，每个字段用','隔开",
                            QLineEdit::Normal, "Info1,Info2,......")
          .split(",");
  QStringList input =
      QInputDialog::getText(this, "输入", "请输入一条记录，每个字段用','隔开",
                            QLineEdit::Normal, "Info1,Info2,......")
          .split(",");
  qDebug() << input << endl;
  std::vector<std::pair<std::string, std::string>> record_in;
  if (fieldName.at(0) == '*') {
    for (int i = 0; i < input.length(); ++i) {
      record_in.push_back({"*", input.at(i).toStdString()});
    }
  } else if (fieldName.length() == input.length()) {
    for (int i = 0; i < input.length(); ++i) {
      record_in.push_back(
          {fieldName.at(i).toStdString(), input.at(i).toStdString()});
    }
  } else {
    QMessageBox::warning(this, "错误", "插入字段与输入数据长度不同\n");
    return;
  }
  int ret =
      DataProcessor::GetInstance().Insert(tbName.toStdString(), record_in);
  if (!ret) {
    QMessageBox::information(this, "通知", "新建记录成功\n");
    return;
  } else if (ret == kDatabaseNotUse) {
    QMessageBox::warning(this, "错误", "未选用数据库\n");
    return;
  } else if (ret == kTableNotFound) {
    QMessageBox::warning(this, "错误", "未找到该表\n");
    return;
  } else if (ret == kDataTypeWrong) {
    QMessageBox::warning(this, "错误", "数据与字段类型不符\n");
    return;

  } else if (ret == kFieldNotFound) {
    QMessageBox::warning(this, "错误", "未找到该字段\n");
    return;
  }
}
