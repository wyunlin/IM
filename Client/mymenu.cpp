#include "mymenu.h"

MyMenu::MyMenu(QWidget *parent):QMenu(parent)
{

}

bool MyMenu::event(QEvent *event)
{
    static bool class_amended = false;
    if(event->type() == QEvent::WinIdChange)
    {
        HWND hwnd = reinterpret_cast<HWND>(winId());
        if(class_amended == false)
        {
            class_amended = true;
//            DWORD class_style = ::GetClassLong(hwnd,GCL_STYLE);
//            class_style &=~CS_DROPSHADOW;
//            ::SetClassLong(hwnd,GCL_STYLE,class_style);
        }
        DWMNCRENDERINGPOLICY val = DWMNCRP_ENABLED;
        //::DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &val, sizeof(DWMNCRENDERINGPOLICY));

        // This will turn OFF the shadow
        // MARGINS m = {0};
        // This will turn ON the shadow
//        MARGINS m = {-1};
//        HRESULT hr = ::DwmExtendFrameIntoClientArea(hwnd, &m);
//        if( SUCCEEDED(hr) )
//        {
//            //do more things
//        }
    }
    return QWidget::event(event);
}

MyMenu::~MyMenu()
{

}
