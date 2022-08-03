#include "VerifyManage.h"
VerifyManage g_FreeYun;

VerifyManage::VerifyManage()
{
	CACEHwid Hwid;
	auto pNet = Hwid.GetActiveNetworkAdapter(FALSE);
	m_Mac = pNet->GetMacAddress();
	m_Heart.reset(1);

}

BOOL VerifyManage::Init()
{
	VMProtectBegin(__FUNCTION__);
	
	TAG_ANTI_FREEYUN_PROXY Proxy;

	if (std::get<0>(Result) == false)
	{
		m_ErrStr = m_Util.UTF8_To_UniCode(std::get<1>(Result));
		return FALSE;
	}
	auto& data = std::get<2>(Result);

	int nCode = data.at(xorstr_("code"));

	if (nCode != 1003)
	{
		m_ErrStr = m_Util.UTF8_To_UniCode(m_FreeYun.GetErrorStr(nCode));
		return TRUE;
	}	
	VMProtectEnd();
	return TRUE;
}

BOOL VerifyManage::Login()
{
	VMProtectBegin(__FUNCTION__);

	BOOL bRet = FALSE;
	
Lab_Login:

	if (std::get<0>(result) == false)
	{
		int nCode = m_FreeYun.GetErrorCode();

		if (nCode == 1010)
		{
			//需解绑

			if (std::get<0>(result) == true)
			{
				goto Lab_Login;
			}
			else
			{
				m_ErrStr = xorstr_(L"解绑失败,卡号过期");
				return false;
			}
		}
		m_ErrStr = m_Util.UTF8_To_UniCode(m_FreeYun.GetErrorStr(nCode));
		return false;
	}
	bRet = std::get<0>(result);
	m_ErrStr = m_Util.UTF8_To_UniCode(std::get<1>(result));
	VMProtectEnd();
	return bRet;
}

BOOL VerifyManage::CheckUpdata()
{
	VMProtectBegin(__FUNCTION__);
		

	if (std::get<0>(result))
	{
		auto& dataVar = std::get<2>(result);
		std::string VarText = dataVar.at(xorstr_("variable"));

		if (lstrcmpiA(VarText.data(), SAGA_VER) == 0)
		{
			return TRUE;
		}

		m_ErrStr = xorstr_(L"版本不匹配,请升级版本！");
		return FALSE;
	}
	m_ErrStr = m_Util.UTF8_To_UniCode(std::get<1>(result));
	VMProtectEnd();
	return  std::get<0>(result);
}

std::wstring VerifyManage::GetErrorStr() const
{
	return m_ErrStr;
}

BOOL VerifyManage::CheckConnect()
{
	VMProtectBegin(__FUNCTION__);
	
	auto pResult = m_FreeYun.CloudHeartBeat(m_user);

	if (std::get<0>(pResult) == false)
	{
		m_ErrStr = m_Util.UTF8_To_UniCode(std::get<1>(pResult));
	}
	
	VMProtectEnd();
	return std::get<0>(pResult);
}
