#ifndef AVERAGE_H
#define AVERAGE_H

#include <QWidget>
#include <QFile>
#include <QDate>

namespace Ui {
class Average;
}

class Average : public QWidget
{
    Q_OBJECT

public:
    explicit Average(QWidget *parent = 0);
    ~Average();
    double averageHumidity;
    double averageTemperature;
    bool eventFilter(QObject *obj, QEvent *e);
    void paintToHumidity(QWidget *w, int value);
    void paintToTemperature(QWidget *w, int value);
private:
    Ui::Average *ui;
    QTimer* timer;

public slots:
    void readFile(QDate date);
    int countAverage(const QString& fileName, double& t, double& h);
    void slotTimeOut();
};

#endif // AVERAGE_H
