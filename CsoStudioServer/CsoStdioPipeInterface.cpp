#include "CsoStdioPipeInterface.h"
#include "HttpManage.h"

void onClientDisconnect(RCF::RcfSession& session)
{
	qDebug() << __FUNCTION__;

}
std::vector<BYTE> CsoStdioPipeInterface::Connect()
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);

	auto onDestroy = [&](RCF::RcfSession& session) { onClientDisconnect(session); };
	ConnectSession.setOnDestroyCallback(onDestroy);

	std::vector<BYTE> Result(1024, 0);
	auto lPackWrite = std::make_shared<PackWriter>(Result);
	lPackWrite->WriteByte(1);
	lPackWrite->WriteString(Session.GenerateSession());

	return Result;
}

std::vector<BYTE> CsoStdioPipeInterface::GetLoginUserName(ULONG ProcessPid)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	std::vector<BYTE> Result(1024, 0);
	auto lPackWrite = std::make_shared<PackWriter>(Result);

	if (m_pLoginInformation->GetMapProcess().count(ProcessPid) == 0)
	{
		lPackWrite->WriteByte(0);
	}
	else
	{
		// session 保存到ProcessMap
		auto ProcessInfo      =  m_pLoginInformation->GetMapProcess().at(ProcessPid);
		ProcessInfo->pSession = &Session;
		// 进程ID保存到 session
		Session.m_ProcessID   = ProcessPid;

		Session.m_pShareInfo = static_cast<PTAG_GAME_AC_LOGIN> (ProcessInfo->pUserData);
		lPackWrite->WriteByte(1);
		lPackWrite->WriteString(Session.m_pShareInfo->pData->GameUserName);
	}

	return Result;
}

std::string CsoStdioPipeInterface::GetLoginUserPassWord()
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	return Session.m_pShareInfo->pData->GameUserPassWord;
}

BOOL CsoStdioPipeInterface::CSONMEventNotice(ULONG MsgID, std::string MsgStr)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);

	LOGIN_NOTICE_CODE NoticeCode = static_cast<LOGIN_NOTICE_CODE>(MsgID);

	ACEWarningFileLog("[{}] MsgID:{} MsgStr:{}", __FUNCTION__, MsgID, CACEInterface::GetInstance()->GetUtil()->UTF8_To_string(MsgStr));

	switch (NoticeCode)
	{
	case LOGIN_NOTICE_CODE::NOTICE_GAME_INIT_SUCCESS:
		Session.m_pShareInfo->pData->clientTimeStamp = CACEInterface::GetInstance()->GetUtil()->GetUnixTimeStamp();
		Session.m_pShareInfo->ulTimeStamp = Session.m_pShareInfo->pData->clientTimeStamp;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_SUCCESS:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_U1:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R2:
		Session.m_pShareInfo->pData->UserStatus = USER_STATUS::USER_ERROR_R3;
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R3:
		Session.m_pShareInfo->pData->UserStatus = USER_STATUS::USER_ERROR_R0;
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R4:
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R5:
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R6:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R7:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_CAPT:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_ROOM_TIMEROUT:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_SERVER_TIMEROUT:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_APP_HEART_TIMEROUT:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AC_ERROR_STATUS:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_ERROR:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_CAPT_COUNT:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_REPLY_INVALID_CLIENT_VERSION:
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_NGS_INIT_ERROR:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R8:
		Session.m_pShareInfo->pData->UserStatus = USER_STATUS::USER_ERROR_R1;
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_UNK_STR:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R9:
		Session.m_pShareInfo->pData->UserStatus = USER_STATUS::USER_ERROR_R4;
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_PP_NOT_PASS:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R11:
		Session.m_pShareInfo->pData->UserStatus = USER_STATUS::USER_ERROR_R5;
		Session.m_pShareInfo->pData->UserTaskStatus = 1;
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_NGS_PASS_INIT_ERROR:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALIDNAME:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_INVALID_CHAR:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_UNCODE:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_SUCCEED:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_GAME_AUTHREPLE_ERROR_R12:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_TER_CONNECT_ER:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_TER_CONNECT_SUCCEED:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_REPLE_SYSERROR:
		//结束进程
		m_pLoginInformation->EndProcess(Session.m_pShareInfo->GUID);
		qInfo() << "user:" << Session.m_pShareInfo->pData->GameUserName.data() << " " << MsgStr.data();
		break;
	case LOGIN_NOTICE_CODE::NOTICE_CPU_SET:
		break;
	case LOGIN_NOTICE_CODE::NOTICE_NEXON_CREATEOK:
		break;
	default:
		break;
	}
	m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::RefreshUserInterface, this, Session.m_pShareInfo->ID));
	return TRUE;
}

