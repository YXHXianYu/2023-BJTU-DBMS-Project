#include "ui_login.h"

#include "ui_ui_login.h"

ui_login::ui_login(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ui_login) {
  ui->setupUi(this);
  ui_regist = new ui_register;
  setWindowTitle("Login");
  ui->lineEdit_password->setEchoMode(QLineEdit::Password);

  connect(ui_regist, SIGNAL(back()), this, SLOT(show()));
}

ui_login::~ui_login() { delete ui; }

void ui_login::clear() {
  ui->lineEdit_account->setText("");
  ui->lineEdit_password->setText("");
}

void ui_login::on_pushButton_login_clicked() {
  user_name = ui->lineEdit_account->text();
  user_pwd = ui->lineEdit_password->text();
  int ret = DataProcessor::GetInstance().Login(user_name.toStdString(),
                                               user_pwd.toStdString());
  if (!ret) {
    emit login();
    this->close();
  } else if (ret == kUserNameNotFound) {
    QMessageBox::warning(this, "错误", "用户不存在");
    clear();
  } else {
    QMessageBox::warning(this, "错误", "用户名或密码错误");
    //    ui->lineEdit_account->setText("");
    ui->lineEdit_password->setText("");
  }
}

void ui_login::on_pushButton_regist_clicked() {
  ui_regist->show();
  clear();
  this->hide();
}
