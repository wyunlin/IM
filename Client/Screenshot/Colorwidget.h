#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QColorDialog>


class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent = 0);
    ~ColorWidget();

public:
	void hideBtnSize();
signals:
	void emitFontSize(int size);
	void emitFontColor(QColor color);
	void emitShow(QColorDialog &color);
public slots:
    void ShowColorBtn();
private:
	QToolButton     *m_minFont;
	QToolButton     *m_midFont;
	QToolButton     *m_maxFont;
    QToolButton     *m_btnRed;
    QToolButton     *m_btnGreen;
    QToolButton     *m_btnBlue;
    QToolButton     *m_btnOrange;
    QToolButton     *m_btnBlack;
    QToolButton     *m_btnGrey;
    QToolButton     *m_colorDialog;
	QHBoxLayout     *m_hLayout;
	QLabel          *m_minLabel;
	QLabel          *m_midLabel;
	QLabel          *m_maxLabel;	
	
};

#endif // COLORWIDGET_H
