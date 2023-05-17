#include "mainwindow.h"

#include "ui_login.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui_log = new ui_login;
    ui_log->show();

    setWindowTitle("ColaSql");

    //  auto streamBuf =
    //  std::make_unique<QTextBrowserStreamBuf>(ui->textBrowser);
    QTextEditStreamBuf* streamBuf = new QTextEditStreamBuf(ui->textEdit_code);

    // 保存原始缓冲区，以便在以后恢复
    //  std::streambuf *originalCoutBuf = std::cout.rdbuf();

    // 重定向 std::cout 到 QTextBrowser
    //  std::cout.rdbuf(streamBuf.get());
    std::cout.rdbuf(streamBuf);

    // login
    connect(ui_log, SIGNAL(login()), this, SLOT(show()));
    connect(ui_log, SIGNAL(login()), this, SLOT(init_treeview()));

    // actions
    connect(ui->btn_create_database, SIGNAL(triggered()), this,
            SLOT(click_create_database()));
    connect(ui->btn_create_table, SIGNAL(triggered()), this,
            SLOT(click_create_table()));
    connect(ui->btn_create_field, SIGNAL(triggered()), this,
            SLOT(click_create_field()));
    connect(ui->btn_create_record, SIGNAL(triggered()), this,
            SLOT(click_create_record()));
    connect(ui->btn_delete_database, SIGNAL(triggered()), this,
            SLOT(click_delete_db()));
    connect(ui->btn_delete_table, SIGNAL(triggered()), this,
            SLOT(click_delete_tb()));
    connect(ui->btn_delete_field, SIGNAL(triggered()), this,
            SLOT(click_delete_field()));
    connect(ui->btn_delete_record, SIGNAL(triggered()), this,
            SLOT(click_delete_record()));
    connect(ui->btn_SQL, SIGNAL(triggered()), this,
            SLOT(click_complex_select()));
    connect(ui->btn_read_sql, SIGNAL(triggered()), this,
            SLOT(click_read_sql()));
    connect(ui->btn_change_user, SIGNAL(triggered()), this,
            SLOT(click_change_user()));
    connect(ui->btn_save, SIGNAL(triggered()), this, SLOT(click_save()));

    // detect enter pressed
    ui->textEdit_code->installEventFilter(this);
}

MainWindow::~MainWindow() { delete ui; }

QString MainWindow::anyToQString(const std::any& value)
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
    else if (value.type() == typeid(float))
    {
        result = QString::number(std::any_cast<float>(value));
    }
    return result;
}

std::string MainWindow::anytoString(const std::any& value)
{
    QString ret = anyToQString(value);
    return ret.toStdString();
}

int MainWindow::use_database(std::string database)
{
    int ret = DataProcessor::GetInstance().UseDatabase(database);
    if (ret != 0)
    {
        qDebug() << "when click table Error using database"
                 << QString::fromStdString(database) << "error code:" << ret;

        QMessageBox::warning(this, "错误", "使用数据库错误\n");
        return ret;
    }
    qDebug() << "use" + QString::fromStdString(database) << ret << endl;
    return kSuccess;
}

int MainWindow::select_all_from_table(
    std::string tbName, std::vector<std::vector<std::any>>& return_records)
{
    std::vector<std::string> f;
    f.push_back("*");
    std::vector<std::tuple<std::string, std::string, int>> c;
    int ret = DataProcessor::GetInstance().Select(tbName, f, c, return_records);
    if (ret != 0)
    {
        qDebug() << "when click table select error";
        QMessageBox::warning(this, "错误", "查询表错误\n");
        return_records.clear();
        return ret;
    }
    return kSuccess;
}

void MainWindow::display_table(
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
    for (size_t i = 1; i < return_records.size(); ++i)
    {
        QList<QStandardItem*> item;
        // add row
        for (auto const& record : return_records[i])
        {
            item.append(new QStandardItem(anyToQString(record)));
        }
        model->appendRow(item);
    }
    ui->tableView->setModel(model);
    connect(model, &QStandardItemModel::dataChanged, this,
            &MainWindow::handleTableModified);
}

