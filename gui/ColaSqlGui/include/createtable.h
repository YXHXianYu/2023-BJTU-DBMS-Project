#ifndef CREATETABLE_H
#define CREATETABLE_H

#include <QDebug>
#include <QFontDatabase>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>

namespace Ui
{
class createtable;
}

class createtable : public QMainWindow
{
    Q_OBJECT

public:
    explicit createtable(QWidget* parent = nullptr);
    ~createtable();

signals:
    void create_table_signal(QString, QString);

private slots:
    void on_cancel_table_clicked();

    void on_add_col_clicked();

    void on_finished_table_clicked();

private:
    Ui::createtable* ui;
    QStandardItemModel* model;
    QString database_name;
    QString table_name;
    std::vector<std::pair<QString, QString>> not_nulls;
    std::vector<std::pair<QString, QString>> primes;
    std::vector<std::pair<QString, QString>> uniques;
    std::vector<std::tuple<QString, QString, QString, QString>> foreigns;
    std::vector<std::pair<QString, QString>> record;
};

#endif // CREATETABLE_H
