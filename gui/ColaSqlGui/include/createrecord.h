#ifndef CREATERECORD_H
#define CREATERECORD_H

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
class createrecord;
}

class createrecord : public QMainWindow
{
    Q_OBJECT

public:
    explicit createrecord(std::vector<std::vector<std::any>> return_records,
                          const QString& tbName, QWidget* parent = nullptr);
    ~createrecord();

private:
    void display_table(std::vector<std::vector<std::any>>& return_records);
    QString anyToQString(const std::any& value);

signals:
    void create_record_signal(QString);

private slots:
    void on_btn_cancel_clicked();

    void on_btn_finish_clicked();

private:
    Ui::createrecord* ui;
    std::vector<std::vector<std::any>> records;
    QString table;
};

#endif // CREATERECORD_H
