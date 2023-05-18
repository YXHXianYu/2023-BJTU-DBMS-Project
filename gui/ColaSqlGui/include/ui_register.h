#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QFontDatabase>
#include <QMainWindow>
#include <QMessageBox>

#include "dataprocessor.h"

namespace Ui
{
class ui_register;
}

class ui_register : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui_register(QWidget* parent = nullptr);
    ~ui_register();

signals:
    void back();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_regist_clicked();

private:
    Ui::ui_register* ui;
    QString user_name;
    QString user_pwd1;
    QString user_pwd2;

    void clear();
};

#endif // UI_REGISTER_H
