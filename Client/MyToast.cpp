#include "MyToast.h"
#include <QPropertyAnimation>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QTimer>

#include <QGridLayout>

MyToast::MyToast(QString warn, int span, QWidget *parent)
	: QDialog(parent)
{
	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Tool);// 无边框 无任务栏
	//this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);   // 背景透明
	setAttribute(Qt::WA_DeleteOnClose);

	//setMaximumSize(300, 60);
	//setMinimumSize(300, 60);
	resize(300, 60);

	setStyleSheet("background-color:rgb(102,102,102);color:white;border:0px solid red;");

	// setStyleSheet("background-color:red;color:white;border:0px solid red");


	setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::Dialog);
	titleLB = new QLabel(this);
	titleLB->setText(warn);
	titleLB->setAlignment(Qt::AlignCenter);
	titleLB->setGeometry(0, 0, width(), height());
	//QGridLayout* warnLayout= new QGridLayout(this);
	//warnLayout->addWidget(titleLB, 0, 0, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter);



	mySpan = span;
	//定时器
	timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(span);
	connect(timer, &QTimer::timeout, [=] {



		close();

		deleteLater();

	});
	timer->start();
}

MyToast::~MyToast()
{
}

void MyToast::setBackgroundColor(QString r)
{
	QString str = QString("background-color:%1;border:0px solid red;background:transparent;").arg(r);
	setStyleSheet(str);
}
void MyToast::setTextColor(QString r)
{

	QString str = QString("color:%1;").arg(r);
	titleLB->setStyleSheet(str);
}

void MyToast::showTip(const QString& text, QWidget* parent /*= nullptr*/)
{
	//	LLToast* toast = new LLToast(parent);
	//	toast->setWindowFlags(toast->windowFlags() | Qt::WindowStaysOnTopHint); // 置顶
	//	toast->setText(text);
	//	toast->adjustSize();    //设置完文本后调整下大小
	//
	//	// 测试显示位于主屏的70%高度位置
	//	QScreen* pScreen = QGuiApplication::primaryScreen();
	//	toast->move((pScreen->size().width() - toast->width()) / 2, pScreen->size().height() * 7 / 10);
	//	toast->showAnimation();
	//
}
