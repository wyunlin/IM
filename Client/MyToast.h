#pragma once

#include <QDialog>
#include <qlabel.h>

#include <QTimer>

class MyToast : public QDialog
{
	Q_OBJECT

public:
	MyToast(QString warn, int span,QWidget *parent=nullptr);
	~MyToast();
	void setBackgroundColor(QString c);
	void setTextColor(QString c);

	//void setText(const QString& text);

	//void showAnimation(int timeout = 2000);// 动画方式show出，默认2秒后消失

public:
	// 静态调用
	static void showTip(const QString& text, QWidget* parent = nullptr);

private:

	QLabel* titleLB;

	QTimer *timer;

	int mySpan;

};
