#include "GameClient.h"
#include "EngineMsg.h"

GameClient::GameClient()
{
	m_pCSOWrapper   = CSOWrapper::GetInstance();
	m_pGamePack     = CGamePack::GetInstance();
	m_pDispatchRecv = CDispatchRecv::GetInstance();

	m_VecClientFun.push_back(m_pCSOWrapper);
	m_VecClientFun.push_back(m_pGamePack);
	m_VecClientFun.push_back(m_pDispatchRecv);
}

GameClient* GameClient::GetInstance()
{
	m_Mutex.lock();
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameClient();
	}
	m_Mutex.unlock();

	return m_pInstance;
}

void GameClient::HookClientFunction()
{
	while (Engine::g_pClient->HUD_PostRunCmd == nullptr)
	{
		Sleep(200);
	}

	//替换函数指针
	Engine::g_pClient->Initialize       = Initialize;
	//Engine::g_pClient->HUD_PostRunCmd   = HUD_PostRunCmd;
	Engine::g_pClient->HUD_Frame        = HUD_Frame_Init;
	//Engine::g_pClient->HUD_Redraw       = HUD_Redraw;
	//Engine::g_pClient->HUD_Key_Event    = HUD_Key_Event;
	//Engine::g_pClient->HUD_AddEntity    = HUD_AddEntity;
	//Engine::g_pClient->HUD_PlayerMove   = HUD_PlayerMove;
	//Engine::g_pClient->CL_IsThirdPerson = CL_IsThirdPerson;
	//Engine::g_pClient->CL_CreateMove    = CL_CreateMove;
	//Engine::g_pClient->V_CalcRefdef     = V_CalcRefdef;

}


void GameClient::HookStudioFunction()
{
	g_Offset.FindStudioTable();

	//第二种获取骨骼的方法
	Engine::g_pStudio->RestoreRenderer = RestoreRenderer;

	//屏幕大小
	Engine::g_Screen.iSize = sizeof(SCREENINFO);

	//初始化字体	
	//g_Font.InitText(xorstr_("Lucida Console"), 12, 8);
	//g_Verdana.InitText(xorstr_("Verdana"), 13, 0);

	g_Offset.HUD_Frame();

	g_UserMsg.HookEngineMsg();
}

int GameClient::Initialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	return GameClient::GetInstance()->vInitialize(pEnginefuncs, iVersion);
}

void GameClient::HUD_Frame_Init(double time)
{
	GameClient::GetInstance()->vInit();
}

void GameClient::HUD_Frame(double time)
{
	GameClient::GetInstance()->vHUD_Frame(time);
}

void GameClient::RestoreRenderer()
{
	// Hook这个可以在有效范围内 都能获取到.其他函数必须在FOV内才可以
	Engine::g_Studio.RestoreRenderer();

	//GameClient::GetInstance()->m_pGamePlayer->StudioEntityLight();
	//获取矩阵
	glGetDoublev(GL_MODELVIEW_MATRIX, GameClient::GetInstance()->m_modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, GameClient::GetInstance()->m_projview);
	glGetIntegerv(GL_VIEWPORT, GameClient::GetInstance()->m_viewport);
}

int GameClient::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	Engine::g_pEngine = (cl_enginefunc_t*)pEnginefuncs;

	RtlCopyMemory(&Engine::g_Engine, pEnginefuncs, sizeof(cl_enginefunc_t));

	for (auto itr : m_VecClientFun)
	{
		itr->vInitialize(pEnginefuncs, iVersion);
	}
	return  Engine::g_Client.Initialize(pEnginefuncs, iVersion);;
}

bool GameClient::vInit()
{
	//初始化 HookStudioFunction
	GameClient::GetInstance()->HookStudioFunction();

	m_GameHook = SetWindowsHookExA(WH_KEYBOARD, GameHookCallBack, NULL, CACEInterface::GetInstance()->GetProcess()->GetMainThreadId());

	g_pUtil->InitGameItemInfo();
	g_pUtil->InitinWeaponItem();


	for (auto itr : m_VecClientFun)
	{
		itr->vInit();
	}
	Engine::g_pClient->HUD_Frame = HUD_Frame;
	return true;
}
LRESULT WINAPI GameClient::GameHookCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (((DWORD)lParam & 0x40000000) && (HC_ACTION == nCode)) //有键按下
	{

		GameClient::GetInstance()->vKeyBoardProc(nCode, wParam, lParam);

	}
	return ::CallNextHookEx(GameClient::GetInstance()->m_GameHook, nCode, wParam, lParam);
}
void GameClient::vHUD_Frame(double time)
{
	memset((PVOID)g_Offset.GetGameAddresInfo()->dwCheckEngineAddres, 0xFFFFFFFF, sizeof(DWORD));

	Engine::g_Engine.pfnGetScreenInfo(&Engine::g_Screen);

	for (auto itr : m_VecClientFun)
	{
		itr->vHUD_Frame(time);
	}

	Engine::g_Client.HUD_Frame(time);
}

int GameClient::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	for (auto itr : m_VecClientFun)
	{
		itr->vKeyBoardProc(nCode, wParam, lParam);
	}
	return nCode;
}

