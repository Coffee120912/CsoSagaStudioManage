#include "CSOWrapper.h"
#include "GameOffset.h"
#include "GamePack.h"


CSOWrapper::CSOWrapper() 
	:m_CaptchaDlg(0), m_LoginDlgClass(0), m_LoginCall(0), m_2ndPasswordClass(0), m_CaptchThreadPool(nullptr),m_CaptchaEventOK(0)
{
	m_pInstance        = this;
	m_CaptchThreadPool = new thread_pool(1);
	m_pGameRCF         = CGameRCF::GetInstance();

}

CSOWrapper* CSOWrapper::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CSOWrapper();
	}
	return m_pInstance;
}

int CSOWrapper::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	//Hook game ui
	InstallGameUiHook();




	return 1;
}

bool CSOWrapper::vInit()
{
	m_Hook   = CACEInterface::GetInstance()->GetHook();
	m_Memory = CACEInterface::GetInstance()->GetMemory();

	//要用这个DLL的导出函数申请内存
	(FARPROC&)_malloc = m_Hook->GetProcAddress(GetModuleHandleA("ucrtbase.dll"), "malloc");

	//申请内存
	m_InputAnswer = (wchar_t*)_malloc(1024);
	::RtlZeroMemory(m_InputAnswer, 1024);
	m_InputAccount = (wchar_t*)_malloc(1024);
	::RtlZeroMemory(m_InputAccount, 1024);
	m_InputPassWord = (wchar_t*)_malloc(1024);
	::RtlZeroMemory(m_InputPassWord, 1024);
	


#if CPX_ALOGIN
	//安装Hook
	InstallHook();
#endif
	
	//加载验证码库
	LoadCaptLib();
	
	if (m_pGameRCF->GetLoginUser().empty())
	{
		//获取账号失败.
	}
	else
	{
		//通知登录器
		m_pGameRCF->LauncherEventNotice();
		m_pGameRCF->GetLoginUserPassWord();
		
#if CPX_ALOGIN
		CSOWrapper::GameLogin();
#endif
		m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_INIT_SUCCESS, u8"初始化成功！");
	}

	return true;
}

int CSOWrapper::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_F1)
	{
		
	}
	return 1;
}

void _stdcall CSOWrapper::CSONMEventListener(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;

	ULONG v1   = Array[1];
	ULONG vCode   = Array[2];

	ACEDebugFileLog("[{}]v1:0x{:X} v2:0x{:X}",__FUNCTION__, v1, vCode);

	auto pThis = CSOWrapper::GetInstance();



	switch (vCode)
	{
	case 0:
		if (v1 == 0)
		{
			pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_SUCCESS, u8"登陆成功");
			
			//销毁打码线程
			//if (pThis->m_CaptchThreadPool)
			//{
			//	delete pThis->m_CaptchThreadPool;
			//	pThis->m_CaptchThreadPool = nullptr;
			//}
		}
		else
		{
			pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_UNK_STR, fmt::format("{}:{:X}", u8"未知事件:Code:", v1));
		}
		break;
	case 10052:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R2, u8"账号密码错误或账号不存在,停止登陆");
		break;
	case 10053:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R6, u8"登陆过于频繁,请稍后再试");
		break;
	case 10054:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R3, u8"非正常途径账号封停");
		break;
	case -10003:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R4, u8"游戏维护中.请稍后再登陆");
		break;
	case -10001:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R5, u8"Ip地址被屏蔽,请更换IP代理");
		break;
	case -10002:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R7, u8"连接游戏服务器失败！");
		//重新登陆
#if CPX_ALOGIN
		pThis->GameLogin();
#endif
		break;
	case -10008:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R9, u8"该账号已锁定登录，请通过[天成账号管家APP]进行解锁。");
		break;
	case -10007:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R11, u8"请进行实名认证");
		break;
	case -10005:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_CAPT, u8"验证码错误,重新打码");
		break;
	default:
		pThis->m_pGameRCF->SendLoginEventNotice(LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_UNK_STR, fmt::format("{}:{:X}",u8"未知事件:Code:", vCode));
		break;
	}


}

