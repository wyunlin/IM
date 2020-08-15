#include "tuopanmenu.h"

TuoPanMenu::TuoPanMenu(QObject *parent)
{
    createTopWidget();
    createBottomWidget();
}

bool TuoPanMenu::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_topWidget && event->type() == QEvent::Paint)
    {
        QPainter painter(m_topWidget);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(42, 120, 192));
        painter.drawRect(m_topWidget->rect());
    }
    return QSystemTrayIcon::eventFilter(obj, event);
}

void TuoPanMenu::createTopWidget()
{
    m_topWidget = new QWidget();
    m_trayMenu = new QMenu((QWidget*)QApplication::desktop());
    m_topWidgetAction = new QWidgetAction(m_trayMenu);
    m_topLabel = new QLabel(QStringLiteral("HUST Information Security Lab"));
    m_topLabel->setObjectName(QStringLiteral("WhiteLabel"));
    m_homeBtn = new QLabel(QStringLiteral("Visit"));
    m_homeBtn->setCursor(Qt::PointingHandCursor);
    m_homeBtn->setObjectName(QStringLiteral("WhiteLabel"));

    QVBoxLayout* m_topLayout = new QVBoxLayout();
    m_topLayout->addWidget(m_topLabel, 0, Qt::AlignLeft|Qt::AlignVCenter);
    m_topLayout->addWidget(m_homeBtn, 0, Qt::AlignRight|Qt::AlignVCenter);

    m_topLayout->setSpacing(5);
    m_topLayout->setContentsMargins(5, 5, 5, 5);

    m_topWidget->setLayout(m_topLayout);
    m_topWidget->installEventFilter(this);
    m_topWidgetAction->setDefaultWidget(m_topWidget);
}

void TuoPanMenu::createBottomWidget()
{
    m_bottomWidget = new QWidget();
    m_bottomWidgetAction = new QWidgetAction(m_trayMenu);

    m_updateBtn = new QPushButton(QIcon(":/menu/update"), QStringLiteral("Update"));
    m_updateBtn->setObjectName(QStringLiteral("TrayButton"));
    m_updateBtn->setFixedSize(60, 25);

    m_aboutBtn = new QPushButton(QIcon(":/menu/about"), QStringLiteral("About"));
    m_aboutBtn->setObjectName(QStringLiteral("TrayButton"));
    m_aboutBtn->setFixedSize(60, 25);

    m_exitBtn = new QPushButton(QIcon(":/menu/quit"), QStringLiteral("Exit"));
    m_exitBtn->setObjectName(QStringLiteral("TrayButton"));
    m_exitBtn->setFixedSize(60, 25);

    QHBoxLayout* m_bottomLayout = new QHBoxLayout();
    m_bottomLayout->addWidget(m_updateBtn, 0, Qt::AlignCenter);
    m_bottomLayout->addWidget(m_aboutBtn, 0, Qt::AlignCenter);
    m_bottomLayout->addWidget(m_exitBtn, 0, Qt::AlignCenter);

    m_bottomLayout->setSpacing(5);
    m_bottomLayout->setContentsMargins(5,5,5,5);

    m_bottomWidget->setLayout(m_bottomLayout);
    m_bottomWidgetAction->setDefaultWidget(m_bottomWidget);
}


TuoPanMenu::~TuoPanMenu()
{

}
