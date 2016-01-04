#include "Change.h"
#include "ui_Change.h"
#include <QDate>
#include <QFile>
#include <QPainter>
#include <QMouseEvent>
#include <QString>
#include <QDebug>


#define EVERY_LINE_SIZE 36
#define TIME_BETWEEN_MESSAGE 3
#define MAX_POINT_COUNT 220

Change::Change(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Change)
{
	ui->setupUi(this);
	ui->dateEdit->setDate(QDate::currentDate());
	ui->checkBoxH->setCheckState(Qt::Checked);
	ui->checkBoxT->setCheckState(Qt::Checked);
	this->setWindowIcon(QIcon("xd1.png"));
	space = 1;
	spaceTime = 0.05; // 0.05minute = 3 second
	data = new QVector<Data>;
	point = QPoint(0,0);

	powerFlagH = true;
	powerFlagT = true;
	timer = new QTimer();
	timer->setInterval(5000);

	connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
	connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(readFile(QDate)));
	connect(ui->checkBoxH, SIGNAL(clicked()), this, SLOT(powerH()));
	connect(ui->checkBoxT, SIGNAL(clicked()), this, SLOT(powerT()));

	timer->start();

	readFile(ui->dateEdit->date());


}

Change::~Change()
{
	delete ui;
}

void Change::paintEvent(QPaintEvent *)
{

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	//TODO
	p.drawRect(0, 30, 480, 180);//Can use QGraphics
	p.drawLine(20, 30, 20, 210);
	p.drawText(3, 40, "100");
	p.drawText(8, 210, "0");
	p.drawText(5,197,"10");
	p.drawText(5,178,"20");
	p.drawText(5,160,"30");
	p.drawText(5,142,"40");
	p.drawText(5,122,"50");
	p.drawText(5,105,"60");
	p.drawText(5,88,"70");
	p.drawText(5,70,"80");
	p.drawText(5,52,"90");
	p.setPen(QPen(Qt::DashLine));

	for(int i = 1;i < 10; i++)
	{
		p.drawLine(20, 210 - i*18, 480, 210 - i*18);
	}

	p.setPen(QPen(Qt::blue, 3));
	p.drawLine(20, 10, 60, 10);
	p.setPen(QPen(Qt::red, 3));
	p.drawLine(20, 20, 60, 20);

	p.setPen(QPen(Qt::black, 1));
	p.drawText(65, 12, "H");
	p.drawText(65, 23, "T");

	if((this->point.x() >= 30 && this->point.x() <= 470) && (powerFlagH || powerFlagT))
	{
        p.setPen(QPen(Qt::DashLine));
        p.drawLine(point.x(), 30, point.x(), 210);
		int index = (470 - point.x())/2;
		Data temp;
		if(data->size() > index)
		{
			temp = data->at(index);
			temp.strTime.insert(2,':');
			temp.strTime.insert(5,':');
			p.drawText(50, 225, "Time: " + temp.strTime + " H = " +
					   QString::number(temp.curHumidity) +
					   " T = " +
					   QString::number(temp.curTemperature));
		}

	}

	if(data->size() != 0)
	{
		Data temp;
		QPoint lastHPoint;
		QPoint lastTPoint;
		for(int i = 0; i < data->size(); ++i)
		{
			temp = data->at(i);
			if(powerFlagH)
			{
				p.setPen(QPen(Qt::blue, 2));
				p.drawPoint(470 - i*2, 210 - temp.curHumidity * 1.8);
			}

			if(powerFlagT)
			{
				p.setPen(QPen(Qt::red, 2));
				p.drawPoint(470 - i*2, 210 - temp.curTemperature * 1.8);
			}

			if(i != 0)
			{
				if(powerFlagH)
				{
					p.setPen(QPen(Qt::blue, 1));
					p.drawLine(QPoint(470 - i*2, 210 - temp.curHumidity * 1.8), lastHPoint);
				}

				if(powerFlagT)
				{
					p.setPen(QPen(Qt::red, 1));
					p.drawLine(QPoint(470 - i*2, 210 - temp.curTemperature * 1.8), lastTPoint);
				}
			}
			if(i != 0)
				temp = data->at(i-1);
			lastHPoint = QPoint(470 - i*2, 210 - temp.curHumidity * 1.8);
			lastTPoint = QPoint(470 - i*2, 210 - temp.curTemperature * 1.8);
		}


		if(powerFlagH || powerFlagT)
		{
			spaceTime = space * TIME_BETWEEN_MESSAGE;
			p.setPen(QPen(Qt::black, 1));
			p.drawText(280, 225,"Space: " + QString::number(spaceTime) + "s");
		}
	}
	//else
		//TODO

}

void Change::readFile(QDate date)
{
	if(data != NULL)
		delete data;
	data = getData(date.toString("yyyyMMdd.txt"));
	if(data->size() == 0)
		qDebug() << "open LogFile error";
	this->update();
}

void Change::powerH()
{
	if(ui->checkBoxH->isChecked())
		this->powerFlagH = true;
	else
		this->powerFlagH = false;
	readFile(ui->dateEdit->date());
}

void Change::powerT()
{
	if(ui->checkBoxT->isChecked())
		this->powerFlagT = true;
	else
		this->powerFlagT = false;
	readFile(ui->dateEdit->date());
}

QVector<Data> *Change::getData(const QString &fileName)
{
	QVector<Data>* temp = new QVector<Data>;
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		//TODO
		return temp;
	int fileSize = file.size();
	int fileLine = fileSize / EVERY_LINE_SIZE;
	space = fileLine / MAX_POINT_COUNT;
	if(space == 0)
		space ++;
	fileSize = fileSize - EVERY_LINE_SIZE;
	file.seek(fileSize);

	//max 220 points
	for(int n = 0; n < MAX_POINT_COUNT; n++)
	{
		if(n == 0)
			fileSize -= EVERY_LINE_SIZE;
		else
			fileSize -= (space * EVERY_LINE_SIZE);
		if(fileSize < 0)
			break;
		file.seek(fileSize);
		QByteArray line = file.readLine();
		QList<QByteArray> list = line.split('\n').at(0).split(' ');

		QByteArray time = list.at(0);
		QList<QByteArray> timeList = time.split('-');

		QString strTime = timeList.at(1);
		QString strHumidity = list.at(6);
		QString strTemperature = list.at(12);

		temp->append(Data(strHumidity.toInt(), strTemperature.toInt(), strTime));
	}
	file.close();
	return temp;
}

void Change::mousePressEvent(QMouseEvent *event)
{
	QPoint pt = event->pos();
	if(pt.x()%2 != 0)
		pt.setX(pt.x()-1);
	this->point = pt;
	update();
}

void Change::slotTimeOut()
{
	readFile(ui->dateEdit->date());
}
