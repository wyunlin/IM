#include "informationwidget.h"
#include <QPainter>
#include <qmath.h>

InformationWidget::InformationWidget(QPixmap pixmap, QWidget *parent) : QWidget(parent)
  ,m_pixmap{pixmap}
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setStyleSheet("font-family:Microsoft YaHei;");
    m_contentWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->addWidget(m_contentWidget);
    this->setFixedSize(300,240);
    m_topWidget = new QWidget(m_contentWidget);
    m_bottomWidget = new QWidget(m_contentWidget);
    m_btnSplit = new QPushButton(m_contentWidget);
    m_btnSplit->setFocusPolicy(Qt::NoFocus);
    m_btnSplit->setFixedHeight(1);
    m_btnSplit->setStyleSheet("background-color:rgb(255,255,255);border:1px solid #E6E6E6;");
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->addWidget(m_topWidget);
    m_contentLayout->addWidget(m_btnSplit);
    m_contentLayout->addWidget(m_bottomWidget);
    m_nameLabel = new QLabel(m_topWidget);
    m_name = new QLabel(m_nameLabel);
    m_name->setGeometry(10,10,30,30);
    m_name->setStyleSheet("font-size:12pt;");
    m_name->setText("free");
    m_icoLabel = new QLabel(m_nameLabel);
    m_icoLabel->setPixmap(QPixmap(":/Resources/Contact.png"));
    m_icoLabel->setGeometry(40,10,30,30);
    m_btnHead = new QPushButton(m_topWidget);
    m_btnHead->setFixedSize(50,50);

    QSize sz(50, 50);
    QPixmap tempHead_1 = m_pixmap.scaled(sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPixmap ico(50, 50);
    ico.fill(Qt::transparent);
    QPainter painter(&ico);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    // 裁剪为圆角
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, 50, 50),3,3);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 50, 50, tempHead_1);
    m_btnHead->setText("");
    m_btnHead->setIcon(ico);
    QSize size(50, 50);
    m_btnHead->setIconSize(size);
    connect(m_btnHead,&QPushButton::clicked,this,&InformationWidget::slot_Scale);
    m_btnHead->setStyleSheet("background-color:grey;border:1px solid #FFFFFF;border-radius:3px;");
    m_btnHead->setCursor(Qt::PointingHandCursor);
    m_topLayout = new QGridLayout(m_topWidget);
    m_topLayout->setContentsMargins(10,0,10,0);
    m_topLayout->addWidget(m_nameLabel,0,0,1,1);
    m_topLayout->addWidget(m_btnHead,0,1,1,1);
}

void InformationWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width() - 20, this->height() - 20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(192, 192, 192, 50);
    for (int i = 0; i < 10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
        color.setAlpha(120 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QPainter painter2(this);
    painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter2.setBrush(QBrush(Qt::white));
    painter2.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setLeft(9);
    rect.setTop(9);
    rect.setWidth(rect.width() - 9);
    rect.setHeight(rect.height() - 9);
    painter2.drawRoundedRect(rect, 4, 4);
    QWidget::paintEvent(event);
}

bool InformationWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(QApplication::activeWindow() != this)
        {
            this->close();
        }
    }

    return QWidget::event(event);
}

void InformationWidget::slot_Scale()
{
    m_scaleWidget = new ScaleWidget(m_pixmap);
    m_scaleWidget->show();
}

InformationWidget::~InformationWidget()
{

}
