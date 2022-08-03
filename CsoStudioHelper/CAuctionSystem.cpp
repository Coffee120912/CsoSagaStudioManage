#include "CAuctionSystem.h"
#include "CVars.h"


CAuctionSystem::CAuctionSystem()
	:m_BuyAutionID(0)
{

	m_bRefStatus  = TRUE;
	m_BuyTaskID   = 0;
	m_pACEUtil    = CACEInterface::GetInstance()->GetUtil();
	m_AuctionFlag = 0;
}

CAuctionSystem::~CAuctionSystem()
{

}



void CAuctionSystem::Init()
{
	// ��ȡ������������Ϣ
	m_AuctionFlag =  CGameRCF::GetInstance()->GetShareAuctionEvents();
}

bool CAuctionSystem::WeaponAuctionEvent(std::shared_ptr<PackReader> lPackRead)
{
	ULONG Index = lPackRead->ReadChar();

	ACEDebugLog("[{}] Index:{}", __FUNCTION__, Index);
	
	g_pUtil->ViewHexCode(lPackRead);

	// ������1 �����2 � 3
	int nItemType = 0;
	
	if (Index != 0x10 && Index != 0xF)
	{
		nItemType = lPackRead->ReadChar();
	}

	switch (Index)
	{
	case 1:
	{
		if (nItemType == 3)
		{
			// - ���
			int unFlag = lPackRead->ReadChar();
		}
		else if (nItemType == 2)
		{
			//�����ʱ������
			return true;
		}

		// ʣ��ʱ��-����
		short OtherMinute  = lPackRead->ReadShort();
		// ��ʾ����
		int ShowType       = lPackRead->ReadChar();
		int nOffset        = lPackRead->GetIndex();
		// Item���� 1:���� 2:��� 3:�
		BYTE ItemType      = 0;


		if (ShowType & 0x1)
		{
			ItemType = lPackRead->ReadChar();
			
			if (ItemType == 1)
			{
				this->ParseAuctionView(lPackRead);
			}			
		}
		if (ShowType & 0x2)
		{
			ItemType = lPackRead->ReadChar();
			
			if (ItemType == 1)
			{
				this->ParseAuctionView(lPackRead);
			}
			else if (ItemType == 3)
			{
				ULONG ItemCount = lPackRead->ReadChar();

				for (size_t i = 0; i < ItemCount; i++)
				{
					this->ParseAuctionViewData(lPackRead, m_SpecialItems, 0);
					
					// ���-��������ʱ��
					if (m_SpecialItems.AuctionID && this->IsJoinAuctionSpecialEvents())
					{
						//û���жϵ�ǰ�Ƿ��Ѿ������˴���Ʒ.�ް�
						if (auto Items = this->GetCanTimeActive())
						{
							CGamePack::GetInstance()->AuctionEventsBidding(Items->nSign, Items->nTimeStamp, 1000);
							
							// ����break.��Ҫ������
							//break;
						}
					}

				}

			}
		}
		if (ShowType & 4)
		{
			this->ParseAuctionViewData(lPackRead, m_SpecialItems, 1);

			if ( m_SpecialItems.AuctionID && m_SpecialItems.RemainTime == 0)
			{
				
				ACEDebugLog("[{}]ȡ��item_id:{}", __FUNCTION__, m_SpecialItems.ItemID);
				//ȡ�ص���
				CGamePack::GetInstance()->AuctionEventsGetBack(m_SpecialItems.AuctionID, m_SpecialItems.AddedTimer);
			}
		}
		if (ShowType & 8)	//�ϼܵ�
		{
			this->ParseAuctionSellItem(lPackRead);
			
			this->NotifySellWeapon(0xFF);
			
			//int SavaIndex = lPackRead->GetIndex();
			//ShowType &= ~(1 << 3) & 0xF;
			//lPackRead->SetIndex(nOffset);
			//lPackRead->WriteBYTE(ShowType);
			//lPackRead->SetIndex(SavaIndex);
			
		}
		ACEDebugLog("[{}] view_type:{}", __FUNCTION__, ShowType);

	}
	break;
	case 0xC:
	{
		ParseAuctionNotice(lPackRead);
	}
	break;
	case 0x3:
	{
		ParseAuctionRefreshSpecifyItem(lPackRead);
	}
	break;
	case 0x10:
	{
		BYTE MsgType = lPackRead->ReadChar();
		

		switch (MsgType)
		{
		case 0x5:	//���ֲ���-���������
		{
			ACEDebugLog("[{}] ���ֲ��� ����ʧ��:{}", __FUNCTION__, m_BuyTaskID);
			CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Integral_Lack_SV, m_BuyTaskID,nullptr);
		}
			break;
		case 0x10:	//�������Ϣ
		case 0x19:	//ָ������ˢ��̫��
		case 0x21:	//δ�ҵ����ϵ�����
		case 0x7:	//err 07
			return false;
		default:
			ACEDebugLog("[{}] ����ID:{}", __FUNCTION__, MsgType);
			break;
		}
		
	}
	break;
	case 0xE:		//�����������ϼ���Ϣ
	{
		if (nItemType == 1)
		{
			TAG_GAME_AUCTION Items = {};

			this->ParseAuctionViewData(lPackRead, Items, 1);

			if (Items.AuctionID && Items.RemainTime == 0)
			{
				if (Items.Earnings)
				{
					// ȡ�ػ���
					CGamePack::GetInstance()->AuctionGetBackIntegrate(Items.AuctionID, Items.AddedTimer);
				}
				else
				{
					//ȡ������
					CGamePack::GetInstance()->AuctionGetBackWeapon(Items.AuctionID, Items.AddedTimer);
				}

			}
			m_RackingItems[Items.Row] = Items;
			
			lPackRead->ReadInt();

			this->NotifySellWeapon(Items.Row);
		}
	}
	break;
	case 0x2:
	{
		
	}
	break;

	case 0xF:
	{
		//���η��������ڼ�Ĵ�����ʾ
		return false;
	}
	default:
		
		break;
	}
	return true;
}

