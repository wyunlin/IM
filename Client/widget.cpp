#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <qmath.h>
#include <QClipboard>
#include <QMimeData>
#include <QTextDocumentFragment>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextImageFormat>
#include <QPixmap>
#include <QTextCursor>
#include <QFile>
#include <QDateTime>
#include <QTextDocumentFragment>
#include <QSize>
#include <QTextFrameFormat>
#include <QTextFrame>
#include <QCoreApplication>
#include <QDir>
#include <QRegExp>
#include <QListWidgetItem>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
  , imageIndex(0)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowTitle(QString::fromLocal8Bit("W IM"));
    this->setWindowIcon(QIcon(":/Resources/IM.ico"));
    m_changeColor.setRgb(75,164,242);
    m_btnSendStyle = QString("QPushButton{font-size: 12pt;"
                             "background-color: #F5F5F5;"
                             "color: 	#A9A9A9;"
                             "border:1px solid #C0C0C0;}"
                             "QPushButton:hover{background-color:	#1E90FF;"
                             "color:#FFFFFF;"
                             "border:none;"
                             "}"
                             "QPushButton:pressed{background-color:#1E90FF;"
                             "color:#FFFFFF;"
                             "border:none;"
                             "}");
    initWidgetStyle();
    connect(m_btnHImg,&QPushButton::clicked,this,&Widget::slotHeadImage);
    connect(ui->textInput,SIGNAL(emitSend()),ui->btnSend,SIGNAL(clicked()));
    connect(ui->textInput,&MyTextEdit::textChanged,this,&Widget::btnSendStyle);
    //this->grabKeyboard();
    //    //实例阴影shadow
    //    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //    //设置阴影距离
    //    shadow->setOffset(0, 0);
    //    //设置阴影颜色
    //    shadow->setColor(QColor("#444444"));
    //    //设置阴影圆角
    //    shadow->setBlurRadius(30);
    //    //给嵌套QWidget设置阴影
    //    ui->mainWidget->setGraphicsEffect(shadow);
    //    ui->mainLayout->setMargin(5);
    connectToHost();

    createMenu();
    //判断系统是否支持托盘图标显示
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }

    //实例化托盘图标控件
    //aademo = new QSystemTrayIcon(this);
    //m_trayIcon = new TuoPanMenu(this);
    QFile file(":/qss/tuopanmenu.qss");
    file.open(QFile::ReadOnly);
    QString style = tr(file.readAll());
    m_trayMenu->setStyleSheet(style);
    file.close();

    m_trayMenu->setFixedWidth(150);
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/Resources/IM.ico"));
    m_trayIcon->setToolTip("W IM");
    m_trayIcon->showMessage(QString::fromLocal8Bit("托盘"),QString::fromLocal8Bit("托盘管理"),QSystemTrayIcon::Information,10000);
    m_trayIcon->setContextMenu(m_trayMenu); //绑定托盘菜单
    m_trayIcon->show(); //在任务栏显示图标
    connect(m_trayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));

}

void Widget::connectToHost()
{
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadReady()));
    tcpSocket.connectToHost("127.0.0.1",8888);
    sizePackLast=0;
}

