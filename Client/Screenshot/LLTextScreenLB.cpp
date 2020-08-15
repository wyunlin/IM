#include "LLTextScreenLB.h"

#include <QApplication>

LLTextScreenLB::LLTextScreenLB(QWidget *parent):QLabel(parent)
{

	setStyleSheet("background:transparent;border:none;color:red");

	this->installEventFilter(this);
}



void LLTextScreenLB::mouseReleaseEvent(QMouseEvent *event)
{

	if (event->button() == Qt::LeftButton ) {
		isLeftPressDown = false;

		dragPosition = event->globalPos() - pos();
		this->releaseMouse();

		this->setCursor(QCursor(Qt::ArrowCursor));

	}

	return QWidget::mouseReleaseEvent(event);
}
void LLTextScreenLB::enterEvent(QEvent *event)
{

	isContain = true;
	this->setCursor(QCursor(Qt::ArrowCursor));

	return QWidget::enterEvent(event);
}

void LLTextScreenLB::leaveEvent(QEvent *event)
{
	isContain = false;
	return QWidget::leaveEvent(event);
}

bool LLTextScreenLB::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == this)
	{
		//单击
	/*	if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				
			}
		}*/
		//双击
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			//QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

			emit wakeupTextInput();
			return true;

		}
	}
	
	return false;
}

void LLTextScreenLB::mousePressEvent(QMouseEvent *event)
{


	if (event->button() == Qt::LeftButton ) {
		isLeftPressDown = true;

		dragPosition = event->globalPos() - pos();
		this->releaseMouse();


	
		this->setCursor(QCursor(Qt::SizeAllCursor));
		//this->setCursor(QCursor(Qt::ArrowCursor));

	}

	return QWidget::mousePressEvent(event);
}

void LLTextScreenLB::mouseMoveEvent(QMouseEvent *event)
{

	if (isLeftPressDown && (event->buttons() && Qt::LeftButton )
		&& (event->globalPos() - dragPosition).manhattanLength() > QApplication::startDragDistance())
	{
		move(event->globalPos() - dragPosition);
		dragPosition = event->globalPos() - pos();
	}
	return QWidget::mouseMoveEvent(event);


}
LLTextScreenLB::~LLTextScreenLB()
{
}