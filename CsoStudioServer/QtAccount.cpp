#include "QtAccount.h"

QtAccount::QtAccount(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	Init();
	
}

QtAccount::~QtAccount()
{
	
}

void QtAccount::Init()
{
	m_pLoginModule = nullptr;
	connect(ui.pushButton_Submit, &QPushButton::clicked, this, &QtAccount::slSubmit);
}

void QtAccount::SetTypeHandle(int type)
{
	m_TypeHandle = type;
}


void QtAccount::SetLoginModule(CLoginModule* pModule)
{
	m_pLoginModule = pModule;
}

void QtAccount::showEvent(QShowEvent* event)
{
	switch (m_TypeHandle)
	{
	case 1:
	{
		setWindowTitle("添加账号信息");
		m_pAccountInfo = new SE_GAME_AC_CLIENT();
		ui.lineEdit_GameAccount->setEnabled(true);
		break;
	}
	case 2:
	{
		setWindowTitle("修改账号信息");
		ui.lineEdit_GameAccount->setEnabled(false);
		View();
		break;
	}
	default:
		break;
	}
}

void QtAccount::SetAccountInfo(PTAG_GAME_AC_LOGIN pAccountInfo)
{
	m_LoginInfo    = pAccountInfo;
	m_pAccountInfo = m_LoginInfo->pData;
}

void QtAccount::View()
{
	auto SetReg = [&]() ->int {
		int nRet = 0;
		switch (m_pAccountInfo->GameReg)
		{
		case 5:
			nRet = 0;
			break;
		case 6:
			nRet = 1;
			break;
		case 15:
			nRet = 2;
			break;
		default:
			break;
		}
		return nRet;
	};

	ui.comboBox_GameReg->setCurrentIndex(SetReg());
	ui.lineEdit_GameAccount->setText(m_pAccountInfo->GameUserName.data());
	ui.lineEdit_GamePassWord->setText(m_pAccountInfo->GameUserPassWord.data());
	ui.lineEdit_Game2ndPassWord->setText(m_pAccountInfo->Game2ndpassword.data());
	ui.comboBox_GameTask->setCurrentIndex(static_cast<int>(m_pAccountInfo->UserTask));
}

void QtAccount::slSubmit()
{
	auto GameRegFuns = [&]() ->int {
		int nRet = 0;
		switch (ui.comboBox_GameReg->currentIndex())
		{
		case 0:
			nRet = 5;
			break;
		case 1:
			nRet = 6;
			break;
		case 2:
			nRet = 15;
			break;
		default:
			break;
		}
		return nRet;
	};

	m_pAccountInfo->GameReg          = GameRegFuns();
	m_pAccountInfo->GameUserName     = ui.lineEdit_GameAccount->text().toStdString();
	m_pAccountInfo->GameUserPassWord = ui.lineEdit_GamePassWord->text().toStdString();
	m_pAccountInfo->Game2ndpassword  = ui.lineEdit_Game2ndPassWord->text().toStdString();
	m_pAccountInfo->UserTask         = static_cast<TASK_MODE>(ui.comboBox_GameTask->currentIndex());

	if (m_TypeHandle == 1)
	{
		//检查账号是否存在
		if (m_pLoginModule->SearchUserName(m_pAccountInfo->GameUserName))
		{
			QMessageBox::warning(this, "警告", "账号已存在");
			return;
		}
		if (m_pAccountInfo->GameUserName.empty())
		{
			QMessageBox::warning(this, "警告", "账号不能为空");
			return;
		}
		if (m_pAccountInfo->GameUserPassWord.empty())
		{
			QMessageBox::warning(this, "警告", "密码不能为空");
			return;
		}
		emit sig_AddUserInfo(m_pLoginModule->AddLoginInfo(m_pAccountInfo));
	}
	else if (m_TypeHandle == 2)
	{
		emit sig_AddUserInfo(m_LoginInfo);
	}


	
;

	this->close();
}
