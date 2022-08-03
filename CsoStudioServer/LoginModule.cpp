#include "LoginModule.h"


CLoginModule::CLoginModule()
{
	m_ulTimeout = 60 * 1000 * 3;
	m_uConut    = 0;
	m_uMaxCount = 50;
	m_pACEUtil  = CACEInterface::GetInstance()->GetUtil();
	m_LoginThread.reset(1);
	m_pInstance = this;
}


CLoginModule* CLoginModule::GetInstance()
{
	return m_pInstance;
}

bool CLoginModule::Init(std::string GameAPP, std::string GamePath, LoginModuleMessage MsgPointer)
{
	m_GameAPP  = GameAPP;
	m_GamePath = GamePath;
	m_msg      = MsgPointer;
	std::thread(std::bind(&CLoginModule::HeartBeatThread, this)).detach();
	return true;
}

PTAG_GAME_AC_LOGIN CLoginModule::AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient)
{
	PTAG_GAME_AC_LOGIN LoginInfo = new TAG_GAME_AC_LOGIN();
	LoginInfo->GUID              = m_pACEUtil->GenerateGUID();
	LoginInfo->pData             = pDataClient;
	LoginInfo->ID                = m_uConut;

	m_MapLogin.insert(std::map<ULONG, PTAG_GAME_AC_LOGIN>::value_type(m_uConut++, LoginInfo));
	return LoginInfo;
}

void CLoginModule::StartProcess()
{
#if 0
	//启动检测游戏更新线程
	while (true)
	{
		Sleep(1500);
		auto updataStatus = CheckGameUpdataThread();
		m_msg(fmt::format("更新游戏状态:{}", updataStatus));
		if (updataStatus == false)
		{
			auto Text = fmt::format("{}\\hw.dll", m_GamePath);
			DeleteFileA(Text.data());
			continue;
		}
		break;

	}
#endif
	m_LoginStatus = TRUE;
	
	auto thread_loop_login = [&]() {
		
		while (m_LoginStatus)
		{
			for (auto& ite : m_MapLogin)
			{
				PTAG_GAME_AC_LOGIN LoginInfo = ite.second;

				//任务状态=1 停止自动登录
				if (LoginInfo->pData->UserTaskStatus)
					continue;
				if (LoginInfo->pData->UserLoginCount >= m_uMaxCount)
				{
					LoginInfo->pData->UserTaskStatus = 1;
					m_msg(MSG_ID::MSG_STOPLOGIN_OUTNUMBER, "登陆次数超过指定次数,停止登陆.", LoginInfo);
					continue;
				}

				//判断是否可登陆
				std::string Key = fmt::format("{}_AC_{}", ANTI_SDK_EVENT, LoginInfo->GUID);
				auto hEvent = CreateEventA(NULL, FALSE, FALSE, Key.data());
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{

					//ACEErrorFileLog("[{}] {} -> session is login", __FUNCTION__, LoginInfo->pData->GameUserName);
					CloseHandle(hEvent);
					continue;
				}
				LoginInfo->hLoginEvent = hEvent;

				//登陆次数+1
				LoginInfo->pData->UserLoginCount++;

				// 设置大区信息
				g_Util.SetGameRegion(LoginInfo->pData->GameReg);

				// 设置全局共享信息
				SetGlobalShare(LoginInfo);
				
				//启动游戏进程
				StartGameProcess(LoginInfo);

				if (!m_LoginStatus)
				{
					break;
				}
			}
			Sleep(1000 * 5);
		}
	};

	m_LoginThread.push_task(thread_loop_login);
}

void CLoginModule::WaitForProcessThread(PROCESS_INFORMATION Process, PTAG_GAME_AC_LOGIN pInfo)
{
	pInfo->dwProceeExitCode = 0;
	WaitForSingleObject(Process.hProcess, INFINITE);
	GetExitCodeProcess(Process.hProcess, &pInfo->dwProceeExitCode);
	
	// 自身强制结束进程的ID = 0x1234
	if (pInfo->dwProceeExitCode == EXIT_CODE)
	{
		ACEErrorFileLog("[{}] normal exit",__FUNCTION__);
	}
	else
	{
		ACEErrorFileLog("[{}] Exception exit :0x{:X}", __FUNCTION__, pInfo->dwProceeExitCode);
	}
	
	//通知客户端的退出代码 0x1234
	TerminateProcess(pInfo->hProcess, EXIT_CODE);
	//通知事件
	SetEvent(pInfo->hLoginEvent);
	//关闭句柄
	CloseHandle(pInfo->hLoginEvent);

	// delete
	if (m_MapProcessInfo.count(Process.dwProcessId))
	{
		//不要销毁共享结构内存
		//delete m_MapProcessInfo[Process.dwProcessId];
		m_MapProcessInfo.erase(Process.dwProcessId);
	}

	//-------------------------------------------------------------------
	// 0x9C410C7A 处(位于 007StudioStartApp.exe 中)引发的异常: 0xC0000008: An invalid handle was specified。
	//-------------------------------------------------------------------
}

