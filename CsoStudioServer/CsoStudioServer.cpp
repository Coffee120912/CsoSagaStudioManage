#include "CsoStudioServer.h"
#include <csv2/writer.hpp>
#include "VerifyManage.h"

CsoStudioServer::CsoStudioServer(QWidget *parent)
    : QWidget(parent), m_pChildMenuDelDispatchTask(nullptr), m_pChildMenuAddDispatchTask(nullptr), m_pMainAutionMenu(nullptr)
{
	m_pInstance = this;
	m_bIsLogin  = false;
    ui.setupUi(this);

	// 安装消息处理程序,主要是为了捕获QT debug 信息
	qInstallMessageHandler(myMessageOutput);
	
	
    uiInit();
    init();
	initMenu();
	configInit();
} 

CsoStudioServer* CsoStudioServer::GetInstance()
{
	return m_pInstance;
}

bool CsoStudioServer::configInit()
{
	m_Login.GetGlobalShare().AutoJoinAuctionEvent = g_pConfig->GetAutoAuction1000M();
	ui.checkBox_AutoAuction1000M->setCheckState(static_cast<Qt::CheckState>(g_pConfig->GetAutoAuction1000M()));

	ui.lineEdit_GamePath->setText(g_pConfig->GetGamePath().data());
	ui.lineEdit_AgentUser->setText(g_pConfig->GetAgentUser().data());


	return true;
}

bool CsoStudioServer::uiInit()
{
	//	游戏监控
	ui.tableWidget_GameMonitor->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_GameMonitor->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget_GameMonitor->setShowGrid(false); //设置不显示格子线
	ui.tableWidget_GameMonitor->verticalHeader()->setVisible(false); //设置垂直头不可见
	ui.tableWidget_GameMonitor->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为时每次选择一行
	ui.tableWidget_GameMonitor->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	ui.tableWidget_GameMonitor->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);//设置平滑一点
	
	//拍卖行
	ui.tableWidget_GameTaskAuction->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_GameTaskAuction->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget_GameTaskAuction->setShowGrid(false); 
	ui.tableWidget_GameTaskAuction->verticalHeader()->setVisible(false); 
	ui.tableWidget_GameTaskAuction->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_GameTaskAuction->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget_GameTaskAuction->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


	//拍卖行上架
	ui.tableWidget_RackingItem->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_RackingItem->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget_RackingItem->setShowGrid(false);
	ui.tableWidget_RackingItem->verticalHeader()->setVisible(false);
	ui.tableWidget_RackingItem->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget_RackingItem->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget_RackingItem->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


	//点击表时不对表头行光亮（获取焦点） 
	ui.tableWidget_GameMonitor->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_GameTaskAuction->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget_RackingItem->horizontalHeader()->setHighlightSections(false);

	//右键菜单
	ui.tableWidget_GameTaskAuction->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_GameTaskAuction, &QTableWidget::customContextMenuRequested, this, &CsoStudioServer::slRightClickAuctionMmenu);
	//鼠标双击bind
	connect(ui.tableWidget_GameTaskAuction, &QTableWidget::doubleClicked, this, &CsoStudioServer::slDoubleClickAuction);
	
	ui.tableWidget_GameMonitor->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tableWidget_GameMonitor, &QTableWidget::customContextMenuRequested, this, &CsoStudioServer::slRightClickAccountMmenu);
	//鼠标双击bind
	connect(ui.tableWidget_GameMonitor, &QTableWidget::doubleClicked, this, &CsoStudioServer::slDoubleClickAccount);
	


	//文件导入账号信息
	connect(ui.pushButton_BatchImpUser, &QPushButton::clicked, this, &CsoStudioServer::slImpUserInfo);
	connect(ui.pushButton_BatchExpUser, &QPushButton::clicked, this, &CsoStudioServer::slExportUserInfo);
	connect(ui.pushButton_UserConfigSava, &QPushButton::clicked, this, &CsoStudioServer::slSaveUserCfgInfo);
	connect(ui.pushButton_UserConfigLoad, &QPushButton::clicked, this, &CsoStudioServer::slLoadUserCfgInfo);
	connect(ui.pushButton_UserCfgDel, &QPushButton::clicked, this, &CsoStudioServer::slDelUserCfgInfo);
	
	//connect(ui.pushButton_UserConfigFlush, &QPushButton::clicked, this, &CsoStudioAgent::slGetGameUserInfo);
	//connect(ui.pushButton_UserCfgDel, &QPushButton::clicked, this, &CsoStudioAgent::slDelGameAllUserInfo);
	//connect(ui.pushButton_UserPassExport, &QPushButton::clicked, this, &CsoStudioAgent::slExportGameUserPassWord);


	connect(ui.pushButton_SavaAuctionCFG, &QPushButton::clicked, this, &CsoStudioServer::slSavaAuctionCFG);
	connect(ui.pushButton_ReadAuctionCFG, &QPushButton::clicked, this, &CsoStudioServer::slReadAuctionCFG);
	connect(ui.pushButton_DelAuctionCFG, &QPushButton::clicked, this, &CsoStudioServer::slDeleteAuctionCFG);
	connect(ui.pushButton_ViewAuction, &QPushButton::clicked, this, static_cast<void(CsoStudioServer::*)(void)>(&CsoStudioServer::slRefreshAuctionView));
	connect(ui.pushButton_ViewWeapon, &QPushButton::clicked, this, &CsoStudioServer::slOpenWeaponItemFile);
	connect(ui.pushButton_ViewPaint, &QPushButton::clicked, this, &CsoStudioServer::slOpenPaintItemFile);
	
	//日志
	connect(this, &CsoStudioServer::sig_DebugInfo, this, &CsoStudioServer::debugLogApped);
	//显示账号信息
	connect(this, &CsoStudioServer::sig_AddShowUserInfo, this, &CsoStudioServer::slAddShowGameUserInfo);
	//绑定启动
	connect(ui.checkBox_StartGame, &QCheckBox::clicked, this, &CsoStudioServer::slStartGame);
	
	m_pDlgAution = new QtAution;
	connect(m_pDlgAution, &QtAution::sigAddAutionTask, this, &CsoStudioServer::slAddAutionTask);
	
	m_pDlgAccount = new QtAccount;
	m_pDlgAccount->SetLoginModule(&m_Login);
	connect(m_pDlgAccount, &QtAccount::sig_AddUserInfo, this, &CsoStudioServer::slAddShowGameUserInfo);


	
	connect(this, &CsoStudioServer::sig_RefreshUserUI, this, &CsoStudioServer::slRefreshUserView);
	
	connect(this, &CsoStudioServer::sig_RefreshAuctionUI, this, static_cast<void(CsoStudioServer::*)(qint64)>(&CsoStudioServer::slRefreshAuctionView));
	

	//拍卖会全局功能绑定
	connect(ui.checkBox_AutoAuction1000M, &QCheckBox::clicked, this, &CsoStudioServer::slAutoAuction1000M);
	
	connect(ui.pushButton_SetGamePath, &QPushButton::clicked, this, &CsoStudioServer::slSetGameDir); 
	connect(ui.pushButton_Login, &QPushButton::clicked, this, &CsoStudioServer::slLogin);

	
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, [=](int index)
	{
			VMProtectBegin(__FUNCTION__);

			if (m_bIsLogin == false && index != 0 )
			{
				ui.tabWidget->setCurrentIndex(0);
				qInfo() << "请先登录！";
			}

			VMProtectEnd();
	});
	connect(this, &CsoStudioServer::sig_RefreshAuctionTaskUI, this, &CsoStudioServer::slRefreshAuctionOnSaleView);
	
	

    return true;
}
void CsoStudioServer::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QByteArray localMsg = msg.toLocal8Bit();

	//ACEDebugFileLog("[{}]type {} : {} ({}:{}, {})", __FUNCTION__, type, localMsg.constData(), context.file, context.line, context.function);

	if (context.function == nullptr || context.line == 0)
	{
		ACEWarningFileLog("MsgType:{} {}", type, localMsg.constData());
		return;
	}
	else
	{

#if ANTI_DEBUG		
	#define QTACEDebugFileLog(format,...) ACELogOut(ACE_LOG_DEBUG,true,context.function, context.line,format,__VA_ARGS__)
	#define QTACEinfoFileLog(format,...) ACELogOut(ACE_LOG_INFO,true,context.function, context.line,format,__VA_ARGS__)
	#define QTACEWanringFileLog(format,...) ACELogOut(ACE_LOG_WARNING,true,context.function, context.line,format,__VA_ARGS__)	
	#define QTACEErrorFileLog(format,...) ACELogOut(ACE_LOG_ERROR,true,context.function, context.line,format,__VA_ARGS__)	
#else
	#define QTACEDebugFileLog(format,...) 
	#define QTACEinfoFileLog(format,...) 
	#define QTACEWanringFileLog(format,...)
	#define QTACEErrorFileLog(format,...) 
#endif // ANTI_DEBUG



	}

	switch (type)
	{
	case QtDebugMsg:
		QTACEDebugFileLog("{} ", localMsg.constData());
		break;
	case QtWarningMsg:
		QTACEWanringFileLog("{} ", localMsg.constData());
		break;
	case QtCriticalMsg:
		QTACEErrorFileLog("{} ", localMsg.constData());
		break;
	case QtFatalMsg:
		QTACEErrorFileLog("[QtFatalMsg]{} ", localMsg.constData());
		break;
	case QtInfoMsg:
		QTACEinfoFileLog("{} ", localMsg.constData());
		emit CsoStudioServer::GetInstance()->sig_DebugInfo(1, msg);
		break;

	default:
		break;
	}
}
void CsoStudioServer::debugLogApped(int type, const QString& data)
{
	static int currentCount = 0;
	//最大行
	static int maxCount = 3000;

	if (currentCount >= maxCount)
	{
		ui.textEdit_debugLog->clear();
		currentCount = 0;
	}

	//过滤回车换行符
	QString strData = data;

	//不同类型不同颜色显示
	QString strType = "->";

	switch (type)
	{
	case 0:				//调试日志
		ui.textEdit_debugLog->setTextColor(QColor("gray"));
		break;
	case 1:				//警告日志
		ui.textEdit_debugLog->setTextColor(QColor("dodgerblue"));
		break;
	case 2:				//错误日志
		ui.textEdit_debugLog->setTextColor(QColor("red"));
		break;
	default:
		break;
	}

	strData = QString("[%1] %2 %3").arg(DATETIME).arg(strType).arg(strData);
	ui.textEdit_debugLog->append(strData);
	currentCount++;
}
void Notice(MSG_ID MsgId, std::string MsgStr, PTAG_GAME_AC_LOGIN Client)
{
	ACEWarningFileLog("MsgStr:{}  msg_id:{}", MsgStr, int(MsgId));
	
	switch (MsgId)
	{
	case MSG_ID::MSG_STOPLOGIN_OUTNUMBER:
		break;
	case MSG_ID::MSG_START_GAME:
		qInfo() << "账号:[" << Client->pData->GameUserName .data()<< "]启动游戏";
		break;
	default:
		break;
	}

}
bool CsoStudioServer::init()
{
	VMProtectBegin(__FUNCTION__);
	
	m_RCF = new QtRCFServer(this);
	m_RCF->start();
	m_RCF->SetLoginInterface(&m_Login);
	
	auto h = LoadLibraryA("007Helper.dll");
	m_Login.Init(g_pConfig->GetGamePath() + "\\cstrike-online.exe", g_pConfig->GetGamePath(), Notice);


	m_Http = new HttpManage("http://127.0.0.1:8801");
	

	VMProtectEnd();
    return true;
}

