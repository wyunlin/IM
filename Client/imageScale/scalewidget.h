#ifndef SCALEWIDGET_H
#define SCALEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPixmap>
#include <QButtonGroup>
#include "imageScale/imagescale.h"


enum IsMaxWidget{No,Yes};
class ScaleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ScaleWidget(QPixmap pixmap, QWidget *parent = nullptr);
    ~ScaleWidget();
signals:
    void emitScaleClose();
public slots:
    void slotBtnGroup(int i);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
private:
    bool			mousePress = false; //按钮点击标志位
    QPoint			movePoint; //鼠标移动
    QWidget         *m_mainWidget = nullptr;
    QVBoxLayout     *m_mainLayout = nullptr;
    QVBoxLayout     *m_contentLayout = nullptr;
    QPushButton     *m_btnMin = nullptr;
    QPushButton     *m_btnMax = nullptr;
    QPushButton     *m_btnClose = nullptr;
    QWidget         *m_topWidget = nullptr;
    //QWidget         *m_imageWidget = nullptr;
    QWidget         *m_bottomWidget = nullptr;
    QHBoxLayout     *m_topLayout = nullptr;
    QHBoxLayout     *m_bottomLayout = nullptr;
    QLabel          *m_spaceLabel = nullptr;
    QPixmap         m_pixmap;
    ImageScale      *m_imageScale = nullptr;

    QLabel          *m_bottomSpace1 = nullptr;
    QPushButton     *m_enlarge = nullptr;
    QPushButton     *m_zoomin = nullptr;
    QPushButton     *m_ratio = nullptr;
    QPushButton     *m_spin = nullptr;
    QPushButton     *m_downLoad = nullptr;
    QButtonGroup    *m_btnGroup = nullptr;
    QLabel          *m_bottomSpace2 = nullptr;
    IsMaxWidget     m_maxWidget = Yes;
};

#endif // SCALEWIDGET_H
