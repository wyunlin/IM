#pragma once

#include <QScrollBar>
#include <QWheelEvent>

class MyScrollBar : public QScrollBar
{
	Q_OBJECT

public:
	MyScrollBar(int type = 0, QWidget *parent = nullptr);
	~MyScrollBar();

protected:
	void wheelEvent(QWheelEvent *event);

};