bool CsoStudioServer::initMenu()
{
	//设置菜单选项 
	m_pMainAutionMenu   = new QMenu(this);

	//拍卖行任务_子项
	QAction* pAuctionTaskAdd            = new QAction("添加武器拍卖任务", m_pMainAutionMenu);
	QAction* pAuctionTaskModify         = new QAction("修改武器拍卖任务", m_pMainAutionMenu);
	QAction* pAuctionTaskDel            = new QAction("删除武器拍卖任务", m_pMainAutionMenu);
	QAction* pAuctionTaskDispatchAcc    = new QAction("添加派发任务账号", m_pMainAutionMenu);
	QAction* pAuctionDelTaskDispatchAcc = new QAction("删除派发任务账号", m_pMainAutionMenu);
	//子菜单_添加派发任务账号
	m_pChildMenuAddDispatchTask            = new	QMenu();
	//子菜单_删除派发任务账号
	m_pChildMenuDelDispatchTask            = new	QMenu();
	
	//账号是动态添加的
	pAuctionTaskDispatchAcc->setMenu(m_pChildMenuAddDispatchTask);
	pAuctionDelTaskDispatchAcc->setMenu(m_pChildMenuDelDispatchTask);

	m_pMainAutionMenu->addAction(pAuctionTaskAdd);
	m_pMainAutionMenu->addAction(pAuctionTaskModify);
	m_pMainAutionMenu->addAction(pAuctionTaskDel);
	m_pMainAutionMenu->addAction(pAuctionTaskDispatchAcc);
	m_pMainAutionMenu->addAction(pAuctionDelTaskDispatchAcc);


	connect(m_pChildMenuAddDispatchTask, &QMenu::triggered, this, &CsoStudioServer::slPopMenuDispatchTask);
	connect(m_pChildMenuDelDispatchTask, &QMenu::triggered, this, &CsoStudioServer::slDeleteDispatchAccount);
	connect(m_pMainAutionMenu, &QMenu::triggered, this, &CsoStudioServer::slPopMenuAutionTask);


	m_pMainAccountMenu = new QMenu(this);

	QAction* pAccountAdd                = new QAction("添加账号信息", m_pMainAccountMenu);
	QAction* pAccountModify             = new QAction("修改账号信息", m_pMainAccountMenu);
	QAction* pAccountDel                = new QAction("删除账号信息", m_pMainAccountMenu);
	QAction* pAccountBatchReg           = new QAction("批量修改大区", m_pMainAccountMenu);
	QAction* pAccountBatchTask          = new QAction("批量修改任务", m_pMainAccountMenu);
	QAction* pAccountWarehouse          = new QAction("查看仓库信息", m_pMainAccountMenu);
	QAction* pAccountEndProcess         = new QAction("结束游戏进程", m_pMainAccountMenu);
	QAction* pAccountGetAuctionMetadata = new QAction("查看拍卖会商品价格", m_pMainAccountMenu);
	
	//子菜单_修改大区
	QMenu * m_pChildMenuBatchReg = new QMenu();
	//子菜单_修改任务
	QMenu* m_pChildMenuGameTask  = new QMenu();
	
	QAction* pGameRegD1 = new QAction("电信一区", m_pChildMenuBatchReg);
	QAction* pGameRegD2 = new QAction("电信二区", m_pChildMenuBatchReg);
	QAction* pGameRegW1 = new QAction("网通一区", m_pChildMenuBatchReg);

	for (size_t i = 0; i < static_cast<int>(TASK_MODE::GAME_TASK_UNKONW); i++)
	{
		QAction* pGameTask = new QAction(QString::fromStdString(g_Util.GetTaskByIndex((TASK_MODE)i)->Text), m_pChildMenuGameTask);
		m_pChildMenuGameTask->addAction(pGameTask);
	}

	
	m_pChildMenuBatchReg->addAction(pGameRegD1);
	m_pChildMenuBatchReg->addAction(pGameRegD2);
	m_pChildMenuBatchReg->addAction(pGameRegW1);
	pAccountBatchReg->setMenu(m_pChildMenuBatchReg);
	pAccountBatchTask->setMenu(m_pChildMenuGameTask);
	
	connect(m_pChildMenuBatchReg, &QMenu::triggered, this, &CsoStudioServer::slPopMenuBatchReg);
	connect(m_pChildMenuGameTask, &QMenu::triggered, this, &CsoStudioServer::slPopMenuBatchTask);

	m_pMainAccountMenu->addAction(pAccountAdd);
	m_pMainAccountMenu->addAction(pAccountModify);
	m_pMainAccountMenu->addAction(pAccountDel);
	m_pMainAccountMenu->addAction(pAccountBatchReg);
	m_pMainAccountMenu->addAction(pAccountBatchTask);
	m_pMainAccountMenu->addAction(pAccountWarehouse);
	m_pMainAccountMenu->addAction(pAccountEndProcess);
	m_pMainAccountMenu->addAction(pAccountGetAuctionMetadata);
	
	connect(m_pMainAccountMenu, &QMenu::triggered, this, &CsoStudioServer::slPopMenuAccount);
	
	return  true;
}

void CsoStudioServer::RefreshAutionTaskView()
{

	ui.tableWidget_GameTaskAuction->clearContents();
	ui.tableWidget_GameTaskAuction->setRowCount(0);

	for (auto &iter : m_RCF->GetAuctionTask())
	{
		slAddAutionTask(iter.second);

	}

}

void CsoStudioServer::slImpUserInfo(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"请选择要导入的用户配置",
		"",
		"文本文件(*txt)");

	if (fileName.isEmpty())
	{
		//未选择导入的文件
		return;
	}
	qDebug()<< "导入用户账号密码文件,路径:" << fileName;

	QFile file;
	file.setFileName(fileName);


	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "警告", "打开文件失败!请检查是否有打开的权限！");
		return;
	}
	int nImpCount = 0;
	while (!file.atEnd())
	{
		char cBuf[1024];
		file.readLine(cBuf, sizeof(cBuf));

		QString str = cBuf;
		QStringList list = str.split("----");

		if (list.size() >= 2)
		{
			// 去除特殊字符.如空格换行符等
			QString	user     = list[0].simplified();
			QString password = list[1].simplified();

			//判断账号是否存在
			if (m_Login.SearchUserName(user.toLocal8Bit().data()))
			{
				qInfo() << "[账号批量导入]账号:" << user << "已经存在跳过";
				continue;
			}
			SE_GAME_AC_CLIENT* pLoginInfo = new SE_GAME_AC_CLIENT();
			pLoginInfo->GameUserName      = user.toLocal8Bit().data();
			pLoginInfo->GameUserPassWord  = password.toLocal8Bit().data();
			pLoginInfo->GameReg           = 6;
			pLoginInfo->UserTask          = TASK_MODE::GAME_TASK_AUCTION;
			auto result                   =  m_Login.AddLoginInfo(pLoginInfo);

			emit sig_AddShowUserInfo(result);
			
			qInfo() << "[账号批量导入]账号:" << user << "添加完毕";
			
			nImpCount++;

		}
	}
	file.close();

	if (nImpCount)
	{
		qInfo() << "[账号批量导入]一共导入:" << nImpCount << "个账号";
	}
}