std::string CsoStdioPipeInterface::GetEventGuid(ULONG ProcessPid)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	return Session.m_pShareInfo->GUID;
}

BOOL CsoStdioPipeInterface::GetTaskMode(ULONG &TaskMode)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	TaskMode = static_cast<ULONG>(Session.m_pShareInfo->pData->UserTask);
	return TRUE;
}

BOOL CsoStdioPipeInterface::SendGameUserInfo(std::vector<BYTE>& pGameUserInfo)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session       = ConnectSession.getSessionObject<CsoStudioSession>(true);
	auto lPackReader                = std::make_shared<PackReader>(pGameUserInfo);

	BYTE Type  = lPackReader->ReadChar();

	BOOL bRet = TRUE;
	switch (Type)
	{
	case 1:
	{
		//等级和名字
		Session.m_pShareInfo->pData->Lv         = lPackReader->ReadInt();
		Session.m_pShareInfo->pData->PlayerName = m_Util.string_To_UTF8(lPackReader->ReadStr());
		qDebug() << __FUNCTION__ << "LV:" << Session.m_pShareInfo->pData->Lv << "PlayerName:" << Session.m_pShareInfo->pData->PlayerName.data();
	}
		break;		
	case 2:
	{
		//金币
		Session.m_pShareInfo->pData->Gold = lPackReader->ReadLong64();
		qDebug() << __FUNCTION__ << "Gold:" << Session.m_pShareInfo->pData->Gold;
		
	}
	break;
	case 3:
	{
		//游戏币
		Session.m_pShareInfo->pData->Mileage = lPackReader->ReadLong64();
		qDebug() << __FUNCTION__ << "Mileage:" << Session.m_pShareInfo->pData->Mileage;

	}
	break;

	default:
		bRet = FALSE;
		break;
	}
	
	if (bRet)
	{
		
		m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::RefreshUserInterface, this, Session.m_pShareInfo->ID));
	}
	return bRet;
}

BOOL CsoStdioPipeInterface::Timestamp()
{
	RCF::RcfSession& ConnectSession   = RCF::getCurrentRcfSession();
	CsoStudioSession& Session         = ConnectSession.getSessionObject<CsoStudioSession>(true);


	



	Session.m_pShareInfo->ulTimeStamp = m_Util.GetUnixTimeStamp();
	
	m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::RefreshUserInterface, this, Session.m_pShareInfo->ID));
	return TRUE;
}

