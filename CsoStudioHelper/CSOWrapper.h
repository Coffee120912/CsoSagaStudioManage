#pragma once
#include "SagaConnect.h"
#include "GameClienBase.hpp"
#include "thread_pool.hpp"
#include "GameRCF.h"

// �Զ���¼
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
	//��֤��ģ���ʼ��
	static inline	void(__stdcall* CaptInit)() = nullptr;
	//��֤��ͼƬ·��
	static inline char* (__stdcall* CaptPath)(char* Path) = nullptr;
	// @CSO��½�¼�����
	static void _stdcall CSONMEventListener(PTAG_PEB_STACK Stack);
	// @��½�ص�
	static void _stdcall LoginCallBack(PTAG_PEB_STACK Stack);
	// @��֤�����
	static void _stdcall exCaptcha(PTAG_PEB_STACK Stack);
	// @��ȡ��½��ָ��
	static void _stdcall exVguiGetLoginClass(PTAG_PEB_STACK Stack);

	// @��Ϸ��¼
	void GameLogin();
	// @���òֿ��������
	void Set2ndPassWord(PASSWORD_2ND_TYPE Type,int Value);
	// @��֤������
	void CaptchCall(std::string Type);

	// @���������߳�
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
	// �����߳�
	thread_pool*                m_CaptchThreadPool;
	CGameRCF*                   m_pGameRCF;
};