void _stdcall CSOWrapper::LoginCallBack(PTAG_PEB_STACK Stack)
{
	char* lpLoginType = (char*)*(PULONG)((ULONG)Stack->ECX + 0x4C);

	ACEDebugFileLog("[{}] Type:{}",__FUNCTION__,lpLoginType);

	auto pThis = CSOWrapper::GetInstance();

	// 验证码输入
	if (lstrcmpiA(lpLoginType,"AnswerInput") == 0)
	{
		*(PULONG)((ULONG)Stack->ECX + 0xD0) = (ULONG)m_InputAnswer;

		*(PULONG)((ULONG)Stack->ECX + 0xD4) = wcslen(m_InputAnswer) + 1;
		*(PULONG)((ULONG)Stack->ECX + 0xDC) = wcslen(m_InputAnswer);

	}
	else if (lstrcmpiA(lpLoginType, "ID") == 0)
	{
		//获取账号
		auto UserName = pThis->m_pGameRCF->GetLoginUserInfo();
		auto ACText =  CACEInterface::GetInstance()->GetUtil()->StringToWstring(std::get<0>(UserName));

		StrCpyW(pThis->m_InputAccount, ACText.data());

		ACEInfoFileLogW(L"[{}] InputAccount:{}",__FUNCTIONW__, pThis->m_InputAccount);

		*(PULONG)((ULONG)Stack->ECX + 0xD0) = (ULONG)pThis->m_InputAccount;
		*(PULONG)((ULONG)Stack->ECX + 0xD4) = wcslen(pThis->m_InputAccount) + 1;
		*(PULONG)((ULONG)Stack->ECX + 0xDC) = wcslen(pThis->m_InputAccount);
	}
	else if (lstrcmpiA(lpLoginType, "Password") == 0)
	{
		//获取账号
		auto UserPassWord = pThis->m_pGameRCF->GetLoginUserInfo();
		auto ACText = CACEInterface::GetInstance()->GetUtil()->StringToWstring(std::get<1>(UserPassWord));

		StrCpyW(pThis->m_InputPassWord, ACText.data());

		ACEInfoFileLogW(L"[{}] InputPassWord:{}", __FUNCTIONW__, pThis->m_InputPassWord);

		*(PULONG)((ULONG)Stack->ECX + 0xD0) = (ULONG)pThis->m_InputPassWord;
		*(PULONG)((ULONG)Stack->ECX + 0xD4) = wcslen(pThis->m_InputPassWord) + 1;
		*(PULONG)((ULONG)Stack->ECX + 0xDC) = wcslen(pThis->m_InputPassWord);
	}

}

void _stdcall CSOWrapper::exCaptcha(PTAG_PEB_STACK Stack)
{
	//检查读取验证码的时机
	ULONG* Array          = (PULONG)Stack->ESP;
	char* TextMsg         = (char*)Array[0];
	static int capt_count = 0;

	ACEDebugFileLog("[{}] msg:{}",__FUNCTION__,TextMsg);

	auto CaptchaHumanBypass = [=]()
	{
		ACEDebugFileLog("[{}] {}",__FUNCTION__,"识别验证码");

		auto Text  =  CaptPath("capt.jpg");
		int Number = 0;
		try
		{
			Number = std::stoi(Text);

		}
		catch (const std::exception&)
		{
			Number = 0;
		}

		// 超出这个范围 都是错误的验证码 .可切换验证码.但是懒得找切换验证码的call了
		if (Number < 0 || Number > 81)
		{
			Text = "0";
			// 返回-1 服务器连接失败.所以一定要设置0
		}
		// > 91 错误的.
		CACEInterface::GetInstance()->GetUtil()->AnsiToUnicode(Text, m_InputAnswer);

		ACEDebugFileLog("[{}] capt result : {} {}", __FUNCTION__, Text, Number);

		CSOWrapper::GetInstance()->CaptchCall("ok");
		
		
		
	};


	//第一次输入或换验证码图片
	if (lstrcmpA(TextMsg, "#CSO_CaptchaMsg") == 0)
	{
		capt_count = 0;
		CSOWrapper::GetInstance()->m_CaptchThreadPool->push_task(CaptchaHumanBypass);
	}
	//验证不正确或重新输入验证码
	else if (lstrcmpA(TextMsg, "#CSO_AuthReply_Error(R9)") == 0)
	{
		if (++capt_count >= 2)
		{
			capt_count = 0;
			CSOWrapper::GetInstance()->m_CaptchThreadPool->push_task(CaptchaHumanBypass);
		}
	}
	else
	{
		capt_count = 0;
	}
}