void MainWindow::init_treeview()
{
    clicked_column = -1;
    clicked_row = -1;
    QStandardItemModel* model = new QStandardItemModel(this);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    model->setHorizontalHeaderLabels(QStringList(QStringLiteral("database")));

    ui->treeView->setModel(model);

    int ret = DataProcessor::GetInstance().GetCurrentDatabase(current_database);
    if (ret)
    {
        current_database = "";
    }
    // get databases
    std::vector<std::string> databases;
    ret = DataProcessor::GetInstance().ShowDatabases(databases);
    if (ret != 0)
    {
        qDebug() << "Error loading databases, error code:" << ret;
        return;
    }

    // add databases to treeview
    for (auto const& database : databases)
    {
        QStandardItem* item_database =
            new QStandardItem(QString::fromStdString(database));
        model->appendRow(item_database);

        // get tables in current database
        ret = use_database(database);
        if (ret != kSuccess)
            return;
        std::vector<std::string> tables;
        ret = DataProcessor::GetInstance().ShowTables(tables);
        if (ret != 0)
        {
            qDebug() << "Error loading tables for database"
                     << QString::fromStdString(database)
                     << "error code:" << ret;
            assert(false);
        }
        // add tables to treeview under the current database
        for (const auto& table : tables)
        {
            QStandardItem* item_table =
                new QStandardItem(QString::fromStdString(table));
            item_database->appendRow(item_table);
        }
    }
    // set current databases to null
    ret = DataProcessor::GetInstance().UseDatabase(current_database);
    if (ret != 0)
    {
        qDebug() << "Error loading databases, error code:" << ret;
        return;
    }
    ui->treeView->expandAll();
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex& index)
{
    // click a table then display it
    if (index.parent().isValid())
    {
        multiple_select = 0;
        std::string database = index.parent().data().toString().toStdString();
        std::string table = index.data().toString().toStdString();
        qDebug() << "click on " + QString::fromStdString(table);
        int ret = use_database(database);
        if (ret != kSuccess)
            return;
        // select * from table
        std::vector<std::vector<std::any>> return_records;
        ret = select_all_from_table(table, return_records);
        if (ret != kSuccess)
            return;
        // display table on tableview
        current_table = table;
        display_table(return_records);
    }
    // click a database
    else
    {
        qDebug() << "click database";
    }
}

void MainWindow::click_create_database()
{
    //    新建数据库
    qDebug() << "add database ";
    QString dbName =
        QInputDialog::getText(this, "输入", "数据库名:", QLineEdit::Normal);
    if (dbName.isEmpty())
    {
        return;
    }
    qDebug() << dbName;
    int ret = DataProcessor::GetInstance().CreateDatabase(
        dbName.toLower().toStdString());
    if (!ret)
    {
        QMessageBox::information(this, "通知", "数据库已创建");
    }
    else if (ret == kDatabaseExisted)
    {
        QMessageBox::warning(this, "错误", "数据库已存在\n");
        return;
    }
    else
    {
        qDebug() << "Error create database" << dbName << "error code:" << ret;
        assert(false);
    }
    init_treeview();
}

void MainWindow::click_create_table()
{
    // add table
    ui_create_table = new createtable();
    connect(ui_create_table, SIGNAL(create_table_signal(QString, QString)),
            this, SLOT(create_table(QString, QString)));
    ui_create_table->show();
}

void MainWindow::create_table(QString database, QString opt)
{
    qDebug() << database;
    qDebug() << opt;
    QString ret = QString::fromStdString(
        ColaSQLCommand::CommandProcessor::GetInstance().Run(
            database.toStdString()));
    if (ret != "Success!")
    {
        QMessageBox::warning(this, "错误", "建表失败，错误信息：" + ret);
        return;
    }
    ret = QString::fromStdString(
        ColaSQLCommand::CommandProcessor::GetInstance().Run(opt.toStdString()));
    if (ret != "Success!")
    {
        QMessageBox::warning(this, "错误", "建表失败，错误信息：" + ret);
        return;
    }
    QMessageBox::information(this, "通知", "新建表成功\n");
    init_treeview();
}

