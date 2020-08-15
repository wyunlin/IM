#include "Colorwidget.h"


ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent)
{
    //this->setGeometry(0,0,500,40);
    this->setFixedSize(500 ,40);
    this->setStyleSheet("background-color:#16181C;");
	this->setWindowFlags(Qt::FramelessWindowHint);
	//this->setAttribute(Qt::WA_TranslucentBackground);
	
    m_minFont = new QToolButton(this);
    m_minFont->setCursor(QCursor(Qt::PointingHandCursor));
    m_minFont->setAutoRaise(true);
    m_minFont->setFixedSize(32,32);
    //m_minFont->setStyleSheet("background-color:red;border-radius:3px;");

    m_midFont = new QToolButton(this);
    m_midFont->setCursor(QCursor(Qt::PointingHandCursor));
    m_midFont->setAutoRaise(true);
    //m_midFont->setStyleSheet("background-color:red;border-radius:3px");
    m_midFont->setFixedSize(32,32);
    m_maxFont = new QToolButton(this);
    m_maxFont->setCursor(QCursor(Qt::PointingHandCursor));
    m_maxFont->setAutoRaise(true);
    //m_maxFont->setStyleSheet("background-color:red;border-radius:3px");
    m_maxFont->setFixedSize(32,32);
	m_minLabel = new QLabel(m_minFont);
	m_minLabel->setGeometry(12, 12, 8, 8);
	m_minLabel->setStyleSheet("background-color:grey;border-radius:4px;");
	m_midLabel = new QLabel(m_midFont);
	m_midLabel->setGeometry(10, 10, 12, 12);
	m_midLabel->setStyleSheet("background-color:grey;border-radius:6px;");
	m_maxLabel = new QLabel(m_maxFont);
	m_maxLabel->setGeometry(8, 8, 16, 16);
	m_maxLabel->setStyleSheet("background-color:grey;border-radius:8px;");

    m_btnRed = new QToolButton(this);
    m_btnRed->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnRed->setAutoRaise(true);
    m_btnRed->setStyleSheet("background-color:red;border-radius:3px");
    m_btnRed->setFixedSize(32,32);

    m_btnGreen = new QToolButton(this);
    m_btnGreen->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnGreen->setAutoRaise(true);
    m_btnGreen->setStyleSheet("background-color:green;border-radius:3px");
    m_btnGreen->setFixedSize(32,32);

    m_btnBlue = new QToolButton(this);
    m_btnBlue->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnBlue->setAutoRaise(true);
    m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px");
    m_btnBlue->setFixedSize(32,32);

    m_btnOrange = new QToolButton(this);
    m_btnOrange->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnOrange->setAutoRaise(true);
    m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px");
    m_btnOrange->setFixedSize(32,32);

    m_btnBlack = new QToolButton(this);
    m_btnBlack->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnBlack->setAutoRaise(true);
    m_btnBlack->setStyleSheet("background-color:black;border-radius:3px");
    m_btnBlack->setFixedSize(32,32);

    m_btnGrey = new QToolButton(this);
    m_btnGrey->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnGrey->setAutoRaise(true);
    m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px");
    m_btnGrey->setFixedSize(32,32);

    //m_colorDialog = new QToolButton(this);
	m_colorDialog = new QToolButton();
    m_colorDialog->setCursor(QCursor(Qt::PointingHandCursor));
    m_colorDialog->setAutoRaise(true);
	m_colorDialog->setIcon(QIcon(":/Resources/colorBtn.png"));
    m_colorDialog->setFixedSize(32,32);

    m_hLayout = new QHBoxLayout(this);
	m_hLayout->setContentsMargins(0, 0, 0, 0);
    m_hLayout->addWidget(m_minFont);
    m_hLayout->addWidget(m_midFont);
    m_hLayout->addWidget(m_maxFont);
    m_hLayout->addWidget(m_btnRed);
    m_hLayout->addWidget(m_btnGreen);
    m_hLayout->addWidget(m_btnBlue);
    m_hLayout->addWidget(m_btnOrange);
    m_hLayout->addWidget(m_btnBlack);
    m_hLayout->addWidget(m_btnGrey);
    m_hLayout->addWidget(m_colorDialog);

	connect(m_minFont, &QToolButton::clicked, [=]() {emit emitFontSize(2); 
	m_minLabel->setStyleSheet("background-color:#1E90FF;border-radius:4px;"); 
	m_midLabel->setStyleSheet("background-color:grey;border-radius:6px;"); 
	m_maxLabel->setStyleSheet("background-color:grey;border-radius:8px;"); });
	connect(m_midFont, &QToolButton::clicked, [=]() {emit emitFontSize(4); 
	m_minLabel->setStyleSheet("background-color:grey;border-radius:4px;");
	m_midLabel->setStyleSheet("background-color:#1E90FF;border-radius:6px;");
	m_maxLabel->setStyleSheet("background-color:grey;border-radius:8px;"); });
	connect(m_maxFont, &QToolButton::clicked, [=]() {emit emitFontSize(6); 
	m_minLabel->setStyleSheet("background-color:grey;border-radius:4px;");
	m_midLabel->setStyleSheet("background-color:grey;border-radius:6px;");
	m_maxLabel->setStyleSheet("background-color:#1E90FF;border-radius:8px;"); });

    connect(m_colorDialog,&QToolButton::clicked,this,&ColorWidget::ShowColorBtn);
	connect(m_btnRed, &QToolButton::clicked, [=]() {emit emitFontColor(Qt::red); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;border:1px solid #00FF7F"); 
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;"); 
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;"); 
	m_colorDialog->setStyleSheet("border:0px;"); });
	connect(m_btnGreen, &QToolButton::clicked, [=]() {emit emitFontColor(Qt::green); 
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;border:1px solid #00FF7F"); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;");
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;");
	m_colorDialog->setStyleSheet("border:0px;"); });
	connect(m_btnBlue, &QToolButton::clicked, [=]() {emit emitFontColor(qRgb(30, 144, 255));
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;border:1px solid 	#00FF7F"); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;");
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;");
	m_colorDialog->setStyleSheet("border:0px;"); });
	connect(m_btnOrange, &QToolButton::clicked, [=]() {emit emitFontColor(qRgb(255, 165, 0)); 
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;border:1px solid #00FF7F"); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;");
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;");
	m_colorDialog->setStyleSheet("border:0px;"); });
	connect(m_btnBlack, &QToolButton::clicked, [=]() {emit emitFontColor(Qt::black); 
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;border:1px solid #00FF7F"); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;");
	m_colorDialog->setStyleSheet("border:0px;"); });
	connect(m_btnGrey, &QToolButton::clicked, [=]() {emit emitFontColor(qRgb(128, 128, 128)); 
	m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;border:1px solid #00FF7F"); 
	m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
	m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
	m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
	m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
	m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;");
	m_colorDialog->setStyleSheet("border:0px;"); });
}


