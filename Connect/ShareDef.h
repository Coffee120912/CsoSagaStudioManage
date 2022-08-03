#pragma once
#include <CACEInterface.h>
#include <map>


enum class TASK_MODE
{

	GAME_TASK_LOBBY,						//大厅挂机
	GAME_TASK_MP_ZBS_NOTKEY,				//生化Z 挂积分 不可加密码
	//GAME_TASK_MP_PVE_ONLINE,				//绝命档案联机 10人房间
	GAME_TASK_ZBS_ONLINE,					//生化Z,多人挂机
	GAME_TASK_BATING,						//批量登陆检测
	GAME_TASK_Vxl_PVE,						//缔造者单人挂机
	//GAME_TASK_PVP_ONLINE,					//绝命档案-多人在线挂机 挂多人任务
	GAME_TASK_AUCTION,						//拍卖行Auction
	GAME_TASK_UNKONW,						//未知任务
};

enum class USER_STATUS
{
	USER_SUCCEED,					//账号正常
	USER_ERROR_R0,					//保护封停
	USER_ERROR_R1,					//外挂封停
	USER_ERROR_R2,					//登陆上限
	USER_ERROR_R3,					//密码错误
	USER_ERROR_R4,					//该账号已锁定登录，请通过[天成账号管家APP]进行解锁
	USER_ERROR_R5,					//身份证未认证
	USER_ERROR_UN,					//未知状态
};

//拍卖行信息
typedef struct GAME_AUCTION
{
	BYTE		Row;			        //itemRow
	int			AuctionID;		        //拍卖的序号
	int			AddedTimer;		        //上架的时间戳
	SHORT		ItemID;			        //物品ID
	int			Bidding;		        //竞价价格
	int			Price;			        //一口价
	int			RemainTime;		        //剩余时间_秒
	short		CurViewPaintID;		    //当前显示的彩漆ID
	short		PaintCount;			    //彩漆数量
	short		PaintID[16];			//一个武器最多16个彩漆
	int			ItemAttribute;		    //属性
	BYTE		HitRate;				//属性_命中
	BYTE		Damage;					//属性_伤害
	BYTE		Recoil;					//属性_后坐力
	BYTE		Weight;					//属性_重量
	BYTE		Bursts;					//属性_连发
	BYTE		Clip;					//属性_弹夹
	short		Enchantlv;				//武器强化等级
	int			Earnings;				//实际收益积分-收回或成功拍卖的价格
	inline GAME_AUCTION()
	{
		RtlZeroMemory(this, sizeof(GAME_AUCTION));
	}

}TAG_GAME_AUCTION, * PTAG_GAME_AUCTION;

//拍卖行任务信息
typedef struct GAME_TASK_AUCTION
{
	ULONG ulTaskID;					        //任务ID
	ULONG ulTaskType;				        //任务类型 0=搜索指定item ID  1 = 随机武器  2 = 转武器-商家  3 = 转武器-客户
	short ItemID;					        //物品ID
	int	  Price;			                //一口价
	int   BuyCount;					        //购买次数 0 = 购买完毕任务完成 
	BYTE  PaintType;				        //彩漆类型 0 = 无彩漆 1 = 必须符合当前设置的彩漆ID   2 = 有彩漆就行,没彩漆就不符合
	BYTE  PaintCount;				        //彩漆数量
	short PaintID[16];				        //彩漆ID
	int   Enchantlv;				        //强化等级 0 = 不限制
	int   ItemAttribute;			        //强制属性 0 = 不限制	1 = 必须全部符合
	BYTE		HitRate;				    //属性_命中
	BYTE		Damage;					    //属性_伤害
	BYTE		Recoil;					    //属性_后坐力
	BYTE		Weight;					    //属性_重量
	BYTE		Bursts;					    //属性_连发
	BYTE		Clip;					    //属性_弹夹
	BYTE		TaskStatus;				    //任务状态 0 = 任务未完成 1 = 任务已完成	2 = 任务已删除
	//std::list<int> *lstAcID;				//任务派发的账号ID
	BYTE		ACIDCount;				    //派发账号的数量
	char		szACArray[100][MAX_PATH];	//派发的账号数组最高100个
	//-------------------------------------------------------------------
	// 排除项
	//-------------------------------------------------------------------
	BYTE		BlackListItem;				//黑名单道具数量
	short		BlackListItemID[100];		//黑名单道具ID
	BYTE		BlackListPaint;				//黑名单喷漆数量
	short		BlackListPaintID[100];		//黑名单道具ID
	//BYTE		EnableEnchant;				//关闭强化-武器必须是没有强化的
	BYTE		FindType;					//搜索类型
	short		PaintShow;					//要搜索的显示的武器的彩漆.
	inline GAME_TASK_AUCTION()
	{
		RtlZeroMemory(this, sizeof(GAME_TASK_AUCTION));
	}


}TAG_GAME_TASK_AUCTION, * PTAG_GAME_TASK_AUCTION;

