#include "mainwindow.h"

#include <memory>

#include "ui_login.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui_log = new ui_login;
  ui_log->show();

  setWindowTitle("ColaSql");

  //  auto streamBuf = std::make_unique<QTextBrowserStreamBuf>(ui->textBrowser);
  QTextEditStreamBuf *streamBuf = new QTextEditStreamBuf(ui->textEdit_code);

  // 保存原始缓冲区，以便在以后恢复
  //  std::streambuf *originalCoutBuf = std::cout.rdbuf();

  // 重定向 std::cout 到 QTextBrowser
  //  std::cout.rdbuf(streamBuf.get());
  std::cout.rdbuf(streamBuf);

  // 现在，你可以使用 std::cout 输出到 QTextBrowser
  ui->textBrowser->append("Hello, I am Co1aSql!\n");

  connect(ui_log, SIGNAL(login()), this, SLOT(show()));
  connect(ui_log, SIGNAL(login()), this, SLOT(addTreeItems()));
  connect(ui->action_database, SIGNAL(triggered()), this,
          SLOT(click_action_database()));
  connect(ui->action_table, SIGNAL(triggered()), this,
          SLOT(click_action_table()));
  connect(ui->action_column, SIGNAL(triggered()), this,
          SLOT(click_action_column()));
  connect(ui->action_row, SIGNAL(triggered()), this, SLOT(click_action_row()));
  ui->textEdit_code->installEventFilter(this);
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

void MainWindow::ConnectTreeItem(QTreeWidgetItem *item, QString Name) {
  item->setText(0, Name);
  item->setText(1, "");

  connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this,
          [=](QTreeWidgetItem *clickedItem, int column) {
            handleItemDoubleClick(clickedItem, item);
          });
}

void MainWindow::handleItemDoubleClick(QTreeWidgetItem *clickedItem,
                                       QTreeWidgetItem *item) {
  int level = getLevel(clickedItem);

  if (level == 0) {
    deselectOtherTopLevelItems(clickedItem);
  }

  if (clickedItem == item) {
    if (level == 0) {
      handleDatabaseClick(clickedItem);
    } else if (level == 1) {
      handleTableClick(clickedItem);
    }
  }
}

void MainWindow::deselectOtherTopLevelItems(QTreeWidgetItem *clickedItem) {
  for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
    if (item != clickedItem) {
      item->setText(1, "");
    }
  }
}

void MainWindow::handleDatabaseClick(QTreeWidgetItem *clickedItem) {
  qDebug() << "You clicked on item: " << clickedItem->text(0);
  clickedItem->setSelected(true);
  clickedItem->setText(1, "Selected");
  std::string dbName = clickedItem->text(0).toStdString();
  int use_db_ret = DataProcessor::GetInstance().UseDatabase(dbName);
  if (use_db_ret != 0) {
    qDebug() << "when click database Error using database"
             << QString::fromStdString(dbName) << "error code:" << use_db_ret;
    assert(false);
  }
  qDebug() << "use" + clickedItem->text(0) << use_db_ret << endl;
}

void MainWindow::handleTableClick(QTreeWidgetItem *clickedItem) {
  qDebug() << "You clicked on item: " << clickedItem->text(0);
  clickedItem->setSelected(true);
  QTreeWidgetItem *parentItem = clickedItem->parent();
  parentItem->setText(1, "Selected");
  deselectOtherTopLevelItems(parentItem);

  std::string dbName = parentItem->text(0).toStdString();
  int use_db_ret = DataProcessor::GetInstance().UseDatabase(dbName);
  if (use_db_ret != 0) {
    qDebug() << "when click table Error using database"
             << QString::fromStdString(dbName) << "error code:" << use_db_ret;
    assert(false);
  }
  qDebug() << "use" + parentItem->text(0) << use_db_ret << endl;

  std::vector<std::string> fields;
  fields.push_back("*");
  std::string tbName = clickedItem->text(0).toStdString();
  std::vector<std::tuple<std::string, std::string, int>> conditions;
  std::vector<std::vector<std::any>> return_records;
  int select_ret = DataProcessor::GetInstance().Select(
      tbName, fields, conditions, return_records);
  if (select_ret != 0) {
    qDebug() << "when click table select error";
    assert(false);
  }
  std::string ret = ColasqlTool::OutputSelectResult(return_records);
  ui->textBrowser->setText(QString::fromStdString(ret));
}

