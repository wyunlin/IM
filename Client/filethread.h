#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QFile>
#include <QMutexLocker>
#include <QMutex>
#include <chrono>
#include <thread>

class FileThread : public QObject
{
    Q_OBJECT
public:
    explicit FileThread(QObject *parent = nullptr);
    ~FileThread();

signals:
    void sig_finish();
    void linkSuccess();
    void changeProgress(qint64 totalB,qint64 byteW);
    void emitSocketError();

public slots:
    void slot_dowork(QString filename);
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);
private:
    QTcpSocket *m_tcpClient;
    QFile *m_localFile;
    qint64 m_totalBytes;        //文件总字节数
    qint64 m_bytesWritten;      //已发送的字节数
    qint64 m_bytesToWrite;      //尚未发送的字节数
    qint64 m_payloadSize;       //每次发送数据大小
    QString m_fileName;         //文件名字的字节数
    QByteArray m_outBlock;      //数据缓冲区
    QMutex m_mutex;
    //QString      m_file;
};

#endif // FILETHREAD_H
