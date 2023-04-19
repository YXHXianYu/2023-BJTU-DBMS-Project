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
  connect(ui->action_database, SIGNAL(triggered()), this,
          SLOT(click_action_database()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::click_action_database() {
  QString dbName =
      QInputDialog::getText(this, "输入", "数据库名:", QLineEdit::Normal);
  if (dbName.isEmpty()) {
    return;
  } else {
    qDebug() << dbName;
    int ret = DataProcessor::GetInstance().CreateDatabase(dbName.toStdString());
    if (!ret) {
      ui->textEdit_code->append("\nCola>数据库" + dbName + "已创建\n");
      QMessageBox::information(this, "通知", "数据库已创建");
    } else if (ret == kDatabaseExisted) {
      ui->textEdit_code->append("\nCola>数据库" + dbName + "已存在\n");
      QMessageBox::warning(this, "错误", "数据库已存在\n");
      return;
    }
  }
}
