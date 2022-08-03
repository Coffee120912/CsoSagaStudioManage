#include "GamePack.h"
#include "Util.h"
#include "GameOffset.h"
PfnEVP_DecryptUpdate EVP_DecryptUpdate007 = nullptr;
PfnEVP_EncryptUpdate EVP_EncryptUpdate007 = nullptr;
PfnEVP_EncryptInit	EVP_EncryptInit007    = nullptr;
PfnSend				_Send              = nullptr;
PfnSSL_read			SSL_read007           = nullptr;



CGamePack::CGamePack()
{
	m_PackSeqAddress      = 0;
	m_AceMemory           = g_pUtil->GetAceMemory();
	m_IDisconnectRoomText = "";
}

CGamePack* CGamePack::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGamePack();
	}
	return m_pInstance;
}

bool CGamePack::vInit()
{
	m_IDisconnectRoomText = xorstr_("+PackDisconnectRoom");
	Engine::g_Engine.pfnAddCommand(m_IDisconnectRoomText.data(), InterfaceDisconnectRoom);

	m_IJoinRoomText = xorstr_("+PackJoinRoom");
	Engine::g_Engine.pfnAddCommand(m_IJoinRoomText.data(), InterfaceJoinRoom);

	m_IStartGameText = xorstr_("+PackStartGame");
	Engine::g_Engine.pfnAddCommand(m_IStartGameText.data(), InterfaceStartGame);
	

	return true;
}

bool CGamePack::Kick_RoomPlayer(int PlayerSign)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(0x70);

	lPackWrite->WriteByte(0x01);

	lPackWrite->WriteInt(PlayerSign);

	lPackWrite->WriteByte(0x00);

	lPackWrite->WriteString(xorstr_("0"));

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::Room_AlterTeam(TEAM Value)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(0x41);

	lPackWrite->WriteByte(0x11);
	
	lPackWrite->WriteByte(static_cast<BYTE>(Value));

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::Room_Disconnect()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(0x41);

	lPackWrite->WriteByte(0x02);

	lPackWrite->WriteByte(1);

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::Room_joinRoom(ULONG RoomIndex)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x41);
	lPackWrite->WriteByte(0x17);
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteInt(RoomIndex);
	lPackWrite->WriteByte(0x00);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::Room_StartGame()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x41);
	lPackWrite->WriteByte(0x04);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::Item_Roulette()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	//lPackWrite->WriteByte(0x4E);
	//lPackWrite->WriteByte(0x09);
	//lPackWrite->WriteByte(0x01);
	//GeneratePkgBuffer(lPackWrite.get());

	//lPackWrite->Clear();
	lPackWrite->WriteByte(0x4E);
	lPackWrite->WriteByte(0x08);
	lPackWrite->WriteByte(0x01);
	GeneratePkgBuffer(lPackWrite.get());
	//lPackWrite->Clear();
	//lPackWrite->WriteByte(0x4E);
	//lPackWrite->WriteByte(0x09);
	//lPackWrite->WriteByte(0x00);
	//GeneratePkgBuffer(lPackWrite.get());
	return true;
}

bool CGamePack::Discipline(std::string name)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024 + name.length());
	lPackWrite->WriteByte(0x65);
	lPackWrite->WriteByte(0x01);
	lPackWrite->WriteString(name);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::DoubleExp()
{
	
#if ANTI_DEBUG
	hud_player_info_t Info;

	for (int i = 0; i < 3; i++)
	{
		cl_entity_s* Entity = Engine::g_Engine.GetLocalPlayer();
		Engine::g_Engine.pfnGetPlayerInfo(Entity->index, &Info);

		if (Info.mark)
		{
			auto lPackWrite = std::make_shared<PackWriter>(1024);
			lPackWrite->WriteByte(0x44);
			lPackWrite->WriteByte(0x0B);
			lPackWrite->WriteByte(129);
			lPackWrite->WriteInt(Info.mark);
			lPackWrite->WriteByte(2);
			GeneratePkgBuffer(lPackWrite.get());
		}

	}
#endif
	return true;
}

