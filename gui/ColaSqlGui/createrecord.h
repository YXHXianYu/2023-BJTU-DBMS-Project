#ifndef CREATERECORD_H
#define CREATERECORD_H

#include <QMainWindow>

namespace Ui {
class createrecord;
}

class createrecord : public QMainWindow
{
    Q_OBJECT

public:
    explicit createrecord(QWidget *parent = nullptr);
    ~createrecord();

private:
    Ui::createrecord *ui;
};

#endif // CREATERECORD_H