// 共享游戏账号数据
typedef struct SE_GAME_AC_CLIENT
{
	ULONG		nID;					//ID
	BYTE		GameReg;				//注册大区
	std::string GameUserName;           //游戏账号
	std::string GameUserPassWord;       //游戏密码
	ULONG		Lv;						//等级
	std::string PlayerName;				//角色名
	ULONG64		Mileage;				//欢乐积分
	ULONG64		Gold;					//金币
	std::string Game2ndpassword;		//游戏仓库密码

	
	TASK_MODE	UserTask;					//游戏任务
	BYTE		UserTaskStatus;				//账号任务状态 0 = 正常 1 =停止
	int			UserLoginCount;				//账号登陆次数
	USER_STATUS	UserStatus;					//账号状态
	
	//-------------------------------------------------------------------
	// 功能选择
	//-------------------------------------------------------------------
	ULONG64		AuctionFlag;			//拍卖会功能标志
	// 保存的上架的拍卖会信息
	std::map<int, TAG_GAME_AUCTION> AuctionRackingItems;
	// 保存的转武器-客户要搜索的任务
	std::map<int, TAG_GAME_TASK_AUCTION> AuctionClientTask;
	// 当前上架激活的武器
	BYTE AuctionActiveWeapon;	

	BYTE	eventdecoderAutoOepn;		//活动破译芯片自动开启
	BYTE	roomTimerCheck;				//房间时间检测
	BYTE	roomTimerEvent;				//房间时间检测事件

	ULONG64	clientTimeStamp;			//客户端时间戳
	int		clientTimerOutErrorCount;	//客户端超时计次
	int		clientSocketId;				//客户端连接
	int		connectPort;				//连接端口

	
	BYTE	alldecoderAutoOepn;			//全部破译芯片自动开启

	BYTE	autoGift;					//自动赠送礼物
	BYTE	giftReg;					//礼物大区
	int		giftID;						//礼物ID
	char	giftPlayerName[MAX_PATH];	//被赠送人名称

	BYTE	up2ndpassword;				//修改仓库二级

	BYTE	bAutoOpenItemBox;			//自动打开道具盒子

	int     nClanID[10];				//军团ID  一共10个

	BYTE	nSetCPUCore;				//设置CPU核心,本机多开默认开启 如果是虚拟机则不开启

	BYTE	nJojnClan;					//是否加入军团

	BYTE	nSetminiWindows;			//是否设置小窗口

	BYTE	undefined[1024];

	SE_GAME_AC_CLIENT()
	{
		nID              = 0;
		GameReg          = 0;
		GameUserName     = std::string();
		GameUserPassWord = std::string();
		UserTask         = TASK_MODE::GAME_TASK_LOBBY;
		UserTaskStatus   = 0;
		UserLoginCount   = 0;
		UserStatus       = USER_STATUS::USER_SUCCEED;
		Mileage		     = 0;
		Lv               = 1;
		Gold             = 0;
		AuctionFlag      = 0;
	}
}TAG_SE_GAME_AC_CLIENT, * PTAG_SE_GAME_AC_CLIENT;

//全局共享功能
typedef struct GLOBAL_FUNCTION
{
	BYTE AutoJoinAuctionEvent;				//自动参加拍卖会活动
	
	


	inline GLOBAL_FUNCTION()
	{
		RtlZeroMemory(this,sizeof(GLOBAL_FUNCTION));
	}
}TAG_GLOBAL_FUNCTION,* PTAG_GLOBAL_FUNCTION;




