#include "screen.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QFileDialog>
#include <QRectF>
#include "controlwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QApplication>
#include <QClipboard>
#include "myrect.h"

Screen::Screen(QWidget *parent)
	: QWidget(parent),
	widthInfoRect(QRectF(0, 0, 0, 0)),
	control(NULL),
	isShow{true}
{
	showFullScreen();//将窗口部件全屏显示
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	//this->setFocusPolicy(Qt::ClickFocus);
	//截取全屏
	QScreen *scrPix = QGuiApplication::primaryScreen();
	pixmap = scrPix->grabWindow(0);
	//全局路径的初始化，一个全屏闭合回路
	globalPath.lineTo(pixmap.width(), 0);
	globalPath.lineTo(pixmap.width(), pixmap.height());
	globalPath.lineTo(0, pixmap.height());
	globalPath.lineTo(0, 0);

	oncePress = true;
	//想到了添加一个bool labelimageShow()的函数的方法，找时间试一下	
	setMouseTracking(true);
	labelimage = new QSLabel(this);
	Qt::WindowFlags nType = (Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	labelimage->setWindowFlags(nType);
	labelimage->raise();
	type = WZ_NO;
	controlUi = new ControlWidget();	//新建控制窗口
	m_colorWidget = new ColorWidget();
	m_colorWidget2 = new ColorWidget();
	m_colorWidget3 = new ColorWidget();
	m_colorWidget4 = new ColorWidget();
	m_colorWidget4->hideBtnSize();
	m_fontSelect = new FontSelect;
	/*connect(m_colorWidget, &ColorWidget::emitShow, [=](QColorDialog &colorD) {
		Qt::WindowFlags nType = (Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
		colorD.setWindowFlags(nType);
		colorD.stackUnder(this);
		labelimage->stackUnder(this);
		colorD.raise();
		labelimage->raise();
		this->update();
	});*/
	connect(controlUi, &ControlWidget::emitResize, this, &Screen::slotControlResize);
	connect(controlUi, &ControlWidget::emitHide, [=]() {
		m_colorWidget->hide();
	});
	connect(controlUi, &ControlWidget::emitRectResize, this, &Screen::slotRectResize);
	connect(controlUi, &ControlWidget::emitRectHide, [=]() {
		m_colorWidget2->hide();
	});
	connect(controlUi, &ControlWidget::emitRoundResize, this, &Screen::slotRoundResize);
	connect(controlUi, &ControlWidget::emitRoundHide, [=]() {
		m_colorWidget3->hide();
	});
	connect(controlUi, &ControlWidget::emitArrowResize, this, &Screen::slotArrowResize);
	connect(controlUi, &ControlWidget::emitArrowHide, [=]() {
		m_colorWidget4->hide();
	});
	connect(controlUi, &ControlWidget::emitTextResize, this, &Screen::slotTextResize);
	connect(controlUi, &ControlWidget::emitTextHide, [=]() {
		m_fontSelect->hide();
	});
	connect(m_colorWidget, &ColorWidget::emitFontSize, [=](int size) {
		labelimage->linePen.setWidth(size); });
	connect(m_colorWidget2, &ColorWidget::emitFontSize, [=](int size) {
		labelimage->rectPen.setWidth(size); });
	connect(m_colorWidget3, &ColorWidget::emitFontSize, [=](int size) {
		labelimage->roundPen.setWidth(size); });
	connect(m_colorWidget, &ColorWidget::emitFontColor, [=](QColor color) {
		labelimage->linePen.setColor(color); });
	connect(m_colorWidget2, &ColorWidget::emitFontColor, [=](QColor color) {
		labelimage->rectPen.setColor(color); });
	connect(m_colorWidget3, &ColorWidget::emitFontColor, [=](QColor color) {
		labelimage->roundPen.setColor(color);});
	connect(m_colorWidget4, &ColorWidget::emitFontColor, [=](QColor color) {
		labelimage->arrowPen.setColor(color); 
		labelimage->brush.setColor(color); 
		//int size = labelimage->getArrowSize();
		//labelimage->arrowHash.insert(size, color); 
	});
	connect(m_fontSelect, &FontSelect::emitColor, [=](QColor color) {
		labelimage->m_plaintextedit->setTextColor(color);
		labelimage->textPen.setColor(color); });
	connect(m_fontSelect, &FontSelect::emitSize, [=](int size) {
		labelimage->m_plaintextedit->setFontPointSize(size);
		labelimage->font->setPointSize(size); });
	connect(m_fontSelect, &FontSelect::emitbtnClicked, [=](bool btnClicked) {
		if (btnClicked)
		{
			labelimage->m_plaintextedit->setFontWeight(QFont::Bold);
		}
		else
		{
			labelimage->m_plaintextedit->setFontWeight(QFont::Normal);
		}
		labelimage->font->setBold(btnClicked); });
	connect(m_fontSelect, &FontSelect::emitItalicClick, [=](bool btnClicked) {
		labelimage->m_plaintextedit->setFontItalic(btnClicked);
		labelimage->font->setItalic(btnClicked); });
	connect(m_fontSelect, &FontSelect::emitUnderlineClick, [=](bool btnClicked) {
		labelimage->m_plaintextedit->setFontUnderline(btnClicked);
		labelimage->font->setUnderline(btnClicked); });
	connect(m_fontSelect, &FontSelect::emitFontFamily, [=](QString str) {
		labelimage->m_plaintextedit->setFontFamily(str);
		labelimage->font->setFamily(str); });
	connect(controlUi, &ControlWidget::emitundo, this, &Screen::slotUndo);
}

void Screen::paintEvent(QPaintEvent *e)//将上一个函数得到的全屏灰色图绘制在painter上，并添加宽高信息矩形和边角拖曳小正方形
{
	QPainter paint(this);
	paint.drawPixmap(0, 0, pixmap);
	//初始化画笔操作
	paint.setPen(QColor(0, 191, 255));
	paint.setBrush(QColor(0, 0, 0, 100));
	//设置路径
	QPainterPath path = getPath();

	paint.drawPath(path);
	//绘制选取左上角显示宽高的区域
	widthInfoRect.drawMe(paint);
	//绘制四个小正方形
	drawControlArea(paint);
	QWidget::paintEvent(e);
}
//绘制正方形
void Screen::drawControlArea(QPainter &painter)
{
	//计算四个小正方形
	rect1.setX(movePoint.x() - 3);
	rect1.setY(pressedPoint.y() - 3);
	rect1.setWidth(6);
	rect1.setHeight(6);
	rect2.setX(pressedPoint.x() - 3);
	rect2.setY(pressedPoint.y() - 3);
	rect2.setWidth(6);
	rect2.setHeight(6);
	rect3.setX(pressedPoint.x() - 3);
	rect3.setY(movePoint.y() - 3);
	rect3.setWidth(6);
	rect3.setHeight(6);
	rect4.setX(movePoint.x() - 3);
	rect4.setY(movePoint.y() - 3);
	rect4.setWidth(6);
	rect4.setHeight(6);
	painter.save();
	painter.setBrush(Qt::blue);
	painter.drawRect(rect1);
	painter.drawRect(rect2);
	painter.drawRect(rect3);
	painter.drawRect(rect4);
	painter.restore();
}



//得到选区之外的路径
QPainterPath Screen::getPath()   //这个函数得到的是开始截图时候得到的白色选区
{
	//选取路径
	QPainterPath path;
	path.moveTo(pressedPoint.x(), pressedPoint.y());
	path.lineTo(movePoint.x(), pressedPoint.y());
	path.lineTo(movePoint.x(), movePoint.y());
	path.lineTo(pressedPoint.x(), movePoint.y());
	path.lineTo(pressedPoint.x(), pressedPoint.y());
	return globalPath.subtracted(path);    //从灰色全屏路径中扣掉这个白色路径区域，然后作为截取的图片？
}


void Screen::mousePressEvent(QMouseEvent *e)
{
	if (e->button() & Qt::LeftButton && oncePress)   //如果鼠标左键第一次按下
	{
		pressedPoint = e->pos();
	}
	else
	{
		oldPoint = e->pos();
	}
}

void Screen::mouseReleaseEvent(QMouseEvent *e)    //只有已经按下鼠标按键，才会存在鼠标释放行为，释放鼠标后会立刻出现控制面板
{
	oncePress = false;
	if (!control)        //如果未出现截图操作控件
	{
		//qDebug() << "new ControlWidget";
		control = new QWidget(this);         //新建一个窗口控件
		layout = new QHBoxLayout(control);    //在control上建立水平布局
		layout->addWidget(controlUi);         //将控制窗口应用于水平布局
		layout->setContentsMargins(0, 0, 0, 0);
		control->setObjectName("control");
		control->setStyleSheet("QWidget#control{\
                               background-color: #eaecf0;}");
		controlUi->setScreenQuote(this);
		connect(controlUi, &ControlWidget::emitslot, this, &Screen::slotClose);
		//connect(this,&Screen::emitUndo,labelimage->m_plaintextedit,&QTextEdit::undo);
	}
	//设置控制面板的位置

	CoordinateTrans(rect);
	qreal xx = rect.bottomRight().x();
	qreal yy = rect.bottomRight().y();
	labelimage->slotBottomRight(QPointF(rect.width(),rect.height()));
	if ((rect.bottomLeft().y() >= (pixmap.height() - 90)) && (rect.topLeft().x() >= (pixmap.width() - 500)))
	{
		control->setGeometry(xx - 500, yy - rect.height() - 46, 500, 40);
	}
	else if (rect.height() >= (pixmap.height() - 90))
	{
		control->setGeometry(pixmap.width()-520, pixmap.height()-50, 500, 40);
	}
	else if ((rect.bottomLeft().y() >= (pixmap.height() - 90)) && rect.topLeft().x() <= 500)
	{
		control->setGeometry(xx - rect.width(), yy - rect.height() - 46, 500, 40);
	}
	else if (rect.bottomLeft().y() >= (pixmap.height() - 90))
	{
		control->setGeometry(xx - 500, yy - rect.height() - 46, 500, 40);
	}
	else if (rect.topLeft().x() <= 500)
	{
		control->setGeometry(xx - rect.width(), yy + 6, 500, 40);
	}
	else
	{
		control->setGeometry(xx - 500, yy + 6, 500, 40);
	}
	control->show();
}

void Screen::mouseMoveEvent(QMouseEvent *e)
{
	QPoint tempPoint = e->pos();			//当前鼠标位置
	if (!controlUi->rectangleClicked && !controlUi->drawRoundClicked && !controlUi->drawLineClicked && !controlUi->arrowClicked && !controlUi->textEditClicked)
	{
	}
	else
	{
		setselectimagelabel(e);
	}
	if (e->buttons() == Qt::LeftButton)   //如果左键按下
	{
		//选取左上角的信息更改
		int x = qAbs(movePoint.x() - pressedPoint.x());			//movepoint即终点坐标
		int y = qAbs(movePoint.y() - pressedPoint.y());
		widthInfoRect.setText(QString::fromLocal8Bit("%1 × %2").arg(x).arg(y));   //将截图选区的长宽信息显示在widthinforect中
		if (comparePoint(pressedPoint, movePoint)) {
			CoordinateTrans(rect);
			widthInfoRect.setLocation(rect.topLeft().x(), rect.topRight().y());
			//计算选区矩形
			rect.setX(pressedPoint.x());
			rect.setY(pressedPoint.y());
			rect.setWidth(movePoint.x() - pressedPoint.x());
			rect.setHeight(movePoint.y() - pressedPoint.y());
		}
		else {
			CoordinateTrans(rect);
			widthInfoRect.setLocation(rect.topLeft().x(), rect.topRight().y());
			rect.setX(movePoint.x());
			rect.setY(movePoint.y());
			rect.setWidth(pressedPoint.x() - movePoint.x());
			rect.setHeight(pressedPoint.y() - movePoint.y());
		}
		if (oncePress)				//如果是第一次按下时候，
		{
			movePoint = e->pos();   //终点即为鼠标此时位置
		//此时的移动代表选区的变化
		}
		else
		{

			if (control)  //如果控制面板已经出现，则第二次以后的每一次鼠标按键都会使其暂时隐藏
				control->hide();
			int moveX = tempPoint.x() - oldPoint.x();  //鼠标移动的x距离
			int moveY = tempPoint.y() - oldPoint.y();   //鼠标移动的y距离
			switch (type)         //选区角落四个小矩形的位置
			{
			case WZ_RECT1:               //意思是第一次选区之后，分别拖动四个角落的小矩形时候，截图选区的变化
				pressedPoint.setY(pressedPoint.y() + moveY);  //右上角的矩形
				movePoint.setX(movePoint.x() + moveX);
				break;
			case WZ_RECT2:
				pressedPoint.setX(pressedPoint.x() + moveX);    //左上角的矩形
				pressedPoint.setY(pressedPoint.y() + moveY);
				break;
			case WZ_RECT3:
				pressedPoint.setX(pressedPoint.x() + moveX);  //左下角的矩形
				movePoint.setY(movePoint.y() + moveY);
				break;
			case WZ_RECT4:
				movePoint.setX(movePoint.x() + moveX);     //右下角的矩形
				movePoint.setY(movePoint.y() + moveY);
				break;
			case WZ_RECT:                  //指的是当鼠标在截图选区中按下左键然后拖动时候，截图选区的整体位置变化
			{

				int tempPressX = pressedPoint.x() + moveX;
				int tempPressY = pressedPoint.y() + moveY;
				int tempMoveX = movePoint.x() + moveX;
				int tempMoveY = movePoint.y() + moveY;
				int deskWidth = pixmap.width();
				int deskHeight = pixmap.height();
				if (tempPressX < 0)
				{
					tempPressX = 0;
					tempMoveX = movePoint.x();
				}
				if (tempPressX > deskWidth)
				{
					tempPressX = deskHeight;
				}
				if (tempPressY < 0)
				{
					tempPressY = 0;
					tempMoveY = movePoint.y();
				}
				if (tempPressY > deskHeight)
				{

					tempPressY = deskHeight;
				}
				if (tempMoveX < 0)
				{
					tempMoveX = 0;
				}
				if (tempMoveX > deskWidth)
				{
					tempMoveX = deskWidth;
					tempPressX = pressedPoint.x();
				}
				if (tempMoveY < 0)
				{
					tempMoveY = 0;
				}
				if (tempMoveY > deskHeight)
				{
					tempMoveY = deskHeight;
					tempPressY = pressedPoint.y();

				}
				pressedPoint.setX(tempPressX);    //鼠标在截图区域中拖动结束后，选区的位置
				pressedPoint.setY(tempPressY);
				movePoint.setX(tempMoveX);
				movePoint.setY(tempMoveY);
				break;
			}
			case WZ_NO:
				break;
			default:
				break;
			}
			oldPoint = tempPoint;    //更新位置坐标信息
		}

	}
	else                          //指的是当鼠标位于四个角落小矩形中和选区中时候鼠标的图标变化
	{
	WZType r = pointInWhere(e->pos());
		if (r == WZ_RECT1)
		{
			type = WZ_RECT1;
			setCursor(Qt::SizeBDiagCursor);
		}
		else if (r == WZ_RECT2)
		{
			type = WZ_RECT2;
			setCursor(Qt::SizeFDiagCursor);
		}
		else if (r == WZ_RECT3)
		{
			type = WZ_RECT3;
			setCursor(Qt::SizeBDiagCursor);
		}
		else if (r == WZ_RECT4)
		{
			type = WZ_RECT4;
			setCursor(Qt::SizeFDiagCursor);
		}
		else if (r == WZ_RECT)
		{
			type = WZ_RECT;
			setCursor(Qt::SizeAllCursor);
		}
		else
		{
			type = WZ_NO;
			setCursor(Qt::ArrowCursor);
		}
	}
	//repaint();
	update();
}

bool Screen::compareRect(QRectF &r1, QRectF &r2)
{
	return false;
}

void Screen::CoordinateTrans(QRectF &rect)
{

	if (rect.bottomRight().x() < rect.bottomLeft().x())
	{
		QPointF exchange(0, 0);
		exchange = rect.bottomLeft();
		rect.setBottomLeft(rect.bottomRight());
		rect.setBottomRight(exchange);
	}
	if (rect.topRight().x() < rect.topLeft().x())
	{
		QPointF exchange(0, 0);
		exchange = rect.topLeft();
		rect.setTopLeft(rect.topRight());
		rect.setTopRight(exchange);
	}
	if (rect.topRight().y() > rect.bottomRight().y())
	{
		QPointF exchange(0, 0);
		exchange = rect.topRight();
		rect.setTopRight(rect.bottomRight());
		rect.setBottomRight(exchange);
	}
	if (rect.topLeft().y() > rect.bottomLeft().y())
	{
		QPointF exchange(0, 0);
		exchange = rect.topLeft();
		rect.setTopLeft(rect.bottomLeft());
		rect.setBottomLeft(exchange);
	}
}

void Screen::posPoint(QWidget * widget)
{
	qreal xx = rect.bottomRight().x();
	qreal yy = rect.bottomRight().y();
	if ((rect.bottomLeft().y() >= (pixmap.height() - 90)) && (rect.topLeft().x() >= (pixmap.width() - 500)))
	{
		widget->setGeometry(xx - 500, yy - rect.height() - 86, 500, 40);
	}
	else if (rect.height() >= (pixmap.height() - 90))
	{
		widget->setGeometry(pixmap.width() - 520, pixmap.height() - 90, 500, 40);
	}
	else if ((rect.bottomLeft().y() >= (pixmap.height() - 90)) && rect.topLeft().x() <= 500)
	{
		widget->setGeometry(xx - rect.width(), yy - rect.height() - 86, 500, 40);
	}
	else if (rect.bottomLeft().y() >= (pixmap.height() - 90))
	{
		widget->setGeometry(xx - 500, yy - rect.height() - 86, 500, 40);
	}
	else if (rect.topLeft().x() <= 500)
	{
		widget->setGeometry(xx - rect.width(), yy + 46, 500, 40);
	}
	else
	{
		widget->setGeometry(xx - 500, yy + 46, 500, 40);
	}
	widget->show();
}

//计算此时鼠标移动在哪个选区上面
WZType Screen::pointInWhere(QPoint p)
{
	if (pointInRect(p, rect1))
		return WZ_RECT1;
	else if (pointInRect(p, rect2))
		return WZ_RECT2;
	else if (pointInRect(p, rect3))
		return WZ_RECT3;
	else if (pointInRect(p, rect4))
		return WZ_RECT4;
	else if (pointInRect(p, rect))
		return WZ_RECT;
	else
		return WZ_NO;

}

//判断点是否在矩形之内
bool Screen::pointInRect(QPoint &p, QRectF &r)
{
	if (p.x() > r.x() && p.x() < r.x() + r.width() &&
		p.y() > r.y() && p.y() < r.y() + r.height())
		return true;
	return false;
}

void Screen::slotControlResize()
{
	CoordinateTrans(rect);
	m_colorWidget2->hide();
	m_colorWidget3->hide();
	m_colorWidget4->hide();
	m_fontSelect->hide();
	posPoint(m_colorWidget);
}

//void Screen::slotControlHide()
//{
//	m_colorWidget->hide();
//}

void Screen::slotRectResize()
{
	CoordinateTrans(rect);
	m_colorWidget->hide();
	m_colorWidget3->hide();
	m_colorWidget4->hide();
	m_fontSelect->hide();
	posPoint(m_colorWidget2);
}

//void Screen::slotRectHide()
//{
//	m_colorWidget->hide();
//}

void Screen::slotRoundResize()
{
	CoordinateTrans(rect);
	m_colorWidget->hide();
	m_colorWidget2->hide();
	m_colorWidget4->hide();
	m_fontSelect->hide();
	posPoint(m_colorWidget3);
}

void Screen::slotArrowResize()
{
	CoordinateTrans(rect);
	m_colorWidget->hide();
	m_colorWidget2->hide();
	m_colorWidget3->hide();
	m_fontSelect->hide();
	posPoint(m_colorWidget4);
}

//void Screen::slotRoundHide()
//{
//	m_colorWidget->hide();
//}

void Screen::slotTextResize()
{
	CoordinateTrans(rect);
	m_colorWidget->hide();
	m_colorWidget2->hide();
	m_colorWidget3->hide();
	posPoint(m_fontSelect);
}

void Screen::slotUndo()
{
	m_colorWidget->hide();
	m_colorWidget2->hide();
	m_colorWidget3->hide();
	m_fontSelect->hide();
	m_colorWidget4->hide();
	labelimage->undoDealWith();
}

void Screen::slotFontSize(int size)
{
	setSize(size);
}

void Screen::slotFontColor(QColor color)
{
	setColor(color);
}

void Screen::slotShow()
{
	if (!labelimage->isActiveWindow())
	{
		labelimage->activateWindow();
	}
}

//void Screen::slotTextHide()
//{
//	m_fontSelect->hide();
//}

void Screen::slotClose()
{
	emit emitClose();
}

void Screen::keyPressEvent(QKeyEvent *e)    //键盘事件，包括esc退出截图，回车键完成截图，返回键完成截图
{
	int key = e->key();
	switch (key)
	{
	case Qt::Key_Escape:
		//Exit();
		if (labelimage)
		{
			labelimage->close();
			m_colorWidget->close();
			m_colorWidget2->close();
			m_colorWidget3->close();
			m_fontSelect->close();
			m_colorWidget4->close();
		}
		close();
		break;
	case Qt::Key_Enter:
		if (controlUi)
		{
			controlUi->finishBtn_slot();
		}
		break;
	case Qt::Key_Return:
		if (controlUi)
		{
			controlUi->finishBtn_slot();
		}
		break;
	default:
		break;
	}
	if ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Z))
	{
		labelimage->undoDealWith();
	}
}

