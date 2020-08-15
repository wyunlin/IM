#ifndef SCREEN_H
#define SCREEN_H
#include <QWidget>
#include "myrect.h"
#include "qslabel.h"
#include "controlwidget.h"
#include "Colorwidget.h"
#include "FontSelect.h"
enum WZType {
    WZ_RECT1, WZ_RECT2, WZ_RECT3, WZ_RECT4, WZ_RECT, WZ_NO
};


class QSLabel;
class ControlWidget;

class Screen : public QWidget
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = 0);
    void drawline();
    void textedit();
    void drawarrow();
    void drawround();
    void drawrectang();

    //绘制控制选区，也就是四个小三角
    void drawControlArea(QPainter &);

    bool comparePoint(QPoint &p1, QPoint &p2);

    bool compareRect(QRectF &r1, QRectF &r2);

    void CoordinateTrans(QRectF &rect);             //坐标转换

	void posPoint(QWidget *widget);

	WZType pointInWhere(QPoint p);
    //返回截取之后的图片
    QPixmap getGrabPixmap();

    QPainterPath getPath();
    //保存截图
    void savePixmap();

	void setColor(QColor &color);
	void setSize(int size);

    virtual ~Screen();
    void Exit();

protected:
	void paintEvent(QPaintEvent *);

	void mousePressEvent(QMouseEvent *);

	void mouseReleaseEvent(QMouseEvent *);

	void mouseMoveEvent(QMouseEvent *);

	void setselectimagelabel(QMouseEvent* event);

	void keyPressEvent(QKeyEvent *e);

private:
    bool   pointInRect(QPoint &p, QRectF &r);

signals:
    void emitClose();
	void emitPixmap();
public slots:
    void slotClose();
	void slotControlResize();
	//void slotControlHide();
	void slotRectResize();
	//void slotRectHide();
	void slotRoundResize();
	void slotArrowResize();
	//void slotRoundHide();
	void slotTextResize();
	//void slotTextHide();
	void slotUndo();
	void slotFontSize(int size);
	void slotFontColor(QColor color);
	void slotShow();

private:
    QPixmap pixmap;               //全屏
    QPoint pressedPoint;          //鼠标左键按下后的坐标
    QPoint movePoint;             //终点坐标
    QPainterPath globalPath;      //保存全局路径
    bool         oncePress;       //是不是第一次左键按下
    MyRect       widthInfoRect;//截取区域的长宽信息矩形
    //控制选区变化的正方形
    QRectF       rect1;           //第一象限的正方形
    QRectF       rect2;           //第二象限的正方形
    QRectF       rect3;           //第三象限的正方形
    QRectF       rect4;           //第四象限的正方形
    QRectF       rect;            //代表选区的正方形
	WZType         type;
    QPoint       oldPoint;        //原先的坐标
    QWidget     *control;
    QRect rec;
    QSLabel *labelimage;
	QHBoxLayout *layout;
	bool		isShow;
public:
    ControlWidget	*controlUi;
	ColorWidget		*m_colorWidget;
	ColorWidget		*m_colorWidget2;
	ColorWidget		*m_colorWidget3;
	ColorWidget		*m_colorWidget4;
	FontSelect		*m_fontSelect;
};

#endif // SCREEN_H
