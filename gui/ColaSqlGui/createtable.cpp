#include "createtable.h"

#include "ui_createtable.h"

createtable::createtable(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::createtable) {
  ui->setupUi(this);
  model = new QStandardItemModel();
  model->setColumnCount(2);
  model->setHeaderData(0, Qt::Horizontal, "字段");
  model->setHeaderData(1, Qt::Horizontal, "类型");
  model->setHeaderData(2, Qt::Horizontal, "约束");
  ui->tableView->setModel(model);
}

createtable::~createtable() { delete ui; }

void createtable::on_cancel_table_clicked() { this->close(); }

void createtable::on_add_col_clicked() {
  //  ui->tableView->close();
  //  QString col_name = ui->col_name->text();
  //  QString col_type = ui->comboBox->currentText();
  //  if (ui->notnull_check->isChecked()) {
  //    not_nulls.push_back("not null");
  //  } else {
  //    not_nulls.push_back("null");
  //  }
  //  if (ui->pk_check->isChecked()) {
  //    pks.push_back(col_name);
  //  }
  //  if (col_type == "VARCHAR") {
  //    if (ui->varchar_n->text() == "") {
  //      QMessageBox::warning(this, "警告",
  //                           "此列数据类型为varchar但是你未输入n值！");
  //      // 输入有问题此时需要回退notnull和primary key
  //      not_nulls.pop_back();
  //      if (ui->pk_check->isChecked()) {
  //        pks.pop_back();
  //      }
  //      return;
  //    } else {
  //      col_type += "(" + ui->varchar_n->text() + ")";
  //    }
  //  }
  //  col_ty.emplace_back(col_name, col_type);
  //  QList<QStandardItem *> item;
  //  item.append(new QStandardItem(col_name));
  //  item.append(new QStandardItem(col_type));
  //  model.appendRow(item);
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
  QString opt;
  database_name = ui->dbName_lineEdit->text();
  if (table_name == "") {
    QMessageBox::warning(this, "警告", "请给出数据库名！");
    return;
  }
  table_name = ui->tableName_lineEdit->text();
  if (table_name == "") {
    QMessageBox::warning(this, "警告", "请给出表名！");
    return;
  }

  emit create_table_signal(opt);
  this->close();
}
