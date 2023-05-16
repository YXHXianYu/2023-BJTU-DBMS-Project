#include "createrecord.h"
#include "ui_createrecord.h"

createrecord::createrecord(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::createrecord)
{
    ui->setupUi(this);
}

createrecord::~createrecord()
{
    delete ui;
}
