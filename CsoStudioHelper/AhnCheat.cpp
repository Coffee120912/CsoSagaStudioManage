#include "AhnCheat.h"
#include "Util.h"
#include "GameOffset.h"
#include "GameClient.h"
#include "GamePack.h"

NTSTATUS(NTAPI* _LdrLoadDll)(PWCHAR PathToFile OPTIONAL, ULONG Flags OPTIONAL, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle) = nullptr;
FARPROC(WINAPI* _GetProcAddress)(HMODULE hModule, LPCSTR lpProcName)                                                                 = nullptr;
int(*_Client_F)(PVOID Pointer)																										 = nullptr;
FileHandle_t(__fastcall* g_File_Open)(void* pthis, int edx, const char* pFileName, const char* pOptions, const char* pathID)         = nullptr;

AhnCheat* AhnCheat::m_Instance = nullptr;
AhnCheat::AhnCheat()
{
	m_Instance  = this;
	m_EngineLib = nullptr;
	g_pUtil     = new Util();
}

AhnCheat* AhnCheat::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new AhnCheat();
	}
	return m_Instance;
}

void AhnCheat::CheatInit()
{
	auto sv = g_pUtil->GetGameServer();
	g_Offset.SetGameServer(std::get<0>(sv));

	g_pUtil->GetAceHook()->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("ntdll.dll"), xorstr_("LdrLoadDll"), extLdrLoadDll, &_LdrLoadDll, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));

	LoadAddLibrary();
}

FileHandle_t __fastcall AhnCheat::Open(void* pthis, int edx, const char* pFileName, const char* pOptions, const char* pathID)
{

#if 0
	ACEDebugFileLog("[{}]{} {} {}", __FUNCTION__, pFileName, pOptions, pathID == nullptr ? "null": pathID);
#endif 
	FileHandle_t Result = g_File_Open(pthis, edx, pFileName, pOptions, pathID);
	static BOOL isLogin = FALSE;
	if (!isLogin && pFileName && lstrcmpA(pFileName, xorstr_("resource/control/tabbutton/tab_big_abled_right_c.tga")) == 0)
	{
		isLogin = TRUE;
		//g_CSOWrapper.Game_Login();
	}
	return Result;
}


NTSTATUS NTAPI AhnCheat::extLdrLoadDll(PWCHAR PathToFile OPTIONAL, ULONG Flags OPTIONAL, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle)
{
	NTSTATUS	Status = _LdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);;


	if (NT_SUCCESS(Status))
	{
		char	lpLibFileName[MAX_PATH] = {0};
		HMODULE	lib                     = *(HMODULE*)ModuleHandle;
		char* pszName                   = (strrchr(lpLibFileName, '\\') != NULL) ? (char*)strrchr(lpLibFileName, '\\') + 1 : (char*)lpLibFileName;
		g_pUtil->GetAceTool()-> UnicodeToAnsi(ModuleFileName->Buffer, lpLibFileName);

		if (lstrcmpiA(pszName, "hw.dll") == 0)
		{
			std::thread CheatThread(&AhnCheat::InternetEngineStartMode, AhnCheat::GetInstance(), &lib);
			CheatThread.join();
			return Status;
		}
		else if (lstrcmpiA(pszName, "FileSystem_Nar.dll") == 0)
		{
			// * 监控游戏登陆时机 2022-06-19 14:48:24 不需要这个了
			/*CreateInterfaceFn filesystem_factory = CaptureFactory(pszName);
			auto  pFileSystem = (IFileSystem*)(CaptureInterface(filesystem_factory, xorstr_("VFileSystem009")));
			CACEInterface::GetInstance()->GetHook()->VFTHook(pFileSystem,0,10, &Open, &g_File_Open);*/
			return Status;
		}

	}
	return	Status;
}

FARPROC WINAPI AhnCheat::extGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{

	if (hModule == GetModuleHandleA(xorstr_("client.dll")))
	{
		if (lstrcmpiA(lpProcName, xorstr_("F")) == 0)
		{

			(FARPROC &)_Client_F = ::GetProcAddress(hModule, lpProcName);

			return (FARPROC)AhnCheat::GetInstance()->F;

		}
	}
	//else if (hModule == GetModuleHandleA(xorstr_("mp.dll")))
	//{
	//	return CGameMp::GetInstance()->extGetProcAddress(hModule, lpProcName);
	//}


	return ::GetProcAddress(hModule, lpProcName);
}

BOOL WINAPI AhnCheat::extCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	DWORD	nFlags = dwCreationFlags | CREATE_SUSPENDED;

	//阻止天成的广告
	if (lpCommandLine && lstrcmpA(lpCommandLine, xorstr_("CSOBanner.exe")) == 0)
	{
		return FALSE;
	}
	//阻止 发送Dump文件 
	else if ((lpCommandLine && strstr(lpCommandLine, xorstr_("CSOLauncher.exe"))) || (lpApplicationName && strstr(lpApplicationName, xorstr_("CSOLauncher.exe"))))
	{
		return FALSE;
	}

	return CreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
}

HWND WINAPI AhnCheat::extCreateWindowExA(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam)
{

	HWND result = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

	//if (result && g_Vars.m_GameHwnd == nullptr)
	//{
	//	if (lpClassName && lpWindowName)
	//	{

	//		if (lstrcmpiA(lpClassName,xorstr_("Valve001")) == 0)
	//		{
	//		
	//			g_Vars.m_GameHwnd = result;
	//		}

	//	}
	//}

	return result;
}

