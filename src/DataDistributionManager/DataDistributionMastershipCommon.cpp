/*
*  Copyright 2021 MASES s.r.l.
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*  Refer to LICENSE for more information.
*/

#include "DataDistributionMastershipCommon.h"

DataDistributionMastershipCommon::DataDistributionMastershipCommon()
{
	m_keepAliveInterval = 1000;
	m_hKeepAlive = NULL;
	m_bSystemRunning = FALSE;
	m_MyIdentifier = 0;
	m_IamNextPrimary = FALSE;
	m_myState = DDM_INSTANCE_STATE::UNKNOWN;
	m_tKeepAlive = NULL;
	m_csState = new DataDistributionLockWrapper();
	m_startupTime.ResetTime();
}

OPERATION_RESULT DataDistributionMastershipCommon::Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress, const char* arrayParams[], int length)
{
	std::string server1Name;

	if (!szMyAddress)
	{
		char szHost[255];
		szHost[0] = '0';
		gethostname(szHost, sizeof(szHost));
		m_szServerName = _strdup(szHost);
	}
	else m_szServerName = _strdup(szMyAddress);

	m_arrayParamsLen = (arrayParams != NULL) ? length : 0;
	if (arrayParams != NULL)
	{
		m_arrayParams = (const char**)malloc(m_arrayParamsLen * sizeof(const char*));
		for (int i = 0; i < m_arrayParamsLen; i++)
		{
			m_arrayParams[i] = _strdup(arrayParams[i]);
		}
	}

	m_pDataDistributionManagerSubsystem = transportManager;
	m_pMastershipCallback = cbs;

	std::hash<std::string> hash_fn;
	SetLocalServerId(hash_fn(server1Name));

	return Initialize();
}

OPERATION_RESULT DataDistributionMastershipCommon::Initialize()
{
	TRACESTART("DataDistributionCommon", "Initialize");

	m_hKeepAlive = m_pDataDistributionManagerSubsystem->CreateChannel("KeepAlive", this);
	if (!m_hKeepAlive) return DDM_POINTER_NOT_SET;
	return DDM_NO_ERROR_CONDITION;
}

OPERATION_RESULT DataDistributionMastershipCommon::Start(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");

	if (m_hKeepAlive && m_pDataDistributionManagerSubsystem->StartChannel(m_hKeepAlive, dwMilliseconds))
	{
		m_tKeepAlive = new DataDistributionThreadWrapper(keepAliveHandler, this);
		return m_tKeepAlive->Start(dwMilliseconds);
	}

	return DDM_UNMAPPED_ERROR_CONDITION;
}

OPERATION_RESULT DataDistributionMastershipCommon::Stop(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Stop");

	if (m_hKeepAlive)
	{
		return m_pDataDistributionManagerSubsystem->StopChannel(m_hKeepAlive, dwMilliseconds);
	}

	return DDM_UNMAPPED_ERROR_CONDITION;
}


BOOL DataDistributionMastershipCommon::GetIamNextPrimary()
{
	DataDistributionAutoLockWrapper lock(m_csState);
	return m_IamNextPrimary;
}

int64_t DataDistributionMastershipCommon::GetUpTime()
{
	return m_startupTime.ElapsedMicroseconds();
}

void DataDistributionMastershipCommon::AddRandomToMyTime()
{
	unsigned int randomNum;
	rand_s(&randomNum);
	m_startupTime.AddNanoseconds(randomNum);
}

int DataDistributionMastershipCommon::SendKeepAlive()
{
	TRACESTART("DataDistributionCommon", "SendKeepAlive");

	ALIVE alive(m_MyIdentifier, GetUpTime(), m_myState);
	std::string keepAlive("KeepAlive");
	OPERATION_RESULT hRes = m_pDataDistributionManagerSubsystem->WriteOnChannel(m_hKeepAlive, keepAlive.c_str(), keepAlive.size(), &alive, sizeof(ALIVE));

	if (OPERATION_FAILED(hRes))
	{
		LOG_ERROR0("WriteOnChannel failed");
	}

	return m_keepAliveInterval;
}

void DataDistributionMastershipCommon::OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData* uEvent)
{
	TRACESTART("DataDistributionCommon", "OnUnderlyingEvent");

	if (uEvent->IsDataAvailable)
	{
		std::string key(uEvent->Key, uEvent->KeyLen);
		if (key != "KeepAlive")
		{
			LOG_ERROR("Received an event with key %s", key.c_str());
			return;
		}

		BaseKeepAlive* pBaseKeepAlive = (BaseKeepAlive*)uEvent->Buffer;

		switch (pBaseKeepAlive->Type)
		{
		case DDM_KEEPALIVE_TYPE::ALIVE:
			OnALIVE((ALIVE*)pBaseKeepAlive);
			break;
		case DDM_KEEPALIVE_TYPE::HELLO:
			OnHELLO((HELLO_WELCOME*)pBaseKeepAlive);
			break;
		case DDM_KEEPALIVE_TYPE::WELCOME:
			OnWELCOME((HELLO_WELCOME*)pBaseKeepAlive);
			break;
		case DDM_KEEPALIVE_TYPE::GOODBYE:
			OnGOODBYE((GOODBYE*)pBaseKeepAlive);
			break;
		case DDM_KEEPALIVE_TYPE::STATECHANGEREQUEST:
			OnSTATECHANGEREQUEST((STATECHANGEREQUEST*)pBaseKeepAlive);
			break;
		case DDM_KEEPALIVE_TYPE::STATECHANGERESPONSE:
			OnSTATECHANGERESPONSE((STATECHANGERESPONSE*)pBaseKeepAlive);
			break;
		default:
			break;
		}
	}
	else OnCondition(uEvent->ChannelName, uEvent->Condition, uEvent->NativeCode, uEvent->SubSystemReason);
}