void Widget::initWidgetStyle()
{
    m_chatWidget_1 = new ChatWidget(m_changeColor);
    m_chatWidget_2 = new ChatWidget(m_changeColor);
    ui->page_2->setStyleSheet("background-color:#F5F5F5;");
    ui->stackedWidget->addWidget(ui->page_2);
    ui->stackedWidget->addWidget(ui->page);
    ui->stackedWidget->addWidget(m_chatWidget_1);
    ui->stackedWidget->addWidget(m_chatWidget_2);
    connect(this,&Widget::emitThemeStyle,m_chatWidget_1,&ChatWidget::slotThemeStyle);
    connect(this,&Widget::emitThemeStyle,m_chatWidget_2,&ChatWidget::slotThemeStyle);
    //ui->stackedWidget->setCurrentWidget(ui->page);
    const QString emotionText(QString::fromLocal8Bit("表情"));
    ui->emotionBtn->setToolTip(emotionText);
    ui->emotionBtn->setStyleSheet("border-image:url(:/Resources/emotion.png)");
    connect(ui->emotionBtn,&QPushButton::clicked,this,&Widget::slotEmotion);
    const QString imageText(QString::fromLocal8Bit("图片"));
    ui->btnImage->setToolTip(imageText);
    const QString shotScreenText(QString::fromLocal8Bit("截图"));
    ui->shotScreenBtn->setToolTip(shotScreenText);
    const QString fileText(QString::fromLocal8Bit("发送文件"));
    ui->fileBtn->setToolTip(fileText);
    m_taskWidget = new QWidget(this);
    m_taskWidget->setGeometry(9,9,62,612);
    m_taskWidget->setStyleSheet("background-color:#1E90FF;");
    m_btnHImg = new QPushButton(m_taskWidget);
    m_btnHImg->setGeometry(11,20,40,40);
    m_btnHImg->setStyleSheet("border:1px solid #FFFFFF;border-radius:3px;");
    m_btnHImg->setCursor(Qt::PointingHandCursor);

    tempHead.load(":/Resources/timg.jpg");

    QSize sz(40, 40);
    QPixmap tempHead_1 = tempHead.scaled(sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPixmap ico(40, 40);
    ico.fill(Qt::transparent);
    QPainter painter(&ico);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    // 裁剪为圆角
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, 40, 40),3,3);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, 40, 40, tempHead_1);
    m_btnHImg->setText("");
    m_btnHImg->setIcon(ico);
    QSize size(40, 40);
    m_btnHImg->setIconSize(size);

    ui->mainWidget->setStyleSheet("background-color:white");
    //ui->textInput->move(0,30);
    //ui->textMsg->setReadOnly(true);
    ui->listWidget->setStyleSheet("border:none;border-bottom:1px solid #C0C0C0;");
    m_scrollBar = new MyScrollBar;
    ui->listWidget->setVerticalScrollBar(m_scrollBar);
    ui->listWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->toolWidget->setStyleSheet("background-color:white;");
    ui->mainLayout->setContentsMargins(10,10,10,10);
    ui->huanfuBtn->setStyleSheet("border-image:url(:/Resources/theme.png);background-color:transparent");
    ui->minBtn->setStyleSheet("border-image:url(:/Resources/min.png);background-color:transparent");
    ui->maxBtn->setStyleSheet("border-image:url(:/Resources/max.png);background-color:transparent");
    ui->closeBtn->setStyleSheet("border-image:url(:/Resources/close.png);background-color:transparent");
    ui->btnImage->setStyleSheet("border-image:url(:/Resources/tu_pian.png);background-color:transparent");
    ui->shotScreenBtn->setStyleSheet("border-image:url(:/Resources/jietu.png);background-color:transparent");
    //ui->textMsg->setStyleSheet("border:none;border-bottom:1px solid #C0C0C0;");
    ui->textInput->setStyleSheet("border:none;");
    ui->listView->setIconSize(QSize(40,40));
    ui->listView->installEventFilter(this);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollBarList = new MyScrollBar;
    ui->listView->setVerticalScrollBar(m_scrollBarList);
    ui->listView->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    m_listMenu = new QMenu;
    m_topAction = new QAction(QString::fromLocal8Bit("置顶"),this);
    m_addAction = new QAction(QString::fromLocal8Bit("添加"), this);
    m_delAction = new QAction(QString::fromLocal8Bit("删除"), this);
    m_listMenu->addAction(m_topAction);
    m_listMenu->addAction(m_addAction);
    m_listMenu->addAction(m_delAction);
    QFile filemenu(":/qss/menu.qss");
    filemenu.open(QFile::ReadOnly);
    QString styleSheet = tr(filemenu.readAll());
    m_listMenu->setStyleSheet(styleSheet);
    connect(m_topAction,&QAction::triggered,[=]
    {
       QListWidgetItem *item = ui->listView->currentItem();
       ui->listWidget->insertItem(0,item);
    });
    connect(m_addAction,&QAction::triggered,[=]
    {
        QListWidgetItem *Item = new QListWidgetItem();
        Item->setIcon(ico);
        Item->setSizeHint(QSize(180,60));
        Item->setText(QString("demo%1").arg(ui->listView->count()+1));
        ui->listView->insertItem(0,Item);
    });
    connect(m_delAction,&QAction::triggered,[=]
    {
       QListWidgetItem *item = ui->listView->currentItem();
       ui->listView->removeItemWidget(item);
       delete item;
       item = nullptr;
    });

    QListWidgetItem *listItem = new QListWidgetItem();
    listItem->setIcon(ico);
    listItem->setSizeHint(QSize(180,60));
    listItem->setText(QString("demo1"));
    ui->listView->insertItem(0,listItem);

    QListWidgetItem *listItem2 = new QListWidgetItem();
    listItem2->setIcon(ico);
    listItem2->setSizeHint(QSize(180,60));
    listItem2->setText(QString("demo2"));
    //ui->listView->addItem(listItem2);
    ui->listView->insertItem(0,listItem2);

    QListWidgetItem *listItem3 = new QListWidgetItem();
    listItem3->setIcon(ico);
    listItem3->setSizeHint(QSize(180,60));
    listItem3->setText(QString("demo3"));
    //ui->listView->addItem(listItem3);
    ui->listView->insertItem(0,listItem3);

    ui->listView->setStyleSheet("border:none;border-top:1px solid #C0C0C0;border-right:1px solid #C0C0C0;");
    connect(ui->listView,&QListWidget::itemClicked,this,&Widget::slotListItemClicked);
    connect(ui->listView,&QListWidget::currentRowChanged,this,&Widget::slotStackWidget);
    //    ui->listView->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
    //                               "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
    //                               "QListWidget::Item:hover{background:skyblue; }"
    //                               "QListWidget::item:selected{background:lightgray; color:red; }"
    //                               "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
    //                               );
    ui->btnSend->setStyleSheet(m_btnSendStyle);
    ui->fileBtn->setStyleSheet("border-image:url(:/Resources/file.png);background-color:transparent;");
    ui->textInput->installEventFilter(this);
}

