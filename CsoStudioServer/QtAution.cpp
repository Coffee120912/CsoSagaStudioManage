#include "QtAution.h"


QtAution::QtAution(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_pInstance = this;
	init();
}

QtAution::~QtAution()
{
	
}

void QtAution::init()
{
	m_AutionTask             = new TAG_GAME_TASK_AUCTION();
	// 彩漆默认数量是 1
	m_AutionTask->PaintCount = 1;
	//鼠标离开回调
	ui.lineEdit_ItemName->setMouseleaveCallBack(lineEditMouseleave);


	auto PaintMap = g_Util.GetPaintMap();
	for (auto it = PaintMap.begin(); it != PaintMap.end(); it++)
	{
		ui.comboBox_PaintID_1->insertItem(it->first, it->second.c_str());
		ui.comboBox_PaintID_2->insertItem(it->first, it->second.c_str());
		ui.comboBox_PaintID_3->insertItem(it->first, it->second.c_str());
		ui.comboBox_PaintID_4->insertItem(it->first, it->second.c_str());
		ui.comboBox_PaintID_5->insertItem(it->first, it->second.c_str());
		ui.comboBox_PaintID_6->insertItem(it->first, it->second.c_str());
	}

	omboBox_PaintID_View(QString::number(ui.comboBox_PaintCount->currentIndex() + 1));
	connect(ui.comboBox_PaintCount, &QComboBox::currentTextChanged, this, &QtAution::omboBox_PaintID_View);

	//重载
	connect(ui.comboBox_TaskType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slTaskType);
	connect(ui.comboBox_PaintType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintType);
	connect(ui.comboBox_PaintCount, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintCount);
	connect(ui.comboBox_PaintID_1, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	connect(ui.comboBox_PaintID_2, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	connect(ui.comboBox_PaintID_3, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	connect(ui.comboBox_PaintID_4, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	connect(ui.comboBox_PaintID_5, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	connect(ui.comboBox_PaintID_6, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slPaintID);
	
	// -- 返回类型 type (形参) > (address)

	connect(ui.lineEdit_EnchantvLV, static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textEdited), this, static_cast<void (QtAution::*)(const QString & Text)>(&QtAution::slEnchantvLV));
	connect(ui.comboBox_EnchantvLV, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, static_cast<void (QtAution::*)(int)>(&QtAution::slEnchantvLV));

	
	
	connect(ui.lineEdit_Price, static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textEdited), this, &QtAution::slPrice);
	connect(ui.lineEdit_BuyCount, static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textEdited), this, &QtAution::slBuyCount);
	connect(ui.comboBox_ItemAttribute, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slItemAttribute);

	connect(ui.comboBox_HitRate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.comboBox_Damage, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.comboBox_Recoil, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.comboBox_Weight, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.comboBox_Bursts, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.comboBox_Clip, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slAttributeID);
	connect(ui.pushButton_Submit, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slSubmit);
	connect(ui.pushButton_Cancel, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slCancel);


	
	connect(ui.pushButton_ClearPanitBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slClearPanitBlackList);
	connect(ui.pushButton_ReadPanitBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slReadPanitBlackList);
	connect(ui.pushButton_SavaPaintBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slSavaPaintBlackList);

	connect(ui.pushButton_ClearItemBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slClearItemBlackList);
	connect(ui.pushButton_ReadItemBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slReadItemBlackList);
	connect(ui.pushButton_SavaItemBlackList, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), this, &QtAution::slSavaItemBlackList);
	connect(ui.comboBox_FindType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QtAution::slFindTpye);
	


}


void QtAution::showEvent(QShowEvent* event)
{
	switch (m_TypeHandle)
	{
	case 1:
	{	
		this->setWindowTitle("添加拍卖任务");

		m_AutionTask->TaskStatus = 0;

		//clear array
		for (size_t i = 0; i < m_AutionTask->ACIDCount; i++)
		{
			RtlZeroMemory(m_AutionTask->szACArray[i],MAX_PATH);
		}
		m_AutionTask->ACIDCount = 0;
	}

		break;
	case 2:
		this->setWindowTitle("修改拍卖信息");
		break;
	default:
		break;
	}


	
	viewAutionInfo();
}



bool QtAution::event(QEvent* e)
{
	//qDebug() << e;

	if (e->type() == QEvent::GraphicsSceneWheel)
	{
		QMouseEvent* keyEvent = (QMouseEvent*)e;
		
	}

	return QWidget::event(e);
}

void QtAution::viewAutionInfo()
{
	ui.label->setText(QString("任务ID:%1").arg(m_AutionTask->ulTaskID));
	ui.comboBox_TaskType->setCurrentIndex(m_AutionTask->ulTaskType);
	//item_name->item_ID
	ui.lineEdit_ItemName->setText(g_Util.GetItemNameByID(m_AutionTask->ItemID).c_str());
	ui.label_ItemID->setText(QString("武器ID:%1").arg(g_Util.GetItemIDByName(ui.lineEdit_ItemName->text().toUtf8().constData())));
	ui.lineEdit_Price->setText(QString::number(m_AutionTask->Price));
	ui.lineEdit_BuyCount->setText(QString::number(m_AutionTask->BuyCount));
	ui.comboBox_PaintType->setCurrentIndex(m_AutionTask->PaintType);
	ui.comboBox_PaintCount->setCurrentIndex(m_AutionTask->PaintCount - 1 < 0 ? 0 : m_AutionTask->PaintCount - 1);

	std::map<ULONG, std::string>&  Paint = g_Util.GetPaintMap();

	for (auto & iter : Paint)
	{
		if (iter.first == m_AutionTask->PaintShow)
		{
			ui.lineEdit_ItemShowPaint->setText(iter.second.c_str());
		}
	}


	


	auto FindPaintNameRetID = [&](ULONG uItemID) -> ULONG 
	{
		auto PaintMap = g_Util.GetPaintMap();
		
		if (PaintMap.count(uItemID))
		{
			for (size_t i = 0; i < ui.comboBox_PaintID_1->count(); i++)
			{
				if (ui.comboBox_PaintID_1->itemText(i) == PaintMap.at(uItemID).data())
				{
					return i;
				}
			}
		}


		return 0;
	};

	ui.comboBox_PaintID_1->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[0]));
	ui.comboBox_PaintID_2->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[1]));
	ui.comboBox_PaintID_3->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[2]));
	ui.comboBox_PaintID_4->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[3]));
	ui.comboBox_PaintID_5->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[4]));
	ui.comboBox_PaintID_6->setCurrentIndex(FindPaintNameRetID(m_AutionTask->PaintID[5]));

	ui.lineEdit_EnchantvLV->setText(QString::number(m_AutionTask->Enchantlv));
	ui.comboBox_EnchantvLV->setCurrentIndex(m_AutionTask->Enchantlv);
	
	ui.comboBox_ItemAttribute->setCurrentIndex(m_AutionTask->ItemAttribute);
	ui.comboBox_HitRate->setCurrentIndex(m_AutionTask->HitRate);
	ui.comboBox_Damage->setCurrentIndex(m_AutionTask->Damage);
	ui.comboBox_Recoil->setCurrentIndex(m_AutionTask->Recoil);
	ui.comboBox_Weight->setCurrentIndex(m_AutionTask->Weight);
	ui.comboBox_Bursts->setCurrentIndex(m_AutionTask->Bursts);
	ui.comboBox_Clip->setCurrentIndex(m_AutionTask->Clip);
	

	//彩漆黑名单显示到界面上
	ui.textEdit_BlackList_Paint->clear();

	for (size_t i = 0; i < m_AutionTask->BlackListPaint; i++)
	{
		for (auto& iter : g_Util.GetPaintMap())
		{
			if (iter.first == m_AutionTask->BlackListPaintID[i])
			{
				ui.textEdit_BlackList_Paint->append(iter.second.data());
			}
		}
	}

	//武器黑名单显示到界面上
	ui.textEdit_BlackList_Item->clear();

	for (size_t i = 0; i < m_AutionTask->BlackListItem; i++)
	{
		ui.textEdit_BlackList_Item->append(g_Util.GetItemNameByID(m_AutionTask->BlackListItemID[i]).data());
	}
}