void CAuctionSystem::ParseAuctionView(std::shared_ptr<PackReader> lPackRead)
{
	//��ǰ��Ʒ����
	ULONG ItemCount = lPackRead->ReadChar();

	for (size_t i = 0; i < ItemCount; i++)
	{
		ParseAuctionLine(lPackRead);
		// ������Ϣ
		m_AuctionMap[m_AuctionInfo.Row] = m_AuctionInfo;

		this->RunTask(&m_AuctionInfo);
	}
	if (CGameRCF::GetInstance()->GetTaskMode()== TASK_MODE::GAME_TASK_AUCTION)
	{
		//����ˢ�½����߳�
		if (m_WorkThread.get_tasks_running() == 0 && m_WorkThread.get_tasks_queued() == 0)
		{
			if (ItemCount == 7 && m_AuctionInfo.Row == 13)
			{
				m_WorkThread.push_task(std::bind(&CAuctionSystem::ThreadRef, this));
			}
		}
	}

}

TAG_GAME_AUCTION& CAuctionSystem::ParseAuctionLine(std::shared_ptr<PackReader> lPackRead )
{
	return ParseAuctionViewData(lPackRead,m_AuctionInfo,0);
}


TAG_GAME_AUCTION& CAuctionSystem::ParseAuctionViewData(std::shared_ptr<PackReader> lPackRead, TAG_GAME_AUCTION& Data, int Type)
{
	//�ڼ���
	BYTE Row            = lPackRead->ReadChar();
	//����ID
	int nAuctionID      = lPackRead->ReadInt();
	//�ϼܵ�ʱ���
	int nAddedTimestamp = lPackRead->ReadInt();
	//��ƷID
	short nItemID       = lPackRead->ReadShort();
	//δ֪1
	BYTE nItemUn1       = lPackRead->ReadChar();
	//δ֪2
	BYTE nItemUn2       = lPackRead->ReadChar();
	//����۸�
	int nBidPrice       = lPackRead->ReadInt();
	//һ�ڼ�
	int nPrice          = lPackRead->ReadInt();
	//ʣ��ʱ��
	int nRemainTime     = lPackRead->ReadInt();
	//��ǰ��ʾ�Ĳ���ID->������Ա��ֶ�����
	short CurSprayID    = lPackRead->ReadShort();
	//��������->����޷�������.�����ѵ�����Ƥ��
	short nSprayCount   = lPackRead->ReadShort();

	Data.Row            = Row;
	Data.AuctionID      = nAuctionID;
	Data.AddedTimer     = nAddedTimestamp;
	Data.ItemID         = nItemID;
	Data.Bidding        = nBidPrice;
	Data.Price          = nPrice;
	Data.RemainTime     = nRemainTime;
	Data.CurViewPaintID = CurSprayID;
	Data.PaintCount     = nSprayCount;


	if (nSprayCount)
	{

		for (size_t k = 0; k < nSprayCount; k++)
		{
			//����ID
			short nSprayID = lPackRead->ReadShort();
			Data.PaintID[k] = nSprayID;
		}
	}

	//����
	int nAttribute   = lPackRead->ReadInt();
	//ǿ���ȼ�
	short Enchantlvl = lPackRead->ReadShort();

	if (Type == 1)
	{
		Data.Earnings = lPackRead->ReadInt();
	}


	Data.ItemAttribute = nAttribute;
	Data.Enchantlv     = Enchantlvl;


	if (nAttribute)
	{
		/*
		*	6                 =����
			5                 =����
			4                 =����
			3                 =������
			2                 =�˺�
			1                 =����
		*/
		Data.HitRate = BYTE(((nAttribute) & 0xF));
		Data.Damage  = BYTE(((nAttribute >> 4) & 0xF));
		Data.Recoil  = BYTE(((nAttribute >> 8) & 0xF));
		Data.Weight  = BYTE(((nAttribute >> 12) & 0xF));
		Data.Bursts  = BYTE(((nAttribute >> 16) & 0xF));
		Data.Clip    = BYTE(((nAttribute >> 20) & 0xF));

	}
	
	ACEInfoFileLog("[{}] Index:{:X} ItemID:{} AuctionID:{:X} ",__FUNCTION__, Data.Row, Data.ItemID, Data.AuctionID);



	return Data;
}