void MainWindow::addTreeItems() {
  // Load databases
  int ret = DataProcessor::GetInstance().ShowDatabases(databases);
  if (ret != 0) {
    qDebug() << "Error loading databases, error code:" << ret;
    return;
  }

  // Clear the tree widget
  ui->treeWidget->clear();

  // Add databases and tables to the tree widget
  for (const auto &database : databases) {
    ret = DataProcessor::GetInstance().UseDatabase(database);
    if (ret != 0) {
      qDebug() << "Error using database" << QString::fromStdString(database)
               << "error code:" << ret;
      assert(false);
    }

    QTreeWidgetItem *databaseItem = new QTreeWidgetItem(ui->treeWidget);
    ConnectTreeItem(databaseItem, QString::fromStdString(database));

    std::vector<std::string> tables;
    ret = DataProcessor::GetInstance().ShowTables(tables);
    if (ret != 0) {
      qDebug() << "Error loading tables for database"
               << QString::fromStdString(database) << "error code:" << ret;
      assert(false);
    }

    for (const auto &table : tables) {
      QTreeWidgetItem *tableItem = new QTreeWidgetItem(databaseItem);
      ConnectTreeItem(tableItem, QString::fromStdString(table));
    }
  }

  // Reset the current database
  DataProcessor::GetInstance().UseDatabase();
}

void MainWindow::click_action_database() {
  //    新建数据库
  qDebug() << "add database ";
  QString dbName =
      QInputDialog::getText(this, "输入", "数据库名:", QLineEdit::Normal);
  if (dbName.isEmpty()) {
    return;
  }
  qDebug() << dbName;
  int ret = DataProcessor::GetInstance().CreateDatabase(dbName.toStdString());
  if (!ret) {
    // 将数据库添加到treeWidget中
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    ConnectTreeItem(item, dbName);
    QMessageBox::information(this, "通知", "数据库已创建");
  } else if (ret == kDatabaseExisted) {
    QMessageBox::warning(this, "错误", "数据库已存在\n");
    return;
  } else {
    qDebug() << "Error create database" << dbName << "error code:" << ret;
    assert(false);
  }
}

void MainWindow::click_action_table() {
  //    新建表
  qDebug() << "add table\n";
  QString tbName =
      QInputDialog::getText(this, "输入", "表名:", QLineEdit::Normal);
  if (tbName.isEmpty()) {
    return;
  }
  qDebug() << tbName;
  std::vector<std::pair<std::string, std::string>> fields;
  std::vector<Constraint *> constraints;
  int ret = DataProcessor::GetInstance().CreateTable(tbName.toStdString(),
                                                     fields, constraints);
  if (!ret) {
    QTreeWidgetItem *fatherItem = nullptr;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
      QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
      if (item->text(1) == "Selected") {
        fatherItem = item;
        break;
      }
    }
    if (fatherItem == nullptr) {
      QMessageBox::warning(this, "错误", "未选用数据库\n");
      return;
    }
    QTreeWidgetItem *childItem = new QTreeWidgetItem(fatherItem);
    ConnectTreeItem(childItem, tbName);

  } else if (ret == kDatabaseNotUse) {
    QMessageBox::warning(this, "错误", "未选用数据库\n");
    return;
  } else {
    qDebug() << "Error create table" << tbName << "error code:" << ret;
    assert(false);
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

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->textEdit_code && event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    QTextCursor cursor = ui->textEdit_code->textCursor();

    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
      // 当按下回车键时，触发槽函数
      onEnterPressed();
      return true;  // 返回 true 表示事件已被处理
    } else if (keyEvent->key() == Qt::Key_Backspace) {
      // 当按下退格键时
      cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                          prefix.length());
      QString textBeforeCursor = cursor.selectedText();

      if (textBeforeCursor == prefix) {
        return true;  // 如果光标前的字符串等于指定字符串，不允许删除
      } else if (cursor.atBlockStart()) {
        return true;  // 如果光标在行首，不允许删除
      }
    } else if (keyEvent->key() == Qt::Key_Delete) {
      // 当按下删除键时
      if (cursor.atBlockEnd()) {
        return true;  // 如果光标在行尾，不允许删除
      }
    }
  }
  // 对于其他事件和对象，调用基类的事件过滤器
  return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onEnterPressed() {
  QTextCursor cursor = ui->textEdit_code->textCursor();
  cursor.select(QTextCursor::LineUnderCursor);
  QString input = cursor.selectedText().mid(prefix.length());
  qDebug() << "回车键被按下，当前行内容：" << input;
  QString ret = QString::fromStdString(
      ColaSQLCommand::CommandProcessor::GetInstance().Run(input.toStdString()));
  qDebug() << ret;
  if (ret != "") std::cout << ret.toStdString();
  ui->textEdit_code->append("Co1aSQL > ");
}