void _stdcall CSOWrapper::exVguiGetLoginClass(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;
	CSOWrapper::GetInstance()->m_LoginDlgClass = (ULONG)Stack->EDI;
	ACEDebugFileLog("[{}] LoginDlgClass :0x{:X}",__FUNCTION__, (ULONG)Stack->EDI);
}



void CSOWrapper::GameLogin()
{
	char szText[] = "Login";
	ULONG This = m_LoginDlgClass;
	__asm
	{
		pushad
		lea eax, szText
		push eax
		mov ecx, dword ptr[This]
		mov eax, dword ptr[ecx]
		// 0x188 虚函数偏移 
		call  dword ptr[eax + 188h]
		popad
	}


}

void CSOWrapper::Set2ndPassWord(PASSWORD_2ND_TYPE Type, int Value)
{
	std::string Text;

	switch (Type)
	{
	case PASSWORD_2ND_TYPE::PasswordNumber:
		Text = fmt::format("PasswordNumber{}", Value);
		break;
	case PASSWORD_2ND_TYPE::Check:
		Text = "Check";
		break;
	case PASSWORD_2ND_TYPE::DoubleCheck:
		Text = "DoubleCheck";
		break;
	case PASSWORD_2ND_TYPE::DoubleCheck_OK:
		Text = "DoubleCheck_OK";
		break;
	default:
		break;
	}
	ULONG This = m_2ndPasswordClass;

	auto szValue = Text.data();
	__asm
	{
		pushad
		mov eax, szValue
		push eax
		mov ecx, dword ptr[This]
		mov ecx, dword ptr[ecx]
		mov eax, dword ptr[ecx]  // vtable
		call  dword ptr[eax + 188h]
		popad
	}

}

void CSOWrapper::CaptchCall(std::string Type)
{
	ULONG uThis = *(PULONG)(m_CaptchaDlg);
	ULONG uCall = m_CaptchaEventOK;
	char* Value = Type.data();
	__asm
	{
		pushad
		push Value
		mov ecx, uThis
		mov edi, ecx
		mov eax, uCall
		call eax
		popad
	}
}

void CSOWrapper::StartTaskThread()
{
	auto result = m_pGameRCF->GetLoginUserInfo();
	
	if (std::get<0>(result).empty())
	{
		ACEErrorLog("[{}] 账号为空 任务停止", __FUNCTION__);
		return;
	}
	auto ThreadFuns = [&]() {

		Sleep(10 * 1000);

		//刷新仓库
		CGamePack::GetInstance()->RefreshStorage();
		//刷新积分商城
		CGamePack::GetInstance()->RefreshScoreShop();
		
		
		auto TaskMode =  m_pGameRCF->GetTaskMode();

		switch (TaskMode)
		{
		case TASK_MODE::GAME_TASK_LOBBY:
			break;
		case TASK_MODE::GAME_TASK_MP_ZBS_NOTKEY:
			break;
		case TASK_MODE::GAME_TASK_ZBS_ONLINE:
			break;
		case TASK_MODE::GAME_TASK_BATING:
			break;
		case TASK_MODE::GAME_TASK_Vxl_PVE:
			break;
		case TASK_MODE::GAME_TASK_AUCTION:
		{
			if (m_pGameRCF->GetAuctionTaskMap().size())
			{
				// 有拍卖任务则先刷新拍卖列表
				CGamePack::GetInstance()->RefreshAuction();
				CGamePack::GetInstance()->RefreshLefAuction();
			}

		}
			break;
		case TASK_MODE::GAME_TASK_UNKONW:
			break;
		default:
			break;
		}


	};

	std::thread(ThreadFuns).detach();
	ACEErrorLog("[{}] 启动任务线程", __FUNCTION__);
}

