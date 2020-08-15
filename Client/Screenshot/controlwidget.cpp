#include "controlwidget.h"
#include <QPoint>
#include "screen.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QToolTip>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
	rectangleClicked{false},
	drawRoundClicked{false},
	arrowClicked{false},
	drawLineClicked{false},
	textEditClicked{false},
	m_rectangleClicked{ false },
	m_drawRoundClicked{ false },
	m_arrowClicked{ false },
	m_drawLineClicked{ false },
	m_textEditClicked{ false }
{
	//this->setGeometry(0, 0, 500, 40);
	this->setFixedSize(500, 40);
    this->setStyleSheet("background-color:transparent;");
    m_widget = new QWidget(this);
    m_widget->setStyleSheet("background-color:#212C33;");
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->addWidget(m_widget);
    horizontalLayout = new QHBoxLayout(m_widget);
	horizontalLayout->setSpacing(6);
	horizontalLayout->setContentsMargins(0, 6, 0, 6);
	rectangleBtn = new QToolButton;
	rectangleBtn->setCursor(Qt::PointingHandCursor);
	const QString rectangleText(QString::fromLocal8Bit("画矩形"));
	rectangleBtn->setToolTip(rectangleText);
	rectangleBtn->setIcon(QIcon(":/Resources/square.png"));
	rectangleBtn->setIconSize(QSize(25, 25));
	rectangleBtn->setAutoRaise(true);

	drawRoundBtn = new QToolButton;
	drawRoundBtn->setCursor(Qt::PointingHandCursor);
	const QString drawRoundText(QString::fromLocal8Bit("画圆"));
	drawRoundBtn->setToolTip(drawRoundText);
    drawRoundBtn->setIcon(QIcon(":/Resources/circle.png"));
	drawRoundBtn->setIconSize(QSize(25, 25));
	drawRoundBtn->setAutoRaise(true);

	arrowBtn = new QToolButton;
	arrowBtn->setCursor(Qt::PointingHandCursor);
	const QString arrowText(QString::fromLocal8Bit("画箭头"));
	arrowBtn->setToolTip(arrowText);
	arrowBtn->setIcon(QIcon(":/Resources/arrow.png"));
	arrowBtn->setIconSize(QSize(25, 25));
	arrowBtn->setAutoRaise(true);

	drawLineBtn = new QToolButton;
	drawLineBtn->setCursor(Qt::PointingHandCursor);
	const QString drawLineText(QString::fromLocal8Bit("画笔"));
	drawLineBtn->setToolTip(drawLineText);
	drawLineBtn->setIcon(QIcon(":/Resources/pen.png"));
	drawLineBtn->setIconSize(QSize(25, 25));
	drawLineBtn->setAutoRaise(true);

	textEditBtn = new QToolButton;
	textEditBtn->setCursor(Qt::PointingHandCursor);
	const QString textEditText(QString::fromLocal8Bit("添加文字"));
	textEditBtn->setToolTip(textEditText);
	textEditBtn->setIcon(QIcon(":/Resources/text.png"));
	textEditBtn->setIconSize(QSize(25, 25));
	textEditBtn->setAutoRaise(true);

	saveBtn = new QToolButton;
	saveBtn->setCursor(Qt::PointingHandCursor);
	const QString saveText(QString::fromLocal8Bit("保存截图"));
	saveBtn->setToolTip(saveText);
	saveBtn->setIcon(QIcon(":/Resources/download.png"));
	saveBtn->setIconSize(QSize(25, 25));
	saveBtn->setAutoRaise(true);

	returneditBtn = new QToolButton;
	returneditBtn->setCursor(Qt::PointingHandCursor);
	const QString returneditText(QString::fromLocal8Bit("撤销"));
	returneditBtn->setToolTip(returneditText);
	returneditBtn->setIcon(QIcon(":/Resources/Revoke.png"));
	returneditBtn->setIconSize(QSize(25, 25));
	returneditBtn->setAutoRaise(true);

	cancelBtn = new QToolButton;
	cancelBtn->setCursor(Qt::PointingHandCursor);
	const QString cancelText(QString::fromLocal8Bit("退出截图"));
	cancelBtn->setToolTip(cancelText);
	cancelBtn->setIcon(QIcon(":/Resources/dropout.png"));
	cancelBtn->setIconSize(QSize(25, 25));
	cancelBtn->setAutoRaise(true);

	finishBtn = new QToolButton;
	finishBtn->setCursor(Qt::PointingHandCursor);
	const QString finishText(QString::fromLocal8Bit("完成截图"));
	finishBtn->setToolTip(finishText);
	finishBtn->setIcon(QIcon(":/Resources/complete.png"));
	finishBtn->setIconSize(QSize(25, 25));
	finishBtn->setAutoRaise(true);
	horizontalLayout->addWidget(rectangleBtn);
	horizontalLayout->addWidget(drawRoundBtn);
	horizontalLayout->addWidget(arrowBtn);
	horizontalLayout->addWidget(drawLineBtn);
	horizontalLayout->addWidget(textEditBtn);
	horizontalLayout->addWidget(saveBtn);
	horizontalLayout->addWidget(returneditBtn);
	horizontalLayout->addWidget(cancelBtn);
	horizontalLayout->addWidget(finishBtn);
    connectSignalAndSlot();
}