void to_json(nlohmann::ordered_json& j, const TAG_PARSE_ACINFO& p)
{

	std::map<ULONG, PTAG_GAME_AC_LOGIN>* Task = (std::map<ULONG, PTAG_GAME_AC_LOGIN>*)(p.This);

	for (auto& iter : *Task)
	{
		PTAG_GAME_AC_LOGIN pUserInfo = iter.second;

		j[std::to_string(iter.first)] = {
			{"nID", pUserInfo->pData->nID},
			{"GameReg", pUserInfo->pData->GameReg},
			{"GameUserName", pUserInfo->pData->GameUserName},
			{"GameUserPassWord", pUserInfo->pData->GameUserPassWord},
			{"Game2ndpassword", pUserInfo->pData->Game2ndpassword},
			{"Lv", pUserInfo->pData->Lv},
			{"PlayerName", pUserInfo->pData->PlayerName},
			{"Mileage", pUserInfo->pData->Mileage},
			{"Gold", pUserInfo->pData->Gold},
			{"UserTask", pUserInfo->pData->UserTask},
			{"UserTaskStatus", pUserInfo->pData->UserTaskStatus},
			{"UserStatus", pUserInfo->pData->UserStatus},
		};
	}
}

void CsoStudioServer::slSaveUserCfgInfo(bool checked)
{
	auto& UserInfo = m_Login.GetMapLogin();

	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\UserInfo.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));

	auto pACEFile = std::make_shared<CACEFile>(PathItem);

	if (pACEFile->OpenFile(CREATE_ALWAYS) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);
		qInfo() << "保存账号配置失败";
		return;
	}
	TAG_PARSE_ACINFO v1(UserInfo);

	nlohmann::ordered_json  Info = v1;

	//重写
	pACEFile->SetFilePointer(FILE_BEGIN);

	auto info = Info.dump();

	pACEFile->WriteFile(info.data(), info.size());

	pACEFile->CloseFile();

	qInfo() << "保存账号配置成功";

}
void from_json(const nlohmann::json& j, PARSE_ACINFO& p)
{
	CsoStudioServer::GetInstance()->GetLogin().ClearLoginInfo();

	//未释放内存,
	for (auto& item : j.items())
	{
		auto vTaskID = item.key();
		auto vText = item.value();


		SE_GAME_AC_CLIENT* pLoginInfo = new SE_GAME_AC_CLIENT();
		pLoginInfo->GameUserName      = vText.at("GameUserName").get<std::string>();
		pLoginInfo->GameUserPassWord  = vText.at("GameUserPassWord").get<std::string>();
		pLoginInfo->GameReg           = vText.at("GameReg").get<ULONG>();
		pLoginInfo->Lv                = vText.at("Lv").get<ULONG>();
		pLoginInfo->Game2ndpassword   = vText.at("Game2ndpassword").get<std::string>();
		pLoginInfo->PlayerName        = vText.at("PlayerName").get<std::string>();
		pLoginInfo->Mileage           = vText.at("Mileage").get<ULONG64>();
		pLoginInfo->Gold              = vText.at("Gold").get<ULONG64>();
		pLoginInfo->UserTask          = static_cast<TASK_MODE>(vText.at("UserTask").get<ULONG>());
		pLoginInfo->UserTaskStatus    = vText.at("UserTaskStatus").get<BYTE>();
		pLoginInfo->UserStatus        = static_cast<USER_STATUS>(vText.at("UserStatus").get<ULONG>());
		
		


		auto result = CsoStudioServer::GetInstance()->GetLogin().AddLoginInfo(pLoginInfo);

	}

}
void CsoStudioServer::slLoadUserCfgInfo(bool checked)
{
	// open cfg file
	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\UserInfo.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));

	auto pACEFile = std::make_shared<CACEFile>(PathItem);

	if (pACEFile->OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);

		qInfo() << "读取配置失败";

		return;
	}
	// read cfg file
	auto  VecByte = pACEFile->ReadFile();
	nlohmann::json CFG = nlohmann::json::parse(VecByte.begin(), VecByte.end());
	std::map<ULONG, PTAG_GAME_AC_LOGIN>& Info = m_Login.GetMapLogin();
	CFG.get<PARSE_ACINFO>();

	//刷新界面
	RefreshAcView();

	
	qInfo() << "读取账号信息配置完毕！";
}

void CsoStudioServer::slDelUserCfgInfo(bool checked)
{
	if (QMessageBox::information(NULL, "Title", "你确定要删除保存的账号配置么?",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		CACEHook   ACEHook;
		auto PathItem = fmt::format(xorstr_("{}\\Config\\UserInfo.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));
		
		DeleteFileA(PathItem.data());

		qInfo()<<"删除账号配置成功";

	}
}

void CsoStudioServer::slExportUserInfo(bool checked)
{
	//账号信息导出

	std::string FileName;
	
	CACEFile ACEFile;




	[&]() {

		auto Path =  CACEInterface::GetInstance()->GetUtil()->UTF8_To_string(ACEFile.GetCurrentDirectoryA("账号导出"));
		CreateDirectoryA(Path.data(), NULL);

		SYSTEMTIME	szTime;
		GetLocalTime(&szTime);

		auto Name = QString("%1-%2-%3-%4-%5-%6.txt").arg(szTime.wYear).arg(szTime.wMonth).arg(szTime.wDay).arg(szTime.wHour).arg(szTime.wMinute).arg(szTime.wSecond);

		FileName = fmt::format("{}\\{}",Path, Name.toLocal8Bit().constData());

		CloseHandle(CreateFileA(FileName.data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL));

		
	}();

	QFile file;
	file.setFileName(QString::fromLocal8Bit(FileName.data()));


	if (file.open(QIODevice::WriteOnly))
	{

		QString	StrSumMileage;

		ULONG64	nSumMileage = 0;

		auto pLoginInfo = m_Login.GetMapLogin();

		for (auto &iter :pLoginInfo)
		{
			PTAG_GAME_AC_LOGIN accountInfo = iter.second;


			QString	ExportStrDate;

			ExportStrDate.append("ID:");
			ExportStrDate.append(QString::number(iter.first));
			ExportStrDate.append("--");
			
			ExportStrDate.append("游戏大区:");
			ExportStrDate.append(g_Util.GetServerRegionByIndexToStr(accountInfo->pData->GameReg).data());
			ExportStrDate.append("--");
			
			ExportStrDate.append("账号:");
			ExportStrDate.append(accountInfo->pData->GameUserName.data());
			ExportStrDate.append("--");

			ExportStrDate.append("密码:");
			ExportStrDate.append(accountInfo->pData->GameUserPassWord.data());
			ExportStrDate.append("--");
			
			ExportStrDate.append("等级:");
			ExportStrDate.append(QString::number(accountInfo->pData->Lv));
			ExportStrDate.append("--");

			ExportStrDate.append("角色名:");
			ExportStrDate.append(accountInfo->pData->PlayerName.data());
			ExportStrDate.append("--");

			ExportStrDate.append("积分:");
			ExportStrDate.append(QString("%L1").arg(accountInfo->pData->Mileage));
			ExportStrDate.append("--");

			ExportStrDate.append("仓库密码:");
			ExportStrDate.append(accountInfo->pData->Game2ndpassword.data());
			ExportStrDate.append("--");

			ExportStrDate.append("\r\n");
			file.write(ExportStrDate.toUtf8());

			//统计积分信息
			nSumMileage += accountInfo->pData->Mileage;
		}

		
		StrSumMileage.append("\r\n");
		StrSumMileage.append("---------------------------");
		StrSumMileage.append("总积分:");
		StrSumMileage.append(QString("%L1").arg(nSumMileage));
		StrSumMileage.append("\r\n");
		StrSumMileage.append("---------------------------");

		file.write(StrSumMileage.toUtf8());

	}
	file.close();

	QMessageBox::information(this,"", "导出完毕！");

	QString filePath = "file:///" + file.fileName();

	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
void CsoStudioServer::slAddShowGameUserInfo(PTAG_GAME_AC_LOGIN AcInfo)
{
	int rowcount = AcInfo->ID;

	if (rowcount >= ui.tableWidget_GameMonitor->rowCount())
	{
		ui.tableWidget_GameMonitor->insertRow(rowcount);
	}
	
	int CountRow = 0;

	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(g_Util.GetServerRegionByIndexToStr(AcInfo->pData->GameReg).data()));


	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->GameUserName.data()));
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->GameUserPassWord.data()));
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AcInfo->pData->Lv)));
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->PlayerName.data()));
	
	//转为金额方式
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString("%L1").arg(AcInfo->pData->Mileage)));
	//金币
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString("%L1").arg(AcInfo->pData->Gold)));
	//仓库密码
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->Game2ndpassword.data()));

	auto UserStatus = g_Util.GetUserStatusByIndex(AcInfo->pData->UserStatus);


	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(UserStatus->Text.data()));


	auto TaskText = g_Util.GetTaskByIndex(AcInfo->pData->UserTask);

	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(TaskText->Text.data()));
	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->UserTaskStatus == 1 ? "停止" :"正常"));


	//时间戳转时间
	QDateTime time = QDateTime::fromTime_t(AcInfo->ulTimeStamp); //时间戳非毫秒级
	//QDateTime dtime = QDateTime::fromMSecsSinceEpoch(AcInfo.ac_timestamp.toULongLong()); //时间戳-毫秒级

	ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(time.toString("MM-dd hh:mm:ss")));

	ui.tableWidget_GameMonitor->show();

	bool bStatus = false;
	for (auto &iter : m_UserList)
	{
		if (iter->ID == AcInfo->ID)
		{
			iter = AcInfo;
			bStatus = true;
			break;
		}
	}

	if (bStatus == false)
	{
		m_UserList.push_back(AcInfo);

		//动态添加账号
		QAction* pAcTask_Index = new	QAction(AcInfo->pData->GameUserName.data(), m_pChildMenuAddDispatchTask);
		m_pChildMenuAddDispatchTask->addAction(pAcTask_Index);
	}


}

