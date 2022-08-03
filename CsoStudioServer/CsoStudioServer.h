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
	
	// @右键菜单初始化
	bool initMenu();


	// @刷新任务界面
	void RefreshAutionTaskView();
	
	// @重载事件
	virtual  bool nativeEvent(const QByteArray& eventType, void* message, long* result);
	
	// @修改用户信息显示
	void modifyingUserInfoView(PTAG_GAME_AC_LOGIN AcInfo);

	// @用户账号界面刷新
	void RefreshAcView();

	QtRCFServer* GetRCF();
	
	CLoginModule& GetLogin();
	
	//覆写closeEvent函数
	void closeEvent(QCloseEvent*);
	
    //槽函数
public slots:
	// @自定义信息输出
    static void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    // @客户端日志输出
    void debugLogApped(int type, const QString& data);

	// @导入用户信息
	void slImpUserInfo(bool checked);	

	// @导出用户信息
	void slExportUserInfo(bool checked);	

	// @保存用户配置信息
	void slSaveUserCfgInfo(bool checked);

	// @加载用户配置信息
	void slLoadUserCfgInfo(bool checked);
	
	// @删除用户配置信息
	void slDelUserCfgInfo(bool checked);
	
	// @添加显示用户信息
	void slAddShowGameUserInfo(PTAG_GAME_AC_LOGIN AcInfo);

	
	// @批量启动游戏
	void slStartGame(bool checked);

	// @右键菜单（拍卖行）
	void slRightClickAuctionMmenu(const QPoint pos);
	// @右键子菜单（拍卖行任务）
	void slPopMenuAutionTask(QAction* act);
	
	// @添加拍卖行任务
	void slAddAutionTask(PTAG_GAME_TASK_AUCTION AuctionTask);

	// @双击拍卖任务
	void slDoubleClickAuction(const QModelIndex& index);

	// @用户界面刷新
	void slRefreshUserView(qint64 qID);
	
	// @拍卖行界面刷新
	void slRefreshAuctionView(qint64 qID);
	
	// @任务派发
	void slPopMenuDispatchTask(QAction* act);

	// @删除任务派发账号
	void slDeleteDispatchAccount(QAction* act);

	// @保存拍卖会配置
	void slSavaAuctionCFG(bool checked);
	
	// @读取拍卖会配置
	void slReadAuctionCFG(bool checked);

	// @删除拍卖会配置
	void slDeleteAuctionCFG(bool checked);

	// @刷新拍卖会界面
	void slRefreshAuctionView();

	// @打开武器item文件
	void slOpenWeaponItemFile(bool checked);

	// @打开彩漆item文件
	void slOpenPaintItemFile(bool checked);
	
	// @右键菜单（账号）
	void slRightClickAccountMmenu(const QPoint pos);

	// @右键子菜单（账号）
	void slPopMenuAccount(QAction* act);	

	// @双击账号信息
	void slDoubleClickAccount(const QModelIndex& index);

	// @批量修改大区
	void slPopMenuBatchReg(QAction* act);
	
	// @批量修改任务
	void slPopMenuBatchTask(QAction* act);

	// @特别活动-拍卖会
	void slAutoAuction1000M(bool checked);
	
	// @设置游戏目录
	void slSetGameDir(bool checked);

	// @登陆
	void slLogin(bool checked);

	// @刷新拍卖会上架界面
	void slRefreshAuctionOnSaleView();

signals:
	// @输出调试日志
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
	// @写出csv.拍卖会价格表
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


	//当前选中的任务
	PTAG_GAME_TASK_AUCTION                  m_CurSelectedTask;
	//选中的任务索引
	std::vector<int>                        m_VecSelectedTaskIndex;
	std::vector<int>                        m_VecSelectedAccountIndex;
	std::list<PTAG_GAME_AC_LOGIN>			m_UserList;
	QtRCFServer*	                        m_RCF;
	HttpManage*                             m_Http;
};