void QtAution::omboBox_PaintID_View(const QString& text)
{

	ULONG Count = text.toInt();


	ui.comboBox_PaintID_1->setEnabled(int(Count - 1) >= 0 ? true : false);
	ui.comboBox_PaintID_2->setEnabled(int(Count - 2) >= 0 ? true : false);
	ui.comboBox_PaintID_3->setEnabled(int(Count - 3) >= 0 ? true : false);
	ui.comboBox_PaintID_4->setEnabled(int(Count - 4) >= 0 ? true : false);
	ui.comboBox_PaintID_5->setEnabled(int(Count - 5) >= 0 ? true : false);
	ui.comboBox_PaintID_6->setEnabled(int(Count - 6) >= 0 ? true : false);
	

}

void QtAution::lineEditMouseleave(PVOID pThis)
{
	MyLineEdit* EditThis= (MyLineEdit*)pThis;

	if (EditThis->objectName() == "lineEdit_ItemName")
	{
		QString str = m_pInstance->ui.lineEdit_ItemName->text();
		
		m_pInstance->m_AutionTask->ItemID = g_Util.GetItemIDByName(str.toUtf8().constData());
		
		m_pInstance->ui.label_ItemID->setText(QString("武器ID:%1").arg(m_pInstance->m_AutionTask->ItemID));
	

	}

}

