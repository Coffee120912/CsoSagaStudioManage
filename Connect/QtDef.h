#pragma once
#include "QtConnectFile.h"


#define	AM_CLIENT_VER					94	//�ͻ��˰汾��

#define	AM_AGNET_VER					62	//����˰汾��


#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

//�̹߳�������
enum TWorkIndex
{
	AllPowerOn	=	0,	//ȫ������
	AllPowerOff	=	1,	//ȫ���ػ�
	PowerOn	    =	2,	//ָ������
	PowerOff	=	3,	//ָ���ػ�
};


typedef struct QT_WORK_THREAD
{
	TWorkIndex	tIndex;					//����
	char		szVmPath[MAX_PATH];		//VM·��

}TAG_QT_WORK_THREAD, * PTAG_QT_WORK_THREAD;

//��Ϸ��������Ϣ
typedef struct SERVER_INFO
{
	ULONG			Index;
	QString			Region;
}GAME_SERVER_INFO, * PGAME_SERVER_INFO;



//��Ϸ������IP�Ͷ˿�
typedef struct SERVER_ADDRES_INFO
{	
	SERVER_INFO		ServerInfo;
	QString			IP;
	ULONG			Port;
}GAME_SERVER_ADDRES_INFO, * PGAME_SERVER_ADDRES_INFO;


//��Ϸ�˺�״̬
typedef struct GAME_USER_STATUS
{
	ULONG			Index;
	QString			Text;
}TAG_GAME_USER_STATUS, * PTAG_GAME_USER_STATUS;

//��Ϸ����˵��
typedef struct GAME_TASK_SRC
{
	ULONG			Index;
	QString			Text;
}TAG_GAME_TASK_SRC, * PTAG_GAME_TASK_SRC;


//���ؽ��̰�
typedef struct PROCESS_INFO
{
	ULONG			Pid;				//����ID
	QString			KeepAlive;			//ʱ���
}TAG_PROCESS_INFO, * PTAG_PROCESS_INFO;


//Agent->Serber
enum A_PACKET_INDEX
{
	as_Packet_Login     = 1,		     //�˺ŵ�½
	as_Packet_PcLogin   = 2,		     //PC��½
	as_Packet_PcInfo    = 3,		     //ȡPC��Ϣ
	as_Packet_UserInfo  = 4,		     //ȡ�˺���Ϣ
	as_Packet_SetPcInfo = 5,		     //����Pc��Ϣ
	as_Packet_SetPcName,		         //����Pc��
	as_Packet_AddGameUserInfo,		     //�����Ϸ�˺���Ϣ
	as_Packet_GetGameUserInfo,		     //��ȡ��Ϸ�˺���Ϣ
	as_Packet_DelGameAllUserInfo,		 //ɾ��ȫ����Ϸ�˺���Ϣ
	as_Packet_UpGameUserInfo,		     //������Ϸ�˺���Ϣ
	as_Packet_GetStudioInfo,		     //��ȡ��������Ϣ
	as_Packet_GameUserTime,				 //�˺�ʱ���
	as_Packet_GameUserHeartBeat,		 //�������
	as_Packet_DelGameUserInfo,			 //ɾ��ָ���˺�
	as_Packet_KeepAlive,				 //ά������
	as_Packet_QueryGameUser,			 //��ѯ��Ϸ�˺�
};
//Client<->Agent
enum C_PACKET_INDEX
{
	ca_Packet_Ver = 1,					//�汾��֤
	ca_Packet_Reply,					//��֤��
	ca_Packet_GetGameUserConfig,		//��ȡ��Ϸ�˺�����
	ca_Packet_SendGameUserInfo,			//���͵�����˵���Ϸ�˺���Ϣ
	ca_Packet_UpGameUserInfo,			//�����˺���Ϣ
	ca_Packet_VmInfo,					//�������Ϣ
	ca_Packet_GameFun,					//��Ϸ����
	ca_Packet_ClientCheck,				//�ͻ����ж�-����ά��
	ca_Packet_GameInfo,					//��Ϸ��Ϣ
	ca_Packet_GameClan,					//��Ϸ����
};