void CsoStudioServer::slStartGame(bool checked)
{
	qDebug() << checked;

	if (checked)
	{
		m_Login.StartProcess();
	}
	else
	{
		m_Login.StopLogin(FALSE);
	}





}

void CsoStudioServer::slRightClickAuctionMmenu(const QPoint pos)
{
	m_pMainAutionMenu->move(cursor().pos());
	m_pMainAutionMenu->show();



	//获得鼠标点击的x，y坐标点 
	int x = pos.x();
	int y = pos.y();
	QModelIndex index= ui.tableWidget_GameTaskAuction->indexAt(QPoint(x, y));
	int row = index.row();//获得QTableWidget列表点击的行数 

	m_VecSelectedTaskIndex.clear();

	QItemSelectionModel* selections = ui.tableWidget_GameTaskAuction->selectionModel(); //返回当前的选择模式  
	QModelIndexList selectedsList = selections->selectedIndexes(); //返回所有选定的模型项目索引列表  

	for (int i = 0; i < selectedsList.count(); i++)
	{
		m_VecSelectedTaskIndex.push_back(selectedsList.at(i).row());
	}
	std::sort(m_VecSelectedTaskIndex.begin(), m_VecSelectedTaskIndex.end());
	///*
	//* 该函数的作用是“去除”容器或者数组中相邻元素的重复出现的元素，注意
	//(1) 这里的去除并非真正意义的erase，而是将重复的元素放到容器的末尾，返回值是去重之后的尾地址。
	//(2) unique针对的是相邻元素，所以对于顺序顺序错乱的数组成员，或者容器成员，需要先进行排序，可以调用std::sort()函数
	//*/
	m_VecSelectedTaskIndex.erase(std::unique(m_VecSelectedTaskIndex.begin(), m_VecSelectedTaskIndex.end()), m_VecSelectedTaskIndex.end());




	//-------------------------------------------------------------------
	// 动态添加 删除派发账号
	//-------------------------------------------------------------------
	m_pChildMenuDelDispatchTask->clear();
	if (m_VecSelectedTaskIndex.size() == 1)
	{
		//get ui task id
		auto uiTaskID = ui.tableWidget_GameTaskAuction->item(m_VecSelectedTaskIndex.at(0), 0);
		ULONG uTaskID = uiTaskID->text().toInt();

		if (m_RCF->GetAuctionTask().count(uTaskID) != 0)
		{
			auto CurSelectedTask = m_RCF->GetAuctionTask().at(uTaskID);

			for (size_t i = 0; i < CurSelectedTask->ACIDCount; i++)
			{
				QAction* pAcTaskDelDispatch = new	QAction(CurSelectedTask->szACArray[i], m_pChildMenuDelDispatchTask);
				m_pChildMenuDelDispatchTask->addAction(pAcTaskDelDispatch);
			}
		}
		
	}
	




}

void CsoStudioServer::slPopMenuAutionTask(QAction* act)
{
	qDebug() << act->text();

	if (act->text() == "添加武器拍卖任务")
	{
		popMenuCreateAutionDlg();
	}	
	else if (act->text() == "修改武器拍卖任务")
	{
		popMenuChangeAution();
	}
	else if (act->text() == "删除武器拍卖任务")
	{
		popMenuDelAution();
	}

	
}

void CsoStudioServer::slAddAutionTask(PTAG_GAME_TASK_AUCTION AuctionTask)
{
	qDebug() << __FUNCTION__;


	//添加到map
	PTAG_GAME_TASK_AUCTION pInfo = nullptr;

	if (m_RCF->GetAuctionTask().count(AuctionTask->ulTaskID))
	{
		pInfo = m_RCF->GetAuctionTask().at(AuctionTask->ulTaskID);
		memcpy(pInfo, AuctionTask, sizeof(TAG_GAME_TASK_AUCTION));
	
	}
	else
	{
		pInfo = new TAG_GAME_TASK_AUCTION();
		memcpy(pInfo, AuctionTask, sizeof(TAG_GAME_TASK_AUCTION));
		m_RCF->GetAuctionTask().insert(std::map<ULONG, PTAG_GAME_TASK_AUCTION>::value_type(pInfo->ulTaskID, pInfo));

	}

	int rowcount = AuctionTask->ulTaskID;

	if (rowcount >= ui.tableWidget_GameTaskAuction->rowCount())
	{
		ui.tableWidget_GameTaskAuction->insertRow(rowcount);

	}







	int CountRow = 0;

	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AuctionTask->ulTaskID)));

	auto TaskTypeText = [&]()->QString 
	{
		QString Text;

		switch (AuctionTask->ulTaskType)
		{
		case 0:
			Text = "搜索指定武器";
			break;
		case 1:
			Text = "搜索随机武器";
			break;
		case 2:
			Text = "转武器-商家";
			break;
		case 3:
			Text = "转武器-客户";
			break;
		default:
			break;
		}
		return Text;
	};
	auto PaintTypeText = [&]()->QString
	{
		QString Text;

		switch (AuctionTask->PaintType)
		{
		case 0:
			Text = "不需要彩漆";
			break;
		case 1:
			Text = "符合条件的彩漆";
			break;
		case 2:
			Text = "有彩漆就符合";
			break;
		default:
			break;
		}
		return Text;
	};
	
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(TaskTypeText()));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(g_Util.GetItemNameByID(AuctionTask->ItemID).data()));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AuctionTask->Price)));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AuctionTask->BuyCount)));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(PaintTypeText()));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AuctionTask->PaintCount)));

	//有几个就加入几个彩漆
	auto PaintText = [&]()->QString
	{
		QString Text;
		
		std::map<ULONG, std::string> PaintMap = g_Util.GetPaintMap();


		for (size_t i = 0; i < AuctionTask->PaintCount; i++)
		{
			if (PaintMap.count(AuctionTask->PaintID[i]))
			{
				Text.append(PaintMap.at(AuctionTask->PaintID[i]).data());

				if (i < AuctionTask->PaintCount -1)
				{
					Text.append(",");
				}
			}

		}
		

		return Text;
	};

	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(PaintText()));

	auto EnchantText = [&]()->QString
	{
		QString Text;

		switch (AuctionTask->ItemAttribute)
		{
		case 0:
			Text = "不限制";
			break;
		case 1:
			Text = "必须符合条件";
			break;
		default:
			break;
		}
		return Text;
	};
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AuctionTask->Enchantlv)));
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(EnchantText()));
	
	auto AttributeText = [&]()->QString
	{
		QString Text;
		
		if (AuctionTask->HitRate)
		{
			Text.append("命中:");
			Text.append(QString::number(AuctionTask->HitRate));
		}
		if (AuctionTask->Damage)
		{
			Text.append("伤害:");
			Text.append(QString::number(AuctionTask->Damage));
		}
		if (AuctionTask->Recoil)
		{
			Text.append("后座:");
			Text.append(QString::number(AuctionTask->Recoil));
		}
		if (AuctionTask->Weight)
		{
			Text.append("重量:");
			Text.append(QString::number(AuctionTask->Weight));
		}
		if (AuctionTask->Bursts)
		{
			Text.append("连发:");
			Text.append(QString::number(AuctionTask->Bursts));
		}
		if (AuctionTask->Clip)
		{
			Text.append("弹夹:");
			Text.append(QString::number(AuctionTask->Clip));
		}

		return Text;
	};

	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(AttributeText()));

	
	auto TaskStatus = [&]()->QString
	{
		QString Text;

		switch (AuctionTask->TaskStatus)
		{
		case 0:
			Text = "任务未完成";
			break;
		case 1:
			Text = "任务已完成";
			break;
		case 2:
			Text = "任务已删除";
			break;
		default:
			break;
		}
		return Text;
	};

	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(TaskStatus()));

	QString TextUser;
	
	// 判断派发的账号是否存在

	/*for (auto& iter : m_Login.GetMapLogin())
	{
		for (size_t i = 0; i < AuctionTask->ACIDCount; i++)
		{
			if (iter.second->pData->GameUserName == AuctionTask->szACArray[i])
			{
				TextUser.append(iter.second->pData->GameUserName.data());
				TextUser.append(",");

			}

		}

	}*/

	//-------------------------------------------------------------------
	// 2022-06-26 17:55:18 更新为不需要判断账号是否存在.直接显示到界面上
	// 因为读取配置要显示,如果没添加账号.会无法显示派发的账号的
	//-------------------------------------------------------------------
	for (size_t i = 0; i < AuctionTask->ACIDCount; i++)
	{
		TextUser.append(AuctionTask->szACArray[i]);
		TextUser.append(",");
	}

	if (TextUser.isEmpty() == false)
	{
		//末尾删除
		TextUser.chop(1);
	}
	ui.tableWidget_GameTaskAuction->setItem(rowcount, CountRow++, new QTableWidgetItem(TextUser));

}