void MainWindow::click_create_field()
{
    qDebug() << "add column\n";
    ColumnDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        QString dbName = dialog.lineEdit4->text();
        qDebug() << dbName;
        QString tbName = dialog.lineEdit1->text();
        QString columnName = dialog.lineEdit2->text();
        QString columnType = dialog.comboBox->currentText();
        columnType = columnType.toLower();
        std::pair<std::string, std::string> field;
        field =
            std::make_pair(columnName.toStdString(), columnType.toStdString());
        int ret = use_database(dbName.toLower().toStdString());
        if (ret != kSuccess)
            return;
        ret = DataProcessor::GetInstance().AlterTableAdd(
            tbName.toLower().toStdString(), field);
        if (!ret)
        {
            QMessageBox::information(this, "通知", "新建列成功\n");
        }
        else if (ret == kDatabaseNotUse)
        {
            QMessageBox::warning(this, "错误", "未选用数据库\n");
        }
        else if (ret == kTableNotFound)
        {
            QMessageBox::warning(this, "错误", "未找到该表\n");
        }
    }
}

void MainWindow::click_create_record()
{
    qDebug() << "add row\n";
    QDialog dialog;
    QFormLayout layout(&dialog);
    dialog.setWindowTitle("输入");

    QLineEdit input1;
    QLineEdit input2;

    layout.addRow("数据库:", &input1);
    layout.addRow("插入表:", &input2);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog,
                     &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog,
                     &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString dbName = "";
        QString tbName = "";
        dbName = input1.text();
        tbName = input2.text();
        if (tbName == "" || dbName == "")
            return;
        qDebug() << dbName;
        qDebug() << tbName;
        int ret = use_database(dbName.toLower().toStdString());
        if (ret != kSuccess)
            return;
        std::vector<std::vector<std::any>> return_records;
        ret = select_all_from_table(tbName.toStdString(), return_records);
        if (return_records.size() == 0 || ret != kSuccess)
            return;
        ui_create_record = new createrecord(return_records, tbName);
        connect(ui_create_record, SIGNAL(create_record_signal(QString)), this,
                SLOT(create_record(QString)));
        ui_create_record->show();
    }
}

void MainWindow::create_record(QString opt)
{
    QString ret = QString::fromStdString(
        ColaSQLCommand::CommandProcessor::GetInstance().Run(opt.toStdString()));
    if (ret != "Success!")
    {
        QMessageBox::warning(this, "错误", "新建记录失败，错误信息：" + ret);
        return;
    }
    QMessageBox::information(this, "通知", "新建记录成功\n");
}

void MainWindow::click_delete_db()
{
    qDebug() << "delete database ";
    QString dbName =
        QInputDialog::getText(this, "输入", "数据库名:", QLineEdit::Normal);
    if (dbName.isEmpty())
        return;
    qDebug() << dbName;
    int ret = DataProcessor::GetInstance().DeleteDatabase(
        dbName.toLower().toStdString());
    if (ret != kSuccess)
    {
        QMessageBox::warning(
            this, "错误", "删除数据库失败，错误信息：" + QString::number(ret));
        return;
    }
    init_treeview();
    current_table = "";
    QStandardItemModel* model = new QStandardItemModel();
    ui->tableView->setModel(model);
    ui->treeView->expandAll();
    QMessageBox::information(this, "通知", "删除数据库成功\n");
}

