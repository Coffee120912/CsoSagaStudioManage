#pragma once
#include "Util.h"
#include "thread_pool.hpp"

//消息通知
typedef std::function<void(MSG_ID MsgId,std::string MsgStr, PTAG_GAME_AC_LOGIN Client)> LoginModuleMessage;
//退出进程代码
#define EXIT_CODE	0x1234


class CLoginModule
{
public:
	CLoginModule();
	
	static CLoginModule* GetInstance();

	// @初始化
	bool Init(std::string GameAPP,std::string GamePath, LoginModuleMessage MsgPointer);

	// @添加登陆信息
	PTAG_GAME_AC_LOGIN AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient);
	
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

	// @查询用户账号
	PTAG_GAME_AC_LOGIN SearchUserName(std::string UserName);

	// @返回登陆信息
	std::map<ULONG, PTAG_GAME_AC_LOGIN>& GetMapLogin();

	// @结束指定进程
	bool EndProcess(std::string Guid);

	// @停止登陆账号
	bool StopLogin(BOOL bStatus);

	// @心跳检测线程
	void HeartBeatThread();

	/*
	*	@返回登录线程状态
	*	@return true:线程运行中 false:线程已经结束
	*/
	BOOL GetLoginThreadStatus();


	// @账号是否已经登陆
	BOOL IsLogin(std::string UserName);

	// @保存的登陆信息ID重新排序
	void ReSortLoginInfo();

	// @清空登陆信息
	void ClearLoginInfo();
	
	// @设置全局共享功能
	void SetGlobalShare(PTAG_GAME_AC_LOGIN pInfo);

	GLOBAL_FUNCTION& GetGlobalShare();
private:
	// 登陆超时时间
	ULONG                               m_ulTimeout;
	//登陆信息
	std::map<ULONG, PTAG_GAME_AC_LOGIN> m_MapLogin;
	std::string                         m_GameAPP;
	std::string                         m_GamePath;
	ULONG                               m_uConut;
	ULONG								m_uMaxCount;
	std::shared_ptr<CACEUtil>		    m_pACEUtil;
	LoginModuleMessage					m_msg;
	std::map<DWORD, PLocaProcessInfo>	m_MapProcessInfo;
	BOOL								m_LoginStatus;
	thread_pool							m_LoginThread;
	static inline CLoginModule*         m_pInstance = nullptr;
	GLOBAL_FUNCTION						m_ShareFuns;
};

