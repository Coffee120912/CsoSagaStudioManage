#pragma once
#include "Util.h"

//消息通知
typedef std::function<void(MSG_ID MsgId,std::string MsgStr, PTAG_GAME_AC_LOGIN Client)> LoginModuleMessage;
//退出进程代码
#define EXIT_CODE	0x1234


class CLoginModule
{
public:
	CLoginModule();

	// @初始化
	bool Init(std::string GameAPP,std::string GamePath, LoginModuleMessage MsgPointer);

	// @添加登陆信息返回mapId
	ULONG AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient);
	
	// @启动进程
	void StartProcess();

	// @等待线程结束线程
	void WaitForProcessThread(PROCESS_INFORMATION Process, PTAG_GAME_AC_LOGIN pInfo);

	// @检测游戏更新线程
	bool CheckGameUpdataThread();

	// @获取本地进程结构	
	std::map<DWORD, PLocaProcessInfo>& GetMapProcess();

	// @启动游戏进程
	bool StartGameProcess(PTAG_GAME_AC_LOGIN pInfo);
private:
	// 登陆超时时间
	ULONG                               m_ulTimeout;
	//登陆信息
	std::map<ULONG, PTAG_GAME_AC_LOGIN> m_MapLogin;
	std::string                         m_GameAPP;
	std::string                         m_GamePath;
	ULONG                               m_uConut;
	std::shared_ptr<CACEUtil>		    m_pACEUtil;
	LoginModuleMessage					m_msg;
	std::map<DWORD, PLocaProcessInfo>	m_MapProcessInfo;
};

