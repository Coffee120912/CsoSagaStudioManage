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
	void(*IN_MouseRawInput) (int mstate);			    //2021-03-12 11:07:02  ������ӵ�
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
	ȫ����Ϸ������
*/
enum GameServer
{
	Chinese,		                                    // * ����
	TaiWan,			                                    // * ̨��
	Steam,			                                    // * CSNS
	Kor				                                    // * ����
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
	int     money;		                                //�۸�
	int     un_int_3;		                            //�������ƣ��� 7�� ͨ��
	int     un_int_4;
	int     type;		                                //��������
	float	un_float_1;		                            //һ�㶼Ϊ-1
	float	un_float_2;

}TAG_GAME_ITEMS, * PTAG_GAME_ITEMS;

enum Game_ClassType
{
	ClassType_Unknown,
	ClassType_Equipment,			                    //װ��
	ClassType_Class,				                    //��ɫ
	ClassType_PeriodicItem,			                    //����
	ClassType_Tattoo,				                    //����
	ClassType_zbRespawnEffect,		                    //��������Ч��
	ClassType_WeaponPaintItem,		                    //��������
	

};
typedef struct
{
	char	RecourceName[MAX_PATH];				        //��Դ��
	char	ClassName[MAX_PATH];				        //����
	ULONG	Category;							        //�����ID
	ULONG	InGameID;							        //����Ϸ���ID
	ULONG	SortingIndex;						        //����ID
	Game_ClassType RecourceType;				        //��Դ����
	WCHAR	ItemSrc[MAX_PATH];							//itemName
}GAME_ITEMMAP, * PGAME_ITEMMAP;