std::vector<BYTE> CsoStdioPipeInterface::GetAuctionTask()
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	
	std::vector<BYTE> Result(m_pAutionTaskMap.size() * sizeof(TAG_GAME_TASK_AUCTION) + 1024, 0);
	auto lPackWriter = std::make_shared<PackWriter>(Result);
	
	ULONG uCount = 0;
	
	lPackWriter->WriteByte(uCount);

	for (auto & iter : m_pAutionTaskMap)
	{
		if (iter.second->TaskStatus == 2)
			continue;
		for (size_t i = 0; i < iter.second->ACIDCount; i++)
		{
			if (Session.m_pShareInfo->pData->GameUserName == iter.second->szACArray[i] )
			{
				lPackWriter->WriteInt(iter.second->ulTaskID);
				lPackWriter->WriteInt(iter.second->ulTaskType);
				lPackWriter->WriteShort(iter.second->ItemID);
				lPackWriter->WriteInt(iter.second->Price);
				lPackWriter->WriteInt(iter.second->BuyCount);
				lPackWriter->WriteByte(iter.second->PaintType);
				lPackWriter->WriteByte(iter.second->PaintCount);

				for (size_t i = 0; i < iter.second->PaintCount; i++)
				{
					lPackWriter->WriteShort(iter.second->PaintID[i]);

				}
				lPackWriter->WriteInt(iter.second->Enchantlv);
				lPackWriter->WriteInt(iter.second->ItemAttribute);
				lPackWriter->WriteByte(iter.second->HitRate);
				lPackWriter->WriteByte(iter.second->Damage);
				lPackWriter->WriteByte(iter.second->Recoil);
				lPackWriter->WriteByte(iter.second->Weight);
				lPackWriter->WriteByte(iter.second->Bursts);
				lPackWriter->WriteByte(iter.second->Clip);
				lPackWriter->WriteByte(iter.second->TaskStatus);

				lPackWriter->WriteByte(iter.second->BlackListItem);
				for (size_t i = 0; i < iter.second->BlackListItem; i++)
				{
					lPackWriter->WriteShort(iter.second->BlackListItemID[i]);
				}
				
				lPackWriter->WriteByte(iter.second->BlackListPaint);
				for (size_t i = 0; i < iter.second->BlackListPaint; i++)
				{
					lPackWriter->WriteShort(iter.second->BlackListPaintID[i]);
				}

				lPackWriter->WriteByte(iter.second->FindType);
				
				lPackWriter->WriteShort(iter.second->PaintShow);
				uCount++;
			}
			
		}

	}
	int Index = lPackWriter->GetBytesLength();
	lPackWriter->SetOffset(0);
	lPackWriter->WriteByte(uCount);

	
	return Result;
}

