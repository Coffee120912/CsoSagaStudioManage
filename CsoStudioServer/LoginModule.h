#pragma once
#include "Util.h"
#include "thread_pool.hpp"

//��Ϣ֪ͨ
typedef std::function<void(MSG_ID MsgId,std::string MsgStr, PTAG_GAME_AC_LOGIN Client)> LoginModuleMessage;
//�˳����̴���
#define EXIT_CODE	0x1234


class CLoginModule
{
public:
	CLoginModule();
	
	static CLoginModule* GetInstance();

	// @��ʼ��
	bool Init(std::string GameAPP,std::string GamePath, LoginModuleMessage MsgPointer);

	// @��ӵ�½��Ϣ
	PTAG_GAME_AC_LOGIN AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient);
	
	// @��������
	void StartProcess();

	// @�ȴ��߳̽����߳�
	void WaitForProcessThread(PROCESS_INFORMATION Process, PTAG_GAME_AC_LOGIN pInfo);

	// @�����Ϸ�����߳�
	bool CheckGameUpdataThread();

	// @��ȡ���ؽ��̽ṹ	
	std::map<DWORD, PLocaProcessInfo>& GetMapProcess();

	// @������Ϸ����
	bool StartGameProcess(PTAG_GAME_AC_LOGIN pInfo);

	// @��ѯ�û��˺�
	PTAG_GAME_AC_LOGIN SearchUserName(std::string UserName);

	// @���ص�½��Ϣ
	std::map<ULONG, PTAG_GAME_AC_LOGIN>& GetMapLogin();

	// @����ָ������
	bool EndProcess(std::string Guid);

	// @ֹͣ��½�˺�
	bool StopLogin(BOOL bStatus);

	// @��������߳�
	void HeartBeatThread();

	/*
	*	@���ص�¼�߳�״̬
	*	@return true:�߳������� false:�߳��Ѿ�����
	*/
	BOOL GetLoginThreadStatus();


	// @�˺��Ƿ��Ѿ���½
	BOOL IsLogin(std::string UserName);

	// @����ĵ�½��ϢID��������
	void ReSortLoginInfo();

	// @��յ�½��Ϣ
	void ClearLoginInfo();
	
	// @����ȫ�ֹ�����
	void SetGlobalShare(PTAG_GAME_AC_LOGIN pInfo);

	GLOBAL_FUNCTION& GetGlobalShare();
private:
	// ��½��ʱʱ��
	ULONG                               m_ulTimeout;
	//��½��Ϣ
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

