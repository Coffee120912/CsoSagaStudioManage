#include "LoginModule.h"


CLoginModule::CLoginModule()
{
	m_ulTimeout = 60 * 1000 * 3;
	m_uConut    = 0;
	m_pACEUtil  = CACEInterface::GetInstance()->GetUtil();
}


bool CLoginModule::Init(std::string GameAPP, std::string GamePath, LoginModuleMessage MsgPointer)
{
	m_GameAPP  = GameAPP;
	m_GamePath = GamePath;
	m_msg      = MsgPointer;
	return true;
}

ULONG CLoginModule::AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient)
{
	PTAG_GAME_AC_LOGIN LoginInfo = new TAG_GAME_AC_LOGIN;
	LoginInfo->GUID              = m_pACEUtil->GenerateGUID();
	LoginInfo->pData             = pDataClient;
	

	m_MapLogin.insert(std::map<ULONG, PTAG_GAME_AC_LOGIN>::value_type(m_uConut++, LoginInfo));
	return m_uConut;
}

void CLoginModule::StartProcess()
{
#if 0
	//���������Ϸ�����߳�
	while (true)
	{
		Sleep(1500);
		auto updataStatus = CheckGameUpdataThread();
		m_msg(fmt::format("������Ϸ״̬:{}", updataStatus));
		if (updataStatus == false)
		{
			auto Text = fmt::format("{}\\hw.dll", m_GamePath);
			DeleteFileA(Text.data());
			continue;
		}
		break;

	}
#endif

	while (true)
	{
		for (auto& ite : m_MapLogin)
		{
			PTAG_GAME_AC_LOGIN LoginInfo = ite.second;
			
			//����״̬=1 ֹͣ�Զ���¼
			if (LoginInfo->pData->UserTaskStatus)
				continue;
			if (LoginInfo->pData->UserLoginCount>=50)
			{
				LoginInfo->pData->UserTaskStatus = 1;
				m_msg(MSG_ID::MSG_STOPLOGIN_OUTNUMBER,"��½��������50��,ֹͣ��½.", LoginInfo);
				continue;
			}

			//�ж��Ƿ�ɵ�½
			std::string Key = fmt::format("{}_AC_{}", ANTI_SDK_EVENT, LoginInfo->GUID);
			auto hEvent = CreateEventA(NULL, FALSE, FALSE, Key.data());
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				ACEErrorFileLog("[{}] {} -> �����ѵ�½", __FUNCTION__, LoginInfo->pData->GameUserName);
				CloseHandle(hEvent);
				continue;
			}
			LoginInfo->hLoginEvent = hEvent;
			
			//��½����+1
			LoginInfo->pData->UserLoginCount++;

			// ���ô�����Ϣ
			g_Util.SetGameRegion(LoginInfo->pData->GameReg);

			//������Ϸ����
			StartGameProcess(LoginInfo);

		}
		Sleep(1000 * 5);
	}

}

void CLoginModule::WaitForProcessThread(PROCESS_INFORMATION Process, PTAG_GAME_AC_LOGIN pInfo)
{
	pInfo->dwProceeExitCode = 0;
	WaitForSingleObject(Process.hProcess, INFINITE);
	GetExitCodeProcess(Process.hProcess, &pInfo->dwProceeExitCode);
	
	// ����ǿ�ƽ������̵�ID = 0x1234
	if (pInfo->dwProceeExitCode == EXIT_CODE)
	{
		ACEErrorFileLog("[{}] �����˳�",__FUNCTION__);
	}
	else
	{
		ACEErrorFileLog("[{}] �쳣�˳�:0x{:X}", __FUNCTION__, pInfo->dwProceeExitCode);
	}
	
	//֪ͨ�ͻ��˵��˳����� 0x1234
	TerminateProcess(pInfo->hProcess, EXIT_CODE);
	//֪ͨ�¼�
	SetEvent(pInfo->hLoginEvent);
	//�رվ��
	CloseHandle(pInfo->hLoginEvent);

	// delete
	if (m_MapProcessInfo.count(Process.dwProcessId))
	{
		//��Ҫ���ٹ���ṹ�ڴ�
		//delete m_MapProcessInfo[Process.dwProcessId];
		m_MapProcessInfo.erase(Process.dwProcessId);
	}

	//-------------------------------------------------------------------
	// 0x9C410C7A ��(λ�� 007StudioStartApp.exe ��)�������쳣: 0xC0000008: An invalid handle was specified��
	//-------------------------------------------------------------------
}

