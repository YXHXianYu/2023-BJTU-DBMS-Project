#include "createtable.h"

#include "ui_createtable.h"

createtable::createtable(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::createtable) {
  ui->setupUi(this);
  model = new QStandardItemModel();
  model->setColumnCount(2);
  model->setHeaderData(0, Qt::Horizontal, "字段");
  model->setHeaderData(1, Qt::Horizontal, "类型");
  ui->tableView->setModel(model);
}

createtable::~createtable() { delete ui; }

void createtable::on_cancel_table_clicked() { this->close(); }

void createtable::on_add_col_clicked() {
  ui->tableView->close();
  QString field = ui->col_name->text();
  QString type = ui->comboBox->currentText();
  if (field == "") {
    QMessageBox::warning(this, "警告", "未输入字段名！");
    return;
  }
  if (ui->foreign_check->isChecked()) {
    QString reference_table = ui->foreign_table->text();
    if (reference_table == "") {
      QMessageBox::warning(this, "警告", "未输入参考表！");
      return;
    } else {
      foreigns.push_back({field, reference_table});
    }
  }
  if (ui->notnull_check->isChecked()) {
    not_nulls.push_back("not null");
  }
  if (ui->pk_check->isChecked()) {
    primes.push_back(field);
  }
  if (ui->unique_check->isChecked()) {
    uniques.push_back(field);
  }
  record.push_back({field, type});
  QList<QStandardItem *> item;
  item.append(new QStandardItem(field));
  item.append(new QStandardItem(type));
  model->appendRow(item);
  // 展示所加如的列
  //    for (int i = 0 ; i < col_ty.size(); ++i) {
  //        QList<QStandardItem*> item;
  //        item.append(new QStandardItem(col_ty[i].first));
  //        item.append(new QStandardItem(col_ty[i].second));
  //        model.appendRow(item);
  //    }

  ui->tableView->show();
  ui->col_name->clear();
  ui->foreign_table->clear();
  ui->foreign_check->setChecked(false);
  ui->notnull_check->setChecked(false);
  ui->pk_check->setChecked(false);
  ui->unique_check->setChecked(false);
}

void createtable::on_finished_table_clicked() {
  QString db;
  QString opt;
  database_name = ui->dbName_lineEdit->text();
  if (database_name == "") {
    QMessageBox::warning(this, "警告", "请给出数据库名！");
    return;
  }
  db += "use " + database_name + ";";
  table_name = ui->tableName_lineEdit->text();
  if (table_name == "") {
    QMessageBox::warning(this, "警告", "请给出表名！");
    return;
  }
  opt += "create table " + table_name + " ";
  for (auto const &pair : record) {
    QString field = pair.first;
    QString type = pair.second;
    opt += field + ' ' + type + ' ';
  }
  for (auto const &not_null : not_nulls) {
    opt += "CONSTRAINT NOT NULL " + not_null + " ";
  }
  for (auto const &unique : uniques) {
    opt += "CONSTRAINT UNIQUE " + unique + " ";
  }
  for (auto const &prime : primes) {
    opt += "CONSTRAINT PRIMARY KEY " + prime + " ";
  }
  for (auto const &pair : foreigns) {
    QString key = pair.first;
    QString table = pair.second;
    opt += "CONSTRAINT FOREIGN KEY " + key + " REFERENCES " + table + " " +
           key + " ";
  }
  opt += ";";
  qDebug() << opt;
  emit create_table_signal(db, opt);
  this->close();
}
