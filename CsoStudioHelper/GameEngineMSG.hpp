#pragma once
#include "SagaConnect.h"

/*
*	1 - ����ִ����Ϸ�ص�
*	2 - ����ִ����Ϸ�ص�,���ִ����Ϸ�ص�
*	3 - ����֮����麯��,���ִ����Ϸ�ص�
*	4 - ����֮����麯��,���ִ����Ϸ�ص�
*	
*/


class GameEngineMSG
{

public:
	//@��ʼ��
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@״ֵ̬-������ƶ�������ʱ��ʾ ��״̬�����ı�ʱ
	virtual	int vStatusValue(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@���˻�
	virtual int vPatrolDrone(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@Gui�˵�
	virtual int vVGUIMenu(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@����ת�ͻ�����Ϣ
	virtual int vMPToCL(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@����ת�ͻ�����Ϣ
	virtual int vMPToCL2(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@ը��������Ϣ
	virtual int vBombDrop(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}

	//@����HUD
	virtual int vResetHUD(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@��ɱ��Ϣ
	virtual int vDeathMsg(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@��ɱ��Ϣ
	virtual int vKillInfo(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@��Ǯ
	virtual int vMoney(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@����
	virtual int vBattery(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@״̬ͼ��
	virtual int vStatusIcon(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@����
	virtual int vHealth(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}
	//@�����Ϣ
	virtual int vPlayerInfo(const char* pszName, int iSize, void* pbuf)
	{
		return 1;
	}

};