bool CLoginModule::CheckGameUpdataThread()
{
	ACEDebugFileLog("[{}]启动游戏更新线程",__FUNCTION__);

	auto Process = CACEInterface::GetInstance()->GetProcess();

	while (Process->GetProcessId(xorstr_("cstrike-online.exe")))
	{
		std::map<DWORD, PROCESSENTRY32W>* result = Process->GetProcessArrayInfoW(xorstr_(L"cstrike-online.exe"));
		for (auto& ite : *result)
		{
			Process->KillProcess(ite.second.th32ProcessID);
		}
		Sleep(2500);
	}
	while (Process->GetProcessId(xorstr_("CSOLauncher.exe")))
	{
		std::map<DWORD, PROCESSENTRY32W>* result = Process->GetProcessArrayInfoW(xorstr_(L"CSOLauncher.exe"));
		for (auto& ite : *result)
		{
			Process->KillProcess(ite.second.th32ProcessID);
		}
		Sleep(500);
	}

	//关闭游戏共享信息
	g_Util.CloseCsoSharedDict();
	
	Sleep(500);
	
	//启动游戏启动器
	auto LanuncherApp = fmt::format("{}\\CSOLauncher.exe", m_GamePath);

	ACEDebugFileLog("[{}]启动游戏登录器更新:{}", __FUNCTION__, LanuncherApp);	

	auto GameLanuncherProcess = Process->Create(LanuncherApp.data(),"", m_GamePath.data());
	
	//等待10分钟
	DWORD dwCode = WaitForSingleObject(std::get<1>(GameLanuncherProcess).hProcess, 1000 * 60 * 10);
	
	if (dwCode == WAIT_TIMEOUT)
	{
		// 超时了.
		TerminateProcess(std::get<1>(GameLanuncherProcess).hProcess, 0);

		Sleep(1000);

		//emit sig_DebugInfo(2, u8"更新游戏超时.关闭进程.重新更新");
		return false;
	}
	
	//emit sig_DebugInfo(1, u8"更新游戏完毕!");
	return true;
}

std::map<DWORD, PLocaProcessInfo>& CLoginModule::GetMapProcess()
{
	return m_MapProcessInfo;
}

bool CLoginModule::StartGameProcess(PTAG_GAME_AC_LOGIN LoginInfo)
{
	bool bRet = false;
	
	//启动进程
	auto ProInfo           = CACEInterface::GetInstance()->GetProcess()->CreateEx(m_GameAPP.data(), LoginInfo->CmdLine.data(), m_GamePath.data());
	LoginInfo->hProcess    = std::get<1>(ProInfo).hProcess;
	LoginInfo->dwProcessID = std::get<1>(ProInfo).dwProcessId;
	ACEWarningFileLog("[{}] ProcessStart {} -> {}  Pid:{}", __FUNCTION__, LoginInfo->pData->GameUserName, LoginInfo->GUID, LoginInfo->dwProcessID);
	
	m_msg(MSG_ID::MSG_START_GAME, "启动游戏", LoginInfo);

	LoginInfo->ulTimeStamp            = m_pACEUtil->GetUnixTimeStamp();
	LoginInfo->pData->clientTimeStamp = 0;
	//join map
	PLocaProcessInfo pProInfo         = new LocaProcessInfo;
	pProInfo->pSession                = nullptr;
	pProInfo->pUserData               = LoginInfo;
	m_MapProcessInfo.insert(std::map<DWORD, PLocaProcessInfo>::value_type(std::get<1>(ProInfo).dwProcessId, pProInfo));

	//启动等待进程结束线程
	std::thread(&CLoginModule::WaitForProcessThread, this, std::get<1>(ProInfo), LoginInfo).detach();

	//恢复暂停线程
	ResumeThread(std::get<1>(ProInfo).hThread);

	//等待通讯事件通知
	if (WaitForSingleObject(LoginInfo->hLoginEvent, m_ulTimeout) == WAIT_TIMEOUT)
	{
		ACEErrorFileLog("[{}] 登录超时", __FUNCTION__);
		TerminateProcess(LoginInfo->hProcess, EXIT_CODE);
		CloseHandle(LoginInfo->hLoginEvent);
	}
	else
	{
		//-------------------------------------------------------------------
		// 收到通知有两种情况
		// 1:进程退出,线程发来的信号
		// 2:登陆完毕,游戏发来的成功的信号
		//-------------------------------------------------------------------

		//判断游戏进程是否结束
		DWORD	lpExitCode = 0;
		GetExitCodeProcess(LoginInfo->hProcess, &lpExitCode);

		if (lpExitCode != STILL_ACTIVE)
		{
			//ACEDebugFileLog("[Session] 线程发来的.");
			bRet = false;

		}
		else
		{
			bRet = true;
			//游戏发来的
			ACEDebugFileLog("[Session] Login Succeed");
	
		}
		
	}
	return bRet;
}

