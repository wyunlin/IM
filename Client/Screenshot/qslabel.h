#ifndef QSLABEL_H
#define QSLABEL_H
#include <QMetaType>
#include <QLabel>
#include <QWidget>
#include <QTextEdit>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRgb>

#include "LLTextScreenLB.h"
class Screen;

typedef struct myLine {
	QPen pen;
	QPoint startPoint;
	QPoint endPoint;
	int type = 0;
}myLine;
Q_DECLARE_METATYPE(myLine*)
typedef struct myRectangle {
	QPen pen;
	QPoint startPoint;
	QPoint endPoint;
	int type = 0;
}myRectangle;
Q_DECLARE_METATYPE(myRectangle*)
typedef struct myRound {
	QPen pen;
	QPoint startPoint;
	QPoint endPoint;
	int type = 0;
}myRound;
Q_DECLARE_METATYPE(myRound*)
typedef struct myArrow {
	QPen pen;
	QBrush brush;
	QPoint startPoint;
	QPoint endPoint;
	int type = 0;
}myArrow;
Q_DECLARE_METATYPE(myArrow*)
typedef struct myText {
	QString mText;
	QRect mRect;
	int type = 0;
}myText;
Q_DECLARE_METATYPE(myText*)

class QSLabel : public QLabel
{
	Q_OBJECT
public:
	QSLabel(QWidget *parent=nullptr);
	~QSLabel();

	void setdrawlineenable();
	void setrectangleenable();
	void setdrawarrowenable();
	void setroundenable();
	void settexteditenable();
	void settextedittovector();
	void drawarrow(QPoint startpoint, QPoint endpoint, QPainter &p);
	void setimagetolabel(const QImage &image);
	QImage resultimage();
	void undoDealWith();
	void slotBottomRight(QPointF brPoint);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void keyPressEvent(QKeyEvent *e);

public slots:
	void ontextchanged();
	void setTextinputWakeup();//唤醒输入框
public:
	QTextEdit	*m_plaintextedit;
	QPen		linePen;
	QPen		rectPen;
	QPen		roundPen;
	QBrush		brush;
	QPen		arrowPen;
	QPen		textPen;
	QFont		*font;
	QList<QVariant*> shapes;
	//QHash<int, QColor>	arrowHash;
private:
	QPoint startPoint;
	QPoint endPoint;
	QVector<myLine*> lines;
	QVector<myRectangle*> rectangles;
	QVector<myRound*> rounds;
	//QVector<myText*> texts;


	QVector<LLTextScreenLB*> lltexts;
	bool needPaint = false; 


	//bool label_edit = false;

	LLTextScreenLB *chooseAgainLB =NULL;

	QVector<myArrow*> arrows;
	
	QString text;
	QImage selectimage;

	bool ispressed;
	bool isdrawline;
	bool isdrawrectangle;
	bool isdrawround;
	bool istextedit;
	bool isdrawarrow;
	Screen *screen;
	QPointF m_brPoint;
	int width;
	int height;
	int index;
};

#endif // QSLABEL_H
