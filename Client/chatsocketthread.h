#ifndef CHATSOCKETTHREAD_H
#define CHATSOCKETTHREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

class ChatSocketThread : public QObject
{
    Q_OBJECT
public:
    explicit ChatSocketThread(QObject *parent = nullptr);
    ~ChatSocketThread();
    void connectToHost();

signals:
    void emitReturnData(QByteArray dataFull);
public slots:
    void sendData(QByteArray dataSend);
    void onConnected();
    void onDisConnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadReady();

private:
    QTcpSocket      tcpSocket;
    int             imageIndex;
    quint32         sizePackLast;
};

#endif // CHATSOCKETTHREAD_H
