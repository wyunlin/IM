#ifndef TUOPANMENU_H
#define TUOPANMENU_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QPushButton>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QWidgetAction>
#include <QEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>

class TuoPanMenu : public QSystemTrayIcon
{
public:
    TuoPanMenu(QObject *parent = Q_NULLPTR);
    ~TuoPanMenu();

    bool eventFilter(QObject *obj, QEvent *event);
    void createTopWidget();
    void createBottomWidget();
private:
    QMenu* m_trayMenu;

    QWidget* m_topWidget;
    QWidgetAction* m_topWidgetAction;
    QLabel* m_topLabel;
    QLabel* m_homeBtn;

    QWidget* m_bottomWidget;
    QWidgetAction* m_bottomWidgetAction;
    QPushButton* m_updateBtn;
    QPushButton* m_aboutBtn;
    QPushButton* m_exitBtn;

    QAction* m_runOnSystemBoot;
    QAction* m_helpOnline;
    QAction* m_homePage;
    QAction* m_notification;
    QAction* m_settings;
};

#endif // TUOPANMENU_H
