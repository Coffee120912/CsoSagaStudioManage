#pragma once
#include "QtConnectFile.h"


#define	AM_CLIENT_VER					94	//客户端版本号

#define	AM_AGNET_VER					62	//代理端版本号


#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

//线程工作索引
enum TWorkIndex
{
	AllPowerOn	=	0,	//全部开机
	AllPowerOff	=	1,	//全部关机
	PowerOn	    =	2,	//指定开机
	PowerOff	=	3,	//指定关机
};


typedef struct QT_WORK_THREAD
{
	TWorkIndex	tIndex;					//索引
	char		szVmPath[MAX_PATH];		//VM路径

}TAG_QT_WORK_THREAD, * PTAG_QT_WORK_THREAD;

//游戏服务器信息
typedef struct SERVER_INFO
{
	ULONG			Index;
	QString			Region;
}GAME_SERVER_INFO, * PGAME_SERVER_INFO;



//游戏服务器IP和端口
typedef struct SERVER_ADDRES_INFO
{	
	SERVER_INFO		ServerInfo;
	QString			IP;
	ULONG			Port;
}GAME_SERVER_ADDRES_INFO, * PGAME_SERVER_ADDRES_INFO;


//游戏账号状态
typedef struct GAME_USER_STATUS
{
	ULONG			Index;
	QString			Text;
}TAG_GAME_USER_STATUS, * PTAG_GAME_USER_STATUS;

//游戏任务说明
typedef struct GAME_TASK_SRC
{
	ULONG			Index;
	QString			Text;
}TAG_GAME_TASK_SRC, * PTAG_GAME_TASK_SRC;


//本地进程绑定
typedef struct PROCESS_INFO
{
	ULONG			Pid;				//进程ID
	QString			KeepAlive;			//时间戳
}TAG_PROCESS_INFO, * PTAG_PROCESS_INFO;


//Agent->Serber
enum A_PACKET_INDEX
{
	as_Packet_Login     = 1,		     //账号登陆
	as_Packet_PcLogin   = 2,		     //PC登陆
	as_Packet_PcInfo    = 3,		     //取PC信息
	as_Packet_UserInfo  = 4,		     //取账号信息
	as_Packet_SetPcInfo = 5,		     //设置Pc信息
	as_Packet_SetPcName,		         //设置Pc名
	as_Packet_AddGameUserInfo,		     //添加游戏账号信息
	as_Packet_GetGameUserInfo,		     //获取游戏账号信息
	as_Packet_DelGameAllUserInfo,		 //删除全部游戏账号信息
	as_Packet_UpGameUserInfo,		     //更新游戏账号信息
	as_Packet_GetStudioInfo,		     //获取工作室信息
	as_Packet_GameUserTime,				 //账号时间戳
	as_Packet_GameUserHeartBeat,		 //检查心跳
	as_Packet_DelGameUserInfo,			 //删除指定账号
	as_Packet_KeepAlive,				 //维持心跳
	as_Packet_QueryGameUser,			 //查询游戏账号
};
//Client<->Agent
enum C_PACKET_INDEX
{
	ca_Packet_Ver = 1,					//版本验证
	ca_Packet_Reply,					//认证答复
	ca_Packet_GetGameUserConfig,		//获取游戏账号配置
	ca_Packet_SendGameUserInfo,			//发送到代理端的游戏账号信息
	ca_Packet_UpGameUserInfo,			//更新账号信息
	ca_Packet_VmInfo,					//虚拟机信息
	ca_Packet_GameFun,					//游戏功能
	ca_Packet_ClientCheck,				//客户端判断-心跳维持
	ca_Packet_GameInfo,					//游戏信息
	ca_Packet_GameClan,					//游戏军团
};


enum SQL_INDEX
{
	sql_Connect			= 0,		        //数据库连接
	sql_AgentLogin,		                    //代理端登陆
	sql_AgentPcLogin,	                    //Pc登陆
	sql_AgentInfo,	                        //取代理端信息
	sql_UserInfo,	                        //取账号信息
	sql_SetPcInfo,	   		                //设置Pc信息
	sql_SetPcName,	   		                //设置Pc名
	sql_AddGameUserInfo,	   		        //添加游戏账号信息
	sql_GetGameUserInfo,	   		        //获取游戏账号信息
	sql_DelGameAllUserInfo,	   		        //删除全部游戏账号信息
	sql_UpGameUserInfo,	   					//更新游戏账号信息
	sql_GetStudioInfo,	   					//获取工作室信息
	sql_CheckHeartBeat,	   					//检查心跳
	sql_SetTimeStamp,	   					//设置时间戳
	sql_DelGameUser,	   					//删除游戏账号
	sql_KeepAlive,	   						//维持心跳
	sql_QueryGameUser,	   					//查询游戏账号
};

typedef struct QT_AGENT_INFO
{
	QString User;
	QString MAC;
	QString PcName;
	BYTE	PcTopNotice;	            //主机上线通知
	BYTE	PcDownNotice;	            //主机下线通知
	BYTE	LoginHaltNotice;	        //登陆维护通知
	BYTE	MonitoringHaltNotice;	    //监控维护通知
	BYTE	CsoUserOutIimeNotice;		//游戏账号超时通知
	int		NoticeType;		            //通知类型

}TAG_QT_AGENT_INFO, * PTAG_QT_AGENT_INFO;


typedef struct QT_AGENT_USERINFO
{
	QString Mobile;
	QString Email;
}TAG_QT_AGENT_USERINFO, * PTAG_QT_AGENT_USERINFO;