typedef struct
{
	int map_id;					                        //��ͼID
	std::string map_name;			                    //��ͼ��
	std::string map_code;			                    //��ͼ����
	std::wstring map_translation;                       //����ĵ�ͼ��
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
	//����ģʽ
	CSO_GameMode_Original,
	//���˾�սģʽ
	CSO_GameMode_DeathMatch,
	//�ŶӾ�սģʽ
	CSO_GameMode_TeamDeathMatch,
	//����ģʽ (����)
	CSO_GameMode_Bot,
	//,���˾�ս (����)
	CSO_GameMode_BotDM,
	//�ŶӾ�ս (����
	CSO_GameMode_BotTDM,
	//���ʴ���ģʽ
	CSO_GameMode_Official,
	//���ʴ���ģʽ��ʱ��
	CSO_GameMode_Official_TieBreak,
	//����ģʽ��Ⱦ
	CSO_GameMode_Zombi,
	//����ģʽ���
	CSO_GameMode_Zombi_Expand,
	//������ս-��ɾ��
	CSO_GameMode_Zombi_Team,
	CSO_GameMode_NULL,
	//����ģʽ
	CSO_GameMode_Challenge,
	//������ս(����)-��ɾ��
	CSO_GameMode_Zombi_Team_Ann,
	//����ģʽӢ��
	CSO_GameMode_Zombi_3,
	//���ֱ�
	CSO_GameMode_Zombi_Survival,
	//����ģʽ
	CSO_GameMode_Soccer,
	//���׷籩
	CSO_GameMode_Human_Scenario,
	// �Ͻ�ս��ģʽ-��ɾ��
	CSO_GameMode_PVPVE,
	//����ս
	CSO_GameMode_TDM_Item,
	//��������ɱ
	CSO_GameMode_Zombi_Escape,
	// �����ģʽ-��ɾ��
	CSO_GameMode_Bazooka,
	//ǹ����սģʽ
	CSO_GameMode_GunDeath,
	//���о���ģʽ
	CSO_GameMode_LightOri,
	//����Σ��
	CSO_GameMode_BotZombie,
	//�ݵ�ͻ��ģʽ-��ɾ��
	CSO_GameMode_BattleRush,
	//��ҹ����ģʽ-��ɾ��
	CSO_GameMode_ZombieShelter,
	//�������ģʽ
	CSO_GameMode_Snowman,
	//��ҹ����II
	CSO_GameMode_ZombieShelterT,
	//����4
	CSO_GameMode_Zombi_4,
	//���׶���ģʽ
	CSO_GameMode_ZombieGiant,
	//�Կ�ģʽ-��ɾ��
	CSO_GameMode_FightYard,
	//��������ս
	CSO_GameMode_Zombi_Exterminate,
	//��������
	CSO_GameMode_PVE,
	//�����Ҷ�-��ɾ��
	CSO_GameMode_SuddenDeath,
	//��������ս����ģʽ
	CSO_GameMode_ZombieOfficial,
	//��������ս��ʱ��
	CSO_GameMode_ZombieOfficial_TieBreak,
	//���ʬ��
	CSO_GameMode_ZombieTag,
	//������-����ģʽ
	CSO_GameMode_VoxelCreate,
	//������-��Ϸģʽ
	CSO_GameMode_VoxelPVE,
	//��սȫ����
	CSO_GameMode_Allstar,
	//��װչʾ��
	CSO_GameMode_PlayRoom,
	//��λ��������ģʽ
	CSO_GameMode_SeasonOriginal,
	//��λ������������ս
	CSO_GameMode_SeasonZombieEx,
	//��λ��������ģʽ(Ӣ��)
	CSO_GameMode_SeasonZombi_3,
	//����ģʽZ(����)
	CSO_GameMode_Zombi_3z,
	//��������ģʽ
	CSO_GameMode_ZombieTouchDown,
	//��λ������������ģʽ
	CSO_GameMode_SeasonZombieTouchDown,
	//����չʾ����
	CSO_GameMode_PlayRoom2,
	//׽�Բ�ģʽ
	CSO_GameMode_VoxelPropHunt,
	//��Ӷ��ģʽ
	CSO_GameMode_partnermode,
	//�����Գ�
	CSO_GameMode_ZHE,
	//��ҹ������
	CSO_GameMode_Vxlzshelter,
	CSO_GameMode_NULL2,
	//����5-����S
	CSO_GameMode_Zombi_5,
	CSO_GameMode_PlayRoom3,								//չʾ�� CSO_GameMode_PlayRoom3
	CSO_GameMode_Lab,


};
struct GameMpAddres
{
	DWORD	Map;			                            //���ػ�ͼ��ַ
	DWORD	Hp;				                            //Ѫ��
	DWORD	HpArmor;		                            //����
	DWORD	dwBase;			                            //��ַ
	DWORD	dwMpBase;		                            //���ػ�ַ
	DWORD	dwGod;			                            //�޵�
	DWORD	dwFly;			                            //����
	DWORD	dwGSpeed;		                            //����  ���390
	DWORD	dwIsMpRoom;		                            //�Ƿ��ڱ��ط���
	DWORD	dwReleaseHold;	                            //���ˢǹ����
	DWORD	dwRecoil;		                            //��������
	DWORD   dwhookDamage;
	DWORD   dwAlreadyOwnWeapon;                         //����ͬһ����
	DWORD   dwBuyThis;		                            //�޷�����˵���
	DWORD   dwBuyThisCallCheck;	                        //�޷�����˵����ϲ�Call ���滹�м��
	DWORD   dwBuyThisJmpAddres;	                        //�޷�����˵�����һ��CALL ����ת��ַ
	DWORD   dwCheckBuyTimer;	                        //��鹺��ʱ��
	DWORD   dwMpRoomCheck;	                            //���ط�����
	DWORD   dwEnoughMoney;	                            //�ʽ�����
	DWORD   dwCannotBeDropped;                          //�޷������������
	DWORD   dwHm_skill;		                            //��������
	DWORD   dwBuyKnife;		                            //��������
	DWORD   dwReciprocalTimerCheck;		                //������ ����ǹ֧����

	DWORD   dwHm_skill_Z;				                //��������_����Z
	DWORD   dwCantHeadShotUsed;			                //��������Ƿ����
	DWORD   dwHeadShot_Offset;			                //�������ƫ��
	DWORD   dwHm_skillDuration_Offset;                  //�����������ʱ��ƫ��
	DWORD   dwHm_skillDuration_Hook;                    //�������Hook������ַ

	DWORD   PVEDamageCall;	                            //PVE (�ֱ�)
	DWORD   PVPDamageCall;	                            //PVP (�Ŷ�)
	DWORD   enginefuncs_s;
	DWORD   dwKnifeOffset;
	DWORD   dwPlayerRoleOffset;

	DWORD   dwPlayerRpnOffset;		                    //���︴��_��ѡ����
	DWORD   dwPlayerRoleDetection;	                    //�����ɫ���

	DWORD   dwReleaseHoldEx;	                        //������� 2019��11��6�� 22:51:32 ���º������



														//----------------------�������ص���Ϣ-----------

	DWORD dwPlayer;				                        //�����ص���Ϣ			
	DWORD dwPlayerOffSet;		                        //ƫ��

	DWORD dwNextPlayerOffSet;	                        //���֮���ƫ��

	DWORD dwCamp;				                        //��Ӫ
	DWORD dwCampOffSet;			                        //��Ӫƫ��

