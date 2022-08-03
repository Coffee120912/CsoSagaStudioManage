#pragma once
#include "QtConnect.h"
#include "Config.h"
#include <FreeYun.h>
#include "thread_pool.hpp"
#pragma comment(lib,"FreeYun.lib")

#define  SAGA_VER  xorstr_("saga_ver_1.1.4")


class VerifyManage
{
public:
	VerifyManage();

	// @初始化
	BOOL Init();
	
	// @登陆
	BOOL Login();
	
	// @检测版本升级
	BOOL CheckUpdata();
	
	std::wstring GetErrorStr() const ;

	// @检查通讯是否正常
	BOOL CheckConnect();


private:
	std::string  m_Mac;
	CACEUtil     m_Util;
	FreeYun      m_FreeYun;
	std::wstring m_ErrStr;
	std::string  m_user;
	thread_pool  m_Heart;
};
extern VerifyManage g_FreeYun;
