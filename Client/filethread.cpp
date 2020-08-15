#include "filethread.h"
#include <QDataStream>

FileThread::FileThread(QObject *parent) : QObject(parent)
{
    m_payloadSize = 64*1024;
    m_totalBytes = 0;
    m_bytesWritten = 0;
    m_bytesToWrite = 0;
    m_tcpClient = new QTcpSocket(this);
    connect(m_tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
//    connect(m_tcpClient,&QTcpSocket::connected,[=]
//    {
//        qDebug()<<"demo";
//        qDebug()<<"demo";
//        qDebug()<<"demo";
//        qDebug()<<"demo";
//    });
    connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
    connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
}

void FileThread::slot_dowork(QString filename)
{
    m_fileName = filename;
    m_tcpClient->connectToHost("127.0.0.1",10086);
}

void FileThread::startTransfer()
{
    m_localFile = new QFile(m_fileName);
    if(!m_localFile->open(QFile::ReadOnly))
    {
        qDebug()<<"Client:open file error";
        return;
    }
    emit linkSuccess();
    m_totalBytes = m_localFile->size();
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_7);
    QString currentFileName = m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    m_totalBytes+=m_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<m_totalBytes<<qint64(m_outBlock.size()-sizeof(qint64)*2);
    m_bytesToWrite = m_totalBytes-m_tcpClient->write(m_outBlock);
    m_outBlock.resize(0);
}

void FileThread::updateClientProgress(qint64 numBytes)
{
    m_bytesWritten += numBytes;
    if(m_bytesToWrite>0)
    {
        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        m_bytesToWrite-=static_cast<qint64>(m_tcpClient->write(m_outBlock));
        m_outBlock.resize(0);
    }
    else
    {
        m_localFile->close();
    }
    //m_mutex.lock();
    //std::chrono::milliseconds dura(100);
    //std::this_thread::sleep_for(dura);
    changeProgress(m_totalBytes,m_bytesWritten);
    //m_mutex.unlock();
    if(m_bytesWritten == m_totalBytes)
    {
        emit sig_finish();
        //ui->label_3->setText(QString::fromLocal8Bit("传送文件%1成功").arg(m_fileName));
        m_localFile->close();
        m_tcpClient->close();
    }
}

void FileThread::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<m_tcpClient->errorString();
    m_tcpClient->close();
    emit emitSocketError();
}

FileThread::~FileThread()
{

}
