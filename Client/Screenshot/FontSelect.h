#pragma once

#include <QWidget>
#include <qtoolbutton.h>
#include <QHBoxLayout>
#include <qcombobox.h>
#include <QFontComboBox>
#include <QFontDatabase>
#include "qabstractitemview.h"
#include "MyScrollBar.h"

class FontSelect : public QWidget
{
	Q_OBJECT

public:
	FontSelect(QWidget *parent = nullptr);
	~FontSelect();
signals:
	void emitColor(QColor color);
	void emitSize(int size);
	void emitFontFamily(QString string);
	void emitbtnClicked(bool btnClicked);
	void emitItalicClick(bool btnClicked);
	void emitUnderlineClick(bool btnClicked);
public slots:
	void ShowColorBtn();
	void slotCurrentIndexChanged(const QString &str);
	void slotFontIndexChanged(const QString &str);
private:
	QHBoxLayout     *m_hLayout;
	QFontComboBox	*fontComboBox;
	QComboBox		*sizeComboBox;
	QToolButton		*boldBtn;			//粗体
	QToolButton		*italicBtn;			//斜体
	QToolButton		*underlineBtn;		//下划线
	QToolButton		*colorBtn;			//颜色
	bool			m_isBoldClick;
	bool			m_isItalicClick;
	bool			m_isUnderlineClick;
};
