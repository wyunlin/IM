#include "Client.h"
#include "ui_Client.h"
#include <QtNetwork>
#include <QFileDialog>
#include <QPainter>

Client::Client(bool ischange, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowIcon(QIcon(":/Resources/IM.ico"));
    this->setWindowTitle(QString::fromLocal8Bit("发送文件"));
//    m_payloadSize = 64*1024;
//    m_totalBytes=0;
//    m_bytesWritten=0;
//    m_bytesToWrite=0;
    if(!ischange)
    {
        QFile file(":/qss/blue.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->pushButton->setStyleSheet(styleSheet);
        ui->pushButton_2->setStyleSheet(styleSheet);
        file.close();
    }
    else
    {
        QFile file(":/qss/orange.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->pushButton->setStyleSheet(styleSheet);
        ui->pushButton_2->setStyleSheet(styleSheet);
        file.close();
    }

    ui->pushButton_2->setEnabled(false);
    dealWithFileThread();
    ui->widget->setStyleSheet("background-color:white;font-family:Microsoft YaHei;");
    ui->closeBtn->setStyleSheet("border-image:url(:/Resources/blackclose.png);background-color:transparent;");
    connect(ui->closeBtn,&QPushButton::clicked,[=]{
        emit emitClose();
        this->close();
    });
    //connect(m_tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    //connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    //connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(sendBtnClicked()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(openBtnClicked()));
    connect(m_fileThread,&FileThread::linkSuccess,[=]{
        ui->label_3->setText(QString::fromLocal8Bit("已连接"));
    });
//    connect(m_fileThread,&FileThread::changeProgress,[=](qint64 totalB,qint64 byteW){
//            ui->progressBar->setMaximum(totalB);
//            ui->progressBar->setValue(byteW);
//    });
    connect(m_fileThread,&FileThread::changeProgress,this,&Client::updateClientProgress,Qt::QueuedConnection);
    connect(m_fileThread,&FileThread::emitSocketError,[=]
    {
        ui->label_3->setText(QString::fromLocal8Bit("客户端就绪"));

        ui->progressBar->reset();
        ui->pushButton_2->setEnabled(true);
    });
}

void Client::dealWithFileThread()
{
    m_thread = new QThread;
    m_fileThread = new FileThread;
    m_fileThread->moveToThread(m_thread);
    connect(this,&Client::emitStartFileThread,[=]{
        m_thread->start();
        emit emitFileDoWork(m_fileName);
    });
    connect(this,&Client::emitFileDoWork,m_fileThread,&FileThread::slot_dowork);
    connect(m_fileThread,&FileThread::sig_finish,[=]
    {
        ui->pushButton_2->setEnabled(false);
        ui->label_3->setText(QString::fromLocal8Bit("传送文件%1成功").arg(m_fileName));
    });
    connect(m_thread, &QThread::finished, m_fileThread, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
}

void Client::openBtnClicked()
{
    ui->progressBar->setValue(0);
    ui->label_3->setText(QString::fromLocal8Bit("状态：等待打开文件!"));
    openFile();
}

void Client::openFile()
{
    m_fileName = QFileDialog::getOpenFileName(this);
    if(!m_fileName.isEmpty())
    {
        ui->pushButton_2->setEnabled(true);
        ui->label_3->setText(QString::fromLocal8Bit("打开文件%1成功").arg(m_fileName));
    }
}

void Client::sendBtnClicked()
{
    send();
}

void Client::themeStyle(bool isChange)
{
    if(isChange)
    {
        QFile file(":/qss/orange.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->pushButton->setStyleSheet(styleSheet);
        ui->pushButton_2->setStyleSheet(styleSheet);
        file.close();
    }
    else
    {
        QFile file(":/qss/blue.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = tr(file.readAll());
        ui->pushButton->setStyleSheet(styleSheet);
        ui->pushButton_2->setStyleSheet(styleSheet);
        file.close();
    }
}

void Client::send()
{
    ui->pushButton_2->setEnabled(false);
    //m_bytesWritten = 0;
    ui->label_3->setText(QString::fromLocal8Bit("连接中..."));
    emit emitStartFileThread();
    //m_tcpClient->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());
}

void Client::startTransfer()
{
//    m_localFile = new QFile(m_fileName);
//    if(!m_localFile->open(QFile::ReadOnly))
//    {
//        qDebug()<<"Client:open file error";
//        return;
//    }

//    m_totalBytes = m_localFile->size();
//    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
//    sendOut.setVersion(QDataStream::Qt_5_7);
//    QString currentFileName = m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);
//    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
//    m_totalBytes+=m_outBlock.size();
//    sendOut.device()->seek(0);
//    sendOut<<m_totalBytes<<qint64(m_outBlock.size()-sizeof(qint64)*2);
//    m_bytesToWrite = m_totalBytes-m_tcpClient->write(m_outBlock);
//    m_outBlock.resize(0);
}

void Client::updateClientProgress(qint64 totalB,qint64 byteW)
{
    ui->progressBar->setMaximum(totalB);
    ui->progressBar->setValue(int(((float)byteW/totalB)*100));
    //qDebug()<<"byteW/totalB)*100"<<((float)byteW/totalB)*100;
    ui->progressBar->repaint();
//    m_bytesWritten += numBytes;
//    if(m_bytesToWrite>0)
//    {
//        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
//        m_bytesToWrite-=static_cast<qint64>(m_tcpClient->write(m_outBlock));
//        m_outBlock.resize(0);
//    }
//    else
//    {
//        m_localFile->close();
//    }
//    ui->progressBar->setMaximum(m_totalBytes);
//    ui->progressBar->setValue(m_bytesWritten);

//    if(m_bytesWritten == m_totalBytes)
//    {
//        ui->label_3->setText(QString::fromLocal8Bit("传送文件%1成功").arg(m_fileName));
//        m_localFile->close();
//        m_tcpClient->close();
//    }
}

void Client::displayError(QAbstractSocket::SocketError)
{
//    qDebug()<<m_tcpClient->errorString();
//    ui->label_3->setText(QString::fromLocal8Bit("客户端就绪"));
//    m_tcpClient->close();
//    ui->progressBar->reset();
//    ui->pushButton_2->setEnabled(true);
}

void Client::paintEvent(QPaintEvent * event)
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
    QWidget::paintEvent(event);
}

void Client::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    movePoint = event->globalPos() - this->pos();
}

void Client::mouseReleaseEvent(QMouseEvent * event)
{
    mousePress = false;
}

void Client::mouseMoveEvent(QMouseEvent * event)
{
    if (mousePress)
    {
        QPoint movePos = event->globalPos();
        this->move(movePos - movePoint);
    }
}


Client::~Client()
{
    delete ui;
}