void ControlWidget::connectSignalAndSlot()
{
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtn_slot()));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtn_slot()));
    connect(finishBtn, SIGNAL(clicked()), this, SLOT(finishBtn_slot()));
	connect(textEditBtn, SIGNAL(clicked()), this, SLOT(on_textEditBtn_clicked()));
	connect(arrowBtn, &QToolButton::clicked, this, &ControlWidget::on_arrowBtn_clicked);
	connect(drawRoundBtn, &QToolButton::clicked, this, &ControlWidget::on_drawRoundBtn_clicked);
	connect(returneditBtn, &QToolButton::clicked, this, &ControlWidget::on_returneditBtn_clicked);
	connect(drawLineBtn, &QToolButton::clicked, this, &ControlWidget::on_drawLineBtn_clicked);
	connect(rectangleBtn, &QToolButton::clicked, this, &ControlWidget::on_rectangleBtn_clicked);
}
//退出截图按钮
void ControlWidget::cancelBtn_slot()
{
	if (screen)
	{
		screen->close();
		screen->Exit();
		emit emitslot();
		//emit emitClose();
	}
}
//保存截图按钮
void ControlWidget::saveBtn_slot()
{
    screen->savePixmap();
    cancelBtn_slot();
}

//完成按钮将截图保存到剪贴板
void ControlWidget::finishBtn_slot()
{
    //QClipboard *board = QApplication::clipboard();
    screen->getGrabPixmap();
    //board->setImage(pix.toImage());
    cancelBtn_slot();
}


//保存Screen类的引用
void ControlWidget::setScreenQuote(Screen *screen)
{
    this->screen = screen;
}

void ControlWidget::on_drawLineBtn_clicked()
{
	drawLineClicked = true;
	m_rectangleClicked = false;
	m_drawRoundClicked = false;
	m_arrowClicked = false;
	m_textEditClicked = false;
	if (!m_drawLineClicked)
	{
		emit emitResize();
		m_drawLineClicked = !m_drawLineClicked;
	}
	else
	{
		emit emitHide();
		m_drawLineClicked = !m_drawLineClicked;
	}
    screen->drawline();
}

void ControlWidget::on_textEditBtn_clicked()
{
	textEditClicked = true;
	m_rectangleClicked = false;
	m_drawRoundClicked = false;
	m_arrowClicked = false;
	m_drawLineClicked = false;
	if (!m_textEditClicked)
	{
		emit emitTextResize();
		m_textEditClicked = !m_textEditClicked;
	}
	else
	{
		emit emitTextHide();
		m_textEditClicked = !m_textEditClicked;
	}
    screen->textedit();
}

void ControlWidget::on_rectangleBtn_clicked()
{
	rectangleClicked = true;
	m_textEditClicked = false;
	m_drawRoundClicked = false;
	m_arrowClicked = false;
	m_drawLineClicked = false;
	if (!m_rectangleClicked)
	{
		emit emitRectResize();
		m_rectangleClicked = !m_rectangleClicked;
	}
	else
	{
		emit emitRectHide();
		m_rectangleClicked = !m_rectangleClicked;
	}
    screen->drawrectang();
}

void ControlWidget::on_drawRoundBtn_clicked()
{
	drawRoundClicked = true;
	m_textEditClicked = false;
	m_rectangleClicked = false;
	m_arrowClicked = false;
	m_drawLineClicked = false;
	if (!m_drawRoundClicked)
	{
		emit emitRoundResize();
		m_drawRoundClicked = !m_drawRoundClicked;
	}
	else
	{
		emit emitRoundHide();
		m_drawRoundClicked = !m_drawRoundClicked;
	}
    screen->drawround();
}

void ControlWidget::on_arrowBtn_clicked()
{
	arrowClicked = true;
	m_textEditClicked = false;
	m_rectangleClicked = false;
	m_drawRoundClicked = false;
	m_drawLineClicked = false;
	if (!m_arrowClicked)
	{
		emit emitArrowResize();
		m_arrowClicked = !m_arrowClicked;
	}
	else
	{
		emit emitArrowHide();
		m_arrowClicked = !m_arrowClicked;
	}
    screen->drawarrow();
}

void ControlWidget::on_mosaicBtn_clicked()
{
    //
}

void ControlWidget::on_returneditBtn_clicked()
{
    //qDebug()<<"click";
	m_textEditClicked = false;
	m_drawRoundClicked = false;
	m_arrowClicked = false;
	m_drawLineClicked = false;
	m_rectangleClicked = false;
    emit emitundo();
}

ControlWidget::~ControlWidget()
{
	delete rectangleBtn;		
	rectangleBtn = nullptr;
	delete drawRoundBtn;		
	drawRoundBtn = nullptr;
	delete arrowBtn;			
	arrowBtn = nullptr;
	delete drawLineBtn;		
	drawLineBtn = nullptr;
	delete textEditBtn;		
	textEditBtn = nullptr;
	delete saveBtn;			
	saveBtn = nullptr;
	delete returneditBtn;		
	returneditBtn = nullptr;
	delete cancelBtn;			
	cancelBtn = nullptr;
	delete finishBtn;			
	finishBtn = nullptr;
	delete horizontalLayout;
	horizontalLayout = nullptr;
}
