#ifndef RADIUSPROGRESSBAR_H
#define RADIUSPROGRESSBAR_H

#include <QWidget>
#include <QProgressBar>
#include <QPainter>
#include <QPixmap>

class RadiusProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit RadiusProgressBar(QWidget *parent = nullptr);
    ~RadiusProgressBar();

protected:
    void paintEvent(QPaintEvent *event);
private:
    QPixmap buttomMap;
    QPixmap fillMap;
    QRect rect;
};

#endif // RADIUSPROGRESSBAR_H