void Widget::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(631);
    qDebug()<<"text"<<text;
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(m_isSending, text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void Widget::dealImage(QNChatMessage *messageW, QListWidgetItem *item, QPixmap pixmap, QString time, QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(631);
    item->setSizeHint(QSize(631,200));
    messageW->setImageLabel(pixmap,time,type);
    ui->listWidget->setItemWidget(item,messageW);
}

void Widget::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
        //        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(631, 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(m_isSending, curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void Widget::onConnected()
{
    qDebug()<<"connected";
    m_isSending = true;
}

void Widget::onDisconnected()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    socket->deleteLater();
    qDebug()<<"disconnectioned";
    m_isSending = false;
}

void Widget::onError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"error"<<socketError;
}

void Widget::onReadReady()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    //当前缓冲区里面数据的大小，收到数据大小
    qint64 sizeNow=0;
    do{
        sizeNow=socket->bytesAvailable();
        QDataStream stream(socket);
        if(sizePackLast==0)
        {
            if(sizeNow<sizeof(quint32))
                return;
            stream>>sizePackLast;
        }
        //包不完整
        if(sizeNow < sizePackLast-4)
        {
            return;
        }
        qDebug()<<"full pack";
        QByteArray dataFull;
        stream>>dataFull;
        sizePackLast=0;
        //判断剩下的字节数，是否有粘包的情况
        sizeNow=socket->bytesAvailable();

        QString datafull = QString::fromUtf8(dataFull);
        qDebug()<<"dataFull"<<datafull;
        QString prefix=QString(dataFull).mid(0,4);
        if(prefix=="TXT:")
        {
            //ui->textMsg->append("<p>"+datafull.mid(4)+"</p>");
            //ui->textMsg->append(dataFull.mid(4));
            QString msg = dataFull.mid(4);
            QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
        }
        else if(prefix=="IMG:")
        {
            QDateTime time = QDateTime::currentDateTime();   //获取当前时间
            int timeT = time.toTime_t();   //将当前时间转为时间戳
            QString filename = QString("./tempImage/%1.png").arg(QString::number(timeT));
            //QString filename = QString("%1.png").arg(QString::number(timeT));
            QString htmlTag=QString("<img src=\"%1\"></img>");
            //QString index=QString::number(imageIndex);
            htmlTag=htmlTag.arg(filename);
            QFile file(filename);
            file.open(QIODevice::WriteOnly);
            file.write(dataFull.mid(4));
            file.close();
            imageIndex+=1;

            //            QDir dir;
            //            QString pathname;
            //            //修改相对路径为exe目录路径
            //            QDir::setCurrent(QCoreApplication::applicationDirPath());
            //            pathname = dir.currentPath();
            //            qDebug()<<pathname;
            QString time_t = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            dealMessageTime(time_t);
            QPixmap pixmap;
            pixmap.loadFromData(dataFull.mid(4));
            qDebug()<<"pixmap"<<pixmap;
            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget(),1);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealImage(messageW,item,pixmap,time_t,QNChatMessage::User_She);
            //dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
            //ui->textMsg->append(htmlTag);
        }
    }while(sizeNow>0);
}

void Widget::slotShowNormal()
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->show();
}

void Widget::demo()
{
    this->close();
}

