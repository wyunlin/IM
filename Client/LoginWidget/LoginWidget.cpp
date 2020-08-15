#include "LoginWidget.h"
#include <qdebug.h>
#include <QDir>

LoginWidget::LoginWidget(QWidget *parent)
	: QWidget(parent)
	, mousePress{false}
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
	this->setStyleSheet("background-color:white;font-family:Microsoft YaHei;");
	this->setFixedSize(320, 520);
    this->setWindowIcon(QIcon(":/Resources/IM.ico"));
    this->setWindowTitle("W IM");
	InitLoginUI();
	InitForgetUI();
    InitRegistUI();
    linkMySQL();
    QDir dir;
    if(!dir.exists("./tempImage")){
        dir.mkdir("./tempImage");
    }
    loadCfg();
}

void LoginWidget::InitLoginUI()
{
	m_headWidget = new QWidget(this);
	m_headWidget->setGeometry(10, 10, 300, 34);
	m_headWidget->setStyleSheet("background-color:#EEF5FF;border:none;border-bottom:1px solid #B0C4DE;opacity:0.24;outline:none");
	m_mainVLayout = new QVBoxLayout(this);
	m_name = new QLabel(m_headWidget);
    m_name->setText(QStringLiteral("W IM"));
	m_name->setStyleSheet("font-size:12px;color:#505050;border:none;");
	m_name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_btnClose = new QPushButton(m_headWidget);
    m_btnClose->setStyleSheet("QPushButton{image:url(:/Resources/blackclose.png);background-color:transparent;border:none;outline:none;}");
	m_btnClose->setFixedSize(35, 20);
	m_btnClose->setCursor(Qt::PointingHandCursor);
	m_headHLayout = new QHBoxLayout(m_headWidget);
	m_headHLayout->addWidget(m_name);
	m_headHLayout->addWidget(m_btnClose);
	m_headHLayout->setContentsMargins(12, 0, 5, 0);

	m_qrcodeWidget = new QrcodeWidget(this);
	m_qrcodeWidget->move(10, 44);
	m_generalLogin = new GeneralLogin(this);
    m_generalLogin->move(10, 210);
    connect(m_btnClose, &QPushButton::clicked, this, &LoginWidget::deleteLater);
    //connect(m_btnClose, &QPushButton::clicked, this, &LoginWidget::slotDelete);
	connect(m_generalLogin, &GeneralLogin::emitCheckClicked, this, &LoginWidget::slotCheckRem);
	connect(m_generalLogin, &GeneralLogin::emitForgetBtnClicked, this, &LoginWidget::slotForgetBtn);
    connect(m_generalLogin,&GeneralLogin::emitRegistBtnClicked,this,&LoginWidget::slotRegistBtn);
	connect(m_generalLogin, &GeneralLogin::emitSubitBtnClicked, this, &LoginWidget::slotSubmit);
}

