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

	// @��ʼ��
	BOOL Init();
	
	// @��½
	BOOL Login();
	
	// @���汾����
	BOOL CheckUpdata();
	
	std::wstring GetErrorStr() const ;

	// @���ͨѶ�Ƿ�����
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
