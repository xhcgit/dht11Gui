#include "MyBeep.h"
#include "ui_MyBeep.h"

#ifdef linux
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <iostream>
#endif
#include <QFile>

using namespace std;
static int beep_fb;

MyBeep::MyBeep(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyBeep)
{
    ui->setupUi(this);
    freqBeep = 1;
    powerFlag = 1;
    connect(ui->sliderBeep, SIGNAL(sliderMoved(int)), ui->spinBoxBeep, SLOT(setValue(int)));
    connect(ui->spinBoxBeep, SIGNAL(valueChanged(int)), ui->sliderBeep, SLOT(setValue(int)));

    connect(ui->spinBoxBeep, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

    this->setWindowIcon(QIcon("xd1.png"));
#ifdef linux
    QFile f("/dev/beep");
    if (f.exists())
    {
        beep_fb = open("/dev/beep", O_RDWR);
        if (beep_fb < 0)
        {
            perror("open device leds fail");
        }
    }
#endif
}

MyBeep::~MyBeep()
{
    delete ui;
#ifdef linux
    ::close(beep_fb);
#endif
}

void MyBeep::powerBeep()
{
#ifdef linux
    ioctl(beep_fb, powerFlag, freqBeep);
#endif
}

void MyBeep::closeBeep()
{
#ifdef linux
    ioctl(beep_fb, 0, 1);//关闭蜂鸣器
#endif
}

void MyBeep::setValue(int value)
{
    freqBeep = value;
}

void MyBeep::on_buttonPowerOn_clicked()
{
   powerFlag = 1;
}

void MyBeep::on_buttonPowerOff_clicked()
{
    powerFlag = 0;
    closeBeep();
}
