#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QLabel>
#include "imageScale/imagewidget.h"
#include "imageScale/imagescale.h"
#include "imageScale/scalewidget.h"
class ImageLabel : public QLabel
{
    Q_OBJECT
public:
   // explicit ImageLabel(QPixmap pixmap, QWidget *parent = nullptr);
    explicit ImageLabel(QWidget *parent = nullptr);
    ~ImageLabel();
    void setImage(QPixmap pixmap);
    QPixmap getImage() const;
signals:

public slots:
protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QPixmap         m_pixmap;
    //ImageScale      *m_imageScale = nullptr;
    ScaleWidget     *m_scaleWidget = nullptr;
};

#endif // IMAGELABEL_H
