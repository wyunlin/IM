#include "chatwidget.h"
#include "ui_chatwidget.h"
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

ChatWidget::ChatWidget(QColor color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    //m_changeColor.setRgb(75,164,242);
    m_changeColor = color;
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
    initChatThread();
    initWidgetStyle();
    connect(ui->m_textInput,SIGNAL(emitSend()),ui->m_btnSend,SIGNAL(clicked()));
    connect(ui->m_textInput,&MyTextEdit::textChanged,this,&ChatWidget::btnSendStyle);
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

}

void ChatWidget::connectToHost()
{
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadReady()));
    tcpSocket.connectToHost("127.0.0.1",8888);
    sizePackLast=0;
}

void ChatWidget::initChatThread()
{
    m_thread = new QThread;
    m_chatThread = new ChatSocketThread;
    m_chatThread->moveToThread(m_thread);
    //m_thread->start();
    connect(this,&ChatWidget::emitSendData,m_chatThread,&ChatSocketThread::sendData,Qt::QueuedConnection);
    connect(m_chatThread,&ChatSocketThread::emitReturnData,this,&ChatWidget::onReturnData);
    connect(m_thread, &QThread::finished, m_chatThread, &QObject::deleteLater,Qt::QueuedConnection);
    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
}

void ChatWidget::initWidgetStyle()
{
    const QString emotionText(QString::fromLocal8Bit("表情"));
    ui->m_emotionBtn->setToolTip(emotionText);
    ui->m_emotionBtn->setStyleSheet("border-image:url(:/Resources/emotion.png)");
    connect(ui->m_emotionBtn,&QPushButton::clicked,this,&ChatWidget::slotEmotion);
    //ui->stackedWidget->setCurrentWidget(ui->page);
    const QString imageText(QString::fromLocal8Bit("图片"));
    ui->m_btnImage->setToolTip(imageText);
    const QString shotScreenText(QString::fromLocal8Bit("截图"));
    ui->m_shotScreenBtn->setToolTip(shotScreenText);
    //const QString fileText(QString::fromLocal8Bit("发送文件"));
    //ui->m_fileBtn->setToolTip(fileText);

    //ui->textInput->move(0,30);
    //ui->textMsg->setReadOnly(true);
    ui->m_listWidget->setStyleSheet("border:none;border-bottom:1px solid #C0C0C0;");
    m_scrollBar = new MyScrollBar;
    ui->m_listWidget->setVerticalScrollBar(m_scrollBar);
    ui->m_listWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->m_toolWidget->setStyleSheet("background-color:white;");
    ui->m_btnImage->setStyleSheet("border-image:url(:/Resources/tu_pian.png);background-color:transparent");
    ui->m_shotScreenBtn->setStyleSheet("border-image:url(:/Resources/jietu.png);background-color:transparent");
    //ui->textMsg->setStyleSheet("border:none;border-bottom:1px solid #C0C0C0;");
    ui->m_textInput->setStyleSheet("border:none;");
//    ui->listView->setStyleSheet("QListWidget{border:1px solid gray; color:black; }"
//                               "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
//                               "QListWidget::Item:hover{background:skyblue; }"
//                               "QListWidget::item:selected{background:lightgray; color:red; }"
//                               "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
//                               );
    ui->m_btnSend->setStyleSheet(m_btnSendStyle);
    ui->m_fileBtn->setStyleSheet("border-image:url(:/Resources/file.png);background-color:transparent;");
    ui->m_textInput->installEventFilter(this);
}

void ChatWidget::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(631);
    qDebug()<<"text"<<text;
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(m_isSending, text, time, size, type);
    ui->m_listWidget->setItemWidget(item, messageW);
}

void ChatWidget::dealImage(QNChatMessage *messageW, QListWidgetItem *item, QPixmap pixmap, QString time, QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(631);
    item->setSizeHint(QSize(631,200));
    messageW->setImageLabel(pixmap,time,type);
    ui->m_listWidget->setItemWidget(item,messageW);
}

void ChatWidget::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->m_listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->m_listWidget->item(ui->m_listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->m_listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
        //        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->m_listWidget);

        QSize size = QSize(631, 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(m_isSending, curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->m_listWidget->setItemWidget(itemTime, messageTime);
    }
}

void ChatWidget::onConnected()
{
    qDebug()<<"connected";
    m_isSending = true;
}

void ChatWidget::onDisconnected()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    socket->deleteLater();
    qDebug()<<"disconnectioned";
    m_isSending = false;
}

void ChatWidget::onError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"error"<<socketError;
}

void ChatWidget::onReadReady()
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

            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
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
            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget(),1);
            QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
            dealImage(messageW,item,pixmap,time_t,QNChatMessage::User_She);
            //dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
            //ui->textMsg->append(htmlTag);
        }
    }while(sizeNow>0);
}

