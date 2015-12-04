#include "Limit.h"
#include "ui_Limit.h"

Limit::Limit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Limit)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("xd1.png"));
    connect(ui->SliderMaxH, SIGNAL(sliderMoved(int)), ui->spinBoxMaxH, SLOT(setValue(int)));
    connect(ui->SliderMinH, SIGNAL(sliderMoved(int)), ui->spinBoxMinH, SLOT(setValue(int)));
    connect(ui->SliderMaxT, SIGNAL(sliderMoved(int)), ui->spinBoxMaxT, SLOT(setValue(int)));
    connect(ui->SliderMinT, SIGNAL(sliderMoved(int)), ui->spinBoxMinT, SLOT(setValue(int)));

    connect(ui->spinBoxMaxH, SIGNAL(valueChanged(int)), ui->SliderMaxH, SLOT(setValue(int)));
    connect(ui->spinBoxMinH, SIGNAL(valueChanged(int)), ui->SliderMinH, SLOT(setValue(int)));
    connect(ui->spinBoxMaxT, SIGNAL(valueChanged(int)), ui->SliderMaxT, SLOT(setValue(int)));
    connect(ui->spinBoxMinT, SIGNAL(valueChanged(int)), ui->SliderMinT, SLOT(setValue(int)));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSendLimit()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(slotCancelLimit()));

}

Limit::~Limit()
{
    delete ui;
}

void Limit::slotSendLimit()
{
    emit sendLimitData(ui->spinBoxMaxH->value(),
                       ui->spinBoxMinH->value(),
                       ui->spinBoxMaxT->value(),
                       ui->spinBoxMinT->value());
}

void Limit::slotCancelLimit()
{
    emit cancelLimit();
    this->hide();
}