void CsoStudioServer::slDoubleClickAuction(const QModelIndex& index)
{
	ULONG uTaskID = index.row();

	if (m_RCF->GetAuctionTask().count(uTaskID) == 0)
	{
		qInfo() << "未找到任务ID";
		return;
	}
	m_pDlgAution->setWindowModality(Qt::ApplicationModal);
	m_pDlgAution->SetTypeHandle(2);
	m_pDlgAution->SetTaskInfo(m_RCF->GetAuctionTask().at(uTaskID));
	m_pDlgAution->show();
}

void CsoStudioServer::slRefreshUserView(qint64 qID)
{

	if (m_Login.GetMapLogin().count(qID))
	{
		auto Info =  m_Login.GetMapLogin().at(qID);
		modifyingUserInfoView(Info);
	}
}

void CsoStudioServer::slRefreshAuctionView(qint64 qID)
{
	ULONG uTaskID = qID;

	if (m_RCF->GetAuctionTask().count(uTaskID) == 0)
	{
		qDebug() << "未找到任务ID";
		
		return;
	}
	slAddAutionTask(m_RCF->GetAuctionTask().at(uTaskID));
}

void CsoStudioServer::slRefreshAuctionView()
{
	//清空界面
	ui.tableWidget_GameTaskAuction->clearContents();

	for (auto& iter : m_RCF->GetAuctionTask())
	{
		emit sig_RefreshAuctionUI(iter.first);
	}
}

