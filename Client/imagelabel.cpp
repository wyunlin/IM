#include "imagelabel.h"

//ImageLabel::ImageLabel(QPixmap pixmap, QWidget *parent) : QWidget(parent),m_pixmap{pixmap}
//{

//}
ImageLabel::ImageLabel(QWidget *parent) : QLabel(parent)
{
        //resize(400,400);
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
//        m_imageScale = new ImageScale(m_pixmap);
//        m_imageScale->show();
        m_scaleWidget = new ScaleWidget(m_pixmap);
        m_scaleWidget->show();
    }
}


void ImageLabel::setImage(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

QPixmap ImageLabel::getImage() const
{
    return m_pixmap;
}

ImageLabel::~ImageLabel()
{

}

