#include "qslabel.h"
#include "controlwidget.h"
#include <QPalette>
#include "screen.h"
#include <qdebug.h>
#include <QColorDialog>
using namespace Qt;


QSLabel::QSLabel(QWidget* parent)
{
	setMouseTracking(true);
	ispressed = false;
	isdrawline = false;
	isdrawrectangle = false;
	isdrawround = false;
	istextedit = false;
	isdrawarrow = false;
	index = 1;
	m_plaintextedit = new QTextEdit(this);
	m_plaintextedit->hide();
	m_plaintextedit->resize(60, 40);
	QPalette p1 = m_plaintextedit->palette();
	p1.setBrush(QPalette::Base, QBrush(QColor(255, 0, 0, 0)));
	m_plaintextedit->setPalette(p1);
	connect(m_plaintextedit, SIGNAL(textChanged()), this, SLOT(ontextchanged()));
	m_plaintextedit->setStyleSheet("QTextEdit{ border: 1px solid #dadada;}"
		"QTextEdit{font-family:'Microsoft YaHei'; color:#ff0000;}"
		""
	);
	m_plaintextedit->setFontPointSize(12);
	m_plaintextedit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	linePen.setColor(red); 
	linePen.setWidth(2);
	rectPen.setColor(red);
	rectPen.setWidth(2);
	roundPen.setColor(red);
	roundPen.setWidth(2);
	arrowPen.setColor(red);
	arrowPen.setWidth(2);
	textPen.setColor(red);
	textPen.setWidth(2);
	brush = QBrush(Qt::SolidPattern);
	brush.setColor(Qt::red);
	font = new QFont("Microsoft YaHei", 12, 50, false);
}

void QSLabel::ontextchanged()
{
	if (width <= m_plaintextedit->width() && height <= m_plaintextedit->height())
	{
		m_plaintextedit->setEnabled(false);
		return;
	}
	else if (width <= m_plaintextedit->width())
	{
		if (m_plaintextedit->toPlainText().size() < 10)
		{
			m_plaintextedit->setFixedHeight(40);
		}
		else
		{
			m_plaintextedit->setFixedHeight(m_plaintextedit->document()->size().rheight() + 10);

		}
	}
	else if (height <= m_plaintextedit->height())
	{
		m_plaintextedit->setEnabled(false);
		if (m_plaintextedit->toPlainText().size() < 10)
		{
			m_plaintextedit->setFixedWidth(m_plaintextedit->toPlainText().size() * 10 + 50);
		}
		else
		{
			m_plaintextedit->setFixedWidth(m_plaintextedit->toPlainText().size() * 15 + 20);

		}
	}
	else
	{
		if (m_plaintextedit->toPlainText().size() < 10)
		{
			m_plaintextedit->resize(m_plaintextedit->toPlainText().size() * 10 + 50, 40);
		}
		else
		{
			m_plaintextedit->resize(m_plaintextedit->toPlainText().size() * 15 + 20, m_plaintextedit->document()->size().rheight() + 10);

		}
	}
}
//唤醒输入框
void QSLabel::setTextinputWakeup()
{

	LLTextScreenLB *temp =  (LLTextScreenLB*)sender();
	QRect rect = temp->geometry();


	

	temp->hasEdit = true;
	temp->setHidden(true);
	chooseAgainLB = temp;

	m_plaintextedit->show();
	m_plaintextedit->setEnabled(true);
	m_plaintextedit->move(rect.x(),rect.y());
	m_plaintextedit->resize(rect.size());

	m_plaintextedit->setText(temp->text());
	
	m_plaintextedit->setFocus();

	
	//for (auto iter = lltexts.begin(); iter != lltexts.end(); iter++)
	//{

	//	if (*iter == temp)
	//	{

	//		lltexts.erase(iter);
	//		break;
	//	}
	//}




}
void QSLabel::mouseMoveEvent(QMouseEvent *event)
{
	if (isdrawline || isdrawrectangle || isdrawround || istextedit || isdrawarrow)
	{

		setCursor(CrossCursor);
	}
	if (ispressed)
	{
		if (isdrawline)
		{
			endPoint = event->pos();
			myLine *line = new myLine;
			line->startPoint = startPoint;
			line->endPoint = endPoint;
			line->pen = linePen;
			line->type = index;
			lines.push_back(line);
			QVariant *lineV = new QVariant;
			lineV->setValue(line);
			shapes.push_back(lineV);
			startPoint = endPoint;
		}
		else if (isdrawrectangle || isdrawround || isdrawarrow)
		{
			endPoint = event->pos();
		}
		update();
	}
}

