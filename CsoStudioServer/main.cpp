/************************************************************************/
/* @file:  main													*/
/* @brief: QT下的环境都是UTF8编码.请一定要注意            		*/
/* @birth: created by Amber on [2022/6/18]                		*/
/* @version: V1.0.0.0											*/
/* @revision: last revised by Amber on [2022/6/18]        		*/
/************************************************************************/
#include "QtConnect.h"
#include "CsoStudioServer.h"
#include "Config.h"
QSettings* g_pAppConfig = nullptr;
#include "VerifyManage.h"


void CreateThreadMonitorinGame()
{
	auto MonitoringGame = [&]()->void
	{
		VMProtectBegin(__FUNCTION__);
		auto pProcess = std::make_shared<CACEProcess>();

		while (true)
		{
			BOOL GameInit = FALSE;
			BOOL CheatInit = FALSE;
			BOOL PassNGSInit = FALSE;
			auto pProcessInfo = pProcess->GetProcessArrayInfoW(xorstr_(L"cstrike-online.exe"));

			for (auto& iter : *pProcessInfo)
			{
				auto CallBack = [&](MODULEENTRY32W* Module)
				{
					if (lstrcmpW(Module->szModule, xorstr_(L"mp.dll")) == 0)
					{
						GameInit = TRUE;

						if (!CheatInit || !PassNGSInit)
						{
							//结束进程
							pProcess->KillProcess(iter.second.th32ProcessID);

						}
					}

					if (lstrcmpW(Module->szModule, xorstr_(L"007Helper.dll")) == 0)
					{
						CheatInit = TRUE;
					}

					if (lstrcmpW(Module->szModule, xorstr_(L"AmberHelper.dll")) == 0)
					{
						PassNGSInit = TRUE;
					}
				};

				pProcess->EnumProcessModule(iter.second.th32ProcessID, CallBack);
			}

			if (GameInit)
			{
				if (!CheatInit)
				{
					MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(xorstr_(u8"脚本模块未加载！")).data(), 0, MB_ICONSTOP);
				}
				if (!PassNGSInit)
				{
					MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(xorstr_(u8"防御模块未加载！")).data(), 0, MB_ICONSTOP);
				}
			}
			Sleep(10 * 1000);
		}
		VMProtectEnd();
	};

#if !ANTI_DEBUG
	//创建监测游戏线程
	std::thread th(MonitoringGame);
	th.detach();
#endif
}

LONG NTAPI AhnExceptionHandler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	ULONG				nRet = EXCEPTION_CONTINUE_SEARCH;

	PEXCEPTION_RECORD	Record = ExceptionInfo->ExceptionRecord;
	PCONTEXT			Context = ExceptionInfo->ContextRecord;

	switch (Record->ExceptionCode)
	{

		case EXCEPTION_INT_DIVIDE_BY_ZERO:
		case 0xE06D7363:	//RadioException
		case EXCEPTION_ILLEGAL_INSTRUCTION:
		case EXCEPTION_PRIV_INSTRUCTION:
		case 0x40010006:
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}
		case  0xC0000374:
		{
			/*
			游戏未正常运行,关闭句柄出现的异常
			*/
			nRet = EXCEPTION_CONTINUE_EXECUTION;
			break;

		}
		case  0xC0000005:
		{
			/*
					C05 Dump下来
			*/
			break;

		}

	}

	return nRet;
}

int main(int argc, char *argv[])
{
	VMProtectBegin(__FUNCTION__);
	
	auto ACEInterface = CACEInterface::GetInstance();

	DisableThreadLibraryCalls(ACEInterface->GetHook()->GetSelfModuleByAddress(main));

	TAG_ACE_HOOK_MODULE_INFO AppList[] =
	{
		ACE_HOOK_MODULE_INFO("CsoStudioServer.exe"),
		ACE_HOOK_MODULE_INFO("SagaStudio.exe"),
	};
	if (!CACEModule::GetModuleDetect(AppList, sizeof(AppList) / sizeof(TAG_ACE_HOOK_MODULE_INFO)))
	{
		MessageBoxW(0, L"请勿更改进程名！", 0, MB_ICONERROR);
		return false;
	}

	auto AppName = ACEInterface->GetModule()->GetModuleInfo()->AppName;

#if ANTI_DEBUG
	//安装日志
	ACELog.ACELogInstal(AppName, ACE_LOG_FILE_ONCUT);
	ACELog.ACEDebugView(CONSOLE /*| DEBUG_VIEW*/);
#endif

//	//PVOID	VectException = AddVectoredExceptionHandler(EXCEPTION_EXECUTE_HANDLER, AhnExceptionHandler);

	QApplication a(argc, argv);
	g_pConfig = new Config();
	
	
	//结束所有游戏进程
	CACEProcess Process;

	std::map<DWORD, PROCESSENTRY32>* ArrayInfo = Process.GetProcessArrayInfo(xorstr_("cstrike-online.exe"));

	for (auto& iter : *ArrayInfo)
	{
		Process.KillProcess(iter.second.th32ProcessID);
	}

	CreateThreadMonitorinGame();

	//if (g_pConfig->GetAutoStart())
	//{
	//	//自动启动.延迟多少秒才启动
	//	Sleep(g_pConfig->GetStartDelay());
	//}

	if (!g_FreeYun.Init())
	{
		MessageBoxW(0, g_FreeYun.GetErrorStr().data(),0, MB_ICONSTOP);
		TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);
		return false;
	}

    CsoStudioServer w;
    w.show();

	VMProtectEnd();
    return a.exec();
}
