#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_login.h>

#include <QDebug>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>

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

 private slots:
  void click_action_database();

 private:
  Ui::MainWindow *ui;
  ui_login *ui_log;
};
#endif  // MAINWINDOW_H