void QSLabel::mousePressEvent(QMouseEvent *event)
{
	startPoint = event->pos();
	endPoint = event->pos();
	//qDebug() << "startPoint" << startPoint;

	bool isContain = false;
	for (LLTextScreenLB * lb : lltexts)
	{
		if (lb->isContain == true) {

			isContain = true;
			break;
		}
	}

	ispressed = true;
	if (istextedit)
	{
		QString contentStr = m_plaintextedit->toPlainText();

	
	
		if (contentStr.size())
		{

			QRect  temp = QRect(QPoint(m_plaintextedit->x(), m_plaintextedit->y()), QSize(m_plaintextedit->width(), m_plaintextedit->height()));
			/*myText *text = new myText;
			
			QRect  temp= QRect(QPoint(m_plaintextedit->x(), m_plaintextedit->y()), QSize(m_plaintextedit->width(), m_plaintextedit->height()));
			text->mRect = temp;

			text->mText = contentStr;
			QVariant *textV = new QVariant;
			textV->setValue(text);
			shapes.push_back(textV);
			texts.push_back(text);*/


			if (chooseAgainLB)
			{
				qDebug() << " label_edit ";
			
				chooseAgainLB->hasEdit = false;

				chooseAgainLB->setGeometry(temp);

				/*textlabel->resize(temp.size());
				textlabel->move(temp.x(),temp.y());*/
				chooseAgainLB->setText(contentStr);
				chooseAgainLB->setFont(*font);
				
				chooseAgainLB->show();

			}
			else {


				LLTextScreenLB *textlabel = new LLTextScreenLB(this);
				connect(textlabel, SIGNAL(wakeupTextInput()), this, SLOT(setTextinputWakeup()));

				textlabel->setGeometry(temp);

				/*textlabel->resize(temp.size());
				textlabel->move(temp.x(),temp.y());*/
				textlabel->setText(contentStr);
				textlabel->setFont(*font);
				lltexts.push_back(textlabel);
				textlabel->show();
				textlabel->setAlignment(Qt::AlignCenter);


				QVariant *item = new QVariant;
				item->setValue(textlabel);
				shapes.push_back(item);
				//update();
				
				qDebug() << "create new";
			}


			m_plaintextedit->clear();
			m_plaintextedit->setHidden(true);

			chooseAgainLB = NULL;
			return;
		}
		else if (isContain)
		{

			qDebug() << " isContain return";
			return;
		}


			width = m_brPoint.x() - startPoint.x();
			height = m_brPoint.y() - startPoint.y();
			m_plaintextedit->setMaximumSize(width, height);
			m_plaintextedit->move(startPoint);
			m_plaintextedit->resize(60, 40);
			m_plaintextedit->setFixedHeight(40);
			m_plaintextedit->show();
			m_plaintextedit->setFocus();
			m_plaintextedit->clear();
			m_plaintextedit->setEnabled(true);

			qDebug() << "create empty";
		
	}
}

