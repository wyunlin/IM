#ifndef MYMENU_H
#define MYMENU_H

#include <QWidget>
#include <QMenu>
#include <QEvent>
#include <Windows.h>
#include <WinUser.h>
#include "dwmapi.h"
#pragma comment( lib, "dwmapi.lib" )

class MyMenu : public QMenu
{
    Q_OBJECT
public:
    explicit MyMenu(QWidget *parent = nullptr);
    ~MyMenu();

    bool event(QEvent *event);

signals:

public slots:
protected:
};

#endif // MYMENU_H