void ColorWidget::hideBtnSize()
{
	m_minFont->hide();
	m_midFont->hide();
	m_maxFont->hide();
	m_hLayout->setContentsMargins(40, 0, 40, 0);
}

void ColorWidget::ShowColorBtn()
{
	QColorDialog colorD;
	//colorD.setGeometry(200, 0, 500, 500);
	emit emitShow(colorD);
    QColor color= colorD.getColor(Qt::red);
	colorD.setWindowFlags(Qt::WindowStaysOnTopHint);
    if(color.isValid())
    {
		emit emitFontColor(color);
		m_colorDialog->setStyleSheet("border-radius:3px;border:1px solid #00FF7F");
		m_btnRed->setStyleSheet("background-color:red;border-radius:3px;");
		m_btnGreen->setStyleSheet("background-color:green;border-radius:3px;");
		m_btnBlue->setStyleSheet("background-color:#1E90FF;border-radius:3px;");
		m_btnOrange->setStyleSheet("background-color:orange;border-radius:3px;");
		m_btnBlack->setStyleSheet("background-color:black;border-radius:3px;");
		m_btnGrey->setStyleSheet("background-color:grey;border-radius:3px;");
    }
}

ColorWidget::~ColorWidget()
{
	delete m_minLabel;
	m_minLabel = nullptr;
	delete m_midLabel;
	m_midLabel = nullptr;
	delete m_maxLabel;
	m_maxLabel = nullptr;
	delete m_minFont;
	m_minFont = nullptr;
	delete m_midFont;
	m_midFont = nullptr;
	delete m_maxFont;
	m_maxFont = nullptr;
	delete m_btnRed;
	m_btnRed = nullptr;
	delete m_btnGreen;
	m_btnGreen = nullptr;
	delete m_btnBlue;
	m_btnBlue = nullptr;
	delete m_btnOrange;
	m_btnOrange = nullptr;
	delete m_btnBlack;
	m_btnBlack = nullptr;
	delete m_btnGrey;
	m_btnGrey = nullptr;
	delete m_colorDialog;
	m_colorDialog = nullptr;
	delete m_hLayout;
	m_hLayout = nullptr;
}
