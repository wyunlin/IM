#ifndef MYEMOTIONWIDGET_H
#define MYEMOTIONWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>
#include <qstandarditemmodel.h>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QEvent>

class MyEmotionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyEmotionWidget(QWidget *parent = nullptr);
    ~MyEmotionWidget();

signals:
    void emitEmotionPath(QString filename);
public slots:
    void slotBtn();
protected:
    void paintEvent(QPaintEvent *event);
    bool event(QEvent *event);
private:
    void addEmotionItem(QString fileName);
    void initEmotion();
private:
    QTableWidget  *m_tableWidget = nullptr;
    QHBoxLayout   *m_mainLayout = nullptr;
    QStringList   m_emotionList;
    QList<QPushButton*>     m_btnList;
};

#endif // MYEMOTIONWIDGET_H
