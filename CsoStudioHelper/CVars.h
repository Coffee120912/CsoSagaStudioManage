#pragma once
#include <ACEConnect.h>
#include <string>


class CVars
{
public:
	CVars();
	
	int m_nUserSign;
	std::string m_UserName;
	std::string m_FakeUserName;
	std::string m_PlayerName;
	ULONG m_Level;
	ULONG64 m_Gold;
	ULONG64 m_Integral;


};

extern CVars g_Vars;
