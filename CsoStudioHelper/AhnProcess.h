#pragma once
#include "SagaConnect.h"

BOOL(WINAPI* _CreateProcessA)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) = NULL;

BOOL WINAPI extCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{

	DWORD	nFlags = dwCreationFlags | CREATE_SUSPENDED;
#if ANTI_DEBUG
	if (!(dwCreationFlags & DETACHED_PROCESS)) nFlags |= CREATE_NEW_CONSOLE;
#endif

	if (!_CreateProcessA) _CreateProcessA = CreateProcessA;


#if 1 
	//调试摸下 不要开这个.否则无法启动游戏进程
	auto ACEInterface = CACEInterface::GetInstance();
	//禁止游戏启动器 来 启动游戏 cstrike-online.exe
	if (lstrcmpA(ACEInterface->GetModule()->GetModuleInfo()->AppName, xorstr_("CSOLauncher.exe")) == 0 && lpApplicationName && lstrcmpA(lpApplicationName, xorstr_("cstrike-online.exe")) == 0)
	{
		ACEDebugFileLog("[{}] 禁止启动游戏进程:{}", __FUNCTION__, lpApplicationName);

		return TRUE;
	}
#endif // !ANTI_AUTO_DEBUG


	BOOL nRet = _CreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, nFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
	if (nRet)
	{
		char	szFileName[MAX_PATH];
		auto Hook    = CACEInterface::GetInstance()->GetHook();
		auto Process = CACEInterface::GetInstance()->GetProcess();

		GetModuleFileNameA(Hook->GetSelfModuleHandle(), szFileName, MAX_PATH);
		Process->ProcessCreateRmoterThreadEx(lpProcessInformation->hProcess, lpProcessInformation->hThread, szFileName);
	}

	if (!(dwCreationFlags & CREATE_SUSPENDED)) ResumeThread(lpProcessInformation->hThread);

	return nRet;
}
NTSTATUS(WINAPI* _ZwCreateUserProcess)(PHANDLE ProcessHandle, PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, ULONG CreateProcessFlags, ULONG CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, void* AttributeList) = NULL;
NTSTATUS WINAPI extZwCreateUserProcess(PHANDLE ProcessHandle, PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, ULONG CreateProcessFlags, ULONG CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, void* AttributeList)
{
	NTSTATUS	Status = _ZwCreateUserProcess(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, CreateProcessFlags, CreateThreadFlags, ProcessParameters, Parameter9, AttributeList);
	if (NT_SUCCESS(Status))
	{
		char	szFileName[MAX_PATH];
		auto Hook    = CACEInterface::GetInstance()->GetHook();
		auto Process = CACEInterface::GetInstance()->GetProcess();

		auto ImagePathName =  CACEInterface::GetInstance()->GetUtil()->UnicodeToUtf8(ProcessParameters->ImagePathName.Buffer);

		auto AppName = strrchr(ImagePathName.data(), L'\\') + 1;

		for (auto & iter :AppList)
		{
			if (lstrcmpA(iter.AppName, AppName) == 0)
			{
				GetModuleFileNameA(Hook->GetSelfModuleHandle(), szFileName, MAX_PATH);
				Process->ProcessCreateRmoterThreadEx(*ProcessHandle, *ThreadHandle, szFileName);
				
				break;
			}
		}

	}
	return Status;
}