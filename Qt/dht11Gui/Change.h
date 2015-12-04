#ifndef CHANGE_H
#define CHANGE_H

#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QDate>
#include <QMouseEvent>

namespace Ui {
class Change;
}

class Data
{
public:
    Data(int H = 0, int T = 0, QString str = "000000"):
        curHumidity(H), curTemperature(T), strTime(str){}
    int curHumidity;
    int curTemperature;
    QString strTime;
};

class Change : public QWidget
{
    Q_OBJECT

public:
    explicit Change(QWidget *parent = 0);
    ~Change();
    void paintEvent(QPaintEvent *);    
    QVector<Data>* getData(const QString &fileName);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Change *ui;
    QTimer* timer;
    QVector<Data>* data;
    int space;
    int spaceTime;
    bool powerFlagH;
    bool powerFlagT;
    QPoint point;

public slots:
    void slotTimeOut();
    void readFile(QDate date);
    void powerH();
    void powerT();
signals:
};

#endif // CHANGE_H