bool CGamePack::RefreshAuction()
{
	//68 00 01 02 00 00 00 00 00 00
	auto lPackWrite = std::make_shared<PackWriter>(1024 );
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteByte(0x01);	
	lPackWrite->WriteByte(0x02);
	//item代码
	lPackWrite->WriteShort(0);
	//type
	lPackWrite->WriteByte(0);
	//强化LV
	lPackWrite->WriteByte(0);
	
	lPackWrite->WriteByte(0);
	lPackWrite->WriteByte(0);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::RefreshAuction(short item, byte type, byte strengthening,short Paint)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteByte(0x01);
	lPackWrite->WriteByte(0x02);
	//item代码
	if (item == -1)
	{
		item = 0;
	}

	lPackWrite->WriteShort(item);
	//type
	lPackWrite->WriteByte(type);
	//强化LV
	lPackWrite->WriteByte(strengthening);

	// 后面是彩漆名
	lPackWrite->WriteShort(Paint);


	ACEDebugFileLog("[{}] item:{} find_type:{} strengthening:{} PaintID:{}", __FUNCTION__, item, type, strengthening, Paint);

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::RefreshLefAuction()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteByte(0x01);
	lPackWrite->WriteByte(0x01);

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::RefreshAuctionSpecifyItem(int AuctionID, int TimeStamp, BYTE Row)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x02);
	lPackWrite->WriteByte(0x01);

	lPackWrite->WriteInt(AuctionID);

	lPackWrite->WriteInt(TimeStamp);
	
	lPackWrite->WriteByte(Row);
	
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::BuyAuctionPrice(int AuctionID, int TimeStamp, BYTE Row)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x06);		//金钱类型 5 竞标 6 一口价
	lPackWrite->WriteByte(0x01);		//购买类型 1 武器 2 配件
	lPackWrite->WriteByte(Row);			//武器位置索引
	lPackWrite->WriteInt(AuctionID);	//拍卖ID
	lPackWrite->WriteInt(TimeStamp);	

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::RefreshStorage()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x45);
	lPackWrite->WriteByte(0x0C);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::RefreshScoreShop()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x5B);
	lPackWrite->WriteByte(0x12);
	GeneratePkgBuffer(lPackWrite.get());
	lPackWrite->Clear();
	lPackWrite->WriteByte(0x6C);
	lPackWrite->WriteByte(0x02);

	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::AuctionEventsBidding(int ItemSign, int TimeStamp, int Price)
{
	/*
	*	55 1F 10 00 68 11 03 07 21 00 A9 19 08 9E E0 62 10 27 00 00
		55 50 10 00 68 11 03 07 22 00 34 76 08 9E E0 62 10 27 00 00
		55 28 10 00 68 11 03 07 23 00 49 0E 08 9E E0 62 10 27 00 00
	*/
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x11);
	lPackWrite->WriteByte(0x03);
	lPackWrite->WriteByte(0x07);
	lPackWrite->WriteInt(ItemSign);
	lPackWrite->WriteInt(TimeStamp);
	lPackWrite->WriteInt(Price);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::AuctionEventsGetBack(int AuctionID, int TimeStamp)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x07);
	lPackWrite->WriteByte(0x03);

	lPackWrite->WriteInt(AuctionID);
	lPackWrite->WriteInt(TimeStamp);
	lPackWrite->WriteByte(0);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::AuctionGetBackIntegrate(int AuctionID, int TimeStamp)
{

	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x07);
	lPackWrite->WriteByte(0x01);

	lPackWrite->WriteInt(AuctionID);
	lPackWrite->WriteInt(TimeStamp);
	lPackWrite->WriteByte(1);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::AuctionGetBackWeapon(int AuctionID, int TimeStamp)
{
	//55 46 0C 00 68 08 01 B3 14 B6 5D E5 A1 E8 62 01
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x08);
	lPackWrite->WriteByte(0x01);

	lPackWrite->WriteInt(AuctionID);
	lPackWrite->WriteInt(TimeStamp);
	lPackWrite->WriteByte(1);
	return GeneratePkgBuffer(lPackWrite.get());
}

bool CGamePack::AuctionEventsRefresh()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(0x68);
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteByte(0x03);
	lPackWrite->WriteByte(0x02);
	lPackWrite->WriteInt(0);
	lPackWrite->WriteShort(0);
	lPackWrite->WriteByte(0x65);
	
	return  GeneratePkgBuffer(lPackWrite.get());

}

bool CGamePack::GeneratePkgBuffer(PackWriter* lPack)
{
	VMProtectBegin(__FUNCTION__);

	TAG_GAME_PACKET_HEAD PackHead;

	auto result = GetPacketSeq();

	if (std::get<bool>(result) == false)
	{
		return false;
	}

	PackHead.Seq = std::get<BYTE>(result);

	PackHead.Lenght = lPack->GetBytesLength();

	PackWriter  WriterPackBuff = PackWriter(PackHead.Lenght + sizeof(TAG_GAME_PACKET_HEAD));

	//写入头部
	WriterPackBuff.WriteBytes((BYTE*)&PackHead, sizeof(TAG_GAME_PACKET_HEAD));

	WriterPackBuff.WriteBytes(lPack->GetBytes(), PackHead.Lenght);

	VMProtectEnd();
	return SendToPacket(&WriterPackBuff);;

}
std::tuple<bool, BYTE> CGamePack::GetPacketSeq()
{
	if (!m_PackSeqAddress)
	{
		//Init
		if (m_AceMemory->GetScanCode(xorstr_("56 8B F1 8B 0D ?? ?? ?? ?? 8B 06 FF 70 F4 50"), m_PackSeqAddress, g_Offset.GetEngineModuleInfo()->hModule))
		{
			m_PackSeqAddress = *(PDWORD)(m_PackSeqAddress + 5);
		}
		ACEDebugFileLog("[{}] PackSeqAddress:{:X}",__FUNCTION__,m_PackSeqAddress);
	}

	int BASE =  m_AceMemory->Read<int>(m_PackSeqAddress,0x4);

	if (BASE == 0xFFFFFFFF || !BASE)
		return std::make_tuple<bool,BYTE>(false,0);

	BYTE* Seq = (PBYTE)(BASE + 0x3A);

	return std::make_tuple<bool, BYTE>(true, InterlockedIncrement((LPLONG)Seq)); 
}