void MainWindow::click_delete_tb()
{
    qDebug() << "delete table\n";
    QDialog dialog;
    QFormLayout layout(&dialog);
    dialog.setWindowTitle("输入");

    QLineEdit input1;
    QLineEdit input2;

    layout.addRow("数据库:", &input1);
    layout.addRow("删除表:", &input2);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog,
                     &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog,
                     &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString dbName = "";
        QString tbName = "";
        dbName = input1.text();
        tbName = input2.text();
        if (tbName.isEmpty() || dbName.isEmpty())
            return;
        qDebug() << dbName;
        qDebug() << tbName;
        int ret = use_database(dbName.toLower().toStdString());
        if (ret != kSuccess)
            return;
        ret = DataProcessor::GetInstance().DropTable(
            tbName.toLower().toStdString());
        if (ret != kSuccess)
        {
            QMessageBox::warning(
                this, "错误", "删除表失败，错误信息：" + QString::number(ret));
            return;
        }
        init_treeview();
        current_table = "";
        QStandardItemModel* model = new QStandardItemModel();
        ui->tableView->setModel(model);
        ui->treeView->expandAll();
        QMessageBox::information(this, "通知", "删除表成功！");
    }
}

void MainWindow::click_delete_field()
{
    // fk multiple select
    if (multiple_select != 0)
    {
        QMessageBox::warning(this, "错误", "多表查询不支持修改\n");
        return;
    }
    if (current_table == "")
    {
        QMessageBox::warning(this, "错误", "请先选用表");
        return;
    }

    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (model)
    {
        QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
        QModelIndexList selectedColumns = selectionModel->selectedColumns();
        foreach (const QModelIndex& index, selectedColumns)
        {
            QString field =
                model->headerData(index.column(), Qt::Horizontal).toString();
            qDebug() << QString::number(clicked_column);
            qDebug() << "delete field" + field + "from table" +
                            QString::fromStdString(current_table);
            int ret = DataProcessor::GetInstance().AlterTableDrop(
                current_table, field.toLower().toStdString());
            if (ret != kSuccess)
            {
                QMessageBox::warning(this, "错误", "删除字段错误");
                qDebug() << "delete field error" + QString::number(ret);
                return;
            }
        }
        QMessageBox::information(this, "通知", "删除字段成功！");
        std::vector<std::vector<std::any>> return_records;
        int ret = select_all_from_table(current_table, return_records);
        if (ret != kSuccess)
        {
            return;
        }
        display_table(return_records);
    }
    else
    {
        QMessageBox::warning(this, "错误", "请先选用表");
        return;
    }
}

void MainWindow::click_delete_record()
{
    // fk multiple select
    if (multiple_select != 0)
    {
        QMessageBox::warning(this, "错误", "多表查询不支持修改\n");
        return;
    }
    if (current_table == "")
    {
        QMessageBox::warning(this, "错误", "请先选用表");
        return;
    }
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (model)
    {
        QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
        QModelIndexList selectedRows = selectionModel->selectedRows();
        foreach (const QModelIndex& index, selectedRows)
        {
            int row = index.row();
            std::vector<std::tuple<std::string, std::string, int>> conditions;
            for (int column = 0; column < model->columnCount(); ++column)
            {
                QModelIndex cellIndex = model->index(row, column);
                QString data = model->data(cellIndex).toString();
                QString field =
                    model->headerData(column, Qt::Horizontal).toString();
                qDebug() << data;
                qDebug() << field;
                conditions.push_back(std::make_tuple(
                    field.toStdString(), data.toStdString(), kEqualConditon));
            }
            int ret =
                DataProcessor::GetInstance().Delete(current_table, conditions);
            if (ret != kSuccess)
            {
                QMessageBox::warning(this, "错误", "删除记录错误");
                qDebug() << "delete record error" + QString::number(ret);
                return;
            }
        }
        QMessageBox::information(this, "通知", "删除记录成功！");
        std::vector<std::vector<std::any>> return_records;
        int ret = select_all_from_table(current_table, return_records);
        if (ret != kSuccess)
        {
            return;
        }
        display_table(return_records);
    }
    else
    {
        QMessageBox::warning(this, "错误", "请先选用表");
        return;
    }
}

void MainWindow::click_save()
{
    std::string ret =
        ColaSQLCommand::CommandProcessor::GetInstance().Run("COMMIT;");
    QMessageBox::information(this, "通知", QString::fromStdString(ret));
}

