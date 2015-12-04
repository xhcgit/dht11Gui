#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QDate>

#include "Limit.h"
#include "MyBeep.h"
#include "Average.h"
#include "Change.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

    QFile* file;
    QTimer* timer;
    Limit* limit;
    Average* average;
    Change* change;
private:
    Ui::MainWindow *ui;
    char* buf;
    int curHumidity;
    int curTemperature;
    int maxHumidity;
    int minHumidity;
    int maxTemperature;
    int minTemperature;
    bool isSetLimit;
    QDate *curDate;
    QMessageBox* messagebox;
    MyBeep* beep;
    QFile* logEveryay;


public slots:
    void slotTimeOut();
    //void slotOutRange(const QString& message);
    void slotSetLimit(int maxH, int minH, int maxT, int minT);
    void slotCancelLimit();
signals:
    void revData(char* buf, int len);
private slots:
    void on_actionLimit_triggered();
    void on_actionBeepFreq_triggered();
    void on_actionAverage_triggered();
    void on_actionChange_triggered();
};

#endif // MAINWINDOW_H
