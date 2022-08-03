#include "CsoStudiHelper.h"
std::array<TAG_ACE_HOOK_MODULE_INFO, 10> AppList;


BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    VMProtectBegin(__FUNCTION__);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
       auto ACEInterface =  CACEInterface::GetInstance();

       DisableThreadLibraryCalls(ACEInterface->GetHook()->GetSelfModuleByAddress(hModule));

	   AppList =
	   {
		   ACE_HOOK_MODULE_INFO("007StudioStartApp.exe"),
		   ACE_HOOK_MODULE_INFO("CSOLauncher.exe"),
		   ACE_HOOK_MODULE_INFO("cstrike-online.exe"),
		   ACE_HOOK_MODULE_INFO("CsoStudioServer.exe"),
		   ACE_HOOK_MODULE_INFO("SagaStudio.exe"),
	   };

       if (!CACEModule::GetModuleDetect(&(AppList.at(0)), AppList.size()))  return FALSE;

       auto AppName = ACEInterface->GetModule()->GetModuleInfo()->AppName;

#if ANTI_DEBUG
	   //安装日志
	   ACELog.ACELogInstal(AppName, ACE_LOG_FILE_ONCUT);
	   ACELog.ACEDebugView(CONSOLE | DEBUG_VIEW);
#endif
	   ACEInterface->GetFile()->SetFileName(fmt::format(xorstr_("{}\\ACE_DEBUG"), ACEInterface->GetModule()->GetModuleInfo()->ModulePath));

	   if (ACEInterface->GetFile()->OpenFile(OPEN_EXISTING) != INVALID_HANDLE_VALUE)
	   {
		   ACEInterface->GetFile()->CloseFile();
		   MessageBoxA(NULL, GetCommandLineA(), xorstr_("拦截启动"), MB_OK);
	   }
	   //Hook启动进程
	   ACEInterface->GetHook()->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("kernel32.dll"), xorstr_("CreateProcessA"), extCreateProcessA, &_CreateProcessA, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));
	  
	
	 if (lstrcmpiA(AppName, xorstr_("CSOLauncher.exe")) == 0)
	 {
		  //PVOID	VectException = AddVectoredExceptionHandler(EXCEPTION_EXECUTE_HANDLER, AhnExceptionHandler);
		 CSOPatcher* Pathcher = new CSOPatcher();
		 Pathcher->InstallHook();
	 }
	 else if (lstrcmpiA(AppName, xorstr_("cstrike-online.exe")) == 0)
	 {
		 AhnCheat::GetInstance()->CheatInit();
		 CGameRCF::GetInstance()->StartThreadRCFInit();
	 }
	 else if (lstrcmpiA(AppName, xorstr_("SagaStudio.exe")) == 0 || lstrcmpiA(AppName, xorstr_("CsoStudioServer.exe")) == 0)
	 {
		 ACEInterface->GetHook()->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO("ntdll.dll", "NtCreateUserProcess",extZwCreateUserProcess, &_ZwCreateUserProcess, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));
	 }
	

    }

    VMProtectEnd();
    return TRUE;
}

