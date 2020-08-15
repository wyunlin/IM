#include "GeneralLogin.h"

GeneralLogin::GeneralLogin(QWidget *parent)
	: QWidget(parent)
	, mousePress{ false }
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(255, 255, 255)));
	this->setFixedSize(300, 260);
	this->setStyleSheet("background-color:white;font-family:Microsoft YaHei;");
    m_canLabel = new QLabel;
    //m_canLabel->setGeometry(150, 15, 40, 40);
    //m_canLabel->setStyleSheet("border-image:url(:/Resources/can.png);background-color:transparent");
    m_qcodeLabel = new QLabel;
	m_qcodeLabel->setStyleSheet("background-color:transparent;font-weight:bold;");
    //m_qcodeLabel->setGeometry(200, 6, 80, 20);
    //m_qcodeLabel->setText(QStringLiteral("账号登录"));
	m_nameLineEdit = new QLineEdit(this);
	m_nameLineEdit->setGeometry(25,70,250,30);
	m_nameLineEdit->setPlaceholderText(QStringLiteral("请输入手机号"));
	m_nameLineEdit->setStyleSheet("background-color:transparent;border:none;border-bottom:1px solid #292929;font-size:14px;outline:none");
	m_pwdLineEdit = new QLineEdit(this);
	m_pwdLineEdit->setGeometry(25, 110, 250, 30);
	m_pwdLineEdit->setPlaceholderText(QStringLiteral("请输入密码"));
	m_pwdLineEdit->setStyleSheet("background-color:transparent;border:none;border-bottom:1px solid #292929;font-size:14px;outline:none");
	m_pwdLineEdit->setEchoMode(QLineEdit::Password);
	m_rememberCheckBox = new QCheckBox(this);
	m_rememberCheckBox->setGeometry(25, 145, 60, 20);
	m_rememberCheckBox->setText(QStringLiteral("记住密码"));
	//m_rememberCheckBox->setStyleSheet("color:#808080;outline:none");
    m_rememberCheckBox->setStyleSheet("QCheckBox::indicator::checked{border:1px solid #1E90FF; image: url(:/Resources/check.png);border-radius: 2px;}\
		QCheckBox::indicator::unchecked {border:1px solid #D9D8D8;background-color:transparent;border-radius: 2px;}\
		QCheckBox::indicator { width: 10px;height: 10px;}\
		QCheckBox{background-color:transparent;color:#505050;outline:none;font-size:10px;}");
	m_forgetBtn = new QPushButton(this);
	m_forgetBtn->setGeometry(220, 145, 40, 20);
	m_forgetBtn->setText(QStringLiteral("忘记密码"));
	m_forgetBtn->setCursor(Qt::PointingHandCursor);
	m_forgetBtn->setStyleSheet("background-color:transparent;border:none;color:#505050;outline:none;font-size:10px;");

    m_registBtn = new QPushButton(this);
    m_registBtn->setGeometry(170, 145, 40, 20);
    m_registBtn->setText(QStringLiteral("注册账号"));
    m_registBtn->setCursor(Qt::PointingHandCursor);
    m_registBtn->setStyleSheet("background-color:transparent;border:none;color:#505050;outline:none;font-size:10px;");

	m_submitBtn = new QPushButton(this);
	m_submitBtn->setText(QStringLiteral("登录"));
	m_submitBtn->setGeometry(25, 194, 250, 36);
	m_submitBtn->setCursor(Qt::PointingHandCursor);
	m_submitBtn->setStyleSheet("border:none;background-color:#5a9afe;font-size:14px;color:white;border-radius:5px;outline:none");
    polygon << QPoint(0, 30) << QPoint(300, 30) << QPoint(300, 252) << QPoint(0, 252);
	m_pen.setColor(QColor(qRgb(90, 154, 254)));
	m_pen.setWidthF(1);
	m_pen.setCapStyle(Qt::RoundCap);
	connect(m_rememberCheckBox, &QCheckBox::toggled, [=](bool state) {
		emit emitCheckClicked(state); });
	connect(m_forgetBtn, &QPushButton::clicked, [=]() {
		emit emitForgetBtnClicked(); });
	connect(m_submitBtn, &QPushButton::clicked, [=]() {
		emit emitSubitBtnClicked(); });
    connect(m_registBtn, &QPushButton::clicked, [=]() {
        emit emitRegistBtnClicked(); });
//    connect(m_nameLineEdit,&QLineEdit::returnPressed,m_submitBtn,&QPushButton::clicked);
//    connect(m_pwdLineEdit,&QLineEdit::returnPressed,m_submitBtn,&QPushButton::clicked);
    connect(m_nameLineEdit,SIGNAL(returnPressed()),m_submitBtn,SIGNAL(clicked()));
    connect(m_pwdLineEdit,SIGNAL(returnPressed()),m_submitBtn,SIGNAL(clicked()));
}

void GeneralLogin::paintEvent(QPaintEvent * event)
{
	QPainter painter2(this);
	painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    //painter2.setBrush(QBrush(QColor(qRgb(238, 245, 255))));
    painter2.setBrush(QColor(Qt::white));
	//painter2.setBrush(QBrush(Qt::red));
	painter2.setPen(Qt::transparent);
	painter2.drawPolygon(polygon);
//	QPainter painter(this);
//	painter.setPen(m_pen);
//	painter.drawLine(0, 30, 140, 30);
//	painter.drawLine(160, 1, 300, 1);
//	QPainter painter3(this);
//	painter3.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
//	painter3.setPen(m_pen);
//	painter3.drawLine(141, 30, 160, 1);
	QWidget::paintEvent(event);
}

QString GeneralLogin::getName() const
{
   QString name = m_nameLineEdit->text();
   return name;
}

QString GeneralLogin::getPassword() const
{
    QString password = m_pwdLineEdit->text();
    return password;
}

GeneralLogin::~GeneralLogin()
{
	delete m_qcodeLabel;
	m_qcodeLabel = nullptr;
	delete m_nameLineEdit;
	m_nameLineEdit = nullptr;
	delete m_pwdLineEdit;
	m_pwdLineEdit = nullptr;
	delete m_rememberCheckBox;
	m_rememberCheckBox = nullptr;
	delete m_forgetBtn;
	m_forgetBtn = nullptr;
	delete m_submitBtn;
	m_submitBtn = nullptr;
	delete m_canLabel;
    m_canLabel = nullptr;
}

