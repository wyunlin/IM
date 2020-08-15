#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QAbstractSocket>
#include <QThread>
#include <QMouseEvent>
#include <QFile>
#include "filethread.h"
#include "radiusprogressbar.h"
class QTcpSocket;
class QFile;

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(bool ischange, QWidget *parent = 0);
    ~Client();

    void dealWithFileThread();

public slots:
    void openFile();
    void send();
    void startTransfer();
    void updateClientProgress(qint64 totalB,qint64 byteW);
    void displayError(QAbstractSocket::SocketError);
    void openBtnClicked();
    void sendBtnClicked();
    void themeStyle(bool isChange);

signals:
    void emitStartFileThread();
    void emitFileDoWork(QString filename);
    void emitClose();

private slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    Ui::Client *ui;
//    QTcpSocket *m_tcpClient;
//    QFile *m_localFile;
//    qint64 m_totalBytes;        //文件总字节数
//    qint64 m_bytesWritten;      //已发送的字节数
//    qint64 m_bytesToWrite;      //尚未发送的字节数
//    qint64 m_payloadSize;       //每次发送数据大小
    QString         m_fileName;         //文件名字的字节数
    QByteArray      m_outBlock;      //数据缓冲区
    QThread         *m_thread = nullptr;
    FileThread      *m_fileThread = nullptr;
    bool			mousePress = false; //按钮点击标志位
    QPoint			movePoint; //鼠标移动
};

#endif // CLIENT_H
