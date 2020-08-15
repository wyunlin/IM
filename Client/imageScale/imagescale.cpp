#include "imagescale.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>

ImageScale::ImageScale(QPixmap pixmap, QWidget *parent) :
    QWidget(parent)
  , m_pixmap(pixmap)
{
    //this->setFixedSize(600,600);
    //this->setWindowFlag(Qt::FramelessWindowHint);
    //m_mainLayout = new QVBoxLayout(this);
    //m_contentWidget = new QWidget;
    //m_mainLayout->addWidget(m_contentWidget);
    m_ratio = 0;
    m_Image = m_pixmap.toImage();
    this->setStyleSheet("background-color:white");
}

ImageScale::~ImageScale()
{

}

void ImageScale::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    pos = this->mapToGlobal(pos);
    QMenu *menu = new QMenu(this);

    //QAction *loadImage = new QAction(QString::fromLocal8Bit("Load Image"));
    //connect(loadImage,&QAction::triggered,this,&ImageScale::onLoadImage);
    //menu->addAction(loadImage);
    //menu->addSeparator();

    QAction *zoomInAction = new QAction(QString::fromLocal8Bit("Zoom in"));
    connect(zoomInAction,&QAction::triggered,this,&ImageScale::onZoomInImage);
    menu->addAction(zoomInAction);

    QAction *zoomoutAction = new QAction(QString::fromLocal8Bit("Zoom out"));
    connect(zoomoutAction,&QAction::triggered,this,&ImageScale::onZoomOutImage);
    menu->addAction(zoomoutAction);

    QAction *presetAction = new QAction(QString::fromLocal8Bit("Preset"));
    connect(presetAction,&QAction::triggered,this,&ImageScale::onPresetImage);
    menu->addAction(presetAction);

    QAction *saceAction = new QAction(QString::fromLocal8Bit("save"));
    connect(saceAction,&QAction::triggered,this,&ImageScale::onDownLoadImage);
    menu->addAction(saceAction);

//    m_menuStyle = "QMenu{ background-color: rgb(255, 255, 255); border: 1px solid rgb(125, 125, 125); /*整个菜单区域的边框粗细、样式、颜色*/ \
//            border-radius: 5px; }"
//    "QMenu::item { background-color: transparent; \
//                   padding:3px 20px;/*设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔*/ \
//                   margin:5px 10px;/*设置菜单项的外边距: 长(height) 宽(width)*/ \
//                   /*border-bottom:1px solid #DBDBDB;*//*为菜单项之间添加横线间隔*/ }"
//    "QMenu::item:selected { background-color:#1E90FF;/*这一句是设置菜单项鼠标经过选中的样式*/ }";
//    m_menuStyle = "\
//            QMenu {\
//            background-color:rgb(89,87,87); /*整个背景*/\
//            border: 3px solid rgb(235,110,36);/*整个菜单边缘*/\
//            }\
//        QMenu::item {\
//            font-size: 10pt; \
//            color: rgb(225,225,225);  /*字体颜色*/\
//            border: 3px solid rgb(60,60,60);    /*item选框*/\
//            background-color:rgb(89,87,87);\
//            padding:16px 16px; /*设置菜单项文字上下和左右的内边距，效果就是菜单中的条目左右上下有了间隔*/\
//            margin:2px 2px;/*设置菜单项的外边距*/\
//             }\
//        QMenu::item:selected { \
//            background-color:rgb(235,110,36);/*选中的样式*/\
//            }\
//        QMenu::item:pressed {/*菜单项按下效果*/\
//                                  border: 1px solid rgb(60,60,61); \
//                                  background-color: rgb(220,80,6); \
//                              }";
    QFile file(":/qss/menu.qss");
    file.open(QFile::ReadOnly);
    QString m_menuStyle = tr(file.readAll());
    file.close();
    menu->setStyleSheet(m_menuStyle);
    menu->exec(pos);
}

void ImageScale::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    if(m_Image.isNull())
        return QWidget::paintEvent(event);
    int width = qMin(m_Image.width(),this->width());
    int height = width*1.0/(m_Image.width()*1.0/m_Image.height());
    height = qMin(height,this->height());
    width = height*1.0*(m_Image.width()*1.0/m_Image.height());

    painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);

    // 缩放

    painter.scale(m_ZoomValue, m_ZoomValue);

    QRect picRect(-width/2,-height/2,width,height);
    painter.drawImage(picRect,m_Image);

}

void ImageScale::wheelEvent(QWheelEvent *event)
{
    int value = event->delta();
    if(value>0)
        onZoomInImage();
    else {
        onZoomOutImage();
    }
    this->update();
}

void ImageScale::mousePressEvent(QMouseEvent *event)
{
    m_OldPos = event->pos();
    m_Pressed = true;
}

void ImageScale::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_Pressed)
        return QWidget::mouseMoveEvent(event);

    this->setCursor(Qt::SizeAllCursor);
    QPoint pos = event->pos();
    int xPtInterval = pos.x() - m_OldPos.x();
    int yPtInterval = pos.y() - m_OldPos.y();

    m_XPtInterval += xPtInterval;
    m_YPtInterval += yPtInterval;

    m_OldPos = pos;
    this->update();
}

void ImageScale::mouseReleaseEvent(QMouseEvent *event)
{
    m_Pressed = false;
    this->setCursor(Qt::ArrowCursor);
}

void ImageScale::onLoadImage()
{
    QString imageFile = QFileDialog::getOpenFileName(this, "Open Image", "./", tr("Images (*.png *.xpm *.jpg)"));

    QFile file(imageFile);
    if(!file.exists())
        return;
    m_Image.load(imageFile);
}

void ImageScale::onZoomInImage()
{
    m_ZoomValue += 0.2;
    this->update();
}

void ImageScale::onZoomOutImage()
{
    m_ZoomValue -= 0.2;
    if (m_ZoomValue <= 0)
    {
        m_ZoomValue += 0.2;
        return;
    }

    this->update();
}

void ImageScale::onPresetImage()
{
    m_Image = m_pixmap.toImage();
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    this->update();
}

void ImageScale::onSpinImage()
{
    m_ZoomValue = 1.0;
    m_XPtInterval = 0;
    m_YPtInterval = 0;
    m_ratio+=90;
    QMatrix matrix;
    matrix.rotate(m_ratio);
    m_Image = m_Image.transformed(matrix, Qt::SmoothTransformation);
    this->repaint();
}

void ImageScale::onDownLoadImage()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)")); //选择路径
    m_Image.save(filename,"png");
}

