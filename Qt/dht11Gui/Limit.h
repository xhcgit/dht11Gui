#ifndef LIMIT_H
#define LIMIT_H

#include <QWidget>
#include "systemcontrol.h"

namespace Ui {
class Limit;
}

class Limit : public QWidget
{
    Q_OBJECT

public:
    explicit Limit(QWidget *parent = 0);
    ~Limit();

private:
    Ui::Limit *ui;
signals:
    void sendLimitData(int, int, int, int);
    void cancelLimit();
public slots:
    void slotSendLimit();
    void slotCancelLimit();
};

#endif // LIMIT_H
