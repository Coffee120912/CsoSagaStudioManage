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


	// @�����̳߳�ʼ��
	VOID StartThreadRCFInit();

	// @��ȡ��½�˺�
	std::string GetLoginUser();

	// @��ȡ��½����
	std::string GetLoginUserPassWord();

	// @��ȡ������˺ź�����
	std::tuple<std::string, std::string> GetLoginUserInfo();

	/*
	*	@���͵�½ʱ����Ϣ
	*	@Param:��ϢID
	*	@Param:��Ϣ�ı� UTF-8����
	*/
	bool SendLoginEventNotice(LOGIN_NOTICE_CODE MsgID,std::string MsgStr);

	// @֪ͨ��¼����ʼ�����
	BOOL LauncherEventNotice();

	// @��ȡ����ģʽ
	TASK_MODE GetTaskMode();

	// @�����û���Ϣ
	bool SendUserInfo(std::string UserName, ULONG lv);
	// @���ͽ����Ϣ
	bool SendGoldInfo(ULONG64 Gold);
	// @���ͻ�����Ϣ
	bool SendIntegralInfo(ULONG64 Integral);

	// @����ά��
	bool SendHeartBeat();

	// @��ȡ����������
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionTaskMap();

	// @����������֪ͨ
	bool SendAutionNotice(AuctionNotice Type, int nTask, PTAG_GAME_AUCTION pAution);

	// @��ȡ�����Ṳ��ȫ�ֹ���
	ULONG64 GetShareAuctionEvents();

	// @�����������ϼ���Ʒ
	bool SendAutionItem(std::map<ULONG, TAG_GAME_AUCTION> & Items,int Activate);

	// @��ȡ�����пͻ�����
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
	// ��ǰ���������
	ULONG m_ActiveWeapon;
};