void QSLabel::mouseReleaseEvent(QMouseEvent *event)
{
	//setCursor(ArrowCursor);
	ispressed = false;
	if (isdrawline)
	{
		index += 1;
	}
	if (isdrawrectangle)
	{
		endPoint = event->pos();
		myRectangle *rectangle = new myRectangle;
		rectangle->startPoint = startPoint;
		rectangle->endPoint = endPoint;
		rectangle->pen = rectPen;
		rectangles.push_back(rectangle);
		QVariant *rectangleV = new QVariant;
		rectangleV->setValue(rectangle);
		shapes.push_back(rectangleV);
		update();
	}
	else if (isdrawround)
	{
		endPoint = event->pos();
		myRound *round = new myRound;
		round->startPoint = startPoint;
		round->endPoint = endPoint;
		round->pen = roundPen;
		rounds.push_back(round);
		QVariant *roundV = new QVariant;
		roundV->setValue(round);
		shapes.push_back(roundV);
		update();
	}
	else if (isdrawarrow)
	{
		endPoint = event->pos();
		myArrow *arrow = new myArrow;
		arrow->startPoint = startPoint;
		arrow->endPoint = endPoint;
		arrow->pen = arrowPen;
		arrow->brush = brush;
		arrows.push_back(arrow);
		QVariant *arrowV = new QVariant;
		arrowV->setValue(arrow);
		shapes.push_back(arrowV);
		update();
	}
}


void QSLabel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawImage(0, 0, selectimage); //表示是在selectimage上面做编辑
	painter.setRenderHint(QPainter::Antialiasing);
	
	for (int i = 0; i < lines.size(); i++)
	{
		myLine *line = lines[i];
		painter.setPen(line->pen);
		painter.drawLine(line->startPoint, line->endPoint);

	}
	for (int j = 0; j < rectangles.size(); j++)
	{
		myRectangle *rectangle = rectangles[j];
		painter.setPen(rectangle->pen);
		int x1 = rectangle->startPoint.x() < rectangle->endPoint.x() ? rectangle->startPoint.x() : rectangle->endPoint.x();
		int y1 = rectangle->startPoint.y() < rectangle->endPoint.y() ? rectangle->startPoint.y() : rectangle->endPoint.y();
		painter.drawRect(x1, y1, abs(rectangle->endPoint.x() - rectangle->startPoint.x()), abs(rectangle->endPoint.y() - rectangle->startPoint.y()));

	}
	for (int k = 0; k < rounds.size(); k++)
	{
		myRound *round = rounds[k];
		painter.setPen(round->pen);
		int x2 = round->startPoint.x() < round->endPoint.x() ? round->startPoint.x() : round->endPoint.x();
		int y2 = round->startPoint.y() < round->endPoint.y() ? round->startPoint.y() : round->endPoint.y();
		painter.drawEllipse(x2, y2, abs(round->endPoint.x() - round->startPoint.x()), abs(round->endPoint.y() - round->startPoint.y()));

	}
	for (int m = 0; m < arrows.size(); m++)
	{
		myArrow *arrow = arrows[m];
		painter.save();
		painter.setPen(arrow->pen);
		painter.setBrush(arrow->brush);
		drawarrow(arrow->startPoint, arrow->endPoint, painter);
		painter.restore();
	}
	/*for (int l = 0; l < texts.size(); l++)
	{
		myText *text = texts[l];
		QRect rect = text->mRect;
		painter.setPen(textPen);
		painter.setFont(*font);
		painter.drawText(rect, Qt::TextWrapAnywhere, text->mText);
	}*/
	if (needPaint)
	{

		for (int l = 0; l < lltexts.size(); l++)
		{
			LLTextScreenLB *text = lltexts[l];
			QRect rect = text->geometry();
			painter.setPen(textPen);
			painter.setFont(*font);
			painter.drawText(rect, Qt::TextWrapAnywhere, text->text());
		}
	}
	int xx = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
	int yy = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();

	if (isdrawrectangle)
	{
		painter.setPen(rectPen);
		painter.drawRect(xx, yy, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
	}
	else if (isdrawround)
	{
		painter.setPen(roundPen);
		painter.drawEllipse(xx, yy, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
	}
	else if (isdrawarrow)
	{
		painter.save();
		painter.setPen(arrowPen);
		painter.setBrush(brush);
		drawarrow(startPoint, endPoint, painter);
		painter.restore();
	}
}

void QSLabel::keyPressEvent(QKeyEvent * e)
{
	if ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_Z))
	{
		this->undoDealWith();
	}
	QLabel::keyPressEvent(e);
}

