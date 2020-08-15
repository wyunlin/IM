#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include "imageScale/scalewidget.h"
#include "imageScale/imagescale.h"

class InformationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InformationWidget(QPixmap pixmap, QWidget *parent = nullptr);
    ~InformationWidget();
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);
    void slot_Scale();
private:
    QWidget     *m_contentWidget = nullptr;
    QPushButton *m_btnHead = nullptr;
    QLabel      *m_nameLabel = nullptr;
    QVBoxLayout *m_contentLayout = nullptr;
    QWidget     *m_topWidget = nullptr;
    QWidget     *m_bottomWidget = nullptr;
    QGridLayout *m_topLayout = nullptr;
    QPixmap     m_pixmap;
    QPushButton *m_btnSplit = nullptr;
    QVBoxLayout *m_mainLayout = nullptr;

    ScaleWidget *m_scaleWidget = nullptr;
    //ImageScale  *m_imageScale = nullptr;

    QLabel      *m_name = nullptr;
    QLabel      *m_icoLabel = nullptr;
    //bool        m_once = true;
};

#endif // INFORMATIONWIDGET_H
