#include "scalewidget.h"
#include <QPainter>
#include <qmath.h>
#include <QDebug>

ScaleWidget::ScaleWidget(QPixmap pixmap, QWidget *parent) : QWidget(parent),m_pixmap{pixmap}
{
    this->resize(800,600);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("W IM");
    m_mainLayout = new QVBoxLayout(this);
    m_mainWidget = new QWidget();
    m_mainLayout->addWidget(m_mainWidget);
    m_topWidget = new QWidget();
    //m_imageWidget = new QWidget();
    m_imageScale = new ImageScale(m_pixmap);
    m_bottomWidget = new QWidget();
    m_contentLayout = new QVBoxLayout(m_mainWidget);
    m_contentLayout->setContentsMargins(0,0,0,0);
    m_contentLayout->addWidget(m_topWidget,1);
    //m_contentLayout->addWidget(m_imageWidget,10);
    m_contentLayout->addWidget(m_imageScale,16);
    m_contentLayout->addWidget(m_bottomWidget,2);
    m_topLayout = new QHBoxLayout(m_topWidget);
    m_spaceLabel = new QLabel();
    m_spaceLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_btnMin = new QPushButton();
    m_btnMin->setFixedSize(20,20);
    m_btnMin->setCursor(Qt::PointingHandCursor);
    connect(m_btnMin,&QPushButton::clicked,[=]
    {
        this->showMinimized();
    });
    m_btnMax = new QPushButton();
    m_btnMax->setFixedSize(20,20);
    m_btnMax->setCursor(Qt::PointingHandCursor);
    connect(m_btnMax,&QPushButton::clicked,[=]{
        if(m_maxWidget == Yes)
        {
            this->showMaximized();
            m_maxWidget = No;
        }
        else if(m_maxWidget == No)
        {
            this->showNormal();
            m_maxWidget = Yes;
        }
    });
    m_btnClose = new QPushButton();
    m_btnClose->setFixedSize(20,20);
    m_btnClose->setCursor(Qt::PointingHandCursor);
    m_topLayout->setContentsMargins(0,0,10,0);
    m_topLayout->addWidget(m_spaceLabel);
    m_topLayout->addWidget(m_btnMin);
    m_topLayout->addWidget(m_btnMax);
    m_topLayout->addWidget(m_btnClose);

    m_btnMin->setStyleSheet("border-image:url(:/Resources/min.png);background-color:transparent");
    m_btnMax->setStyleSheet("border-image:url(:/Resources/max.png);background-color:transparent");
    m_btnClose->setStyleSheet("border-image:url(:/Resources/blackclose.png);background-color:transparent");
    connect(m_btnClose,&QPushButton::clicked,[=]{
        this->close();
    });

    m_enlarge = new QPushButton(m_bottomWidget);
    m_enlarge->setFixedSize(20,20);
    m_enlarge->setCursor(Qt::PointingHandCursor);
    m_enlarge->setStyleSheet("border-image:url(:/Resources/enlarge.png)");
    m_zoomin = new QPushButton(m_bottomWidget);
    m_zoomin->setFixedSize(20,20);
    m_zoomin->setCursor(Qt::PointingHandCursor);
    m_zoomin->setStyleSheet("border-image:url(:/Resources/Zoomout.png)");
    m_ratio = new QPushButton(m_bottomWidget);
    m_ratio->setFixedSize(20,20);
    m_ratio->setCursor(Qt::PointingHandCursor);
    m_ratio->setStyleSheet("border-image:url(:/Resources/ratio.png)");
    m_spin = new QPushButton(m_bottomWidget);
    m_spin->setFixedSize(20,20);
    m_spin->setCursor(Qt::PointingHandCursor);
    m_spin->setStyleSheet("border-image:url(:/Resources/Spin.png)");
    m_downLoad = new QPushButton(m_bottomWidget);
    m_downLoad->setFixedSize(20,20);
    m_downLoad->setCursor(Qt::PointingHandCursor);
    m_downLoad->setStyleSheet("border-image:url(:/Resources/download.png)");
    m_btnGroup = new QButtonGroup(m_bottomWidget);
    m_btnGroup->addButton(m_enlarge,1);
    m_btnGroup->addButton(m_zoomin,2);
    m_btnGroup->addButton(m_ratio,3);
    m_btnGroup->addButton(m_spin,4);
    m_btnGroup->addButton(m_downLoad,5);
    m_bottomSpace1 = new QLabel();
    m_bottomSpace1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_bottomSpace2 = new QLabel();
    m_bottomSpace2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_bottomLayout = new QHBoxLayout(m_bottomWidget);
    m_bottomLayout->addWidget(m_bottomSpace1);
    m_bottomLayout->addWidget(m_enlarge);
    m_bottomLayout->addWidget(m_zoomin);
    m_bottomLayout->addWidget(m_ratio);
    m_bottomLayout->addWidget(m_spin);
    m_bottomLayout->addWidget(m_downLoad);
    m_bottomLayout->addWidget(m_bottomSpace2);
    connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotBtnGroup(int)));

}

void ScaleWidget::slotBtnGroup(int i)
{
    switch (i) {
    case 1:
        qDebug()<<"1";
        m_imageScale->onZoomInImage();
        break;
    case 2:
        qDebug()<<"2";
        m_imageScale->onZoomOutImage();
        break;
    case 3:
        qDebug()<<"3";
        m_imageScale->onPresetImage();
        break;
    case 4:
        qDebug()<<"4";
        m_imageScale->onSpinImage();
        break;
    case 5:
        qDebug()<<"5";
        m_imageScale->onDownLoadImage();
        break;
    default:
        break;
    }
}

void ScaleWidget::paintEvent(QPaintEvent *event)
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
void ScaleWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    movePoint = event->globalPos() - this->pos();
}

void ScaleWidget::mouseReleaseEvent(QMouseEvent * event)
{
    mousePress = false;
}

void ScaleWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (mousePress)
    {
        QPoint movePos = event->globalPos();
        this->move(movePos - movePoint);
    }
}

ScaleWidget::~ScaleWidget()
{
    delete m_spaceLabel;
    m_spaceLabel = nullptr;
    delete m_btnMin;
    m_btnMin = nullptr;
    delete m_btnMax;
    m_btnMax = nullptr;
    delete m_btnClose;
    m_btnClose = nullptr;
    delete m_topLayout;
    m_topLayout = nullptr;
    delete m_topWidget;
    m_topWidget = nullptr;
    delete m_imageScale;
    m_imageScale = nullptr;
    delete m_bottomSpace1;
    m_bottomSpace1 = nullptr;
    delete m_bottomSpace2;
    m_bottomSpace2 = nullptr;
    delete m_enlarge;
    m_enlarge = nullptr;
    delete m_zoomin;
    m_zoomin = nullptr;
    delete m_ratio;
    m_ratio = nullptr;
    delete m_spin;
    m_spin = nullptr;
    delete m_downLoad;
    m_downLoad = nullptr;
    delete m_btnGroup;
    m_btnGroup = nullptr;
    delete m_bottomLayout;
    m_bottomLayout = nullptr;
    delete m_bottomWidget;
    m_bottomWidget = nullptr;
}
