#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_login.h>

#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>

#include "columndialog.h"
#include "dataprocessor.h"

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
  int getLevel(QTreeWidgetItem *item);
  void addTreeItem();
  void setConnectTreeItem(QTreeWidgetItem *item, QString dbName);
 private slots:
  void click_action_database();
  void click_action_table();
  void click_action_column();
  void click_action_row();

 private:
  Ui::MainWindow *ui;
  ui_login *ui_log;
  std::vector<std::string> databases;
};
#endif  // MAINWINDOW_H