Ui::QtAutionClass& QtAution::GetUI()
{
	return ui;
}

void QtAution::SetTaskID(ULONG uID)
{
	m_AutionTask->ulTaskID = uID;
}

void QtAution::SetTypeHandle(ULONG uType)
{
	m_TypeHandle = uType;
}

void QtAution::SetTaskInfo(PTAG_GAME_TASK_AUCTION pTask)
{
	memcpy(m_AutionTask, pTask,sizeof(TAG_GAME_TASK_AUCTION));
}

void QtAution::slTaskType(int index)
{
	m_AutionTask->ulTaskType = index;
	
}

void QtAution::slPaintType(int index)
{
	m_AutionTask->PaintType = index;
}

void QtAution::slPaintCount(int index)
{
	m_AutionTask->PaintCount = index + 1;
}

void QtAution::slPaintID(int index)
{
	auto PaintMap = g_Util.GetPaintMap();
	
	// 取信号发送者
	QComboBox* pComBox = qobject_cast<QComboBox*>(sender());


	for (auto& iter : PaintMap)
	{
		if (pComBox->objectName() == "comboBox_PaintID_1" && iter.second == ui.comboBox_PaintID_1->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[0] = iter.first;
			break;
		}
		else if (pComBox->objectName() == "comboBox_PaintID_2" && iter.second == ui.comboBox_PaintID_2->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[1] = iter.first;
			break;
		}
		else if (pComBox->objectName() == "comboBox_PaintID_3" && iter.second == ui.comboBox_PaintID_3->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[2] = iter.first;
			break;
		}
		else if (pComBox->objectName() == "comboBox_PaintID_4" && iter.second == ui.comboBox_PaintID_4->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[3] = iter.first;
			break;
		}
		else if (pComBox->objectName() == "comboBox_PaintID_5" && iter.second == ui.comboBox_PaintID_5->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[4] = iter.first;
			break;
		}
		else if (pComBox->objectName() == "comboBox_PaintID_6" && iter.second == ui.comboBox_PaintID_6->itemText(index).toUtf8().constData())
		{
			m_AutionTask->PaintID[5] = iter.first;
			break;
		}
		
	}


}

void QtAution::slEnchantvLV(const QString& Text)
{
	m_AutionTask->Enchantlv = Text.toInt();
}

void QtAution::slEnchantvLV(int index)
{
	m_AutionTask->Enchantlv = index;
}

void QtAution::slFindTpye(int index)
{
	m_AutionTask->FindType = index;
}

