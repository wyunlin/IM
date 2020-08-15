#-------------------------------------------------
#
# Project created by QtCreator 2020-03-07T08:19:53
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    LoginWidget/GeneralLogin.cpp \
    LoginWidget/LoginWidget.cpp \
    LoginWidget/QrcodeWidget.cpp \
    Screenshot/Colorwidget.cpp \
    Screenshot/controlwidget.cpp \
    Screenshot/FontSelect.cpp \
    Screenshot/myrect.cpp \
    Screenshot/qslabel.cpp \
    Screenshot/screen.cpp \
    Screenshot/LLTextScreenLB.cpp \
    Client/Client.cpp \
    MyScrollBar.cpp \
    MyToast.cpp \
    mytextedit.cpp \
    filethread.cpp \
    radiusprogressbar.cpp \
    chatmessage/qnchatmessage.cpp \
    imagelabel.cpp \
    imageScale/imagescale.cpp \
    informationwidget.cpp \
    chatwidget.cpp \
    chatsocketthread.cpp \
    imageScale/scalewidget.cpp \
    QrCode/BitBuffer.cpp \
    QrCode/QrCode.cpp \
    QrCode/QrSegment.cpp \
    myemotionwidget.cpp \
    tuopanmenu.cpp \
    mymenu.cpp \
    closedialog.cpp

HEADERS += \
        widget.h \
    LoginWidget/GeneralLogin.h \
    LoginWidget/LoginWidget.h \
    LoginWidget/QrcodeWidget.h \
    Screenshot/Colorwidget.h \
    Screenshot/controlwidget.h \
    Screenshot/FontSelect.h \
    Screenshot/myrect.h \
    Screenshot/qslabel.h \
    Screenshot/screen.h \
    Screenshot/LLTextScreenLB.h \
    Client/Client.h \
    MyScrollBar.h \
    MyToast.h \
    mytextedit.h \
    filethread.h \
    radiusprogressbar.h \
    chatmessage/qnchatmessage.h \
    imagelabel.h \
    imageScale/imagescale.h \
    informationwidget.h \
    chatwidget.h \
    chatsocketthread.h \
    imageScale/scalewidget.h \
    QrCode/BitBuffer.hpp \
    QrCode/QrCode.hpp \
    QrCode/QrSegment.hpp \
    myemotionwidget.h \
    tuopanmenu.h \
    mymenu.h \
    closedialog.h

FORMS += \
        widget.ui \
    Client/Client.ui \
    chatwidget.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    Resources/arrow.png \
    Resources/blackclose.png \
    Resources/bluecalender.png \
    Resources/blueclose.png \
    Resources/bluelast.png \
    Resources/bluenext.png \
    Resources/bold.png \
    Resources/calendar.png \
    Resources/can.png \
    Resources/check.png \
    Resources/clickxiala.png \
    Resources/colorBtn.png \
    Resources/complete.png \
    Resources/demoimg.png \
    Resources/download.png \
    Resources/dropout.png \
    Resources/italicBtn.png \
    Resources/last.png \
    Resources/next.png \
    Resources/or.png \
    Resources/passApproval.png \
    Resources/pen.png \
    Resources/return.png \
    Resources/Revoke.png \
    Resources/Round.png \
    Resources/square.png \
    Resources/text.png \
    Resources/underlineBtn.png \
    Resources/waitApproval.png \
    Resources/xiala.png \
    Resources/Xialat.png \
    qss/blue.qss \
    qss/orange.qss
