#pragma once

#include <QWidget>
#include <qpainter.h>
#include <QMouseEvent>
#include <QLabel>
#include <qlineedit.h>
#include <QCheckBox>
#include <QPushButton>
#include <qline.h>

class GeneralLogin : public QWidget
{
	Q_OBJECT

public:
	GeneralLogin(QWidget *parent = Q_NULLPTR);
	~GeneralLogin();
    QString getName() const;
    QString getPassword() const;
signals:
	void emitCheckClicked(bool state);
	void emitForgetBtnClicked();
    void emitRegistBtnClicked();
	void emitSubitBtnClicked();

protected:
	void paintEvent(QPaintEvent *event);

public:
    QLineEdit		*m_nameLineEdit;
    QLineEdit		*m_pwdLineEdit;
    QCheckBox		*m_rememberCheckBox;
private:
	bool			mousePress; //按钮点击标志位
	QPoint			movePoint; //鼠标移动
	QPolygon		polygon;
	QLabel			*m_qcodeLabel;
	QPushButton		*m_forgetBtn;
    QPushButton     *m_registBtn;
	QPushButton		*m_submitBtn;
	QLine			m_line;
	QPen			m_pen;
	QLabel			*m_canLabel;
};
