#pragma once
#include "QtConnect.h"
#include "LoginModule.h"
#include "thread_pool.hpp"
#include <set>

class CsoStudioSession
{
public:
	CsoStudioSession()
	{
		m_pShareInfo = nullptr;
		m_ProcessID  = 0;
	};
	~CsoStudioSession();
	std::string& GenerateSession()
	{
		CACEUtil Util;
		return m_Session = Util.GetTimeStampStr();
	}
	std::string& GetSessionStr()
	{
		return m_Session;
	}
	// ��ȡ������Ϣ
	PTAG_GAME_AC_LOGIN GetShareInfo()
	{
		return m_pShareInfo;
	}

	PTAG_GAME_AC_LOGIN m_pShareInfo;
	// �����������ID
	std::set<int> m_SetAuctionID;
	// ����ID
	int m_ProcessID;

private:
	std::string m_Session;
	
};


class CsoStdioPipeInterface
{
public:
	
	//-------------------------------------------------------------------
	// ʵ�ַ���
	//-------------------------------------------------------------------
	
	std::vector<BYTE> Connect();
	std::vector<BYTE> GetLoginUserName(ULONG ProcessPid);
	std::string  GetLoginUserPassWord();
	BOOL  CSONMEventNotice(ULONG MsgID, std::string MsgStr);
	std::string  GetEventGuid(ULONG ProcessPid);
	BOOL  GetTaskMode(ULONG &TaskMode);
	BOOL SendGameUserInfo(std::vector<BYTE>& pGameUserInfo);
	BOOL Timestamp();
	std::vector<BYTE> GetAuctionTask();
	BOOL AuctionNotice(std::vector<BYTE>& pAuctionNotice);
	ULONG64 GetAuctionEventsStatus();
	BOOL SendAuctionRackingItem(std::vector<BYTE>&);


	//-------------------------------------------------------------------
	// 	����API
	//-------------------------------------------------------------------
	
	// @����ģ��ָ��
	void SetLoginModule(CLoginModule* pThis);
	// @ˢ���û�����
	void RefreshUserInterface(const ULONG uID);
	// @���Ҵ��ھ��
	void FindQtWindow();
	// @������������ָ��
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionTaskThis();
	
	// @ˢ�������н���
	void RefreshAuctionInterface(ULONG uID);

	// @ˢ���������ϼܵĽ���
	void RefreshAuctionShelveInterface();

	// @�ͻ���������֪ͨ
	void ClientSearchTaskNotice();

private:
	// ����ĵ�½ģ��ָ��
	CLoginModule*	m_pLoginInformation;
	//���������������
	std::map<ULONG, PTAG_GAME_TASK_AUCTION> m_pAutionTaskMap;

	CACEUtil		        m_Util;
	thread_pool*		    m_ThreadPool;
	HWND                    m_hClientHWND;
	TAG_GAME_AUCTION	    m_BuyWeapion;
};