CGameRCF* CSOWrapper::GetRCF()
{
	return m_pGameRCF;
}

void CSOWrapper::InstallHook()
{
	ULONG ulLoginCallBackAddress = 0;

	if (m_Memory->GetScanCode("55 8B EC 51 83 B9 DC 00 00 00 00 74 4D", ulLoginCallBackAddress, g_Offset.GetGameUiModuleInfo()->hModule))
	{
		m_Hook->HookWithNakedEx(ulLoginCallBackAddress, 0xA, LoginCallBack);
	}
	//获取CSONMEventListener
	ULONG ulCSONMEventListenerAddress = 0;
	if (m_Memory->GetScanCode("85 FF 74 1C 8B CF E8 ?? ?? ?? ??", ulCSONMEventListenerAddress, g_Offset.GetEngineModuleInfo()->hModule))
	{
		auto Result =  m_Hook->GetScanOpCodeByAddress(ulCSONMEventListenerAddress, 0x20, 1, ACE_SCAN_OPCODE_TYPE_CALL);

		if (std::get<0>(Result))
		{
			ulCSONMEventListenerAddress = *(PULONG)(std::get<4>(Result) + 2);
			//read tabble
			m_Hook->HookWithNakedEx(*(PULONG)(ulCSONMEventListenerAddress), 0x5, CSONMEventListener);

		}
	}

	//Hook 读取验证码的时机
	ULONG ulCaptchaAddress = 0;
	if (m_Memory->GetScanCode("8B 8E 1C 02 00 00 8B 01 FF 90 5C 02 00 00", ulCaptchaAddress, g_Offset.GetGameUiModuleInfo()->hModule))
	{
		//83 C4 14 8B BB ?? ?? 00 00
		m_Hook->HookWithNakedEx(ulCaptchaAddress, 0x6, exCaptcha);
		/*
		* 搜索字符串 capt.jpg
		* 写入 jpg 后就是读取验证码的实际
		* 所以特征码 前面就是 fclose add esp,14
		*/

		/*
		*			2020年12月25日 16:05:00更新
		* 8B 8E 1C 02 00 00 8B 01 FF 90 5C 02 00 00  往下找
		* 登陆
		#CSO_CaptchaMsg
		第二次换验证码_验证这个
		#CSO_AuthReply_Error(R9)
		*/
	}
	//获取验证码Dlg
	if (m_Memory->GetScanCode("8B 1D ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? FF 15 ?? ?? ?? ??", m_CaptchaDlg, g_Offset.GetGameUiModuleInfo()->hModule))
	{
		m_CaptchaDlg = *(PULONG)(m_CaptchaDlg + 2);

		/*
		*
			33A4D460 - 8B 1D D86AED33        - mov ebx,[33ED6AD8] : [00000000]
			33A4D466 - 68 BC92DB33           - push 33DB92BC : [00006277]
			33A4D46B - 68 B027D433           - push 33D427B0 : ["capt.jpg"]
			33A4D470 - FF 15 B005CB33        - call dword ptr [33CB05B0]
		*/

		if (m_Memory->GetScanCode("55 8B EC 81 EC CC 02 00 00 A1 ?? ?? ?? ??", m_CaptchaEventOK, g_Offset.GetGameUiModuleInfo()->hModule))
		{
			/*
			* ok
			* change
				login
			*/

			/*
33AD4934  | 6A 00                        | push 0x0                                                              |
33AD4936  | 68 2CC0DC33                  | push gameui.33DCC02C                                                  | 33DCC02C:"saveID"
33AD493B  | 8BCE                         | mov ecx,esi                                                           |
33AD493D  | 8B16                         | mov edx,dword ptr ds:[esi]                                            |
33AD493F  | FF52 5C                      | call dword ptr ds:[edx+0x5C]                                          |
33AD4942  | 8B0E                         | mov ecx,dword ptr ds:[esi]                                            |
33AD4944  | 8B51 68                      | mov edx,dword ptr ds:[ecx+0x68]                                       |
33AD4947  | 8BCE                         | mov ecx,esi                                                           |
33AD4949  | 85C0                         | test eax,eax                                                          |
33AD494B  | 74 09                        | je gameui.33AD4956                                                    |
33AD494D  | 8D85 FCFEFFFF                | lea eax,dword ptr ss:[ebp-0x104]                                      |
33AD4953  | 50                           | push eax                                                              |
33AD4954  | EB 05                        | jmp gameui.33AD495B                                                   |
33AD4956  | 68 5759E333                  | push gameui.33E35957                                                  |
33AD495B  | 68 54B4DC33                  | push gameui.33DCB454                                                  | 33DCB454:"userID"
33AD4960  | FFD2                         | call edx                                                              |
			*/
		}
	}



	ACEDebugFileLog("[{}] ulLoginCallBackAddress:0x{:X}",__FUNCTION__, ulLoginCallBackAddress);
	ACEDebugFileLog("[{}] CSONMEventTable:0x{:X}", __FUNCTION__, ulCSONMEventListenerAddress);
	ACEDebugFileLog("[{}] ulCaptchaAddress:0x{:X}", __FUNCTION__, ulCaptchaAddress);
	ACEDebugFileLog("[{}] m_CaptchaDlg:0x{:X}", __FUNCTION__, m_CaptchaDlg);
	ACEDebugFileLog("[{}] m_CaptchaEventOK:0x{:X}", __FUNCTION__, m_CaptchaEventOK);
}

