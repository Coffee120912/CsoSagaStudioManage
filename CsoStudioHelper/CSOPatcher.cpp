#include "CSOPatcher.h"


CSOPatcher::CSOPatcher()
{
	m_Hook       = CACEInterface::GetInstance()->GetHook();
	m_bFindIEWnd = FALSE;
	m_pInstance  = this;
}

BOOL CSOPatcher::InstallHook()
{
	m_Hook->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("ws2_32.dll"), xorstr_("send"), extSend, NULL, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));
	m_Hook->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("ws2_32.dll"), xorstr_("recv"), extRecv, NULL, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));
	m_Hook->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("ws2_32.dll"), xorstr_("connect"), extConnect, NULL, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));

	std::thread th(&CSOPatcher::CSOLauncherUpdateThread,this);
	th.detach();


	return TRUE;
}

BOOL CALLBACK CSOPatcher::EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	char strBuffer[MAX_PATH] = { 0 };
	GetWindowTextA(hwndChild, strBuffer, MAX_PATH);

	char ClassName[MAX_PATH];
	GetClassNameA(hwndChild, ClassName, MAX_PATH);


	if (lstrcmpA(ClassName, xorstr_("Internet Explorer_Server")) == 0)
	{

		POINT pt;
		pt.x = 580;
		pt.y = 91;
		LPARAM lParam = MAKELPARAM(pt.x, pt.y);

		SendMessageA(hwndChild, WM_LBUTTONDOWN, 0, lParam);
		Sleep(20);
		SendMessageA(hwndChild, WM_LBUTTONUP, 0, lParam);
		::PostMessage(hwndChild, WM_KEYDOWN, VK_RETURN, 0);

		CSOPatcher::m_pInstance->m_bFindIEWnd = TRUE;
	}

	return TRUE;
}

void CSOPatcher::CSOLauncherUpdateThread()
{
	while (!m_bFindIEWnd)
	{
		Sleep(5000);
		HWND	HwndParent = FindWindowA(xorstr_("#32770"), xorstr_("CSOLauncher"));
		if (!HwndParent)
		{
			continue;
		}
		LPARAM lParam = NULL;
		EnumChildWindows(HwndParent, EnumChildProc, lParam);
	}
}

int WSAAPI CSOPatcher::extSend(SOCKET s, const char* buf, int len, int flags)
{
	std::vector<BYTE> VecSend(len,0);
	
	memcpy(VecSend.data(), buf,len);

	if (len == 0x18)
	{

		std::thread th([=]()
		{

			/*	PackWriter Pack = PackWriter(1024);
				Pack.WriteByte(1);

				Pack.WriteBytes((BYTE*)buf, len);

				COPYDATASTRUCT data;
				data.dwData = 0;
				data.cbData = Pack.GetBytesLength();
				data.lpData = Pack.GetBytes();
				::SendMessageA(hClient, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);*/
				
		});
		th.detach();
	}

	ACEDebugFileLog("[{}] len:{} buf: {:02X} ", __FUNCTION__, len, fmt::join(VecSend.begin(), VecSend.end(), " "));


	return send(s, buf, len, flags);
}

int WSAAPI CSOPatcher::extRecv(SOCKET s, char* buf, int len, int flags)
{
	int result = recv(s, buf, len, flags);

	if (result != SOCKET_ERROR && result >= 3 && strstr(buf, xorstr_("http://cdnupdatecso.tiancity.cn")))
	{
		//PackWriter Pack = PackWriter(1024);

		//Pack.WriteByte(2);

		//Pack.WriteBytes((BYTE*)buf, result);

		//COPYDATASTRUCT data;
		//data.dwData = 0;
		//data.cbData = Pack.GetBytesLength();
		//data.lpData = Pack.GetBytes();
		//::SendMessageA(hClient, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);


		// 验证成功
	}

	ACEDebugFileLog("[{}] result:{} buf {} ", __FUNCTION__, result, buf);
	
	return result;
}

int WSAAPI CSOPatcher::extConnect(_In_ SOCKET s, _In_reads_bytes_(namelen) const struct sockaddr FAR* name, _In_ int namelen)
{
	int nResult = ::connect(s, name, namelen);

	if (nResult == SOCKET_ERROR)
	{
		int error_code = WSAGetLastError();

		ACEDebugFileLog("[{}] error_code :{:x}  ", __FUNCTION__, error_code);
	}

	return nResult;
}
