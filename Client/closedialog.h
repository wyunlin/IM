#ifndef CLOSEDIALOG_H
#define CLOSEDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <qmath.h>
#include <QMouseEvent>

class CloseDialog : public QWidget
{
    Q_OBJECT
public:
    explicit CloseDialog(QWidget *parent = nullptr);
    ~CloseDialog();
signals:
    void emitStatus(bool isClose);
public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    bool			mousePress = false; //按钮点击标志位
    QPoint			movePoint; //鼠标移动
    QLabel      *m_tipLabel = nullptr;
    QPushButton *m_closeBtn = nullptr;
    QLabel      *m_textLabel = nullptr;
    QRadioButton *m_minBtn = nullptr;
    QRadioButton *m_quitBtn = nullptr;
    QPushButton *m_okBtn = nullptr;
    QWidget     *m_mainWidget = nullptr;
    QHBoxLayout *m_mainLayout = nullptr;
    QWidget     *m_headWidet = nullptr;
    QWidget     *m_contentWidget = nullptr;
    QWidget     *m_footWidget = nullptr;
    QVBoxLayout *m_vLayout = nullptr;
    QHBoxLayout *m_headLayout = nullptr;
    QVBoxLayout *m_contentLayout = nullptr;
    QHBoxLayout *m_footLayout = nullptr;
    bool        m_status = false;
};

#endif // CLOSEDIALOG_H
