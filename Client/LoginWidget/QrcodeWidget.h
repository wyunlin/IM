#pragma once

#include <QWidget>
#include <qpainter.h>
#include <QMouseEvent>
#include <qlabel.h>
#include <QPushButton>
#include <string>
#include <vector>
#include "QrCode/QrCode.hpp"
#include "QrCode/BitBuffer.hpp"
#include "QrCode/QrSegment.hpp"
using namespace qrcodegen;

class QrcodeWidget : public QWidget
{
	Q_OBJECT

public:
	QrcodeWidget(QWidget *parent = Q_NULLPTR);
	~QrcodeWidget();
    void Show_QRcode_Picture(QString message);
protected:
	void paintEvent(QPaintEvent *event);

private:
	bool			mousePress; //按钮点击标志位
	QPoint			movePoint; //鼠标移动
	QPolygon		polygon;
    QLabel          *m_qrcodeImg;
	QLabel			*m_qcodeLabel;
	QLabel			*m_orLabel;
};
