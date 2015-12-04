#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QDebug>
#include <QLCDNumber>
#include <QString>
#include <QDateTime>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("xd1.png"));

    limit = new Limit();
    limit->setWindowTitle("Set Limit");

    //messagebox = new QMessageBox();
    beep = new MyBeep();
    beep->setWindowTitle("Set Beep");

    logEveryay = new QFile();

    average = new Average();

    change = new Change();

    curDate = new QDate();

    buf = new char[5];
    curHumidity = 0;
    curTemperature = 0;
    maxHumidity = 0;
    minHumidity = 0;
    maxTemperature = 0;
    minTemperature = 0;

    isSetLimit = false;

    file = new QFile("/dev/dht11");
    //file = new QFile("test.txt");
    timer = new QTimer();
    if(file->open(QFile::ReadOnly))
    {
        timer->setInterval(3000);
        connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
        timer->start();
    }
    else
    {
        QMessageBox* messagebox = new QMessageBox(this);
        messagebox->setText("Open Error");
        messagebox->setWindowTitle("Warring");
        messagebox->show();
    }
    connect(limit, SIGNAL(sendLimitData(int,int,int,int)), this, SLOT(slotSetLimit(int,int,int,int)));
    connect(limit, SIGNAL(cancelLimit()), this, SLOT(slotCancelLimit()));
}

MainWindow::~MainWindow()
{
    logEveryay->close();
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(Qt::white);
    p.drawRect(0,0,480,272);
    QTransform transform;
    transform.scale(0.8, 0.8);
    p.setTransform(transform);
    p.drawPixmap(QPoint(110,50), QPixmap("xd.png"));
}


void MainWindow::slotTimeOut()
{ 
    logEveryay->setFileName(QDateTime::currentDateTime().toString("yyyyMMdd") + ".txt");
    if(!logEveryay->isOpen())
        logEveryay->open(QFile::ReadWrite|QFile::Append);
    if(file->read(buf, 5) != -1)
    {
        curHumidity = int(*(buf+0));
        curTemperature = int(*(buf+2));

        if(!(((curHumidity >= 20)&&(curHumidity <= 90))&&
             ((curTemperature >= 0)&&(curTemperature <= 50))))
            return;
        ui->lcdHumidity->display(curHumidity);
        ui->lcdTemperature->display(curTemperature);

        QTextStream logText(logEveryay);
        if(curHumidity < 10 || curTemperature < 10)
        {

            logText << QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")<<"    ";
            if(curHumidity < 10)
                logText<<"H = 0" << curHumidity << "    ";
            else
                logText<<"H = " << curHumidity << "    ";
            if(curTemperature < 10)
                logText<<"T = 0" << curTemperature << "\n";
            else
                logText<<"T = " << curTemperature << "\n";
        }
        else
        {
            logText << QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")
                    <<"    "
                   <<"H = "
                  <<curHumidity
                 <<"    "
                <<"T = "
               <<curTemperature
              <<"\n";
        }



        if(isSetLimit == true)
        {

            if(curHumidity >= maxHumidity)
            {
                //slotOutRange("The Humidity is too high");
                beep->powerBeep();
            }
            else if(curHumidity < minHumidity)
            {
                //slotOutRange("The Humidity is too low");
                beep->powerBeep();
            }
            else if(curTemperature >= maxTemperature)
            {
                //slotOutRange("The Temperature is too high");
                beep->powerBeep();
            }
            else if(curTemperature < minTemperature)
            {
                //slotOutRange("The Temperature is too low");
                beep->powerBeep();
            }
            else
                beep->closeBeep();
        }

    }

    else
    {
        QMessageBox* messagebox = new QMessageBox(this);
        messagebox->setText("Read Error");
        messagebox->setWindowTitle("Warring");
        messagebox->show();
    }
    logEveryay->close();
}
/*
void MainWindow::slotOutRange(const QString &message)
{
    messagebox->setText(message);
    if(messagebox->isHidden())
        messagebox->show();
}
*/
void MainWindow::slotSetLimit(int maxH, int minH, int maxT, int minT)
{
    maxHumidity = maxH;
    minHumidity = minH;
    maxTemperature = maxT;
    minTemperature = minT;
    isSetLimit = true;
}

void MainWindow::slotCancelLimit()
{
    isSetLimit = false;
}

void MainWindow::on_actionLimit_triggered()
{
    if(limit->isHidden())
    {
        limit->show();
    }
    else
        limit->hide();
}

void MainWindow::on_actionBeepFreq_triggered()
{
    if(beep->isHidden())
    {
        beep->show();
    }
    else
        beep->hide();
}

void MainWindow::on_actionAverage_triggered()
{
    if(average->isHidden())
    {
        average->show();
        average->readFile(QDate::currentDate());
    }
    else
        average->hide();
}

void MainWindow::on_actionChange_triggered()
{
    if(change->isHidden())
    {
        change->show();
    }
    else
        change->hide();
}
