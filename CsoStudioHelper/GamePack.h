#pragma once
#include "PacketDef.h"
#include "GameClienBase.hpp"

class CGamePack :public IGameClient
{
public:
	CGamePack();
	static CGamePack* GetInstance();

	virtual bool vInit();



	static int WINAPIV extEVP_EncryptInit(VOID* ctx, const VOID* type, unsigned char* key, unsigned char* iv);
	static int WSAAPI  extSend(SOCKET s, const char* buf, int len, int flags);
	static int WINAPIV extEVP_EncryptUpdate(VOID* ctx, unsigned char* out, int* outl, unsigned char* in, int inl);

	static int WSAAPI FAR extsendto(
		_In_ SOCKET s,
	 const char FAR* buf,
		_In_ int len,
		_In_ int flags,
	 const struct sockaddr FAR* to,
		_In_ int tolen);

	void PostOnDispatch(unsigned char* out, int inl);


	// *举报踢出房间用户
	// @Param:玩家标志
	bool Kick_RoomPlayer(int PlayerSign);

	/*
	*	@切换阵营
	*/
	bool Room_AlterTeam(TEAM Value);

	// @ 离开房间
	bool Room_Disconnect();

	// @ 进入房间
	bool Room_joinRoom(ULONG RoomIndex);

	// @开始游戏
	bool Room_StartGame();

	// @领取每日补给
	bool Item_Roulette();

	// @刷战场纪律
	bool Discipline(std::string name);

	// @三倍经验
	bool DoubleExp();

	// @刷新拍卖行
	bool RefreshAuction();
	
	/*
	*	@刷新指定枪支
	*	@Param:枪支索引
	*	@Param:按什么类型刷新 0:竞拍 1:一口价
	*	@Param:强化 0:无限制 1:强化0...
	*	@Param:喷漆
	*/
	bool RefreshAuction(short item,byte type,byte strengthening,short Paint);

	// @刷新左边的
	bool RefreshLefAuction();
	
	// @刷新拍卖行指定物品
	bool RefreshAuctionSpecifyItem(int AuctionID,int TimeStamp,BYTE Row);

	// @一口价购买
	bool BuyAuctionPrice(int AuctionID, int TimeStamp, BYTE Row);

	// @刷新仓库
	bool RefreshStorage();

	// @刷新积分商城
	bool RefreshScoreShop();

	// @拍卖会活动竞拍
	bool AuctionEventsBidding(int ItemSign, int TimeStamp, int Price);
	
	// @拍卖会取回道具
	bool AuctionEventsGetBack(int AuctionID, int TimeStamp);

	// @拍卖会收回积分
	bool AuctionGetBackIntegrate(int AuctionID, int TimeStamp);

	// @拍卖会收回武器
	bool AuctionGetBackWeapon(int AuctionID, int TimeStamp);

	// @拍卖会刷新活动
	bool AuctionEventsRefresh();

private:
	// *生成Pkg包
	bool  GeneratePkgBuffer(PackWriter* lPack);
	std::tuple<bool,BYTE> GetPacketSeq();
	bool SendToPacket(PackWriter* lWriterPack);


	//*接口命令.离开房间
	static void InterfaceDisconnectRoom();
	//*接口命令.加入房间
	static void InterfaceJoinRoom();
	//*接口命令.开始游戏
	static void InterfaceStartGame();

private:
	static inline CGamePack*    m_pInstance = nullptr;
	DWORD	                    m_PackSeqAddress;
	std::shared_ptr<CACEMemory>	m_AceMemory;
	evp_cipher_ctx_st           m_ctx;
	SOCKET                      m_Socket;
	std::string	                m_IDisconnectRoomText;
	std::string	                m_IJoinRoomText;
	std::string	                m_IStartGameText;

};