void MainWindow::click_complex_select()
{
    qDebug() << "complex select";
    multiple_select = 1;
    QDialog dialog;
    QFormLayout layout(&dialog);
    dialog.setWindowTitle("输入");

    QLineEdit dbInput;
    QTextEdit input;

    layout.addRow("所属数据库", &dbInput);
    layout.addRow(&input);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog,
                     &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog,
                     &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString dbName = dbInput.text();
        int db_ret = use_database(dbName.toStdString());
        if (db_ret != kSuccess)
        {
            QMessageBox::warning(
                this, "错误",
                "使用数据库失败，错误信息：" + QString::number(db_ret));
            return;
        }
        QString sql = input.toPlainText();
        qDebug() << sql;
        std::vector<std::vector<std::any>> return_records;
        QString ret = QString::fromStdString(
            ColaSQLCommand::CommandProcessor::GetInstance().ComplexSelect(
                sql.toStdString(), return_records));
        qDebug() << "return success!";
        if (ret != "Success!")
        {
            QMessageBox::warning(this, "错误",
                                 "复杂查询失败，错误信息：" + ret);
            return;
        }
        display_table(return_records);
    }
}

void MainWindow::click_read_sql()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Select File");
    QString filePath = dialog.getOpenFileName();
    if (!filePath.isEmpty())
    {
        QFileInfo fileInfo(filePath);
        if (fileInfo.suffix() != "colasql")
        {
            QMessageBox::warning(this, "错误文件类型", "请选择.colasql文件");
        }
        else
        {
            qDebug() << "Selected File: " << filePath;
            QString opt = "run " + filePath + ";";
            qDebug() << "cmd run: " << opt;
            QString ret = QString::fromStdString(
                ColaSQLCommand::CommandProcessor::GetInstance().Run(
                    opt.toStdString()));
            qDebug() << "return success!";
            if (ret != "Success!")
            {
                QMessageBox::warning(this, "错误",
                                     "执行.colasql错误，错误信息：" + ret);
                return;
            }
        }
    }
}

void MainWindow::click_change_user()
{
    ui_log = new ui_login;
    ui_log->show();
    this->hide();
    connect(ui_log, SIGNAL(login()), this, SLOT(show()));
    connect(ui_log, SIGNAL(login()), this, SLOT(init_treeview()));
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->textEdit_code && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        QTextCursor cursor = ui->textEdit_code->textCursor();

        if (keyEvent->key() == Qt::Key_Return ||
            keyEvent->key() == Qt::Key_Enter)
        {
            // 当按下回车键时，触发槽函数
            onEnterPressed();
            return true; // 返回 true 表示事件已被处理
        }
        else if (keyEvent->key() == Qt::Key_Backspace)
        {
            // 当按下退格键时
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor,
                                prefix.length());
            QString textBeforeCursor = cursor.selectedText();

            if (textBeforeCursor == prefix)
            {
                return true; // 如果光标前的字符串等于指定字符串，不允许删除
            }
            else if (cursor.atBlockStart())
            {
                return true; // 如果光标在行首，不允许删除
            }
        }
        else if (keyEvent->key() == Qt::Key_Delete)
        {
            // 当按下删除键时
            if (cursor.atBlockEnd())
            {
                return true; // 如果光标在行尾，不允许删除
            }
        }
    }
    // 对于其他事件和对象，调用基类的事件过滤器
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onEnterPressed()
{
    QTextCursor cursor = ui->textEdit_code->textCursor();
    cursor.select(QTextCursor::LineUnderCursor);
    QString input = cursor.selectedText().mid(prefix.length());
    qDebug() << "回车键被按下，当前行内容：" << input;
    if (input == "cls;")
    {
        ui->textEdit_code->clear();
    }
    else
    {
        QString ret = QString::fromStdString(
            ColaSQLCommand::CommandProcessor::GetInstance().Run(
                input.toStdString()));
        qDebug() << ret;
        if (ret != "")
        {
            std::cout << ret.toStdString();
            init_treeview();
        }
    }
    ui->textEdit_code->append("Co1aSQL > ");
}

