#pragma once
#include "SagaConnect.h"

enum class UMsgType {

	Chat        = 0,		//私聊
	Notice		= 10,		//公告	_大厅提示框+游戏里中间绿色提示
	SysNotice   = 11,		//系统公告	_橘黄色大厅与游戏左下角 提示
	MsgStr      = 30,		//提示文本消息
	LIKE        = 31,		//点赞
	AdminNotice = 51,		//管理员公告
	ItemBoard	= 60,		//道具栏
	NoticeBoard = 65,		//公告栏
	Lottery     = 80,		//公屏中奖信息

};

enum class KickType {

	Error    = 0,			//错误信息
	ReMain   = 1,			//剩余投票
	KickInfo = 2,			//举报信息
	KickRoom = 3,			//被踢出房间
	KickInit = 4,			//初始化

};

enum class UpRoomType {

	RoomInfo        = 0,			//房间信息
	RoomPlayerCount = 4,	       //修改房间人数
	RoomGameEnd     = 6,           //游戏结算
	RoomKick        = 7,           //房主踢人
	RoomKickExp     = 0x11,        //被踢结算经验
	RoomWeaponRate  = 0x23,        //结束后武器打分

};


typedef struct GAME_PACKET_HEAD
{
	BYTE	Head;		//头部 固定 0x55
	BYTE	Seq;		//封包序列 0-255->0
	SHORT	Lenght;		//封包长度 (一个封包长度 -不包括头部四个字节)

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
	Packet_Version              = 0,		                                                 //版本号
	Packet_Reply                = 1,		                                                 //答复
	Packet_Transfer             = 2,
	Packet_ServerList           = 5,
	Packet_Character            = 6,		                                                 //创建人物名称
	Packet_Crypt                = 7,
	Packet_MileageBingo         = 15,
	Packet_Statistico           = 16,										                 //统计
	Packet_SessionID            = 17,		                                                 //会话ID
	Packet_Mobile               = 18,		                                                 //移动电话验证
	Packet_Policy               = 19,
	Packet_UpROOM               = 65,		                                                 //游戏房间内的操作->创建房间->开始游戏等
	Packet_ClientCheck          = 66,		                                                 //客户端检查
	Packet_UMsg                 = 67,		                                                 //服务器消息
	Packet_Host                 = 68,
	Packet_UpdateInfo           = 69,		                                                 //更新信息
	Packet_UDP                  = 70,
	Packet_Clan                 = 71,		                                                 //军团信息
	Packet_Shop                 = 72,		                                                 //商店
	Packet_Rank                 = 73,				                                         //2020年5月4日 19:05:47  Rank
	Packet_Ban                  = 74,
	Packet_Option               = 76,		                                                 //用户配置(控制台指令之类的)  来三次 0 1 2 
	Packet_Favorite             = 77,
	Packet_Item                 = 78,
	Packet_SearchRoom           = 80,
	Packet_HostServer           = 81,
	Packet_Report               = 83,
	Packet_Title                = 84,                                                        //荣誉完成后显示的.
	Packet_Buff                 = 85,
	Packet_QuickStart           = 86,
	Packet_UserSurvey           = 87,		                                                 //用户调查
	Packet_Quest                = 88,
	Packet_MiniGame             = 89,
	Packet_Hack                 = 90,
	Packet_Metadata             = 91,		                                                 //元数据 地图列表 游戏模式之类的
	Packet_PlayerInfo           = 93,		                                                 //在线列表->查看玩家信息
	Packet_Gift_Item            = 96,	                                                     //礼物
	Packet_2nd_Password         = 97,	                                                     //二级密码
	Packet_GameMatch            = 99,		                                                 //游戏比赛
	Packet_UpdateWarehouse      = 100,		                                                 //刷新仓库信息
	Packet_CleanSystem          = 101,		                                                 //
	Packet_Ribbon               = 102,		                                                 //成就记录
	Packet_VoxelOutUI           = 103,
	Packet_WeaponAuctionEvent   = 104,		                                                 //武器拍卖活动
	Packet_Analysis             = 105,
	Packet_CoDisassemble        = 107,														 //分解嘉年华
	Packet_MileageShop          = 108,
	Packet_Help                 = 109,
	Packet_PopularInfo          = 110,		                                                 //欢迎信息
	Packet_Kick                 = 112,
	Packet_HonorShop            = 113,	                                                     //纪念商店
	Packet_EpicPieceShop        = 114,					                                     //史诗商店.一般是新年活动兑换年神
	Packet_Addon                = 115,		                                                 //插件
	Packet_QuestBadgeShop       = 116,		                                                 //问卷调查
	Packet_ReverseAuctionSystem = 117,		                                                 //求购系统-拍卖
	Packet_SeasonSystem         = 118,		                                                 //赛季系统
	Packet_GuideQuest           = 120,		                                                 //新手指引
	Packet_SantaAnim            = 122,		                                                 //圣诞老人
	Packet_UserStart            = 150,		                                                 //用户开始
	Packet_GameMatchRoomList    = 151,	                                                     //房间列表
	Packet_Inventory            = 152,		                                                 //仓库单
	Packet_Lobby                = 153,		                                                 //大厅 上线下线通知_可获取其他用户帐号和名称
	Packet_Inventory2           = 154,		                                                 //存货？
	Packet_CafeItems            = 155,
	Packet_DefaultItems         = 156,
	Packet_UserUpdateInfo       = 157,		                                                 //更新用户信息
	Packet_FabItems             = 158,		                                                 //收藏夹.喜爱的
	Packet_Event                = 159,
	Packet_CostumeInven         = 160,		                                                 //服装.可能是装饰
	Packet_ZombieScenarioMaps   = 161,	                                                     //僵尸场景地图
	Packet_RotationWeaponInven  = 162,
	Packet_Alarm                = 164,				                                         //公告
	Packet_MonthlyWeapon        = 165,			                                             //免费月武器_好像是吧
	Packet_LifeWeaponInven      = 166,
	Packet_VipSystem            = 167,                                                       //VIP系统
	Packet_FreePassWeaponInven  = 168,                                                       //免费武器
	Packet_ServerLog            = 169,	                                                     //服务器日志
	Packet_Dictionary           = 172,                                                       // 字典	-战场百科
	Packet_TestPacket           = 173,
	Packet_ZBSGoldenZB          = 174,
	Packet_Friend               = 175,                                                       //朋友
	Packet_EventItemInve        = 176,
	Packet_Expedition           = 177,                                                       //远征
	Packet_ScenarioTX           = 178,
	Packet_League               = 180,			                                             //联赛
	Packet_ClassInven           = 181,			                                             //类零件
	Packet_PartsInven           = 182,			                                             //部分件
	Packet_ItemInven            = 183,			                                             //物品零件》？
	Packet_SwitchConfig         = 184,

};
typedef struct GAME_PACKET_TYPE_INFO
{
	Packet_Type		Type;			                    //封包类型
	ULONG			Addres;			                    //跳转的地址
	ULONG			SvAddres;		                    //保存的地址
	BOOL			Hook;			                    //是否Hook
	TAG_PEB_STACK	Stack;			                    //保存的栈信息
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
	ULONG Class;	                                    //必须保存这个
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