void CSOWrapper::LoadCaptLib()
{
	auto Path = fmt::format("{}\\Capt.dll", m_Hook->GetModulePath(m_Hook->GetSelfModuleHandle()));


	
	HMODULE	hModule = LoadLibraryA(Path.data());

	if (hModule)
	{
		(FARPROC&) CaptInit = m_Hook->GetProcAddress(hModule, "C_Init");
		(FARPROC&) CaptPath = m_Hook->GetProcAddress(hModule, "C_ASR");
		CaptInit();
	}

	ACEDebugFileLog("[{}]ModulePath:{} CaptModule:0x{:X} ", __FUNCTION__, Path,(DWORD)hModule);
}

void CSOWrapper::InstallGameUiHook()
{
	HMODULE lib = g_Offset.GetGameUiModuleInfo()->hModule;


	//获取LoginPopup
	for (ULONG Offset = 0; m_Memory->GetScanCodeAddress("6A 01 68 ?? ?? ?? ?? FF 75 08 E8 ?? ?? ?? ?? 8D B7 ?? ?? ?? ??", m_LoginDlgClass, lib, Offset); Offset++)
	{
		char* Text = (char*)*(PULONG)((ULONG)m_LoginDlgClass + 3);

		if (Text[0] == '\0')
			continue;

		if (lstrcmpA(Text, "LoginPopup") == 0)
		{
			m_LoginDlgClass += m_Hook->GetDisasmLenght(m_LoginDlgClass, 4);
			m_Hook->HookWithNakedEx(m_LoginDlgClass, 6, exVguiGetLoginClass);
			break;
		}

		/*
		*		搜索字符串  #CSO_CaptchaMsg LoginPopup Password
		* 6A 01                 - push 01
		  68 C027D433           - push GameUI.xmlXIncludeSetFlags+1A1260
		  FF 75 08              - push [ebp+08]
		  E8 05F81600           - call GameUI.xmlXIncludeSetFlags+1ACF0
		  8D B7 AC010000        - lea esi,[edi+000001AC]
		*/
	}
#if 0
	//登陆CALL .不直接搜了 用偏移
	for (ULONG Offset = 0, Count = 0; m_Memory->GetScanCodeAddress("55 8B EC 81 EC 04 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56", m_LoginCall, lib, Offset); Offset++, Count++)
	{

		//2021-07-14 14:53:42 第五个
		if (Count++ == 5)
		{
			break;
		}
		/*
				8B 3D 9CE6AE2F        - mov edi,[2FAEE69C] : [767B92B0]
				68 DC95B82F           - push 2FB895DC : ["Login"]
				56                    - push esi
				FF D7                 - call edi
				83 C4 08              - add esp,08

		*/
	}
#endif
	
	//二级仓库
	// 找到虚表指针就可以找到 this

	//	3D5578AA | 8B3D 0840D03D | mov edi, dword ptr ds : [0x3DD04008] |
	//	3D5578B0 | 85FF | test edi, edi |
	//	3D5578B2 | 0F85 21010000 | jne gameui.3D5579D9 |
	//	3D5578B8 | 68 E0010000 | push 0x1E0 |
	//	3D5578BD | E8 A6C34D00 | call gameui.3DA33C68 | //申请内存大小
	//	3D5578C2 | 8BF8 | mov edi, eax |
	//	3D5578C4 | 83C4 04 | add esp, 0x4 |
	//	3D5578C7 | 85FF | test edi, edi |
	//	3D5578C9 | 0F84 02010000 | je gameui.3D5579D1 |
	//	3D5578CF | 6A 01 | push 0x1 |
	//	3D5578D1 | 68 6C45AC3D | push gameui.3DAC456C | 3DAC456C:"2ndPasswordDlg"
	//	3D5578D6 | 6A 00 | push 0x0 |
	//	3D5578D8 | 8BCF | mov ecx, edi |
	//	3D5578DA | E8 B15C4600 | call gameui.3D9BD590 | // 初始化
	//	3D5578DF | 8DB7 AC010000 | lea esi, dword ptr ds : [edi + 0x1AC] | esi : &"Command"
	//	3D5578E5 | C707 0440AC3D | mov dword ptr ds : [edi] , gameui.3DAC4004 | //写入虚表
	//	3D5578EB | C706 7443AC3D | mov dword ptr ds : [esi] , gameui.3DAC4374 | [esi] : "Command"
	//	3D5578F1 | 803D 1840D03D 00 | cmp byte ptr ds : [0x3DD04018] , 0x0 |
	//	3D5578F8 | 75 30 | jne gameui.3D55792A |
	//	3D5578FA | 68 9843AC3D | push gameui.3DAC4398 | 3DAC4398 : "C2ndPasswordDlg"
	//	3D5578FF | C605 1840D03D 01 | mov byte ptr ds : [0x3DD04018] , 0x1 |
	//	3D557906 | E8 65CA4400 | call gameui.3D9A4370 |

	// 2022-05-15 17:31:49 现在的偏移 0x188

	if (m_Memory->GetScanCode("89 87 D0 01 00 00 8B 3D", m_2ndPasswordClass, lib))
	{
		m_2ndPasswordClass += m_Hook->GetDisasmLenght(m_2ndPasswordClass);
		m_2ndPasswordClass = *(PDWORD)(m_2ndPasswordClass + 2);

		/*
			第二个 6A 01 68 ?? ?? ?? ?? 6A 00 8B CF E8
			6A 01                 - push 01
			68 AC14FE32           - push 32FE14AC : ["2ndPasswordDlg"]
			6A 00                 - push 00
			8B CF                 - mov ecx,edi
			E8 41254100           - call 32EE6100
			8D B7 AC010000        - lea esi,[edi+000001AC]
		*/


	}

	ACEDebugFileLog("[{}] m_LoginDlgClass:0x{:X}", __FUNCTION__, m_LoginDlgClass);
	//ACEDebugFileLog("[{}] m_LoginCall:0x{:X}", __FUNCTION__, m_LoginCall);
	ACEDebugFileLog("[{}] m_2ndPasswordClass:0x{:X}", __FUNCTION__, m_2ndPasswordClass);
}

