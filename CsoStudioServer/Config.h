#pragma once
#include "QtConnect.h"
class Config
{
public:
	Config();
	
	// 是否自动启动APP
	int GetAutoStart();

	// 取启动延迟
	int GetStartDelay();

	// 返回游戏路径
	const std::string GetGamePath();

	// 返回代理账号
	const std::string GetAgentUser();


	// 返回特别拍卖活动
	const int GetAutoAuction1000M();

	// 设置特别拍卖活动
	void SetAutoAuction1000M(int value);

	// @设置游戏路径
	void SetGamePath(std::string Path);
	
	// @设置代理账号
	void SetAgentUser(std::string User);

private:
	CACEFile m_File;
};
extern Config* g_pConfig;
