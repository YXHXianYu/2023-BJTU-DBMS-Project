/********************************************************************************
** Form generated from reading UI file 'ui_register.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_REGISTER_H
#define UI_UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ui_register
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_password1;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QLineEdit *lineEdit_password2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_account;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_regist;
    QPushButton *pushButton_back;

    void setupUi(QMainWindow *ui_register)
    {
        if (ui_register->objectName().isEmpty())
            ui_register->setObjectName(QString::fromUtf8("ui_register"));
        ui_register->resize(400, 300);
        ui_register->setMinimumSize(QSize(400, 300));
        ui_register->setMaximumSize(QSize(400, 300));
        centralwidget = new QWidget(ui_register);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(75, 0, 250, 221));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, 9, 9);
        lineEdit_password1 = new QLineEdit(groupBox_2);
        lineEdit_password1->setObjectName(QString::fromUtf8("lineEdit_password1"));

        gridLayout_2->addWidget(lineEdit_password1, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 9, 9);
        lineEdit_password2 = new QLineEdit(groupBox_3);
        lineEdit_password2->setObjectName(QString::fromUtf8("lineEdit_password2"));

        gridLayout_4->addWidget(lineEdit_password2, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_3, 2, 0, 1, 1);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 9, 9);
        lineEdit_account = new QLineEdit(groupBox);
        lineEdit_account->setObjectName(QString::fromUtf8("lineEdit_account"));

        gridLayout->addWidget(lineEdit_account, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(150, 210, 100, 80));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_regist = new QPushButton(frame_2);
        pushButton_regist->setObjectName(QString::fromUtf8("pushButton_regist"));

        verticalLayout->addWidget(pushButton_regist);

        pushButton_back = new QPushButton(frame_2);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));

        verticalLayout->addWidget(pushButton_back);

        ui_register->setCentralWidget(centralwidget);

        retranslateUi(ui_register);

        QMetaObject::connectSlotsByName(ui_register);
    } // setupUi

    void retranslateUi(QMainWindow *ui_register)
    {
        ui_register->setWindowTitle(QApplication::translate("ui_register", "MainWindow", nullptr));
        groupBox_2->setTitle(QApplication::translate("ui_register", "\345\257\206\347\240\201", nullptr));
        groupBox_3->setTitle(QApplication::translate("ui_register", "\351\207\215\345\244\215\345\257\206\347\240\201", nullptr));
        groupBox->setTitle(QApplication::translate("ui_register", "\347\224\250\346\210\267\345\220\215", nullptr));
        pushButton_regist->setText(QApplication::translate("ui_register", "\346\263\250\345\206\214", nullptr));
        pushButton_back->setText(QApplication::translate("ui_register", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ui_register: public Ui_ui_register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_REGISTER_H