void Widget::slotHeadImage()
{
    //    ImageScale *viewHeadImg = new ImageScale(tempHead);
    //    viewHeadImg->show();
    QPoint clickedPoint = this->cursor().pos();
    //    QPoint windowPoint = mapFromGlobal(clickedPoint);
    //    qDebug()<<"windowPoint"<<windowPoint;
    viewHeadImg = new InformationWidget(tempHead);
    viewHeadImg->move(clickedPoint);
    viewHeadImg->show();
}

void Widget::slotListItemClicked(QListWidgetItem *item)
{
    QString str = item->text();
    ui->linkName->setText(str);
}

void Widget::slotStackWidget(int index)
{
    ui->stackedWidget->setCurrentIndex(index+1);
}

void Widget::slotEmotion()
{
    QPoint clickedPoint = this->cursor().pos();
    m_emotionWidget = new MyEmotionWidget;
    clickedPoint.setY(clickedPoint.y()-200);
    m_emotionWidget->move(clickedPoint);
    m_emotionWidget->show();
    connect(m_emotionWidget,&MyEmotionWidget::emitEmotionPath,this,&Widget::slotEmotionImage);
}

void Widget::slotEmotionImage(QString fileName)
{
    //    QPixmap pixmap(fileName);
    //    //QSize picSize(150,90);
    //    //QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //    QImage Image_1 = pixmap.toImage();
    //    Image_1.save(filename,"png");
    //    file_1.close();
    QTextDocumentFragment fragment;
    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(fileName));
    ui->textInput->textCursor().insertFragment(fragment);
    ui->textInput->setVisible(true);
    qDebug()<<"textInput"<<ui->textInput->toHtml();
}


