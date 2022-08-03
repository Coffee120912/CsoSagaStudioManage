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


	//事件
	bool WeaponAuctionEvent(std::shared_ptr<PackReader> lPackRead);

	// @解析拍卖行显示
	void ParseAuctionView(std::shared_ptr<PackReader> lPackRead);
	
	// @解析拍卖行
	TAG_GAME_AUCTION& ParseAuctionLine(std::shared_ptr<PackReader> lPackRead);
	
	// @解析拍卖行信息
	TAG_GAME_AUCTION& ParseAuctionViewData(std::shared_ptr<PackReader> lPackRead, TAG_GAME_AUCTION& Data,int Type);


	// @解析拍卖行通知
	void ParseAuctionNotice(std::shared_ptr<PackReader> lPackRead);
	
	// @解析拍卖行刷新指定物品
	void ParseAuctionRefreshSpecifyItem(std::shared_ptr<PackReader> lPackRead);

	// @解析拍卖会上架信息
	void ParseAuctionSellItem(std::shared_ptr<PackReader> lPackRead);

	std::map<ULONG, TAG_GAME_AUCTION>&GetAuctionMap();

	// @运行任务
	void RunTask(PTAG_GAME_AUCTION pAuction);

	// @取当前拍卖特别活动
	PTAG_AUTCION_SPECIAL_EVENTS GetCurrentAuctionSpecialEvents();

	// @解析拍卖会特别活动
	void ParseAuctionSpecialEvents(std::vector<BYTE> Data);

	// @取活动当前时间段可拍卖的物品
	PTAG_AUTCION_SPECIAL_EVENTS_ITEMS GetCanTimeActive();

	// @取是否参加拍卖特别活动
	bool IsJoinAuctionSpecialEvents();
	

	std::map<ULONG, TAG_GAME_AUCTION>& GetItems();
	
private:
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& GetTask();
	// @检查彩漆类型 
	bool CheckPaintType(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction);
	// @检查强化属性
	bool CheckStrengthenAttr(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction);
	// @刷新
	void ThreadRef();

	// @通知服务端上架的武器
	void NotifySellWeapon(int Activate);


private:
	TAG_GAME_AUCTION                  m_AuctionInfo;
	std::map<ULONG, TAG_GAME_AUCTION> m_AuctionMap;
	std::shared_ptr<CACEUtil>		  m_pACEUtil;
	static inline thread_pool         m_WorkThread = thread_pool(1);
	BOOL						      m_bRefStatus;

	ULONG							  m_BuyTaskID;
	int							      m_BuyAutionID;
	// 拍卖会特殊活动
	TAG_AUTCION_SPECIAL_EVENTS		  m_SpecialEvents;	
	// 特别活动拍卖道具信息
	TAG_GAME_AUCTION                  m_SpecialItems;
	ULONG64							  m_AuctionFlag;
	// 上架的物品
	std::map<ULONG, TAG_GAME_AUCTION> m_RackingItems;
};

