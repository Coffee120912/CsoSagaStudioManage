#pragma once
#include "Util.h"

//��Ϣ֪ͨ
typedef std::function<void(MSG_ID MsgId,std::string MsgStr, PTAG_GAME_AC_LOGIN Client)> LoginModuleMessage;
//�˳����̴���
#define EXIT_CODE	0x1234


class CLoginModule
{
public:
	CLoginModule();

	// @��ʼ��
	bool Init(std::string GameAPP,std::string GamePath, LoginModuleMessage MsgPointer);

	// @��ӵ�½��Ϣ����mapId
	ULONG AddLoginInfo(PTAG_SE_GAME_AC_CLIENT pDataClient);
	
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
private:
	// ��½��ʱʱ��
	ULONG                               m_ulTimeout;
	//��½��Ϣ
	std::map<ULONG, PTAG_GAME_AC_LOGIN> m_MapLogin;
	std::string                         m_GameAPP;
	std::string                         m_GamePath;
	ULONG                               m_uConut;
	std::shared_ptr<CACEUtil>		    m_pACEUtil;
	LoginModuleMessage					m_msg;
	std::map<DWORD, PLocaProcessInfo>	m_MapProcessInfo;
};

