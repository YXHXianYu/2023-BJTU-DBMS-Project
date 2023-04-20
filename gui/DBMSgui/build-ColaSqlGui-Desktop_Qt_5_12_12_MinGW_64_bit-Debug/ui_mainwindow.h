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
#include <QtWidgets/QTextBrowser>
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
    QAction *action_table;
    QAction *action_row;
    QAction *action_column;
    QAction *action_database;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTreeWidget *treeWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QTextEdit *textEdit_code;
    QTextBrowser *textBrowser;
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
        action_table = new QAction(MainWindow);
        action_table->setObjectName(QString::fromUtf8("action_table"));
        action_row = new QAction(MainWindow);
        action_row->setObjectName(QString::fromUtf8("action_row"));
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
        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(80, 200));

        gridLayout->addWidget(pushButton, 1, 2, 1, 1);

        textEdit_code = new QTextEdit(groupBox);
        textEdit_code->setObjectName(QString::fromUtf8("textEdit_code"));

        gridLayout->addWidget(textEdit_code, 1, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 1, 1, 1);

        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QFont font;
        font.setPointSize(15);
        textBrowser->setFont(font);

        gridLayout_2->addWidget(textBrowser, 0, 1, 1, 1);

        gridLayout_2->setRowStretch(0, 2);

        gridLayout_3->addWidget(groupBox_2, 0, 1, 1, 1);

        gridLayout_3->setColumnStretch(0, 1);
        gridLayout_3->setColumnStretch(1, 3);
        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 21));
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
        menu_create->addAction(action_table);
        menu_create->addAction(action_column);
        menu_create->addAction(action_row);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actiontable->setText(QApplication::translate("MainWindow", "table", nullptr));
        actionrow->setText(QApplication::translate("MainWindow", "row", nullptr));
        action->setText(QApplication::translate("MainWindow", "\350\241\250", nullptr));
        action_table->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\350\241\250", nullptr));
        action_row->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\350\256\260\345\275\225", nullptr));
        action_column->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\345\255\227\346\256\265", nullptr));
        action_database->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\225\260\346\215\256\345\272\223", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "using", nullptr));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Database", nullptr));
        groupBox_2->setTitle(QString());
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
