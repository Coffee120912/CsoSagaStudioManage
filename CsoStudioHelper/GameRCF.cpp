#include "GameRCF.h"
#include "DispatchRecv.h"

CGameRCF::CGameRCF()
{
	m_pRcfInit  = nullptr;
	m_pClient   = nullptr;
	m_pInstance = this;
	m_pGameProxyClient = new GameProxyClient();
	m_WordThread = new thread_pool(1);
	m_ErrID = 0;
}

CGameRCF* CGameRCF::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameRCF();
	}
	return m_pInstance;
}

VOID CGameRCF::StartThreadRCFInit()
{

	auto ThreadFuns = [&]()->void
	{
		m_pRcfInit = new RCF::RcfInit();

		try
		{

			m_pClient = new RcfClient<I_CsoStdioPipe>(RCF::Win32NamedPipeEndpoint("SagaSvrPipe"));
			//取消自动连接
			m_pClient->getClientStub().setAutoReconnect(false);


			auto Result = m_pClient->Connect();

			auto lPackRead = std::make_shared<PackReader>(Result.get());

			if (lPackRead->ReadChar())
			{
				m_ClientSession = lPackRead->ReadStr();

				//设置请求用户数据
				m_pClient->getClientStub().setRequestUserData(m_ClientSession);

				//验证成功.启动代理
				static RCF::RcfServer proxyServer(RCF::ProxyEndpoint(RCF::TcpEndpoint(xorstr_("127.0.0.1"), 52122), m_ClientSession));
				
				proxyServer.bind<I_GameProxyClient>(*m_pGameProxyClient);
				
				proxyServer.start();
			}

		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};
	std::thread (ThreadFuns).detach();
}

std::string CGameRCF::GetLoginUser()
{

	try
	{
		auto Result =  m_pClient->GetLoginUserName(GetCurrentProcessId());
		auto lPackRead = std::make_shared<PackReader>(Result.get());

		if (lPackRead->ReadChar())
		{
			m_LoginUser = lPackRead->ReadStr();

			ACEInfoFileLog("[{}]LoginUser:{}",__FUNCTION__, m_LoginUser);

		}

	}
	catch (const RCF::Exception& e)
	{
		ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
		m_ErrID = e.getErrorId();
	}

	return m_LoginUser;
}

std::string CGameRCF::GetLoginUserPassWord()
{
	try
	{
		m_LoginUserPassWord = m_pClient->GetLoginUserPassWord();
		ACEInfoFileLog("[{}]LoginUserPassWord:{}", __FUNCTION__, m_LoginUserPassWord);

	}
	catch (const RCF::Exception& e)
	{
		ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
		m_ErrID = e.getErrorId();		
	}

	return m_LoginUserPassWord;
}

std::tuple<std::string, std::string> CGameRCF::GetLoginUserInfo()
{
	return std::make_tuple<std::string, std::string>(static_cast<std::string>( m_LoginUser),static_cast<std::string>(m_LoginUserPassWord));
}

bool CGameRCF::SendLoginEventNotice(LOGIN_NOTICE_CODE MsgID, std::string MsgStr)
{
	try
	{
		m_pClient->CSONMEventNotice(static_cast<ULONG>(MsgID), MsgStr);

	}
	catch (const RCF::Exception& e)
	{
		ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
		m_ErrID = e.getErrorId();

		return false;
	}
	return true;
}

BOOL CGameRCF::LauncherEventNotice()
{
	//get login guid
	try
	{
		m_Guid = m_pClient->GetEventGuid(GetCurrentProcessId());
		ACEInfoFileLog("[{}]guid:{}", __FUNCTION__, m_Guid);

	}
	catch (const RCF::Exception& e)
	{
		ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
		m_ErrID = e.getErrorId();
	}

	std::string Key = fmt::format("{}_AC_{}", ANTI_SDK_EVENT, m_Guid);

	auto  hEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, Key.data());

	BOOL bRet = FALSE;

	if (hEvent)
	{
		bRet = SetEvent(hEvent);
	}

	return bRet;
}