void MainWindow::handleTableModified() { save_change(); }

// 单点修改
void MainWindow::cancel_change()
{
    qDebug() << "cancel edit";
    int ret = DataProcessor::GetInstance().GetCurrentDatabase(current_database);
    if (ret)
    {
        current_database = "";
    }
    if (current_table == "" || current_database == "")
        return;
    ret = use_database(current_database);
    if (ret != kSuccess)
        return;
    qDebug() << "use current_database";
    // select * from table
    std::vector<std::vector<std::any>> return_records;
    ret = select_all_from_table(current_table, return_records);
    if (ret != kSuccess)
        return;
    // display table on tableview
    display_table(return_records);
}
void MainWindow::save_change()
{
    // fk multiple select
    if (multiple_select != 0)
    {
        QMessageBox::warning(this, "错误", "多表查询不支持修改\n");
        return;
    }
    int ret = DataProcessor::GetInstance().GetCurrentDatabase(current_database);
    if (ret)
    {
        current_database = "";
    }
    // get origin table data
    if (current_table == "" || current_database == "")
        return;
    ret = use_database(current_database);
    if (ret != kSuccess)
        return;
    // select * from table
    std::vector<std::vector<std::any>> origin_records;
    ret = select_all_from_table(current_table, origin_records);
    if (ret != kSuccess)
        return;
    QStandardItemModel* model =
        qobject_cast<QStandardItemModel*>(ui->tableView->model());
    int rows = model->rowCount();
    int cols = model->columnCount();
    int diff = 0;
    std::vector<std::pair<std::string, std::string>> modify;
    std::vector<std::tuple<std::string, std::string, int>> condition;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            QString new_data = model->data(model->index(i, j)).toString();
            QString origin_data = anyToQString(origin_records[i + 1][j]);
            if (new_data != origin_data)
            {
                qDebug() << "modify:" << new_data;
                diff++;
                if (diff > 1)
                {
                    QMessageBox::warning(this, "错误",
                                         "不支持多条处记录同时修改\n");
                    cancel_change();
                    return;
                }
                modify.push_back({anytoString(origin_records[0][j]),
                                  new_data.toStdString()});
                for (int k = 0; k < cols; ++k)
                {
                    condition.push_back(std::make_tuple(
                        anytoString(origin_records[0][k]),
                        anytoString(origin_records[i + 1][k]), kEqualConditon));
                }
            }
        }
    }
    ret = DataProcessor::GetInstance().Update(current_table, modify, condition);
    if (ret != kSuccess)
    {
        QMessageBox::warning(this, "错误", "修改错误" + QString::number(ret));
        qDebug() << "update error：" << ret;
        cancel_change();
    }
}

void MainWindow::on_btn_commit_clicked()
{
    int ret = DataProcessor::GetInstance().Write();
    if (ret != kSuccess)
    {
        QMessageBox::warning(this, "错误", "commit错误" + QString::number(ret));
        qDebug() << "commit error：" << ret;
    }
    QMessageBox::information(this, "通知", "commit success!");
}

void MainWindow::on_btn_rollback_clicked()
{
    qDebug() << "click rollback";
    int ret = DataProcessor::GetInstance().Read();
    qDebug() << "get return";
    if (ret != kSuccess)
    {
        QMessageBox::warning(this, "错误",
                             "rollback错误" + QString::number(ret));
        qDebug() << "rollback error：" << ret;
        return;
    }
    init_treeview();
    current_table = "";
    QStandardItemModel* model = new QStandardItemModel();
    ui->tableView->setModel(model);
    ui->treeView->expandAll();
    QMessageBox::information(this, "通知", "rollback success!");
}

void MainWindow::on_tableView_clicked(const QModelIndex& index)
{
    clicked_column = index.column();
    clicked_row = index.row();
}
