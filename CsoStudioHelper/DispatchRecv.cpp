#include "DispatchRecv.h"
#include "GameOffset.h"
#include "Util.h"
#include <regex>
#include "CSOWrapper.h"
#include <csv2/writer.hpp>



CDispatchRecv::CDispatchRecv()
{
	m_SeCallAddress = 0;
	m_PacketInfo    = 0;
	m_pGamePack     = nullptr;
	m_ItemCount     = 0;
	m_ItemSite      = 0;

}

CDispatchRecv* CDispatchRecv::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CDispatchRecv();
	}
	return m_Instance;
}

int CDispatchRecv::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	static TAG_GAME_AUCTION Info;
	
	if (wParam == VK_F1)
	{
		auto fun = [&]() {
			

			for (auto & iter : m_AuctionSystem.GetItems())
			{
				if (iter.second.Price)
				{
					CGamePack::GetInstance()->BuyAuctionPrice(iter.second.AuctionID, iter.second.AddedTimer, 0xE);
				}
			}

		};

		std::thread(fun).detach();
		

	}
	//if (wParam == VK_F2)
	//{
	//	auto v1 = m_AuctionSystem.GetAuctionMap();

	//	Info = v1.at(7);
	//	ACEDebugFileLog("购买的价格:{}", Info.Price);
	//	CGamePack::GetInstance()->BuyAuctionPrice(Info.AuctionID, Info.AddedTimer, Info.Row);

	//}
	//if (wParam == VK_F3)
	//{

	//	//CGamePack::GetInstance()->Item_Roulette();

	//}
	return 0;
}

void CDispatchRecv::vHUD_Frame(double time)
{
	if (CSOWrapper::GetInstance()->GetRCF()->GetErrID())
	{
		ACEDebugFileLog("[{}] 出现错误ID",__FUNCTION__);

#if !ANTI_DEBUG
		exit(0);
#endif
		
	}
}

