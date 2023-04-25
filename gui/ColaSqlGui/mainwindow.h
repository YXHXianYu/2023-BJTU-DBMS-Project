#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_login.h>

#include <QDebug>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextEdit>
#include <QTreeWidget>

#include "columndialog.h"
#include "command_processor.h"
#include "dataprocessor.h"
#include "qtbstreambuf.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  // 返回item在树状结构的第几层
  int getLevel(QTreeWidgetItem *item);
  // 给item设置connect
  void ConnectTreeItem(QTreeWidgetItem *item, QString Name);
  // 处理双击树形结构
  void handleItemDoubleClick(QTreeWidgetItem *clickedItem,
                             QTreeWidgetItem *item);
  // 处理双击数据库
  void handleDatabaseClick(QTreeWidgetItem *clickedItem);
  // 处理双击表
  void handleTableClick(QTreeWidgetItem *clickedItem);
  // 清空选择的数据库
  void deselectOtherTopLevelItems(QTreeWidgetItem *clickedItem);

 private slots:
  void click_action_database();
  void click_action_table();
  void click_action_column();
  void click_action_row();
  void addTreeItems();
  void onEnterPressed();

 protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

 private:
  Ui::MainWindow *ui;
  ui_login *ui_log;
  std::vector<std::string> databases;
  QString prefix = "Co1aSQL > ";
};
#endif  // MAINWINDOW_H