BOOL CsoStdioPipeInterface::AuctionNotice(std::vector<BYTE>& pAuctionNotice)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	auto lPackReader = std::make_shared<PackReader>(pAuctionNotice);
	
	byte AuctionNoticeType = lPackReader->ReadChar();

	switch (AuctionNoticeType)
	{
	case 0:	//购买完毕
	{
		int nTaskID = lPackReader->ReadInt();

		//购买的商品信息
		m_BuyWeapion.Row            = lPackReader->ReadChar();
		m_BuyWeapion.AuctionID      = lPackReader->ReadInt();
		m_BuyWeapion.AddedTimer     = lPackReader->ReadInt();
		m_BuyWeapion.ItemID         = lPackReader->ReadShort();
		m_BuyWeapion.Bidding        = lPackReader->ReadInt();
		m_BuyWeapion.Price          = lPackReader->ReadInt();
		m_BuyWeapion.RemainTime     = lPackReader->ReadInt();
		m_BuyWeapion.CurViewPaintID = lPackReader->ReadShort();
		m_BuyWeapion.PaintCount     = lPackReader->ReadShort();

		for (size_t i = 0; i < m_BuyWeapion.PaintCount; i++)
		{
			m_BuyWeapion.PaintID[i]= lPackReader->ReadShort();
		}
		m_BuyWeapion.ItemAttribute = lPackReader->ReadInt();
		m_BuyWeapion.HitRate       = lPackReader->ReadChar();
		m_BuyWeapion.Damage        = lPackReader->ReadChar();
		m_BuyWeapion.Recoil        = lPackReader->ReadChar();
		m_BuyWeapion.Weight        = lPackReader->ReadChar();
		m_BuyWeapion.Bursts        = lPackReader->ReadChar();
		m_BuyWeapion.Clip          = lPackReader->ReadChar();
		m_BuyWeapion.Enchantlv     = lPackReader->ReadShort();

		//取任务列表
		if (m_pAutionTaskMap.count(nTaskID))
		{
			auto pTask = m_pAutionTaskMap[nTaskID];

			//更新任务状态
			pTask->BuyCount--;

			if (pTask->BuyCount <= 0)
			{
				pTask->TaskStatus = 1;
			}
		}
		//ACEErrorFileLog("[{}] AuctionNotice: TaskID[{}] not exist", Session.m_pShareInfo->pData->GameUserName, nTaskID);
		//break;
		
		

		std::string Text = fmt::format("[{}] 任务ID[{}]拍卖ID:{:X} 上架时间:{} 购买时间:{}武器名:{} 价格:{}", Session.m_pShareInfo->pData->GameUserName, nTaskID, m_BuyWeapion.AuctionID, m_Util.TimeStampToStr(m_BuyWeapion.AddedTimer), m_Util.TimeStampToStr(m_Util.GetUnixTimeStamp()), g_Util.GetItemNameByID(m_BuyWeapion.ItemID), m_BuyWeapion.Price);

		if (m_BuyWeapion.PaintCount)
		{
			Text.append(fmt::format("彩漆数量:{}", m_BuyWeapion.PaintCount));
			
			for (size_t i = 0; i < m_BuyWeapion.PaintCount; i++)
			{
				if (!g_Util.GetPaintMap().count(m_BuyWeapion.PaintID[i]))
				{
					qDebug() << "未找到彩漆ID" << m_BuyWeapion.PaintID[i];
					continue;
				}
				Text.append(g_Util.GetPaintMap().at(m_BuyWeapion.PaintID[i]) + " ");
			}
		}
		
		if (m_BuyWeapion.Enchantlv)
		{
			Text.append(fmt::format("强化等级:{} ", m_BuyWeapion.Enchantlv));
		}
		if (m_BuyWeapion.ItemAttribute)
		{
			if (m_BuyWeapion.HitRate)
			{
				Text.append(fmt::format("命中:{} ", m_BuyWeapion.HitRate));
			}
			if (m_BuyWeapion.Damage)
			{
				Text.append(fmt::format("伤害:{} ", m_BuyWeapion.Damage));
			}
			if (m_BuyWeapion.Recoil)
			{
				Text.append(fmt::format("后座:{} ", m_BuyWeapion.Recoil));
			}
			if (m_BuyWeapion.Weight)
			{
				Text.append(fmt::format("重量:{} ", m_BuyWeapion.Weight));
			}
			if (m_BuyWeapion.Bursts)
			{
				Text.append(fmt::format("连发:{} ", m_BuyWeapion.Bursts));
			}
			if (m_BuyWeapion.Clip)
			{
				Text.append(fmt::format("弹夹:{} ", m_BuyWeapion.Clip));
			}
		}
		
		//显示到UI
		qInfo() << "购买完成 " << Text.data();
		
		//发送给客户端
		HttpManage::getInstance()->SendAuctionResult(&m_BuyWeapion);

		m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::RefreshAuctionInterface, this, nTaskID));
	}
	break;
	case 1:	//积分不足
	{
		int nTaskID = lPackReader->ReadInt();

		//商品信息
		m_BuyWeapion.Row            = lPackReader->ReadChar();
		m_BuyWeapion.AuctionID      = lPackReader->ReadInt();
		m_BuyWeapion.AddedTimer     = lPackReader->ReadInt();
		m_BuyWeapion.ItemID         = lPackReader->ReadShort();
		m_BuyWeapion.Bidding        = lPackReader->ReadInt();
		m_BuyWeapion.Price          = lPackReader->ReadInt();
		m_BuyWeapion.RemainTime     = lPackReader->ReadInt();
		m_BuyWeapion.CurViewPaintID = lPackReader->ReadShort();
		m_BuyWeapion.PaintCount     = lPackReader->ReadShort();

		for (size_t i = 0; i < m_BuyWeapion.PaintCount; i++)
		{
			m_BuyWeapion.PaintID[i] = lPackReader->ReadShort();
		}
		m_BuyWeapion.ItemAttribute = lPackReader->ReadInt();
		m_BuyWeapion.HitRate       = lPackReader->ReadChar();
		m_BuyWeapion.Damage        = lPackReader->ReadChar();
		m_BuyWeapion.Recoil        = lPackReader->ReadChar();
		m_BuyWeapion.Weight        = lPackReader->ReadChar();
		m_BuyWeapion.Bursts        = lPackReader->ReadChar();
		m_BuyWeapion.Clip          = lPackReader->ReadChar();
		m_BuyWeapion.Enchantlv     = lPackReader->ReadShort();

		
		std::string Text = fmt::format("任务ID[{}] 拍卖ID:{:X} 上架时间:{} 武器名:{} 价格:{}", nTaskID, m_BuyWeapion.AuctionID, m_Util.TimeStampToStr(m_BuyWeapion.AddedTimer),  g_Util.GetItemNameByID(m_BuyWeapion.ItemID), m_BuyWeapion.Price);

		if (m_BuyWeapion.PaintCount)
		{
			Text.append(fmt::format("彩漆数量:{}", m_BuyWeapion.PaintCount));

			for (size_t i = 0; i < m_BuyWeapion.PaintCount; i++)
			{
				if (!g_Util.GetPaintMap().count(m_BuyWeapion.PaintID[i]))
				{
					qDebug() << "未找到彩漆ID" << m_BuyWeapion.PaintID[i];
					continue;
				}
				Text.append(g_Util.GetPaintMap().at(m_BuyWeapion.PaintID[i]) + " ");
			}
		}

		if (m_BuyWeapion.Enchantlv)
		{
			Text.append(fmt::format("强化等级:{} ", m_BuyWeapion.Enchantlv));
		}
		if (m_BuyWeapion.ItemAttribute)
		{

			if (m_BuyWeapion.HitRate)
			{
				Text.append(fmt::format("命中:{} ", m_BuyWeapion.HitRate));
			}
			if (m_BuyWeapion.Damage)
			{
				Text.append(fmt::format("伤害:{} ", m_BuyWeapion.Damage));
			}
			if (m_BuyWeapion.Recoil)
			{
				Text.append(fmt::format("后座:{} ", m_BuyWeapion.Recoil));
			}
			if (m_BuyWeapion.Weight)
			{
				Text.append(fmt::format("重量:{} ", m_BuyWeapion.Weight));
			}
			if (m_BuyWeapion.Bursts)
			{
				Text.append(fmt::format("连发:{} ", m_BuyWeapion.Bursts));
			}
			if (m_BuyWeapion.Clip)
			{
				Text.append(fmt::format("弹夹:{} ", m_BuyWeapion.Clip));
			}

		}
		//显示到UI
		qInfo() << "无法购买,积分不足 " << Text.data();


		break;
	}
	case 2:	//积分不足,购买失败
	{
		int nTaskID = lPackReader->ReadInt();

		qInfo() << "购买失败,积分不足 任务ID:"<< nTaskID;
		break;
	}
	case 3:	//强化不匹配
	{
		int nTaskID      = lPackReader->ReadInt();
		short ItemID     = lPackReader->ReadShort();
		short Enchantlv  = lPackReader->ReadShort();
		int  AuctionID   = lPackReader->ReadInt();
		
		if (Session.m_SetAuctionID.count(AuctionID))
		{
			// 已存在跳过
			break;
		}
		Session.m_SetAuctionID.emplace(AuctionID);

		std::string Text = fmt::format("[{}] 任务ID[{}] 强化不匹配,跳过购买 武器:{} 强化等级:{}", Session.m_pShareInfo->pData->GameUserName, nTaskID, g_Util.GetItemNameByID(ItemID), Enchantlv);

		qInfo() << Text.data();
		break;
	}
	case 4:	//黑名单彩漆
	{
		int nTaskID = lPackReader->ReadInt();
		short ItemID = lPackReader->ReadShort();
		short PaintCount = lPackReader->ReadShort();
		short PaintID[16] = { 0 };

		std::string Text = fmt::format("[{}] 任务ID[{}] 武器彩漆在黑名单,跳过购买 武器:{}", Session.m_pShareInfo->pData->GameUserName, nTaskID, g_Util.GetItemNameByID(ItemID));
		Text.append(fmt::format("彩漆数量:{}", PaintCount));
		for (size_t i = 0; i < PaintCount; i++)
		{
			PaintID[i] = lPackReader->ReadShort();
			Text.append(g_Util.GetPaintMap().at(PaintID[i]) + " ");
		}
		int  AuctionID = lPackReader->ReadInt();
		if (Session.m_SetAuctionID.count(AuctionID))
		{
			// 已存在跳过
			break;
		}
		Session.m_SetAuctionID.emplace(AuctionID);

		qInfo() << Text.data();
		break;
	}
	case 5:	//武器黑名单
	{
		int nTaskID    = lPackReader->ReadInt();
		short ItemID   = lPackReader->ReadShort();
		int  AuctionID = lPackReader->ReadInt();
		if (Session.m_SetAuctionID.count(AuctionID))
		{
			// 已存在跳过
			break;
		}
		Session.m_SetAuctionID.emplace(AuctionID);
		std::string Text = fmt::format("[{}] 任务ID[{}] 武器在黑名单,跳过购买 武器:{}", Session.m_pShareInfo->pData->GameUserName, nTaskID, g_Util.GetItemNameByID(ItemID));
		

		qInfo() << Text.data();
		break;
	}
	default:
		break;
	}



	return TRUE;
}