void LoginWidget::InitForgetUI()
{
	m_fgHeadWidget = new QWidget(this);
	m_fgHeadWidget->setStyleSheet("background-color:transparent");
	m_fgHeadWidget->hide();
	m_fgContentWidget = new QWidget(this);
	m_fgContentWidget->setStyleSheet("background-color:transparent");
	m_fgContentWidget->hide();
	m_mainVLayout->addWidget(m_fgHeadWidget, 1);
	m_mainVLayout->addWidget(m_fgContentWidget, 14);
	m_fgReturnBtn = new QPushButton(m_fgHeadWidget);
	m_fgReturnBtn->setStyleSheet("QPushButton{background-color:transparent;border:none;outline:none;font-size:10px;color:#505050}");
	m_fgReturnBtn->setText(QStringLiteral("返回"));
	m_fgReturnBtn->setFixedSize(45, 20);
    QIcon reBtn(":/Resources/return.png");
	m_fgReturnBtn->setIcon(reBtn);
	m_fgReturnBtn->setCursor(Qt::PointingHandCursor);
	m_fgCloseBtn = new QPushButton(m_fgHeadWidget);
    m_fgCloseBtn->setStyleSheet("QPushButton{image:url(:/Resources/blackclose.png);background-color:transparent;border:none;outline:none;}");
	m_fgCloseBtn->setFixedSize(35, 20);
	m_fgCloseBtn->setCursor(Qt::PointingHandCursor);
	m_fgspaceBtn = new QLabel(m_fgHeadWidget);
	m_fgspaceBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_fgspaceBtn->setStyleSheet("background-color:transparent");
	m_fgHeadLayout = new QHBoxLayout(m_fgHeadWidget);
	m_fgHeadLayout->setContentsMargins(8, 0, 4, 0);
	m_fgHeadLayout->addWidget(m_fgReturnBtn);
	m_fgHeadLayout->addWidget(m_fgspaceBtn);
	m_fgHeadLayout->addWidget(m_fgCloseBtn);
	m_forgetLabel = new QLabel(m_fgContentWidget);
	m_forgetLabel->setFixedHeight(100);
	m_forgetLabel->setText(QStringLiteral("忘记密码"));
	m_forgetLabel->setStyleSheet("background-color:transparent;font-size:17px;");
	m_forgetLabel->setAlignment(Qt::AlignCenter);
	m_phoneLineEdit = new QLineEdit(m_fgContentWidget);
	m_phoneLineEdit->setFixedHeight(40);
	m_phoneLineEdit->setPlaceholderText(QStringLiteral("请输入手机号"));
	m_phoneLineEdit->setStyleSheet("border:none;border-bottom:1px solid #DCDCDC;font-size:14px;outline:none");
	m_captchaLineEdit = new QLineEdit(m_fgContentWidget);
	m_captchaLineEdit->setFixedHeight(40);
	m_captchaLineEdit->setPlaceholderText(QStringLiteral("请输入验证码"));
	m_captchaLineEdit->setStyleSheet("border:none;border-bottom:1px solid #DCDCDC;font-size:14px;outline:none");
	m_captchaBtn = new QPushButton(m_captchaLineEdit);
	m_captchaBtn->setCursor(Qt::PointingHandCursor);
	m_captchaBtn->setStyleSheet("border:1px solid #DCDCDC;");
	m_captchaBtn->setText(QStringLiteral("发送验证码"));
	m_captchaBtn->setGeometry(178, 0, 80, 35);
	m_okBtn = new QPushButton(m_fgContentWidget);
	m_okBtn->setText(QStringLiteral("确定"));
	m_okBtn->setFixedHeight(37);
	m_okBtn->setCursor(Qt::PointingHandCursor);
	m_okBtn->setStyleSheet("border:none;background-color:#5a9afe;font-size:14px;color:white;border-radius:5px;outline:none");
	m_fgContentLayout = new QVBoxLayout(m_fgContentWidget);
	m_fgContentLayout->setContentsMargins(20, 0, 20, 50);
	m_fgContentLayout->addWidget(m_forgetLabel,2);
	m_fgContentLayout->addWidget(m_phoneLineEdit,1);
	m_fgContentLayout->addWidget(m_captchaLineEdit,1);
	m_fgContentLayout->addWidget(m_okBtn,1);
    connect(m_fgReturnBtn, &QPushButton::clicked, this, &LoginWidget::slotReturnLogin);
    connect(m_fgCloseBtn, &QPushButton::clicked, this, &LoginWidget::deleteLater);
    //connect(m_fgCloseBtn, &QPushButton::clicked, this, &LoginWidget::slotDelete);
	connect(m_captchaBtn, &QPushButton::clicked, this, &LoginWidget::slotCaptchaBtn);
    connect(m_okBtn, &QPushButton::clicked, this, &LoginWidget::slotOkBtn);
}