void QSLabel::slotBottomRight(QPointF brPoint)
{
	m_brPoint = brPoint;
}

void QSLabel::drawarrow(QPoint startpoint, QPoint endpoint, QPainter &p)
{
	double par = 15.0;
	double slopy = atan2((endpoint.y() - startpoint.y()), (endpoint.x() - startpoint.x()));
	double cosy = cos(slopy);
	double siny = sin(slopy);
	QPoint point1 = QPoint(endpoint.x() + int(-par * cosy - (par / 2.0*siny)), endpoint.y() + int(-par * siny + (par / 2.0*cosy)));
	QPoint point2 = QPoint(endpoint.x() + int(-par * cosy + (par / 2.0*siny)), endpoint.y() - int(par / 2.0*cosy + par * siny));
	QPoint points[3] = { endpoint,point1,point2 };
	p.setRenderHint(QPainter::Antialiasing, true);
	p.drawPolygon(points, 3);
	int offsetx = int(par*siny / 3);
	int offsety = int(par*cosy / 3);
	QPoint point3, point4;
	point3 = QPoint(endpoint.x() + int(-par * cosy - (par / 2.0*siny)) + offsetx, endpoint.y() + int(-par * siny + (par / 2.0*cosy)) - offsety);
	point4 = QPoint(endpoint.x() + int(-par * cosy + (par / 2.0*siny) - offsetx), endpoint.y() - int(par / 2.0*cosy + par * siny) + offsety);
	QPoint arrbodypoints[3] = { startpoint,point3,point4 };
	p.drawPolygon(arrbodypoints, 3);
}

void QSLabel::setimagetolabel(const QImage &image)
{
	selectimage = image;

	endPoint = QPoint(0, 0);
	startPoint = QPoint(0, 0);
}


void QSLabel::setdrawlineenable()
{
	settextedittovector();

	isdrawline = true;
	isdrawrectangle = false;
	isdrawround = false;
	istextedit = false;
	isdrawarrow = false;
	m_plaintextedit->hide();
}

void QSLabel::setrectangleenable()
{
	settextedittovector();
	isdrawline = false;
	isdrawrectangle = true;
	isdrawround = false;
	istextedit = false;
	isdrawarrow = false;

	m_plaintextedit->hide();
}

void QSLabel::setdrawarrowenable()
{
	settextedittovector();
	isdrawline = false;
	isdrawarrow = true;
	isdrawrectangle = false;
	isdrawround = false;
	istextedit = false;

	m_plaintextedit->hide();
}

void QSLabel::setroundenable()
{
	settextedittovector();
	isdrawline = false;
	isdrawrectangle = false;
	isdrawround = true;
	isdrawarrow = false;
	istextedit = false;

	m_plaintextedit->hide();
}

void QSLabel::settexteditenable()
{
	settextedittovector();
	isdrawline = false;
	isdrawrectangle = false;
	isdrawround = false;
	isdrawarrow = false;
	istextedit = true;

	m_plaintextedit->hide();
}