ULONG64 CsoStdioPipeInterface::GetAuctionEventsStatus()
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();

	CsoStudioSession& Session = ConnectSession.getSessionObject<CsoStudioSession>(true);
	
	return Session.m_pShareInfo->pData->AuctionFlag;
}

BOOL CsoStdioPipeInterface::SendAuctionRackingItem(std::vector<BYTE>& Data)
{
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
	CsoStudioSession& Session       = ConnectSession.getSessionObject<CsoStudioSession>(true);
	
	auto lPackReader = std::make_shared<PackReader>(Data);

	BYTE Count                                         = lPackReader->ReadChar();
	Session.GetShareInfo()->pData->AuctionActiveWeapon = lPackReader->ReadChar();
		
	for (size_t i = 0; i < Count; i++)
	{
		m_BuyWeapion.Row            = lPackReader->ReadChar();
		m_BuyWeapion.AuctionID      = lPackReader->ReadInt();
		m_BuyWeapion.AddedTimer     = lPackReader->ReadInt();
		m_BuyWeapion.ItemID         = lPackReader->ReadShort();
		m_BuyWeapion.Bidding        = lPackReader->ReadInt();
		m_BuyWeapion.Price          = lPackReader->ReadInt();
		m_BuyWeapion.RemainTime     = lPackReader->ReadInt();
		m_BuyWeapion.CurViewPaintID = lPackReader->ReadShort();
		m_BuyWeapion.PaintCount     = lPackReader->ReadShort();

		for (size_t i = 0; i < m_BuyWeapion.PaintCount; i++)
		{
			m_BuyWeapion.PaintID[i] = lPackReader->ReadShort();
		}
		m_BuyWeapion.ItemAttribute = lPackReader->ReadInt();
		m_BuyWeapion.HitRate       = lPackReader->ReadChar();
		m_BuyWeapion.Damage        = lPackReader->ReadChar();
		m_BuyWeapion.Recoil        = lPackReader->ReadChar();
		m_BuyWeapion.Weight        = lPackReader->ReadChar();
		m_BuyWeapion.Bursts        = lPackReader->ReadChar();
		m_BuyWeapion.Clip          = lPackReader->ReadChar();
		m_BuyWeapion.Enchantlv     = lPackReader->ReadShort();
		m_BuyWeapion.Earnings      = lPackReader->ReadInt();

		Session.GetShareInfo()->pData->AuctionRackingItems[m_BuyWeapion.Row] = m_BuyWeapion;


		//-------------------------------------------------------------------
		// 生成客户的搜索任务
		//-------------------------------------------------------------------

		TAG_GAME_TASK_AUCTION AuctionClientTask = {};
		
		AuctionClientTask.ulTaskID   = m_BuyWeapion.Row;
		AuctionClientTask.ulTaskType = 3;
		AuctionClientTask.ItemID     = m_BuyWeapion.ItemID;
		AuctionClientTask.Price      = m_BuyWeapion.AuctionID;		//价格 == ID
		AuctionClientTask.PaintShow  = m_BuyWeapion.CurViewPaintID;
		//如果竞标价格+1 == 一口价 那么搜索类型== 一口价最低 否则就是竞标价最低
		AuctionClientTask.FindType   = /*m_BuyWeapion.Bidding + 100 == m_BuyWeapion.Price ? 1 : 0*/1 ;
		
		// 如果是无限制 那么强化等级强制为0 这样能缩小范围
		if (m_BuyWeapion.Enchantlv == 0)
		{
			AuctionClientTask.Enchantlv = 1;
		}
		else
		{
			AuctionClientTask.Enchantlv = m_BuyWeapion.Enchantlv + 1;
		}
		

		Session.GetShareInfo()->pData->AuctionClientTask[m_BuyWeapion.Row] = AuctionClientTask;
	}
	if (Session.GetShareInfo()->pData->AuctionActiveWeapon != 0xFF)
	{
		//提醒任务通知
		m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::ClientSearchTaskNotice, this));
	}

	//提醒更新界面
	m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::RefreshAuctionShelveInterface, this));

	return TRUE;
}

