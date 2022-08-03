#pragma once
#include "GameClienBase.hpp"
#include "Util.h"
#include "GameOffset.h"
#include "CSOWrapper.h"
#include "GamePack.h"
#include "DispatchRecv.h"
class GameClient :public IGameClient
{
public:
	GameClient();
	static GameClient* GetInstance();
	void  HookClientFunction();
	void  HookStudioFunction();
	static LRESULT WINAPI GameHookCallBack(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static int  Initialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	static void HUD_Frame_Init(double time);
	static void HUD_Frame(double time);
	static void RestoreRenderer();
public:
	virtual int  vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	virtual bool vInit();
	virtual void vHUD_Frame(double time);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);




private:
	static inline GameClient*        m_pInstance = nullptr;
	static inline std::mutex         m_Mutex;
	std::vector<IGameClient*>        m_VecClientFun;
	static inline HHOOK	             m_GameHook        = nullptr;
	GLdouble		                 m_modelview[16]; 
	GLdouble		                 m_projview[16];
	GLint			                 m_viewport[4];
	CSOWrapper*                      m_pCSOWrapper;
	CGamePack*						 m_pGamePack;
	CDispatchRecv*					 m_pDispatchRecv;
};

