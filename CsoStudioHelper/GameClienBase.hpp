#pragma once
#include "SagaConnect.h"

class IGameClient
{
public:

	//��ʼ��,����һ��
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
	{
		return 1;
	}

	// ��ʼ��
	virtual bool vInit()
	{
		return true;
	}

	virtual void vHUD_Frame(double time)
	{
		return;
	}

	//@���ڻص�
	virtual int  vGameUI_WndProc(HWND hwnd, int nCode, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	//���̹���
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	//�����ƶ�
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
	{
		return;
	}
	//����
	virtual void vV_CalcRefdef(ref_params_s* pparams)
	{
		return;
	}
	//���ʵ��
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
	{
		return;
	}
	//�ػ�
	virtual void vHUD_Redraw(float time, int intermission)
	{
		return;
	}
	//����ƶ�
	virtual void vHUD_PlayerMove(struct playermove_s* ppmove, int server)
	{
		return;
	}
	//��Ϸ�ڰ���
	virtual int  vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
	{
		return 1;
	}
	//���Ԥ��
	virtual void vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
	{
		return;
	}
	//͸��Hook
	virtual int vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
	{
		return 1;
	}


};
