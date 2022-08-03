#pragma once
#include "QtConnect.h"
#include "ui_CsoStudioServer.h"
#include "QtAution.h"
#include "QtRCFServer.h"
#include "nlohmann/json.hpp"
#include "nlohmann/fifo_map.hpp"
#include <fstream>
#include "QtAccount.h"
#include "HttpManage.h"
#include "Config.h"
class CsoStudioServer : public QWidget
{
    Q_OBJECT
public:
    CsoStudioServer(QWidget *parent = Q_NULLPTR);
    static CsoStudioServer* GetInstance();
	
	bool configInit();
    bool uiInit();
    bool init();
	
	// @�Ҽ��˵���ʼ��
	bool initMenu();


	// @ˢ���������
	void RefreshAutionTaskView();
	
	// @�����¼�
	virtual  bool nativeEvent(const QByteArray& eventType, void* message, long* result);
	
	// @�޸��û���Ϣ��ʾ
	void modifyingUserInfoView(PTAG_GAME_AC_LOGIN AcInfo);

	// @�û��˺Ž���ˢ��
	void RefreshAcView();

	QtRCFServer* GetRCF();
	
	CLoginModule& GetLogin();
	
	//��дcloseEvent����
	void closeEvent(QCloseEvent*);
	
    //�ۺ���
public slots:
	// @�Զ�����Ϣ���
    static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    // @�ͻ�����־���
    void debugLogApped(int type, const QString& data);

	// @�����û���Ϣ
	void slImpUserInfo(bool checked);	

	// @�����û���Ϣ
	void slExportUserInfo(bool checked);	

	// @�����û�������Ϣ
	void slSaveUserCfgInfo(bool checked);

	// @�����û�������Ϣ
	void slLoadUserCfgInfo(bool checked);
	
	// @ɾ���û�������Ϣ
	void slDelUserCfgInfo(bool checked);
	
	// @�����ʾ�û���Ϣ
	void slAddShowGameUserInfo(PTAG_GAME_AC_LOGIN AcInfo);

	
	// @����������Ϸ
	void slStartGame(bool checked);

	// @�Ҽ��˵��������У�
	void slRightClickAuctionMmenu(const QPoint pos);
	// @�Ҽ��Ӳ˵�������������
	void slPopMenuAutionTask(QAction* act);
	
	// @�������������
	void slAddAutionTask(PTAG_GAME_TASK_AUCTION AuctionTask);

	// @˫����������
	void slDoubleClickAuction(const QModelIndex& index);

	// @�û�����ˢ��
	void slRefreshUserView(qint64 qID);
	
	// @�����н���ˢ��
	void slRefreshAuctionView(qint64 qID);
	
	// @�����ɷ�
	void slPopMenuDispatchTask(QAction* act);

	// @ɾ�������ɷ��˺�
	void slDeleteDispatchAccount(QAction* act);

	// @��������������
	void slSavaAuctionCFG(bool checked);
	
	// @��ȡ����������
	void slReadAuctionCFG(bool checked);

	// @ɾ������������
	void slDeleteAuctionCFG(bool checked);

	// @ˢ�����������
	void slRefreshAuctionView();

	// @������item�ļ�
	void slOpenWeaponItemFile(bool checked);

	// @�򿪲���item�ļ�
	void slOpenPaintItemFile(bool checked);
	
	// @�Ҽ��˵����˺ţ�
	void slRightClickAccountMmenu(const QPoint pos);

	// @�Ҽ��Ӳ˵����˺ţ�
	void slPopMenuAccount(QAction* act);	

	// @˫���˺���Ϣ
	void slDoubleClickAccount(const QModelIndex& index);

	// @�����޸Ĵ���
	void slPopMenuBatchReg(QAction* act);
	
	// @�����޸�����
	void slPopMenuBatchTask(QAction* act);

	// @�ر�-������
	void slAutoAuction1000M(bool checked);
	
	// @������ϷĿ¼
	void slSetGameDir(bool checked);

	// @��½
	void slLogin(bool checked);

	// @ˢ���������ϼܽ���
	void slRefreshAuctionOnSaleView();

signals:
	// @���������־
	void sig_DebugInfo(qint64 type, QString str);	
	void sig_AddShowUserInfo(PTAG_GAME_AC_LOGIN AcInfo);
	void sig_RefreshUserUI(qint64 qID);
	void sig_RefreshAuctionUI(qint64 qID);
	void sig_RefreshAuctionTaskUI();
private:
	void popMenuCreateAutionDlg();
	void popMenuChangeAution();
	void popMenuDelAution();

	void popMenuCreateAccountDlg();	
	void popMenuChangeAccountDlg();
	void popMenuDelAccountDlg();
	void popMenuEndProcessAccountDlg();
	void popMenuGetAuctionMetadata();
	// @д��csv.������۸��
	void slWriteAuctionPriceCSV(std::vector<BYTE> & VecData,BYTE GameReg);
private:
    Ui::CsoStudioServerClass ui;
    CLoginModule                   m_Login;
    static inline CsoStudioServer* m_pInstance = nullptr;
	QMenu* m_pMainAutionMenu ;
	QMenu* m_pChildMenuAddDispatchTask;
	QMenu* m_pChildMenuDelDispatchTask;
	QtAution* m_pDlgAution;
	QtAccount* m_pDlgAccount;
	QMenu* m_pMainAccountMenu;
	bool m_bIsLogin;


	//��ǰѡ�е�����
	PTAG_GAME_TASK_AUCTION                  m_CurSelectedTask;
	//ѡ�е���������
	std::vector<int>                        m_VecSelectedTaskIndex;
	std::vector<int>                        m_VecSelectedAccountIndex;
	std::list<PTAG_GAME_AC_LOGIN>			m_UserList;
	QtRCFServer*	                        m_RCF;
	HttpManage*                             m_Http;
};