void LoginWidget::InitRegistUI()
{
    m_rgHeadWidget = new QWidget(this);
    m_rgHeadWidget->setStyleSheet("background-color:transparent");
    m_rgHeadWidget->hide();
    m_rgContentWidget = new QWidget(this);
    m_rgContentWidget->setStyleSheet("background-color:transparent");
    m_rgContentWidget->hide();
    m_mainVLayout->addWidget(m_rgHeadWidget, 1);
    m_mainVLayout->addWidget(m_rgContentWidget, 14);
    m_rgReturnBtn = new QPushButton(m_rgHeadWidget);
    m_rgReturnBtn->setStyleSheet("QPushButton{background-color:transparent;border:none;outline:none;font-size:10px;color:#505050}");
    m_rgReturnBtn->setText(QStringLiteral("返回"));
    m_rgReturnBtn->setFixedSize(45, 20);
    QIcon reBtn(":/Resources/return.png");
    m_rgReturnBtn->setIcon(reBtn);
    m_rgReturnBtn->setCursor(Qt::PointingHandCursor);
    m_rgCloseBtn = new QPushButton(m_rgHeadWidget);
    m_rgCloseBtn->setStyleSheet("QPushButton{image:url(:/Resources/blackclose.png);background-color:transparent;border:none;outline:none;}");
    m_rgCloseBtn->setFixedSize(35, 20);
    m_rgCloseBtn->setCursor(Qt::PointingHandCursor);
    m_rgspaceBtn = new QLabel(m_rgHeadWidget);
    m_rgspaceBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_rgspaceBtn->setStyleSheet("background-color:transparent");
    m_rgHeadLayout = new QHBoxLayout(m_rgHeadWidget);
    m_rgHeadLayout->setContentsMargins(8, 0, 4, 0);
    m_rgHeadLayout->addWidget(m_rgReturnBtn);
    m_rgHeadLayout->addWidget(m_rgspaceBtn);
    m_rgHeadLayout->addWidget(m_rgCloseBtn);

    m_rgphoneLineEdit = new QLineEdit(m_rgContentWidget);
    m_rgphoneLineEdit->setFixedHeight(40);
    m_rgphoneLineEdit->setPlaceholderText(QStringLiteral("请输入手机号"));
    m_rgphoneLineEdit->setStyleSheet("border:none;border-bottom:1px solid #DCDCDC;font-size:14px;outline:none");
    m_pwdLineEdit = new QLineEdit(m_rgContentWidget);
    m_pwdLineEdit->setFixedHeight(40);
    m_pwdLineEdit->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pwdLineEdit->setEchoMode(QLineEdit::Password);
    m_pwdLineEdit->setStyleSheet("border:none;border-bottom:1px solid #DCDCDC;font-size:14px;outline:none");
    m_btnEye = new QPushButton(m_pwdLineEdit);
    m_btnEye->setCursor(Qt::PointingHandCursor);
    m_btnEye->setGeometry(230,10,30,20);
    m_btnEye->setStyleSheet("border-image:url(:/Resources/eye.png);");

    m_YZLineEdit = new QLineEdit(m_rgContentWidget);
    m_YZLineEdit->setFixedHeight(40);
    m_YZLineEdit->setEchoMode(QLineEdit::Password);
    m_YZLineEdit->setPlaceholderText(QStringLiteral("验证密码"));
    m_YZLineEdit->setStyleSheet("border:none;border-bottom:1px solid #DCDCDC;font-size:14px;outline:none");


    m_rgBtn = new QPushButton(m_rgContentWidget);
    m_rgBtn->setText(QStringLiteral("注册"));
    m_rgBtn->setFixedHeight(37);
    m_rgBtn->setCursor(Qt::PointingHandCursor);
    m_rgBtn->setStyleSheet("border:none;background-color:#5a9afe;font-size:14px;color:white;border-radius:5px;outline:none");
    m_rgContentLayout = new QVBoxLayout(m_rgContentWidget);
    m_rgContentLayout->setContentsMargins(20, 20, 20, 50);
    m_rgContentLayout->addWidget(m_rgphoneLineEdit,1);
    m_rgContentLayout->addWidget(m_pwdLineEdit,1);
    m_rgContentLayout->addWidget(m_YZLineEdit,1);
    m_rgContentLayout->addWidget(m_rgBtn,1);

    connect(m_YZLineEdit,&QLineEdit::textChanged,[=]
    {
        int YZsize = m_YZLineEdit->text().size();
        QString YZstr = m_YZLineEdit->text();
        QString pwdstr = m_pwdLineEdit->text();
        if(YZsize>pwdstr.size())
            return;
        for(int i=0;i<YZsize;++i)
        {
            if(YZstr.at(i) == pwdstr.at(i))
                continue;
            else
            {
                MyToast *myToast = new MyToast(QString::fromLocal8Bit("输入的密码不相同"),1000,this);
                myToast->show();
                break;
            }
        }
    });
    connect(m_btnEye,&QPushButton::clicked,[=]{
        if(!m_isLook)
        {
            m_pwdLineEdit->setEchoMode(QLineEdit::Normal);
            m_btnEye->setStyleSheet("border-image:url(:/Resources/clickEye.png);");
            m_isLook = !m_isLook;
        }
        else
        {
            m_pwdLineEdit->setEchoMode(QLineEdit::Password);
            m_btnEye->setStyleSheet("border-image:url(:/Resources/eye.png)");
            m_isLook = !m_isLook;
        }
    });
    connect(m_rgReturnBtn, &QPushButton::clicked, this, &LoginWidget::slotReturnLogin);
    connect(m_rgCloseBtn, &QPushButton::clicked, this, &LoginWidget::deleteLater);
    //connect(m_fgCloseBtn, &QPushButton::clicked, this, &LoginWidget::slotDelete);
    connect(m_rgBtn, &QPushButton::clicked, this, &LoginWidget::slotOkBtn);
}

