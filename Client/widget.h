#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QList>
#include <QKeyEvent>
#include <QSystemTrayIcon>  //任务栏类
#include <qmenu.h>
#include <QListWidgetItem>
#include "imageScale/imagescale.h"
#include "chatmessage/qnchatmessage.h"
#include "MyScrollBar.h"
#include "mytextedit.h"
#include "Screenshot/screen.h"
#include "Client/Client.h"
#include "informationwidget.h"
#include "chatwidget.h"
#include "myemotionwidget.h"
#include "tuopanmenu.h"
#include "mymenu.h"
#include "closedialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void connectToHost();

    void initWidgetStyle();
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealImage(QNChatMessage *messageW, QListWidgetItem *item, QPixmap pixmap, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void setShadow(QWidget *widget,qreal dx,qreal dy);
public slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadReady();
    void slotShowNormal();
    void demo();
    void slotHeadImage();
    void slotListItemClicked(QListWidgetItem *item);
    void slotStackWidget(int index);
    void slotEmotion();
    void slotEmotionImage(QString fileName);
private slots:
    void on_btnSend_clicked();

    void on_btnImage_clicked();

    void on_closeBtn_clicked();

    void on_shotScreenBtn_clicked();

    void on_minBtn_clicked();

    void on_maxBtn_clicked();

    void btnSendStyle();

    void iconIsActived(QSystemTrayIcon::ActivationReason reason);

    void on_huanfuBtn_clicked();

    void slotshotPixmap();

    void on_fileBtn_clicked();


signals:
    void emitClose();
    void emitPixmap();
    void emitThemeStyle(bool isChange);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *e) override;
    void changeEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void createMenu();
private:
    Ui::Widget *ui;
    bool			mousePress; //按钮点击标志位
    QPoint			movePoint; //鼠标移动
    QTcpSocket      tcpSocket;
    int             imageIndex;
    quint32         sizePackLast;
    Screen          *m_shotScreen = nullptr;
    bool            m_isMaxShow=false;
    QWidget         *m_taskWidget = nullptr;
    QString         m_btnSendStyle;
    bool            m_colorStyle = false;

    //窗口任务栏属性
    QSystemTrayIcon *m_trayIcon = nullptr;
    //QSystemTrayIcon *aademo = nullptr;
    //TuoPanMenu      *m_trayIcon = nullptr;
    //QMenu           *m_trayMenu = nullptr;
    MyMenu          *m_trayMenu = nullptr;
    QAction         *m_quitAction = nullptr;
    QAction         *m_restoreWinAction = nullptr;
    QAction         *m_settingAction = nullptr;
    QAction         *m_helpAction = nullptr;
    QAction         *m_opinionAction = nullptr;
    Client          *m_fileClient = nullptr;

    bool            m_isOnce = false;
    MyScrollBar     *m_scrollBar = nullptr;
    QColor          m_changeColor;
    QStringList     m_imageUrl;
    QPushButton     *m_btnHImg;
    QPixmap         tempHead;
    InformationWidget *viewHeadImg = nullptr;
    QWidget         *m_widget = nullptr;
    ChatWidget      *m_chatWidget_1 = nullptr;
    ChatWidget      *m_chatWidget_2 = nullptr;
    bool            m_isSending = false; // 发送中

    MyEmotionWidget *m_emotionWidget = nullptr;

    //listwidget菜单
    QMenu           *m_listMenu = nullptr;
    QAction         *m_addAction = nullptr;
    QAction         *m_delAction = nullptr;
    QAction         *m_topAction = nullptr;
    MyScrollBar     *m_scrollBarList = nullptr;
    CloseDialog     *m_closeDlg = nullptr;
};

#endif // WIDGET_H
