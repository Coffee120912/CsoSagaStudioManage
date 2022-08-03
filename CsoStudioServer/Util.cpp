#include "Util.h"

Util g_Util;


void Util::CloseCsoSharedDict()
{

	LPVOID pBuffer = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE);

	if (pBuffer)
	{
		ULONG ulRetLent = 0;

		NTSTATUS Status = NtQuerySystemInformation(SystemHandleInformation, pBuffer, 0x1000, &ulRetLent);

		if (!NT_SUCCESS(Status))
		{
			pBuffer = VirtualAlloc(NULL, ulRetLent, MEM_COMMIT, PAGE_READWRITE);

			NtQuerySystemInformation(SystemHandleInformation, pBuffer, ulRetLent, &ulRetLent);
		}

		PSYSTEM_HANDLE_INFORMATION pSysInfo = (PSYSTEM_HANDLE_INFORMATION)pBuffer;

		for (ULONG r = 0; r < pSysInfo->NumberOfHandles; r++)
		{
			if (pSysInfo->Handles[r].UniqueProcessId == GetCurrentProcessId())
			{

				if (pSysInfo->Handles[r].ObjectTypeIndex == 42 || pSysInfo->Handles[r].ObjectTypeIndex == 33)
				{

					wchar_t wszName[0x1024];
					ULONG uRet = 0;

					NtQueryObject((HANDLE)pSysInfo->Handles[r].HandleValue, ObjectNameInformation, wszName, 0x200, &uRet);

					ACEDebugFileLogW(L"[{}] {}", __FUNCTIONW__, wszName);

					if (StrStrW(wszName, L"CSO.SharedDict"))
					{
						CloseHandle((HANDLE)pSysInfo->Handles[r].HandleValue);
					}

				}
			}

		}

	}
}

bool Util::SetGameRegion(ULONG RegionID)
{
	const char* Name = "CSO.SharedDict";

	ULONG	hSize = 0x8000;

	HANDLE hFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, Name);

	if (!hFile)
	{
		hFile = CreateFileMappingA(GetCurrentProcess(), NULL, PAGE_EXECUTE_READWRITE, 0, hSize, Name);
	}

	if (!hFile) return false;


	LPVOID	Info = MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, hSize);

	if (!Info) return false;

	char	Region[MAX_PATH];
	wsprintfA(Region, "%d", RegionID);

	TAG_LAUNCHER_PARAM	ArrayInfo[] =
	{
		TAG_LAUNCHER_STRING("launched")	,TAG_LAUNCHER_STRING("1"),
		TAG_LAUNCHER_STRING("mode")		,TAG_LAUNCHER_STRING(""),
		TAG_LAUNCHER_STRING("passport")	,TAG_LAUNCHER_STRING(""),
		TAG_LAUNCHER_STRING("region")	,TAG_LAUNCHER_STRING(Region),	//大区
		TAG_LAUNCHER_STRING("type")		,TAG_LAUNCHER_STRING("0"),
	};

	//填充数据
	ULONG	Number = sizeof(ArrayInfo) / sizeof(TAG_LAUNCHER_PARAM);;
	ULONG	Value = Number;
	int		Index = 4;
	SetValue(Info, Index, 4, &Value);	//参数数量

	for (int i = 0; i < (int)Number; i++)
	{
		PTAG_LAUNCHER_PARAM	ParamInfo = &ArrayInfo[i];

		Value = ParamInfo->Name.Size;

		SetValue(Info, Index, 4, &Value);
		SetValue(Info, Index, ParamInfo->Name.Size, (ULONG*)ParamInfo->Name.Name);

		Value = ParamInfo->Value.Size;
		SetValue(Info, Index, 4, &Value);
		SetValue(Info, Index, ParamInfo->Value.Size, (ULONG*)ParamInfo->Value.Name);
	}
	return true;
}

std::string Util::GetServerRegionByIndexToStr(ULONG ulRegion)
{
	std::string strRegion;
	switch (ulRegion)
	{
	case 5:
		strRegion = "电信一区";
		break;
	case 6:
		strRegion = "电信二区";
		break;
	case 15:
		strRegion = "网通一区";
		break;
	default:
		strRegion = "未知大区";
		break;
	}
	return strRegion;
}



void Util::SetValue(LPVOID Info, int& Index, int Size, ULONG* Value)
{
	memcpy((void*)((ULONG)Info + Index), Value, Size);
	Index = Index + Size;
}

TAG_GAME_USER_STATUS GameUserStatus[] =
{
	USER_STATUS::USER_SUCCEED	,	"账号正常",
	USER_STATUS::USER_ERROR_R0	,	"保护封停",
	USER_STATUS::USER_ERROR_R1	,	"外挂封停",
	USER_STATUS::USER_ERROR_R2	,	"登陆上限",
	USER_STATUS::USER_ERROR_R3	,	"密码错误",
	USER_STATUS::USER_ERROR_R4	,	"账号锁定",
	USER_STATUS::USER_ERROR_R5	,	"未实名认证",


	USER_STATUS::USER_ERROR_UN	,	"未知状态",

};
PTAG_GAME_USER_STATUS Util::GetUserStatusByIndex(USER_STATUS Index)
{
	for (size_t i = 0; i < sizeof(GameUserStatus) / sizeof(TAG_GAME_USER_STATUS); i++)
	{
		if (GameUserStatus[i].Index == Index)
		{
			return &GameUserStatus[i];
		}
	}
	return &GameUserStatus[(int)USER_STATUS::USER_ERROR_UN];
}

