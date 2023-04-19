#include "mainwindow.h"

#include "ui_login.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui_log = new ui_login;
  ui_log->show();

  setWindowTitle("ColaSql");

  connect(ui_log, SIGNAL(login()), this, SLOT(show()));
}

MainWindow::~MainWindow() { delete ui; }
