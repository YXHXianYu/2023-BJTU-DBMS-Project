#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <ui_register.h>

#include <QFontDatabase>
#include <QMainWindow>
#include <QMessageBox>

#include "dataprocessor.h"

namespace Ui
{
class ui_login;
}

class ui_login : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui_login(QWidget* parent = nullptr);
    ~ui_login();

signals:
    void login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_regist_clicked();

private:
    Ui::ui_login* ui;
    ui_register* ui_regist;
    QString user_name;
    QString user_pwd;
    void clear();
};

#endif // UI_LOGIN_H