void Widget::on_btnSend_clicked()
{
    QString textInput = ui->textInput->toPlainText();

    QString textHtml = ui->textInput->toHtml();
    qDebug()<<"textInput"<<textInput.size();
    if(textInput.size()!=0)
    {
        QString msgInput ="TXT:" + ui->textInput->toPlainText();

        QString msg = ui->textInput->toPlainText();

        //封装数据包、包头
        QByteArray dataSend;
        QDataStream stream(&dataSend,QIODevice::WriteOnly);
        stream<<(quint32)0<<msgInput.toUtf8();
        stream.device()->seek(0);
        stream<<dataSend.size();

        tcpSocket.write(dataSend);
        //ui->textInput->clear();
        if(!m_colorStyle)
        {
            QFile file(":/qss/blue.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->btnSend->setStyleSheet(styleSheet);
            file.close();
        }
        else
        {
            QFile file(":/qss/orange.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->btnSend->setStyleSheet(styleSheet);
            file.close();
        }

        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

        bool isSending = true; // 发送中

        qDebug()<<"addMessage" << msg << time << ui->listWidget->count();
        qDebug()<<"m_isSending"<<m_isSending;
        //qDebug()<<"issendding"<<isSending;
        //if(!ui->listWidget->count()%2) {
        if(isSending) {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            connect(this,&Widget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
        } else {
            bool isOver = true;
            for(int i = ui->listWidget->count() - 1; i > 0; i--) {
                QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
                connect(this,&Widget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess(m_isSending);
                }
            }
            if(isOver) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                connect(this,&Widget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
                messageW->setTextSuccess(m_isSending);
            }
        }
    }

    qDebug()<<"textHtml"<<textHtml;
    QRegExp reg("<img src=\"(.*)\" />");
    int pos = textHtml.indexOf(reg);
    if(pos>=0)
    {
        qDebug()<<"pos"<<pos;
        qDebug()<<"reg.matchedLength()"<<reg.matchedLength();
        qDebug()<<reg.cap(0);
        qDebug()<<reg.cap(1);
        //        QString image = reg.cap(1);
        //        image = image.remove(image.size()-6,2);
        //        qDebug()<<"image"<<image;
        //        QFile file(image);
        //        file.open(QIODevice::ReadOnly);
        //        QByteArray data="IMG:" + file.readAll();
        //        QPixmap pixmap;
        //        pixmap.loadFromData(data.mid(4));
        //        file.close();
        QString image = reg.cap(1);
        QPixmap pixmap;
        QByteArray data;
        QRegExp emotionExp(":/Resources/(.*).png");
        int emotionPos = image.indexOf(emotionExp);
        if(emotionPos>=0)
        {
            QFile file(image);
            file.open(QIODevice::ReadOnly);
            data="IMG:" + file.readAll();
            pixmap.loadFromData(data.mid(4));
            file.close();
        }
        else
        {
            image = image.remove(image.size()-6,2);
            QFile file(image);
            file.open(QIODevice::ReadOnly);
            data="IMG:" + file.readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data.mid(4));
            file.close();
        }
        //        QDateTime time = QDateTime::currentDateTime();   //获取当前时间
        //        int timeT = time.toTime_t();   //将当前时间转为时间戳
        //        QString filename = QString("./tempImage/%1.png").arg(QString::number(timeT));
        //        QFile file_1(filename);
        //        bool is_create_1 = file_1.open(QFile::WriteOnly | QFile::Truncate);
        //        if (!is_create_1)
        //        {
        //            qDebug() << QStringLiteral("打开文件失败");
        //            return;
        //        }

        QString time_t = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
        dealMessageTime(time_t);
        qDebug()<<"pixmap"<<pixmap;
        QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget(),1);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealImage(messageW,item,pixmap,time_t,QNChatMessage::User_Me);

        //        QSize picSize(150,90);
        //        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //        QImage Image_1 = scaledPixmap.toImage();
        //        Image_1.save(filename,"png");
        //        file_1.close();
        //        QTextDocumentFragment fragment;
        //        fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(filename));
        //        ui->textInput->textCursor().insertFragment(fragment);
        //        ui->textInput->setVisible(true);
        //        qDebug()<<"textInput"<<ui->textInput->toHtml();

        //封装数据包、包头
        QByteArray dataSend;
        QDataStream stream(&dataSend,QIODevice::WriteOnly);
        stream<<(quint32)0<<data;
        stream.device()->seek(0);
        stream<<dataSend.size();

        tcpSocket.write(dataSend);
    }
    ui->textInput->clear();
    //    for(int pos=0;pos=textHtml.indexOf(reg); pos+=reg.matchedLength())
    //    {
    //       qDebug()<<pos;       //返回查找到序号
    //       qDebug()<<reg.cap(0);//返回匹配到的字串
    //    }
    //}
    /*else {
        if(msg != "") {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
        }
    }*/
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void Widget::on_btnImage_clicked()
{
    QString image=QFileDialog::getOpenFileName(this,"title",".","Image File (*.png *.jpg *.bmp");
    qDebug()<<"image"<<image;
    if(image.isEmpty())
        return;
    QFile file(image);
    file.open(QIODevice::ReadOnly);
    QByteArray data="IMG:" + file.readAll();
    QPixmap pixmap;
    pixmap.loadFromData(data.mid(4));
    file.close();


    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    QString fileImage = QString("./tempImage/%1.png").arg(QString::number(timeT));
    QFile fileI(fileImage);
    if(!fileI.open(QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << QStringLiteral("打开文件失败");
        return;
    }
    QImage yuanImage = pixmap.toImage();
    yuanImage.save(fileImage,"png");
    fileI.close();

    QString filename = QString("./tempImage/%1_1.png").arg(QString::number(timeT));
    QFile file_1(filename);
    bool is_create_1 = file_1.open(QFile::WriteOnly | QFile::Truncate);
    if (!is_create_1)
    {
        qDebug() << QStringLiteral("打开文件失败");
        return;
    }

    //    QString time_t = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
    //    dealMessageTime(time_t);
    //    qDebug()<<"pixmap"<<pixmap;
    //    QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->listWidget->parentWidget(),1);
    //    QListWidgetItem* item = new QList WidgetItem(ui->listWidget);
    //    dealImage(messageW,item,pixmap,time_t,QNChatMessage::User_Me);

    QSize picSize(150,90);
    QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage Image_1 = scaledPixmap.toImage();
    Image_1.save(filename,"png");
    file_1.close();
    QTextDocumentFragment fragment;
    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(filename));
    ui->textInput->textCursor().insertFragment(fragment);
    ui->textInput->setVisible(true);
    qDebug()<<"textInput"<<ui->textInput->toHtml();

    //    //封装数据包、包头
    //    QByteArray dataSend;
    //    QDataStream stream(&dataSend,QIODevice::WriteOnly);
    //    stream<<(quint32)0<<data;
    //    stream.device()->seek(0);
    //    stream<<dataSend.size();

    //    tcpSocket.write(dataSend);

}

void Widget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    movePoint = event->globalPos() - this->pos();
    //qDebug()<<"this->pos"<<this->pos();
}

void Widget::mouseReleaseEvent(QMouseEvent * event)
{
    mousePress = false;
}

void Widget::mouseMoveEvent(QMouseEvent * event)
{
    if (mousePress)
    {
        QPoint movePos = event->globalPos();
        this->move(movePos - movePoint);
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    //    if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_V) && ui->textInput->hasFocus())
    //    {
    //        const QClipboard *clipboard = QApplication::clipboard();
    //        const QMimeData *mimeData = clipboard->mimeData();
    //        if(mimeData->hasImage())
    //        {
    //            QPixmap image = qvariant_cast<QPixmap>(mimeData->imageData());
    //            //QTextDocument * textDocument = ui->textInput->document();
    //            //textDocument->addResource( QTextDocument::ImageResource, QVariant ( image ) );
    //            QTextCursor cursor = ui->textInput->textCursor();
    //            QTextImageFormat imageFormat;
    //            imageFormat.setWidth( image.width() );
    //            imageFormat.setHeight( image.height() );
    //            QLabel *imageLabel = new QLabel;

    //            //imageFormat.setName( Uri.toString() );
    //            cursor.insertImage(imageFormat);
    //        }
    //        else if(mimeData->hasHtml())
    //        {
    //            ui->textInput->setText(mimeData->html());
    //        }

    //        return;
    //    }
    //    if ((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && ui->textInput->hasFocus())
    //    {
    //        on_btnSend_clicked();
    //    }

    QWidget::keyPressEvent(event);
}

bool Widget::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == ui->textInput)
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent *event = static_cast<QKeyEvent*>(e);
            if((event->key() == Qt::Key_Enter && (event->modifiers() == Qt::ControlModifier)) ||(event->key() == Qt::Key_Return && (event->modifiers() == Qt::ControlModifier)))
            {
                ui->textInput->append("");
                return true;
            }
            else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
            {
                QTextCursor cursor = ui->textInput->textCursor();
                cursor.clearSelection();
                //cursor.deletePreviousChar();
                if(ui->textInput->toPlainText()!="")
                {
                    qDebug()<<"ui->textInput->toPlainText()"<<ui->textInput->toPlainText();
                    on_btnSend_clicked();
                }
                return true;
            }
            else if((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_V))
            {
                const QClipboard *clipboard = QApplication::clipboard();
                const QMimeData *mimeData = clipboard->mimeData();
                if(mimeData->hasImage())
                {
                    QPixmap image = qvariant_cast<QPixmap>(mimeData->imageData());
                    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
                    int timeT = time.toTime_t();   //将当前时间转为时间戳
                    QString filename = QString("./tempImage/%1.png").arg(QString::number(timeT));
                    QString filename_1 = QString("./tempImage/%1_1.png").arg(QString::number(timeT));
                    //qDebug()<<"filename"<<filename;
                    QFile file(filename);
                    bool is_create = file.open(QFile::WriteOnly | QFile::Truncate);
                    if (!is_create)
                    {
                        qDebug() << QStringLiteral("打开文件失败");
                        return false;
                    }
                    QImage Image = image.toImage();
                    Image.save(filename,"png");
                    file.close();

                    QFile file_1(filename_1);
                    bool is_create_1 = file_1.open(QFile::WriteOnly | QFile::Truncate);
                    if (!is_create_1)
                    {
                        qDebug() << QStringLiteral("打开文件失败");
                        return false;
                    }
                    QSize picSize(150,90);
                    QPixmap scaledPixmap = image.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    QImage Image_1 = scaledPixmap.toImage();
                    Image_1.save(filename_1,"png");
                    file_1.close();

                    QTextDocumentFragment fragment;
                    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(filename_1));
                    ui->textInput->textCursor().insertFragment(fragment);
                    ui->textInput->setVisible(true);
                    return true;
                }
                else if(mimeData->hasHtml())
                {
                    ui->textInput->setText(mimeData->html());
                }
            }
        }
    }
    else if(obj == ui->listView)
    {
        if (e->type() == QEvent::ContextMenu && (ui->listView->itemAt(mapFromGlobal(QCursor::pos())) != NULL))
        {
            m_topAction->setEnabled(true);
            m_delAction->setEnabled(true);
            m_listMenu->exec(QCursor::pos());
            return true;
        }
        else if(e->type() == QEvent::ContextMenu && (ui->listView->itemAt(mapFromGlobal(QCursor::pos())) == NULL))
        {
            m_listMenu->exec(QCursor::pos());
            m_topAction->setEnabled(false);
            m_delAction->setEnabled(false);
            return true;
        }
    }
        return false;
}