void QtAution::slPrice(const QString& Text)
{
	m_AutionTask->Price = Text.toInt();
}

void QtAution::slBuyCount(const QString& Text)
{
	m_AutionTask->BuyCount = Text.toInt();
}

void QtAution::slItemAttribute(int index)
{
	m_AutionTask->ItemAttribute = index;
}

void QtAution::slAttributeID(int index)
{
	// 取信号发送者
	QComboBox* pComBox = qobject_cast<QComboBox*>(sender());

	if (pComBox->objectName() == "comboBox_HitRate")
	{
		m_AutionTask->HitRate = index;
	}
	else if (pComBox->objectName() == "comboBox_Damage")
	{
		m_AutionTask->Damage = index;
	}
	else if (pComBox->objectName() == "comboBox_Recoil")
	{
		m_AutionTask->Recoil = index;
	}
	else if (pComBox->objectName() == "comboBox_Weight")
	{
		m_AutionTask->Weight = index;
	}
	else if (pComBox->objectName() == "comboBox_Bursts")
	{
		m_AutionTask->Bursts = index;
	}
	else if (pComBox->objectName() == "comboBox_Clip")
	{
		m_AutionTask->Clip = index;
	}


}

void QtAution::slSubmit(bool checked)
{
	if (ui.lineEdit_ItemName->text().isEmpty() != true)
	{
		//判断名称是否存在于道具列表里
		if (g_Util.GetItemIDByName(ui.lineEdit_ItemName->text().toUtf8().constData()) == 0)
		{
			QMessageBox::warning(this, "错误", "该武器名不存在,请[查看武器列表]");
			return;
		}

	}

	//搜索的彩漆判断
	if (ui.lineEdit_ItemShowPaint->text().isEmpty() != true)
	{
		BOOL bStatus = FALSE;
		
		for (auto& iter : g_Util.GetPaintMap())
		{
			if (iter.second == ui.lineEdit_ItemShowPaint->text().toStdString())
			{
				m_AutionTask->PaintShow = iter.first;
				bStatus = TRUE;
				break;
			}
		}
		if (!bStatus)
		{
			QMessageBox::warning(this, "错误", "该彩漆不存在,请[查看彩漆列表]");
			return;
		}
	}



	

	


	//彩漆黑名单列表判断
	QString BlackListPaint = ui.textEdit_BlackList_Paint->toPlainText();

	QStringList PaintList = BlackListPaint.split('\n');
	
	std::queue <short> PaintIDList;

	if (PaintList.size() != 0)
	{
		if (PaintList.at(0).isEmpty() == false)
		{
			for (size_t i = 0; i < PaintList.size(); i++)
			{
				bool bExist = false;
				for (auto& iter : g_Util.GetPaintMap())
				{
					if (iter.second == PaintList.at(i).toUtf8().constData())
					{
						bExist = true;
						PaintIDList.push(iter.first);
						break;
					}
				}
				if (!bExist)
				{					
					QMessageBox::warning(this, "错误", fmt::format("彩漆黑名单中的{},并不存在！", PaintList.at(i).toUtf8().constData()).data());
					return;
				}
			}
			//添加到任务里
			m_AutionTask->BlackListPaint = PaintIDList.size();
			for (size_t i = 0; i < m_AutionTask->BlackListPaint; i++, PaintIDList.pop())
			{
				m_AutionTask->BlackListPaintID[i] = PaintIDList.front();
			}
		}
	}

	//武器黑名单列表判断
	QString BlackListWeapon = ui.textEdit_BlackList_Item->toPlainText();
	QStringList ItemList    = BlackListWeapon.split('\n');

	std::queue <short> ItemIDList;

	if (ItemList.size() != 0)
	{
		if (ItemList.at(0).isEmpty() == false)
		{
			for (size_t i = 0; i < ItemList.size(); i++)
			{
				bool bExist = false;
				int nItemID = g_Util.GetItemIDByName(ItemList.at(i).toUtf8().constData());
				if (nItemID != -1)
				{
					bExist = true;
					ItemIDList.push(nItemID);
					continue;;
				}

				if (!bExist)
				{
					QMessageBox::warning(this, "错误", fmt::format("武器黑名单中的{},并不存在！", ItemList.at(i).toUtf8().constData()).data());
					return;
				}
			}
			//添加到任务里
			m_AutionTask->BlackListItem = ItemIDList.size();
			for (size_t i = 0; i < m_AutionTask->BlackListItem; i++, ItemIDList.pop())
			{
				m_AutionTask->BlackListItemID[i] = ItemIDList.front();
			}
		}
	}




	emit  sigAddAutionTask(m_AutionTask);
	this->close();
}

