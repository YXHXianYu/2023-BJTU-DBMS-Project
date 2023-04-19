/********************************************************************************
** Form generated from reading UI file 'ui_login.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_LOGIN_H
#define UI_UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ui_login
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_account;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_password;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_login;
    QPushButton *pushButton_regist;

    void setupUi(QMainWindow *ui_login)
    {
        if (ui_login->objectName().isEmpty())
            ui_login->setObjectName(QString::fromUtf8("ui_login"));
        ui_login->resize(400, 300);
        ui_login->setMinimumSize(QSize(400, 300));
        ui_login->setMaximumSize(QSize(400, 300));
        centralwidget = new QWidget(ui_login);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 30, 151, 40));
        QFont font;
        font.setPointSize(20);
        label->setFont(font);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(75, 80, 250, 130));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 9, 9);
        lineEdit_account = new QLineEdit(groupBox);
        lineEdit_account->setObjectName(QString::fromUtf8("lineEdit_account"));

        gridLayout->addWidget(lineEdit_account, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, -1, -1);
        lineEdit_password = new QLineEdit(groupBox_2);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));

        gridLayout_2->addWidget(lineEdit_password, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(150, 210, 100, 80));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_login = new QPushButton(frame_2);
        pushButton_login->setObjectName(QString::fromUtf8("pushButton_login"));

        verticalLayout->addWidget(pushButton_login);

        pushButton_regist = new QPushButton(frame_2);
        pushButton_regist->setObjectName(QString::fromUtf8("pushButton_regist"));

        verticalLayout->addWidget(pushButton_regist);

        ui_login->setCentralWidget(centralwidget);

        retranslateUi(ui_login);

        QMetaObject::connectSlotsByName(ui_login);
    } // setupUi

    void retranslateUi(QMainWindow *ui_login)
    {
        ui_login->setWindowTitle(QApplication::translate("ui_login", "MainWindow", nullptr));
        label->setText(QApplication::translate("ui_login", "ColaSql", nullptr));
        groupBox->setTitle(QApplication::translate("ui_login", "\347\224\250\346\210\267\345\220\215", nullptr));
        groupBox_2->setTitle(QApplication::translate("ui_login", "\345\257\206\347\240\201", nullptr));
        pushButton_login->setText(QApplication::translate("ui_login", "\347\231\273\345\275\225", nullptr));
        pushButton_regist->setText(QApplication::translate("ui_login", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ui_login: public Ui_ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_LOGIN_H