void Widget::changeEvent(QEvent *event)
{
    //    if((event->type()==QEvent::WindowStateChange) && this->isMinimized())
    //    {
    //        this->hide();
    //    }
    QWidget::changeEvent(event);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


    //ui->textEdit->resize(this->width() - 20, ui->widget->height() - 20);
    //ui->textEdit->move(10, 10);

    //ui->pushButton->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() - 10,
    //ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() - 10);


    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}

void Widget::createMenu()
{
    m_restoreWinAction = new QAction(QString::fromLocal8Bit("打开IM(&R)"),this);
    m_settingAction = new QAction(QString::fromLocal8Bit("设置"),this);
    m_quitAction = new QAction(QString::fromLocal8Bit("退出(&Q)"),this);
    m_helpAction = new QAction(QString::fromLocal8Bit("帮助"),this);
    m_opinionAction = new QAction(QString::fromLocal8Bit("意见"),this);

    //恢复窗口
    connect(m_restoreWinAction,&QAction::triggered,this,&Widget::slotShowNormal);

    //退出程序
    connect(m_quitAction,&QAction::triggered,[=]{
        emit emitClose();
    });

    //添加菜单
    m_trayMenu = new MyMenu((QWidget*)QApplication::desktop());
    setShadow(m_trayMenu,5,5);
    m_trayMenu->addAction(m_restoreWinAction);
    m_trayMenu->addAction(m_settingAction);
    QMenu *childMenu = new QMenu;
    QFile file(":/qss/tuopanmenu.qss");
    if(file.open(QIODevice::ReadOnly))
    {
        QString styleSheet = file.readAll();
        childMenu->setStyleSheet(styleSheet);
    }
    file.close();
    QAction *demoA1 = new QAction(QString::fromLocal8Bit("child_1"));
    QAction *demoA2 = new QAction(QString::fromLocal8Bit("child_2"));
    childMenu->addAction(demoA1);
    childMenu->addAction(demoA2);
    m_settingAction->setMenu(childMenu);
    m_trayMenu->addMenu(childMenu);
    m_trayMenu->addAction(m_helpAction);
    m_trayMenu->addAction(m_opinionAction);

    //添加分隔符
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_quitAction);
}