void QSLabel::settextedittovector()
{
	if (istextedit&&m_plaintextedit->toPlainText().size())
	{
		/*myText *text = new myText;
		text->mRect = QRect(QPoint(m_plaintextedit->x(), m_plaintextedit->y()), QSize(m_plaintextedit->width(), m_plaintextedit->height()));
		text->mText = m_plaintextedit->toPlainText();
		texts.push_back(text);
		QVariant *textV = new QVariant;
		textV->setValue(text);
		shapes.push_back(textV);
		update();*/

		QRect temp = QRect(QPoint(m_plaintextedit->x(), m_plaintextedit->y()), QSize(m_plaintextedit->width(), m_plaintextedit->height()));

		LLTextScreenLB *textlabel = new LLTextScreenLB(this);
		connect(textlabel, SIGNAL(wakeupTextInput()), this, SLOT(setTextinputWakeup()));

		textlabel->setGeometry(temp);

		/*textlabel->resize(temp.size());
		textlabel->move(temp.x(),temp.y());*/
		textlabel->setText(m_plaintextedit->toPlainText());
		textlabel->setFont(*font);
		lltexts.push_back(textlabel);
		textlabel->show();
		textlabel->setAlignment(Qt::AlignCenter);


		QVariant *item = new QVariant;
		item->setValue(textlabel);
		shapes.push_back(item);

		m_plaintextedit->clear();
		m_plaintextedit->setHidden(true);
	}
}

QImage QSLabel::resultimage()
{


	if (lltexts.size())
	{
		for (int i = 0; i < lltexts.size(); i++)
		{

			lltexts[i]->setHidden(true);
		}

		needPaint = true;
		update();

	}
	return QImage(QPixmap::grabWidget(this).toImage());

}

void QSLabel::undoDealWith()
{
	if (shapes.size() == 0)
	{
		return;
	}
	const char *type = shapes.back()->typeName();
	QString qtype = QString(QLatin1String(type));
	//qDebug() << "type" << qtype;
	if (QString("myLine*") == qtype)
	{
		QVariant *line = shapes.back();
		myLine *mline = line->value<myLine*>();
		int num = mline->type;
		int i = 0;

		int shapesSize = shapes.size();
		for (; i < shapesSize; ++i)
		{
			const char *stype = shapes.at(i)->typeName();
			QString sqtype = QString(QLatin1String(stype));
			if (QString("myLine*") == sqtype)
			{
				myLine *changeLine = shapes.at(i)->value<myLine*>();
				if (changeLine->type == num)
				{
					//qDebug() << "i" << i;
					break;
				}
			}
			else
			{
				continue;
			}
		}
		//qDebug() << "i" << i;
		int loopNum = shapesSize - i;
		for (int j = 0; j < loopNum; ++j)
		{
			shapes.pop_back();
			lines.pop_back();
		}
		//lines.pop_back();
	}
	else if (QString("myRectangle*") == qtype)
	{
		shapes.pop_back();
		rectangles.pop_back();
	}
	else if (QString("myRound*") == qtype)
	{
		shapes.pop_back();
		rounds.pop_back();
	}
	else if (QString("myArrow*") == qtype)
	{
		shapes.pop_back();
		arrows.pop_back();
	}
	else if (QString("LLTextScreenLB*") == qtype)
	{
		
		LLTextScreenLB *temp = lltexts.last();

		QString textTemp = temp->text();
		int len = textTemp.length();
		if (len > 1)
		{
			textTemp = textTemp.mid(0, len - 1);
			temp->setText(textTemp);
		}
		else {

			temp->setHidden(true);
			lltexts.pop_back();

			temp = NULL;
			delete temp;

			shapes.pop_back();
		}
		
		
	}
	repaint();
	//update();
}

QSLabel::~QSLabel()
{
	delete m_plaintextedit;
	m_plaintextedit = nullptr;
	delete font;
	font = nullptr;
	for (auto &item : lines)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}
	for (auto &item : rectangles)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}
	for (auto &item : rounds)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}
	for (auto &item : lltexts)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}
	for (auto &item : arrows)
	{
		if (item != nullptr)
		{
			delete item;
			item = nullptr;
		}
	}
	for (auto &item : shapes)
	{
		if (item != nullptr)
		{
			item = nullptr;
		}
	}
}