enum class LOGIN_NOTICE_CODE 
{
	NOTICE_GAME_INIT_SUCCESS                 = 1000,								//初始化成功
	NOTICE_GAME_AUTHREPLE_SUCCESS            = 1001,							    //登陆成功
	NOTICE_GAME_AUTHREPLE_ERROR_U1           = 1002,							    //未知错误
	NOTICE_GAME_AUTHREPLE_ERROR_R2           = 1003,							    //密码错误或账号不存在。
	NOTICE_GAME_AUTHREPLE_ERROR_R3           = 1004,							    //此账号已被封停，详情请登录官网查询。
	NOTICE_GAME_AUTHREPLE_ERROR_R4           = 1005,							    //服务器维护中，无法登录游戏。
	NOTICE_GAME_AUTHREPLE_ERROR_R5           = 1006,							    //您的IP已被屏蔽，请联系客服中心
	NOTICE_GAME_AUTHREPLE_ERROR_R6           = 1007,							    //登录过于频繁，请稍后再试
	NOTICE_GAME_AUTHREPLE_ERROR_R7           = 1008,							    //服务器连接失败，请稍后再试
	NOTICE_GAME_AUTHREPLE_ERROR_CAPT         = 1009,							    //验证码错误上线
	NOTICE_GAME_ROOM_TIMEROUT                = 1010,							    //房间超时
	NOTICE_GAME_SERVER_TIMEROUT              = 1011,							    //服务器超时
	NOTICE_APP_HEART_TIMEROUT                = 1012,							    //软件通讯超时
	NOTICE_GAME_AC_ERROR_STATUS              = 1013,							    //账号状态异常...有很多原因
	NOTICE_NEXON_REPLE_ERROR                 = 1014,							    //服务器发生故障
	NOTICE_GAME_AUTHREPLE_ERROR_CAPT_COUNT   = 1015,				                //验证码错误计次
	NOTICE_GAME_REPLY_INVALID_CLIENT_VERSION = 1016,			                    //客户端版本不一致
	NOTICE_GAME_NGS_INIT_ERROR               = 1017,							    //NGS初始化错误
	NOTICE_GAME_AUTHREPLE_ERROR_R8           = 1018,						        //外挂封停
	NOTICE_GAME_AUTHREPLE_ERROR_UNK_STR      = 1019,					            //未知错误格式化
	NOTICE_GAME_AUTHREPLE_ERROR_R9           = 1020,						        //该账号已锁定登录，请通过[天成账号管家APP]进行解锁
	NOTICE_GAME_PP_NOT_PASS                  = 1021,								//防挂机系统未检测到PASS
	NOTICE_GAME_AUTHREPLE_ERROR_R11          = 1022,						        //实名认证
	NOTICE_GAME_NGS_PASS_INIT_ERROR          = 1023,						        //NGS插件初加载失败
	NOTICE_NEXON_REPLE_INVALIDNAME           = 1024,						        //错误的ID		
	NOTICE_NEXON_REPLE_INVALID_CHAR			 = 1025,						        //非法字符.						---↓ 2021-09-11 16:03:59
	NOTICE_NEXON_REPLE_UNCODE				 = 1026,						        //认证未知代码
	NOTICE_NEXON_REPLE_SUCCEED				 = 1027,						        //认证成功

	NOTICE_GAME_AUTHREPLE_ERROR_R12			 = 1028,						        //账号进入NGS黑名单.
	NOTICE_TER_CONNECT_ER					 = 1029,						        //连接Teriri服务器失败
	NOTICE_TER_CONNECT_SUCCEED				 = 1030,						        //连接Teriri服务器成功
	NOTICE_NEXON_REPLE_SYSERROR				 = 1031,							    //认证系统错误
	NOTICE_CPU_SET							 = 1032,							    //CPU核心
	
	NOTICE_NEXON_CREATEOK = 1033,						        //用户创建成功

};

