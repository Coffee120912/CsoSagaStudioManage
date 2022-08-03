#pragma once
#include "SagaConnect.h"
#include "GameClienBase.hpp"
#include "thread_pool.hpp"
#include "GameRCF.h"

// 自动登录
#define CPX_ALOGIN	1


class CSOWrapper : public IGameClient
{
public:
	CSOWrapper();
	static CSOWrapper* GetInstance();
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	virtual bool vInit();
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);


	static inline void* (*_malloc)(size_t size) = nullptr;
	//验证码模块初始化
	static inline	void(__stdcall* CaptInit)() = nullptr;
	//验证码图片路径
	static inline char* (__stdcall* CaptPath)(char* Path) = nullptr;
	// @CSO登陆事件监听
	static void _stdcall CSONMEventListener(PTAG_PEB_STACK Stack);
	// @登陆回调
	static void _stdcall LoginCallBack(PTAG_PEB_STACK Stack);
	// @验证码打码
	static void _stdcall exCaptcha(PTAG_PEB_STACK Stack);
	// @获取登陆框指针
	static void _stdcall exVguiGetLoginClass(PTAG_PEB_STACK Stack);

	// @游戏登录
	void GameLogin();
	// @设置仓库二级密码
	void Set2ndPassWord(PASSWORD_2ND_TYPE Type,int Value);
	// @验证码类型
	void CaptchCall(std::string Type);

	// @启动任务线程
	void StartTaskThread();

	CGameRCF* GetRCF();
private:
	void InstallHook();
	void LoadCaptLib();
	void InstallGameUiHook();
private:
	std::shared_ptr<CACEHook>   m_Hook;
	std::shared_ptr<CACEMemory> m_Memory;
	static inline CSOWrapper*   m_pInstance        = nullptr;
	ULONG	                    m_CaptchaDlg;
	ULONG	                    m_CaptchaEventOK;
	static inline wchar_t*      m_InputAnswer      = nullptr;
	static inline wchar_t*      m_InputAccount     = nullptr;
	static inline wchar_t*      m_InputPassWord    = nullptr;

	ULONG	                    m_LoginDlgClass;
	ULONG	                    m_LoginCall;
	ULONG						m_2ndPasswordClass;
	// 打码线程
	thread_pool*                m_CaptchThreadPool;
	CGameRCF*                   m_pGameRCF;
};