bool CLoginModule::CheckGameUpdataThread()
{
	ACEDebugFileLog("[{}]������Ϸ�����߳�",__FUNCTION__);

	auto Process = CACEInterface::GetInstance()->GetProcess();

	while (Process->GetProcessId(xorstr_("cstrike-online.exe")))
	{
		auto result = Process->GetProcessArrayInfo(xorstr_("cstrike-online.exe"));
		for (auto& ite : *result)
		{
			Process->KillProcess(ite.second.th32ProcessID);
		}
		Sleep(2500);
	}
	while (Process->GetProcessId(xorstr_("CSOLauncher.exe")))
	{
		auto result = Process->GetProcessArrayInfo(xorstr_("CSOLauncher.exe"));
		for (auto& ite : *result)
		{
			Process->KillProcess(ite.second.th32ProcessID);
		}
		Sleep(500);
	}

	//�ر���Ϸ������Ϣ
	g_Util.CloseCsoSharedDict();
	
	Sleep(500);
	
	//������Ϸ������
	auto LanuncherApp = fmt::format("{}\\CSOLauncher.exe", m_GamePath);

	ACEDebugFileLog("[{}]������Ϸ��¼������:{}", __FUNCTION__, LanuncherApp);	

	auto GameLanuncherProcess = Process->Create(LanuncherApp.data(),"", m_GamePath.data());
	
	//�ȴ�10����
	DWORD dwCode = WaitForSingleObject(std::get<1>(GameLanuncherProcess).hProcess, 1000 * 60 * 10);
	
	if (dwCode == WAIT_TIMEOUT)
	{
		// ��ʱ��.
		TerminateProcess(std::get<1>(GameLanuncherProcess).hProcess, 0);

		Sleep(1000);

		//emit sig_DebugInfo(2, u8"������Ϸ��ʱ.�رս���.���¸���");
		return false;
	}
	
	//emit sig_DebugInfo(1, u8"������Ϸ���!");
	return true;
}

std::map<DWORD, PLocaProcessInfo>& CLoginModule::GetMapProcess()
{
	return m_MapProcessInfo;
}

bool CLoginModule::StartGameProcess(PTAG_GAME_AC_LOGIN LoginInfo)
{
	bool bRet = false;
	
	//��������
	auto ProInfo = CACEInterface::GetInstance()->GetProcess()->CreateEx(m_GameAPP.data(), LoginInfo->CmdLine.data(), m_GamePath.data());
	LoginInfo->hProcess = std::get<1>(ProInfo).hProcess;

	ACEWarningFileLog("[{}] �������� {} -> {}  Pid:{}", __FUNCTION__, LoginInfo->pData->GameUserName, LoginInfo->GUID, std::get<1>(ProInfo).dwProcessId);

	//join map
	PLocaProcessInfo pProInfo = new LocaProcessInfo;
	pProInfo->pSession        = nullptr;
	pProInfo->pUserData       = LoginInfo;
	m_MapProcessInfo.insert(std::map<DWORD, PLocaProcessInfo>::value_type(std::get<1>(ProInfo).dwProcessId, pProInfo));

	//�����ȴ����̽����߳�
	std::thread(&CLoginModule::WaitForProcessThread, this, std::get<1>(ProInfo), LoginInfo).detach();

	//�ָ���ͣ�߳�
	ResumeThread(std::get<1>(ProInfo).hThread);

	//�ȴ�ͨѶ�¼�֪ͨ
	if (WaitForSingleObject(LoginInfo->hLoginEvent, m_ulTimeout) == WAIT_TIMEOUT)
	{
		ACEErrorFileLog("[{}] ��¼��ʱ", __FUNCTION__);
		//TerminateProcess(LoginInfo->hProcess, EXIT_CODE);
		//CloseHandle(LoginInfo->hLoginEvent);
	}
	else
	{
		//-------------------------------------------------------------------
		// �յ�֪ͨ���������
		// 1:�����˳�,�̷߳������ź�
		// 2:��½���,��Ϸ�����ĳɹ����ź�
		//-------------------------------------------------------------------

		//�ж���Ϸ�����Ƿ����
		DWORD	lpExitCode = 0;
		GetExitCodeProcess(LoginInfo->hProcess, &lpExitCode);

		if (lpExitCode != STILL_ACTIVE)
		{
			//ACEDebugFileLog("[Session] �̷߳�����.");
			bRet = false;

		}
		else
		{
			bRet = true;
			//��Ϸ������
			ACEDebugFileLog("[Session] ��¼�ɹ�");
	
		}
		
	}
	return bRet;
}
