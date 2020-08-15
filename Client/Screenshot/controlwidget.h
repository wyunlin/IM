#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <qtoolbutton.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "qslabel.h"
#include "screen.h"
#include "Colorwidget.h"


//仿照QQ截图的控制面板
class ControlWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ControlWidget(QWidget *parent = 0);

	void connectSignalAndSlot();

	void setScreenQuote(Screen *screen);

	~ControlWidget();

signals:
	void emitslot();
	void emitundo();
	void emitResize();
	void emitHide();
	void emitRectResize();
	void emitRectHide();
	void emitRoundResize();
	void emitRoundHide();
	void emitArrowResize();
	void emitArrowHide();
	void emitTextResize();
	void emitTextHide();
	//void emitClose();

public slots:
	void cancelBtn_slot();

	void saveBtn_slot();

	void finishBtn_slot();

public slots:
	void on_textEditBtn_clicked();


	void on_rectangleBtn_clicked();

	void on_drawRoundBtn_clicked();

	void on_arrowBtn_clicked();

	void on_mosaicBtn_clicked();

	void on_returneditBtn_clicked();

	void on_drawLineBtn_clicked();

protected:
	//void mousePressEvent(QMouseEvent *event);
	//bool eventFilter(QObject *watched, QEvent *event);

private:
	Screen				*screen = nullptr;
	QPainter			pt;
	QImage				bg_gray;
	QToolButton			*rectangleBtn;		//正方形
	QToolButton			*drawRoundBtn;		//圆
	QToolButton			*arrowBtn;			//箭头
	QToolButton			*drawLineBtn;		//画线
	QToolButton			*textEditBtn;		//文本
	QToolButton			*saveBtn;			//保存图片
	QToolButton			*returneditBtn;		//撤销
	QToolButton			*cancelBtn;			//取消
	QToolButton			*finishBtn;			//完成截图
	QHBoxLayout			*horizontalLayout;
    QWidget             *m_widget = nullptr;
    QHBoxLayout         *m_mainLayout = nullptr;
	//ColorWidget			*m_colorWidget;
	//QVBoxLayout			*m_vLayout;
public:
	bool				rectangleClicked;
	bool				drawRoundClicked;
	bool				arrowClicked;
	bool				drawLineClicked;
	bool				textEditClicked;
	bool				m_rectangleClicked;
	bool				m_drawRoundClicked;
	bool				m_arrowClicked;
	bool				m_drawLineClicked;
	bool				m_textEditClicked;
};

#endif  //CONTROLWIDGET_H
