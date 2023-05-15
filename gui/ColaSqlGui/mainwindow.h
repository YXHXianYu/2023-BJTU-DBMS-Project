#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_login.h>

#include <QDebug>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QTreeWidget>

#include "columndialog.h"
#include "command_processor.h"
#include "createtable.h"
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
  QString anyToQString(const std::any &value);

 private slots:
  void click_action_database();
  void click_action_table();
  void click_action_column();
  void click_action_row();
  void onEnterPressed();
  void init_treeview();
  void on_treeView_doubleClicked(const QModelIndex &index);
  void create_table(QString);

 protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

 private:
  Ui::MainWindow *ui;
  ui_login *ui_log;
  createtable *ui_create_table;
  std::vector<std::string> databases;
  QString prefix = "Co1aSQL > ";
};
#endif  // MAINWINDOW_H
