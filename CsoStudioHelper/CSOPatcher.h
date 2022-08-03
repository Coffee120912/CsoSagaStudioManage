#pragma once
#include "SagaConnect.h"

class CSOPatcher
{
public:
	CSOPatcher();

	BOOL InstallHook();

	//启动器更新线程
	void  CSOLauncherUpdateThread();



private:
	static int WSAAPI  extSend(SOCKET s, const char* buf, int len, int flags);
	static int WSAAPI  extRecv(SOCKET s, char* buf, int len, int flags);
	static int WSAAPI  extConnect(_In_ SOCKET s, _In_reads_bytes_(namelen) const struct sockaddr FAR* name, _In_ int namelen);
	static	BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam);
private:
	std::shared_ptr<CACEHook> m_Hook;
	BOOL                      m_bFindIEWnd;
	static inline CSOPatcher* m_pInstance = nullptr;
};

