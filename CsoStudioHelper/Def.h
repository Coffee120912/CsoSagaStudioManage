#pragma once
#include "SagaConnect.h"

typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t* pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t* pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s* ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s* ppmove);
	char(*HUD_PlayerMoveTexture) (char* name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_MouseRawInput) (int mstate);			    //2021-03-12 11:07:02  更新添加的
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s* cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float* ofs);
	struct kbutton_s* (*KB_Find) (const char* name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s* pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s* ent, const char* modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s* event, const struct cl_entity_s* entity);
	void(*HUD_PostRunCmd) (struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s* state, const struct clientdata_s* client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s* dst, const struct entity_state_s* src);
	void(*HUD_TxferPredictionData) (struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char* buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float* mins, float* maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char* pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
	struct cl_entity_s* (*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int* x, int* y);
	void(*CLIENTFACTORY) (void);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
} cl_clientfunc_t;
typedef struct UserMsg
{
	int	number;
	int	size;
	char name[16];
	struct UserMsg* next;
	pfnUserMsgHook pfn;
} *PUserMsg;

enum GameServerRegion
{
	D1,
	D2,
	D3,
	W1,
	W2
};
/*
	全服游戏服务器
*/
enum GameServer
{
	Chinese,		                                    // * 国服
	TaiWan,			                                    // * 台服
	Steam,			                                    // * CSNS
	Kor				                                    // * 韩服
};

typedef struct cvar7_s
{
	std::string	name;
	std::string	string;
	int		flags;
	float	value;
	DWORD	hotkey;
	struct cvar7_s* next;
} cvar7_s;

typedef struct
{
	int		count;
	char* Ponint_un_name_1[1];
	char* Ponint_un_name_2[1];
	char* Ponint_item_code[1];
	char* Ponint_item_name[1];
	int     un_int_1;
	int     money;		                                //价格
	int     un_int_3;		                            //武器限制？？ 7是 通用
	int     un_int_4;
	int     type;		                                //武器类型
	float	un_float_1;		                            //一般都为-1
	float	un_float_2;

}TAG_GAME_ITEMS, * PTAG_GAME_ITEMS;

enum Game_ClassType
{
	ClassType_Unknown,
	ClassType_Equipment,			                    //装备
	ClassType_Class,				                    //角色
	ClassType_PeriodicItem,			                    //喷漆
	ClassType_Tattoo,				                    //纹身
	ClassType_zbRespawnEffect,		                    //生化复活效果
	ClassType_WeaponPaintItem,		                    //武器彩漆
	

};
typedef struct
{
	char	RecourceName[MAX_PATH];				        //资源名
	char	ClassName[MAX_PATH];				        //类名
	ULONG	Category;							        //分类的ID
	ULONG	InGameID;							        //在游戏里的ID
	ULONG	SortingIndex;						        //排序ID
	Game_ClassType RecourceType;				        //资源类型
	WCHAR	ItemSrc[MAX_PATH];							//itemName
}GAME_ITEMMAP, * PGAME_ITEMMAP;

typedef struct
{
	int map_id;					                        //地图ID
	std::string map_name;			                    //地图名
	std::string map_code;			                    //地图代码
	std::wstring map_translation;                       //翻译的地图名
}GAME_MAPINFO, * PGAME_MAPINFO;

enum TEAM
{
	SPECTATOR = 0,
	TERRORIST = 1,
	CT = 2,
};

typedef struct
{
	double v1;
	double mouse_timer;
	double keyboard_timer;
}TAG_VGUI_INPUT, * PTAG_VGUI_INPUT;