TASK_MODE CGameRCF::GetTaskMode()
{

	auto ThreadFuns = [&]()->ULONG
	{
		ULONG uTaskMode = 0;
		try
		{
			m_pClient->GetTaskMode(uTaskMode);

		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
		return uTaskMode;
	};
	auto result = m_WordThread->submit(ThreadFuns);

	ULONG uResult = result.get();

	ACEDebugLog("{} TaskMode:{}",__FUNCTION__, uResult);

	return static_cast<TASK_MODE>(uResult);
}

bool CGameRCF::SendUserInfo(std::string UserName, ULONG lv)
{
	auto ThreadFuns = [=]()
	{
		try
		{
			auto lPackWrite = std::make_shared<PackWriter>(1024 + UserName.length());

			lPackWrite->WriteByte(1);
			lPackWrite->WriteInt(lv);
			lPackWrite->WriteString(UserName);



			std::vector<BYTE> Buffer(lPackWrite->GetBytesLength());
			memcpy(Buffer.data(), lPackWrite->GetBytes(), lPackWrite->GetBytesLength());
			m_pClient->SendGameUserInfo(Buffer);

		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();			
		}
	};
	m_WordThread->push_task(ThreadFuns);
	
	return true;
}

bool CGameRCF::SendGoldInfo(ULONG64 Gold)
{
	auto ThreadFuns = [=]()
	{
		try
		{
			auto lPackWrite = std::make_shared<PackWriter>(1024);

			lPackWrite->WriteByte(2);
			lPackWrite->WriteULong64(Gold);

			std::vector<BYTE> Buffer(lPackWrite->GetBytesLength());
			memcpy(Buffer.data(), lPackWrite->GetBytes(), lPackWrite->GetBytesLength());
			m_pClient->SendGameUserInfo(Buffer);

		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};
	m_WordThread->push_task(ThreadFuns);
	
	return true;
}

bool CGameRCF::SendIntegralInfo(ULONG64 Integral)
{
	auto ThreadFuns = [=]()
	{
		try
		{
			auto lPackWrite = std::make_shared<PackWriter>(1024);

			lPackWrite->WriteByte(3);
			lPackWrite->WriteULong64(Integral);

			std::vector<BYTE> Buffer(lPackWrite->GetBytesLength());
			memcpy(Buffer.data(), lPackWrite->GetBytes(), lPackWrite->GetBytesLength());
			m_pClient->SendGameUserInfo(Buffer);
		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};
	
	m_WordThread->push_task(ThreadFuns);
	
	return true;
}

bool CGameRCF::SendHeartBeat()
{
	auto ThreadFuns = [&]()
	{
		try
		{
			m_pClient->Timestamp();
		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};
	m_WordThread->push_task(ThreadFuns);
	return true;
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& CGameRCF::GetAuctionTaskMap()
{

	auto ThreadFuns = [&]()
	{
		try
		{	
			auto Result = m_pClient->GetAuctionTask();
			auto lPackRead = std::make_shared<PackReader>(Result.get());

			ULONG uCount = lPackRead->ReadChar();
			
			for (size_t i = 0; i < uCount; i++)
			{
				PTAG_GAME_TASK_AUCTION pTask = new TAG_GAME_TASK_AUCTION();
				pTask->ulTaskID              = lPackRead->ReadInt();
				pTask->ulTaskType            = lPackRead->ReadInt();
				pTask->ItemID                = lPackRead->ReadShort();
				pTask->Price                 = lPackRead->ReadInt();
				pTask->BuyCount              = lPackRead->ReadInt();
				pTask->PaintType             = lPackRead->ReadChar();
				pTask->PaintCount            = lPackRead->ReadChar();
				
				for (size_t i = 0; i < pTask->PaintCount; i++)
				{
					pTask->PaintID[i]= lPackRead->ReadShort();
				}
				pTask->Enchantlv     = lPackRead->ReadInt();
				pTask->ItemAttribute = lPackRead->ReadInt();
				pTask->HitRate       = lPackRead->ReadChar();
				pTask->Damage        = lPackRead->ReadChar();
				pTask->Recoil        = lPackRead->ReadChar();
				pTask->Weight        = lPackRead->ReadChar();
				pTask->Bursts        = lPackRead->ReadChar();
				pTask->Clip          = lPackRead->ReadChar();
				pTask->TaskStatus    = lPackRead->ReadChar();

				pTask->BlackListItem = lPackRead->ReadChar();
				for (size_t i = 0; i < pTask->BlackListItem; i++)
				{
					pTask->BlackListItemID[i] = lPackRead->ReadShort();
				}
				
				pTask->BlackListPaint = lPackRead->ReadChar();
				for (size_t i = 0; i < pTask->BlackListPaint; i++)
				{
					pTask->BlackListPaintID[i] = lPackRead->ReadShort();
				}
				pTask->FindType      = lPackRead->ReadChar();
				pTask->PaintShow	 = lPackRead->ReadShort();
				
				//inster map
				m_AuctionTaskMap.insert(std::make_pair(pTask->ulTaskID, pTask));
			}



		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};

	//清空且回收内存
	for (auto &iter: m_AuctionTaskMap)
	{
		delete iter.second;
	}
	m_AuctionTaskMap.clear();

	auto result = m_WordThread->submit(ThreadFuns);

	result.wait();

	//ACEDebugLog("[{}] GetAuctionTaskMapSize:{}", __FUNCTION__, m_AuctionTaskMap.size());

	return m_AuctionTaskMap;
}

bool CGameRCF::SendAutionNotice(AuctionNotice Type, int nTask, PTAG_GAME_AUCTION pAution)
{
	auto ThreadFuns = [&]()
	{
		try
		{
			std::vector<BYTE> Buffer(4096,0);
			auto lPackWrite = std::make_shared<PackWriter>(Buffer);

			lPackWrite->WriteByte(static_cast<BYTE>(Type));
			lPackWrite->WriteInt(nTask);

			switch (Type)
			{
			case AuctionNotice::BuyItem_Succeed:		//购买成功后,显示购买的详细信息
			case AuctionNotice::Integral_Lack:			//脚本判断的,通知积分不足,并显示详细信息
			{
				lPackWrite->WriteByte(pAution->Row);
				lPackWrite->WriteInt(pAution->AuctionID);
				lPackWrite->WriteInt(pAution->AddedTimer);
				lPackWrite->WriteShort(pAution->ItemID);
				lPackWrite->WriteInt(pAution->Bidding);
				lPackWrite->WriteInt(pAution->Price);
				lPackWrite->WriteInt(pAution->RemainTime);
				lPackWrite->WriteShort(pAution->CurViewPaintID);
				lPackWrite->WriteShort(pAution->PaintCount);
				for (size_t i = 0; i < pAution->PaintCount; i++)
				{
					lPackWrite->WriteShort(pAution->PaintID[i]);
				}
				lPackWrite->WriteInt(pAution->ItemAttribute);
				lPackWrite->WriteByte(pAution->HitRate);
				lPackWrite->WriteByte(pAution->Damage);
				lPackWrite->WriteByte(pAution->Recoil);
				lPackWrite->WriteByte(pAution->Weight);
				lPackWrite->WriteByte(pAution->Bursts);
				lPackWrite->WriteByte(pAution->Clip);
				lPackWrite->WriteShort(pAution->Enchantlv);
			}
				break;
			case AuctionNotice::Integral_Lack_SV:
				// 只是通知而已
				break;
			case AuctionNotice::Mismatching_EnableEnchant:
				lPackWrite->WriteShort(pAution->ItemID);
				lPackWrite->WriteShort(pAution->Enchantlv);
				lPackWrite->WriteInt(pAution->AuctionID);
				break;
			case AuctionNotice::Mismatching_Paint_BlackList:
				lPackWrite->WriteShort(pAution->ItemID);
				lPackWrite->WriteShort(pAution->PaintCount);
				//黑名单中彩漆
				for (size_t i = 0; i < pAution->PaintCount; i++)
				{
					lPackWrite->WriteShort(pAution->PaintID[i]);
				}
				lPackWrite->WriteInt(pAution->AuctionID);
				break;
			case AuctionNotice::Mismatching_Item_BlackList:
				lPackWrite->WriteShort(pAution->ItemID);
				lPackWrite->WriteInt(pAution->AuctionID);
				break;
			default:
				break;
			}


			m_pClient->AuctionNotice(Buffer);
		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};


	auto result = m_WordThread->submit(ThreadFuns);
	result.wait();
	return true;
}

ULONG64 CGameRCF::GetShareAuctionEvents()
{
	ULONG64 Result = 0;
	
	auto ThreadFuns = [&]()
	{
		try
		{
			Result = m_pClient->GetAuctionEventsStatus();

			
			
		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	};
	auto result = m_WordThread->submit(ThreadFuns);
	result.wait();
	
	ACEDebugLog("[{}] Result:{}",__FUNCTION__, Result);

	return Result;
}

bool CGameRCF::SendAutionItem(std::map<ULONG, TAG_GAME_AUCTION>& Items, int Activate)
{
	ULONG64 Result = 0;

	auto result = m_WordThread->submit([&]() {
		try
		{
			std::vector<BYTE> Buffer(1024 + sizeof(TAG_GAME_AUCTION) * Items.size(), 0);
			auto lPackWrite = std::make_shared<PackWriter>(Buffer);

			lPackWrite->WriteByte(Items.size());
			lPackWrite->WriteByte(Activate);
			for (auto & iter : Items)
			{
				auto pAuction = &iter.second;
				
				lPackWrite->WriteByte(pAuction->Row);
				lPackWrite->WriteInt(pAuction->AuctionID);
				lPackWrite->WriteInt(pAuction->AddedTimer);
				lPackWrite->WriteShort(pAuction->ItemID);
				lPackWrite->WriteInt(pAuction->Bidding);
				lPackWrite->WriteInt(pAuction->Price);
				lPackWrite->WriteInt(pAuction->RemainTime);
				lPackWrite->WriteShort(pAuction->CurViewPaintID);
				lPackWrite->WriteShort(pAuction->PaintCount);
				for (size_t i = 0; i < pAuction->PaintCount; i++)
				{
					lPackWrite->WriteShort(pAuction->PaintID[i]);
				}
				lPackWrite->WriteInt(pAuction->ItemAttribute);
				lPackWrite->WriteByte(pAuction->HitRate);
				lPackWrite->WriteByte(pAuction->Damage);
				lPackWrite->WriteByte(pAuction->Recoil);
				lPackWrite->WriteByte(pAuction->Weight);
				lPackWrite->WriteByte(pAuction->Bursts);
				lPackWrite->WriteByte(pAuction->Clip);
				lPackWrite->WriteShort(pAuction->Enchantlv);
				lPackWrite->WriteInt(pAuction->Earnings);
			}

			Result = m_pClient->SendAuctionRackingItem(Buffer);

		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
			m_ErrID = e.getErrorId();
		}
	});
	result.wait();

	ACEDebugLog("[{}] Result:{}", __FUNCTION__, Result);

	return Result;
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& CGameRCF::GetAuctionClientTaskMap()
{
	return m_AuctionClientTaskMap;
}

thread_pool* CGameRCF::GetWorkThread()
{
	return m_WordThread;
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& CGameRCF::GetClientTaskAuction()
{
	return m_AuctionClientTaskMap;
}

int CGameRCF::GetErrID()
{
	return m_ErrID;
}

GameProxyClient::GameProxyClient()
{
	m_ClientThread = new thread_pool(1);
}

std::vector<BYTE> GameProxyClient::UMSG(std::vector<BYTE>& v)
{
	return std::vector<BYTE>();
}

std::vector<BYTE> GameProxyClient::GetClientAuctionMetadate()
{
	return CDispatchRecv::GetInstance()->GetAuctionMetadate();
}

std::vector<BYTE> GameProxyClient::SendClientAuctionTask(std::vector<BYTE>& v)
{
	ACEWarningFileLog("[{}]",__FUNCTION__);

	auto ThreadFuns = [&]()
	{
		try
		{
			auto SvRCF =  CGameRCF::GetInstance();

			auto lPackRead = std::make_shared<PackReader>(v);

			ULONG uCount   = lPackRead->ReadChar();
			m_ActiveWeapon = lPackRead->ReadChar();
			
			for (size_t i = 0; i < uCount; i++)
			{
				PTAG_GAME_TASK_AUCTION pTask = new TAG_GAME_TASK_AUCTION();
				pTask->ulTaskID              = lPackRead->ReadInt();
				pTask->ulTaskType            = lPackRead->ReadInt();
				pTask->ItemID                = lPackRead->ReadShort();
				pTask->Price                 = lPackRead->ReadInt();
				pTask->BuyCount              = lPackRead->ReadInt();
				pTask->PaintType             = lPackRead->ReadChar();
				pTask->PaintCount            = lPackRead->ReadChar();

				for (size_t i = 0; i < pTask->PaintCount; i++)
				{
					pTask->PaintID[i] = lPackRead->ReadShort();
				}
				pTask->Enchantlv     = lPackRead->ReadInt();
				pTask->ItemAttribute = lPackRead->ReadInt();
				pTask->HitRate       = lPackRead->ReadChar();
				pTask->Damage        = lPackRead->ReadChar();
				pTask->Recoil        = lPackRead->ReadChar();
				pTask->Weight        = lPackRead->ReadChar();
				pTask->Bursts        = lPackRead->ReadChar();
				pTask->Clip          = lPackRead->ReadChar();
				pTask->TaskStatus    = lPackRead->ReadChar();

				pTask->BlackListItem = lPackRead->ReadChar();
				for (size_t i = 0; i < pTask->BlackListItem; i++)
				{
					pTask->BlackListItemID[i] = lPackRead->ReadShort();
				}

				pTask->BlackListPaint = lPackRead->ReadChar();
				for (size_t i = 0; i < pTask->BlackListPaint; i++)
				{
					pTask->BlackListPaintID[i] = lPackRead->ReadShort();
				}
				pTask->FindType  = lPackRead->ReadChar();
				pTask->PaintShow = lPackRead->ReadShort();

				//inster map
				SvRCF->GetClientTaskAuction().insert(std::make_pair(pTask->ulTaskID, pTask));
			}



		}
		catch (const RCF::Exception& e)
		{
			ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());

		}
	};

	//清空且回收内存
	for (auto& iter : CGameRCF::GetInstance()->GetClientTaskAuction())
	{
		delete iter.second;
	}
	CGameRCF::GetInstance()->GetClientTaskAuction().clear();

	auto result = CGameRCF::GetInstance()->GetWorkThread()->submit(ThreadFuns);

	result.wait();
	

	if (CGameRCF::GetInstance()->GetClientTaskAuction().empty() == true)
	{
		ACEDebugLog("{}", "客户拍卖任务为空");
		
	}
	else
	{
		m_ClientThread->push_task([&]()
		{
			ACEDebugLog("{}", "执行扫描任务.开始");

			if (CGameRCF::GetInstance()->GetClientTaskAuction().count(m_ActiveWeapon))
			{
				auto& pTask = CGameRCF::GetInstance()->GetClientTaskAuction().at(m_ActiveWeapon);

				ACEDebugLog("{} ItemID:{} ID:{:X} FindType:{:X} Enchantlv:{:X}", __FUNCTION__, pTask->ItemID, pTask->Price, pTask->FindType, pTask->Enchantlv);

				if (pTask->ItemID)
				{
					Sleep(5 * 1000);

					if (CGameRCF::GetInstance()->GetClientTaskAuction().count(m_ActiveWeapon))
					{
						ACEDebugLog("{}", "尝试刷新！");

						CGamePack::GetInstance()->RefreshAuction(pTask->ItemID, pTask->FindType, pTask->Enchantlv, pTask->PaintShow);
					}

				}
			}
			else
			{

			}
		});
	}
	

	return std::vector<BYTE>();
}
