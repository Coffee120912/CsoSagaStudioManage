#pragma once
#include <CACEInterface.h>
#include "./../Connect/ShareDef.h"
#include <thread>

typedef struct TAG_LAUNCHER_STRING
{
	int		Size;
	char	Name[MAX_PATH];
	TAG_LAUNCHER_STRING(char* Value) : Size(0)
	{
		Size = wsprintfA(Name, "%s", Value);
	}
}*PTAG_LAUNCHER_STRING;

typedef struct TAG_LAUNCHER_PARAM
{
	TAG_LAUNCHER_STRING	Name;
	TAG_LAUNCHER_STRING	Value;
}*PTAG_LAUNCHER_PARAM;



class Util
{
public:
	// @�ر�CSO������
	void	CloseCsoSharedDict();

	// @������Ϸ��������Ϣ
	bool SetGameRegion(ULONG Region);
private:
	void SetValue(LPVOID Info, int& Index, int Size, ULONG* Value);
};

extern Util g_Util;