void CAuctionSystem::ParseAuctionNotice(std::shared_ptr<PackReader> lPackRead)
{
	ParseAuctionLine(lPackRead);

	//-------------------------------------------------------------------
	// ������Ƚϸı�����Щ��Ϣ
	// ���ۼ۸�-ʣ��ʱ��
	// ���ʣ��ʱ��Ϊ0�����ʾ��������-�Ƚ�ʱ������ֱ���Ƿ������Ļ��Ǳ���������
	// 
	//-------------------------------------------------------------------

	auto result =  m_AuctionMap.find(m_AuctionInfo.Row);
	
	if (result != m_AuctionMap.end())
	{
		if (m_AuctionInfo.RemainTime == 0)
		{
			int nEndTimer = m_AuctionInfo.AddedTimer + 3600;
			int SubValue = m_pACEUtil->GetUnixTimeStamp() - nEndTimer;

			if (SubValue >= 0)
			{
				ACEDebugFileLog("����:{}", m_AuctionInfo.Row);
			}
			else
			{
				if (m_AuctionInfo.AuctionID == m_BuyAutionID)
				{
					//֪ͨ����������ɹ�
					CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::BuyItem_Succeed, m_BuyTaskID, &m_AuctionInfo);

				}

				ACEDebugFileLog("����:{} �ѱ�����", m_AuctionInfo.Row);
			}

			//-��Ҫˢ�� ûʲô��
			//CGamePack::GetInstance()->RefreshAuctionSpecifyItem(m_AuctionInfo.AuctionID, m_AuctionInfo.AddedTimer, m_AuctionInfo.Row);
		}
		else if (result->second.Bidding != m_AuctionInfo.Bidding)
		{
			//������ID��� ���Ǿ���۸�䶯
			if (result->second.AuctionID == m_AuctionInfo.AuctionID)
			{
				auto v1 = g_pUtil->GetItemMap();

				auto Text = m_pACEUtil->WstringToString(v1->at(m_AuctionInfo.ItemID)->ItemSrc);

				ACEDebugFileLog("Row:{} Item:{} ����۸�������:{} ���ڵļ۸�:{}", m_AuctionInfo.Row, Text, m_AuctionInfo.Bidding - result->second.Bidding, m_AuctionInfo.Bidding);
			}

		}
		

		result->second = m_AuctionInfo;
	}
	else
	{
		//δ�ҵ� ����
		m_AuctionMap[m_AuctionInfo.Row] = m_AuctionInfo;
	}
}

