#pragma once
#include <CACEInterface.h>
#include <map>


enum class TASK_MODE
{

	GAME_TASK_LOBBY,						//�����һ�
	GAME_TASK_MP_ZBS_NOTKEY,				//����Z �һ��� ���ɼ�����
	//GAME_TASK_MP_PVE_ONLINE,				//������������ 10�˷���
	GAME_TASK_ZBS_ONLINE,					//����Z,���˹һ�
	GAME_TASK_BATING,						//������½���
	GAME_TASK_Vxl_PVE,						//�����ߵ��˹һ�
	//GAME_TASK_PVP_ONLINE,					//��������-�������߹һ� �Ҷ�������
	GAME_TASK_AUCTION,						//������Auction
	GAME_TASK_UNKONW,						//δ֪����
};

enum class USER_STATUS
{
	USER_SUCCEED,					//�˺�����
	USER_ERROR_R0,					//������ͣ
	USER_ERROR_R1,					//��ҷ�ͣ
	USER_ERROR_R2,					//��½����
	USER_ERROR_R3,					//�������
	USER_ERROR_R4,					//���˺���������¼����ͨ��[����˺Źܼ�APP]���н���
	USER_ERROR_R5,					//���֤δ��֤
	USER_ERROR_UN,					//δ֪״̬
};

//��������Ϣ
typedef struct GAME_AUCTION
{
	BYTE		Row;			        //itemRow
	int			AuctionID;		        //���������
	int			AddedTimer;		        //�ϼܵ�ʱ���
	SHORT		ItemID;			        //��ƷID
	int			Bidding;		        //���ۼ۸�
	int			Price;			        //һ�ڼ�
	int			RemainTime;		        //ʣ��ʱ��_��
	short		CurViewPaintID;		    //��ǰ��ʾ�Ĳ���ID
	short		PaintCount;			    //��������
	short		PaintID[16];			//һ���������16������
	int			ItemAttribute;		    //����
	BYTE		HitRate;				//����_����
	BYTE		Damage;					//����_�˺�
	BYTE		Recoil;					//����_������
	BYTE		Weight;					//����_����
	BYTE		Bursts;					//����_����
	BYTE		Clip;					//����_����
	short		Enchantlv;				//����ǿ���ȼ�
	int			Earnings;				//ʵ���������-�ջػ�ɹ������ļ۸�
	inline GAME_AUCTION()
	{
		RtlZeroMemory(this, sizeof(GAME_AUCTION));
	}

}TAG_GAME_AUCTION, * PTAG_GAME_AUCTION;

//������������Ϣ
typedef struct GAME_TASK_AUCTION
{
	ULONG ulTaskID;					        //����ID
	ULONG ulTaskType;				        //�������� 0=����ָ��item ID  1 = �������  2 = ת����-�̼�  3 = ת����-�ͻ�
	short ItemID;					        //��ƷID
	int	  Price;			                //һ�ڼ�
	int   BuyCount;					        //������� 0 = �������������� 
	BYTE  PaintType;				        //�������� 0 = �޲��� 1 = ������ϵ�ǰ���õĲ���ID   2 = �в������,û����Ͳ�����
	BYTE  PaintCount;				        //��������
	short PaintID[16];				        //����ID
	int   Enchantlv;				        //ǿ���ȼ� 0 = ������
	int   ItemAttribute;			        //ǿ������ 0 = ������	1 = ����ȫ������
	BYTE		HitRate;				    //����_����
	BYTE		Damage;					    //����_�˺�
	BYTE		Recoil;					    //����_������
	BYTE		Weight;					    //����_����
	BYTE		Bursts;					    //����_����
	BYTE		Clip;					    //����_����
	BYTE		TaskStatus;				    //����״̬ 0 = ����δ��� 1 = ���������	2 = ������ɾ��
	//std::list<int> *lstAcID;				//�����ɷ����˺�ID
	BYTE		ACIDCount;				    //�ɷ��˺ŵ�����
	char		szACArray[100][MAX_PATH];	//�ɷ����˺��������100��
	//-------------------------------------------------------------------
	// �ų���
	//-------------------------------------------------------------------
	BYTE		BlackListItem;				//��������������
	short		BlackListItemID[100];		//����������ID
	BYTE		BlackListPaint;				//��������������
	short		BlackListPaintID[100];		//����������ID
	//BYTE		EnableEnchant;				//�ر�ǿ��-����������û��ǿ����
	BYTE		FindType;					//��������
	short		PaintShow;					//Ҫ��������ʾ�������Ĳ���.
	inline GAME_TASK_AUCTION()
	{
		RtlZeroMemory(this, sizeof(GAME_TASK_AUCTION));
	}


}TAG_GAME_TASK_AUCTION, * PTAG_GAME_TASK_AUCTION;