//如果p1的x和y坐标都小于p2的x和y坐标返回true
bool Screen::comparePoint(QPoint &p1, QPoint &p2)      //点p1的位置坐标是否小于p2点
{
	if (p1.x() < p2.x() && p1.y() < p2.y())
		return true;
	return false;
}

void Screen::setselectimagelabel(QMouseEvent* event)
{
	int wid = abs(movePoint.x() - pressedPoint.x());
	int hei = abs(movePoint.y() - pressedPoint.y());
	int x = pressedPoint.x() < movePoint.x() ? pressedPoint.x() : movePoint.x();
	int y = pressedPoint.y() < movePoint.y() ? pressedPoint.y() : movePoint.y();
	QImage selectimage = pixmap.copy(x, y, wid, hei).toImage();
	labelimage->setimagetolabel(selectimage);
	labelimage->setFixedSize(wid, hei);
	labelimage->move(QPoint(x, y));
	labelimage->show();
}

//保存截取出来的图片
void Screen::savePixmap()
{

	//生成图片名称
	QString picName;
	QTime time;
	//获取当前系统时间，用做伪随机数的种子
	time = QTime::currentTime();
	qsrand(time.msec() + time.second() * 1000);
	//随机字符串
	QString randStr;
	randStr.setNum(qrand());
	picName.append(randStr);
	//QFileDialog fileDialog;
	//fileDialog.setWindowFlags(Qt::WindowStaysOnTopHint);
	QString filename = QFileDialog::getSaveFileName(this, QStringLiteral("保存截图"), picName, "JPEG Files(*.jpg)");

	if (filename.length() > 0) {
		QImage pimage = this->getGrabPixmap().toImage();
		pimage.save(filename, "jpg");
	}
}