bool CGamePack::SendToPacket(PackWriter* lWriterPack)
{
	VMProtectBegin(__FUNCTION__);

	std::vector<BYTE> VecEncrypt;
	//分配2倍大小
	VecEncrypt.resize(lWriterPack->GetSize() * 2);

	int EncryptOutLengt = 0;

	int Ret = EVP_EncryptUpdate007((void*)&this->m_ctx, VecEncrypt.data(), &EncryptOutLengt, lWriterPack->GetBytes(), lWriterPack->GetSize());
	if (!Ret)
		return false;

	Ret = send(this->m_Socket, (char*)VecEncrypt.data(), EncryptOutLengt, 0);

	if (!Ret)
		return false;

	VMProtectEnd();

	return true;
}

void CGamePack::InterfaceDisconnectRoom()
{
	CGamePack::GetInstance()->Room_Disconnect();
}

void CGamePack::InterfaceJoinRoom()
{
	
}

void CGamePack::InterfaceStartGame()
{
	CGamePack::GetInstance()->Room_StartGame();
}

int WINAPIV CGamePack::extEVP_EncryptInit(VOID* ctx, const VOID* type, unsigned char* key, unsigned char* iv)
{
	int ret = EVP_EncryptInit007(ctx, type, key, iv);

	// 初始化 evp_cipher_ctx_st 结构指针
	CGamePack::GetInstance()->m_ctx = *(evp_cipher_ctx_st*)ctx;

	return ret;
}

int WSAAPI CGamePack::extSend(SOCKET s, const char* buf, int len, int flags)
{
	int Ret = send(s, buf, len, flags);
	// 设置 SOCKET
	CGamePack::GetInstance()->m_Socket = s;

	//ACEDebugFileLog("[{}] s:0x{:X}", __FUNCTION__, s);
	return Ret;
}

int WINAPIV CGamePack::extEVP_EncryptUpdate(VOID* ctx, unsigned char* out, int* outl, unsigned char* in, int inl)
{
#if ANTI_DEBUG
	//原始的封包
	CGamePack::GetInstance()->PostOnDispatch(in, inl);
#endif
	
	//加密的封包
	int ret = EVP_EncryptUpdate007(ctx, out, outl, in, inl);
	return ret;
}

int WSAAPI FAR CGamePack::extsendto(_In_ SOCKET s, const char FAR* buf, _In_ int len, _In_ int flags, const struct sockaddr FAR* to, _In_ int tolen)
{
	std::vector<BYTE> VecPack(len);

	memcpy(VecPack.data(), buf, len);

	PackReader Pack = PackReader(VecPack);

	ULONG ulFlags = Pack.ReadChar();


	if (ulFlags == 0x56)
	{
		ULONG v1 = Pack.ReadInt();
		BYTE  v2 = Pack.ReadChar();

		ACEWarningFileLog("extsendto:{:02X}",fmt::join(VecPack.begin(), VecPack.end()," "));

	}




	return sendto(s, (const char*)VecPack.data(), VecPack.size(), flags, to, tolen);

}

void CGamePack::PostOnDispatch(unsigned char* out, int inl)
{
	BYTE* CurrePackAddres = out;
	int		CurrePackSize = 0;

	do
	{
		auto lReader = std::make_shared<PackReader>(CurrePackAddres, 4);

		GAME_PACKET_HEAD Ver;
		Ver.Head = lReader->ReadChar();
		if (Ver.Head != 0x55)
		{
			ACEErrorFileLog("[{}] head not 0x55 size :0x{:X} head:0x{:X}", __FUNCTION__, inl, Ver.Head);
			return;
		}
		Ver.Seq                 = lReader->ReadChar();
		Ver.Lenght              = lReader->ReadShort();
		// 开始解包
		auto lReaderPack        = std::make_shared<PackReader>(CurrePackAddres + 4, Ver.Lenght);

		int Index = lReaderPack->ReadChar();

		auto VecPack = std::vector<BYTE>(inl);

		memcpy(VecPack.data(), out, inl);


		ACEWarningFileLog("[{}] ->Index = {}  Size 0x{:X} {:02X}", __FUNCTION__, Index, inl,fmt::join(VecPack.begin(), VecPack.end()," "));



		
		switch (Index)
		{

		case Packet_Report:
		{
			ULONG CheckType = lReaderPack->ReadShort();
			if (CheckType == 5 || CheckType == 8)
			{
				lReaderPack->SetMemProtect(PAGE_READWRITE);
				lReaderPack->WriteShort(0, 1);
			}
			break;
		}


		case Packet_WeaponAuctionEvent:
		{

			break;
		}
		case Packet_ReverseAuctionSystem:
		{
			break;
		}
		case Packet_Host:
		{
			break;
		}
		default:

			break;
		}

		CurrePackSize += Ver.Lenght + 4;

		if (CurrePackSize >= inl) //全部解包完成！
			break;
		// 继续循环解析下个封包
		CurrePackAddres += Ver.Lenght + 4;

	} while (true);
}