void DataDistributionMastershipCommon::OnCondition(const char* channelName, OPERATION_RESULT condition, int nativeCode, const char* subSystemReason)
{

}

void DataDistributionMastershipCommon::OnALIVE(ALIVE* pALIVE)
{

}

void DataDistributionMastershipCommon::OnHELLO(HELLO_WELCOME* pHELLO_WELCOME)
{

}

void DataDistributionMastershipCommon::OnWELCOME(HELLO_WELCOME* pHELLO_WELCOME)
{

}

void DataDistributionMastershipCommon::OnGOODBYE(GOODBYE* pGOODBYE)
{

}

void DataDistributionMastershipCommon::OnSTATECHANGEREQUEST(STATECHANGEREQUEST* pSTATECHANGEREQUEST)
{

}

void DataDistributionMastershipCommon::OnSTATECHANGERESPONSE(STATECHANGERESPONSE* pSTATECHANGERESPONSE)
{

}

void DataDistributionMastershipCommon::ChangeMyState(DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionCommon", "ChangeMyState");
	LOG_INFO("Value changing from %d to %d", m_myState, newState);

	m_csState->Lock();

	if (m_myState == DDM_INSTANCE_STATE::UNKNOWN && newState != DDM_INSTANCE_STATE::UNKNOWN)
	{
		m_pMastershipCallback->FirstStateChange(newState);
	}

	if (newState == m_myState)
	{
		m_csState->Unlock();
		return;
	}

	m_pMastershipCallback->ChangingState(m_myState, newState);

	m_myState = newState;

	m_csState->Unlock();

	m_pMastershipCallback->ChangedState(newState);
}

void DataDistributionMastershipCommon::ChangeState(int64_t instanceId, DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionCommon", "ChangeState");
	LOG_INFO("Value changing on %d to %d", instanceId, newState);
}

DDM_INSTANCE_STATE DataDistributionMastershipCommon::GetMyState()
{
	DataDistributionAutoLockWrapper lock(m_csState);
	TRACESTART("DataDistributionCommon", "GetMyState");
	return m_myState;
}

int64_t DataDistributionMastershipCommon::GetMessageDelay()
{
	return m_PrimaryKeepAliveDelay;
}

BOOL DataDistributionMastershipCommon::RequestIAmNextPrimary()
{
	return m_IamNextPrimary;
}

void DataDistributionMastershipCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...)
{
	if (NULL == m_pDataDistributionManagerSubsystem) return;

	va_list args = NULL;
	va_start(args, format);
	Log(level, sourceName, function, format, args);
	va_end(args);
}

void DataDistributionMastershipCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args)
{
	if (NULL == m_pDataDistributionManagerSubsystem) return;
	m_pDataDistributionManagerSubsystem->Log(level, sourceName, function, format, args);
}

void DataDistributionMastershipCommon::SetLocalServerId(int64_t identifier)
{
	TRACESTART("DataDistributionCommon", "SetLocalServerId");
	if (m_bSystemRunning)
	{
		LOG_INFO0("Cannot change identifier during run.");
		return;
	}

#ifdef _WIN64
	LOG_INFO("New identifier is %lld", identifier);
#else
	LOG_INFO("New identifier is %d", identifier);
#endif

	m_MyIdentifier = identifier;
}

int64_t DataDistributionMastershipCommon::GetLocalServerId()
{
	return m_MyIdentifier;
}

void DataDistributionMastershipCommon::SetPrimaryServerId(int64_t primaryId)
{
	m_PrimaryIdentifier = primaryId;
}

int64_t DataDistributionMastershipCommon::GetPrimaryServerId()
{
	return m_PrimaryIdentifier;
}

void FUNCALL DataDistributionMastershipCommon::keepAliveHandler(ThreadWrapperArg *arg)
{
	DataDistributionMastershipCommon* pDataDistributionMastershipCommon = static_cast<DataDistributionMastershipCommon*>(arg->user_arg);
	arg->pEvent->Set();
	while (arg->bIsRunning)
	{
		int nextKeepAliveTrigger = pDataDistributionMastershipCommon->SendKeepAlive();
		Sleep(nextKeepAliveTrigger);
	}
}

