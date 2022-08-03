#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget*parent)
	: QLineEdit(parent)
{
	m_pCallBack = nullptr;
}

MyLineEdit::~MyLineEdit()
{
	
}

void MyLineEdit::leaveEvent(QEvent* e)
{
	if (this->objectName() == "lineEdit_ItemName")
	{
		m_pCallBack(this);

	}
}

void MyLineEdit::setMouseleaveCallBack(MouseleaveCallBack pCallBack)
{
	m_pCallBack = pCallBack;
}