enum Game_Mode
{
	//竞技模式
	CSO_GameMode_Original,
	//个人决战模式
	CSO_GameMode_DeathMatch,
	//团队决战模式
	CSO_GameMode_TeamDeathMatch,
	//竞技模式 (虚拟)
	CSO_GameMode_Bot,
	//,个人决战 (虚拟)
	CSO_GameMode_BotDM,
	//团队决战 (虚拟
	CSO_GameMode_BotTDM,
	//国际大赛模式
	CSO_GameMode_Official,
	//国际大赛模式加时赛
	CSO_GameMode_Official_TieBreak,
	//生化模式感染
	CSO_GameMode_Zombi,
	//生化模式异变
	CSO_GameMode_Zombi_Expand,
	//生化盟战-已删除
	CSO_GameMode_Zombi_Team,
	CSO_GameMode_NULL,
	//极限模式
	CSO_GameMode_Challenge,
	//生化盟战(歼灭)-已删除
	CSO_GameMode_Zombi_Team_Ann,
	//生化模式英雄
	CSO_GameMode_Zombi_3,
	//大灾变
	CSO_GameMode_Zombi_Survival,
	//足球模式
	CSO_GameMode_Soccer,
	//机甲风暴
	CSO_GameMode_Human_Scenario,
	// 合金战争模式-已删除
	CSO_GameMode_PVPVE,
	//道具战
	CSO_GameMode_TDM_Item,
	//生化大逃杀
	CSO_GameMode_Zombi_Escape,
	// 火箭炮模式-已删除
	CSO_GameMode_Bazooka,
	//枪王决战模式
	CSO_GameMode_GunDeath,
	//休闲竞技模式
	CSO_GameMode_LightOri,
	//生化危机
	CSO_GameMode_BotZombie,
	//据点突破模式-已删除
	CSO_GameMode_BattleRush,
	//昼夜求生模式-已删除
	CSO_GameMode_ZombieShelter,
	//怪物变身模式
	CSO_GameMode_Snowman,
	//昼夜求生II
	CSO_GameMode_ZombieShelterT,
	//生化4
	CSO_GameMode_Zombi_4,
	//机甲斗兽模式
	CSO_GameMode_ZombieGiant,
	//对抗模式-已删除
	CSO_GameMode_FightYard,
	//生化歼灭战
	CSO_GameMode_Zombi_Exterminate,
	//绝命档案
	CSO_GameMode_PVE,
	//绝命乱斗-已删除
	CSO_GameMode_SuddenDeath,
	//生化歼灭战大赛模式
	CSO_GameMode_ZombieOfficial,
	//生化歼灭战加时赛
	CSO_GameMode_ZombieOfficial_TieBreak,
	//真假尸王
	CSO_GameMode_ZombieTag,
	//缔造者-创造模式
	CSO_GameMode_VoxelCreate,
	//缔造者-游戏模式
	CSO_GameMode_VoxelPVE,
	//征战全明星
	CSO_GameMode_Allstar,
	//武装展示厅
	CSO_GameMode_PlayRoom,
	//排位赛：竞技模式
	CSO_GameMode_SeasonOriginal,
	//排位赛：生化歼灭战
	CSO_GameMode_SeasonZombieEx,
	//排位赛：生化模式(英雄)
	CSO_GameMode_SeasonZombi_3,
	//生化模式Z(进化)
	CSO_GameMode_Zombi_3z,
	//生化达阵模式
	CSO_GameMode_ZombieTouchDown,
	//排位赛：生化达阵模式
	CSO_GameMode_SeasonZombieTouchDown,
	//武器展示厅Ⅱ
	CSO_GameMode_PlayRoom2,
	//捉迷藏模式
	CSO_GameMode_VoxelPropHunt,
	//雇佣兵模式
	CSO_GameMode_partnermode,
	//生化猎场
	CSO_GameMode_ZHE,
	//昼夜求生Ⅱ
	CSO_GameMode_Vxlzshelter,
	CSO_GameMode_NULL2,
	//生化5-生化S
	CSO_GameMode_Zombi_5,
	CSO_GameMode_PlayRoom3,								//展示厅 CSO_GameMode_PlayRoom3
	CSO_GameMode_Lab,


};
struct GameMpAddres
{
	DWORD	Map;			                            //本地换图地址
	DWORD	Hp;				                            //血量
	DWORD	HpArmor;		                            //护甲
	DWORD	dwBase;			                            //基址
	DWORD	dwMpBase;		                            //本地基址
	DWORD	dwGod;			                            //无敌
	DWORD	dwFly;			                            //飞天
	DWORD	dwGSpeed;		                            //地速  最高390
	DWORD	dwIsMpRoom;		                            //是否在本地房间
	DWORD	dwReleaseHold;	                            //解除刷枪限制
	DWORD	dwRecoil;		                            //反后坐力
	DWORD   dwhookDamage;
	DWORD   dwAlreadyOwnWeapon;                         //限制同一武器
	DWORD   dwBuyThis;		                            //无法购买此道具
	DWORD   dwBuyThisCallCheck;	                        //无法购买此道具上层Call 下面还有检测
	DWORD   dwBuyThisJmpAddres;	                        //无法购买此道具上一层CALL 的跳转地址
	DWORD   dwCheckBuyTimer;	                        //检查购买时间
	DWORD   dwMpRoomCheck;	                            //本地房间检查
	DWORD   dwEnoughMoney;	                            //资金不足检查
	DWORD   dwCannotBeDropped;                          //无法丢弃武器检查
	DWORD   dwHm_skill;		                            //技能限制
	DWORD   dwBuyKnife;		                            //购买刀限制
	DWORD   dwReciprocalTimerCheck;		                //倒数后 购买枪支限制

	DWORD   dwHm_skill_Z;				                //技能限制_生化Z
	DWORD   dwCantHeadShotUsed;			                //致命打击是否可用
	DWORD   dwHeadShot_Offset;			                //致命打击偏移
	DWORD   dwHm_skillDuration_Offset;                  //致命打击持续时间偏移
	DWORD   dwHm_skillDuration_Hook;                    //致命打击Hook持续地址

	DWORD   PVEDamageCall;	                            //PVE (灾变)
	DWORD   PVPDamageCall;	                            //PVP (团队)
	DWORD   enginefuncs_s;
	DWORD   dwKnifeOffset;
	DWORD   dwPlayerRoleOffset;

