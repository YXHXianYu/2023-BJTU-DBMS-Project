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
              int ret = DataProcessor::GetInstance().UseDatabase(
                  clickedItem->text(column).toStdString());
              qDebug() << "use" + clickedItem->text(column) << ret << endl;
            }
            if (clickedItem == item && getLevel(item) == 1) {
              qDebug() << "You clicked on item: " << clickedItem->text(column);
              clickedItem->setSelected(true);
              // todo
              // show table
            }
          });
}

void MainWindow::addTreeItem() {
  DataProcessor::GetInstance().ShowDatabases(databases);
  ui->treeWidget->clear();
  // 将std::vector<std::pair<std::string,
  // std::string>>中的数据添加到树形控件中
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
    }
  }
}

void MainWindow::click_action_column() {}

void MainWindow::click_action_row() {}