void CAuctionSystem::ParseAuctionRefreshSpecifyItem(std::shared_ptr<PackReader> lPackRead)
{
	ParseAuctionLine(lPackRead);

	auto result = m_AuctionMap.find(m_AuctionInfo.Row);

	if (result != m_AuctionMap.end())
	{
		result->second = m_AuctionInfo;
	}
	else
	{
		//δ�ҵ� ����
		m_AuctionMap[m_AuctionInfo.Row] = m_AuctionInfo;
	}
	
	this->RunTask(&m_AuctionInfo);
	
	std::string Text;
	auto v1 = g_pUtil->GetItemMap();
	if (m_AuctionInfo.ItemID)
	{
		Text = m_pACEUtil->WstringToString(v1->at(m_AuctionInfo.ItemID)->ItemSrc);
	}

	ACEDebugFileLog("[ˢ������]Row:{} Item:{} Text:{}  timer:{} Price:{} ID:{:X}", m_AuctionInfo.Row, m_AuctionInfo.ItemID, Text, m_AuctionInfo.RemainTime, m_AuctionInfo.Price, m_AuctionInfo.AuctionID);
	
}

void CAuctionSystem::ParseAuctionSellItem(std::shared_ptr<PackReader> lPackRead)
{
	BYTE ItemCount = lPackRead->ReadChar();

	for (size_t i = 0; i < ItemCount; i++)
	{
		TAG_GAME_AUCTION Items = {};
		
		this->ParseAuctionViewData(lPackRead, Items, 1);

		if (Items.AuctionID && Items.RemainTime == 0)
		{
			if (Items.Earnings)
			{
				// ȡ�ػ���
				CGamePack::GetInstance()->AuctionGetBackIntegrate(Items.AuctionID, Items.AddedTimer);
			}
			else
			{
				//ȡ������
				CGamePack::GetInstance()->AuctionGetBackWeapon(Items.AuctionID, Items.AddedTimer);
			}
		}





		m_RackingItems[Items.Row] = Items;

		lPackRead->ReadInt();
	}
}

std::map<ULONG, TAG_GAME_AUCTION>& CAuctionSystem::GetAuctionMap()
{
	return m_AuctionMap;
}


void CAuctionSystem::RunTask(PTAG_GAME_AUCTION pAuction)
{
	std::map<ULONG, PTAG_GAME_TASK_AUCTION>& Task = GetTask();

	for (auto &iter : Task)
	{
		PTAG_GAME_TASK_AUCTION pTask = iter.second;

		if (pTask->TaskStatus != 0)
		{
			ACEDebugFileLog("AutionTaskID:{} finished", pTask->ulTaskID);
			continue;
		}

		//����ǿͻ�ת��������
		if (pTask->ulTaskType == 3)
		{
			for (auto & pClientTask : CSOWrapper::GetInstance()->GetRCF()->GetAuctionClientTaskMap())
			{
				//�������ID һ��
				if (pAuction->AuctionID == pClientTask.second-> Price)
				{
					m_BuyTaskID = pClientTask.second->ulTaskID;
					m_BuyAutionID = pAuction->AuctionID;
					CGamePack::GetInstance()->BuyAuctionPrice(pAuction->AuctionID, pAuction->AddedTimer, pAuction->Row);

				}
			}
			continue;
		}

		//ָ��ID
		if (pTask->ItemID == pAuction->ItemID || pTask->ItemID == 0 || pTask->ItemID == -1)
		{
			//�жϺ���������
			for (size_t i = 0; i < pTask->BlackListItem; i++)
			{
				if (pTask->BlackListItemID[i] == pAuction->ItemID)
				{
					ACEErrorLog("{} TaskID:{}", "������ID�ں������б�", pTask->ulTaskID);
					CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Mismatching_Item_BlackList, pTask->ulTaskID, pAuction);
					continue;
				}
			}

			//����һ�ڼ�=0 or ����һ�ڼ� >=�����۸�
			if (pTask->Price == 0 || pTask->Price >= pAuction->Price)
			{
				//�������
				if (pTask->BuyCount > 0)
				{
					//�����ж�
					if (CheckPaintType(pTask, pAuction))
					{

						//ǿ���ȼ� = 0 ������
						if (pTask->Enchantlv == 0 || (pTask->Enchantlv - 1) == pAuction->Enchantlv)
						{
							//ǿ������
							if (CheckStrengthenAttr(pTask, pAuction))
							{
								//����
								ACEErrorLog("{} TaskID:{}","�������ϳ��Թ���", pTask->ulTaskID);
								
								if (g_Vars.m_Integral < pAuction->Price)
								{
									//���ֲ���
									ACEErrorLog("{} TaskID:{}","���ֲ���֪ͨ������", pTask->ulTaskID);
									
									CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Integral_Lack, pTask->ulTaskID, pAuction);

									continue;
								}
								m_BuyTaskID   = pTask->ulTaskID;
								m_BuyAutionID = pAuction->AuctionID;
								CGamePack::GetInstance()->BuyAuctionPrice(pAuction->AuctionID, pAuction->AddedTimer, pAuction->Row);
								
							}
						}
						else
						{
							ACEErrorLog("{} TaskID:{}", "ǿ������������", pTask->ulTaskID);
							CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Mismatching_EnableEnchant, pTask->ulTaskID, pAuction);
							continue;
						}
					}
				}
			}
		}

	}

}