void CsoStudioServer::slOpenWeaponItemFile(bool checked)
{
	auto Funs = []()->void
	{
		CACEHook   ACEHook;
		auto FileCmdLine = fmt::format(xorstr_("notepad.exe {}\\Config\\Item.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));
		::system(FileCmdLine.data());
	};
	std::thread th(Funs);
	th.detach();
}

void CsoStudioServer::slOpenPaintItemFile(bool checked)
{
	auto Funs = []()->void
	{
		CACEHook   ACEHook;
		auto FileCmdLine = fmt::format(xorstr_("notepad.exe {}\\Config\\WeaponPaintItem.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));
		::system(FileCmdLine.data());
	};
	std::thread th(Funs);
	th.detach();
}

void CsoStudioServer::slRightClickAccountMmenu(const QPoint pos)
{
	
	m_pMainAccountMenu->move(cursor().pos());
	m_pMainAccountMenu->show();

	//获得鼠标点击的x，y坐标点 
	int x = pos.x();
	int y = pos.y();
	QModelIndex index = ui.tableWidget_GameMonitor->indexAt(QPoint(x, y));
	int row = index.row();

	m_VecSelectedAccountIndex.clear();

	QItemSelectionModel* selections = ui.tableWidget_GameMonitor->selectionModel(); 
	QModelIndexList selectedsList = selections->selectedIndexes(); 

	for (int i = 0; i < selectedsList.count(); i++)
	{
		m_VecSelectedAccountIndex.push_back(selectedsList.at(i).row());
	}
	std::sort(m_VecSelectedAccountIndex.begin(), m_VecSelectedAccountIndex.end());
	m_VecSelectedAccountIndex.erase(std::unique(m_VecSelectedAccountIndex.begin(), m_VecSelectedAccountIndex.end()), m_VecSelectedAccountIndex.end());




	//-------------------------------------------------------------------
	// 动态添加 删除派发账号
	//-------------------------------------------------------------------
	//m_pChildMenuDelDispatchTask->clear();
	//if (m_VecSelectedTaskIndex.size() == 1)
	//{
	//	//get ui task id
	//	auto uiTaskID = ui.tableWidget_GameTaskAuction->item(m_VecSelectedTaskIndex.at(0), 0);
	//	ULONG uTaskID = uiTaskID->text().toInt();

	//	if (m_RCF->GetAuctionTask().count(uTaskID) != 0)
	//	{
	//		auto CurSelectedTask = m_RCF->GetAuctionTask().at(uTaskID);

	//		for (size_t i = 0; i < CurSelectedTask->ACIDCount; i++)
	//		{
	//			QAction* pAcTaskDelDispatch = new	QAction(CurSelectedTask->szACArray[i], m_pChildMenuDelDispatchTask);
	//			m_pChildMenuDelDispatchTask->addAction(pAcTaskDelDispatch);
	//		}
	//	}

	//}


}

void CsoStudioServer::slPopMenuAccount(QAction* act)
{

	if (act->text() == "添加账号信息")
	{
		popMenuCreateAccountDlg();
	}
	else if (act->text() == "修改账号信息")
	{
		popMenuChangeAccountDlg();
	}
	else if (act->text() == "删除账号信息")
	{
		popMenuDelAccountDlg();
	}
	else if (act->text() == "结束游戏进程")
	{
		popMenuEndProcessAccountDlg();
	}
	else if (act->text() == "查看拍卖会商品价格")
	{
		popMenuGetAuctionMetadata();
	}
	
}

void CsoStudioServer::slDoubleClickAccount(const QModelIndex& index)
{
	auto uiItem = ui.tableWidget_GameMonitor->item(index.row(), 1);
	auto UserName = uiItem->text();

	auto pAcountInfo = m_Login.SearchUserName(UserName.toLocal8Bit().constData());

	if (pAcountInfo == nullptr)
	{
		qInfo() << "未找到账号ID";
		return;
	}

	m_pDlgAccount->setWindowModality(Qt::ApplicationModal);
	m_pDlgAccount->SetTypeHandle(2);
	m_pDlgAccount->SetAccountInfo(pAcountInfo);
	m_pDlgAccount->show();
}

void CsoStudioServer::slPopMenuBatchReg(QAction* act)
{

	for (std::vector<int>::iterator itr = m_VecSelectedAccountIndex.begin(); itr != m_VecSelectedAccountIndex.end(); ++itr)
	{
		//取当前行的数据
		QTableWidgetItem* cellItem_GameUser = ui.tableWidget_GameMonitor->item((*itr), 1); 
		
		if (cellItem_GameUser)
		{
			auto AcInfo = m_Login.SearchUserName(cellItem_GameUser->text().toLocal8Bit().constData());

			if (AcInfo == nullptr)
			{
				qInfo() << "修改失败.未找到账号信息 :" << cellItem_GameUser->text();
				continue;
			}
			
			if (act->text() == "电信一区")
			{
				AcInfo->pData->GameReg = 5;
			}
			else if (act->text() == "电信二区")
			{
				AcInfo->pData->GameReg = 6;
			}
			else if (act->text() == "网通一区")
			{
				AcInfo->pData->GameReg = 15;
			}
			emit sig_AddShowUserInfo(AcInfo);
		}
	}

	
}

void CsoStudioServer::slPopMenuBatchTask(QAction* act)
{

	for (std::vector<int>::iterator itr = m_VecSelectedAccountIndex.begin(); itr != m_VecSelectedAccountIndex.end(); ++itr)
	{
		//取当前行的数据
		QTableWidgetItem* cellItem_GameUser = ui.tableWidget_GameMonitor->item((*itr), 1);

		if (cellItem_GameUser)
		{
			auto AcInfo = m_Login.SearchUserName(cellItem_GameUser->text().toLocal8Bit().constData());

			if (AcInfo == nullptr)
			{
				qInfo() << "修改任务失败.未找到账号信息 :" << cellItem_GameUser->text();
				continue;
			}

			auto Result = g_Util.GetTaskByName(act->text().toStdString());

			AcInfo->pData->UserTask = Result->Mode;

			emit sig_AddShowUserInfo(AcInfo);
		}
	}


}

void CsoStudioServer::slAutoAuction1000M(bool checked)
{
	g_pConfig->SetAutoAuction1000M(checked);
	m_Login.GetGlobalShare().AutoJoinAuctionEvent = checked;

	if (checked)
	{
		qInfo() << "游戏启动前,生效！";
	}
}

void CsoStudioServer::slSetGameDir(bool checked)
{
	QString Dirpath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);

	if (Dirpath.isEmpty())
	{
		return;
	}

	auto GamePath = fmt::format("{}\\cstrike-online.exe", QDir::toNativeSeparators(Dirpath).toStdString());
	
	CACEFile File;
	File.SetFileName(CACEInterface::GetInstance()->GetUtil()->UTF8_To_string(GamePath));

	if (File.OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		QMessageBox::information(this, "", "游戏目录安装选择错误,请重新选择");
		return;
	}
	File.CloseFile();

	g_pConfig->SetGamePath(QDir::toNativeSeparators(Dirpath).toStdString());
	ui.lineEdit_GamePath->setText(Dirpath);
}

void CsoStudioServer::slLogin(bool checked)
{
	VMProtectBegin(__FUNCTION__);

	if (!g_FreeYun.Login())
	{
		MessageBoxW(0, g_FreeYun.GetErrorStr().data(), 0, MB_ICONSTOP);
		TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);
		return ;
	}
	if (!g_FreeYun.CheckUpdata())
	{
		MessageBoxW(0, g_FreeYun.GetErrorStr().data(), 0, MB_ICONSTOP);
		TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);
		return ;
	}
	std::string Card = ui.lineEdit_AgentUser->text().toLocal8Bit().constData();
	if (m_Http->Login(Card) == false)
	{
		QMessageBox::critical(this, "错误", m_Http->GetError().c_str());
		//TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);
		return ;
	}

	qInfo() << g_pConfig->GetAgentUser().data() << "登陆成功";

	

	//保存卡号
	g_pConfig->SetAgentUser(Card);

	m_bIsLogin = true;

	ui.lineEdit_AgentUser->setEnabled(false);
	ui.pushButton_Login->setEnabled(false);

	ui.tabWidget->setCurrentIndex(1);

	VMProtectEnd();
}

void CsoStudioServer::slRefreshAuctionOnSaleView()
{
	ULONG RowCount = 0;
	
	//清空界面
	ui.tableWidget_RackingItem->clearContents();
	

	for (auto& iter :m_Login.GetMapLogin())
	{
		PTAG_GAME_AC_LOGIN pInfo = iter.second;
		
		for (auto & iterItem : pInfo->pData->AuctionRackingItems)
		{
			PTAG_GAME_AUCTION pAuction = &(iterItem.second);
			int CountRow               = 0;
			
			if (pAuction->ItemID == 0 && pAuction->AddedTimer == 0)
			{
				//商品下架
				continue;
			}

			if (RowCount >= ui.tableWidget_RackingItem->rowCount())
			{
				ui.tableWidget_RackingItem->insertRow(RowCount);
			}

			ui.tableWidget_RackingItem->setItem(RowCount, CountRow++, new QTableWidgetItem(QString(pInfo->pData->GameUserName.data())));
			ui.tableWidget_RackingItem->setItem(RowCount, CountRow++, new QTableWidgetItem(QString::number(pAuction->Row)));
			ui.tableWidget_RackingItem->setItem(RowCount, CountRow++, new QTableWidgetItem(g_Util.GetItemNameByID(pAuction->ItemID).data()));
			ui.tableWidget_RackingItem->setItem(RowCount, CountRow++, new QTableWidgetItem(CACEInterface::GetInstance()->GetUtil()->TimeStampToStr(pAuction->AddedTimer).data()));
			ui.tableWidget_RackingItem->setItem(RowCount, CountRow++, new QTableWidgetItem(QString("%L1").arg(pAuction->Price)));

			RowCount++;
		}
	}

	ui.tableWidget_RackingItem->show();

}

void CsoStudioServer::slPopMenuDispatchTask(QAction* act)
{

	if (m_VecSelectedTaskIndex.empty())
	{
		qInfo() << "未选中任务ID,请先选中一个ID";
		return;
	}

	if (m_VecSelectedTaskIndex.size() > 1)
	{
		qInfo() << "只能选中一个任务ID,请先选中一个任务ID";
		return;
	}
	//get ui task id
	auto uiTaskID = ui.tableWidget_GameTaskAuction->item(m_VecSelectedTaskIndex.at(0), 0);
	ULONG uTaskID = uiTaskID->text().toInt();

	if (m_RCF->GetAuctionTask().count(uTaskID) == 0)
	{
		qInfo() << "未找到任务ID";
		return;
	}
	
	m_CurSelectedTask = m_RCF->GetAuctionTask().at(uTaskID);


	if (m_CurSelectedTask->TaskStatus == 2)
	{
		qInfo() << "任务状态:已删除.不支持派发账号";
		return;
	}


	for (auto & iter : m_UserList)
	{
		if (iter->pData->GameUserName == act->text().toLocal8Bit().constData())
		{
			bool isExist = false;

			//check 重复派发

			for (size_t i = 0; i < m_CurSelectedTask->ACIDCount; i++)
			{
				if (iter->pData->GameUserName == m_CurSelectedTask->szACArray[i])
				{
					isExist = true;
					qInfo() << "账号已存在,请勿重复添加";
					break;
				}

			}

			if (isExist == false)
			{
				strcpy(m_CurSelectedTask->szACArray[m_CurSelectedTask->ACIDCount++], iter->pData->GameUserName.data());
				slAddAutionTask(m_CurSelectedTask);
			}

			break;
		}
	}

	



}

void CsoStudioServer::slDeleteDispatchAccount(QAction* act)
{

	if (m_VecSelectedTaskIndex.empty())
	{
		qInfo() << "未选中任务ID,请先选中一个ID";
		return;
	}

	if (m_VecSelectedTaskIndex.size() > 1)
	{
		qInfo() << "只能选中一个任务ID,请先选中一个任务ID";
		return;
	}
	//get ui task id
	auto uiTaskID = ui.tableWidget_GameTaskAuction->item(m_VecSelectedTaskIndex.at(0), 0);
	ULONG uTaskID = uiTaskID->text().toInt();

	if (m_RCF->GetAuctionTask().count(uTaskID) == 0)
	{
		qInfo() << "未找到任务ID";
		return;
	}
	m_CurSelectedTask = m_RCF->GetAuctionTask().at(uTaskID);
	if (m_CurSelectedTask->TaskStatus == 2)
	{
		qInfo() << "任务状态:已删除.不支持删除派发账号";
		return;
	}

	// enum szACArray
	for (size_t i = 0; i < m_CurSelectedTask->ACIDCount; i++)
	{
		if (lstrcmpiA(m_CurSelectedTask->szACArray[i], act->text().toLocal8Bit().constData()) == 0)
		{
			RtlZeroMemory(m_CurSelectedTask->szACArray[i],MAX_PATH);
			m_CurSelectedTask->ACIDCount--;
			slAddAutionTask(m_CurSelectedTask);
			break;
		}
	}


}



void to_json(nlohmann::ordered_json& j, const TAG_PARSE_AUTICON& p)
{
	
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>* Task = (std::map<ULONG, PTAG_GAME_TASK_AUCTION>*)(p.This);

	for (auto& iter : *Task)
	{
		PTAG_GAME_TASK_AUCTION pTask = iter.second;

		j[std::to_string(iter.first)] = { 
			{"ulTaskID"        , pTask->ulTaskID}, 
			{"ulTaskType"      ,pTask->ulTaskType},
			{"ItemID"          ,pTask->ItemID},
			{"Price"           ,pTask->Price},
			{"BuyCount"        ,pTask->BuyCount},
			{"PaintType"       ,pTask->PaintType},
			{"PaintCount"      ,pTask->PaintCount},
			{"PaintID"         ,pTask->PaintID},
			{"Enchantlv"       ,pTask->Enchantlv},
			{"ItemAttribute"   ,pTask->ItemAttribute},
			{"HitRate"         ,pTask->HitRate},
			{"Damage"          ,pTask->Damage},
			{"Recoil"          ,pTask->Recoil},
			{"Weight"          ,pTask->Weight},
			{"Bursts"          ,pTask->Bursts},
			{"Clip"            ,pTask->Clip},
			{"TaskStatus"      ,pTask->TaskStatus},
			{"ACIDCount"       ,pTask->ACIDCount},
			{"szACArray"       ,pTask->szACArray},
			{"BlackListItem"   ,pTask->BlackListItem},
			{"BlackListItemID" ,pTask->BlackListItemID},
			{"BlackListPaint"  ,pTask->BlackListPaint},
			{"BlackListPaintID",pTask->BlackListPaintID},

			{"FindType"        ,pTask->FindType},
		};
	}
}



void CsoStudioServer::slSavaAuctionCFG(bool checked)
{
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& AuctionTask = m_RCF->GetAuctionTask();

	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\AuctionTask.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));

	auto pACEFile = std::make_shared<CACEFile>(PathItem);

	if (pACEFile->OpenFile(CREATE_ALWAYS) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);

		qInfo() << "保存配置失败";
		
		return;
	}
	TAG_PARSE_AUTICON v1(AuctionTask);
	
	nlohmann::ordered_json  Info = v1;

	//重写
	pACEFile->SetFilePointer(FILE_BEGIN);

	auto info = Info.dump();
	
	pACEFile->WriteFile(info.data(), info.size());

	pACEFile->CloseFile();


	qInfo() << "保存配置成功";

}
void from_json(const nlohmann::json & j, PARSE_AUTICON & p)
{
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& Task = CsoStudioServer::GetInstance()->GetRCF()->GetAuctionTask();

	Task.clear();

	//未释放内存,
	
	for (auto& item : j.items())
	{
		auto vTaskID = item.key();
		auto vText   = item.value();
		
		PTAG_GAME_TASK_AUCTION pTask = new TAG_GAME_TASK_AUCTION();
		pTask->ulTaskID              = vText.at("ulTaskID").get<ULONG>();
		pTask->ulTaskType            = vText.at("ulTaskType").get<ULONG>();
		pTask->ItemID                = vText.at("ItemID").get<ULONG>();
		pTask->Price                 = vText.at("Price").get<ULONG>();
		pTask->BuyCount              = vText.at("BuyCount").get<ULONG>();
		pTask->PaintType             = vText.at("PaintType").get<ULONG>();
		pTask->PaintCount            = vText.at("PaintCount").get<ULONG>();
		vText.at("PaintID").get_to(pTask->PaintID);
		pTask->Enchantlv             = vText.at("Enchantlv").get<ULONG>();
		pTask->ItemAttribute         = vText.at("ItemAttribute").get<ULONG>();
		pTask->HitRate               = vText.at("HitRate").get<BYTE>();
		pTask->Damage                = vText.at("Damage").get<BYTE>();
		pTask->Recoil                = vText.at("Recoil").get<BYTE>();
		pTask->Weight                = vText.at("Weight").get<BYTE>();
		pTask->Bursts                = vText.at("Bursts").get<BYTE>();
		pTask->Clip                  = vText.at("Clip").get<BYTE>();
		pTask->TaskStatus            = vText.at("TaskStatus").get<BYTE>();
		pTask->ACIDCount             = vText.at("ACIDCount").get<BYTE>();
		std::vector<std::string> ArrayAc;
		vText.at("szACArray").get_to(ArrayAc);

		for (size_t i = 0; i < pTask->ACIDCount; i++)
		{
			lstrcpyA(pTask->szACArray[i], ArrayAc[i].c_str());
		}
		pTask->BlackListItem  = vText.at("BlackListItem").get<BYTE>();
		vText.at("BlackListItemID").get_to(pTask->BlackListItemID);
		pTask->BlackListPaint = vText.at("BlackListPaint").get<BYTE>();
		vText.at("BlackListPaintID").get_to(pTask->BlackListPaintID);
	
		pTask->FindType       = vText.at("FindType").get<BYTE>();

		Task.insert(std::pair(pTask->ulTaskID, pTask));
	}

}
void CsoStudioServer::slReadAuctionCFG(bool checked)
{
	// open cfg file
	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\AuctionTask.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));
	
	auto pACEFile = std::make_shared<CACEFile>(PathItem);
	
	if (pACEFile->OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);
		
		qInfo() << "读取配置失败";

		return;
	}
	// read cfg file
	auto  VecByte = pACEFile->ReadFile();
	nlohmann::json AutionCFG = nlohmann::json::parse(VecByte.begin(), VecByte.end());	
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& AuctionTask = m_RCF->GetAuctionTask();
	AutionCFG.get<PARSE_AUTICON>();

	//刷新界面
	slRefreshAuctionView();
	
	qInfo() << "读取拍卖会配置完毕！";
}

