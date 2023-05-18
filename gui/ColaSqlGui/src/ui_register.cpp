#include "ui_register.h"

#include <QtDebug>

#include "ui_ui_register.h"

ui_register::ui_register(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::ui_register)
{
    ui->setupUi(this);
    setWindowTitle("Register");
    ui->lineEdit_password1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_password2->setEchoMode(QLineEdit::Password);
}

ui_register::~ui_register() { delete ui; }

void ui_register::on_pushButton_back_clicked()
{
    emit back();
    clear();
    this->close();
}

void ui_register::clear()
{
    ui->lineEdit_account->setText("");
    ui->lineEdit_password2->setText("");
    ui->lineEdit_password1->setText("");
}

void ui_register::on_pushButton_regist_clicked()
{
    // if passwords are same then register an account
    // todo
    user_name = ui->lineEdit_account->text();
    user_pwd1 = ui->lineEdit_password1->text();
    user_pwd2 = ui->lineEdit_password2->text();
    if (user_name == "" || user_pwd1 == "" || user_pwd2 == "")
    {
        QMessageBox::warning(this, "错误", "用户名、密码不能为空！");
        return;
    }
    if (user_pwd1 == user_pwd2)
    {
        int ret = DataProcessor::GetInstance().CreateUser(user_name.toStdString(),
                                                          user_pwd1.toStdString());
        if (ret == kUserNameExisted)
            QMessageBox::warning(this, "错误", "用户已存在");
        else if (!ret)
        {
            QMessageBox::information(this, "欢迎", "用户注册成功");
            emit back();
            this->close();
        }
        else
        {
            qDebug() << "i dont know!fk urself!";
            assert(false);
        }
    }
    else
    {
        QMessageBox::warning(this, "错误", "两次密码不同");
    }
    clear();
}
