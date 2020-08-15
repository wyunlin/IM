#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <qpushbutton.h>
#include <qpainter.h>
#include <qmath.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include "GeneralLogin.h"
#include "QrcodeWidget.h"
#include "widget.h"
#include "MyToast.h"

class LoginWidget : public QWidget
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = Q_NULLPTR);
	~LoginWidget();

public:
	void InitLoginUI();					//初始话账号登录ui
	void InitForgetUI();				//初始话忘记密码ui
    void InitRegistUI();                //初始话注册ui
    void saveCfg();
    void loadCfg();
    void linkMySQL();
public slots:
	void slotReturnLogin();
	void slotForgetBtn();				//忘记密码按钮槽函数
    void slotRegistBtn();
	void slotCheckRem(bool state);		//记住密码复选框状态改变槽函数
	void slotSubmit();					//登录提交按钮槽函数
	void slotCaptchaBtn();				//发送验证码槽函数
	void slotOkBtn();					//确定按钮槽函数
	void slotDelete();
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	
private:
	//账号登录变量
	bool			mousePress; //按钮点击标志位
	QPoint			movePoint; //鼠标移动
    QLabel			*m_name = nullptr;
    QPushButton		*m_btnClose = nullptr;
    QHBoxLayout		*m_headHLayout = nullptr;
    QWidget			*m_headWidget = nullptr;
    QVBoxLayout		*m_mainVLayout = nullptr;
    QrcodeWidget	*m_qrcodeWidget = nullptr;
    GeneralLogin	*m_generalLogin = nullptr;

	//忘记密码变量
    QWidget			*m_fgHeadWidget = nullptr;
    QWidget			*m_fgContentWidget = nullptr;
    QHBoxLayout		*m_fgHeadLayout = nullptr;
    QVBoxLayout		*m_fgContentLayout = nullptr;
    QPushButton		*m_fgReturnBtn = nullptr;
    QPushButton		*m_fgCloseBtn = nullptr;
    QLabel			*m_forgetLabel = nullptr;
    QLineEdit		*m_phoneLineEdit = nullptr;
    QLineEdit		*m_captchaLineEdit = nullptr;
    QPushButton		*m_captchaBtn = nullptr;
    QPushButton		*m_okBtn = nullptr;
    QLabel			*m_fgspaceBtn = nullptr;
    Widget          *m_mainWidget = nullptr;

    //注册变量
    QWidget         *m_rgHeadWidget = nullptr;
    QWidget			*m_rgContentWidget = nullptr;
    QHBoxLayout		*m_rgHeadLayout = nullptr;
    QVBoxLayout		*m_rgContentLayout = nullptr;
    QPushButton		*m_rgReturnBtn = nullptr;
    QPushButton		*m_rgCloseBtn = nullptr;
    QLineEdit		*m_rgphoneLineEdit = nullptr;
    QLineEdit		*m_pwdLineEdit = nullptr;
    QLineEdit		*m_YZLineEdit = nullptr;
    QPushButton		*m_rgBtn = nullptr;
    QLabel			*m_rgspaceBtn = nullptr;
    QPushButton     *m_btnEye = nullptr;
    bool            m_isLook = false;

    bool            m_remeberPwd;

    QString         m_RemeberStr;
    QString         m_username;
    QString         m_pwd;

    QSqlDatabase    db;

};
