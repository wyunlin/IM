#include "myemotionwidget.h"
#include <QHeaderView>
#include <QPainterPath>
#include <QPainter>
#include <qmath.h>
#include <QApplication>
#include <QDebug>

MyEmotionWidget::MyEmotionWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
    m_mainLayout = new QHBoxLayout(this);
    m_tableWidget = new QTableWidget(this);
    //m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->addWidget(m_tableWidget);
    m_tableWidget->setRowCount(4);
    m_tableWidget->setColumnCount(4);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->horizontalHeader()->setHighlightSections(false);
    m_tableWidget->horizontalScrollBar()->setEnabled(false);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->setStyleSheet("border:none;");
    initEmotion();
}

void MyEmotionWidget::paintEvent(QPaintEvent *event)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width() - 20, this->height() - 20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(163, 188, 228, 50);
    for (int i = 0; i < 10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
        color.setAlpha(150 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
    QPainter painter2(this);
    painter2.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter2.setBrush(QBrush(Qt::white));
    painter2.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setLeft(9);
    rect.setTop(9);
    rect.setWidth(rect.width() - 9);
    rect.setHeight(rect.height() - 9);
    painter2.drawRoundedRect(rect, 4, 4);
    QWidget::paintEvent(event);
}

void MyEmotionWidget::addEmotionItem(QString fileName)
{
    // 获取当前添加到第几行第几列
    int row = m_emotionList.size() / m_tableWidget->rowCount();
    int column = m_emotionList.size() % m_tableWidget->rowCount();

    QTableWidgetItem* tableWidgetItem = new QTableWidgetItem;
    m_tableWidget->setItem(row, column, tableWidgetItem);

//    QLabel* emotionIcon = new QLabel;
//    emotionIcon->setAlignment(Qt::AlignCenter);
//    emotionIcon->setMargin(4);
//    QPixmap pixmap(fileName);
//    //qDebug()<<"pixmap"<<pixmap;
//    emotionIcon->setPixmap(pixmap);
    QPushButton *emotionIcon = new QPushButton;
    int propertyNum = row*4+(column+1);
    emotionIcon->setProperty("emotionBtn",propertyNum);
    const QString emotionTip(QString::fromLocal8Bit("调皮"));
    emotionIcon->setToolTip(emotionTip);
    emotionIcon->setCursor(Qt::PointingHandCursor);
    emotionIcon->setStyleSheet(QString("QPushButton:hover{background-color:#DCDCDC;}"
                               "QPushButton:pressed{background-color:#DCDCDC;}"
                               "QPushButton{background-color:white;image:url(%1);}").arg(fileName));
    //emotionIcon->setStyleSheet(QString("image:url(%1);").arg(fileName));
    m_tableWidget->setCellWidget(row, column, emotionIcon);
    m_emotionList.push_back(fileName);
    m_btnList.push_back(emotionIcon);
    connect(emotionIcon,&QPushButton::clicked,this,&MyEmotionWidget::slotBtn);
}

void MyEmotionWidget::initEmotion()
{
    m_tableWidget->setFocusPolicy(Qt::NoFocus);

    QString path = ":/Resources/1.png";
    for (int i = 0; i < 16; i++)
    {
        addEmotionItem(path);
    }
}

void MyEmotionWidget::slotBtn()
{
    QPushButton *emotionBtn = qobject_cast<QPushButton*>(sender());
    int index = emotionBtn->property("emotionBtn").toInt();
    QString filename = m_emotionList.at(index-1);
    emit emitEmotionPath(filename);
    qDebug()<<"filename"<<filename;
    this->close();
}

bool MyEmotionWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange)
    {
        if(QApplication::activeWindow() != this)
        {
            this->close();
        }
    }

    return QWidget::event(event);
}

MyEmotionWidget::~MyEmotionWidget()
{

}
