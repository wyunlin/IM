#include "widget.h"
#include <QApplication>
#include "LoginWidget/LoginWidget.h"
#include "chatmessage/qnchatmessage.h"
#include "imagelabel.h"
#include "imageScale/scalewidget.h"
#include "myemotionwidget.h"
#include "closedialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    LoginWidget *w = new LoginWidget;
    w->show();
//    QNChatMessage *w = new QNChatMessage;
//    w->show();
//    ImageWidget *w;
//    w->show();
//    ImageLabel W;
//    W.show();
//    ScaleWidget *w = new ScaleWidget;
//    w->show();
//    MyEmotionWidget *w = new MyEmotionWidget;
//    w->show();
//    CloseDialog *demo = new CloseDialog;
//    demo->show();
    return a.exec();
}
