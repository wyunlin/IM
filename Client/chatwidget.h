#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QThread>
#include "imageScale/imagescale.h"
#include "MyScrollBar.h"
#include "Screenshot/screen.h"
#include "Client/Client.h"
#include "chatmessage/qnchatmessage.h"
#include "imageScale/imagescale.h"
#include "chatsocketthread.h"
#include "myemotionwidget.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QColor color, QWidget *parent = 0);
    ~ChatWidget();


    void connectToHost();
    void initChatThread();
    void initWidgetStyle();
    void dealMessageTime(QString curMsgTime);
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealImage(QNChatMessage *messageW, QListWidgetItem *item, QPixmap pixmap, QString time, QNChatMessage::User_Type type);
public slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadReady();
    void onReturnData(QByteArray dataFull);
    void slotShowNormal();
    void demo();
    void slotThemeStyle(bool isChange);
    void slotEmotion();
    void slotEmotionImage(QString fileName);
private slots:
    void on_m_btnImage_clicked();

    void on_m_shotScreenBtn_clicked();

    void on_m_fileBtn_clicked();

    void on_m_btnSend_clicked();

    void slotshotPixmap();

    void btnSendStyle();

signals:
    void emitSendData(QByteArray dataSend);
    void emitPixmap();
    void emitThemeStyle(bool changeTheme);
protected:
    void changeEvent(QEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
public:
    Ui::ChatWidget *ui;
private:
    QTcpSocket      tcpSocket;
    int             imageIndex;
    quint32         sizePackLast;
    MyScrollBar     *m_scrollBar = nullptr;
    QString         m_btnSendStyle;
    bool            m_colorStyle = false;
    Screen          *m_shotScreen = nullptr;
    bool            m_isOnce = false;
    Client          *m_fileClient = nullptr;
    QColor          m_changeColor;
    QStringList     m_imageUrl;
    QPixmap         tempHead;
    QThread         *m_thread;
    ChatSocketThread *m_chatThread;
    bool            m_isSending = false;  //发送中

    MyEmotionWidget *m_emotionWidget = nullptr;
};

#endif // CHATWIDGET_H
