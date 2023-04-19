#include "ui_login.h"

#include "ui_ui_login.h"

ui_login::ui_login(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ui_login) {
  ui->setupUi(this);
  ui_regist = new ui_register;
  setWindowTitle("Login");

  connect(ui_regist, SIGNAL(back()), this, SLOT(show()));
}

ui_login::~ui_login() { delete ui; }

void ui_login::on_pushButton_login_clicked() {
  // if have this account and password is right
  // todo
  user_name = ui->lineEdit_account->text();
  user_pwd = ui->lineEdit_password->text();
  emit(login());
  this->close();
}

void ui_login::on_pushButton_regist_clicked() {
  ui_regist->show();
  this->hide();
}
