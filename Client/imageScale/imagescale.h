#ifndef IMAGESCALE_H
#define IMAGESCALE_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QFile>
#include <QMatrix>
#include <QFileDialog>

class ImageScale : public QWidget
{
    Q_OBJECT

public:
    explicit ImageScale(QPixmap pixmap, QWidget *parent = 0);
    ~ImageScale();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    void onLoadImage(void);
    void onZoomInImage(void);
    void onZoomOutImage(void);
    void onPresetImage(void);
    void onSpinImage();
    void onDownLoadImage();
private:

    QImage m_Image;
    qreal m_ZoomValue = 1.0;
    int m_XPtInterval = 0;
    int m_YPtInterval = 0;
    QPoint m_OldPos;
    bool m_Pressed = false;
    QPixmap m_pixmap;
    QString m_menuStyle;
    size_t  m_ratio;
    //QWidget *m_contentWidget = nullptr;
    //QVBoxLayout *m_mainLayout = nullptr;
};

#endif // IMAGESCALE_H
