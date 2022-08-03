#include "Util.h"
#include <PackReader.h>
#include "GameOffset.h"
#include "csv.h"

Util* g_pUtil = nullptr;

Util::Util()
{
	m_Hook   = CACEInterface::GetInstance()->GetHook();
	m_Memory = CACEInterface::GetInstance()->GetMemory();
	m_Tool   = CACEInterface::GetInstance()->GetUtil();
}
CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	CreateInterfaceFn Interface = 0;

	while (!Interface)
	{
		HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

		if (hFactoryModule)
		{
			Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, CREATEINTERFACE_PROCNAME));
		}

		Sleep(100);
	}
	return Interface;
}
void* CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	PVOID dwPointer = NULL;

	while (!dwPointer)
	{
		dwPointer = (PVOID)(Interface(InterfaceName, 0));

		Sleep(100);
	}

	return dwPointer;
}
std::string Util::GetCSOItemName(char* ItemCode)
{
	 auto AnsiName =   m_Tool->WstringToString(GetItemName(ItemCode, xorstr_("CSO_Item_Name_")));

	 return  m_Tool->string_To_UTF8(AnsiName);

}

wchar_t* Util::GetItemName(const char* pItemCode, const char* pItemName)
{
	ULONG ulItemCodeToItemName = g_Offset.GetGameFunsInfo()->ItemCodeToItemName;
	__asm
	{
		push pItemCode
		push pItemName
		lea eax, ulItemCodeToItemName
		call dword ptr ds : [eax]
		add esp, 0x8
	}
}

std::tuple<GameServer, std::string> Util::GetGameServer()
{

	std::tuple<GameServer, std::string> result;

	// 取游戏服务器
	if (FindGameResourceFile(xorstr_("cstrike_chn.nar")))
	{
		std::get<0>(result) = GameServer::Chinese;
		std::get<1>(result) = xorstr_("反恐精英OL");
	}
	else if (FindGameResourceFile(xorstr_("cstrike_tw.nar")))
	{
		std::get<0>(result) = GameServer::TaiWan;
		std::get<1>(result) = xorstr_("絕對武力");
	}
	else if (FindGameResourceFile(xorstr_("cstrike_na_en.nar")))
	{
		std::get<0>(result) = GameServer::Steam;
		std::get<1>(result) = xorstr_("CSNS");
	}
	else
	{
		std::get<0>(result) = GameServer::Kor;
		std::get<1>(result) = xorstr_("Korea");
	}

	return result;
}

