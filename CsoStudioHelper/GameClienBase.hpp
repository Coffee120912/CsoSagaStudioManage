#pragma once
#include "SagaConnect.h"

class IGameClient
{
public:

	//初始化,更早一点
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
	{
		return 1;
	}

	// 初始化
	virtual bool vInit()
	{
		return true;
	}

	virtual void vHUD_Frame(double time)
	{
		return;
	}

	//@窗口回调
	virtual int  vGameUI_WndProc(HWND hwnd, int nCode, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	//键盘钩子
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	//创建移动
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
	{
		return;
	}
	//计算
	virtual void vV_CalcRefdef(ref_params_s* pparams)
	{
		return;
	}
	//添加实体
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
	{
		return;
	}
	//重绘
	virtual void vHUD_Redraw(float time, int intermission)
	{
		return;
	}
	//玩家移动
	virtual void vHUD_PlayerMove(struct playermove_s* ppmove, int server)
	{
		return;
	}
	//游戏内按键
	virtual int  vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
	{
		return 1;
	}
	//玩家预测
	virtual void vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
	{
		return;
	}
	//透视Hook
	virtual int vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
	{
		return 1;
	}


};