PTAG_GAME_AC_LOGIN CLoginModule::SearchUserName(std::string UserName)
{
	for (auto iter : m_MapLogin)
	{
		if (iter.second->pData->GameUserName == UserName)
		{
			return iter.second;
		}
	}
	return nullptr;
}

std::map<ULONG, PTAG_GAME_AC_LOGIN>& CLoginModule::GetMapLogin()
{
	return m_MapLogin;
}

bool CLoginModule::EndProcess(std::string Guid)
{

	for (auto &iter : m_MapProcessInfo)
	{
		PTAG_GAME_AC_LOGIN LoginInfo = (PTAG_GAME_AC_LOGIN)iter.second->pUserData;

		if (LoginInfo->GUID == Guid)
		{
			TerminateProcess(LoginInfo->hProcess, EXIT_CODE);
			return true;
		}
		
	}

	return false;
}

bool CLoginModule::StopLogin(BOOL bStatus)
{
	return m_LoginStatus = bStatus;;
}

void CLoginModule::HeartBeatThread()
{
	while (true)
	{
		for (auto &iter : m_MapProcessInfo)
		{
			PTAG_GAME_AC_LOGIN LoginInfo = (PTAG_GAME_AC_LOGIN)iter.second->pUserData;

			if (LoginInfo->pData->clientTimeStamp == 0)
			{
				//还未初始化完毕！
				continue;
			}
			ULONG ulValue = labs(m_pACEUtil->GetUnixTimeStamp() - LoginInfo->ulTimeStamp);

			qDebug() << ulValue;

			if (ulValue > 120)
			{
				qInfo() << LoginInfo->pData->GameUserName.data() << " 超时,结束游戏";
				TerminateProcess(LoginInfo->hProcess, EXIT_CODE);
			}
		}
		Sleep(10 * 1000);

	}
}

BOOL CLoginModule::GetLoginThreadStatus()
{
	return m_LoginThread.get_tasks_running() > 0;
}



BOOL CLoginModule::IsLogin(std::string UserName)
{
	for (auto& iter : m_MapProcessInfo)
	{
		PTAG_GAME_AC_LOGIN LoginInfo = (PTAG_GAME_AC_LOGIN)iter.second->pUserData;

		if (LoginInfo->pData->GameUserName == UserName)
		{
			
			return true;
		}

	}
	return false;
}

void CLoginModule::ReSortLoginInfo()
{
	// 对已登录的.可能会造成影响
	// map 改成list 比较好
	
	std::map<ULONG, PTAG_GAME_AC_LOGIN> SavaMap = m_MapLogin;

	this->ClearLoginInfo();
	
	for (auto & iter : SavaMap)
	{
		iter.second->ID = m_uConut;
		m_MapLogin.insert(std::map<ULONG, PTAG_GAME_AC_LOGIN>::value_type(m_uConut++, iter.second));
	}

}

void CLoginModule::ClearLoginInfo()
{
	m_uConut = 0;
	m_MapLogin.clear();
}

void CLoginModule::SetGlobalShare(PTAG_GAME_AC_LOGIN pInfo)
{
	if (m_ShareFuns.AutoJoinAuctionEvent)
	{
		pInfo->pData->AuctionFlag |= (1 << 0);
	}
	else
	{
		pInfo->pData->AuctionFlag &= ~(1 << 0) & 0xF;
	}

}

GLOBAL_FUNCTION& CLoginModule::GetGlobalShare()
{
	return m_ShareFuns;
}