PTAG_AUTCION_SPECIAL_EVENTS CAuctionSystem::GetCurrentAuctionSpecialEvents()
{
	return &m_SpecialEvents;
}


void CAuctionSystem::ParseAuctionSpecialEvents(std::vector<BYTE> Data)
{

	auto lPackRead = std::make_shared<PackReader>(Data);

	lPackRead->ReadChar();

	m_SpecialEvents.v1      = lPackRead->ReadChar();
	m_SpecialEvents.v2      = lPackRead->ReadChar();
	m_SpecialEvents.v3      = lPackRead->ReadChar();
	m_SpecialEvents.Title   = lPackRead->ReadStr();
	m_SpecialEvents.Content = lPackRead->ReadStr();
	m_SpecialEvents.Count   = lPackRead->ReadChar();

	for (size_t i = 0; i < m_SpecialEvents.Count; i++)
	{
		TAG_AUTCION_SPECIAL_EVENTS_ITEMS Item = {};

		Item.nSign           = lPackRead->ReadInt();
		Item.nTimeStamp      = lPackRead->ReadInt();
		Item.nItemID         = lPackRead->ReadShort();
		Item.nStartTimeStamp = lPackRead->ReadInt();
		Item.Un_1            = lPackRead->ReadChar();
		
		m_SpecialEvents.MapItems.insert(std::make_pair(i, Item));
	}

}

PTAG_AUTCION_SPECIAL_EVENTS_ITEMS CAuctionSystem::GetCanTimeActive()
{
	//����
	int CurLocalTimeStamp = m_pACEUtil->GetUnixTimeStamp();
	auto LocalTime = m_pACEUtil->TimeStampFormatToStr(CurLocalTimeStamp, xorstr_("%Y-%m-%d"));

	for (auto& Iter : m_SpecialEvents.MapItems)
	{
		auto StrDay = m_pACEUtil->TimeStampFormatToStr(Iter.second.nStartTimeStamp, xorstr_("%Y-%m-%d"));

		if (StrDay == LocalTime)
		{
			// 10����  60 * 10
			if (CurLocalTimeStamp >= Iter.second.nStartTimeStamp && CurLocalTimeStamp  <= (Iter.second.nStartTimeStamp + 60 * 10))
			{
				return &Iter.second;
			}
		}
	}
	// û���ڻʱ����
	return nullptr;
}

bool CAuctionSystem::IsJoinAuctionSpecialEvents()
{
	return m_AuctionFlag & (1 << 0);
}

std::map<ULONG, TAG_GAME_AUCTION>& CAuctionSystem::GetItems()
{
	return m_RackingItems;
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& CAuctionSystem::GetTask()
{
	return  CSOWrapper::GetInstance()->GetRCF()->GetAuctionTaskMap();
}

bool CAuctionSystem::CheckPaintType(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction)
{
	bool isRet = false;

	if (pTask->PaintType == 0)
	{
		isRet = true;
	}
	else if(pTask->PaintType == 1)
	{
		if (pAuction->PaintCount == 0)
		{
			//û���� ֱ�ӷ��ؼ�
			return isRet;
		}
		
		int nSucceedCount = 0;

		for (size_t i = 0; i < pTask->PaintCount; i++)
		{
			for (size_t k = 0; k < pAuction->PaintCount; k++)
			{
				if (pTask->PaintID[i] == pAuction->PaintID[k])
				{
					nSucceedCount++;
				}
			}
		}
		// ������������
		if (nSucceedCount == pTask->PaintCount)
		{
			isRet = true;
		}

	}
	else if (pTask->PaintType == 2)
	{
		if (pAuction->PaintCount == 0)
		{
			//û���� ֱ�ӷ��ؼ�
			return isRet;
		}

		if (pAuction->PaintCount != 0)
		{
			//�жϲ��������
			int hit = 0;
			
			for (size_t k = 0; k < pAuction->PaintCount; k++)
			{
				for (size_t i = 0; i < pTask->BlackListPaint; i++)
				{
					if (pTask->BlackListPaintID[i] == pAuction->PaintID[k])
					{
						hit++;
						break;
					}
				}

			}
			//���������ٺ�������.ȫ�������򲻹���
			if (pAuction->PaintCount && pAuction->PaintCount == hit)
			{
				ACEWarningFileLog("[{}]-{}", __FUNCTION__, "�����ں������б�");

				CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Mismatching_Paint_BlackList, pTask->ulTaskID, pAuction);

				return false;
			}

			isRet = true;
		}
	}
	return isRet;
}

