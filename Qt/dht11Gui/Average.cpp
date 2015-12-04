#include "Average.h"
#include "ui_Average.h"
#include <QDebug>
#include <QPainter>
#include <QEvent>
#include <QString>
#include <QMessageBox>
#include <QTimer>
Average::Average(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Average)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("xd1.png"));

    averageHumidity = 0;
    averageTemperature = 0;
    timer = new QTimer();
    //设置定时器，每五秒更新一次
    timer->setInterval(5000);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->widgetHumidity->installEventFilter(this);
    ui->widgetTemperature->installEventFilter(this);

    connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(readFile(QDate)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));

    timer->start();

}

Average::~Average()
{
    delete ui;
}

bool Average::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->widgetHumidity && e->type() == QEvent::Paint)
    {
        paintToHumidity(ui->widgetHumidity, averageHumidity);
        return true;
    }
    else if(obj == ui->widgetTemperature && e->type() == QEvent::Paint)
    {
        paintToTemperature(ui->widgetTemperature, averageTemperature);
        return true;
    }
    else
        return QWidget::eventFilter(obj, e);
}

void Average::paintToHumidity(QWidget *w, int value)
{
    QPainter p(w);
    QString str;
    str.setNum(value);
    p.setBrush(Qt::yellow);
    p.drawRect(0,0,w->width(),w->height());
    p.setBrush(Qt::green);
    p.drawRect(0, 0, (w->width())*(value)/100, w->height());
    p.drawText(w->width()/2,w->height()/2, str);
}

void Average::paintToTemperature(QWidget *w, int value)
{
    QPainter p(w);
    QString str;
    str.setNum(value);
    p.setBrush(Qt::yellow);
    p.drawRect(0,0,w->width(), w->height() );

    p.setBrush(Qt::red);
    p.drawRect(0,0,w->width()*value/100,w->height());
    p.drawText(w->width()/2,w->height()/2, str);
}

void Average::readFile(QDate date)
{
    countAverage(date.toString("yyyyMMdd.txt"), averageHumidity, averageTemperature);
}

int Average::countAverage(const QString &fileName, double& t, double& h)
{
    int sum1 = 0, sum2 = 0;
    int line = 0;
    QFile f(fileName);
    //if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    if(!f.open(QFile::ReadOnly))
    {
        averageHumidity = 0;
        averageTemperature = 0;
        this->update();
        return -1;
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        bool ok;
        lineStr = txtInput.readLine();
        QStringList list = lineStr.split("    ");
        QStringList tlist = list[1].split(" ");
        QStringList hlist = list[2].split(" ");
        sum1 += tlist[2].toInt(&ok);
        sum2 += hlist[2].toInt(&ok);
        line++;
    }
    t = double(sum1)/line;
    h = double(sum2)/line;
    this->update();
    f.close();
    return 0;
}

void Average::slotTimeOut()
{
    readFile(ui->dateEdit->date());
}