void Screen::setColor(QColor & color)
{
	labelimage->linePen.setColor(color);
	labelimage->rectPen.setColor(color);
	labelimage->roundPen.setColor(color);
	labelimage->arrowPen.setColor(color);
	//labelimage->textPen.setColor(color);
}

void Screen::setSize(int size)
{
	labelimage->linePen.setWidth(size);
	labelimage->rectPen.setWidth(size);
	labelimage->roundPen.setWidth(size);
	//labelimage->arrowPen.setWidth(size);
	//labelimage->textPen.setWidth(size);
}

QPixmap Screen::getGrabPixmap()   //返回截到的图片
{
	if (this == nullptr)
		return QPixmap();
	if (labelimage->shapes.size() == 0)
	{
        QPixmap pix;
        if(movePoint.x()<pressedPoint.x() && movePoint.y()<pressedPoint.y())
        {
            pix = pixmap.copy(movePoint.x(), movePoint.y(), pressedPoint.x()-movePoint.x(),
                pressedPoint.y()-movePoint.y());
        }
        else if(movePoint.x()<pressedPoint.x())
        {
            pix = pixmap.copy(movePoint.x(), pressedPoint.y(), pressedPoint.x()-movePoint.x(),
                movePoint.y() - pressedPoint.y());
        }
        else if(movePoint.y()<pressedPoint.y())
        {
            pix = pixmap.copy(pressedPoint.x(), movePoint.y(), movePoint.x() - pressedPoint.x(),
                pressedPoint.y()-movePoint.y());
        }
        else
        {
            pix = pixmap.copy(pressedPoint.x(), pressedPoint.y(), movePoint.x() - pressedPoint.x(),
                movePoint.y() - pressedPoint.y());   //这个地方是关键，可以从这里入手了，得到了pixmap之后，进行toimage，然后进行编辑操作
        }

        qDebug()<<"pix"<<pix;
        qDebug()<<"pressedPoint.x()"<<pressedPoint.x();
        qDebug()<<"movePoint.x()"<<movePoint.x();
		QApplication::clipboard()->setPixmap(pix);
		emit emitPixmap();
		return pix;
	}
	else
	{
		QImage image = labelimage->resultimage();
		QPixmap pixmap = QPixmap::fromImage(image);
		QApplication::clipboard()->setPixmap(pixmap);
		emit emitPixmap();
		return pixmap;
	}
}

void Screen::drawline()
{

	labelimage->setdrawlineenable();

}

void Screen::textedit()
{
	labelimage->settexteditenable();
}

void Screen::drawarrow()
{
	labelimage->setdrawarrowenable();
}

void Screen::drawround()
{
	labelimage->setroundenable();
}

void Screen::drawrectang()
{
	labelimage->setrectangleenable();
}

void Screen::Exit()
{
	if (labelimage)
	{
		labelimage->close();
		m_colorWidget->close();
		m_colorWidget2->close();
		m_colorWidget3->close();
		m_fontSelect->close();
		m_colorWidget4->close();
	}
}

Screen::~Screen()
{
	delete m_colorWidget;
	m_colorWidget = nullptr;
	delete m_colorWidget2;
	m_colorWidget2 = nullptr;
	delete m_colorWidget3;
	m_colorWidget3 = nullptr;
	delete m_colorWidget4;
	m_colorWidget4 = nullptr;
	delete m_fontSelect;
	m_fontSelect = nullptr;
	delete labelimage;
	labelimage = nullptr;
	delete controlUi;
	controlUi = nullptr;
	//delete layout;
	//layout = nullptr;
	delete control;
	control = nullptr;
}
