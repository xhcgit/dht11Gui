#ifndef MYBEEP_H
#define MYBEEP_H

#include <QWidget>
#include "systemcontrol.h"

namespace Ui {
class MyBeep;
}

class MyBeep : public QWidget
{
    Q_OBJECT

public:
    explicit MyBeep(QWidget *parent = 0);
    ~MyBeep();
    int powerFlag;

public slots:
    void powerBeep();
    void setValue(int value);
    void closeBeep();

private slots:
    void on_buttonPowerOn_clicked();

    void on_buttonPowerOff_clicked();

private:
    int freqBeep;
    Ui::MyBeep *ui;
};

#endif // MYBEEP_H