TAG_GAME_TASK_SRC GameTaskInfo[] =
{
	TASK_MODE::GAME_TASK_LOBBY        ,			"游戏大厅挂机",
	TASK_MODE::GAME_TASK_MP_ZBS_NOTKEY,	        "生化单人积分",
	//TASK_MODE::GAME_TASK_MP_PVE_ONLINE,	        "绝命多人挂机",
	TASK_MODE::GAME_TASK_ZBS_ONLINE   ,	        "生化多人挂机",
	TASK_MODE::GAME_TASK_BATING       ,		    "批量账号检测",
	TASK_MODE::GAME_TASK_Vxl_PVE      ,		    "缔造单人挂机",
	//TASK_MODE::GAME_TASK_PVP_ONLINE   ,	        "多人任务挂机",	
	TASK_MODE::GAME_TASK_AUCTION      ,	        "拍卖行扫拍",
	TASK_MODE::GAME_TASK_UNKONW       ,	        "未知的任务",

};
PTAG_GAME_TASK_SRC Util::GetTaskByIndex(TASK_MODE Mode)
{
	for (size_t i = 0; i < sizeof(GameTaskInfo) / sizeof(TAG_GAME_TASK_SRC); i++)
	{
		if (GameTaskInfo[i].Mode == Mode)
		{
			return &GameTaskInfo[i];
		}
	}
	return &GameTaskInfo[(int)TASK_MODE::GAME_TASK_UNKONW];
}

PTAG_GAME_TASK_SRC Util::GetTaskByName(std::string TaskName)
{
	for (size_t i = 0; i < sizeof(GameTaskInfo) / sizeof(TAG_GAME_TASK_SRC); i++)
	{
		if (GameTaskInfo[i].Text == TaskName)
		{
			return &GameTaskInfo[i];
		}
	}
	return &GameTaskInfo[(int)TASK_MODE::GAME_TASK_UNKONW];
}

int Util::GetItemIDByName(std::string ItemName)
{
	if (m_ItemJson.empty())
		JsonInit();

	try
	{
		auto Result =  m_ItemJson.at("Item").contains(ItemName);
		if (Result)
		{
			int ID = m_ItemJson.at("Item").at(ItemName).at("ID").get<int>();
			return ID;
		}

	}
	catch (nlohmann::detail::exception& e)
	{
		ACEErrorLog("{}:parse json error!", e.what());

	}
	return -1;
}

std::string Util::GetItemNameByID(int ItemID)
{
	if (m_ItemJson.empty())
		JsonInit();

	try
	{
		for (auto &item : m_ItemJson.at("Item").items())
		{
			//ACEDebugLog("{} {}", CACEInterface::GetInstance()->GetUtil()->UTF8_To_string(item.key()), item.value().at("ID").get<int>());
			if (item.value().at("ID").get<int>() == ItemID)
			{
				return item.key();
			}
		}
	}
	catch (nlohmann::detail::exception& e)
	{
		ACEErrorLog("{}:parse json error!", e.what());

		return "itemNameException";
	}

	return "";

}

void Util::JsonInit()
{

	CACEHook   ACEHook;
	auto PathItem = fmt::format(xorstr_("{}\\Config\\Item.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));
	auto PathWeaponPaintItem = fmt::format(xorstr_("{}\\Config\\WeaponPaintItem.json"), ACEHook.GetModulePath(ACEHook.GetSelfModuleHandle()));

	auto pACEFile = std::make_shared<CACEFile>(PathItem);

	if (pACEFile->OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);
	}
	else
	{
		auto  VecByte = pACEFile->ReadFile();
		m_ItemJson = nlohmann::json::parse(VecByte.begin(), VecByte.end());
	}
	pACEFile->SetFileName(PathWeaponPaintItem);
	if (pACEFile->OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", PathItem);
	}
	else
	{
		auto  VecByte = pACEFile->ReadFile();
		m_Paintson = nlohmann::json::parse(VecByte.begin(), VecByte.end());
	}

}

std::map<ULONG, std::string> &Util::GetPaintMap()
{
	if (m_PaintMap.empty())
	{
		JsonInit();

		try
		{
			for (auto& item : m_Paintson.at("WeaponPaintItem").items())
			{
				m_PaintMap.insert(std::map<ULONG, std::string>::value_type(item.value().at("ID").get<int>(), item.key()));
			}
		}
		catch (nlohmann::detail::exception& e)
		{
			ACEErrorLog("{}:parse json error!", e.what());
		}

	}

	return m_PaintMap;
}


