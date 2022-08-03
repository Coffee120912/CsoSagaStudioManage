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


	// *�ٱ��߳������û�
	// @Param:��ұ�־
	bool Kick_RoomPlayer(int PlayerSign);

	/*
	*	@�л���Ӫ
	*/
	bool Room_AlterTeam(TEAM Value);

	// @ �뿪����
	bool Room_Disconnect();

	// @ ���뷿��
	bool Room_joinRoom(ULONG RoomIndex);

	// @��ʼ��Ϸ
	bool Room_StartGame();

	// @��ȡÿ�ղ���
	bool Item_Roulette();

	// @ˢս������
	bool Discipline(std::string name);

	// @��������
	bool DoubleExp();

	// @ˢ��������
	bool RefreshAuction();
	
	/*
	*	@ˢ��ָ��ǹ֧
	*	@Param:ǹ֧����
	*	@Param:��ʲô����ˢ�� 0:���� 1:һ�ڼ�
	*	@Param:ǿ�� 0:������ 1:ǿ��0...
	*	@Param:����
	*/
	bool RefreshAuction(short item,byte type,byte strengthening,short Paint);

	// @ˢ����ߵ�
	bool RefreshLefAuction();
	
	// @ˢ��������ָ����Ʒ
	bool RefreshAuctionSpecifyItem(int AuctionID,int TimeStamp,BYTE Row);

	// @һ�ڼ۹���
	bool BuyAuctionPrice(int AuctionID, int TimeStamp, BYTE Row);

	// @ˢ�²ֿ�
	bool RefreshStorage();

	// @ˢ�»����̳�
	bool RefreshScoreShop();

	// @����������
	bool AuctionEventsBidding(int ItemSign, int TimeStamp, int Price);
	
	// @������ȡ�ص���
	bool AuctionEventsGetBack(int AuctionID, int TimeStamp);

	// @�������ջػ���
	bool AuctionGetBackIntegrate(int AuctionID, int TimeStamp);

	// @�������ջ�����
	bool AuctionGetBackWeapon(int AuctionID, int TimeStamp);

	// @������ˢ�»
	bool AuctionEventsRefresh();

private:
	// *����Pkg��
	bool  GeneratePkgBuffer(PackWriter* lPack);
	std::tuple<bool,BYTE> GetPacketSeq();
	bool SendToPacket(PackWriter* lWriterPack);


	//*�ӿ�����.�뿪����
	static void InterfaceDisconnectRoom();
	//*�ӿ�����.���뷿��
	static void InterfaceJoinRoom();
	//*�ӿ�����.��ʼ��Ϸ
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