std::tuple<ULONG, GameServerRegion, std::string > Util::GetGameServerRegion()
{
	auto result = std::make_tuple<ULONG, GameServerRegion, std::string>(0, GameServerRegion::D1,"");

	auto GetSharedDict = [&]() ->PVOID
	{

	};

	HANDLE hFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, xorstr_("CSO.SharedDict"));
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return result;
	}
	LPVOID lpMapping = MapViewOfFile(hFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
	if (lpMapping == nullptr)
	{
		return result;
	}

	auto MappingInfo =  m_Hook->QueryVirtualMemory(lpMapping);
	if (std::get<0>(MappingInfo) == false)
	{
		return result;
	}
	auto mbi = std::get<1>(MappingInfo);

	auto lPackReader = std::make_shared<PackReader>((BYTE*)lpMapping, mbi.RegionSize);

	std::string Region;
	//开始解析
	int unknown = lPackReader->ReadInt();
	//一共几个
	int ValueCount = lPackReader->ReadInt();
	for (size_t i = 0; i < ValueCount; i++)
	{
		ULONG Size            = lPackReader->ReadInt();
		char* Name            = nullptr;
		if (Size)
		{
			Name = (char*)malloc(Size + 1) ;
			if (Name == nullptr)
			{
				return result;
			}
			RtlZeroMemory(Name, Size + 1);
			memcpy(Name, lPackReader->ReadBytes(Size), Size);
		}
		Size        = lPackReader->ReadInt();
		char* Value = nullptr;
		if (Size)
		{	
			Value = (char*)malloc(Size + 1);
			if (!Value)
			{
				return result;
			}
			RtlZeroMemory(Value, Size + 1);
			memcpy(Value, lPackReader->ReadBytes(Size), Size);
		}

		//ACEInfoLog("{} {}", Name, Value);


		if (Name)
		{
			if (lstrcmpiA(Name,xorstr_("region")) == 0)
			{
				Region = Value;
			}

			free(Name);
		}
		if (Value)
		{
			free(Value);
		}
	}

	UnmapViewOfFile(lpMapping);
	CloseHandle(hFile);
	/*
	* 
	2022-01-02:13:57:07 Util.cpp 88 launched 1
	2022-01-02:13:57:07 Util.cpp 88 passport NP12:sso02:46:772605049:muRiY2I4dC3r55nojMBjIpxRelBWs_3SmOYLh1DcS1JNC08xMB~WTIof91DkrJmKN_sHdF_0kqxm4iBw0Ew52IcewSoJjJrJiCxhz4ATfARa2hyM7ac3cXd0DfgXiBOHUwjdelvJN0ul1dlbfXwtmE7o~UVYYwbxD~4y8gSwC1lsmSvtsggeQ5SmHKvAZpZTlQSlyoCe95RsFvhDFMh~G5bkDSCY6vVW8V4v8uZoBKvrheiPblL9nxDY0Rc_fxnMY9nN~RRScbfDjVnIyEVTbAu8lwgmWyzB2u0s9xSRH3ei0VJ~CzN2XC0927ADFeuLU87bQCglQdR_pdjMn4uL8GmIQhDO3IqUwNmryEhvrCmwJoadWZI
	2022-01-02:13:57:07 Util.cpp 88 region 6
	2022-01-02:13:57:07 Util.cpp 88 type 0
	*/


	//大区数据 电信1->5 电信2->6 电信3->7 网通1->15 网通2->16
	ULONG RegionValue =  std::stoi(Region);
	std::get<0>(result) = RegionValue;
	switch (RegionValue)
	{
	case 5:
		std::get<1>(result) = GameServerRegion::D1;
		std::get<2>(result) = xorstr_("电信一区");
		break;
	case 6:
		std::get<1>(result) = GameServerRegion::D2;
		std::get<2>(result) = xorstr_("电信二区");
		break;
	case 7:
		std::get<1>(result) = GameServerRegion::D3;
		std::get<2>(result) = xorstr_("电信三区");
		break;
	case 15:
		std::get<1>(result) = GameServerRegion::W1;
		std::get<2>(result) = xorstr_("网通一区");
		break;
	case 16:
		std::get<1>(result) = GameServerRegion::W2;
		std::get<2>(result) = xorstr_("网通二区");
		break;
	default:
		break;
	}
	return result;
}

std::shared_ptr<CACEHook> Util::GetAceHook()
{
	return m_Hook;
}

std::shared_ptr<CACEMemory> Util::GetAceMemory()
{
	return m_Memory;
}

std::shared_ptr<CACEUtil> Util::GetAceTool()
{
	return m_Tool;
}