void ChatWidget::onReturnData(QByteArray dataFull)
{
    //QString datafull = QString::fromUtf8(dataFull);
    //qDebug()<<"dataFull"<<datafull;
    QString prefix=QString(dataFull).mid(0,4);
    if(prefix=="TXT:")
    {
        //ui->textMsg->append("<p>"+datafull.mid(4)+"</p>");
        //ui->textMsg->append(dataFull.mid(4));
        QString msg = dataFull.mid(4);
        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
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
        QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget(),1);
        QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
        dealImage(messageW,item,pixmap,time_t,QNChatMessage::User_She);
        //dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
        //ui->textMsg->append(htmlTag);
    }
}

void ChatWidget::slotShowNormal()
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    this->show();
}

void ChatWidget::demo()
{
    this->close();
}

void ChatWidget::slotThemeStyle(bool isChange)
{
    emit emitThemeStyle(isChange);
    if(isChange)
    {
        QFile file(":/qss/orange.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->m_btnSend->setStyleSheet(styleSheet);
        m_changeColor.setRgb(255,85,0);
        m_colorStyle = !m_colorStyle;
        file.close();
    }
    else
    {
        QFile file(":/qss/blue.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->m_btnSend->setStyleSheet(styleSheet);
        m_changeColor.setRgb(75,164,242);
        m_colorStyle = !m_colorStyle;
        file.close();
    }
}

void ChatWidget::slotEmotion()
{
    QPoint clickedPoint = this->cursor().pos();
    m_emotionWidget = new MyEmotionWidget;
    clickedPoint.setY(clickedPoint.y()-200);
    m_emotionWidget->move(clickedPoint);
    m_emotionWidget->show();
    connect(m_emotionWidget,&MyEmotionWidget::emitEmotionPath,this,&ChatWidget::slotEmotionImage);
}

void ChatWidget::slotEmotionImage(QString fileName)
{
//    QPixmap pixmap(fileName);
//    //QSize picSize(150,90);
//    //QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    QImage Image_1 = pixmap.toImage();
//    Image_1.save(filename,"png");
//    file_1.close();
    QTextDocumentFragment fragment;
    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(fileName));
    ui->m_textInput->textCursor().insertFragment(fragment);
    ui->m_textInput->setVisible(true);
    qDebug()<<"textInput"<<ui->m_textInput->toHtml();
}

void ChatWidget::on_m_btnSend_clicked()
{
    QString textInput = ui->m_textInput->toPlainText();
    int buf_OUT[65];
    QChar t;
    for(int i = 0;i < textInput.length();++i){
        t = textInput.at(i);
        qDebug()<<"t"<<t.unicode();
        buf_OUT[i] = t.toLatin1()-'0';
    }
    QString textHtml = ui->m_textInput->toHtml();
    qDebug()<<"textInput"<<textInput;
    if(textInput.size()!=0 && t.unicode()!=65532)
    {
        QString msgInput ="TXT:" + ui->m_textInput->toPlainText();

        QString msg = ui->m_textInput->toPlainText();

        //封装数据包、包头
        QByteArray dataSend;
        QDataStream stream(&dataSend,QIODevice::WriteOnly);
        stream<<(quint32)0<<msgInput.toUtf8();
        stream.device()->seek(0);
        stream<<dataSend.size();

        tcpSocket.write(dataSend);
//        QString demo;
//        demo.prepend(dataSend);
//        qDebug()<<demo;
        //qDebug()<<dataSend.size();
        //emit emitSendData(dataSend);
        //ui->textInput->clear();
        if(!m_colorStyle)
        {
            QFile file(":/qss/blue.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->m_btnSend->setStyleSheet(styleSheet);
            file.close();
        }
        else
        {
            QFile file(":/qss/orange.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->m_btnSend->setStyleSheet(styleSheet);
            file.close();
        }

        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

        bool isSending = true; // 发送中

        qDebug()<<"addMessage" << msg << time << ui->m_listWidget->count();
        qDebug()<<"m_isSending" << m_isSending;
        //qDebug()<<"issendding"<<isSending;
        //if(!ui->listWidget->count()%2) {
        if(isSending) {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
            connect(this,&ChatWidget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
        } else {
            bool isOver = true;
            for(int i = ui->m_listWidget->count() - 1; i > 0; i--) {
                QNChatMessage* messageW = (QNChatMessage*)ui->m_listWidget->itemWidget(ui->m_listWidget->item(i));
                connect(this,&ChatWidget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess(m_isSending);
                }
            }
            if(isOver) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
                connect(this,&ChatWidget::emitThemeStyle,messageW,&QNChatMessage::slotThemeStyle);
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
            image = emotionExp.cap(0);
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
            //QPixmap pixmap;
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
        QNChatMessage* messageW = new QNChatMessage(m_changeColor, ui->m_listWidget->parentWidget(),1);
        QListWidgetItem* item = new QListWidgetItem(ui->m_listWidget);
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
        //emit emitSendData(dataSend);
    }
    ui->m_textInput->clear();
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
    ui->m_listWidget->setCurrentRow(ui->m_listWidget->count()-1);
}

void ChatWidget::on_m_btnImage_clicked()
{
    QString image=QFileDialog::getOpenFileName(this,"title",".","Image File (*.png *.jpg *.bmp *.gif");
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
    ui->m_textInput->textCursor().insertFragment(fragment);
    ui->m_textInput->setVisible(true);
    qDebug()<<"textInput"<<ui->m_textInput->toHtml();

//    //封装数据包、包头
//    QByteArray dataSend;
//    QDataStream stream(&dataSend,QIODevice::WriteOnly);
//    stream<<(quint32)0<<data;
//    stream.device()->seek(0);
//    stream<<dataSend.size();

//    tcpSocket.write(dataSend);

}

bool ChatWidget::eventFilter(QObject *obj, QEvent *e)
{
    Q_ASSERT(obj == ui->m_textInput);
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent *event = static_cast<QKeyEvent*>(e);
        if((event->key() == Qt::Key_Enter && (event->modifiers() == Qt::ControlModifier)) ||(event->key() == Qt::Key_Return && (event->modifiers() == Qt::ControlModifier)))
        {
            ui->m_textInput->append("");
            return true;
        }
        else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        {
            QTextCursor cursor = ui->m_textInput->textCursor();
            cursor.clearSelection();
            //cursor.deletePreviousChar();
            if(ui->m_textInput->toPlainText()!="")
            {
                qDebug()<<"ui->textInput->toPlainText()"<<ui->m_textInput->toPlainText();
                on_m_btnSend_clicked();
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
                ui->m_textInput->textCursor().insertFragment(fragment);
                ui->m_textInput->setVisible(true);
                return true;
            }
            else if(mimeData->hasHtml())
            {
                ui->m_textInput->setText(mimeData->html());
            }
        }
    }
    return false;
}

void ChatWidget::changeEvent(QEvent *event)
{
    //    if((event->type()==QEvent::WindowStateChange) && this->isMinimized())
    //    {
    //        this->hide();
    //    }
    QWidget::changeEvent(event);
}

void ChatWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


    //ui->textEdit->resize(this->width() - 20, ui->widget->height() - 20);
    //ui->textEdit->move(10, 10);

    //ui->pushButton->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() - 10,
    //ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() - 10);


    for(int i = 0; i < ui->m_listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->m_listWidget->itemWidget(ui->m_listWidget->item(i));
        QListWidgetItem* item = ui->m_listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}

void ChatWidget::on_m_shotScreenBtn_clicked()
{
    m_shotScreen = new Screen;
    m_shotScreen->show();
    connect(m_shotScreen,&Screen::emitPixmap,this,&ChatWidget::slotshotPixmap);
}

void ChatWidget::btnSendStyle()
{
    if(ui->m_textInput->toPlainText()!="")
    {
        if(!m_colorStyle)
        {
            ui->m_btnSend->setStyleSheet("font-size: 12pt;"
                                       "background-color: rgb(30,144,255);"
                                       "color: rgb(255, 255, 255);"
                                       "border:none;");
        }
        else
        {
            ui->m_btnSend->setStyleSheet("font-size: 12pt;"
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
            ui->m_btnSend->setStyleSheet(styleSheet);
            file.close();
        }
        else
        {
            QFile file(":/qss/orange.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = tr(file.readAll());
            ui->m_btnSend->setStyleSheet(styleSheet);
            file.close();
        }
    }
}

void ChatWidget::slotshotPixmap()
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
    ui->m_textInput->textCursor().insertFragment(fragment);
    ui->m_textInput->setVisible(true);

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

void ChatWidget::on_m_fileBtn_clicked()
{
    if(!m_isOnce)
    {

        m_fileClient = new Client(m_colorStyle);

        m_fileClient->show();
        m_isOnce = true;
        connect(this,&ChatWidget::emitThemeStyle,m_fileClient,&Client::themeStyle);
        connect(m_fileClient,&Client::emitClose,[this]{
            m_isOnce = false;
        });
    }
    else
    {
        m_fileClient->activateWindow();
    }
}

ChatWidget::~ChatWidget()
{
    m_thread->quit();
    m_thread->wait();
    delete m_scrollBar;
    m_scrollBar = nullptr;
    delete ui;
}