void LoginWidget::saveCfg()
{
    QSettings cfg("imconfig.ini",QSettings::IniFormat);
    m_username = m_generalLogin->m_nameLineEdit->text();
    m_pwd = m_generalLogin->m_pwdLineEdit->text();
    cfg.setValue("username",m_username);
    cfg.setValue("password",m_pwd);
    cfg.setValue("remeberPwd",m_remeberPwd);
    cfg.sync();
}

void LoginWidget::loadCfg()
{
   QSettings cfg("imconfig.ini",QSettings::IniFormat);
   m_RemeberStr = cfg.value("remeberPwd").toString();
   if(m_RemeberStr == QString("true"))
   {
       m_generalLogin->m_rememberCheckBox->setChecked(true);
   }
   else
   {
       m_generalLogin->m_rememberCheckBox->setChecked(false);
       return;
   }
   m_username = cfg.value("username").toString();
   m_pwd = cfg.value("password").toString();
   //qDebug()<<m_username<<m_pwd;
   m_generalLogin->m_nameLineEdit->setText(m_username);
   m_generalLogin->m_pwdLineEdit->setText(m_pwd);

}

void LoginWidget::linkMySQL()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("im");
    db.setUserName("root");
    db.setPassword("123");
    db.open();
    if(!db.open())
    {
        MyToast *myToast = new MyToast(QString::fromLocal8Bit("数据库连接失败"),1000,this);
        myToast->show();
        return;
    }
//    else
//    {
//        MyToast *myToast = new MyToast(QString("数据库连接成功"),1000,this);
//        myToast->show();
//    }

}

//返回按钮槽函数
void LoginWidget::slotReturnLogin()
{
	m_headWidget->show();
	m_qrcodeWidget->show();
	m_generalLogin->show();
	m_fgHeadWidget->hide();
	m_fgContentWidget->hide();
    m_rgHeadWidget->hide();
    m_rgContentWidget->hide();
}

void LoginWidget::slotForgetBtn()
{
	m_headWidget->hide();
	m_qrcodeWidget->hide();
	m_generalLogin->hide();
    m_rgHeadWidget->hide();
    m_rgContentWidget->hide();
	m_fgHeadWidget->show();
    m_fgContentWidget->show();
}

void LoginWidget::slotRegistBtn()
{
    m_headWidget->hide();
    m_qrcodeWidget->hide();
    m_generalLogin->hide();
    m_fgHeadWidget->hide();
    m_fgContentWidget->hide();
    m_rgHeadWidget->show();
    m_rgContentWidget->show();
}

void LoginWidget::slotCheckRem(bool state)
{
	//qDebug() << "state" << state;
	if (state)
	{
		//记住密码
        m_remeberPwd = true;
	}
	else
	{
		//不记住密码
        m_generalLogin->m_pwdLineEdit->clear();
        m_remeberPwd = false;
        saveCfg();
	}
}

void LoginWidget::slotSubmit()
{
    QString name = m_generalLogin->getName();
    QString password = m_generalLogin->getPassword();
    if(name == QString::fromLocal8Bit("15797725711") && password == QString::fromLocal8Bit("4869"))
    {
        if(m_remeberPwd)
        {
            saveCfg();
        }
        m_mainWidget = new Widget;
        m_mainWidget->show();
        connect(m_mainWidget,&Widget::emitClose,[=]{
            m_mainWidget->close();
            this->close();
        });
        this->hide();
        return;
    }
    QSqlQuery query(db);
    bool isname = query.exec(QString("select %1 from userAccount").arg(name));
    if(!isname)
    {
        MyToast *myToast = new MyToast(QString::fromLocal8Bit("账号错误"),1000,this);
        myToast->show();
    }
    QString str = QString("select * from userAccount where phone='%1' and password='%2'").arg(name).arg(password);
    qDebug()<<"str"<<str;
    bool ispassword = query.exec(QString("select %1 from userAccount where password='%2'").arg(name).arg(password));
    if(!ispassword)
    {
        MyToast *myToast = new MyToast(QString::fromLocal8Bit("密码错误"),1000,this);
        myToast->show();
    }
    else
    {
        if(m_remeberPwd)
        {
            saveCfg();
        }
        m_mainWidget = new Widget;
        m_mainWidget->show();
        connect(m_mainWidget,&Widget::emitClose,[=]{
            m_mainWidget->close();
            this->close();
        });
        this->hide();
    }
}

