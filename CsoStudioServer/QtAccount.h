#pragma once
#include "QtConnect.h"
#include <QMainWindow>
#include "ui_QtAccount.h"

class QtAccount : public QMainWindow
{
	Q_OBJECT

public:
	QtAccount(QWidget *parent = nullptr);
	~QtAccount();

	void Init();
	// @…Ë÷√¿‡–Õ
	void SetTypeHandle(int type);
	
	void SetLoginModule(CLoginModule* pModule);

	virtual void showEvent(QShowEvent* event);

	void SetAccountInfo(PTAG_GAME_AC_LOGIN pAccountInfo);

	void View();

public slots:
	void slSubmit();
signals:
	void sig_AddUserInfo(PTAG_GAME_AC_LOGIN);
private:
	Ui::QtAccountClass ui;
	ULONG              m_TypeHandle;
	SE_GAME_AC_CLIENT* m_pAccountInfo;
	CLoginModule*      m_pLoginModule;
	PTAG_GAME_AC_LOGIN	m_LoginInfo;
};
