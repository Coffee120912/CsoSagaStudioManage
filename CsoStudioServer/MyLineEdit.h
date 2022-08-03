#pragma once
#include "QtConnect.h"
#include <QObject>
#include <QLineEdit>


typedef void (*MouseleaveCallBack)(PVOID pThis);
class MyLineEdit  : public QLineEdit
{
	Q_OBJECT

public:
	MyLineEdit(QWidget*parent);
	~MyLineEdit();

	virtual void leaveEvent(QEvent* e); 
	
	void setMouseleaveCallBack(MouseleaveCallBack pCallBack);
	
private:
	MouseleaveCallBack m_pCallBack;
};