void CsoStudioServer::slDeleteAuctionCFG(bool checked)
{
	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\AuctionTask.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));

	qInfo() << "拍卖会配置删除结果:" << DeleteFileA(PathItem.data());
	
}

bool CsoStudioServer::nativeEvent(const QByteArray& eventType, void* message, long* result)
{

	if (eventType == "windows_generic_MSG") //windows平台
	{
		MSG* msg = reinterpret_cast<MSG*>(message);

		if (msg->message == WM_COPYDATA)//消息类型
		{

			COPYDATASTRUCT* data = reinterpret_cast<COPYDATASTRUCT*>(msg->lParam);

			auto lPackRead = std::make_shared<PackReader>((BYTE*)data->lpData, data->cbData);

			int nType = lPackRead->ReadChar();


			switch (nType)
			{
			case 1:
			{
				//刷新用户界面
				ULONG uID = lPackRead->ReadInt();
				emit sig_RefreshUserUI(uID);
				break;
			}
			case 2:
			{
				//刷新拍卖行界面
				ULONG uID = lPackRead->ReadInt();

				emit sig_RefreshAuctionUI(uID);
				break;
			}
			case 3:
			{
				//刷新拍卖行上架界面
				emit sig_RefreshAuctionTaskUI();
				break;
			}
			case 4:
			{
				//通知符合的客户端任务
				
				m_RCF->SendClientAuctionSearchTask();




				break;
			}
			default:
				break;
			}


		}
	}

	return QWidget::nativeEvent(eventType, message, result);//交给Qt处理
}

void CsoStudioServer::modifyingUserInfoView(PTAG_GAME_AC_LOGIN AcInfo)
{
	QTableWidgetItem* cellItem = nullptr;

	for (int i = 0; i < ui.tableWidget_GameMonitor->rowCount(); i++) //逐行处理
	{
		for (int j = 0; j < ui.tableWidget_GameMonitor->columnCount(); j++) //逐列处理，
		{
			cellItem = ui.tableWidget_GameMonitor->item(i, j); //获取单元格的item

			if (cellItem && j == 1)
			{
				if (cellItem->text() == AcInfo->pData->GameUserName.data())
				{
					int rowcount = i;
					int CountRow = 0;

					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(g_Util.GetServerRegionByIndexToStr(AcInfo->pData->GameReg).data()));
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->GameUserName.data()));
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->GameUserPassWord.data()));
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString::number(AcInfo->pData->Lv)));
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->PlayerName.data()));
					//转为金额方式
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString("%L1").arg(AcInfo->pData->Mileage)));
					//金币
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(QString("%L1").arg(AcInfo->pData->Gold)));
					//仓库密码
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->Game2ndpassword.data()));
					auto UserStatus = g_Util.GetUserStatusByIndex(AcInfo->pData->UserStatus);
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(UserStatus->Text.data()));
					auto TaskText = g_Util.GetTaskByIndex(AcInfo->pData->UserTask);
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(TaskText->Text.data()));
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(AcInfo->pData->UserTaskStatus == 1 ? "停止" : "正常"));
					//时间戳转时间
					QDateTime time = QDateTime::fromTime_t(AcInfo->ulTimeStamp); //时间戳非毫秒级
					ui.tableWidget_GameMonitor->setItem(rowcount, CountRow++, new QTableWidgetItem(time.toString("MM-dd hh:mm:ss")));
					return;

				}
			}
		}

	}
}

void CsoStudioServer::RefreshAcView()
{
	ui.tableWidget_GameMonitor->clearContents();
	ui.tableWidget_GameMonitor->setRowCount(0);
	
	for (auto &iter : m_Login.GetMapLogin())
	{
		emit sig_AddShowUserInfo(iter.second);
	}
}

QtRCFServer* CsoStudioServer::GetRCF()
{
	return m_RCF;
}

CLoginModule& CsoStudioServer::GetLogin()
{
	return m_Login;
}

void CsoStudioServer::closeEvent(QCloseEvent* e)
{
	//窗口关闭时弹出的提示窗口


	
	QMessageBox msgBox;
	msgBox.setText("提示");
	msgBox.setInformativeText("确认退出? 退出后所有游戏进程将会自动关闭！");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if (ret == QMessageBox::Ok) 
	{
		//若用户点击确认，则接收这个事件,当前窗口会关闭
		
		//结束所有游戏进程
		CACEProcess Process;
		
		std::map<DWORD, PROCESSENTRY32>* ArrayInfo = Process.GetProcessArrayInfo(xorstr_("cstrike-online.exe"));
		
		for (auto & iter : *ArrayInfo)
		{
			Process.KillProcess(iter.second.th32ProcessID);
		}

		//强制结束自身进程
		TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);

	}
	else {
		//若用户点击取消，则忽略这个事件，当前窗口不会关闭
		e->ignore();
	}

}

void CsoStudioServer::popMenuCreateAutionDlg()
{
	m_pDlgAution->setWindowModality(Qt::ApplicationModal);
	m_pDlgAution->SetTypeHandle(1);
	m_pDlgAution->SetTaskID(m_RCF->GetAuctionTask().size());
	m_pDlgAution->show();
}

void CsoStudioServer::popMenuChangeAution()
{
	if (m_VecSelectedTaskIndex.empty())
	{
		qInfo() << "未选中任务ID,请先选中一个ID";
		return;
	}
	
	if (m_VecSelectedTaskIndex.size() > 1)
	{
		qInfo() << "只能选中一个任务ID,请先选中一个任务ID";
		return;
	}
	//get ui task id
	auto uiTaskID =  ui.tableWidget_GameTaskAuction->item(m_VecSelectedTaskIndex.at(0),0);
	ULONG uTaskID = uiTaskID->text().toInt();

	if (m_RCF->GetAuctionTask().count(uTaskID) == 0)
	{
		qInfo() << "未找到任务ID";
		return;
	}

	m_pDlgAution->setWindowModality(Qt::ApplicationModal);
	m_pDlgAution->SetTypeHandle(2);
	m_pDlgAution->SetTaskInfo(m_RCF->GetAuctionTask().at(uTaskID));
	m_pDlgAution->show();
}

