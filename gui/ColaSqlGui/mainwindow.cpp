#include "mainwindow.h"

#include <memory>

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
    connect(ui_log, SIGNAL(login()), this, SLOT(addTreeItems()));
    connect(ui_log, SIGNAL(login()), this, SLOT(init_treeview()));

    // actions
    connect(ui->action_database, SIGNAL(triggered()), this,
            SLOT(click_action_database()));
    connect(ui->action_table, SIGNAL(triggered()), this,
            SLOT(click_action_table()));
    connect(ui->action_column, SIGNAL(triggered()), this,
            SLOT(click_action_column()));
    connect(ui->action_row, SIGNAL(triggered()), this,
            SLOT(click_action_row()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(save()));

    // detect enter pressed
    ui->textEdit_code->installEventFilter(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::use_database(std::string database)
{
    int ret = DataProcessor::GetInstance().UseDatabase(database);
    if (ret != 0)
    {
        qDebug() << "when click table Error using database"
                 << QString::fromStdString(database) << "error code:" << ret;
        assert(false);
    }
    qDebug() << "use" + QString::fromStdString(database) << ret << endl;
}

void MainWindow::select_all_from_table(
    std::string tbName, std::vector<std::vector<std::any>>& return_records)
{
    std::vector<std::string> f;
    f.push_back("*");
    std::vector<std::tuple<std::string, std::string, int>> c;
    int ret = DataProcessor::GetInstance().Select(tbName, f, c, return_records);
    if (ret != 0)
    {
        qDebug() << "when click table select error";
        assert(false);
    }
}

void MainWindow::init_treeview()
{
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
        use_database(database);
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

void MainWindow::click_action_database()
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
    int ret = DataProcessor::GetInstance().CreateDatabase(dbName.toStdString());
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

void MainWindow::click_action_table()
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

void MainWindow::click_action_column()
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
        use_database(dbName.toStdString());
        int ret = DataProcessor::GetInstance().AlterTableAdd(
            tbName.toStdString(), field);
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

void MainWindow::click_action_row()
{
    qDebug() << "add row\n";
    QString tbName =
        QInputDialog::getText(this, "输入", "表名:", QLineEdit::Normal);
    QStringList fieldName =
        QInputDialog::getText(this, "输入",
                              "请输入要插入的字段名，每个字段用','隔开",
                              QLineEdit::Normal, "Info1,Info2,......")
            .split(",");
    QStringList input =
        QInputDialog::getText(this, "输入", "请输入一条记录，每个字段用','隔开",
                              QLineEdit::Normal, "Info1,Info2,......")
            .split(",");
    qDebug() << input << endl;
    std::vector<std::pair<std::string, std::string>> record_in;
    if (fieldName.at(0) == '*')
    {
        for (int i = 0; i < input.length(); ++i)
        {
            record_in.push_back({"*", input.at(i).toStdString()});
        }
    }
    else if (fieldName.length() == input.length())
    {
        for (int i = 0; i < input.length(); ++i)
        {
            record_in.push_back(
                {fieldName.at(i).toStdString(), input.at(i).toStdString()});
        }
    }
    else
    {
        QMessageBox::warning(this, "错误", "插入字段与输入数据长度不同\n");
        return;
    }
    int ret =
        DataProcessor::GetInstance().Insert(tbName.toStdString(), record_in);
    if (!ret)
    {
        QMessageBox::information(this, "通知", "新建记录成功\n");
        return;
    }
    else if (ret == kDatabaseNotUse)
    {
        QMessageBox::warning(this, "错误", "未选用数据库\n");
        return;
    }
    else if (ret == kTableNotFound)
    {
        QMessageBox::warning(this, "错误", "未找到该表\n");
        return;
    }
    else if (ret == kDataTypeWrong)
    {
        QMessageBox::warning(this, "错误", "数据与字段类型不符\n");
        return;
    }
    else if (ret == kFieldNotFound)
    {
        QMessageBox::warning(this, "错误", "未找到该字段\n");
        return;
    }
    else
    {
        QMessageBox::warning(this, "错误", "i dont know! fk u!\n");
        assert(false);
    }
}

void MainWindow::save()
{
    std::string ret =
        ColaSQLCommand::CommandProcessor::GetInstance().Run("COMMIT;");
    QMessageBox::information(this, "通知", QString::fromStdString(ret));
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
    else if (value.type() == typeid(double))
    {
        result = QString::number(std::any_cast<double>(value));
    }
    return result;
}

std::string MainWindow::anytoString(const std::any& value)
{
    QString ret = anyToQString(value);
    return ret.toStdString();
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex& index)
{
    // click a table then display it
    if (index.parent().isValid())
    {
        std::string database = index.parent().data().toString().toStdString();
        std::string table = index.data().toString().toStdString();
        qDebug() << "click on " + QString::fromStdString(table);
        use_database(database);
        // select * from table
        std::vector<std::vector<std::any>> return_records;
        select_all_from_table(table, return_records);
        current_table = table;
        // display table on tableview
        display_table(return_records);
    }
    // click a database
    else
    {
        qDebug() << "click database";
    }
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
}

void MainWindow::on_btn_cancel_clicked()
{
    qDebug() << "cancel edit";
    int ret = DataProcessor::GetInstance().GetCurrentDatabase(current_database);
    if (ret)
    {
        current_database = "";
    }
    if (current_table == "" || current_database == "")
        return;
    use_database(current_database);
    qDebug() << "use current_database";
    // select * from table
    std::vector<std::vector<std::any>> return_records;
    select_all_from_table(current_table, return_records);
    // display table on tableview
    display_table(return_records);
}

void MainWindow::on_btn_save_clicked()
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
    use_database(current_database);
    // select * from table
    std::vector<std::vector<std::any>> origin_records;
    select_all_from_table(current_table, origin_records);
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
                diff++;
                if (diff > 1)
                {
                    QMessageBox::warning(this, "错误",
                                         "不支持多条处记录同时修改\n");
                    on_btn_cancel_clicked();
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
        QMessageBox::warning(this, "错误", "修改错误\n");
        qDebug() << "update error：" << ret;
        assert(false);
    }
}
