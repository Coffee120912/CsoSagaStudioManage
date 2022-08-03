#pragma once
#include "SagaConnect.h"

enum class UMsgType {

	Chat        = 0,		//˽��
	Notice		= 10,		//����	_������ʾ��+��Ϸ���м���ɫ��ʾ
	SysNotice   = 11,		//ϵͳ����	_�ٻ�ɫ��������Ϸ���½� ��ʾ
	MsgStr      = 30,		//��ʾ�ı���Ϣ
	LIKE        = 31,		//����
	AdminNotice = 51,		//����Ա����
	ItemBoard	= 60,		//������
	NoticeBoard = 65,		//������
	Lottery     = 80,		//�����н���Ϣ

};

enum class KickType {

	Error    = 0,			//������Ϣ
	ReMain   = 1,			//ʣ��ͶƱ
	KickInfo = 2,			//�ٱ���Ϣ
	KickRoom = 3,			//���߳�����
	KickInit = 4,			//��ʼ��

};

enum class UpRoomType {

	RoomInfo        = 0,			//������Ϣ
	RoomPlayerCount = 4,	       //�޸ķ�������
	RoomGameEnd     = 6,           //��Ϸ����
	RoomKick        = 7,           //��������
	RoomKickExp     = 0x11,        //���߽��㾭��
	RoomWeaponRate  = 0x23,        //�������������

};


typedef struct GAME_PACKET_HEAD
{
	BYTE	Head;		//ͷ�� �̶� 0x55
	BYTE	Seq;		//������� 0-255->0
	SHORT	Lenght;		//������� (һ��������� -������ͷ���ĸ��ֽ�)

	inline GAME_PACKET_HEAD()
	{
		RtlZeroMemory(this, sizeof(GAME_PACKET_HEAD));
		this->Head = 0x55;
	}
}TAG_GAME_PACKET_HEAD, * PTAG_GAME_PACKET_HEAD;






typedef struct _MEMORY_MAPPED_FILE_NAME_INFORMATION
{
	UNICODE_STRING Name;
	WCHAR Buffer[1024];
} MEMORY_MAPPED_FILE_NAME_INFORMATION, * PMEMORY_MAPPED_FILE_NAME_INFORMATION;
#define		EVP_MAX_IV_LENGTH   16
#define 	EVP_MAX_BLOCK_LENGTH   32
struct evp_cipher_ctx_st {
	const void* cipher;
	void* engine;             /* functional reference if 'cipher' is
								 * ENGINE-provided */
	int encrypt;                /* encrypt or decrypt */
	int buf_len;                /* number we have left */
	unsigned char oiv[EVP_MAX_IV_LENGTH]; /* original iv */
	unsigned char iv[EVP_MAX_IV_LENGTH]; /* working iv */
	unsigned char buf[EVP_MAX_BLOCK_LENGTH]; /* saved partial block */
	int num;                    /* used by cfb/ofb/ctr mode */
	/* FIXME: Should this even exist? It appears unused */
	void* app_data;             /* application stuff */
	int key_len;                /* May change for variable length cipher */
	unsigned long flags;        /* Various flags */
	void* cipher_data;          /* per EVP data */
	int final_used;
	int block_mask;
	unsigned char final[EVP_MAX_BLOCK_LENGTH]; /* possible final block */
};
typedef int(*PfnSSL_read)(void* ssl, void* buf, int num);

typedef int(*PfnEVP_EncryptUpdate)(VOID* ctx, unsigned char* out, int* outl, unsigned char* in, int inl);
typedef int(*PfnEVP_DecryptUpdate)(VOID* ctx, unsigned char* out, int* outl, const unsigned char* in, int inl);
typedef int(*PfnEVP_EncryptInit)  (VOID* ctx, const VOID* type, unsigned char* key, unsigned char* iv);
typedef int(WSAAPI* PfnSend)  (SOCKET s, const char* buf, int len, int flags);

