#ifndef CREATEINDEX_H
#define CREATEINDEX_H

#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>

#include "command_processor.h"
#include "dataprocessor.h"

namespace Ui
{
class createindex;
}

class createindex : public QMainWindow
{
    Q_OBJECT

public:
    explicit createindex(std::vector<std::vector<std::any>> return_records,
                         const QString& tbName, QWidget* parent = nullptr);
    ~createindex();

private:
    void display_table(std::vector<std::vector<std::any>>& return_records);
    QString anyToQString(const std::any& value);

signals:
    void create_index_signal(QString);

private slots:
    void on_btn_cancel_clicked();

    void on_btn_finish_clicked();

private:
    Ui::createindex* ui;
    std::vector<std::vector<std::any>> records;
    QString table;
};

#endif // CREATEINDEX_H
