#pragma once
#include "PacketDef.h"
#include "GameClienBase.hpp"
#include "GamePack.h"
#include "Util.h"
#include "CAuctionSystem.h"
#include "CVars.h"
class CDispatchRecv : public IGameClient
{
public:
	CDispatchRecv();
	static CDispatchRecv* GetInstance();

	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual void vHUD_Frame(double time);
	virtual bool vInit();
	//*初始化Hook
	bool InitializeHook();
	//*安装Hook
	BOOL InstallHook(ULONG Index, GAME_PACKET_TYPE_INFO_HOOK* pInfo);

	static void _stdcall DEBUG_PacketDispatch(PTAG_PEB_STACK Stack);
	static void _stdcall PacketDispatch(PTAG_PEB_STACK Stack);
	static void CallHookAddres(PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO Info);

	static PTAG_PEB_STACK GetPacketStack(Packet_Type type);
	static PTAG_GAME_PACKET_TYPE_INFO GetPacketTypeInfo(Packet_Type type);

	static BYTE CallGamePacket(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);
	static BYTE CallGamePacket2(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);

	static ULONG  WINAPI extMetadataParsing(PTAG_PEB_STACK Stack);

	//hook的派发封包功能
	ODG_EXPORT static int _stdcall Fake_Reply(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UMsg(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Kick(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UserStart(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UpRoom(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Metadata(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Inventory(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Quest(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Option(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Null(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Ribbon(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Hack(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Host(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Item(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_ClientCheck(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Transfer(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fack_ReverseAuctionSystem(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fack_WeaponAuctionEvent(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UserUpdateInfo(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Character(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	

	
	//@返回Pack包指针
	PVOID GetPackThis(Packet_Type type);

	void Debug_Packet(Packet_Type type);

	// @取拍卖行数据
	std::vector<BYTE> GetAuctionMetadate();

	// @取拍卖行特别活动
	std::vector<BYTE> GetAuctionSpecialEventsMetadate();
	
private:
	//*接口命令.超级返回房间
	static void InterfaceCommandPacketSuperRetRoom();

	PSHORT GetItemCount();

	PSHORT GetItemSite();
private:
	static inline CDispatchRecv* m_Instance = nullptr;
	ULONG                        m_PacketInfo;
	CGamePack*					 m_pGamePack;
	//道具数量
	short	                     m_ItemCount;
	//道具标志
	short	                     m_ItemSite;
	std::shared_ptr<PackWriter>  m_lPackInventory;
	//二级地址					
	ULONG						m_SeCallAddress;
	CAuctionSystem				m_AuctionSystem;
	
	std::map<int,std::vector<BYTE>> m_MapMetadate;
};