int AhnCheat::F(PVOID Pointer)
{
	int Ret =  _Client_F(Pointer);

	//开启外挂线程
	std::thread CheatThread(&AhnCheat::CheatEntry, AhnCheat::GetInstance());

	CheatThread.join();

	return Ret;
}


DWORD  AhnCheat::InternetEngineStartMode(LPVOID lpThreadParameter)
{
	if (m_EngineLib)
	{
		return 0;
	}

	HMODULE lib = lpThreadParameter ? *(HMODULE*)lpThreadParameter : NULL;

	while (!lib)
	{
		lib = GetModuleHandleA(xorstr_("hw.dll"));

		Sleep(200);
		if (lib)
			break;
	}

	m_EngineLib = lib;

	g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("Kernel32.dll"), xorstr_("GetProcAddress"), AhnCheat::extGetProcAddress, NULL, ACE_HOOK_TYPE_IAT));
	g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("Kernel32.dll"), xorstr_("CreateProcessA"), AhnCheat::extCreateProcessA, NULL, ACE_HOOK_TYPE_IAT));
	//g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("user32.dll"), xorstr_("CreateWindowExA"), extCreateWindowExA, NULL, ACE_HOOK_TYPE_IAT));
	
	CGamePack* pPacket = CGamePack::GetInstance();
	g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("libeay32.dll"), xorstr_("EVP_EncryptInit"), pPacket->extEVP_EncryptInit, &EVP_EncryptInit007, ACE_HOOK_TYPE_IAT));
	//2019年7月23日 11:40:48 游戏发包
	g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("libeay32.dll"), xorstr_("EVP_EncryptUpdate"), pPacket->extEVP_EncryptUpdate, &EVP_EncryptUpdate007, ACE_HOOK_TYPE_IAT));
	//g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("wsock32.dll"), xorstr_("sendto"), pPacket->extsendto, NULL, ACE_HOOK_TYPE_IAT));
	g_pUtil->GetAceHook()->HookSetDetectImport(lib, &TAG_ACE_HOOK_API_INFO(xorstr_("wsock32.dll"), xorstr_("send"), pPacket->extSend, NULL, ACE_HOOK_TYPE_IAT));


	return 1;
}

ULONG AhnCheat::CheatEntry()
{
	VMProtectBegin(__FUNCTION__);

	//if (!CSagaClient::GetInstance()->SagaCheckVar())
	//{
	//	return 0;
	//}
	if (!g_Offset.GetModuleInfo())
	{
		ACEErrorFileLogW(L"[{}]->error GetModuleInfo", __FUNCTIONW__);
	}
	if (!g_Offset.FindClientTable())
	{
		ACEErrorFileLogW(L"[{}]->error FindClientTable", __FUNCTIONW__);
	}
	if (!g_Offset.FindEngineTable())
	{
		ACEErrorFileLogW(L"[{}]->error FindEngineTable", __FUNCTIONW__);
	}
	if (!g_Offset.FindPlayermove())
	{
		ACEErrorFileLogW(L"[{}]->error FindPlayermove", __FUNCTIONW__);
	}

	g_Offset.FindGameAddres();

	//hook Client
	GameClient::GetInstance()->HookClientFunction();

	VMProtectEnd();
	return 1;
}



bool AhnCheat::LoadAddLibrary()
{
	BOOL(WINAPI * AhnSetInfo)(int Index) = NULL;

	HMODULE	hModule = NULL;

#if ANTI_DEBUG
	hModule = LoadLibraryA("E:\\Amber\\Project\\NexonGameSecurity\\Release\\NexonGameSecurity.dll");

	if (!hModule)
	{
		MessageBoxA(0, "未加载插件", 0, 0);
		exit(0);
	}
	else
	{
		//(FARPROC&)AhnSetInfo = GetProcAddress(hModule, "AhnSetInfo");
		//AhnSetInfo(4);
	}
	ACEWarningFileLog("lib:{:X}", (ULONG)hModule);

#else

	auto Text = fmt::format(xorstr_("{}\\AmberHelper.dll"),g_pUtil->GetAceHook()->GetSelfModulePath());
	hModule = LoadLibraryA(Text.data());

	if (!hModule)
	{
		MessageBoxA(0, xorstr_("未加载模块"), 0, 0);
	}
	else
	{
		//(FARPROC&)AhnSetInfo = GetProcAddress(hModule, "AhnSetInfo");
		//AhnSetInfo(4);
	}
#endif
	return false;
}

namespace Engine
{
	cl_clientfunc_t* g_pClient     = NULL;;
	cl_clientfunc_t g_Client;
	cl_enginefunc_t* g_pEngine     = NULL;;
	cl_enginefunc_t g_Engine;
	engine_studio_api_t g_Studio;
	engine_studio_api_t* g_pStudio = NULL;
	playermove_t* g_pPlayerMove;
	PUserMsg g_pUserMsgBase;
	screenfade_t* g_pScreenFade;
	SCREENINFO g_Screen;
	enginefuncs_t* g_pEngfuncs     = NULL;
	enginefuncs_t  g_Engfuncs;
}