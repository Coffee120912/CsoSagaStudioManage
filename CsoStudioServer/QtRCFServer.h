#pragma once
#include "QtConnect.h"
#include <QObject>
#include "CsoStdioPipeInterface.h"
class QtRCFServer  : public QObject
{
	Q_OBJECT

public:
	QtRCFServer(QObject *parent);
	~QtRCFServer();

	bool start();

	void SetLoginInterface(CLoginModule* Login);
	
	
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetAuctionTask();

	//-------------------------------------------------------------------
	// server send to client
	//-------------------------------------------------------------------
	// @ ��ȡ����������
	std::vector<BYTE> GetClientAuctionMetadate(std::string clientSession);


	//-------------------------------------------------------------------
	// @ ���͵��ͻ�����������������
	//-------------------------------------------------------------------
	void SendClientAuctionSearchTask();
private:
	static bool onServiceAccess(int methodId);
private:
	RCF::RcfInit*          m_RcfInit;
	RCF::RcfServer*        m_RcfServer;
	CsoStdioPipeInterface* m_StdioPipeInterface;
	CLoginModule*          m_Login;
};




