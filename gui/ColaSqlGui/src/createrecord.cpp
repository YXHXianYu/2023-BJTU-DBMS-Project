#include "createrecord.h"

#include "ui_createrecord.h"

createrecord::createrecord(std::vector<std::vector<std::any>> return_records,
                           const QString& tbName, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::createrecord)
{
    ui->setupUi(this);
    QIcon icon = QIcon(":/images/Colasql.png");
    this->setWindowIcon(icon);
    ui->tableView->setStyleSheet(
        "background:transparent;border-width:0;border-style:outset");
    records = return_records;
    table = tbName;
    display_table(records);

    int font_Id =
        QFontDatabase::addApplicationFont(":/font/FiraCode-Regular-1.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    if (!font_list.isEmpty())
    {
        QFont f;
        f.setFamily(font_list[0]);
        f.setPointSize(9);
        ui->tableView->setFont(f);
    }
}

createrecord::~createrecord() { delete ui; }

QString createrecord::anyToQString(const std::any& value)
{
    QString result;
    if (value.type() == typeid(std::string))
    {
        result = QString::fromStdString(std::any_cast<std::string>(value));
    }
    else if (value.type() == typeid(int))
    {
        result = QString::number(std::any_cast<int>(value));
    }
    else if (value.type() == typeid(double))
    {
        result = QString::number(std::any_cast<double>(value));
    }
    return result;
}

void createrecord::display_table(
    std::vector<std::vector<std::any>>& return_records)
{
    // display table on tableview
    std::vector<std::any> fields = return_records[0];
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(fields.size());
    for (size_t i = 0; i < fields.size(); ++i)
    {
        model->setHeaderData(i, Qt::Horizontal, anyToQString(fields[i]));
    }
    QList<QStandardItem*> items;
    // add row
    for (size_t i = 0; i < fields.size(); ++i)
    {
        items.append(new QStandardItem());
    }
    model->appendRow(items);
    ui->tableView->setModel(model);
}

void createrecord::on_btn_cancel_clicked() { this->close(); }

void createrecord::on_btn_finish_clicked()
{
    QString fields;
    QString values;
    QString opt;
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (model)
    {
        int cols = model->columnCount();
        for (int i = 0; i < cols; ++i)
        {
            QString new_data = model->data(model->index(0, i)).toString();
            if (new_data != "")
            {
                fields += anyToQString(records[0][i]) + " ";
                values += new_data + " ";
            }
        }
        qDebug() << fields;
        qDebug() << values;
        opt = "INSERT INTO " + table + " ";
        opt += fields;
        opt += "VALUES ";
        opt += values;
        opt += ";";
        qDebug() << opt;
        emit create_record_signal(opt);
        this->close();
    }
    else
    {
        qDebug() << "Failed to get the model";
    }
}