enum SQL_INDEX
{
	sql_Connect			= 0,		        //���ݿ�����
	sql_AgentLogin,		                    //����˵�½
	sql_AgentPcLogin,	                    //Pc��½
	sql_AgentInfo,	                        //ȡ�������Ϣ
	sql_UserInfo,	                        //ȡ�˺���Ϣ
	sql_SetPcInfo,	   		                //����Pc��Ϣ
	sql_SetPcName,	   		                //����Pc��
	sql_AddGameUserInfo,	   		        //�����Ϸ�˺���Ϣ
	sql_GetGameUserInfo,	   		        //��ȡ��Ϸ�˺���Ϣ
	sql_DelGameAllUserInfo,	   		        //ɾ��ȫ����Ϸ�˺���Ϣ
	sql_UpGameUserInfo,	   					//������Ϸ�˺���Ϣ
	sql_GetStudioInfo,	   					//��ȡ��������Ϣ
	sql_CheckHeartBeat,	   					//�������
	sql_SetTimeStamp,	   					//����ʱ���
	sql_DelGameUser,	   					//ɾ����Ϸ�˺�
	sql_KeepAlive,	   						//ά������
	sql_QueryGameUser,	   					//��ѯ��Ϸ�˺�
};

typedef struct QT_AGENT_INFO
{
	QString User;
	QString MAC;
	QString PcName;
	BYTE	PcTopNotice;	            //��������֪ͨ
	BYTE	PcDownNotice;	            //��������֪ͨ
	BYTE	LoginHaltNotice;	        //��½ά��֪ͨ
	BYTE	MonitoringHaltNotice;	    //���ά��֪ͨ
	BYTE	CsoUserOutIimeNotice;		//��Ϸ�˺ų�ʱ֪ͨ
	int		NoticeType;		            //֪ͨ����

}TAG_QT_AGENT_INFO, * PTAG_QT_AGENT_INFO;


typedef struct QT_AGENT_USERINFO
{
	QString Mobile;
	QString Email;
}TAG_QT_AGENT_USERINFO, * PTAG_QT_AGENT_USERINFO;

// Studio ��Ϣ
typedef struct QT_STUDIO_INFO
{
	QString GameDllSign;				//��Ϸ�ļ�ǩ��
	QString GamePackSize;				//��Ϸ��Դ����С
	QString StudioAgentVersion;			//����˰汾
	QString UpStudioAgentUrl;			//����˸��µ�ַ
	QString StudioClientVersion;		//�ͻ��˰汾
	QString UpStudioClientUrl;			//�ͻ��˸��µ�ַ
	QString GameClanID[3];				//��������Ϸ����ID



}TAG_QT_STUDIO_INFO, * PTAG_QT_STUDIO_INFO;



typedef struct QT_GAME_AC_SOCKET
{
	QString		szSignMac;				//MACǩ��
	QString		ClientConnID;			//����ID
	QStringList	StrUsers;				//�û��б�		
}TAG_QT_GAME_AC_SOCKET, * PTAG_QT_GAME_AC_SOCKET;

typedef struct QT_LOCAL_CLIENT
{
	HANDLE	hPorcess;					//���̾��
	QString	localName;					//������
	QString	localSign;					//ǩ��

}TAG_QT_LOCAL_CLIENT, * PTAG_QT_LOCAL_CLIENT;


