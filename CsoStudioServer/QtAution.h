#pragma once
#include "QtConnect.h"
#include <QWidget>
#include "ui_QtAution.h"
#include "MyLineEdit.h"
class QtAution : public QWidget
{
	Q_OBJECT
public:
	QtAution(QWidget *parent = nullptr);
	~QtAution();
	void init();
	virtual void showEvent(QShowEvent* event);
	virtual bool event(QEvent* e);
	
	void viewAutionInfo();

	void omboBox_PaintID_View(const QString& text);
	
	static void lineEditMouseleave(PVOID pThis);
	
	Ui::QtAutionClass& GetUI();
	
	// @设置任务ID
	void SetTaskID(ULONG uID);
	// @设置类型
	void SetTypeHandle(ULONG uType);
	// @设置任务信息
	void SetTaskInfo(PTAG_GAME_TASK_AUCTION pTask);
public slots:
	void slTaskType(int index);
	void slPaintType(int index);
	void slPaintCount(int index);
	void slPaintID(int index);
	void slEnchantvLV(const QString& Text);
	void slPrice(const QString& Text);
	void slBuyCount(const QString& Text);
	void slItemAttribute(int index);
	void slAttributeID(int index);
	void slSubmit(bool checked);
	void slCancel(bool checked);
	
	void slClearPanitBlackList(bool checked);
	void slReadPanitBlackList(bool checked);
	void slSavaPaintBlackList(bool checked);
	void slClearItemBlackList(bool checked);
	void slReadItemBlackList(bool checked);
	void slSavaItemBlackList(bool checked);
	void slEnchantvLV(int index);
	void slFindTpye(int index);
signals:
	void sigAddAutionTask(PTAG_GAME_TASK_AUCTION AuctionTask);
private:
	Ui::QtAutionClass ui;
	PTAG_GAME_TASK_AUCTION  m_AutionTask;
	static inline QtAution* m_pInstance = nullptr;
	// 操作类型
	ULONG                   m_TypeHandle;
};
