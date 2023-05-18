#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <ui_login.h>

#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QTreeWidget>
#include <memory>

#include "columndialog.h"
#include "command_processor.h"
#include "createindex.h"
#include "createrecord.h"
#include "createtable.h"
#include "dataprocessor.h"
#include "qtbstreambuf.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // 辅助功能
    QString anyToQString(const std::any& value);    // any转QString
    std::string anytoString(const std::any& value); // any转string
    int use_database(std::string database);         // 使用数据库
    int select_all_from_table(std::string tbName,
                              std::vector<std::vector<std::any>>&
                                  return_records); // select * from table
    void display_table(
        std::vector<std::vector<std::any>>& return_records); // 展示查询结果

    void cancel_change(); // 取消单点修改
    void save_change();   // 保存单点修改

private slots:
    // 点击事件
    void click_create_database();   // 创建库
    void click_create_table();      // 创建表
    void click_create_field();      // 创建字段
    void click_create_record();     // 创建记录
    void click_create_index();      // 新建索引
    void click_delete_db();         // 删除数据库
    void click_delete_tb();         // 删除表
    void click_delete_field();      // 删除字段
    void click_delete_record();     // 删除记录
    void click_save();              // 保存文件
    void click_complex_select();    // 复杂查询
    void click_read_sql();          // 执行.sql文件
    void click_change_user();       // 切换用户
    void on_btn_commit_clicked();   // commit
    void on_btn_rollback_clicked(); // rollback

    // 实现功能
    void refresh();                      // 刷新
    void create_table(QString, QString); // 创建表
    void create_record(QString);         // 创建一条记录
    void create_index(QString);          // 创建索引
    void handleTableModified();          // 点击修改某一记录

    // 树形结构
    void init_treeview(); // 初始化树形结构
    void on_treeView_doubleClicked(const QModelIndex& index); // 双击树形结构

    // override命令行回车键
    void onEnterPressed();

protected:
    // override事件过滤器
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    // 界面
    Ui::MainWindow* ui;             // this
    ui_login* ui_log;               // 登录界面
    createtable* ui_create_table;   // 新建表界面
    createrecord* ui_create_record; // 新建记录界面
    createindex* ui_create_index;   // 新建索引界面

    // Co1aSQL!
    QString prefix = "Co1aSQL @ ";

    // 状态
    std::string current_database = ""; // 当前选用的数据库
    std::string current_table = "";    // 当前查询表
    bool multiple_select = 0;          // 是否是多表查询
};
#endif // MAINWINDOW_H
