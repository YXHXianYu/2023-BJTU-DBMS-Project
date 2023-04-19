/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actiontable;
    QAction *actionrow;
    QAction *action;
    QAction *action_addtable;
    QAction *action_addrow;
    QAction *action_column;
    QAction *action_database;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTreeWidget *treeWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTextEdit *textEdit_code;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menu_create;
    QMenu *menu_delete;
    QMenu *menu_update;
    QMenu *menu_query;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 750);
        MainWindow->setMinimumSize(QSize(1000, 750));
        MainWindow->setMaximumSize(QSize(1000, 750));
        actiontable = new QAction(MainWindow);
        actiontable->setObjectName(QString::fromUtf8("actiontable"));
        actionrow = new QAction(MainWindow);
        actionrow->setObjectName(QString::fromUtf8("actionrow"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_addtable = new QAction(MainWindow);
        action_addtable->setObjectName(QString::fromUtf8("action_addtable"));
        action_addrow = new QAction(MainWindow);
        action_addrow->setObjectName(QString::fromUtf8("action_addrow"));
        action_column = new QAction(MainWindow);
        action_column->setObjectName(QString::fromUtf8("action_column"));
        action_database = new QAction(MainWindow);
        action_database->setObjectName(QString::fromUtf8("action_database"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        treeWidget = new QTreeWidget(centralwidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        gridLayout_3->addWidget(treeWidget, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tableWidget = new QTableWidget(groupBox_2);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem6);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout_2->addWidget(tableWidget, 0, 1, 1, 1);

        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit_code = new QTextEdit(groupBox);
        textEdit_code->setObjectName(QString::fromUtf8("textEdit_code"));

        gridLayout->addWidget(textEdit_code, 0, 1, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(80, 200));

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 1, 1, 1);

        gridLayout_2->setRowStretch(0, 2);
        gridLayout_2->setRowStretch(1, 1);

        gridLayout_3->addWidget(groupBox_2, 0, 1, 1, 1);

        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 3);
        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 22));
        menu_create = new QMenu(menuBar);
        menu_create->setObjectName(QString::fromUtf8("menu_create"));
        menu_delete = new QMenu(menuBar);
        menu_delete->setObjectName(QString::fromUtf8("menu_delete"));
        menu_update = new QMenu(menuBar);
        menu_update->setObjectName(QString::fromUtf8("menu_update"));
        menu_query = new QMenu(menuBar);
        menu_query->setObjectName(QString::fromUtf8("menu_query"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_create->menuAction());
        menuBar->addAction(menu_delete->menuAction());
        menuBar->addAction(menu_update->menuAction());
        menuBar->addAction(menu_query->menuAction());
        menu_create->addAction(action_database);
        menu_create->addAction(action_addtable);
        menu_create->addAction(action_column);
        menu_create->addAction(action_addrow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actiontable->setText(QApplication::translate("MainWindow", "table", nullptr));
        actionrow->setText(QApplication::translate("MainWindow", "row", nullptr));
        action->setText(QApplication::translate("MainWindow", "\350\241\250", nullptr));
        action_addtable->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\350\241\250", nullptr));
        action_addrow->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\350\241\214", nullptr));
        action_column->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\345\210\227", nullptr));
        action_database->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\225\260\346\215\256\345\272\223", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Database", nullptr));
        groupBox_2->setTitle(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Sno", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Sname", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Ssex", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "1", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->item(0, 0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "21301034", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->item(0, 1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "\344\273\230\345\256\266\351\275\220", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(0, 2);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\347\224\267", nullptr));
        tableWidget->setSortingEnabled(__sortingEnabled);

        groupBox->setTitle(QApplication::translate("MainWindow", "\345\221\275\344\273\244\350\241\214", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "RUN", nullptr));
        menu_create->setTitle(QApplication::translate("MainWindow", "\346\226\260\345\273\272", nullptr));
        menu_delete->setTitle(QApplication::translate("MainWindow", "\345\210\240\351\231\244", nullptr));
        menu_update->setTitle(QApplication::translate("MainWindow", "\344\277\256\346\224\271", nullptr));
        menu_query->setTitle(QApplication::translate("MainWindow", "\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