void Util::InitGameItemInfo()
{
	m_ItemMap.clear();

	CreateInterfaceFn filesystem_factory = CaptureFactory(xorstr_("FileSystem_Nar.dll"));

	IFileSystem* g_pFileSystem = (IFileSystem*)(CaptureInterface(filesystem_factory,xorstr_("VFileSystem009")));

	if (!g_pFileSystem)
		return;
	FileHandle_t hFile = g_pFileSystem->Open(xorstr_("resource\\item.csv"), xorstr_("rb"), NULL);

	if (!hFile)
		return;

	int FileSize = g_pFileSystem->Size(hFile);

	char* ReadBuff = (char*)VirtualAlloc(NULL, FileSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	g_pFileSystem->Read(ReadBuff, FileSize, hFile);

	struct csv_data* data = Csv_LoadEx(ReadBuff);

	struct csv_data* p = data;

	struct csv_value_list* list;

	int lineindex = 1;

	int col = 0;

	if (!data) return;

#if ANTI_DEBUG
	//序列化
	CACEModule ACEModule;
	std::ofstream ofItem(fmt::format(xorstr_("{}\\item.json"), ACEModule.GetModuleInfo()->ModulePath));
	std::ofstream ofWeaponPaintItem(fmt::format(xorstr_("{}\\WeaponPaintItem.json"), ACEModule.GetModuleInfo()->ModulePath));
	my_json Item;
	my_json WeaponPaintItem;
#endif


	while (p)
	{
		col = 0;

		if (p->value_list)
		{
			list = p->value_list;

			PGAME_ITEMMAP Info = new GAME_ITEMMAP();

			RtlZeroMemory(Info,sizeof(GAME_ITEMMAP));

			int k = 0;

			int ItemId = 0;

			while (list)
			{
				k++;

				switch (k)
				{
				case 1:
					ItemId = list->value.intvalue;
					break;
				case 3:
					if (ItemId == 54)
					{
						lstrcpyA(Info->RecourceName, xorstr_("707"));
						break;
					}
					lstrcpyA(Info->RecourceName, list->value.stringvalue);
					break;
				case 4:
				{
					lstrcpyA(Info->ClassName, list->value.stringvalue);

					if (lstrcmpA(Info->ClassName, xorstr_("Equipment")) == 0)
					{
						Info->RecourceType = ClassType_Equipment;
					}
					else if (lstrcmpA(Info->ClassName, xorstr_("Class")) == 0)
					{
						Info->RecourceType = ClassType_Class;
					}
					else if (lstrcmpA(Info->ClassName, xorstr_("PeriodicItem")) == 0)
					{
						Info->RecourceType = ClassType_PeriodicItem;
					}
					else if (lstrcmpA(Info->ClassName, xorstr_("Tattoo")) == 0)
					{
						Info->RecourceType = ClassType_Tattoo;
					}
					else if (lstrcmpA(Info->ClassName, xorstr_("zbRespawnEffect")) == 0)
					{
						Info->RecourceType = ClassType_zbRespawnEffect;
					}
					else if (lstrcmpA(Info->ClassName, xorstr_("WeaponPaintItem")) == 0)
					{
						Info->RecourceType = ClassType_WeaponPaintItem;
					}		
					else
					{
						Info->RecourceType = ClassType_Unknown;
					}
					break;
				}
				case 5:
					Info->Category = list->value.intvalue;
					break;
				case 6:
					Info->InGameID = list->value.intvalue;
					break;
				case 47:
				{
					Info->SortingIndex = list->value.intvalue;

					StrCpyW(Info->ItemSrc, GetItemName(Info->RecourceName, xorstr_("CSO_Item_Name_")));

					m_ItemMap.insert(std::map<int, PGAME_ITEMMAP>::value_type(ItemId, Info));

#if ANTI_DEBUG
					//uncode->utf8
					auto Utf8Text = m_Tool->UnicodeToUtf8(Info->ItemSrc);

					if (Info->RecourceType == ClassType_WeaponPaintItem)
					{
						m_GamePaintMap.insert(std::map<int, std::wstring>::value_type(ItemId, Info->ItemSrc));
						WeaponPaintItem["WeaponPaintItem"][Utf8Text] = { {"ID", ItemId} };
					}
					//else if (Info->RecourceType == ClassType_Equipment)
					{
						Item["Item"][Utf8Text] = { {"ID", ItemId} };
					}
#endif


					

					

					//ACEDebugFileLog("[{}]Line={} id={} Src={} ClassName ={} Category = {} InGameID ={} SortingIndex = {} Src={}", __FUNCTION__, lineindex, ItemId, Info->RecourceName, Info->ClassName, Info->Category, Info->InGameID, Info->SortingIndex,m_Tool->WstringToString(Info->ItemSrc));

					break;
				}
				
				default:

					break;
				}
				list = list->next;
				col++;

			}
		}
		lineindex++;
		p = p->next;
	}

	g_pFileSystem->Close(hFile);
#if ANTI_DEBUG	
	ofItem << Item.dump(4);
	ofItem.close();
	
	ofWeaponPaintItem << WeaponPaintItem.dump(4);
	ofWeaponPaintItem.close();
#endif
	VirtualFree(ReadBuff, 0, MEM_RELEASE);

}

void Util::InitinWeaponItem()
{
	ULONG ulAddresPoint = g_Offset.GetGameAddresInfo()->dwItemPoint;
	m_ItemWeaponInfo.clear();
	m_ItemWeaponToIndex.clear();

	while (true)
	{
		PTAG_GAME_ITEMS pInfo = (PTAG_GAME_ITEMS)(ulAddresPoint);

		pInfo->money = 0;
#if 0

		if ((pInfo->Ponint_item_name)[0] && lstrcmpA((pInfo->Ponint_item_name)[0], "") != 0)
		{
			char Ansi[1024] = { 0 };

			AntiUnicodeToAnsi(this->ItemCodeToItemNameByUncode((pInfo->Ponint_item_name)[0], ""), Ansi);

			AntiFileLog("[%s]Count=%d Code=%s name %s Src %s type=%d %d %d %d %d", __FUNCTION__, pInfo->count, pInfo->Ponint_item_code[0], Ansi, g_Util.ItemCodeToItemNameByAsii(pInfo->Ponint_un_name_1[0]), pInfo->type, pInfo->un_int_1, pInfo->un_int_3, pInfo->un_int_4, pInfo->money);

		}

#endif 

		if (pInfo->count == -1)
			break;

		//name_1:p228 name_2:p228 item_code:p228 item_name:#CSO_P228
		//ACEDebugLog("count:{} name_1:{} name_2:{} item_code:{} item_name:{} ", pInfo->count, pInfo->Ponint_un_name_1[0] == 0 ? "null" : pInfo->Ponint_un_name_1[0], pInfo->Ponint_un_name_2[0] == 0 ? "null": pInfo->Ponint_un_name_2[0], pInfo->Ponint_item_code[0] == 0 ? "null" : pInfo->Ponint_item_code[0], pInfo->Ponint_item_name[0] == 0 ? "null" : pInfo->Ponint_item_name[0]);

		m_ItemWeaponInfo.insert(std::map<int, PTAG_GAME_ITEMS>::value_type(pInfo->count, pInfo));

		//把ItemIdex 作为Key .额其实在初始化Item那里也可以写
		std::map<int, PGAME_ITEMMAP>::iterator iter = m_ItemMap.begin();

		while (iter != m_ItemMap.end())
		{
			if (iter->second->RecourceType == ClassType_Equipment && iter->second->InGameID == pInfo->count)
			{
				int Index = iter->first;

				m_ItemWeaponToIndex.insert(std::map<int, int>::value_type(pInfo->count, Index));

				break;
			}
			iter++;
		}

		ulAddresPoint += sizeof(TAG_GAME_ITEMS);
	}

	InitGameMode();
	InitGameMap();
}

BOOL Util::IsGranade(int iWeaponID)
{
	switch (iWeaponID)
	{
	case 4:
	case 9:
	case 44:
	case 296:
	case 458:
	case 2002:
	case 2003:
	case 2004:
		return TRUE;
	default:
	{
		PTAG_GAME_ITEMS Info = GetGameWeaponItemInfo(iWeaponID);
		if (!Info)
		{
			return FALSE;
		}
		if (Info->type == 9 && Info->un_int_1 == 4)
		{
			return TRUE;
		}
	}
	break;
	}
	return FALSE;
}

PTAG_GAME_ITEMS Util::GetGameWeaponItemInfo(int iWeaponID)
{
	auto it = m_ItemWeaponInfo.find(iWeaponID);

	if (it == m_ItemWeaponInfo.end())
	{
		return nullptr;
	}

	return m_ItemWeaponInfo.at(iWeaponID);
}

bool Util::GameModeIsZombi()
{
	bool bResult = false;

	short GameMode = m_Memory->Read<SHORT>(g_Offset.GetGameAddresInfo()->dwRoomMode);

	switch (GameMode)
	{
	case CSO_GameMode_Zombi:
	case CSO_GameMode_Zombi_Expand:
	case CSO_GameMode_Zombi_3:
	case CSO_GameMode_Zombi_Escape:
	case CSO_GameMode_BotZombie:
	case CSO_GameMode_Zombi_4:
	case CSO_GameMode_Zombi_Exterminate:
	case CSO_GameMode_ZombieOfficial:
	case CSO_GameMode_ZombieOfficial_TieBreak:
	case CSO_GameMode_SeasonZombi_3:
	case CSO_GameMode_Zombi_3z:
	case CSO_GameMode_Zombi_5:
	case CSO_GameMode_PlayRoom3:
		
		bResult = true;
		break;
	default:
		break;
	}
	return bResult;
}

std::map<int, PTAG_GAME_ITEMS>* Util::GetItemWeaponInfo()
{
	return &m_ItemWeaponInfo;
}

wchar_t* Util::GetPlayerNameByIndex(int Index)
{
	wchar_t* pName = nullptr;
	ULONG v1       = g_Offset.GetGameAddresInfo()->dwPlayerMgrAddres;
	ULONG vCall    = g_Offset.GetGameAddresInfo()->dwPlayerMgrCall;

	__asm
	{
		pushad
		mov ecx, dword ptr ds : [v1]
		mov ecx, dword ptr ds : [ecx]
		push Index
		mov eax, vCall
		call eax
		mov eax, [eax + 0x40]
		// 2020年11月23日 09:53:30 更新偏移 0x40
		mov pName, eax
		popad

	}
	return pName;
}

void Util::GameNoticeBox(std::string TextMsg)
{

	char  Title[] = {'0','0','7','S','e','r','v','e','r','\0' };

	char szTextMsg[1024] = { 0 };
	strcpy(szTextMsg, TextMsg.data());

	static ULONG ulCsoMsgBoxEntry = g_Offset.GetGameAddresInfo()->dwMsg;
	ulong This = 0;

	__asm
	{
		pushad
		mov eax, ulCsoMsgBoxEntry
		mov ecx, dword ptr ds : [eax]
		_emit 0x6A
		_emit 0xFF
		/*
		*	高版本的VS push addres 会变成 push [address]
		*/
		lea edx, szTextMsg
		push edx
		lea edx, Title				
		push edx
		lea edx, This
		push edx
		mov eax, dword ptr ds : [ecx]
		call dword ptr ds : [eax + 000000ACh]
		popad
	}
}

std::map<int, PGAME_ITEMMAP>* Util::GetItemMap()
{
	return &m_ItemMap;
}

PGAME_ITEMMAP Util::GetWeaponInfoByWeaponID(int iWeaponID)
{
	PGAME_ITEMMAP result = nullptr;
	if (m_ItemWeaponToIndex.count(iWeaponID) <= 0)
	{
		return result;
	}

	//获取道具索引
	ULONG uItemIndex = m_ItemWeaponToIndex.at(iWeaponID);

	result = GetItemMap()->at(uItemIndex);

	return result;
}

bool Util::FindGameResourceFile(std::string FileName)
{
	auto AhnFile = std::make_unique<CACEFile>();
	bool  nRet   = false;
	auto AppPath = AhnFile->GetCurrentDirectoryA();

	int Pos = AppPath.rfind(xorstr_("Bin\\"));

	if (Pos == -1)
	{
		Pos = AppPath.rfind(xorstr_("bin\\"));
	}


	AppPath.replace(Pos, strlen(xorstr_("Bin\\")), xorstr_("Data\\"));

	WIN32_FIND_DATA FindFileData;

	HANDLE hFind;

	AppPath.append(FileName);

	hFind = FindFirstFileA(AppPath.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		ACEInfoLog("Game Path :{}", AppPath);
		nRet = true;
	}
	return nRet;
}

PTAG_VGUI_INPUT Util::GetVguiMousetimer()
{
	/*
	*	  8B C8                 - mov ecx,eax
		  8B 10                 - mov edx,[eax]
		  8B 42 44              - mov eax,[edx+44]
		  FF D0                 - call eax
	*/
	//获取鼠标时间地址
	ULONG ulV1Call = g_Offset.GetGameAddresInfo()->dwGameVguiMouseIpunt;

	static ULONG ulV2Call = 0;

	if (ulV2Call)
		return (PTAG_VGUI_INPUT)(ulV2Call);

	_asm
	{
		pushad
		lea eax, ulV1Call
		call dword ptr ds : [eax]
		mov ecx, eax
		mov edx, dword ptr ds : [eax]
		mov eax, dword ptr ds : [edx + 0x44]
		mov ulV2Call, eax
		popad
	}

	//算出鼠标事件地址_找到 Jmp 地址
	ulV2Call = ulV2Call + 5 + *(PULONG)(ulV2Call + 1);

	ulV2Call = *(PULONG)(ulV2Call + 2);
	ulV2Call -= 8;

	/*
	*	0 = 不知道啥地址
	*   8 = 鼠标
	*   16 = 键盘
	*/

	return (PTAG_VGUI_INPUT)(ulV2Call);
}

double Util::GetVguiIpunttimer()
{

	ULONG ulV1Call = g_Offset.GetGameAddresInfo()->dwGameVguiIpuntTime;

	double v2Timer;

	_asm
	{
		pushad
		lea eax, ulV1Call
		call dword ptr ds : [eax]
		mov ecx, eax
		mov edx, dword ptr ds : [eax]

		call dword ptr ds : [edx + 0x10]
		fstp qword ptr ss : [v2Timer]
		popad
	}
	return v2Timer;
}

void Util::InitGameMap()
{
	VMProtectBegin(__FUNCTION__);

	m_GameMap.clear();

	CreateInterfaceFn filesystem_factory = CaptureFactory(xorstr_("FileSystem_Nar.dll"));
	IFileSystem* g_pFileSystem = (IFileSystem*)(CaptureInterface(filesystem_factory, xorstr_("VFileSystem009")));

	if (!g_pFileSystem)
		return;

	FileHandle_t hFile = g_pFileSystem->Open(xorstr_("resource\\maplist.csv"), xorstr_("rb"), NULL);

	int FileSize = g_pFileSystem->Size(hFile);

	char* ReadBuff = (char*)VirtualAlloc(NULL, FileSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	g_pFileSystem->Read(ReadBuff, FileSize, hFile);

	csv_data* data = Csv_LoadEx(ReadBuff);

	csv_data* p = data;

	csv_value_list* list;

	int lineindex = 1;

	int col = 0;

	if (!data) return;

	while (p)
	{

		col = 0;

		if (p->value_list)
		{
			list = p->value_list;

			PGAME_MAPINFO pInfo = new GAME_MAPINFO();

			int k = 0;

			while (list)
			{
				k++;

				switch (k)
				{
				case 1:
					pInfo->map_id = list->value.intvalue;
					break;
				case 2:
				{
					pInfo->map_name        = list->value.stringvalue;
					pInfo->map_translation = GetItemName(list->value.stringvalue, "");

					break;
				}
				case 3:
					pInfo->map_code = list->value.stringvalue;
					break;
				case 4:
				{
#if 0
					ACEInfoFileLog("[{}] ID:{} name:{} code:{} Chn:{}",__FUNCTION__, pInfo->map_id,pInfo->map_name, pInfo->map_code, m_Tool->WstringToString(pInfo->map_translation));
#endif
					auto result =  m_GameMap.insert(std::map<int, PGAME_MAPINFO>::value_type(pInfo->map_id, pInfo));

					if (std::get<1>(result) == false)
					{
						delete m_GameMap.at(pInfo->map_id);
						m_GameMap.at(pInfo->map_id) = pInfo;
					}
				}
					break;

				}
				list = list->next;
				col++;

			}
		}
		lineindex++;
		p = p->next;
	}
	g_pFileSystem->Close(hFile);

	VirtualFree(ReadBuff, 0, MEM_RELEASE);



	VMProtectEnd();
}

std::map<int, PGAME_MAPINFO>* Util::GetGameMap()
{
	return &m_GameMap;
}

void Util::InitGameMode()
{
	m_GameMode.clear();

	ULONG ulIndex = 0;

	auto GetGameMode = [&](std::string Text)->std::wstring {
		return this->GetItemName(Text.data(), "");
	};

	std::vector<std::string> VecGameModeStr;


	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Original"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_DeathMatch"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_TeamDeathMatch"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Bot"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_BotDM"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_BotTDM"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Official"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Official_TieBreak"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Expand"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Team"));
	VecGameModeStr.push_back(xorstr_("unk"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Challenge"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Team_Ann"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_3"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Survival"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Soccer"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Human_Scenario"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_PVPVE"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_TDM_Item"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Escape"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Bazooka"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_GunDeath"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_LightOri"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_BotZombie"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_BattleRush"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieShelter"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Snowman"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieShelterT"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_4"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieGiant"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_FightYard"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_Exterminate"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_PVE"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_SuddenDeath"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieOfficial"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieOfficial_TieBreak"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieTag"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_VoxelCreate"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_VoxelPVE"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Allstar"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_PlayRoom"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_SeasonOriginal"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_SeasonZombieEx"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_SeasonZombi_3"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_3z"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZombieTouchDown"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_SeasonZombieTouchDown"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_PlayRoom2"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_VoxelPropHunt"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_partnermode"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ZHE"));
	VecGameModeStr.push_back(xorstr_("cso_gamemode_vxlzshelter"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_ScenarioTX"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Zombi_5"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_PlayRoom3"));
	VecGameModeStr.push_back(xorstr_("CSO_GameMode_Lab"));
	for (int i = CSO_GameMode_Original; i <= CSO_GameMode_Lab; i++)
	{
		m_GameMode.insert(std::map<int, std::wstring>::value_type(i, GetGameMode(VecGameModeStr.at(i))));
	}

}

std::map<int, std::wstring>* Util::GetGameMode()
{
	return &m_GameMode;
}

std::wstring& Util::SearchPaintItem(ULONG Index)
{
	auto result =  m_GamePaintMap.find(Index);

	if (result != m_GamePaintMap.end())
	{
		return result->second;
	}
	
	ACEDebugFileLogW(L"[{}] Index:{} not found",__FUNCTIONW__, Index);

	return std::wstring(L"not found");
}

void Util::ViewHexCode(std::shared_ptr<PackReader> Pack)
{
	auto VecPack = std::vector<BYTE>(Pack->GetSize());
	memcpy(VecPack.data(), Pack->GetBuffer(), Pack->GetSize());
	ACEWarningFileLog("[{}] {:02X}", __FUNCTION__, fmt::join(VecPack.begin(), VecPack.end(), " "));
}
void Util::CreatePlayerName()
{
	CACEUtil Tool;

	auto RandStrFuns = [&](char* Text)
	{
		srand(time(NULL));

		int len = Tool.GetRandomNumber(4, 16);

		int i;

		for (i = 0; i < len; ++i)
		{
			if (i == 0)
			{
				//第一个字符 必须是大写
				Text[i] = 'A' + rand() % 26;
				continue;;
			}
			switch ((rand() % 3))
			{
			case 1:
				Text[i] = 'A' + rand() % 26;
				break;
			case 2:
				Text[i] = 'a' + rand() % 26;
				break;
			default:
				Text[i] = '0' + rand() % 10;
				break;
			}
		}
		Text[++i] = '\0';
	};


	char		PlalyerName[MAX_PATH] = { 0 };
	RandStrFuns(PlalyerName);

	ULONG ClassPoint = 0;
	if (CACEInterface::GetInstance()->GetMemory()->GetScanCode("8B 0D ?? ?? ?? ?? 85 C9 74 0F 8B 01 8D 95", ClassPoint, g_Offset.GetGameUiModuleInfo()->hModule))
	{
		ClassPoint = *(PULONG_PTR)(ClassPoint + 2);

	};
	wchar_t		CteatePlalyerName[MAX_PATH];
	Tool.AnsiToUnicode(PlalyerName, CteatePlalyerName);

	__asm
	{
		pushad
		lea eax, CteatePlalyerName
		push eax
		mov ecx, dword ptr ss : ClassPoint
		mov ecx, dword ptr ds : [ecx]
		mov eax, dword ptr ds : [ecx]
		call dword ptr ds : [eax + 0x80]
		popad
	}
}
