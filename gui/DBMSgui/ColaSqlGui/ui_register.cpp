#include "ui_register.h"

#include "ui_ui_register.h"

ui_register::ui_register(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ui_register) {
  ui->setupUi(this);
  setWindowTitle("Register");
}

ui_register::~ui_register() { delete ui; }

void ui_register::on_pushButton_back_clicked() {
  emit back();
  this->close();
}

void ui_register::on_pushButton_regist_clicked() {
  // if passwords are same then register an account
  // todo
  emit back();
  this->close();
}
