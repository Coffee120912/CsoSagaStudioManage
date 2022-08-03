#pragma once
#include "QtConnect.h"
class Config
{
public:
	Config();
	
	// �Ƿ��Զ�����APP
	int GetAutoStart();

	// ȡ�����ӳ�
	int GetStartDelay();

	// ������Ϸ·��
	const std::string GetGamePath();

	// ���ش����˺�
	const std::string GetAgentUser();


	// �����ر������
	const int GetAutoAuction1000M();

	// �����ر������
	void SetAutoAuction1000M(int value);

	// @������Ϸ·��
	void SetGamePath(std::string Path);
	
	// @���ô����˺�
	void SetAgentUser(std::string User);

private:
	CACEFile m_File;
};
extern Config* g_pConfig;