bool CAuctionSystem::CheckStrengthenAttr(PTAG_GAME_TASK_AUCTION pTask, PTAG_GAME_AUCTION pAuction)
{
	bool isRet = false;
	
	switch (pTask->ItemAttribute)
	{
	case 0:
		isRet = true;
		break;
	case 1:
	{
		if (pAuction->ItemAttribute == 0)
		{
			ACEDebugFileLogW(L"[{}]��ǰ����û��ǿ���������� ����ID:������:{}", __FUNCTIONW__, pAuction->ItemID, g_pUtil->GetItemMap()->at(pAuction->ItemID)->ItemSrc);
			break;
		}

		if (pTask->HitRate != pAuction->HitRate)
		{
			ACEDebugFileLog("[{}] ��������:{} ��������:{} ����������",__FUNCTION__, pTask->HitRate, pAuction->HitRate);
			break;
		}		
		if (pTask->Damage != pAuction->Damage)
		{
			ACEDebugFileLog("[{}] �����˺�:{} �����˺�:{} ����������", __FUNCTION__, pTask->Damage, pAuction->Damage);
			break;
		}
		if (pTask->Recoil != pAuction->Recoil)
		{
			ACEDebugFileLog("[{}] �������:{} ��������:{} ����������", __FUNCTION__, pTask->Recoil, pAuction->Recoil);
			break;
		}
		if (pTask->Weight != pAuction->Weight)
		{
			ACEDebugFileLog("[{}] ��������:{} ��������:{} ����������", __FUNCTION__, pTask->Weight, pAuction->Weight);
			break;
		}
		if (pTask->Bursts != pAuction->Bursts)
		{
			ACEDebugFileLog("[{}] ��������:{} ��������:{} ����������", __FUNCTION__, pTask->Bursts, pAuction->Bursts);
			break;
		}
		if (pTask->Clip != pAuction->Clip)
		{
			ACEDebugFileLog("[{}] ���񵯼�:{} ��������:{} ����������", __FUNCTION__, pTask->Clip, pAuction->Clip);
			break;
		}

		isRet = true;
	}
	break;
	default:
		break;
	}

	return isRet;
}

void CAuctionSystem::ThreadRef()
{
	m_bRefStatus = TRUE;

	while (m_bRefStatus)
	{

		std::map<ULONG, PTAG_GAME_TASK_AUCTION>& Task = GetTask();

		for (auto& iter : Task)
		{
			auto pTask = iter.second;
			
			if (pTask->TaskStatus != 0)
				continue;
			
			if (pTask->ulTaskType == 0 && pTask->BuyCount >= 1)
			{
				Sleep(1000 * 10);

				CGamePack::GetInstance()->RefreshAuction(pTask->ItemID, pTask->FindType, pTask->Enchantlv, pTask->PaintShow);

			}
			else if (pTask->ulTaskType == 1 && pTask->BuyCount >= 1)
			{
				for (auto& iter : m_AuctionMap)
				{
					TAG_GAME_AUCTION pAuction = iter.second;

					if (pAuction.Row >= 7)
					{
						CGamePack::GetInstance()->RefreshAuctionSpecifyItem(pAuction.AuctionID, pAuction.AddedTimer, pAuction.Row);
						Sleep(200);
					}
				}
				//��ˢ��һ��������.�������
				CGamePack::GetInstance()->RefreshAuction(0, 0, 0, 0);
			}
			else if (pTask->ulTaskType == 2)
			{
				m_bRefStatus = FALSE;
				break;
			}
			else if (pTask->ulTaskType == 3)
			{
				m_bRefStatus = FALSE;
				break;
			}
		}

	}
}

void CAuctionSystem::NotifySellWeapon(int Activate)
{
	//֪ͨ�����
	CSOWrapper::GetInstance()->GetRCF()->SendAutionItem(m_RackingItems, Activate);
}
