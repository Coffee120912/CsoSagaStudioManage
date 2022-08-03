#pragma once
#include "SagaConnect.h"
#include <RCF../../../RCFCsoStudioInterface.hpp>
#include <RCF/RCF.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>

#include "openssl/rc4.h"
#include <PackReader.h>
#include "openssl/bio.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/err.h"
#include "openssl/md5.h"

#include <RCF/ProxyEndpoint.hpp>
#pragma comment(lib,"openssl/win32/lib/libcrypto.lib")
#pragma comment(lib,"openssl/win32/lib/libssl.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"RcfDll.lib")
#include "thread_pool.hpp"


class GameProxyClient;
class CGameRCF
{
public:
	CGameRCF();
	static CGameRCF* GetInstance();


	// @启动线程初始化
	VOID StartThreadRCFInit();

	// @获取登陆账号
	std::string GetLoginUser();

	// @获取登陆密码
	std::string GetLoginUserPassWord();

	// @获取缓存的账号和密码
	std::tuple<std::string, std::string> GetLoginUserInfo();

	/*
	*	@发送登陆时间消息
	*	@Param:消息ID
	*	@Param:消息文本 UTF-8编码
	*/
	bool SendLoginEventNotice(LOGIN_NOTICE_CODE MsgID,std::string MsgStr);

	// @通知登录器初始化完毕
	BOOL LauncherEventNotice();

	// @获取任务模式
	TASK_MODE GetTaskMode();

	// @发送用户信息
	bool SendUserInfo(std::string UserName, ULONG lv);
	// @发送金币信息
	bool SendGoldInfo(ULONG64 Gold);
	// @发送积分信息
	bool SendIntegralInfo(ULONG64 Integral);

	// @心跳维持
	bool SendHeartBeat();

	// @获取拍卖行任务
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionTaskMap();

	// @发送拍卖行通知
	bool SendAutionNotice(AuctionNotice Type, int nTask, PTAG_GAME_AUCTION pAution);

	// @获取拍卖会共享全局功能
	ULONG64 GetShareAuctionEvents();

	// @发送拍卖行上架商品
	bool SendAutionItem(std::map<ULONG, TAG_GAME_AUCTION> & Items,int Activate);

	// @获取拍卖行客户任务
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionClientTaskMap();

	thread_pool* GetWorkThread();
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetClientTaskAuction();
	int GetErrID();
private:
	RCF::RcfInit*                           m_pRcfInit;
	RcfClient<I_CsoStdioPipe>*              m_pClient;
	static inline CGameRCF*                 m_pInstance = nullptr;
	std::string                             m_ClientSession;
	GameProxyClient*                        m_pGameProxyClient;
	std::string                             m_LoginUser;
	std::string                             m_LoginUserPassWord;
	std::string					            m_Guid;
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>	m_AuctionTaskMap;
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>	m_AuctionClientTaskMap;
	thread_pool*                            m_WordThread;
	int                                     m_ErrID;
};

class GameProxyClient
{
public:
	GameProxyClient();

	std::vector<BYTE> UMSG(std::vector<BYTE>& v);
	std::vector<BYTE> GetClientAuctionMetadate();
	std::vector<BYTE> SendClientAuctionTask(std::vector<BYTE>& v);
private:
	thread_pool* m_ClientThread;
	// 当前激活的武器
	ULONG m_ActiveWeapon;
};