	DWORD   dwPlayerRpnOffset;		                    //人物复活_重选人物
	DWORD   dwPlayerRoleDetection;	                    //人物角色检查

	DWORD   dwReleaseHoldEx;	                        //解除限制 2019年11月6日 22:51:32 更新后用这个



														//----------------------与玩家相关的信息-----------

	DWORD dwPlayer;				                        //玩家相关的信息			
	DWORD dwPlayerOffSet;		                        //偏移

	DWORD dwNextPlayerOffSet;	                        //玩家之间的偏移

	DWORD dwCamp;				                        //阵营
	DWORD dwCampOffSet;			                        //阵营偏移

	DWORD dwMoney;				                        //金钱
	DWORD dwMoneyOffSet;		                        //金钱偏移

	DWORD dwSubWeapon;			                        //副武器
	DWORD dwSubWeaponOffSet;	                        //副武器偏移

	DWORD dwMainWeapon;			                        //主武器
	DWORD dwMainWeaponOffSet;	                        //主武器偏移

	DWORD dwWeaponBulletOffSet;                         //武器子弹
	DWORD dwWeaponRecoilOffSet;                         //武器的后坐力
	DWORD dwWeaponSpeedOffSet;                          //武器的加速

	DWORD dwAttackCall;
	DWORD dwAttack2Call;
};





struct Gamefunctions
{
	DWORD	ItemIdToItemCode;			                //ID->代码
	DWORD	ItemCodeToItemName;			                //代码->名称
	DWORD	DispatchEntry;				                //类
	DWORD	GetItemEntryCall;
};
struct GameAddres
{
	DWORD dwCampPtr;	                                //阵营   CT 2 T1
	DWORD dwAngletr;	                                //视角   上下 89 Y 
	DWORD dwCameraFOV;                                  //FOV    90
	DWORD dwMyBone;		                                //自己骨骼
	DWORD dwValid;		                                //死活
	DWORD dwIsGame;		                                //是否在游戏中
	DWORD dwDormant;	                                //休眠
	DWORD dwMixRoom;	                                //混房
	DWORD dwRetRoom;	                                //返回房间
	DWORD dwNightVision;                                //夜视
	DWORD dwShowHP;		                                //显血
	DWORD dwShowHP2;	                                //百分比显血
	DWORD dwThirdPerson;		                        //第三视角
	DWORD dwMsg;		                                //消息弹窗
	DWORD dwBeamPoints;
	DWORD dwPreSCall;
	DWORD dwBallistic;		                            //暴走效果

	DWORD dwGuiMenu;		                            //菜单
	DWORD dwNoKeyInput;		                            //挂机
	DWORD dwGameLogin;		                            //游戏登录Call	不再更新
	DWORD dwPacketCount;	                            //封包地址
	DWORD dwRoomMode;		                            //房间模式
	DWORD dwCheckOpengl;	                            //检查Opengl检测
	DWORD dwCheckProcess;	                            //检查进程
	DWORD dwOutline;		                            //描边透视
	DWORD dwMapBrightness;	                            //地图高亮
	DWORD dwCommandHook;	                            //指令HOOK
	DWORD dwItemPoint;		                            //物品指针
	DWORD dwItemEntry;		                            //主要查找ItemCount
	DWORD dwItemCount;		                            //主要查找↑ 不明所以
	DWORD dwbadWord;		                            //解除脏话
	DWORD dwbadWordJmp;		                            //解除脏话跳转地址
	DWORD dwAFK;			                            //挂机时间判断
	DWORD dwCallEngineSurfaceProc;
	DWORD dwPlayerWeaponInfo;
	DWORD dwVoxlThisPoint;	                            //主要作用还是判断缔造者的房间
	DWORD dwPlayerSequence;
	DWORD dwRandCallRetAddres;							//随机种子.返回地址
	DWORD dwPlayerMgrAddres;
	DWORD dwPlayerMgrCall;								//通过索引获取玩家名称 所需要的CALL
	DWORD dwGetRoundCall;		                        //获取灾变回合数CALL
	DWORD dwAdSpeed;
	DWORD dwGameMesge;			                        //连接超时 游戏通知
	DWORD dwGameVguiIpunt;			                    //鼠标键盘检测 
	DWORD dwGameVguiMouseIpunt;		                    //鼠标检测 
	DWORD dwGameVguiIpuntTime;		                    //输出时间

	DWORD dwNightVisionColor;		                    //夜视颜色
	DWORD dwCheckEngineAddres;							//游戏检查引擎地址

	DWORD dwSpeed[10];									//加速地址
	DWORD PreS_DynamicSoundCall;						//声音CALL
	DWORD dwIsHomeowners;								//是否是房主

};

enum class PASSWORD_2ND_TYPE
{
	PasswordNumber = 0,		//输入密码
	Check ,
	DoubleCheck,			//第二次检查
	DoubleCheck_OK,			//ok
};