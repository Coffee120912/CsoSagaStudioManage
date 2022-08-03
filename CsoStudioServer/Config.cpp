#include "Config.h"
Config* g_pConfig = nullptr;
Config::Config()
{
	//g_pAppConfig = new QSettings(QString::fromLocal8Bit(m_File.GetCurrentDirectoryA("Config.ini").data()), QSettings::IniFormat);
	g_pAppConfig = new QSettings(QString::fromLocal8Bit("C:\\Config.ini"), QSettings::IniFormat);
	g_pAppConfig->setValue("CFG/mode", 1);

	g_pAppConfig->setIniCodec(QTextCodec::codecForName("UTF-8"));
}

int Config::GetAutoStart()
{
	return  g_pAppConfig->value("/AppConfig/App_AutoStart").toInt();
}

int Config::GetStartDelay()
{
	return  g_pAppConfig->value("/AppConfig/App_StartDelay").toInt() * 1000;
}

const std::string Config::GetGamePath()
{
	return  g_pAppConfig->value("/AppConfig/App_GamePath").toString().toLocal8Bit().constData();
}

const std::string Config::GetAgentUser()
{
	return  g_pAppConfig->value("/AppConfig/App_AgentUser").toString().toLocal8Bit().constData();
}

const int Config::GetAutoAuction1000M()
{
	int nRet = g_pAppConfig->value("/FunsConfig/AutoAuction1000M").toInt();
	return  nRet == 1 ? 2 : 0;
}

void Config::SetAutoAuction1000M(int value)
{
	g_pAppConfig->setValue("/FunsConfig/AutoAuction1000M", value);
}

void Config::SetGamePath(std::string Path)
{
	g_pAppConfig->setValue("/AppConfig/App_GamePath", Path.data());
}

void Config::SetAgentUser(std::string User)
{
	g_pAppConfig->setValue("/AppConfig/App_AgentUser", User.data());
}
