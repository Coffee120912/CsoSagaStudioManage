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
	//*��ʼ��Hook
	bool InitializeHook();
	//*��װHook
	BOOL InstallHook(ULONG Index, GAME_PACKET_TYPE_INFO_HOOK* pInfo);

	static void _stdcall DEBUG_PacketDispatch(PTAG_PEB_STACK Stack);
	static void _stdcall PacketDispatch(PTAG_PEB_STACK Stack);
	static void CallHookAddres(PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO Info);

	static PTAG_PEB_STACK GetPacketStack(Packet_Type type);
	static PTAG_GAME_PACKET_TYPE_INFO GetPacketTypeInfo(Packet_Type type);

	static BYTE CallGamePacket(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);
	static BYTE CallGamePacket2(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);

	static ULONG  WINAPI extMetadataParsing(PTAG_PEB_STACK Stack);

	//hook���ɷ��������
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
	

	
	//@����Pack��ָ��
	PVOID GetPackThis(Packet_Type type);

	void Debug_Packet(Packet_Type type);

	// @ȡ����������
	std::vector<BYTE> GetAuctionMetadate();

	// @ȡ�������ر�
	std::vector<BYTE> GetAuctionSpecialEventsMetadate();
	
private:
	//*�ӿ�����.�������ط���
	static void InterfaceCommandPacketSuperRetRoom();

	PSHORT GetItemCount();

	PSHORT GetItemSite();
private:
	static inline CDispatchRecv* m_Instance = nullptr;
	ULONG                        m_PacketInfo;
	CGamePack*					 m_pGamePack;
	//��������
	short	                     m_ItemCount;
	//���߱�־
	short	                     m_ItemSite;
	std::shared_ptr<PackWriter>  m_lPackInventory;
	//������ַ					
	ULONG						m_SeCallAddress;
	CAuctionSystem				m_AuctionSystem;
	
	std::map<int,std::vector<BYTE>> m_MapMetadate;
};

