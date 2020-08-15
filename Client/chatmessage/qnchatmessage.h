#ifndef QNCHATMESSAGE_H
#define QNCHATMESSAGE_H

#include <QWidget>
#include "imageScale/imagewidget.h"
#include "imagelabel.h"

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class QNChatMessage : public QWidget
{
    Q_OBJECT
public:
    explicit QNChatMessage(QColor color , QWidget *parent = nullptr, int type=0);

    enum User_Type{
        User_System,//系统
        User_Me,    //自己
        User_She,   //用户
        User_Time,  //时间
    };
    void setTextSuccess(bool isSending);
    void setText(bool isSending, QString text, QString time, QSize allSize, User_Type userType);

    QSize getRealString(QString src);
    QSize fontRect(QString str);
    void  setPixmap(QPixmap pixmap);
    QPixmap getPixmap();
    void  setImageLabel(QPixmap pixmap, QString time, User_Type userType);

    inline QString text() {return m_msg;}
    inline QString time() {return m_time;}
    inline User_Type userType() {return m_userType;}
public slots:
    void slotThemeStyle(bool isChange);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QString m_msg;
    QString m_time;
    QString m_curTime;
    QColor colme_Kuang;

    QSize m_allSize;
    User_Type m_userType = User_System;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QPixmap m_leftPixmap;
    QPixmap m_rightPixmap;
    QLabel* m_loading = Q_NULLPTR;
    QMovie* m_loadingMovie = Q_NULLPTR;
    bool m_isSending = false;

    QPixmap m_pixmap;
    int     m_type;     //0文本，1图片，2视频
    ImageLabel  *m_imageLabel = nullptr;
    size_t  m_imageX;
    size_t  m_imageY;
};

#endif // QNCHATMESSAGE_H