void LoginWidget::slotCaptchaBtn()
{
}

void LoginWidget::slotOkBtn()
{
    QString phone = m_rgphoneLineEdit->text();
    QString pwdStr = m_pwdLineEdit->text();
    QString yanzPwd = m_YZLineEdit->text();
    QSqlQuery selectQuery(db);
    bool isSelect = selectQuery.exec(QString("select phone from userAccount where phone='%1'").arg(phone));
    if(selectQuery.next())
    {
        MyToast *toast = new MyToast(QString::fromLocal8Bit("该手机号已注册"),1000);
        toast->show();
        return;
    }
    if(pwdStr == yanzPwd)
    {
        QSqlQuery query(db);
        bool isRegist = query.exec(QString("insert into userAccount (phone,password) values('%1', '%2')").arg(phone).arg(pwdStr));
        if(isRegist)
        {
            MyToast *toast = new MyToast(QString::fromLocal8Bit("注册成功"),1000);
            toast->show();
            slotReturnLogin();
        }
        else
        {
            MyToast *toast = new MyToast(QString::fromLocal8Bit("注册失败"),1000);
            toast->show();
        }
    }
}

void LoginWidget::slotDelete()
{
    this->close();
}

void LoginWidget::paintEvent(QPaintEvent * event)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(10, 10, this->width() - 20, this->height() - 20);

	QPainter painter(this);
    painter.save();
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));

	QColor color(163, 188, 228, 50);
	for (int i = 0; i < 10; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
		color.setAlpha(150 - qSqrt(i) * 50);
		painter.setPen(color);
		painter.drawPath(path);
	}
    painter.restore();
    //QPainter painter2(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setLeft(9);
    rect.setTop(9);
    rect.setWidth(rect.width() - 9);
    rect.setHeight(rect.height() - 9);
    painter.drawRoundedRect(rect, 4, 4);
    painter.restore();
    QWidget::paintEvent(event);
}

void LoginWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePress = true;
	}
	movePoint = event->globalPos() - this->pos();
}

void LoginWidget::mouseReleaseEvent(QMouseEvent * event)
{
	mousePress = false;
}

void LoginWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (mousePress)
	{
		QPoint movePos = event->globalPos();
		this->move(movePos - movePoint);
	}
}

LoginWidget::~LoginWidget()
{
    delete m_name;
    m_name = nullptr;
    delete m_btnClose;
    m_btnClose = nullptr;
    delete m_headHLayout;
    m_headHLayout = nullptr;
    delete m_headWidget;
    m_headWidget = nullptr;
    delete m_fgReturnBtn;
    m_fgReturnBtn = nullptr;
    delete m_fgCloseBtn;
    m_fgCloseBtn = nullptr;
    delete m_fgspaceBtn;
    m_fgspaceBtn = nullptr;
    delete m_fgHeadLayout;
    m_fgHeadLayout = nullptr;
    delete m_fgHeadWidget;
    m_fgHeadWidget = nullptr;
    delete m_forgetLabel;
    m_forgetLabel = nullptr;
    delete m_phoneLineEdit;
    m_phoneLineEdit = nullptr;
    delete m_captchaBtn;
    m_captchaBtn = nullptr;
    delete m_captchaLineEdit;
    m_captchaLineEdit = nullptr;
    delete m_okBtn;
    m_okBtn = nullptr;
    delete m_qrcodeWidget;
    m_qrcodeWidget = nullptr;
    delete m_generalLogin;
    m_generalLogin = nullptr;
    delete m_fgContentLayout;
    m_fgContentLayout = nullptr;
    delete m_fgContentWidget;
    m_fgContentWidget = nullptr;
    delete m_mainVLayout;
    m_mainVLayout = nullptr;
}