void Widget::on_closeBtn_clicked()
{
    if(!m_closeDlg)
    {
        m_closeDlg = new CloseDialog(this);
    }
    m_closeDlg->move(this->width()/2-m_closeDlg->width()/2,this->height()/2-m_closeDlg->height()/2);
    m_closeDlg->show();
    connect(m_closeDlg,&CloseDialog::emitStatus,[=](bool staus){
        if(staus)
        {
            emit emitClose();
            m_closeDlg->close();
        }
        else
        {
            m_closeDlg->hide();
            this->hide();
            this->setWindowFlag(Qt::Tool);
        }
    });
}

void Widget::on_shotScreenBtn_clicked()
{
    m_shotScreen = new Screen;
    m_shotScreen->show();
    connect(m_shotScreen,&Screen::emitPixmap,this,&Widget::slotshotPixmap);
}

void Widget::on_minBtn_clicked()
{
    this->showMinimized();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width() - 20, this->height() - 20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(163, 188, 228, 50);
    for (int i = 0; i < 10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
        color.setAlpha(150 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QPainter painter2(this);
    painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter2.setBrush(QBrush(Qt::white));
    painter2.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setLeft(9);
    rect.setTop(9);
    rect.setWidth(rect.width() - 9);
    rect.setHeight(rect.height() - 9);
    painter2.drawRoundedRect(rect, 4, 4);
    QWidget::paintEvent(event);
}

void Widget::on_maxBtn_clicked()
{
    if(!m_isMaxShow)
    {
        this->showMaximized();
        m_isMaxShow = !m_isMaxShow;
    }
    else
    {
        this->showNormal();
        m_isMaxShow = !m_isMaxShow;
    }
}

void Widget::btnSendStyle()
{
    if(ui->textInput->toPlainText()!="")
    {
        if(!m_colorStyle)
        {
            ui->btnSend->setStyleSheet("font-size: 12pt;"
                                       "background-color: rgb(30,144,255);"
                                       "color: rgb(255, 255, 255);"
                                       "border:none;");
        }
        else
        {
            ui->btnSend->setStyleSheet("font-size: 12pt;"
                                       "background-color: #ff5500;"
                                       "color: rgb(255, 255, 255);"
                                       "border:none;");
        }

    }
    else
    {
        if(!m_colorStyle)
        {
            QFile file(":/qss/blue.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->btnSend->setStyleSheet(styleSheet);
            file.close();
        }
        else
        {
            QFile file(":/qss/orange.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->btnSend->setStyleSheet(styleSheet);
            file.close();
        }
    }
}

void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
    {
        slotShowNormal();
        break;
    }
    case QSystemTrayIcon::DoubleClick:
    {
        slotShowNormal();
        break;
    }
    case QSystemTrayIcon::Context:
    {
        QPoint p = QCursor::pos();
        m_trayMenu->move(p-QPoint(m_trayMenu->width()+5,m_trayMenu->height()+5));
    }
    default:
        break;
    }

}

void Widget::on_huanfuBtn_clicked()
{
    if(!m_colorStyle)
    {
        QFile file(":/qss/orange.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->widget_3->setStyleSheet(styleSheet);
        m_taskWidget->setStyleSheet(styleSheet);
        ui->btnSend->setStyleSheet(styleSheet);
        m_colorStyle = !m_colorStyle;
        m_changeColor.setRgb(255,85,0);
        file.close();
    }
    else
    {
        QFile file(":/qss/blue.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->widget_3->setStyleSheet(styleSheet);
        m_taskWidget->setStyleSheet(styleSheet);
        ui->btnSend->setStyleSheet(styleSheet);
        m_colorStyle = !m_colorStyle;
        m_changeColor.setRgb(75,164,242);
        file.close();
    }
    emit emitThemeStyle(m_colorStyle);
}

void Widget::slotshotPixmap()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QPixmap image = qvariant_cast<QPixmap>(mimeData->imageData());
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    QString filename = QString("./tempImage/%1.png").arg(QString::number(timeT));
    QString filename_1 = QString("./tempImage/%1_1.png").arg(QString::number(timeT));
    //qDebug()<<"filename"<<filename;
    QFile file(filename);
    bool is_create = file.open(QFile::WriteOnly | QFile::Truncate);
    if (!is_create)
    {
        qDebug() << QStringLiteral("打开文件失败");
        return;
    }
    QImage Image = image.toImage();
    Image.save(filename,"png");
    file.close();

    QFile file_1(filename_1);
    bool is_create_1 = file_1.open(QFile::WriteOnly | QFile::Truncate);
    if (!is_create_1)
    {
        qDebug() << QStringLiteral("打开文件失败");
        return;
    }
    QSize picSize(150,90);
    QPixmap scaledPixmap = image.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QImage Image_1 = scaledPixmap.toImage();
    Image_1.save(filename_1,"png");
    file_1.close();

    QTextDocumentFragment fragment;
    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(filename_1));
    ui->textInput->textCursor().insertFragment(fragment);
    ui->textInput->setVisible(true);

    //    QTextDocument *doc = ui->textInput->document();
    //    QTextFrame *rootFrame = doc->rootFrame();
    //    QTextFrameFormat frameFormat;
    //    frameFormat.setBackground(Qt::lightGray);
    //    frameFormat.setWidth(150);
    //    frameFormat.setHeight(90);
    //    frameFormat.setMargin(10);
    //    frameFormat.setPadding(5);
    //    frameFormat.setBorder(2);
    //    frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
    //    QTextCursor cursor = ui->textInput->textCursor();
    //    //rootFrame->setFrameFormat(frameFormat);
    //    cursor.insertFrame(frameFormat);
    //    QTextDocument *doc = ui->textInput->document(); //获取文档对象
    //    QTextFrame *rootFrame = doc->rootFrame();   //获取根框架
    //    QTextFrameFormat frameFormat;           //创建框架格式
    //    frameFormat.setBorderBrush(Qt::red);    //设置边界颜色
    //    frameFormat.setBorder(5);       //设置边界宽度
    //    rootFrame->setFrameFormat(frameFormat); //给框架使用格式
}

void Widget::on_fileBtn_clicked()
{
    if(!m_isOnce)
    {

        m_fileClient = new Client(m_colorStyle);

        m_fileClient->show();
        m_isOnce = true;
        connect(this,&Widget::emitThemeStyle,m_fileClient,&Client::themeStyle);
        connect(m_fileClient,&Client::emitClose,[this]{
            m_isOnce = false;
        });
    }
    else
    {
        m_fileClient->activateWindow();
    }
}

//控件阴影
void Widget::setShadow(QWidget *widget,qreal dx,qreal dy)
{
    QGraphicsDropShadowEffect *effert = new QGraphicsDropShadowEffect(widget);
    effert->setOffset(dx,dy);
    widget->setGraphicsEffect(effert);

}

Widget::~Widget()
{
    delete m_chatWidget_1;
    m_chatWidget_1 = nullptr;
    delete m_chatWidget_2;
    m_chatWidget_2 = nullptr;
    delete m_scrollBar;
    m_scrollBar = nullptr;
    delete ui;
}