// Studio 信息
typedef struct QT_STUDIO_INFO
{
	QString GameDllSign;				//游戏文件签名
	QString GamePackSize;				//游戏资源包大小
	QString StudioAgentVersion;			//代理端版本
	QString UpStudioAgentUrl;			//代理端更新地址
	QString StudioClientVersion;		//客户端版本
	QString UpStudioClientUrl;			//客户端更新地址
	QString GameClanID[3];				//各大区游戏军团ID



}TAG_QT_STUDIO_INFO, * PTAG_QT_STUDIO_INFO;



typedef struct QT_GAME_AC_SOCKET
{
	QString		szSignMac;				//MAC签名
	QString		ClientConnID;			//连接ID
	QStringList	StrUsers;				//用户列表		
}TAG_QT_GAME_AC_SOCKET, * PTAG_QT_GAME_AC_SOCKET;

typedef struct QT_LOCAL_CLIENT
{
	HANDLE	hPorcess;					//进程句柄
	QString	localName;					//本地名
	QString	localSign;					//签名

}TAG_QT_LOCAL_CLIENT, * PTAG_QT_LOCAL_CLIENT;


typedef struct QT_VM_CON
{
	QString vm_id;               //ID
	QString vm_name;            //虚拟机名称
	QString vm_path;            //虚拟机路径
	QString	vm_sign;			//虚拟机标志
	QString	vm_socket_status;	//连接状态
	QString vm_status;          //虚拟机状态
	QString vm_file;            //虚拟机文件
	QString vm_timestamp;       //虚拟机时间戳
	QString vm_count;			//虚拟机计次
	QString vm_pwn_on_count;	//虚拟机开机计次
	QString vm_monitor_status;	//虚拟机监控状态
	QString vm_restart_time;	//虚拟机重启时间
	QT_VM_CON()
	{
		vm_monitor_status	= QString::fromLocal8Bit("是");
		vm_status			= QString::fromLocal8Bit("未知状态,等待获取");
		vm_sign				= QString::fromLocal8Bit("等待通讯获取");
		vm_socket_status	= QString::fromLocal8Bit("未连接");
		vm_pwn_on_count		= QString::number(0);
	}

}TAG_QT_VM_CON, * PTAG_QT_VM_CON;

typedef struct QT_GAME_AC_CON
{
	QString ac_id;              //ID
	QString	ac_vm_name;         //分配的虚拟机名
	
	QString ac_reg;             //游戏大区
	QString	ac_user;			//游戏账号
	QString ac_password;        //游戏密码
	QString ac_lv;				//游戏等级

	QString ac_mileage;         //游戏积分
	QString ac_gold;			//游戏金币
	QString ac_name;			//游戏角色名
	QString ac_timestamp;		//时间戳

	//QString ac_decoder_info;	//破译芯片_信息
	QString ac_Inventory_info;	//仓库_信息

	QString ac_cur_task;		//当前执行的任务
	QString ac_TaskStatus;		//账号任务状态

	QString ac_usetStatus;		//账号状态
	QString ac_uset2nd;			//二级密码


}TAG_QT_GAME_AC_CON, * PTAG_QT_GAME_AC_CON;

#define	AM_UDP_TYPE_UN					0	//未知
#define	AM_UDP_TYPE_CLIENT				1	//客户端
#define	AM_UDP_TYPE_SERVER				2	//服务端





typedef struct QT_COMMU
{
	//int		nType;			//	类型		0	=	未知		1	=	客户端			2	=	服务端
	int		nSrcGoal;			//	当前类型
	int		nDescGoal;			//	目标类型
	int		nIndex;				//	功能索引
	int		nSize;				//	大小
	BYTE	cBuf[1];			//	缓冲区
}TAG_QT_COMMU, * PTAG_QT_COMMU;


#define	SF_VM_NAME					0	// 修改虚拟机分配名
#define	SF_GAME_REG					1	// 修改游戏注册大区
#define	SF_AC_STATUS				2	// 修改账号状态
#define	SF_AC_TASK					3	// 修改账号任务
#define	SF_GAME_FUNS				4	// 游戏功能设置
#define	SF_GAME_USER_DEL			5	// 游戏账号删除




enum VM_PACKET_INDEX
{
	vm_Packet_Version		=				1000,		//版本号
	vm_Packet_Reply			=				1001,		//答复
	vm_Packet_ClientCheck	=				1002,		//客户端检查.检查时间戳
	vm_Packet_VmConfig		=				1003,		//VM全局配置
	vm_Packet_GameConfig	=				1004,		//游戏账号全局配置,手动
	vm_Packet_GameInfo		=				1005,		//游戏账号信息
	vm_Packet_GameFuns		=				1006,		//游戏功能
};



#define	CL_MSG_TYPE_NOTICE						1		// 通知
#define	CL_MSG_TYPE_HEART						2		// 心跳维持
#define	CL_MSG_TYPE_AC_INFO						3		// 账号信息






enum USER_STATUS
{
	USER_SUCCEED		,					//账号正常
	USER_ERROR_R0		,					//保护封停
	USER_ERROR_R1		,					//外挂封停
	USER_ERROR_R2		,					//登陆上限
	USER_ERROR_R3		,					//密码错误
	USER_ERROR_R4,							//该账号已锁定登录，请通过[天成账号管家APP]进行解锁
	USER_ERROR_R5,							//身份证未认证
	USER_ERROR_UN,							//未知状态
};


