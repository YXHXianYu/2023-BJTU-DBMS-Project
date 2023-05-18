#include "columndialog.h"

ColumnDialog::ColumnDialog(QWidget* parent) : QDialog(parent)
{
    // 创建三个QLineEdit输入框，并将它们添加到布局中
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* hLayout = new QHBoxLayout();
    QLabel* label = new QLabel("插入数据库:", this);
    lineEdit4 = new QLineEdit(this);
    hLayout->addWidget(label);
    hLayout->addWidget(lineEdit4);
    layout->addLayout(hLayout);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    QLabel* label1 = new QLabel("插入表:", this);
    lineEdit1 = new QLineEdit(this);
    hLayout1->addWidget(label1);
    hLayout1->addWidget(lineEdit1);
    layout->addLayout(hLayout1);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    QLabel* label2 = new QLabel("字段名:", this);
    lineEdit2 = new QLineEdit(this);
    hLayout2->addWidget(label2);
    hLayout2->addWidget(lineEdit2);
    layout->addLayout(hLayout2);

    QHBoxLayout* hLayout3 = new QHBoxLayout();
    QLabel* label3 = new QLabel("类型:", this);
    //  lineEdit3 = new QLineEdit(this);
    comboBox = new QComboBox(this);
    comboBox->addItem("INT");
    comboBox->addItem("FLOAT");
    comboBox->addItem("STRING");
    hLayout3->addWidget(label3);
    hLayout3->addWidget(comboBox);
    layout->addLayout(hLayout3);

    // 添加两个按钮：确认和返回
    QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
    okButton = buttonBox->addButton("OK", QDialogButtonBox::AcceptRole);
    cancelButton = buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    layout->addWidget(buttonBox);

    // 连接确认按钮的信号和槽函数
    connect(okButton, &QPushButton::clicked, this, &ColumnDialog::acceptValues);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    // 设置窗口标题和大小
    setWindowTitle("输入");
    QIcon icon = QIcon(":/images/Colasql.png");
    setWindowIcon(icon);
    resize(300, 150);
}
// 返回输入框内容的函数
QStringList ColumnDialog::getValues()
{
    QStringList values;
    values << lineEdit1->text() << lineEdit2->text() << lineEdit3->text();
    return values;
}

// 确认按钮的槽函数
void ColumnDialog::acceptValues() { accept(); }
