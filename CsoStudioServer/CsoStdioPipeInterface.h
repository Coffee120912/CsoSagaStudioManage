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
	// 获取共享信息
	PTAG_GAME_AC_LOGIN GetShareInfo()
	{
		return m_pShareInfo;
	}

	PTAG_GAME_AC_LOGIN m_pShareInfo;
	// 保存的拍卖会ID
	std::set<int> m_SetAuctionID;
	// 进程ID
	int m_ProcessID;

private:
	std::string m_Session;
	
};


class CsoStdioPipeInterface
{
public:
	
	//-------------------------------------------------------------------
	// 实现方法
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
	// 	公共API
	//-------------------------------------------------------------------
	
	// @设置模块指针
	void SetLoginModule(CLoginModule* pThis);
	// @刷新用户界面
	void RefreshUserInterface(const ULONG uID);
	// @查找窗口句柄
	void FindQtWindow();
	// @返回拍卖任务指针
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionTaskThis();
	
	// @刷新拍卖行界面
	void RefreshAuctionInterface(ULONG uID);

	// @刷新拍卖行上架的界面
	void RefreshAuctionShelveInterface();

	// @客户搜索任务通知
	void ClientSearchTaskNotice();

private:
	// 保存的登陆模块指针
	CLoginModule*	m_pLoginInformation;
	//保存的拍卖行任务
	std::map<ULONG, PTAG_GAME_TASK_AUCTION> m_pAutionTaskMap;

	CACEUtil		        m_Util;
	thread_pool*		    m_ThreadPool;
	HWND                    m_hClientHWND;
	TAG_GAME_AUCTION	    m_BuyWeapion;
};



