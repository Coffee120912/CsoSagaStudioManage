#include "QtRCFServer.h"
#include <RCF/ProxyEndpoint.hpp>

QtRCFServer::QtRCFServer(QObject *parent)
	: QObject(parent)
{
	m_RcfInit = nullptr;
	m_RcfServer = nullptr;
	m_StdioPipeInterface = nullptr;
}

QtRCFServer::~QtRCFServer()
{
	
}
bool QtRCFServer::onServiceAccess(int methodId)
{
	bool bRet = false;
	
	try
	{

		if (methodId == 0)
		{
			//methodId = 0 版本验证
			return true;
		}
		RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();
		CsoStudioSession& SagaSession = ConnectSession.getSessionObject<CsoStudioSession>(true);
		bRet = !SagaSession.GetSessionStr().empty();

		ACEDebugFileLog("[{}] MethodID:{} Session:{}  Ret:{}", __FUNCTION__, methodId, SagaSession.GetSessionStr(), bRet);


		if (bRet)
		{
			//验证 session
			std::string ClientSession = ConnectSession.getRequestUserData();

			if (SagaSession.GetSessionStr() != ClientSession)
			{
				ACEErrorFileLog("[{}] Session Check Fail MethID :{}", __FUNCTION__, methodId);

				return false;
			}

			if (methodId != 1)
			{
				bRet = SagaSession.GetShareInfo() != nullptr;
				ACEWarningFileLog("[{}] ACClient Check MethID :{}  result: {}", __FUNCTION__, methodId, bRet);

			}
		}

	}
	catch (const RCF::Exception& e)
	{
		ACEDebugLog("{} {}", e.getErrorMessage(), e.getErrorId());
	}

	return bRet;
}
bool QtRCFServer::start()
{
	VMProtectBegin(__FUNCTION__);
	
	try
	{
		m_RcfInit = new RCF::RcfInit();
		m_RcfServer = new RCF::RcfServer();
		m_StdioPipeInterface = new CsoStdioPipeInterface();

		m_RcfServer->addEndpoint(RCF::Win32NamedPipeEndpoint(xorstr_("SagaSvrPipe")));
#if 1
		m_RcfServer->addEndpoint(RCF::TcpEndpoint("0.0.0.0", 52122));
#endif
		
		m_RcfServer->setEnableProxyEndpoints(true);
		//设置线程池
		m_RcfServer->setThreadPool(RCF::ThreadPoolPtr(new RCF::ThreadPool(1)));

		auto Ptr = m_RcfServer->bind<I_CsoStdioPipe>(*m_StdioPipeInterface);

		//方法控制
		auto accessControl = [&](int methodId) { return onServiceAccess(methodId); };
		Ptr->setAccessControl(accessControl);


		m_RcfServer->getServerTransport().setMaxIncomingMessageLength(0x7FFFFFFF);
		m_RcfServer->start();
	}
	catch (const RCF::Exception& e)
	{
		auto Text = fmt::format("{} {}", e.getErrorMessage(), e.getErrorId());
	
		MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(Text).data(), 0, MB_ICONSTOP);
		TerminateProcess(INVALID_HANDLE_VALUE, EXIT_CODE);
	}
	VMProtectEnd();
	return true;
}

void QtRCFServer::SetLoginInterface(CLoginModule* Login)
{
	m_Login = Login;
	m_StdioPipeInterface->SetLoginModule(Login);
}

std::map<ULONG, PTAG_GAME_TASK_AUCTION>& QtRCFServer::GetAuctionTask()
{
	return m_StdioPipeInterface->GetAuctionTaskThis();
}

std::vector<BYTE> QtRCFServer::GetClientAuctionMetadate(std::string clientSession)
{
	
	RcfClient<I_GameProxyClient> client(RCF::ProxyEndpoint(*m_RcfServer, clientSession));
	
	RCF::Future<std::vector<BYTE>> fRet = client.GetClientAuctionMetadate();
	fRet.wait(5000);

	std::unique_ptr<RCF::Exception> ePtr = client.getClientStub().getAsyncException();

	if (ePtr)
	{
		auto Text = fmt::format("{} {}", ePtr->getErrorMessage(), ePtr->getErrorId());
		//MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(Text).data(), 0, MB_ICONSTOP);
		ACEDebugLog("[{}] {}",__FUNCTION__,Text);
		return std::vector<BYTE>();
	}
	std::vector<BYTE> Ret = fRet;
	return Ret;
}

void QtRCFServer::SendClientAuctionSearchTask()
{
	//查找符合的客户端


	auto& LocalProcess = m_Login->GetMapProcess();



	

	for (auto & iter : LocalProcess)
	{
		CsoStudioSession* pSession = static_cast<CsoStudioSession*>(iter.second->pSession);

		if (pSession)
		{



			RcfClient<I_GameProxyClient> client(RCF::ProxyEndpoint(*m_RcfServer, pSession->GetSessionStr()));
			
			std::vector<BYTE> Result(pSession->GetShareInfo()->pData->AuctionClientTask.size() * sizeof(TAG_GAME_TASK_AUCTION) + 1024, 0);
			
			auto lPackWriter = std::make_shared<PackWriter>(Result);

			int Count = 0;

			lPackWriter->WriteByte(Count);
			
			lPackWriter->WriteByte(pSession->GetShareInfo()->pData->AuctionActiveWeapon);

			for (auto& iter : pSession->GetShareInfo()->pData->AuctionClientTask)
			{

				auto& Task = iter.second;

				if (Task.ulTaskType == 3 && Task.ItemID != 0)
				{
					Count++;
					
					lPackWriter->WriteInt(Task.ulTaskID);
					lPackWriter->WriteInt(Task.ulTaskType);
					lPackWriter->WriteShort(Task.ItemID);
					lPackWriter->WriteInt(Task.Price);
					lPackWriter->WriteInt(Task.BuyCount);
					lPackWriter->WriteByte(Task.PaintType);
					lPackWriter->WriteByte(Task.PaintCount);

					for (size_t i = 0; i < Task.PaintCount; i++)
					{
						lPackWriter->WriteShort(Task.PaintID[i]);

					}
					lPackWriter->WriteInt(Task.Enchantlv);
					lPackWriter->WriteInt(Task.ItemAttribute);
					lPackWriter->WriteByte(Task.HitRate);
					lPackWriter->WriteByte(Task.Damage);
					lPackWriter->WriteByte(Task.Recoil);
					lPackWriter->WriteByte(Task.Weight);
					lPackWriter->WriteByte(Task.Bursts);
					lPackWriter->WriteByte(Task.Clip);
					lPackWriter->WriteByte(Task.TaskStatus);

					lPackWriter->WriteByte(Task.BlackListItem);
					for (size_t i = 0; i < Task.BlackListItem; i++)
					{
						lPackWriter->WriteShort(Task.BlackListItemID[i]);
					}

					lPackWriter->WriteByte(Task.BlackListPaint);
					for (size_t i = 0; i < Task.BlackListPaint; i++)
					{
						lPackWriter->WriteShort(Task.BlackListPaintID[i]);
					}

					lPackWriter->WriteByte(Task.FindType);

					lPackWriter->WriteShort(Task.PaintShow);

					

				}

			}
			

			
			lPackWriter->SetOffset(0);
			lPackWriter->WriteByte(Count);
			//发送
			client.SendClientAuctionTask(Result);

		}
		
		
	}

	





	



}