typedef struct QT_VM_CON
{
	QString vm_id;               //ID
	QString vm_name;            //���������
	QString vm_path;            //�����·��
	QString	vm_sign;			//�������־
	QString	vm_socket_status;	//����״̬
	QString vm_status;          //�����״̬
	QString vm_file;            //������ļ�
	QString vm_timestamp;       //�����ʱ���
	QString vm_count;			//������ƴ�
	QString vm_pwn_on_count;	//����������ƴ�
	QString vm_monitor_status;	//��������״̬
	QString vm_restart_time;	//���������ʱ��
	QT_VM_CON()
	{
		vm_monitor_status	= QString::fromLocal8Bit("��");
		vm_status			= QString::fromLocal8Bit("δ֪״̬,�ȴ���ȡ");
		vm_sign				= QString::fromLocal8Bit("�ȴ�ͨѶ��ȡ");
		vm_socket_status	= QString::fromLocal8Bit("δ����");
		vm_pwn_on_count		= QString::number(0);
	}

}TAG_QT_VM_CON, * PTAG_QT_VM_CON;

typedef struct QT_GAME_AC_CON
{
	QString ac_id;              //ID
	QString	ac_vm_name;         //������������
	
	QString ac_reg;             //��Ϸ����
	QString	ac_user;			//��Ϸ�˺�
	QString ac_password;        //��Ϸ����
	QString ac_lv;				//��Ϸ�ȼ�

	QString ac_mileage;         //��Ϸ����
	QString ac_gold;			//��Ϸ���
	QString ac_name;			//��Ϸ��ɫ��
	QString ac_timestamp;		//ʱ���

	//QString ac_decoder_info;	//����оƬ_��Ϣ
	QString ac_Inventory_info;	//�ֿ�_��Ϣ

	QString ac_cur_task;		//��ǰִ�е�����
	QString ac_TaskStatus;		//�˺�����״̬

	QString ac_usetStatus;		//�˺�״̬
	QString ac_uset2nd;			//��������


}TAG_QT_GAME_AC_CON, * PTAG_QT_GAME_AC_CON;

#define	AM_UDP_TYPE_UN					0	//δ֪
#define	AM_UDP_TYPE_CLIENT				1	//�ͻ���
#define	AM_UDP_TYPE_SERVER				2	//�����





typedef struct QT_COMMU
{
	//int		nType;			//	����		0	=	δ֪		1	=	�ͻ���			2	=	�����
	int		nSrcGoal;			//	��ǰ����
	int		nDescGoal;			//	Ŀ������
	int		nIndex;				//	��������
	int		nSize;				//	��С
	BYTE	cBuf[1];			//	������
}TAG_QT_COMMU, * PTAG_QT_COMMU;


#define	SF_VM_NAME					0	// �޸������������
#define	SF_GAME_REG					1	// �޸���Ϸע�����
#define	SF_AC_STATUS				2	// �޸��˺�״̬
#define	SF_AC_TASK					3	// �޸��˺�����
#define	SF_GAME_FUNS				4	// ��Ϸ��������
#define	SF_GAME_USER_DEL			5	// ��Ϸ�˺�ɾ��




enum VM_PACKET_INDEX
{
	vm_Packet_Version		=				1000,		//�汾��
	vm_Packet_Reply			=				1001,		//��
	vm_Packet_ClientCheck	=				1002,		//�ͻ��˼��.���ʱ���
	vm_Packet_VmConfig		=				1003,		//VMȫ������
	vm_Packet_GameConfig	=				1004,		//��Ϸ�˺�ȫ������,�ֶ�
	vm_Packet_GameInfo		=				1005,		//��Ϸ�˺���Ϣ
	vm_Packet_GameFuns		=				1006,		//��Ϸ����
};



#define	CL_MSG_TYPE_NOTICE						1		// ֪ͨ
#define	CL_MSG_TYPE_HEART						2		// ����ά��
#define	CL_MSG_TYPE_AC_INFO						3		// �˺���Ϣ






enum USER_STATUS
{
	USER_SUCCEED		,					//�˺�����
	USER_ERROR_R0		,					//������ͣ
	USER_ERROR_R1		,					//��ҷ�ͣ
	USER_ERROR_R2		,					//��½����
	USER_ERROR_R3		,					//�������
	USER_ERROR_R4,							//���˺���������¼����ͨ��[����˺Źܼ�APP]���н���
	USER_ERROR_R5,							//���֤δ��֤
	USER_ERROR_UN,							//δ֪״̬
};