void QtAution::slCancel(bool checked)
{
	this->close();
}


void QtAution::slClearPanitBlackList(bool checked)
{
	//先清空任务里的黑名单
	for (size_t i = 0; i < m_AutionTask->BlackListPaint; i++)
	{
		m_AutionTask->BlackListPaintID[i] = 0;
	}
	m_AutionTask->BlackListPaint = 0;
	
	//清空界面上的黑名单
	ui.textEdit_BlackList_Paint->clear();
}

void QtAution::slReadPanitBlackList(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"请选择要导入的彩漆黑名单",
		"",
		"文本文件(*txt)");

	if (fileName.isEmpty())
	{
		//未选择导入的文件
		return;
	}
	qDebug() << "读取彩漆黑名单文件,路径:" << fileName;

	QFile file;
	file.setFileName(fileName);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "警告", "打开文件失败!请检查是否有打开的权限！");
		return;
	}

	QString str;
	int nImpCount = 0;
	while (!file.atEnd())
	{
		char cBuf[1024];
		file.readLine(cBuf, sizeof(cBuf));
		str.append(cBuf);
		nImpCount++;
	}
	file.close();

	if (nImpCount)
	{
		qInfo() << "[读取彩漆黑名单]一共读取:" << nImpCount << "个彩漆";
		ui.textEdit_BlackList_Paint->setText(str);
	}
	
}

void QtAution::slSavaPaintBlackList(bool checked)
{
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this,
		"请选择要保存的路径",
		"", 
		"文本文件(*txt)"
	);

	if (!fileName.isNull())
	{

		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("打开文件错误"), "");
		}
		QString str;
		str = ui.textEdit_BlackList_Paint->toPlainText();
		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		stream << str;
		file.close();
	
	}

}

void QtAution::slClearItemBlackList(bool checked)
{
	//先清空任务里的黑名单
	for (size_t i = 0; i < m_AutionTask->BlackListItem; i++)
	{
		m_AutionTask->BlackListItemID[i] = 0;
	}
	m_AutionTask->BlackListItem = 0;
	//清空界面上的黑名单
	ui.textEdit_BlackList_Item->clear();
}

void QtAution::slReadItemBlackList(bool checked)
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"请选择要导入的武器黑名单",
		"",
		"文本文件(*txt)");

	if (fileName.isEmpty())
	{
		//未选择导入的文件
		return;
	}
	qDebug() << "读取武器黑名单文件,路径:" << fileName;

	QFile file;
	file.setFileName(fileName);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "警告", "打开文件失败!请检查是否有打开的权限！");
		return;
	}

	QString str;
	int nImpCount = 0;
	while (!file.atEnd())
	{
		char cBuf[1024];
		file.readLine(cBuf, sizeof(cBuf));
		str.append(cBuf);
		nImpCount++;
	}
	file.close();

	if (nImpCount)
	{
		qInfo() << "[读取武器黑名单]一共读取:" << nImpCount << "个武器";
		ui.textEdit_BlackList_Item->setText(str);
	}
}

void QtAution::slSavaItemBlackList(bool checked)
{
	QString fileName;
	fileName = QFileDialog::getSaveFileName(this,
		"请选择要保存的路径",
		"",
		"文本文件(*txt)"
	);

	if (!fileName.isNull())
	{

		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("打开文件错误"), "");
		}
		QString str;
		str = ui.textEdit_BlackList_Item->toPlainText();
		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		stream << str;
		file.close();
	}
}

