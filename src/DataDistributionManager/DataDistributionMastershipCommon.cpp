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
	InitializeCriticalSection(&m_csState);
	m_startupTime.ResetTime();
}

HRESULT DataDistributionMastershipCommon::Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress, const char* arrayParams[], int length)
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

HRESULT DataDistributionMastershipCommon::Initialize()
{
	TRACESTART("DataDistributionCommon", "Initialize");

	m_hKeepAlive = m_pDataDistributionManagerSubsystem->CreateChannel("KeepAlive", this);
	if (!m_hKeepAlive) return E_FAIL;
	return S_OK;
}

HRESULT DataDistributionMastershipCommon::Start(DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");

	if (m_hKeepAlive && m_pDataDistributionManagerSubsystem->StartChannel(m_hKeepAlive, dwMilliseconds))
	{
		HRESULT result = S_OK;
		bKeepAliveRun = TRUE;
		hKeepAliveThread = CreateThread(0, 0, keepAliveHandler, this, 0, &dwKeepAliveThrId);
		auto res = WaitForSingleObject(h_evtKeepAlive, dwMilliseconds);
		switch (res)
		{
		case WAIT_ABANDONED:
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			result = HRESULT_FROM_WIN32(res);
			break;
		case WAIT_OBJECT_0:
		default:
			break;
		}
		return result;
	}

	return E_FAIL;
}

HRESULT DataDistributionMastershipCommon::Stop(DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Stop");

	if (m_hKeepAlive && m_pDataDistributionManagerSubsystem->StopChannel(m_hKeepAlive, dwMilliseconds))
	{
		return S_OK;
	}

	return E_FAIL;
}


BOOL DataDistributionMastershipCommon::GetIamNextPrimary()
{
	BOOL status;
	EnterCriticalSection(&m_csState);
	status = m_IamNextPrimary;
	LeaveCriticalSection(&m_csState);
	return status;
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
	HRESULT hRes = m_pDataDistributionManagerSubsystem->WriteOnChannel(m_hKeepAlive, keepAlive.c_str(), keepAlive.size(), &alive, sizeof(ALIVE));

	if (FAILED(hRes))
	{
		LOG_ERROR0("WriteOnChannel failed");
	}

	return m_keepAliveInterval;
}

void DataDistributionMastershipCommon::OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent)
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

void DataDistributionMastershipCommon::OnCondition(const char* channelName, DDM_UNDERLYING_ERROR_CONDITION condition, int nativeCode, const char* subSystemReason)
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

	EnterCriticalSection(&m_csState);

	if (m_myState == DDM_INSTANCE_STATE::UNKNOWN && newState != DDM_INSTANCE_STATE::UNKNOWN)
	{
		m_pMastershipCallback->FirstStateChange(newState);
	}

	if (newState == m_myState)
	{
		LeaveCriticalSection(&m_csState);
		return;
	}

	m_pMastershipCallback->ChangingState(m_myState, newState);

	m_myState = newState;

	LeaveCriticalSection(&m_csState);

	m_pMastershipCallback->ChangedState(newState);
}

void DataDistributionMastershipCommon::ChangeState(int64_t instanceId, DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionCommon", "ChangeState");
	LOG_INFO("Value changing on %d to %d", instanceId, newState);
}

DDM_INSTANCE_STATE DataDistributionMastershipCommon::GetMyState()
{
	TRACESTART("DataDistributionCommon", "GetMyState");
	DDM_INSTANCE_STATE state = DDM_INSTANCE_STATE::UNKNOWN;
	EnterCriticalSection(&m_csState);
	state = m_myState;
	LeaveCriticalSection(&m_csState);
	return state;
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

DWORD __stdcall DataDistributionMastershipCommon::keepAliveHandler(void * argh)
{
	DataDistributionMastershipCommon* pDataDistributionMastershipCommon = static_cast<DataDistributionMastershipCommon*>(argh);

	SetEvent(pDataDistributionMastershipCommon->h_evtKeepAlive);

	while (pDataDistributionMastershipCommon->bKeepAliveRun)
	{
		int nextKeepAliveTrigger = pDataDistributionMastershipCommon->SendKeepAlive();
		Sleep(nextKeepAliveTrigger);
	}

	return S_OK;
}

