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
		TAG_LAUNCHER_STRING((char*)"launched")	,TAG_LAUNCHER_STRING((char*)"1"),
		TAG_LAUNCHER_STRING((char*)"mode")		,TAG_LAUNCHER_STRING((char*)""),
		TAG_LAUNCHER_STRING((char*)"passport")	,TAG_LAUNCHER_STRING((char*)""),
		TAG_LAUNCHER_STRING((char*)"region")	,TAG_LAUNCHER_STRING(Region),	//大区
		TAG_LAUNCHER_STRING((char*)"type")		,TAG_LAUNCHER_STRING((char*)"0"),
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
}

void Util::SetValue(LPVOID Info, int& Index, int Size, ULONG* Value)
{
	memcpy((void*)((ULONG)Info + Index), Value, Size);
	Index = Index + Size;
}