void CsoStudioServer::popMenuDelAution()
{
	if (QMessageBox::information(NULL, "Title", QString("你确定要删除选中的:%1条任务么?").arg(QString::number(m_VecSelectedTaskIndex.size())),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		int nSize = m_VecSelectedTaskIndex.size();
		
		std::map<ULONG, PTAG_GAME_TASK_AUCTION>::iterator iter;

		for (size_t i = 0; i < nSize; i++)
		{
		
			for (iter = m_RCF->GetAuctionTask().begin(); iter != m_RCF->GetAuctionTask().end();)
			{

				if (m_VecSelectedTaskIndex.at(i) == iter->first)
				{
					iter->second->TaskStatus = 2;
					break;
				}
				else
				{
					iter++;
				}

			}

		}

		//刷新界面
		RefreshAutionTaskView();

	}
}

void CsoStudioServer::popMenuCreateAccountDlg()
{
	m_pDlgAccount->setWindowModality(Qt::ApplicationModal);
	m_pDlgAccount->SetTypeHandle(1);
	m_pDlgAccount->show();
}

void CsoStudioServer::popMenuChangeAccountDlg()
{

	if (m_VecSelectedAccountIndex.empty())
	{
		qInfo() << "未选中账号,请先选中一个账号";
		return;
	}

	if (m_VecSelectedAccountIndex.size() > 1)
	{
		qInfo() << "只能选中一个账号,请先选中一个账号";
		return;
	}

	auto uiItem = ui.tableWidget_GameMonitor->item(m_VecSelectedAccountIndex.at(0), 1);
	auto UserName = uiItem->text();

	auto pAcountInfo = m_Login.SearchUserName(UserName.toLocal8Bit().constData());

	if (pAcountInfo == nullptr)
	{
		qInfo() << "未找到账号ID";
		return;
	}

	m_pDlgAccount->setWindowModality(Qt::ApplicationModal);
	m_pDlgAccount->SetTypeHandle(2);
	m_pDlgAccount->SetAccountInfo(pAcountInfo);
	m_pDlgAccount->show();
}

void CsoStudioServer::popMenuDelAccountDlg()
{ 
	if (m_VecSelectedAccountIndex.empty())
	{
		qInfo() << "未选中账号,请先选中一个账号";
		return;
	}

	//判断登陆线程是否在执行
	if (m_Login.GetLoginThreadStatus())
	{
		qInfo() << "请先停止游戏批量登陆";
		return;
	}
	//检测当前已登录的游戏中是否存在当前账号
	for (size_t i = 0; i < m_VecSelectedAccountIndex.size(); i++)
	{
		auto cellItem = ui.tableWidget_GameMonitor->item(m_VecSelectedAccountIndex.at(i), 1); //账号
		 
		if (m_Login.IsLogin(cellItem->text().toLocal8Bit().constData()))
		{
			qInfo() << "账号:[" << cellItem->text() << "]以登陆,账号已登录,请先结束此账号的游戏进程";
			
			return;
		}
	}

	if (QMessageBox::information(NULL, "Title", QString("你确定要删除选中的:%1账号信息么?").arg(QString::number(m_VecSelectedAccountIndex.size())),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		int nSize = m_VecSelectedAccountIndex.size();

		std::map<ULONG, PTAG_GAME_AC_LOGIN>& Info = m_Login.GetMapLogin();
		
		for (auto& iter : Info)
		{
			for (size_t i = 0; i < nSize; i++)
			{
				auto cellItem = ui.tableWidget_GameMonitor->item(m_VecSelectedAccountIndex.at(i), 1); //账号
				
				if (iter.second->pData->GameUserName == cellItem->text().toLocal8Bit().constData())
				{
					delete iter.second->pData;
					delete iter.second;
					Info.erase(iter.first);
				}
			}
		}

	}
	m_Login.ReSortLoginInfo();
	
	RefreshAcView();
}

void CsoStudioServer::popMenuEndProcessAccountDlg()
{
	if (m_VecSelectedAccountIndex.empty())
	{
		qInfo() << "未选中账号,请先选中账号";
		return;
	}
	//检测当前已登录的游戏中是否存在当前账号
	for (size_t i = 0; i < m_VecSelectedAccountIndex.size(); i++)
	{
		auto cellItem = ui.tableWidget_GameMonitor->item(m_VecSelectedAccountIndex.at(i), 1); //账号

		if (m_Login.IsLogin(cellItem->text().toLocal8Bit().constData()))
		{
			if (auto Info =  m_Login.SearchUserName(cellItem->text().toLocal8Bit().constData()))
			{
				bool bEndProcessStatus = m_Login.EndProcess(Info->GUID);

				qInfo() << "账号:[" << cellItem->text() << "]以登录,结束进程状态:" << bEndProcessStatus;

				continue;;
			}

		}
		qInfo() << "账号:[" << cellItem->text() << "]未登录,无法结束游戏进程";
	}
	

}

void CsoStudioServer::popMenuGetAuctionMetadata()
{

	if (m_VecSelectedAccountIndex.empty())
	{
		qInfo() << "未选中账号,请先选中一个账号";
		return;
	}

	if (m_VecSelectedAccountIndex.size() > 1)
	{
		qInfo() << "只能选中一个账号,请先选中一个账号";
		return;
	}

	auto uiItem = ui.tableWidget_GameMonitor->item(m_VecSelectedAccountIndex.at(0), 1);
	auto UserName = uiItem->text();

	auto pAcountInfo = m_Login.SearchUserName(UserName.toLocal8Bit().constData());

	if (pAcountInfo == nullptr)
	{
		qInfo() << "未找到账号ID";
		return;
	}

	//判断账号是否已经登陆
	if (!m_Login.IsLogin(UserName.toLocal8Bit().constData()))
	{
		qInfo() << "账号:[" << UserName << "]未登录,无法获取拍卖信息";
		return;
	}
	auto &LocalProcess = m_Login.GetMapProcess();
	if (LocalProcess.count(pAcountInfo->dwProcessID) == 0)
	{
		qInfo() << "账号:[" << UserName << "]未启动游戏,无法获取拍卖信息";
		return;
	}
	
	auto Info =  LocalProcess.at(pAcountInfo->dwProcessID);
	
	if (Info->pSession == nullptr)
	{
		qInfo() << "账号:[" << UserName << "]未登录游戏,无法获取拍卖信息";
		return;
	}
	CsoStudioSession* pSession = static_cast<CsoStudioSession*>(Info->pSession);
	
	auto Result =  m_RCF->GetClientAuctionMetadate(pSession->GetSessionStr());

	if (Result.size() == 0)
	{
		qInfo() << "账号:[" << UserName << "]未找到拍卖会信息,可能未在拍卖会期间";
		return;
	}

	slWriteAuctionPriceCSV(Result, pAcountInfo->pData->GameReg);
}

void CsoStudioServer::slWriteAuctionPriceCSV(std::vector<BYTE>& VecData, BYTE GameReg)
{

	auto &Util = CACEInterface::GetInstance()->GetUtil();


	CACEFile ACEFile;

	auto Path = Util->UTF8_To_string(ACEFile.GetCurrentDirectoryA("账号导出"));
	CreateDirectoryA(Path.data(), NULL);

	SYSTEMTIME	szTime;
	GetLocalTime(&szTime);

	auto Name = QString("%1-%2-%3-%4.csv").arg(szTime.wYear).arg(szTime.wMonth).arg(szTime.wDay).arg(g_Util.GetServerRegionByIndexToStr(GameReg).data());

	std::string FileName = fmt::format("{}\\{}", Path, Name.toLocal8Bit().constData());

	QFile file;
	file.setFileName(QString::fromLocal8Bit(FileName.data()));

	std::ofstream stream(FileName);
	csv2::Writer<csv2::delimiter<','>> writer(stream);

	std::vector<std::vector<std::string>> rows =
	{
		{"ItemName", "强化等级", "价格上限","最低价","日期","价格",},
	};

	writer.write_rows(rows);
	
	auto lPackRead = std::make_shared<PackReader>(VecData);
	
	int vType = lPackRead->ReadChar();
	int v1    = lPackRead->ReadChar();
	int v2    = lPackRead->ReadChar();
	int v3    = lPackRead->ReadChar();
	int v4    = lPackRead->ReadChar();
	int v5    = lPackRead->ReadChar();
	int v6    = lPackRead->ReadInt();
	int v7    = lPackRead->ReadInt();
	int v8    = lPackRead->ReadInt();
	int v9    = lPackRead->ReadInt();

	for (size_t i = 0; i < 7; i++)
	{
		lPackRead->ReadChar();
	}

	int ItemCount = lPackRead->ReadShort();

	for (size_t i = 0; i < ItemCount; i++)
	{
		short ItemID = lPackRead->ReadShort();

		BYTE Lv = lPackRead->ReadChar();
		BYTE un_1 = lPackRead->ReadChar();

		int PriceMax = lPackRead->ReadInt();
		int PriceMin = lPackRead->ReadInt();

		BYTE un_2 = lPackRead->ReadChar();
		BYTE Date = lPackRead->ReadChar();

		std::string ItemName = g_Util.GetItemNameByID(ItemID);

		for (size_t i = 0; i < Date; i++)
		{
			BYTE Month = lPackRead->ReadChar();
			BYTE Day   = lPackRead->ReadChar();
			int  Price = lPackRead->ReadInt();

			//ACEInfoFileLog("[{}] 强化:{} 价格上限:{},拍卖最低价{} {}月{}日 价格:{}", ItemName, Lv, PriceMax, PriceMin, Month, Day, Price);

			std::vector<std::vector<std::string>> row_item =
			{
				{ItemName, std::to_string(Lv),std::to_string(PriceMax),std::to_string(PriceMin),fmt::format("{}-{}",Month, Day),std::to_string(Price),}
			};

			writer.write_rows(row_item);

		}

	}

	stream.close();

	QMessageBox::information(this, "", "导出完毕,请用Office Excel或WPS之类的软件打开");

	QString filePath = "file:///" + file.fileName();

	QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