extern PfnEVP_DecryptUpdate EVP_DecryptUpdate007;
extern PfnEVP_EncryptUpdate EVP_EncryptUpdate007;
extern PfnEVP_EncryptInit	EVP_EncryptInit007;
extern PfnSend				_Send;
extern PfnSSL_read			SSL_read007;


enum Packet_Type
{
	Packet_Version              = 0,		                                                 //�汾��
	Packet_Reply                = 1,		                                                 //��
	Packet_Transfer             = 2,
	Packet_ServerList           = 5,
	Packet_Character            = 6,		                                                 //������������
	Packet_Crypt                = 7,
	Packet_MileageBingo         = 15,
	Packet_Statistico           = 16,										                 //ͳ��
	Packet_SessionID            = 17,		                                                 //�ỰID
	Packet_Mobile               = 18,		                                                 //�ƶ��绰��֤
	Packet_Policy               = 19,
	Packet_UpROOM               = 65,		                                                 //��Ϸ�����ڵĲ���->��������->��ʼ��Ϸ��
	Packet_ClientCheck          = 66,		                                                 //�ͻ��˼��
	Packet_UMsg                 = 67,		                                                 //��������Ϣ
	Packet_Host                 = 68,
	Packet_UpdateInfo           = 69,		                                                 //������Ϣ
	Packet_UDP                  = 70,
	Packet_Clan                 = 71,		                                                 //������Ϣ
	Packet_Shop                 = 72,		                                                 //�̵�
	Packet_Rank                 = 73,				                                         //2020��5��4�� 19:05:47  Rank
	Packet_Ban                  = 74,
	Packet_Option               = 76,		                                                 //�û�����(����ָ̨��֮���)  ������ 0 1 2 
	Packet_Favorite             = 77,
	Packet_Item                 = 78,
	Packet_SearchRoom           = 80,
	Packet_HostServer           = 81,
	Packet_Report               = 83,
	Packet_Title                = 84,                                                        //������ɺ���ʾ��.
	Packet_Buff                 = 85,
	Packet_QuickStart           = 86,
	Packet_UserSurvey           = 87,		                                                 //�û�����
	Packet_Quest                = 88,
	Packet_MiniGame             = 89,
	Packet_Hack                 = 90,
	Packet_Metadata             = 91,		                                                 //Ԫ���� ��ͼ�б� ��Ϸģʽ֮���
	Packet_PlayerInfo           = 93,		                                                 //�����б�->�鿴�����Ϣ
	Packet_Gift_Item            = 96,	                                                     //����
	Packet_2nd_Password         = 97,	                                                     //��������
	Packet_GameMatch            = 99,		                                                 //��Ϸ����
	Packet_UpdateWarehouse      = 100,		                                                 //ˢ�²ֿ���Ϣ
	Packet_CleanSystem          = 101,		                                                 //
	Packet_Ribbon               = 102,		                                                 //�ɾͼ�¼
	Packet_VoxelOutUI           = 103,
	Packet_WeaponAuctionEvent   = 104,		                                                 //���������
	Packet_Analysis             = 105,
	Packet_CoDisassemble        = 107,														 //�ֽ���껪
	Packet_MileageShop          = 108,
	Packet_Help                 = 109,
	Packet_PopularInfo          = 110,		                                                 //��ӭ��Ϣ
	Packet_Kick                 = 112,
	Packet_HonorShop            = 113,	                                                     //�����̵�
	Packet_EpicPieceShop        = 114,					                                     //ʷʫ�̵�.һ���������һ�����
	Packet_Addon                = 115,		                                                 //���
	Packet_QuestBadgeShop       = 116,		                                                 //�ʾ����
	Packet_ReverseAuctionSystem = 117,		                                                 //��ϵͳ-����
	Packet_SeasonSystem         = 118,		                                                 //����ϵͳ
	Packet_GuideQuest           = 120,		                                                 //����ָ��
	Packet_SantaAnim            = 122,		                                                 //ʥ������
	Packet_UserStart            = 150,		                                                 //�û���ʼ
	Packet_GameMatchRoomList    = 151,	                                                     //�����б�
	Packet_Inventory            = 152,		                                                 //�ֿⵥ
	Packet_Lobby                = 153,		                                                 //���� ��������֪ͨ_�ɻ�ȡ�����û��ʺź�����
	Packet_Inventory2           = 154,		                                                 //�����
	Packet_CafeItems            = 155,
	Packet_DefaultItems         = 156,
	Packet_UserUpdateInfo       = 157,		                                                 //�����û���Ϣ
	Packet_FabItems             = 158,		                                                 //�ղؼ�.ϲ����
	Packet_Event                = 159,
	Packet_CostumeInven         = 160,		                                                 //��װ.������װ��
	Packet_ZombieScenarioMaps   = 161,	                                                     //��ʬ������ͼ
	Packet_RotationWeaponInven  = 162,
	Packet_Alarm                = 164,				                                         //����
	Packet_MonthlyWeapon        = 165,			                                             //���������_�����ǰ�
	Packet_LifeWeaponInven      = 166,
	Packet_VipSystem            = 167,                                                       //VIPϵͳ
	Packet_FreePassWeaponInven  = 168,                                                       //�������
	Packet_ServerLog            = 169,	                                                     //��������־
	Packet_Dictionary           = 172,                                                       // �ֵ�	-ս���ٿ�
	Packet_TestPacket           = 173,
	Packet_ZBSGoldenZB          = 174,
	Packet_Friend               = 175,                                                       //����
	Packet_EventItemInve        = 176,
	Packet_Expedition           = 177,                                                       //Զ��
	Packet_ScenarioTX           = 178,
	Packet_League               = 180,			                                             //����
	Packet_ClassInven           = 181,			                                             //�����
	Packet_PartsInven           = 182,			                                             //���ּ�
	Packet_ItemInven            = 183,			                                             //��Ʒ�������
	Packet_SwitchConfig         = 184,

};
typedef struct GAME_PACKET_TYPE_INFO
{
	Packet_Type		Type;			                    //�������
	ULONG			Addres;			                    //��ת�ĵ�ַ
	ULONG			SvAddres;		                    //����ĵ�ַ
	BOOL			Hook;			                    //�Ƿ�Hook
	TAG_PEB_STACK	Stack;			                    //�����ջ��Ϣ
	std::string		name;

	inline GAME_PACKET_TYPE_INFO(Packet_Type Type, ULONG Addres, BOOL Hook, std::string name)
	{
		this->Type = Type;
		this->Addres = Addres;
		this->SvAddres = 0;
		this->Hook = Hook;
		this->Stack = TAG_PEB_STACK();
		this->name = name;
	}
	inline GAME_PACKET_TYPE_INFO(Packet_Type Type, ULONG Addres, ULONG SvAddres, BOOL Hook, TAG_PEB_STACK Stack, std::string name)
	{
		this->Type = Type;
		this->Addres = Addres;
		this->SvAddres = SvAddres;
		this->Hook = Hook;
		this->Stack = Stack;
		this->name = name;
	}

	inline GAME_PACKET_TYPE_INFO()
	{
		RtlZeroMemory(this, sizeof(GAME_PACKET_TYPE_INFO));
	}

}TAG_GAME_PACKET_TYPE_INFO, * PTAG_GAME_PACKET_TYPE_INFO;

typedef struct GAME_PACKET_TYPE_INFO_HOOK
{
	ULONG Class;	                                    //���뱣�����
	ULONG v1;
	ULONG v2;
	ULONG v3;											//mov edx, dword ptr ds:[eax+0x8]
	ULONG v4;
	ULONG v5;			//this
	ULONG Arry[1024];
	GAME_PACKET_TYPE_INFO_HOOK(ULONG Addres)
	{
		RtlZeroMemory(this, sizeof(GAME_PACKET_TYPE_INFO_HOOK));
		v3 = Addres;
	}
}TAG_GAME_PACKET_TYPE_INFO_HOOK, * PTAG_GAME_PACKET_TYPE_INFO_HOOK;

#pragma pack (1)




#pragma pack()