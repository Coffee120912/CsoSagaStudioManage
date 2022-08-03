#pragma once
#include "PacketDef.h"
#include "GamePack.h"
#include "Util.h"
#include "GamePack.h"
#include "CSOWrapper.h"
#include "thread_pool.hpp"
class CAuctionSystem
{
public:
	CAuctionSystem();
	~CAuctionSystem();

	void Init();


	//�¼�
	bool WeaponAuctionEvent(std::shared_ptr<PackReader> lPackRead);

	// @������������ʾ
	void ParseAuctionView(std::shared_ptr<PackReader> lPackRead);
	
	// @����������
	TAG_GAME_AUCTION& ParseAuctionLine(std::shared_ptr<PackReader> lPackRead);
	
	// @������������Ϣ
	TAG_GAME_AUCTION& ParseAuctionViewData(std::shared_ptr<PackReader> lPackRead, TAG_GAME_AUCTION& Data,int Type);


	// @����������֪ͨ
	void ParseAuctionNotice(std::shared_ptr<PackReader> lPackRead);
	
	// @����������ˢ��ָ����Ʒ
	void ParseAuctionRefreshSpecifyItem(std::shared_ptr<PackReader> lPackRead);

	// @�����������ϼ���Ϣ
	void ParseAuctionSellItem(std::shared_ptr<PackReader> lPackRead);

	std::map<ULONG, TAG_GAME_AUCTION>&GetAuctionMap();

	// @��������
	void RunTask(PTAG_GAME_AUCTION pAuction);

	// @ȡ��ǰ�����ر�
	PTAG_AUTCION_SPECIAL_EVENTS GetCurrentAuctionSpecialEvents();

	// @�����������ر�
	void ParseAuctionSpecialEvents(std::vector<BYTE> Data);

	// @ȡ���ǰʱ��ο���������Ʒ
	PTAG_AUTCION_SPECIAL_EVENTS_ITEMS GetCanTimeActive();

	// @ȡ�Ƿ�μ������ر�
	bool IsJoinAuctionSpecialEvents();
	

	std::map<ULONG, TAG_GAME_AUCTION>& GetItems();
	
private:
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetTask();
	// @���������� 
	bool CheckPaintType(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction);
	// @���ǿ������
	bool CheckStrengthenAttr(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction);
	// @ˢ��
	void ThreadRef();

	// @֪ͨ������ϼܵ�����
	void NotifySellWeapon(int Activate);


private:
	TAG_GAME_AUCTION                  m_AuctionInfo;
	std::map<ULONG, TAG_GAME_AUCTION> m_AuctionMap;
	std::shared_ptr<CACEUtil>		  m_pACEUtil;
	static inline thread_pool         m_WorkThread = thread_pool(1);
	BOOL						      m_bRefStatus;

	ULONG							  m_BuyTaskID;
	int							      m_BuyAutionID;
	// ����������
	TAG_AUTCION_SPECIAL_EVENTS		  m_SpecialEvents;	
	// �ر�����������Ϣ
	TAG_GAME_AUCTION                  m_SpecialItems;
	ULONG64							  m_AuctionFlag;
	// �ϼܵ���Ʒ
	std::map<ULONG, TAG_GAME_AUCTION> m_RackingItems;
};

