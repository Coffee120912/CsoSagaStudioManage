#pragma once
#include <CACEInterface.h>
#include <thread>
#include "GameSDK.h"
#include "Def.h"
#include <mutex>

#include <PackWrite.h>
#include <PackReader.h>

#include "../Connect/ShareDef.h"

namespace Engine
{
	extern cl_clientfunc_t* g_pClient;
	extern cl_clientfunc_t g_Client;
	extern cl_enginefunc_t* g_pEngine;
	extern cl_enginefunc_t g_Engine;
	extern engine_studio_api_t* g_pStudio;
	extern engine_studio_api_t g_Studio;
	extern playermove_t* g_pPlayerMove;
	extern PUserMsg g_pUserMsgBase;
	extern SCREENINFO g_Screen;
	extern screenfade_t* g_pScreenFade;
	extern enginefuncs_t* g_pEngfuncs;
	extern enginefuncs_t  g_Engfuncs;
}

#if ANTI_DEBUG
#define ODG_EXPORT			EXPORT		//µ÷ÊÔÓÃµÄ

#else

#define ODG_EXPORT			

#endif

extern std::array<TAG_ACE_HOOK_MODULE_INFO, 10> AppList;