// ������Ϸ�˺�����
typedef struct SE_GAME_AC_CLIENT
{
	ULONG		nID;					//ID
	BYTE		GameReg;				//ע�����
	std::string GameUserName;           //��Ϸ�˺�
	std::string GameUserPassWord;       //��Ϸ����
	ULONG		Lv;						//�ȼ�
	std::string PlayerName;				//��ɫ��
	ULONG64		Mileage;				//���ֻ���
	ULONG64		Gold;					//���
	std::string Game2ndpassword;		//��Ϸ�ֿ�����

	
	TASK_MODE	UserTask;					//��Ϸ����
	BYTE		UserTaskStatus;				//�˺�����״̬ 0 = ���� 1 =ֹͣ
	int			UserLoginCount;				//�˺ŵ�½����
	USER_STATUS	UserStatus;					//�˺�״̬
	
	//-------------------------------------------------------------------
	// ����ѡ��
	//-------------------------------------------------------------------
	ULONG64		AuctionFlag;			//�����Ṧ�ܱ�־
	// ������ϼܵ���������Ϣ
	std::map<int, TAG_GAME_AUCTION> AuctionRackingItems;
	// �����ת����-�ͻ�Ҫ����������
	std::map<int, TAG_GAME_TASK_AUCTION> AuctionClientTask;
	// ��ǰ�ϼܼ��������
	BYTE AuctionActiveWeapon;	

	BYTE	eventdecoderAutoOepn;		//�����оƬ�Զ�����
	BYTE	roomTimerCheck;				//����ʱ����
	BYTE	roomTimerEvent;				//����ʱ�����¼�

	ULONG64	clientTimeStamp;			//�ͻ���ʱ���
	int		clientTimerOutErrorCount;	//�ͻ��˳�ʱ�ƴ�
	int		clientSocketId;				//�ͻ�������
	int		connectPort;				//���Ӷ˿�

	
	BYTE	alldecoderAutoOepn;			//ȫ������оƬ�Զ�����

	BYTE	autoGift;					//�Զ���������
	BYTE	giftReg;					//�������
	int		giftID;						//����ID
	char	giftPlayerName[MAX_PATH];	//������������

	BYTE	up2ndpassword;				//�޸Ĳֿ����

	BYTE	bAutoOpenItemBox;			//�Զ��򿪵��ߺ���

	int     nClanID[10];				//����ID  һ��10��

	BYTE	nSetCPUCore;				//����CPU����,�����࿪Ĭ�Ͽ��� �����������򲻿���

	BYTE	nJojnClan;					//�Ƿ�������

	BYTE	nSetminiWindows;			//�Ƿ�����С����

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

//ȫ�ֹ�����
typedef struct GLOBAL_FUNCTION
{
	BYTE AutoJoinAuctionEvent;				//�Զ��μ�������
	
	


	inline GLOBAL_FUNCTION()
	{
		RtlZeroMemory(this,sizeof(GLOBAL_FUNCTION));
	}
}TAG_GLOBAL_FUNCTION,* PTAG_GLOBAL_FUNCTION;




enum class LOGIN_NOTICE_CODE 
{
	NOTICE_GAME_INIT_SUCCESS                 = 1000,								//��ʼ���ɹ�
	NOTICE_GAME_AUTHREPLE_SUCCESS            = 1001,							    //��½�ɹ�
	NOTICE_GAME_AUTHREPLE_ERROR_U1           = 1002,							    //δ֪����
	NOTICE_GAME_AUTHREPLE_ERROR_R2           = 1003,							    //���������˺Ų����ڡ�
	NOTICE_GAME_AUTHREPLE_ERROR_R3           = 1004,							    //���˺��ѱ���ͣ���������¼������ѯ��
	NOTICE_GAME_AUTHREPLE_ERROR_R4           = 1005,							    //������ά���У��޷���¼��Ϸ��
	NOTICE_GAME_AUTHREPLE_ERROR_R5           = 1006,							    //����IP�ѱ����Σ�����ϵ�ͷ�����
	NOTICE_GAME_AUTHREPLE_ERROR_R6           = 1007,							    //��¼����Ƶ�������Ժ�����
	NOTICE_GAME_AUTHREPLE_ERROR_R7           = 1008,							    //����������ʧ�ܣ����Ժ�����
	NOTICE_GAME_AUTHREPLE_ERROR_CAPT         = 1009,							    //��֤���������
	NOTICE_GAME_ROOM_TIMEROUT                = 1010,							    //���䳬ʱ
	NOTICE_GAME_SERVER_TIMEROUT              = 1011,							    //��������ʱ
	NOTICE_APP_HEART_TIMEROUT                = 1012,							    //���ͨѶ��ʱ
	NOTICE_GAME_AC_ERROR_STATUS              = 1013,							    //�˺�״̬�쳣...�кܶ�ԭ��
	NOTICE_NEXON_REPLE_ERROR                 = 1014,							    //��������������
	NOTICE_GAME_AUTHREPLE_ERROR_CAPT_COUNT   = 1015,				                //��֤�����ƴ�
	NOTICE_GAME_REPLY_INVALID_CLIENT_VERSION = 1016,			                    //�ͻ��˰汾��һ��
	NOTICE_GAME_NGS_INIT_ERROR               = 1017,							    //NGS��ʼ������
	NOTICE_GAME_AUTHREPLE_ERROR_R8           = 1018,						        //��ҷ�ͣ
	NOTICE_GAME_AUTHREPLE_ERROR_UNK_STR      = 1019,					            //δ֪�����ʽ��
	NOTICE_GAME_AUTHREPLE_ERROR_R9           = 1020,						        //���˺���������¼����ͨ��[����˺Źܼ�APP]���н���
	NOTICE_GAME_PP_NOT_PASS                  = 1021,								//���һ�ϵͳδ��⵽PASS
	NOTICE_GAME_AUTHREPLE_ERROR_R11          = 1022,						        //ʵ����֤
	NOTICE_GAME_NGS_PASS_INIT_ERROR          = 1023,						        //NGS���������ʧ��
	NOTICE_NEXON_REPLE_INVALIDNAME           = 1024,						        //�����ID		
	NOTICE_NEXON_REPLE_INVALID_CHAR			 = 1025,						        //�Ƿ��ַ�.						---�� 2021-09-11 16:03:59
	NOTICE_NEXON_REPLE_UNCODE				 = 1026,						        //��֤δ֪����
	NOTICE_NEXON_REPLE_SUCCEED				 = 1027,						        //��֤�ɹ�

	NOTICE_GAME_AUTHREPLE_ERROR_R12			 = 1028,						        //�˺Ž���NGS������.
	NOTICE_TER_CONNECT_ER					 = 1029,						        //����Teriri������ʧ��
	NOTICE_TER_CONNECT_SUCCEED				 = 1030,						        //����Teriri�������ɹ�
	NOTICE_NEXON_REPLE_SYSERROR				 = 1031,							    //��֤ϵͳ����
	NOTICE_CPU_SET							 = 1032,							    //CPU����
	
	NOTICE_NEXON_CREATEOK = 1033,						        //�û������ɹ�

};

typedef struct GAME_AC_LOGIN {

	ULONG  ID;	
	std::string GUID;
	std::string CmdLine;				//����������
	DWORD  dwProceeExitCode;		    //�����˳���
	HANDLE hLoginEvent;	                //��½�¼����
	HANDLE hProcess;					//���̾��
	DWORD  dwProcessID;					//����ID
	ULONG64  ulTimeStamp;				//����ʱ��
	
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



//������pid user
typedef struct _LocaProcessInfo
{
	PVOID pSession;				//�����session
	PVOID pUserData;			//����Ĺ����û�����
} LocaProcessInfo, * PLocaProcessInfo;


enum class MSG_ID
{
	MSG_STOPLOGIN_OUTNUMBER,			//��¼ֹͣ,��¼����ָ������
	MSG_START_GAME,						//��ʼ������Ϸ
};

//��Ϸ�˺�״̬
typedef struct GAME_USER_STATUS
{
	USER_STATUS		Index;
	std::string		Text;
}TAG_GAME_USER_STATUS, * PTAG_GAME_USER_STATUS;

//��Ϸ����״̬
typedef struct GAME_TASK_SRC
{
	TASK_MODE		Mode;
	std::string		Text;
}TAG_GAME_TASK_SRC, * PTAG_GAME_TASK_SRC;





//�����н���ת��json->
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

//�˺�����ת��json
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

// ������֪ͨ����
enum class AuctionNotice
{
	BuyItem_Succeed,			            //����ɹ�
	Integral_Lack,				            //����ǰ,���ֲ���
	Integral_Lack_SV,			            //������֪ͨ,������ֲ���
	Mismatching_EnableEnchant,	            //ǿ����ƥ��
	Mismatching_Paint_BlackList,	        //���᲻ƥ��,�ں�������
	Mismatching_Item_BlackList,	            //������ƥ��
};



typedef struct AUTCION_SPECIAL_EVENTS_ITEMS
{
	int		nSign;							//��ʶ
	int		nTimeStamp;						//������ʱ���
	short	nItemID;						//��ƷID
	int		nStartTimeStamp;				//��ʼʱ��
	BYTE	Un_1;							//δ֪
	
	inline AUTCION_SPECIAL_EVENTS_ITEMS()
	{
		nSign           = 0;
		nTimeStamp      = 0;
		nItemID         = 0;
		nStartTimeStamp = 0;
		Un_1            = 0;
	}

	
}TAG_AUTCION_SPECIAL_EVENTS_ITEMS,* PTAG_AUTCION_SPECIAL_EVENTS_ITEMS;
// �������ر�
typedef struct AUTCION_SPECIAL_EVENTS
{
	BYTE v1;
	BYTE v2;
	BYTE v3;
	std::string Title;		//����-UTF8
	std::string Content;	//����
	BYTE Count;				//����
	std::map<ULONG, TAG_AUTCION_SPECIAL_EVENTS_ITEMS> MapItems;

}TAG_AUTCION_SPECIAL_EVENTS, * PTAG_AUTCION_SPECIAL_EVENTS;