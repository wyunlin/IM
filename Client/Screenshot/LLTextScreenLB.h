#pragma once
#include <qlabel.h>
#include <QMouseEvent>
class LLTextScreenLB :
	public QLabel
{
	Q_OBJECT
public:
	LLTextScreenLB(QWidget *parent);
	~LLTextScreenLB();


	QRect mRect;
	int type = 0;


	bool isContain = false;

	bool isLeftPressDown;
	QPoint dragPosition;


	bool hasEdit = false;//编辑过了的，需要过滤掉

signals:

	void  wakeupTextInput();
protected:

	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);

	void enterEvent(QEvent *e);
	
	void leaveEvent(QEvent *e);

	bool eventFilter(QObject *obj, QEvent *event);
};

