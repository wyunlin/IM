#include "closedialog.h"

CloseDialog::CloseDialog(QWidget *parent) : QWidget(parent)
{

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("font-family:Microsoft YaHei;color:white;");
    this->resize(310,280);
    m_mainWidget = new QWidget(this);
    m_mainWidget->setFixedSize(300,270);
    m_mainWidget->setStyleSheet("background-color:#2D2F33");
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(5,5,5,5);
    m_mainLayout->addWidget(m_mainWidget);
    m_headWidet = new QWidget(m_mainWidget);
    m_contentWidget = new QWidget(m_mainWidget);
    m_footWidget = new QWidget(m_mainWidget);
    m_vLayout = new QVBoxLayout(m_mainWidget);
    m_vLayout->addWidget(m_headWidet,1);
    m_vLayout->addWidget(m_contentWidget,3);
    m_vLayout->addWidget(m_footWidget,1);
    m_tipLabel = new QLabel(QStringLiteral("关闭提示"),this);
    m_tipLabel->setStyleSheet("font-size:14px;");
    m_tipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_closeBtn = new QPushButton(m_headWidet);
    m_closeBtn->setFixedSize(30,30);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    m_closeBtn->setStyleSheet("image:url(:/Resources/blackclose.png);background-color:transparent;");
    m_headLayout = new QHBoxLayout(m_headWidet);
    m_headLayout->setContentsMargins(10,0,0,0);
    m_headLayout->addWidget(m_tipLabel);
    m_headLayout->addWidget(m_closeBtn);

    m_textLabel = new QLabel(QStringLiteral("点击关闭按钮时:"));
    m_minBtn = new QRadioButton(QStringLiteral("最小化到系统托盘"));
    m_minBtn->setChecked(true);
    m_quitBtn = new QRadioButton(QStringLiteral("退出"));
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->addWidget(m_textLabel);
    m_contentLayout->addWidget(m_minBtn);
    m_contentLayout->addWidget(m_quitBtn);

    m_okBtn = new QPushButton(QStringLiteral("确定"));
    m_okBtn->setFixedHeight(30);
    m_okBtn->setStyleSheet(R"(QPushButton{background-color:#2E4E7E;border:none;font-size:14px;}
                           QPushButton:hover{background-color:#3A6098;}
                           QPushButton:pressed{background-color:#3A6098;})");
    m_footLayout = new QHBoxLayout(m_footWidget);
    m_footLayout->addWidget(m_okBtn);
    connect(m_closeBtn,&QPushButton::clicked,[=]{
        this->hide();
    });
    connect(m_okBtn,&QPushButton::clicked,[=]{
        emit emitStatus(m_status);
    });
    connect(m_minBtn,&QRadioButton::clicked,[=]
    {
       m_status = false;
    });
    connect(m_quitBtn,&QRadioButton::clicked,[=]{
       m_status = true;
    });
}

CloseDialog::~CloseDialog()
{

}

void CloseDialog::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width() - 20, this->height() - 20);

    QPainter painter(this);
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
    QWidget::paintEvent(event);
}

void CloseDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    movePoint = event->globalPos() - this->pos();
}

void CloseDialog::mouseReleaseEvent(QMouseEvent *event)
{
    mousePress = false;
}

void CloseDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePress)
    {
        QPoint movePos = event->globalPos();
        this->move(movePos - movePoint);
    }
}