void CsoStdioPipeInterface::SetLoginModule(CLoginModule* pThis)
{
	m_ThreadPool        = new thread_pool(1);
	m_pLoginInformation = pThis;
	
	m_ThreadPool->push_task(std::bind(&CsoStdioPipeInterface::FindQtWindow, this));
}

void CsoStdioPipeInterface::RefreshUserInterface(const ULONG uID)
{
	auto lPackWriter = std::make_shared<PackWriter>(1024);

	lPackWriter->WriteByte(1);
	lPackWriter->WriteInt(uID);
	COPYDATASTRUCT data;
	data.dwData = 0;
	data.cbData = lPackWriter->GetBytesLength();
	data.lpData = lPackWriter->GetBytes();
	
	::SendMessageA(m_hClientHWND, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);
}

void CsoStdioPipeInterface::FindQtWindow()
{
	while (true)
	{
		m_hClientHWND = FindWindowA("Qt5QWindowIcon", "CsoStudioServer");
		if (m_hClientHWND)
		{
			break;
		}
		Sleep(200);
	}
	qDebug() << "ClientHWND:" << m_hClientHWND;
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& CsoStdioPipeInterface::GetAuctionTaskThis()
{
	return m_pAutionTaskMap;
}

void CsoStdioPipeInterface::RefreshAuctionInterface(ULONG uID)
{
	auto lPackWriter = std::make_shared<PackWriter>(1024);

	lPackWriter->WriteByte(2);
	lPackWriter->WriteInt(uID);
	COPYDATASTRUCT data;
	data.dwData = 0;
	data.cbData = lPackWriter->GetBytesLength();
	data.lpData = lPackWriter->GetBytes();

	::SendMessageA(m_hClientHWND, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);
}

void CsoStdioPipeInterface::RefreshAuctionShelveInterface()
{
	auto lPackWriter = std::make_shared<PackWriter>(1024);

	lPackWriter->WriteByte(3);
	COPYDATASTRUCT data;
	data.dwData = 0;
	data.cbData = lPackWriter->GetBytesLength();
	data.lpData = lPackWriter->GetBytes();

	::SendMessageA(m_hClientHWND, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);
}

void CsoStdioPipeInterface::ClientSearchTaskNotice()
{
	auto lPackWriter = std::make_shared<PackWriter>(1024);

	lPackWriter->WriteByte(4);
	COPYDATASTRUCT data;
	data.dwData = 0;
	data.cbData = lPackWriter->GetBytesLength();
	data.lpData = lPackWriter->GetBytes();

	::SendMessageA(m_hClientHWND, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);
}

CsoStudioSession::~CsoStudioSession()
{
	ACEErrorFileLog("{}", __FUNCTION__);

	if (CLoginModule::GetInstance()->GetMapProcess().count(m_ProcessID))
	{
		CLoginModule::GetInstance()->GetMapProcess().at(m_ProcessID)->pSession = nullptr;
	}
}
