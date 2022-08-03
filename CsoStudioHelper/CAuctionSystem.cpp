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
	// 获取共享拍卖会信息
	m_AuctionFlag =  CGameRCF::GetInstance()->GetShareAuctionEvents();
}

bool CAuctionSystem::WeaponAuctionEvent(std::shared_ptr<PackReader> lPackRead)
{
	ULONG Index = lPackRead->ReadChar();

	ACEDebugLog("[{}] Index:{}", __FUNCTION__, Index);
	
	g_pUtil->ViewHexCode(lPackRead);

	// 武器：1 配件：2 活动 3
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
			// - 活动栏
			int unFlag = lPackRead->ReadChar();
		}
		else if (nItemType == 2)
		{
			//配件暂时不处理
			return true;
		}

		// 剩余时间-分钟
		short OtherMinute  = lPackRead->ReadShort();
		// 显示类型
		int ShowType       = lPackRead->ReadChar();
		int nOffset        = lPackRead->GetIndex();
		// Item类型 1:武器 2:配件 3:活动
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
					
					// 活动栏-正在拍卖时间
					if (m_SpecialItems.AuctionID && this->IsJoinAuctionSpecialEvents())
					{
						//没有判断当前是否已经竞拍了此物品.无碍
						if (auto Items = this->GetCanTimeActive())
						{
							CGamePack::GetInstance()->AuctionEventsBidding(Items->nSign, Items->nTimeStamp, 1000);
							
							// 不能break.需要解析完
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
				
				ACEDebugLog("[{}]取回item_id:{}", __FUNCTION__, m_SpecialItems.ItemID);
				//取回道具
				CGamePack::GetInstance()->AuctionEventsGetBack(m_SpecialItems.AuctionID, m_SpecialItems.AddedTimer);
			}
		}
		if (ShowType & 8)	//上架的
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
		case 0x5:	//积分不足-发给服务端
		{
			ACEDebugLog("[{}] 积分不足 购买失败:{}", __FUNCTION__, m_BuyTaskID);
			CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Integral_Lack_SV, m_BuyTaskID,nullptr);
		}
			break;
		case 0x10:	//错误的信息
		case 0x19:	//指定武器刷新太快
		case 0x21:	//未找到符合的武器
		case 0x7:	//err 07
			return false;
		default:
			ACEDebugLog("[{}] 错误ID:{}", __FUNCTION__, MsgType);
			break;
		}
		
	}
	break;
	case 0xE:		//更新武器栏上架信息
	{
		if (nItemType == 1)
		{
			TAG_GAME_AUCTION Items = {};

			this->ParseAuctionViewData(lPackRead, Items, 1);

			if (Items.AuctionID && Items.RemainTime == 0)
			{
				if (Items.Earnings)
				{
					// 取回积分
					CGamePack::GetInstance()->AuctionGetBackIntegrate(Items.AuctionID, Items.AddedTimer);
				}
				else
				{
					//取回武器
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
		//屏蔽非拍卖会期间的错误提示
		return false;
	}
	default:
		
		break;
	}
	return true;
}

void CAuctionSystem::ParseAuctionView(std::shared_ptr<PackReader> lPackRead)
{
	//当前物品数量
	ULONG ItemCount = lPackRead->ReadChar();

	for (size_t i = 0; i < ItemCount; i++)
	{
		ParseAuctionLine(lPackRead);
		// 插入信息
		m_AuctionMap[m_AuctionInfo.Row] = m_AuctionInfo;

		this->RunTask(&m_AuctionInfo);
	}
	if (CGameRCF::GetInstance()->GetTaskMode()== TASK_MODE::GAME_TASK_AUCTION)
	{
		//启动刷新界面线程
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
	//第几行
	BYTE Row            = lPackRead->ReadChar();
	//拍卖ID
	int nAuctionID      = lPackRead->ReadInt();
	//上架的时间戳
	int nAddedTimestamp = lPackRead->ReadInt();
	//物品ID
	short nItemID       = lPackRead->ReadShort();
	//未知1
	BYTE nItemUn1       = lPackRead->ReadChar();
	//未知2
	BYTE nItemUn2       = lPackRead->ReadChar();
	//竞标价格
	int nBidPrice       = lPackRead->ReadInt();
	//一口价
	int nPrice          = lPackRead->ReadInt();
	//剩余时间
	int nRemainTime     = lPackRead->ReadInt();
	//当前显示的彩漆ID->这个可以被手动隐藏
	short CurSprayID    = lPackRead->ReadShort();
	//喷漆数量->这个无法被隐藏.可以搜到隐藏皮肤
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
			//喷漆ID
			short nSprayID = lPackRead->ReadShort();
			Data.PaintID[k] = nSprayID;
		}
	}

	//属性
	int nAttribute   = lPackRead->ReadInt();
	//强化等级
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
		*	6                 =弹夹
			5                 =连发
			4                 =重量
			3                 =后坐力
			2                 =伤害
			1                 =命中
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
	// 在这里比较改变了哪些信息
	// 竞价价格-剩余时间
	// 如果剩余时间为0，则表示拍卖结束-比较时间戳来分辨出是否是流拍还是被人拍走了
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
				ACEDebugFileLog("流拍:{}", m_AuctionInfo.Row);
			}
			else
			{
				if (m_AuctionInfo.AuctionID == m_BuyAutionID)
				{
					//通知服务器购买成功
					CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::BuyItem_Succeed, m_BuyTaskID, &m_AuctionInfo);

				}

				ACEDebugFileLog("道具:{} 已被拍走", m_AuctionInfo.Row);
			}

			//-不要刷新 没什么用
			//CGamePack::GetInstance()->RefreshAuctionSpecifyItem(m_AuctionInfo.AuctionID, m_AuctionInfo.AddedTimer, m_AuctionInfo.Row);
		}
		else if (result->second.Bidding != m_AuctionInfo.Bidding)
		{
			//拍卖会ID相等 则是竞标价格变动
			if (result->second.AuctionID == m_AuctionInfo.AuctionID)
			{
				auto v1 = g_pUtil->GetItemMap();

				auto Text = m_pACEUtil->WstringToString(v1->at(m_AuctionInfo.ItemID)->ItemSrc);

				ACEDebugFileLog("Row:{} Item:{} 竞标价格增加了:{} 现在的价格:{}", m_AuctionInfo.Row, Text, m_AuctionInfo.Bidding - result->second.Bidding, m_AuctionInfo.Bidding);
			}

		}
		

		result->second = m_AuctionInfo;
	}
	else
	{
		//未找到 插入
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
		//未找到 插入
		m_AuctionMap[m_AuctionInfo.Row] = m_AuctionInfo;
	}
	
	this->RunTask(&m_AuctionInfo);
	
	std::string Text;
	auto v1 = g_pUtil->GetItemMap();
	if (m_AuctionInfo.ItemID)
	{
		Text = m_pACEUtil->WstringToString(v1->at(m_AuctionInfo.ItemID)->ItemSrc);
	}

	ACEDebugFileLog("[刷新武器]Row:{} Item:{} Text:{}  timer:{} Price:{} ID:{:X}", m_AuctionInfo.Row, m_AuctionInfo.ItemID, Text, m_AuctionInfo.RemainTime, m_AuctionInfo.Price, m_AuctionInfo.AuctionID);
	
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
				// 取回积分
				CGamePack::GetInstance()->AuctionGetBackIntegrate(Items.AuctionID, Items.AddedTimer);
			}
			else
			{
				//取回武器
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

		//如果是客户转武器任务
		if (pTask->ulTaskType == 3)
		{
			for (auto & pClientTask : CSOWrapper::GetInstance()->GetRCF()->GetAuctionClientTaskMap())
			{
				//如果拍卖ID 一致
				if (pAuction->AuctionID == pClientTask.second-> Price)
				{
					m_BuyTaskID = pClientTask.second->ulTaskID;
					m_BuyAutionID = pAuction->AuctionID;
					CGamePack::GetInstance()->BuyAuctionPrice(pAuction->AuctionID, pAuction->AddedTimer, pAuction->Row);

				}
			}
			continue;
		}

		//指定ID
		if (pTask->ItemID == pAuction->ItemID || pTask->ItemID == 0 || pTask->ItemID == -1)
		{
			//判断黑名单武器
			for (size_t i = 0; i < pTask->BlackListItem; i++)
			{
				if (pTask->BlackListItemID[i] == pAuction->ItemID)
				{
					ACEErrorLog("{} TaskID:{}", "该武器ID在黑名单列表", pTask->ulTaskID);
					CSOWrapper::GetInstance()->GetRCF()->SendAutionNotice(AuctionNotice::Mismatching_Item_BlackList, pTask->ulTaskID, pAuction);
					continue;
				}
			}

			//任务一口价=0 or 任务一口价 >=拍卖价格
			if (pTask->Price == 0 || pTask->Price >= pAuction->Price)
			{
				//购买次数
				if (pTask->BuyCount > 0)
				{
					//彩漆判断
					if (CheckPaintType(pTask, pAuction))
					{

						//强化等级 = 0 不限制
						if (pTask->Enchantlv == 0 || (pTask->Enchantlv - 1) == pAuction->Enchantlv)
						{
							//强化属性
							if (CheckStrengthenAttr(pTask, pAuction))
							{
								//购买
								ACEErrorLog("{} TaskID:{}","条件符合尝试购买", pTask->ulTaskID);
								
								if (g_Vars.m_Integral < pAuction->Price)
								{
									//积分不足
									ACEErrorLog("{} TaskID:{}","积分不足通知服务器", pTask->ulTaskID);
									
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
							ACEErrorLog("{} TaskID:{}", "强化条件不符合", pTask->ulTaskID);
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
	//今天
	int CurLocalTimeStamp = m_pACEUtil->GetUnixTimeStamp();
	auto LocalTime = m_pACEUtil->TimeStampFormatToStr(CurLocalTimeStamp, xorstr_("%Y-%m-%d"));

	for (auto& Iter : m_SpecialEvents.MapItems)
	{
		auto StrDay = m_pACEUtil->TimeStampFormatToStr(Iter.second.nStartTimeStamp, xorstr_("%Y-%m-%d"));

		if (StrDay == LocalTime)
		{
			// 10分钟  60 * 10
			if (CurLocalTimeStamp >= Iter.second.nStartTimeStamp && CurLocalTimeStamp  <= (Iter.second.nStartTimeStamp + 60 * 10))
			{
				return &Iter.second;
			}
		}
	}
	// 没有在活动时间内
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
			//没彩漆 直接返回假
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
		// 符合喷漆条件
		if (nSucceedCount == pTask->PaintCount)
		{
			isRet = true;
		}

	}
	else if (pTask->PaintType == 2)
	{
		if (pAuction->PaintCount == 0)
		{
			//没彩漆 直接返回假
			return isRet;
		}

		if (pAuction->PaintCount != 0)
		{
			//判断彩漆黑名单
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
			//武器彩漆再黑名单里.全部命中则不购买
			if (pAuction->PaintCount && pAuction->PaintCount == hit)
			{
				ACEWarningFileLog("[{}]-{}", __FUNCTION__, "喷漆在黑名单列表");

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
			ACEDebugFileLogW(L"[{}]当前武器没有强化属性跳过 武器ID:武器名:{}", __FUNCTIONW__, pAuction->ItemID, g_pUtil->GetItemMap()->at(pAuction->ItemID)->ItemSrc);
			break;
		}

		if (pTask->HitRate != pAuction->HitRate)
		{
			ACEDebugFileLog("[{}] 任务命中:{} 武器命中:{} 不符合跳过",__FUNCTION__, pTask->HitRate, pAuction->HitRate);
			break;
		}		
		if (pTask->Damage != pAuction->Damage)
		{
			ACEDebugFileLog("[{}] 任务伤害:{} 武器伤害:{} 不符合跳过", __FUNCTION__, pTask->Damage, pAuction->Damage);
			break;
		}
		if (pTask->Recoil != pAuction->Recoil)
		{
			ACEDebugFileLog("[{}] 任务后座:{} 武器后座:{} 不符合跳过", __FUNCTION__, pTask->Recoil, pAuction->Recoil);
			break;
		}
		if (pTask->Weight != pAuction->Weight)
		{
			ACEDebugFileLog("[{}] 任务重量:{} 武器重量:{} 不符合跳过", __FUNCTION__, pTask->Weight, pAuction->Weight);
			break;
		}
		if (pTask->Bursts != pAuction->Bursts)
		{
			ACEDebugFileLog("[{}] 任务连发:{} 武器连发:{} 不符合跳过", __FUNCTION__, pTask->Bursts, pAuction->Bursts);
			break;
		}
		if (pTask->Clip != pAuction->Clip)
		{
			ACEDebugFileLog("[{}] 任务弹夹:{} 武器弹夹:{} 不符合跳过", __FUNCTION__, pTask->Clip, pAuction->Clip);
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
				//再刷新一次拍卖行.竞价最低
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
	//通知服务端
	CSOWrapper::GetInstance()->GetRCF()->SendAutionItem(m_RackingItems, Activate);
}
