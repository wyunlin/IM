#include "chatsocketthread.h"

ChatSocketThread::ChatSocketThread(QObject *parent) : QObject(parent)
{
    connectToHost();
}

ChatSocketThread::~ChatSocketThread()
{
    tcpSocket.close();
}

void ChatSocketThread::connectToHost()
{
    connect(&tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(&tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisConnected()));
    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(&tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadReady()));
    tcpSocket.connectToHost("127.0.0.1",8888);
    sizePackLast = 0;
}

void ChatSocketThread::sendData(QByteArray dataSend)
{
//    QString demo(dataSend);
//    qDebug()<<"dataSend"<<demo;
//    qDebug()<<dataSend.size();
    tcpSocket.write(dataSend);
    //QMetaObject::invokeMethod( &tcpSocket, std::bind( static_cast< qint64(QTcpSocket::*)(const QByteArray &) >( &QTcpSocket::write ), &tcpSocket, dataSend ) );
}

void ChatSocketThread::onConnected()
{
    qDebug()<<"connected";
}

void ChatSocketThread::onDisConnected()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    socket->deleteLater();
    qDebug()<<"disconnected";
}

void ChatSocketThread::onError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"error"<<socketError;
}

void ChatSocketThread::onReadReady()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    qint64 sizeNow = 0;
    do{
        sizeNow = socket->bytesAvailable();
        QDataStream stream(socket);
        if(sizePackLast == 0)
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
        sizePackLast = 0;

        //判断剩下的字节数，是否有粘包的情况
        sizeNow = socket->bytesAvailable();

        emit emitReturnData(dataFull);
    }while(sizeNow>0);
}