bool CDispatchRecv::vInit()
{
	InitializeHook();

	Engine::g_Engine.pfnAddCommand("+SuperRetRoom", InterfaceCommandPacketSuperRetRoom);

	m_pGamePack = CGamePack::GetInstance();

	m_ItemSite = 20000;

	m_lPackInventory = std::make_shared<PackWriter>(0x1024 * 100);
		

	return true;
}
GAME_PACKET_TYPE_INFO  GamePacketList[] =
{
#if ANTI_DEBUG
#define xorstr_DEBUG			xorstr_
#else
#define xorstr_DEBUG(str)		xorstr("").crypt_get()
#endif
	GAME_PACKET_TYPE_INFO(Packet_Version	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Version")),
	GAME_PACKET_TYPE_INFO(Packet_Reply	                                 ,(ULONG)CDispatchRecv::Fake_Reply, true,  xorstr_DEBUG("Packet_Reply")),
	GAME_PACKET_TYPE_INFO(Packet_Transfer	                             ,(ULONG)CDispatchRecv::Fake_Transfer,false,  xorstr_DEBUG("Packet_Transfer")),
	GAME_PACKET_TYPE_INFO(Packet_ServerList	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerList")),
	GAME_PACKET_TYPE_INFO(Packet_Character	                             ,(ULONG)CDispatchRecv::Fake_Character, true,  xorstr_DEBUG("Packet_ServerList")),
	GAME_PACKET_TYPE_INFO(Packet_Crypt	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Crypt")),
	GAME_PACKET_TYPE_INFO(Packet_MileageBingo	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MileageBingo")),
	GAME_PACKET_TYPE_INFO(Packet_SessionID	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SessionID")),
	GAME_PACKET_TYPE_INFO(Packet_UpROOM	                                 ,(ULONG)CDispatchRecv::Fake_UpRoom, false,  xorstr_DEBUG("Packet_UpROOM")),
	GAME_PACKET_TYPE_INFO(Packet_ClientCheck	                         ,(ULONG)CDispatchRecv::Fake_ClientCheck, false,  xorstr_DEBUG("Packet_ClientCheck")),
	GAME_PACKET_TYPE_INFO(Packet_UMsg	                                 ,(ULONG)CDispatchRecv::Fake_UMsg, true,  xorstr_DEBUG("Packet_UMsg")),
	GAME_PACKET_TYPE_INFO(Packet_Host	                                 ,(ULONG)CDispatchRecv::Fake_Host, false,  xorstr_DEBUG("Packet_Host")),
	GAME_PACKET_TYPE_INFO(Packet_UDP	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UDP")),
	GAME_PACKET_TYPE_INFO(Packet_Clan	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Clan")),
	GAME_PACKET_TYPE_INFO(Packet_Shop	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Shop")),
	GAME_PACKET_TYPE_INFO(Packet_Rank	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Rank")),
	GAME_PACKET_TYPE_INFO(Packet_Ban	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Ban")),
	GAME_PACKET_TYPE_INFO(Packet_Option	                                 ,(ULONG)CDispatchRecv::Fake_Option, false,  xorstr_DEBUG("Packet_Option")),
	GAME_PACKET_TYPE_INFO(Packet_Favorite	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Favorite")),
	GAME_PACKET_TYPE_INFO(Packet_Item	                                 ,(ULONG)CDispatchRecv::Fake_Item, false,  xorstr_DEBUG("Packet_Item")),
	GAME_PACKET_TYPE_INFO(Packet_QuickStart	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_QuickStart")),
	GAME_PACKET_TYPE_INFO(Packet_Quest	                                 ,(ULONG)CDispatchRecv::Fake_Quest,	  false,  xorstr_DEBUG("Packet_Quest")),
	GAME_PACKET_TYPE_INFO(Packet_Hack	                                 ,(ULONG)CDispatchRecv::Fake_Hack,	  true,  xorstr_DEBUG("Packet_Hack")),
	GAME_PACKET_TYPE_INFO(Packet_Metadata	                             ,(ULONG)CDispatchRecv::Fake_Metadata, true,  xorstr_DEBUG("Packet_Metadata")),
	GAME_PACKET_TYPE_INFO(Packet_PlayerInfo	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PlayerInfo")),
	GAME_PACKET_TYPE_INFO(Packet_GameMatch	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GameMatch")),
	GAME_PACKET_TYPE_INFO(Packet_UpdateWarehouse	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UpdateWarehouse")),
	GAME_PACKET_TYPE_INFO(Packet_Ribbon	                                 ,(ULONG)CDispatchRecv::Fake_Ribbon, false,  xorstr_DEBUG("Packet_Ribbon")),
	GAME_PACKET_TYPE_INFO(Packet_VoxelOutUI	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_VoxelOutUI")),
	GAME_PACKET_TYPE_INFO(Packet_Analysis	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Analysis")),
	GAME_PACKET_TYPE_INFO(Packet_PopularInfo	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PopularInfo")),
	GAME_PACKET_TYPE_INFO(Packet_GameMatchRoomList	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GameMatchRoomList")),
	GAME_PACKET_TYPE_INFO(Packet_UserStart	                             ,(ULONG)CDispatchRecv::Fake_UserStart, true,  xorstr_DEBUG("Packet_UserStart")),
	GAME_PACKET_TYPE_INFO(Packet_Inventory	                             ,(ULONG)CDispatchRecv::Fake_Inventory, false,  xorstr_DEBUG("Packet_Inventory")),
	GAME_PACKET_TYPE_INFO(Packet_Lobby	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Lobby")),
	GAME_PACKET_TYPE_INFO(Packet_Inventory2	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Inventory2")),
	GAME_PACKET_TYPE_INFO(Packet_UserUpdateInfo	                         ,(ULONG)CDispatchRecv::Fake_UserUpdateInfo, true,  xorstr_DEBUG("Packet_UserUpdateInfo")),
	GAME_PACKET_TYPE_INFO(Packet_Event	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Event")),
	GAME_PACKET_TYPE_INFO(Packet_ZombieScenarioMaps	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ZombieScenarioMaps")),
	GAME_PACKET_TYPE_INFO(Packet_Alarm	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Alarm")),
	GAME_PACKET_TYPE_INFO(Packet_VipSystem	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_VipSystem")),
	GAME_PACKET_TYPE_INFO(Packet_Kick	                                 ,(ULONG)CDispatchRecv::Fake_Kick, false,  xorstr_DEBUG("Packet_Kick")),
	GAME_PACKET_TYPE_INFO(Packet_Friend	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Friend")),
	GAME_PACKET_TYPE_INFO(Packet_Expedition	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Expedition")),
	GAME_PACKET_TYPE_INFO(Packet_League	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_League")),
	GAME_PACKET_TYPE_INFO(Packet_SantaAnim	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SantaAnim")),

    GAME_PACKET_TYPE_INFO(Packet_UpdateInfo	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UpdateInfo")),
    GAME_PACKET_TYPE_INFO(Packet_ClassInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ClassInven")),
    GAME_PACKET_TYPE_INFO(Packet_PartsInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PartsInven")),
	GAME_PACKET_TYPE_INFO(Packet_ItemInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ItemInven")),
	GAME_PACKET_TYPE_INFO(Packet_FreePassWeaponInven	                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_FreePassWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_Dictionary	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Dictionary")),
	GAME_PACKET_TYPE_INFO(Packet_GuideQuest	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GuideQuest")),
	GAME_PACKET_TYPE_INFO(Packet_Statistico	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Statistico")),
	GAME_PACKET_TYPE_INFO(Packet_MileageShop	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MileageShop")),
	GAME_PACKET_TYPE_INFO(Packet_SeasonSystem	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SeasonSystem")),
	GAME_PACKET_TYPE_INFO(Packet_EpicPieceShop	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_EpicPieceShop")),
	GAME_PACKET_TYPE_INFO(Packet_ServerList	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerList")),
	GAME_PACKET_TYPE_INFO(Packet_Mobile						             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Mobile")),
	GAME_PACKET_TYPE_INFO(Packet_Policy					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Policy")),
	GAME_PACKET_TYPE_INFO(Packet_SearchRoom					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SearchRoom")),
	GAME_PACKET_TYPE_INFO(Packet_HostServer					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_HostServer")),
	GAME_PACKET_TYPE_INFO(Packet_Report					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Report")),
	GAME_PACKET_TYPE_INFO(Packet_Title					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Title")),
	GAME_PACKET_TYPE_INFO(Packet_Buff					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Buff")),
	GAME_PACKET_TYPE_INFO(Packet_UserSurvey					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UserSurvey")),
	GAME_PACKET_TYPE_INFO(Packet_MiniGame					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MiniGame")),
	GAME_PACKET_TYPE_INFO(Packet_Gift_Item					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Gift_Item")),
	GAME_PACKET_TYPE_INFO(Packet_2nd_Password					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_2nd_Password")),
	GAME_PACKET_TYPE_INFO(Packet_CleanSystem					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CleanSystem")),
	GAME_PACKET_TYPE_INFO(Packet_WeaponAuctionEvent					     ,(ULONG)CDispatchRecv::Fack_WeaponAuctionEvent, true,  xorstr_DEBUG("Packet_WeaponAuctionEvent")),
	GAME_PACKET_TYPE_INFO(Packet_CoDisassemble					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CoDisassemble")),
	GAME_PACKET_TYPE_INFO(Packet_Help					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Help")),
	GAME_PACKET_TYPE_INFO(Packet_HonorShop					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_HonorShop")),
	GAME_PACKET_TYPE_INFO(Packet_Addon					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Addon")),
	GAME_PACKET_TYPE_INFO(Packet_QuestBadgeShop					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_QuestBadgeShop")),
	GAME_PACKET_TYPE_INFO(Packet_ReverseAuctionSystem					 ,(ULONG)CDispatchRecv::Fack_ReverseAuctionSystem, false,  xorstr_DEBUG("Packet_ReverseAuctionSystem")),
	GAME_PACKET_TYPE_INFO(Packet_FabItems					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_FabItems")),
	GAME_PACKET_TYPE_INFO(Packet_CostumeInven					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CostumeInven")),
	GAME_PACKET_TYPE_INFO(Packet_MonthlyWeapon					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MonthlyWeapon")),
	GAME_PACKET_TYPE_INFO(Packet_ServerLog					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerLog")),
	GAME_PACKET_TYPE_INFO(Packet_TestPacket					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_TestPacket")),
	GAME_PACKET_TYPE_INFO(Packet_ZBSGoldenZB					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ZBSGoldenZB")),
	GAME_PACKET_TYPE_INFO(Packet_ScenarioTX					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ScenarioTX")),
	GAME_PACKET_TYPE_INFO(Packet_SwitchConfig					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SwitchConfig")),
	GAME_PACKET_TYPE_INFO(Packet_CafeItems					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CafeItems")),
	GAME_PACKET_TYPE_INFO(Packet_DefaultItems					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_DefaultItems")),
	GAME_PACKET_TYPE_INFO(Packet_RotationWeaponInven					 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_RotationWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_LifeWeaponInven					     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_LifeWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_EventItemInve					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_EventItemInve")),
};


void _stdcall CDispatchRecv:: DEBUG_PacketDispatch(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;

	int Size = Array[1];

	int Index = *(BYTE*)((ULONG)Stack->EBP - 0x10210);

	int Ret = 0;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == (Packet_Type)Index)
		{
			Ret = 1;


			//函数功能： 从已知类模板中导出一个类
			//返回值：导出类的地址，如果void* object＝NULL，则返回值＝NULL
			//	参数1：void* object，已知类的地址
			//	参数2：int32_t vfdelta　＝　0（
			//	参数3：void* srctype　源模板
			//	参数4：void* desttype　目的模板
			//	参数5：int isreference　＝　0　（

			//找到 eax + 0x4 = 地址
			ULONG Ecx = (ULONG)Stack->EDI;
			ULONG v1 = *(PULONG)(Ecx + 0xC);
			ULONG funs = 0;
			if (v1)
			{
				ULONG v2 = *(PULONG)(v1 + 0x8);
				funs = *(PULONG)(*(PULONG)(v2) + 0x4);
			}



			ACEWarningFileLog("{} Index:{} String:{} Fun:{:X} SvAddress {:X} funs:{:X}",__FUNCTION__, Index, GamePacketList[i].name, (ULONG)Stack->EDX, GamePacketList[i].SvAddres, funs);




			break;
		}
	}
	if (!Ret)
	{
		ULONG* v2 = (PULONG)(Stack->EDI);
		ACEErrorFileLog("{}-Index:{} Fun:{:X} v2 {:X}", __FUNCTION__, Index, (ULONG)Stack->EDX, v2[0]);
	}

}
void _stdcall CDispatchRecv::PacketDispatch(PTAG_PEB_STACK Stack)
{
	ULONG* Arry = (ULONG*)Stack->ESP;

	CHAR Index = Arry[9] & 0xFF;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{

		if ((CHAR)GamePacketList[i].Type == Index)
		{
			memcpy(&GamePacketList[i].Stack, Stack, sizeof(TAG_PEB_STACK));

			CallHookAddres(Stack, &GamePacketList[i]);

			break;

		}
	}
}
void CDispatchRecv::CallHookAddres(PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO Info)
{
	ULONG* Arry = (ULONG*)Stack->ESP;

	ULONG	src = Arry[1];

	ULONG	ulSize = Arry[2];

	ULONG   CallAddres = Info->Addres;
	__asm
	{
		pushad
		mov eax, Info
		push eax
		mov eax, ulSize
		push eax
		mov eax, src
		push eax
		call CallAddres
		popad
	}
}


PTAG_PEB_STACK CDispatchRecv::GetPacketStack(Packet_Type type)
{
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == type)
		{
			return &GamePacketList[i].Stack;
		}
	}
	return nullptr;
}

PTAG_GAME_PACKET_TYPE_INFO CDispatchRecv::GetPacketTypeInfo(Packet_Type type)
{
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{

		if ((CHAR)GamePacketList[i].Type == type)
		{
			return  &GamePacketList[i];
		}
	}
	return nullptr;
}

BYTE CDispatchRecv::CallGamePacket(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	PVOID pThis = Stack->ECX;
	ULONG pCall = pHook->SvAddres;
	__asm
	{
		mov ecx, dword ptr ss : [ebp + 0xC]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x8]
		push edx
		mov ecx, pThis
		mov edi, ecx
		call pCall
	}
	return 1;
}

BYTE CDispatchRecv::CallGamePacket2(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	PVOID pThis = Stack->ECX;
	ULONG pCall = pHook->SvAddres;
	ULONG v2 = CDispatchRecv::GetInstance()->m_SeCallAddress;

	__asm
	{
		mov ecx, dword ptr ss : [ebp + 0xC]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x8]
		push edx
		mov ecx, pThis
		mov edi, ecx
		call pCall
		mov ecx, edi
		call v2
	}

}

ULONG WINAPI CDispatchRecv::extMetadataParsing(PTAG_PEB_STACK Stack)
{
	VMProtectBegin(__FUNCTION__);


	ULONG v1           = (ULONG)Stack->EBP;
	PVOID pBuffer      = NULL;
	DWORD dwBufferSize = 0;
	DWORD dwSrcPoint   = 0;
	DWORD dwSrcSize    = 0;

	// [12/20/2021 007]
	// 韩服更新 解压前的 Size = 94->80  压缩包的的Buff = 78->74 ,Size 74->70 
	// [2/16/2022 007]
	// a8 = 0x pk包
	// a0 = size
	// b8 = src size
	// 9c = src buf
	__asm
	{
		pushad
		mov edx, v1
		mov eax, dword ptr ss : [edx - 0x0000009c]	// 解压后的buff
		lea ecx, pBuffer
		mov[ecx], eax
		mov edx, v1
		mov eax, dword ptr ss : [edx - 0x000000b8]	//解压后的buff大小
		mov dwBufferSize, eax
		mov eax, dword ptr ss : [edx - 0x000000a8]	//压缩文件buff
		mov dwSrcPoint, eax
		mov eax, dword ptr ss : [edx - 0x000000a0]	//压缩文件大小
		mov dwSrcSize, eax
		popad
	}

	auto lPackRead = std::make_shared<PackReader>((BYTE*)pBuffer, dwBufferSize);

	//解析zip 
	auto lPackZip = std::make_shared<PackReader>((BYTE*)dwSrcPoint, dwSrcSize);


	int nPackFlag  = lPackZip->ReadChar();
	short nZipSize = lPackZip->ReadShort();

#pragma pack (1)
	typedef struct ZIP_HEART
	{
		int		header_flag;	            //文件头标识，值固定(0x04034b50)
		short	version;		            //解压文件所需 pkware最低版本
		short	bit_flag;		            //通用比特标志位(置比特0位=加密)
		short	compression_method;         //压缩方式
		short	file_last_modification_time;//文件最后修改时间
		short	file_last_modification_date;//文件最后修改日期
		int		crc32;                      //CRC-32校验码
		int		compressed_size;            //压缩大小
		int		uncompressed_size;          //未压缩的大小
		short	file_name_length;	        //文件名长度
		short	extra_field_length;	        //扩展区长度

	}TAG_ZIP_HEART, * PTAG_ZIP_HEART;
#pragma pack()

	PTAG_ZIP_HEART zip = (PTAG_ZIP_HEART)lPackZip->ReadBytes(sizeof(TAG_ZIP_HEART));

	char szFileName[MAX_PATH] = { 0 };

	strncpy(szFileName, (char*)lPackZip->ReadBytes(zip->file_name_length), zip->file_name_length);

#if ANTI_DEBUG
	auto ACEFile = std::make_shared<CACEFile>();
	int day      = zip->file_last_modification_date & 0x1F;
	int month    = zip->file_last_modification_date >> 5 & 0xF;
	int year     = zip->file_last_modification_date >> 9 & 0x7F;

	/*
	*	curpath+Metedata+date+filename
	*/
	std::string Path = fmt::format(xorstr_("{}Metedata"), ACEFile->GetCurrentDirectoryA());

	std::string FileName = fmt::format(xorstr_("{}\\{}-{}-{}-{}"), Path,year + 1980, month, day, szFileName);

	//创建路径
	CreateDirectoryA(Path.data(), nullptr);

	ACEFile->SetFileName(FileName);

	if (ACEFile->OpenFile(CREATE_ALWAYS) != INVALID_HANDLE_VALUE)
	{
		ACEFile->WriteFile(pBuffer, dwBufferSize);
	}
	ACEFile->CloseFile();

#if 0
	//Resource/cso_chn.txt
	CreateInterfaceFn filesystem_factory = CaptureFactory(("FileSystem_Nar.dll"));
	IFileSystem* g_pFileSystem = (IFileSystem*)(CaptureInterface(filesystem_factory, ("VFileSystem009")));

	FileHandle_t hFile = g_pFileSystem->Open(("resource\\cso_chn.txt"), "rb", NULL);
	if (!hFile)
	{
		hFile = g_pFileSystem->Open(("resource\\cso_koreana.txt"), "rb", NULL);

	}

	int FileSize = g_pFileSystem->Size(hFile);

	std::vector<BYTE> VecText(FileSize);

	g_pFileSystem->Read(VecText.data(), FileSize, hFile);

	ACEFile->SetFileName("chn_txt007.txt");
	ACEFile->OpenFile();
	ACEFile->WriteFile(VecText.data(), FileSize);
	ACEFile->CloseFile();

	//hFile = g_pFileSystem->Open(("resource\\item.csv"), "rb", NULL);
	//FileSize = g_pFileSystem->Size(hFile);
	//std::vector<BYTE> VecTextCsv(FileSize);

	//ACEFile->SetFileName("item007.csv");
	//ACEFile->OpenFile();
	//g_pFileSystem->Read(VecText.data(), FileSize, hFile);
	//ACEFile->WriteFile(VecText.data(), FileSize);
	//ACEFile->CloseFile();

#endif // ANTI_DEBUG











#endif


	VMProtectEnd();

	return 0;
}



void __declspec(naked)  Naked_PacketDispatch_DEBUG()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv::DEBUG_PacketDispatch
		popad
	}
}
ODG_EXPORT void __declspec(naked)  Naked_PacketDispatch()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv:: PacketDispatch
		popad
		ret 8
	}
}

bool CDispatchRecv::InitializeHook()
{
	auto AceHook  =  g_pUtil->GetAceHook();
	auto AceMemory = g_pUtil->GetAceMemory();


	ULONG DispatchRecvAddres = 0;

	if (AceMemory->GetScanCode(xorstr_("50 FF D2 84 C0 74 07"), DispatchRecvAddres, g_Offset.GetEngineModuleInfo()->hModule))
	{
		ULONG uAsmLenght = AceHook->GetDisasmLenght(DispatchRecvAddres, 5);

		m_SeCallAddress = AceHook->GetCallAddress(DispatchRecvAddres + uAsmLenght);


#if ANTI_DEBUG
		AceHook->HookWithNaked(DispatchRecvAddres, 5, Naked_PacketDispatch_DEBUG);
#endif // ANTI_DEBUG

		ACEDebugFileLog("{}  DispatchRecvAddres :{:X} m_SeCallAddress:{:X} AsmLenght:{:X}", __FUNCTION__, DispatchRecvAddres, m_SeCallAddress, uAsmLenght);
	}
	

	if (AceMemory->GetScanCode(xorstr_("56 8B F1 8B 0D ?? ?? ?? ?? 8B 06 FF 70 F4 50"), m_PacketInfo, g_Offset.GetEngineModuleInfo()->hModule))
	{
		m_PacketInfo = *(PDWORD)(m_PacketInfo + 5);
	}

	for (size_t i = 0; i < 0xFF; i++)
	{
		Debug_Packet((Packet_Type)i);
	}

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Hook)
		{
			InstallHook(GamePacketList[i].Type, new GAME_PACKET_TYPE_INFO_HOOK((ULONG)Naked_PacketDispatch));
		}
	}





	return true;
}
void CDispatchRecv::Debug_Packet(Packet_Type type)
{

#if ANTI_DEBUG

	ULONG Ecx = (ULONG)GetPackThis(type);

	if (Ecx == 0)
	{
		return;
	}

	ULONG v2 = *(PULONG)(Ecx + 0xC);
	ULONG funs = 0;
	ULONG Addres = *(PULONG)(*(PULONG)Ecx + 0x8);;

	if (v2)
	{
		ULONG v3 = *(PULONG)(v2 + 0x8);
		funs = *(PULONG)(*(PULONG)(v3)+0x4);

	}
	BOOL Status = FALSE;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == type)
		{
			Status = TRUE;
			break;
		}
	}

	if (!Status)
	{
		ACEErrorFileLog("Index:{} {:X} funs:{:X}", (ULONG)type, Addres, funs);
	}
#endif
}



std::vector<BYTE> CDispatchRecv::GetAuctionMetadate()
{
	return m_MapMetadate.count(8) ?  m_MapMetadate.at(8) : std::vector<BYTE>();
}

std::vector<BYTE> CDispatchRecv::GetAuctionSpecialEventsMetadate()
{
	return m_MapMetadate.count(0x31) ? m_MapMetadate.at(0x31) : std::vector<BYTE>();
}

BOOL CDispatchRecv::InstallHook(ULONG Index, GAME_PACKET_TYPE_INFO_HOOK* pInfo)
{
	ULONG v1 = *(PULONG)m_PacketInfo + Index * 4 + 0x10;

	ULONG Result = *(PULONG)v1;

	if (!Result)
	{
		return FALSE;
	}
	ULONG SavaAddres = *(PULONG)(*(PULONG)Result + 0x8);
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == (Packet_Type)Index)
		{
			GamePacketList[i].SvAddres = SavaAddres;
			break;
		}
	}
	ULONG v3 = pInfo->v3;
	//修改地址 保存父类指针
	memcpy(pInfo, (PVOID)(ULONG)(*(PULONG)(Result) - 4 ), sizeof(GAME_PACKET_TYPE_INFO_HOOK));
	pInfo->v3 = v3;
	//当前地址 + 4 = 当前类
	*(PULONG)Result = (ULONG)pInfo + 4;




	return TRUE;
}
void CDispatchRecv::InterfaceCommandPacketSuperRetRoom()
{
	VMProtectBegin(__FUNCTION__);

	cl_entity_s* Entity = Engine::g_Engine.GetLocalPlayer();

	if (Entity && Entity->index)
	{
		hud_player_info_t Info;

		Engine::g_Engine.pfnGetPlayerInfo(Entity->index, &Info);

		if (Info.mark)
		{
			PackWriter Pack = PackWriter(0xC);
			Pack.WriteByte(0x3);
			Pack.WriteInt(Info.mark);
			Pack.WriteInt(Info.mark);

			PTAG_PEB_STACK Stack = GetPacketStack(Packet_Kick);

			CallGamePacket(Pack.GetBytes(), Pack.GetBytesLength(), Stack, GetPacketTypeInfo(Packet_Kick));
		}
	}

	VMProtectEnd();
}


PSHORT CDispatchRecv::GetItemCount()
{
	return &m_ItemCount;
}

PSHORT CDispatchRecv::GetItemSite()
{
	return &m_ItemSite;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UMsg(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	UMsgType Type = static_cast<UMsgType>(lPackRead->ReadChar());

	ACEDebugFileLog("{} Flag :{}", __FUNCTION__, static_cast<int>(Type));
	
	g_pUtil->ViewHexCode(lPackRead);
	
	//重写MSG封包类型
	auto ResetPackType = [&](int Type)
	{
		ULONG v1 = (ULONG)GetPacketStack(Packet_UMsg)->EDI;
		 *(PULONG)(v1 + 0x28) = Type;
	};

	CDispatchRecv* This = CDispatchRecv::GetInstance();


	switch (Type)
	{
	case UMsgType::Chat:
	{
		
	}
		break;
	case UMsgType::SysNotice:	
		break;
	case UMsgType::Notice:
	{
		std::string Text = lPackRead->ReadStr();

		if (This->m_AuctionSystem.IsJoinAuctionSpecialEvents())
		{
			// 新一轮的特别拍卖活动开始了
			if (Text == xorstr_("AUCTION_1000M_BEGIN"))
			{
				CGamePack::GetInstance()->AuctionEventsRefresh();
			}
			// 本轮的特别拍卖活动将在5分钟后结束！
			else if (Text == xorstr_("AUCTION_1000M_REMAIN_5MIN"))
			{
				CGamePack::GetInstance()->AuctionEventsRefresh();
			}
			//恭喜[{0}]！参与特别拍卖活动，获得道具奖励 -- 没中奖也会出这个.
			else if (Text == xorstr_("AUCTION_1000M_WINNER"))
			{
				//提取items

			}
		}


		


	}
		break;
	case UMsgType::MsgStr:
	{
		//if (g_Vars.m_Survial.sk_cata_enable->value && g_Vars.m_Survial.sk_cata_start_game->value)
		//{
		//	std::string Text = lPackRead->ReadStr();
		//	if (Text  == xorstr_("ROOM_START_GUEST_FAILED"))
		//	{
		//		//请等待房主开始游戏

		//		//-延迟10秒 开始游戏
		//		auto FunsThreadStartGame = []() {

		//			Sleep(10 * 1000);
		//			Engine::g_Engine.pfnClientCmd(xorstr_("+PackStartGame"));
		//		};

		//		std::thread t(FunsThreadStartGame);
		//		t.detach();

		//		return 1;
		//	}
		//}
	}
		break;
	case UMsgType::AdminNotice:
	{
		std::string Text = lPackRead->ReadStr();

		ACEErrorLog("{}", Text);

		if (Text == xorstr_("NEXONGUARD2_BLOCK"))
		{
			//帐号异常 进入黑名单
		}
		else if (Text == xorstr_("NEXONGUARD2_CUT"))
		{
			//异常断开连接

			return 1;
		}
		break;
	}
	case UMsgType::NoticeBoard:
		break;	
	case UMsgType::Lottery:
	{
		
	}
		break;
	case UMsgType::LIKE:
	{
		ResetPackType(0xFF);
		return 1;
	}
		break;
	default:
		ACEErrorFileLog("{} default Flag :{}", __FUNCTION__, static_cast<int>(Type));
		break;
	}

	CallGamePacket(Dst, Size,  GetPacketStack(Packet_UMsg), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Kick(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	KickType Index = static_cast<KickType>(lPackRead->ReadChar());

	ACEDebugFileLog("{} Index :{}", __FUNCTION__, static_cast<int>(Index));

	switch (Index)
	{
	case KickType::Error:
		break;
	case KickType::ReMain:
		break;
	case KickType::KickInfo:
	{

	}
		break;
	case KickType::KickRoom:
		//和谐了.防踢
		break;
	case KickType::KickInit:
		
		break;
	default:
		break;
	}
	CallGamePacket(Dst, Size, GetPacketStack(Packet_Kick), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UserStart(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);


	int v1             = lPackRead->ReadInt();
	char* user_account = lPackRead->ReadStr();
	char* player_name  = lPackRead->ReadStr();

	static BOOL bStatus = FALSE;
	
	if (!bStatus)
	{
		bStatus = TRUE;
		CSOWrapper::GetInstance()->StartTaskThread();

		//初始化
		CDispatchRecv::GetInstance()->m_AuctionSystem.Init();


	}

	CallGamePacket(Dst, Size, GetPacketStack(Packet_UserStart), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UpRoom(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	UpRoomType Index = static_cast<UpRoomType>(lPackRead->ReadChar());

	//ACEDebugFileLog("{} Index :{}", __FUNCTION__, static_cast<int>(Index));

	switch (Index)
	{
	case UpRoomType::RoomInfo:
	{
		int Flag        = lPackRead->ReadChar();
		ULONG RoomIndex = lPackRead->ReadInt();

		

	}
		break;
	case UpRoomType::RoomPlayerCount:
		break;
	case UpRoomType::RoomGameEnd:
	{
		
	}
		break;
	case UpRoomType::RoomKick:
		
		break;
	case UpRoomType::RoomKickExp:
	{

	}
		break;
	case UpRoomType::RoomWeaponRate:
		break;
	default:
		break;
	}




	

	CallGamePacket(Dst, Size, GetPacketStack(Packet_UpROOM), pHook);
	return 1;
}


void __declspec(naked)  Naked_Metadata()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv::extMetadataParsing
		popad

	}
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Metadata(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	if (1)
	{
		static bool bInitHookMetadata = true;

		if (!bInitHookMetadata)
		{
			bInitHookMetadata = true;

			auto AceHook   = g_pUtil->GetAceHook();
			auto AceMemory = g_pUtil->GetAceMemory();


			DWORD dwMetadataParsing = 0;
			if (AceMemory->GetScanCode(xorstr_("E8 ?? ?? ?? ?? 8B 85 ?? FF FF FF C6 45 FC 01"), dwMetadataParsing, g_Offset.GetEngineModuleInfo()->hModule))
			{
				dwMetadataParsing += AceHook->GetDisasmLenght(dwMetadataParsing);

				AceHook->HookWithNaked(dwMetadataParsing, 0x6, Naked_Metadata);
			}

		}
		int Type = lPackRead->ReadChar();
		lPackRead->ReadChar();
		lPackRead->ReadChar();
			
		if (lPackRead->ReadShort() != 0x4b50)
		{
			lPackRead->SetIndex(1);
			
			switch (Type)
			{
			//case 0x3:
			//case 0x6:
			//case 0x4:
			//case 0x1E:				
			//case 0x1F:
			//case 0xF:
			//case 0x2B:
			//case 0x14:
			case 0x8:		//拍卖行价格走势
			case 0x31:		//拍卖会特别活动
			{
				std::vector<BYTE> vData;
				vData.resize(Size);
				memcpy(vData.data(), Dst, vData.size());
				m_Instance->m_MapMetadate.insert(std::make_pair(Type, vData));
			}
			break;
			default:
				break;
			}

			//解析
			if (Type == 0x31)
			{
				m_Instance->m_AuctionSystem.ParseAuctionSpecialEvents(m_Instance->GetAuctionSpecialEventsMetadate());
				m_Instance->m_AuctionSystem.GetCanTimeActive();

			}

			ACEDebugFileLog("[{}] Index:{:X}",__FUNCTION__,Type);
		}

	}

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Metadata), pHook);
	return 1;
}


ODG_EXPORT int _stdcall CDispatchRecv::Fake_Inventory(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lpStack = GetPacketStack(Packet_Inventory);

	CDispatchRecv* pDispatch = CDispatchRecv::GetInstance();
	pDispatch->m_lPackInventory->Clear();
	pDispatch->m_lPackInventory->WriteBytes((PBYTE)Dst, Size);
	auto lPackRead = std::make_shared<PackReader>(pDispatch->m_lPackInventory->GetBytes(), pDispatch->m_lPackInventory->GetBytesLength());

	int* Array = (int*)lpStack->ECX;

	if (Array[6] == 0x9A)
	{
		lPackRead->ReadInt();
	}

	//读取道具总数量
	*pDispatch->GetItemCount() = lPackRead->ReadShort();

	CallGamePacket(pDispatch->m_lPackInventory->GetBytes(), pDispatch->m_lPackInventory->GetBytesLength(), lpStack, pHook);
	return 1;
}

PVOID CDispatchRecv::GetPackThis(Packet_Type type)
{
	ULONG v1 = *(PULONG)m_PacketInfo + type * 4 + 0x10;

	return (PVOID) * (PULONG)v1;
}



ODG_EXPORT int _stdcall CDispatchRecv::Fake_Quest(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	ULONG Index = lPackRead->ReadChar();

	ACEDebugLog("{} Index:{}",__FUNCTION__, Index);

	switch (Index)
	{
	case 0xC:
	{
		int QuestId = lPackRead->ReadInt();

		BYTE v1 = lPackRead->ReadChar();
		BYTE v2 = lPackRead->ReadChar();

		auto QuestName = fmt::format(xorstr_("CSO_Quest{}_Name"), QuestId);

		auto AnsiName = g_pUtil->GetAceTool()->WstringToString(g_pUtil->GetItemName(QuestName.data(), ""));

		QuestName = g_pUtil->GetAceTool()->string_To_UTF8(AnsiName);

		//ACEDebugLog("[{}] id:{} v1:{} v2:{} Quest:{}", __FUNCTION__, QuestId, v1,v2, AnsiName);

		if (v2 & 0x4)
		{
			int nCount = lPackRead->ReadChar();

			//ACEDebugLog("[{}] nCount:{}", __FUNCTION__, nCount);

			for (int i = 0; i < nCount; i++)
			{
				int k = lPackRead->ReadInt();

				//ACEDebugLog("[{}] k:{}",__FUNCTION__, k);
			}

		}
		if (v2 & 0x8)
		{
			int v3 = lPackRead->ReadInt();

			auto Text = fmt::format("{}->{}", AnsiName, v3);
			
			ACEDebugLog("[{}] Text:{}", __FUNCTION__, Text);

		}

		//ACEDebugLog(__FUNCTION__);
	}
	break;
	default:
		break;
	}




	CallGamePacket(Dst, Size, GetPacketStack(Packet_Quest), pHook);
	return 1;

}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Option(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);


	CallGamePacket(Dst, Size, GetPacketStack(Packet_Option), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Null(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Ribbon(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	//自动屏蔽傻逼的成就记录
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Hack(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	CSOWrapper::GetInstance()->GetRCF()->SendHeartBeat();

	return 	CallGamePacket(Dst, Size, GetPacketStack(Packet_Hack), pHook);
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Host(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Host), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Item(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

#if 1
	if (Size >= 3)
	{
		UCHAR uFlag = lPackRead->ReadChar();
		switch (uFlag)
		{
		case 8:
		{
			UCHAR uV1 = lPackRead->ReadChar();
			UCHAR uV2 = lPackRead->ReadChar();

			// 在线时间已满10分钟 可以领取补给
			// CGamePack::GetInstance()->Item_Roulette();

			//可以当做每日回调..
			break;
		}
		default:
			break;
		}

	}
#endif
	
	CallGamePacket(Dst, Size, GetPacketStack(Packet_Item), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_ClientCheck(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	//CSagaClient::GetInstance()->HeartBeat();

	CallGamePacket(Dst, Size, GetPacketStack(Packet_ClientCheck), pHook);

	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Transfer(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Transfer), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fack_ReverseAuctionSystem(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	
	return 1;

	CallGamePacket(Dst, Size, GetPacketStack(Packet_ReverseAuctionSystem), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fack_WeaponAuctionEvent(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	auto Result =  m_Instance->m_AuctionSystem.WeaponAuctionEvent(lPackRead);

	return Result ? CallGamePacket(Dst, Size, GetPacketStack(Packet_WeaponAuctionEvent), pHook) : 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UserUpdateInfo(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);
	
	static bool bStatus = true;

	if (bStatus)
	{

		g_Vars.m_nUserSign = lPackRead->ReadInt();

		int flag = lPackRead->ReadInt();

		signed int v9 = flag;

		if (v9 & 1 << 0)
		{
			lPackRead->ReadChar();

			lPackRead->ReadInt();
		}
		if (v9 & 1 << 1)
		{
			g_Vars.m_UserName     = lPackRead->ReadStr();
			g_Vars.m_FakeUserName = lPackRead->ReadStr();
		}
		if (v9 & 1 << 2)
		{
			lPackRead->ReadChar();
			lPackRead->ReadChar();
			lPackRead->ReadChar();
		}
		if (v9 & 1 << 3)
		{
			bStatus ^= true;

			g_Vars.m_Level = lPackRead->ReadInt();

			//发送用户信息
			CSOWrapper::GetInstance()->GetRCF()->SendUserInfo(g_Vars.m_UserName, g_Vars.m_Level);

		}
		if (v9 & 1 << 4)
		{
			lPackRead->ReadChar();
		}
		if (v9 & 1 << 5)
		{
			lPackRead->ReadLong64();
		}
		if (v9 & 1 << 6)
		{
			lPackRead->ReadLong64();
		}
		if (v9 & 1 << 7)
		{
			g_Vars.m_Gold = lPackRead->ReadLong64();
			ACEDebugLog("[{}] 仓库金币 = {}", __FUNCTION__, g_Vars.m_Gold);
			CSOWrapper::GetInstance()->GetRCF()->SendGoldInfo(g_Vars.m_Gold);
		}
	}
	// 不全部写了. 只需重要的
	if (Dst && Size == 0x10)
	{

		lPackRead->ReadInt();

		int flag = lPackRead->ReadInt();

		if (flag == 0x4000000)
		{
			//读取积分
			g_Vars.m_Integral = lPackRead->ReadInt();

			lPackRead->ReadInt();
			
			ACEDebugLog("[{}] 欢乐积分 = {}", __FUNCTION__, g_Vars.m_Integral);

			CSOWrapper::GetInstance()->GetRCF()->SendGoldInfo(g_Vars.m_Gold);
			CSOWrapper::GetInstance()->GetRCF()->SendIntegralInfo(g_Vars.m_Integral);
		}
		else if (flag == 0x00000080)
		{

			g_Vars.m_Gold = lPackRead->ReadLong64();
			ACEDebugLog("[{}] 仓库金币 = {}", __FUNCTION__, g_Vars.m_Gold);
			CSOWrapper::GetInstance()->GetRCF()->SendGoldInfo(g_Vars.m_Gold);
		}

	}

	




	
	return CallGamePacket(Dst, Size, GetPacketStack(Packet_UserUpdateInfo), pHook);
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Character(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	g_pUtil->CreatePlayerName();

	return CallGamePacket(Dst, Size, GetPacketStack(Packet_Character), pHook);
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Reply(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	int	  nType = lPackRead->ReadChar();
	char* pText   = lPackRead->ReadStr();
	
	LOGIN_NOTICE_CODE Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_SUCCEED;
	std::string Msg;
	switch (nType)
	{
	case 0:
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_SUCCEED;
		Msg = u8"用户认证完毕";
		break;
	case 1:
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_CREATEOK;
		Msg = u8"用户创建成功";
		break;
	case 3: // S_REPLY_INVALIDNAME -网络连接中断
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALIDNAME; 
		Msg = u8"网络连接中断";
		break;
	case 15: //S_REPLY_NEXONCOMERROR 服务器发生故障
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_ERROR;
		Msg = u8"服务器发生故障";
		break;
	case 10: //S_REPLY_SYSERROR-认证系统错误
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_SYSERROR;
		Msg = u8"认证系统错误";
		break;
	case 38: //客户端版本不一致.需要更新客户端
		Code = LOGIN_NOTICE_CODE::NOTICE_GAME_REPLY_INVALID_CLIENT_VERSION;
		Msg = u8"游戏客户端版本不一致.请更新游戏客户端";
		break;
	case 22: ///S_REPLY_INVALID_CHAR
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALID_CHAR;
		Msg = u8"玩家名无效字符,重新创建玩家名";
		break;
	case 23: ///S_REPLY_ID_DIGIT_BEFORE_CHAR
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALID_CHAR;
		Msg = u8"玩家名不符合规则,重新创建玩家名";
		break;
	case 25: ///S_REPLY_ALREADY_EXIST_
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALID_CHAR;
		Msg = u8"玩家名已存在,重新创建玩家名";
		break;
	case 26: ///S_REPLY_ID_PROHIBITED
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALID_CHAR;
		Msg = u8"被禁止的玩家名,重新创建玩家名";
		break;
	default:
		Code = LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_UNCODE;
		Msg = fmt::format("{}:{:X}",u8"未知的认证码", nType);
		break;
	}

	CSOWrapper::GetInstance()->GetRCF()->SendLoginEventNotice(Code, Msg);



	return CallGamePacket(Dst, Size, GetPacketStack(Packet_Reply), pHook);
}