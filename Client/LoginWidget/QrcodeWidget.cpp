#include "QrcodeWidget.h"
#include <QIcon>

QrcodeWidget::QrcodeWidget(QWidget *parent)
	: QWidget(parent)
	, mousePress{ false }
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(255, 255, 255)));
	this->setFixedSize(300, 240);
	this->setStyleSheet("background-color:white;font-family:Microsoft YaHei;");
    m_qrcodeImg = new QLabel(this);
	m_qrcodeImg->setStyleSheet("background-color:#DCDCDC;border:none;");
    m_qrcodeImg->setGeometry(70, 20, 160, 160);
    m_qcodeLabel = new QLabel;
    //m_qcodeLabel->setStyleSheet("background-color:white;font-weight:bold;");
    //m_qcodeLabel->setGeometry(50, 220, 80, 20);
    //m_qcodeLabel->setText(QStringLiteral("扫码登录"));
    m_orLabel = new QLabel;
    //m_orLabel->setGeometry(5, 190, 40, 40);
    //m_orLabel->setStyleSheet("border-image:url(:/Resources/or.png);background-color:transparent");
	polygon << QPoint(0, 0) << QPoint(300, 0) << QPoint(300, 210) << QPoint(160, 210) << QPoint(140, 240) << QPoint(0, 240);
    Show_QRcode_Picture(QString("15797725711"));
}

void QrcodeWidget::paintEvent(QPaintEvent * event)
{
	QPainter painter2(this);
	painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter2.setBrush(QBrush(Qt::white));
	painter2.setPen(Qt::transparent);
	painter2.drawPolygon(polygon);
	QWidget::paintEvent(event);
}

void QrcodeWidget::Show_QRcode_Picture(QString message)
{
    std::vector<QrSegment> segs =
           QrSegment::makeSegments(message.toLatin1());
       QrCode qr1 = QrCode::encodeSegments(
           segs, QrCode::Ecc::HIGH, 5, 5, 2, false);
       //创建二维码画布
       QImage QrCode_Image=QImage(qr1.getSize(),qr1.getSize(),QImage::Format_RGB888);

       for (int y = 0; y < qr1.getSize(); y++) {
           for (int x = 0; x < qr1.getSize(); x++) {
               if(qr1.getModule(x, y)==0)
                   QrCode_Image.setPixel(x,y,qRgb(255,255,255));
               else
                   QrCode_Image.setPixel(x,y,qRgb(0,0,0));
           }
       }

       //图像大小转换为适当的大小
       QrCode_Image=QrCode_Image.scaled(m_qrcodeImg->width(),m_qrcodeImg->height(),
                       Qt::KeepAspectRatio);
       //转换为QPixmap在Label中显示
       m_qrcodeImg->setPixmap(QPixmap::fromImage(QrCode_Image));
}

QrcodeWidget::~QrcodeWidget()
{
	delete m_qrcodeImg;
	m_qrcodeImg = nullptr;
	delete m_qcodeLabel;
	m_qcodeLabel = nullptr;
	delete m_orLabel;
    m_orLabel = nullptr;
}