typedef struct GAME_AC_LOGIN {

	ULONG  ID;	
	std::string GUID;
	std::string CmdLine;				//运行命令行
	DWORD  dwProceeExitCode;		    //进程退出码
	HANDLE hLoginEvent;	                //登陆事件句柄
	HANDLE hProcess;					//进程句柄
	DWORD  dwProcessID;					//进程ID
	ULONG64  ulTimeStamp;				//心跳时间
	
	PTAG_SE_GAME_AC_CLIENT	pData;

	inline GAME_AC_LOGIN()
	{
		ID               = 0;
		dwProceeExitCode = 0;
		hLoginEvent      = INVALID_HANDLE_VALUE;
		hProcess         = INVALID_HANDLE_VALUE;
		ulTimeStamp      = 0;
		pData            = nullptr;
		dwProcessID		 = 0;
	}

}TAG_GAME_AC_LOGIN, * PTAG_GAME_AC_LOGIN;



//保存着pid user
typedef struct _LocaProcessInfo
{
	PVOID pSession;				//保存的session
	PVOID pUserData;			//保存的共享用户数据
} LocaProcessInfo, * PLocaProcessInfo;


enum class MSG_ID
{
	MSG_STOPLOGIN_OUTNUMBER,			//登录停止,登录超过指定次数
	MSG_START_GAME,						//开始启动游戏
};

//游戏账号状态
typedef struct GAME_USER_STATUS
{
	USER_STATUS		Index;
	std::string		Text;
}TAG_GAME_USER_STATUS, * PTAG_GAME_USER_STATUS;

//游戏任务状态
typedef struct GAME_TASK_SRC
{
	TASK_MODE		Mode;
	std::string		Text;
}TAG_GAME_TASK_SRC, * PTAG_GAME_TASK_SRC;





//拍卖行解析转换json->
typedef struct PARSE_AUTICON
{
	ULONG* This;

	inline PARSE_AUTICON(std::map<ULONG, PTAG_GAME_TASK_AUCTION> &_This)
	{
		This = (ULONG*) & _This;
	}
	inline PARSE_AUTICON()
	{
		This = nullptr;
	}

}TAG_PARSE_AUTICON, * PTAG_PARSE_AUTICON;

//账号配置转换json
typedef struct PARSE_ACINFO
{
	ULONG* This;

	inline PARSE_ACINFO(std::map<ULONG, PTAG_GAME_AC_LOGIN>& _This)
	{
		This = (ULONG*)&_This;
	}
	inline PARSE_ACINFO()
	{
		This = nullptr;
	}

}TAG_PARSE_ACINFO, * PTAG_PARSE_ACINFO;

// 拍卖会通知类型
enum class AuctionNotice
{
	BuyItem_Succeed,			            //购买成功
	Integral_Lack,				            //购买前,积分不足
	Integral_Lack_SV,			            //服务器通知,购买积分不足
	Mismatching_EnableEnchant,	            //强化不匹配
	Mismatching_Paint_BlackList,	        //彩漆不匹配,在黑名单里
	Mismatching_Item_BlackList,	            //武器不匹配
};



typedef struct AUTCION_SPECIAL_EVENTS_ITEMS
{
	int		nSign;							//标识
	int		nTimeStamp;						//好像是时间戳
	short	nItemID;						//物品ID
	int		nStartTimeStamp;				//开始时间
	BYTE	Un_1;							//未知
	
	inline AUTCION_SPECIAL_EVENTS_ITEMS()
	{
		nSign           = 0;
		nTimeStamp      = 0;
		nItemID         = 0;
		nStartTimeStamp = 0;
		Un_1            = 0;
	}

	
}TAG_AUTCION_SPECIAL_EVENTS_ITEMS,* PTAG_AUTCION_SPECIAL_EVENTS_ITEMS;
// 拍卖会特别活动
typedef struct AUTCION_SPECIAL_EVENTS
{
	BYTE v1;
	BYTE v2;
	BYTE v3;
	std::string Title;		//标题-UTF8
	std::string Content;	//内容
	BYTE Count;				//数量
	std::map<ULONG, TAG_AUTCION_SPECIAL_EVENTS_ITEMS> MapItems;

}TAG_AUTCION_SPECIAL_EVENTS, * PTAG_AUTCION_SPECIAL_EVENTS;