	DWORD dwMoney;				                        //��Ǯ
	DWORD dwMoneyOffSet;		                        //��Ǯƫ��

	DWORD dwSubWeapon;			                        //������
	DWORD dwSubWeaponOffSet;	                        //������ƫ��

	DWORD dwMainWeapon;			                        //������
	DWORD dwMainWeaponOffSet;	                        //������ƫ��

	DWORD dwWeaponBulletOffSet;                         //�����ӵ�
	DWORD dwWeaponRecoilOffSet;                         //�����ĺ�����
	DWORD dwWeaponSpeedOffSet;                          //�����ļ���

	DWORD dwAttackCall;
	DWORD dwAttack2Call;
};





struct Gamefunctions
{
	DWORD	ItemIdToItemCode;			                //ID->����
	DWORD	ItemCodeToItemName;			                //����->����
	DWORD	DispatchEntry;				                //��
	DWORD	GetItemEntryCall;
};
struct GameAddres
{
	DWORD dwCampPtr;	                                //��Ӫ   CT 2 T1
	DWORD dwAngletr;	                                //�ӽ�   ���� 89 Y 
	DWORD dwCameraFOV;                                  //FOV    90
	DWORD dwMyBone;		                                //�Լ�����
	DWORD dwValid;		                                //����
	DWORD dwIsGame;		                                //�Ƿ�����Ϸ��
	DWORD dwDormant;	                                //����
	DWORD dwMixRoom;	                                //�췿
	DWORD dwRetRoom;	                                //���ط���
	DWORD dwNightVision;                                //ҹ��
	DWORD dwShowHP;		                                //��Ѫ
	DWORD dwShowHP2;	                                //�ٷֱ���Ѫ
	DWORD dwThirdPerson;		                        //�����ӽ�
	DWORD dwMsg;		                                //��Ϣ����
	DWORD dwBeamPoints;
	DWORD dwPreSCall;
	DWORD dwBallistic;		                            //����Ч��

	DWORD dwGuiMenu;		                            //�˵�
	DWORD dwNoKeyInput;		                            //�һ�
	DWORD dwGameLogin;		                            //��Ϸ��¼Call	���ٸ���
	DWORD dwPacketCount;	                            //�����ַ
	DWORD dwRoomMode;		                            //����ģʽ
	DWORD dwCheckOpengl;	                            //���Opengl���
	DWORD dwCheckProcess;	                            //������
	DWORD dwOutline;		                            //���͸��
	DWORD dwMapBrightness;	                            //��ͼ����
	DWORD dwCommandHook;	                            //ָ��HOOK
	DWORD dwItemPoint;		                            //��Ʒָ��
	DWORD dwItemEntry;		                            //��Ҫ����ItemCount
	DWORD dwItemCount;		                            //��Ҫ���ҡ� ��������
	DWORD dwbadWord;		                            //����໰
	DWORD dwbadWordJmp;		                            //����໰��ת��ַ
	DWORD dwAFK;			                            //�һ�ʱ���ж�
	DWORD dwCallEngineSurfaceProc;
	DWORD dwPlayerWeaponInfo;
	DWORD dwVoxlThisPoint;	                            //��Ҫ���û����жϵ����ߵķ���
	DWORD dwPlayerSequence;
	DWORD dwRandCallRetAddres;							//�������.���ص�ַ
	DWORD dwPlayerMgrAddres;
	DWORD dwPlayerMgrCall;								//ͨ��������ȡ������� ����Ҫ��CALL
	DWORD dwGetRoundCall;		                        //��ȡ�ֱ�غ���CALL
	DWORD dwAdSpeed;
	DWORD dwGameMesge;			                        //���ӳ�ʱ ��Ϸ֪ͨ
	DWORD dwGameVguiIpunt;			                    //�����̼�� 
	DWORD dwGameVguiMouseIpunt;		                    //����� 
	DWORD dwGameVguiIpuntTime;		                    //���ʱ��

	DWORD dwNightVisionColor;		                    //ҹ����ɫ
	DWORD dwCheckEngineAddres;							//��Ϸ��������ַ

	DWORD dwSpeed[10];									//���ٵ�ַ
	DWORD PreS_DynamicSoundCall;						//����CALL
	DWORD dwIsHomeowners;								//�Ƿ��Ƿ���

};

enum class PASSWORD_2ND_TYPE
{
	PasswordNumber = 0,		//��������
	Check ,
	DoubleCheck,			//�ڶ��μ��
	DoubleCheck_OK,			//ok
};