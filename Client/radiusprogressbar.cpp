#include "radiusprogressbar.h"
#include <QDebug>

RadiusProgressBar::RadiusProgressBar(QWidget *parent): QProgressBar(parent)
{
    setMinimum(0);
    setMaximum(100);
    setValue(0);
    buttomMap = QPixmap(":/Resources/radius_back.png");
    fillMap = QPixmap(":/Resources/radius_front.png");
    rect = QRect(0, 0, 281, 21);
    buttomMap = buttomMap.scaled(QSize(rect.width(),rect.height()),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fillMap = fillMap.scaled(QSize(rect.width(),rect.height()),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void RadiusProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QRect textRect = QRect(0, height()/2, width(), height()/2);

    QRect fillRect = rect.adjusted(0, 0, rect.width()*(((float)this->value())/100), 0);
    fillRect.setWidth(fillRect.width()-rect.width());

//    QRect fillRect;
//    fillRect.setX(rect.width()*(float(this->value())/100));
//    fillRect.setY(rect.height()*(float(this->value())/100));
    //qDebug()<<fillRect.width();
    //qDebug()<<fillRect.height();
//    qDebug()<<"value()"<<value();
//    qDebug()<<"minimum()"<<minimum();
//    qDebug()<<"maximum()"<<maximum();
    QString valueStr = QString("%1%").arg(QString::number(value())); 
//    qDebug()<<buttomMap;
//    qDebug()<<fillMap;
    //画进度条
    p.drawPixmap(rect, buttomMap);
    p.drawPixmap(fillRect, fillMap, fillRect);

    //画文字
    QFont f = QFont("Microsoft YaHei", 12, QFont::Bold);
    p.setFont(f);
    p.setPen(QColor("#555555"));
    p.drawText(textRect, Qt::AlignCenter, valueStr);

    //QProgressBar::paintEvent(event);
}


RadiusProgressBar::~RadiusProgressBar()
{

}
