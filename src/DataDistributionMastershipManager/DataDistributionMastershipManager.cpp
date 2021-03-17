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

#include "DataDistributionMastershipManager.h"

#include "winsock2.h"

extern "C" DDM_EXPORT void* CreateObjectImplementation()
{
	return static_cast<void*> (new DataDistributionMastershipManager);
}

DataDistributionMastershipManager::DataDistributionMastershipManager() : DataDistributionMastershipCommon()
{
	m_csState = new DataDistributionLockWrapper();
}

DataDistributionMastershipManager::~DataDistributionMastershipManager()
{
	delete m_csState;
}

OPERATION_RESULT DataDistributionMastershipManager::Initialize(IDataDistributionMastershipCallback* cbs, const char* szMyAddress, const char* arrayParams[], int length)
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

	m_pMastershipCallback = cbs;

	std::hash<std::string> hash_fn;
	SetLocalServerId(hash_fn(server1Name));

	m_hKeepAlive = m_pDataDistributionManagerSubsystem->CreateChannel("KeepAlive", this);
	if (!m_hKeepAlive) return DDM_POINTER_NOT_SET;
	return DDM_NO_ERROR_CONDITION;
}

void DataDistributionMastershipManager::SetParameter(const char *paramName, const char *paramValue)
{
	TRACESTART("DataDistributionMastershipManager", "SetParameter");
}

const char *DataDistributionMastershipManager::GetParameter(const char *paramName)
{
	TRACESTART("DataDistributionMastershipManager", "GetParameter");
	return NULL;
}

OPERATION_RESULT DataDistributionMastershipManager::Start(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");

	if (m_hKeepAlive && m_pDataDistributionManagerSubsystem->StartChannel(m_hKeepAlive, dwMilliseconds))
	{
		m_tKeepAlive = new DataDistributionThreadWrapper(keepAliveHandler, this);
		return m_tKeepAlive->Start(dwMilliseconds);
	}

	return DDM_UNMAPPED_ERROR_CONDITION;
}


OPERATION_RESULT DataDistributionMastershipManager::Stop(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Stop");

	if (m_hKeepAlive)
	{
		return m_pDataDistributionManagerSubsystem->StopChannel(m_hKeepAlive, dwMilliseconds);
	}

	return DDM_UNMAPPED_ERROR_CONDITION;
}


BOOL DataDistributionMastershipManager::GetIamNextPrimary()
{
	DataDistributionAutoLockWrapper lock(m_csState);
	return m_IamNextPrimary;
}

int64_t DataDistributionMastershipManager::GetUpTime()
{
	return m_startupTime.ElapsedMicroseconds();
}

void DataDistributionMastershipManager::AddRandomToMyTime()
{
	unsigned int randomNum = rand();
	m_startupTime.AddNanoseconds(randomNum);
}

int DataDistributionMastershipManager::SendKeepAlive()
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

void DataDistributionMastershipManager::OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData* uEvent)
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

void DataDistributionMastershipManager::OnCondition(const char* channelName, OPERATION_RESULT condition, int nativeCode, const char* subSystemReason)
{

}

void DataDistributionMastershipManager::OnALIVE(ALIVE* pALIVE)
{

}

void DataDistributionMastershipManager::OnHELLO(HELLO_WELCOME* pHELLO_WELCOME)
{

}

void DataDistributionMastershipManager::OnWELCOME(HELLO_WELCOME* pHELLO_WELCOME)
{

}

void DataDistributionMastershipManager::OnGOODBYE(GOODBYE* pGOODBYE)
{

}

void DataDistributionMastershipManager::OnSTATECHANGEREQUEST(STATECHANGEREQUEST* pSTATECHANGEREQUEST)
{

}

void DataDistributionMastershipManager::OnSTATECHANGERESPONSE(STATECHANGERESPONSE* pSTATECHANGERESPONSE)
{

}

void DataDistributionMastershipManager::ChangeMyState(DDM_INSTANCE_STATE newState)
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

void DataDistributionMastershipManager::ChangeState(int64_t instanceId, DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionCommon", "ChangeState");
	LOG_INFO("Value changing on %d to %d", instanceId, newState);
}

DDM_INSTANCE_STATE DataDistributionMastershipManager::GetMyState()
{
	DataDistributionAutoLockWrapper lock(m_csState);
	TRACESTART("DataDistributionCommon", "GetMyState");
	return m_myState;
}

int64_t DataDistributionMastershipManager::GetMessageDelay()
{
	return m_PrimaryKeepAliveDelay;
}

BOOL DataDistributionMastershipManager::RequestIAmNextPrimary()
{
	return m_IamNextPrimary;
}

int64_t* DataDistributionMastershipManager::GetClusterIndexes(size_t* length)
{
	DataDistributionAutoLockWrapper lock(m_csState);

	ClusterHealthIterator it;

	*length = clusterState.size();
	int64_t* arraElements = (int64_t*)malloc(sizeof(int64_t) * (*length));
	size_t counter = 0;
	for (it = clusterState.begin(); it != clusterState.end(); ++it)
	{
		arraElements[counter] = it->first;
		counter++;
	}
	return arraElements;
}

DDM_INSTANCE_STATE DataDistributionMastershipManager::GetStateOf(int64_t serverId)
{
	DataDistributionAutoLockWrapper lock(m_csState);
	DDM_INSTANCE_STATE state = DDM_INSTANCE_STATE::UNKNOWN;
	ClusterHealthIterator it;
	auto elem = clusterState.at(serverId);
	if (elem != NULL)
	{
		state = elem->Status;
	}

	return state;
}

ClusterHealth DataDistributionMastershipManager::GetClusterHealth()
{
	return clusterState;
}

void DataDistributionMastershipManager::SetLocalServerId(int64_t identifier)
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

int64_t DataDistributionMastershipManager::GetLocalServerId()
{
	return m_MyIdentifier;
}

void DataDistributionMastershipManager::SetPrimaryServerId(int64_t primaryId)
{
	m_PrimaryIdentifier = primaryId;
}

int64_t DataDistributionMastershipManager::GetPrimaryServerId()
{
	return m_PrimaryIdentifier;
}

void FUNCALL DataDistributionMastershipManager::keepAliveHandler(ThreadWrapperArg *arg)
{
	DataDistributionMastershipManager* pDataDistributionMastershipCommon = static_cast<DataDistributionMastershipManager*>(arg->user_arg);
	arg->pEvent->Set();
	while (arg->bIsRunning)
	{
		int nextKeepAliveTrigger = pDataDistributionMastershipCommon->SendKeepAlive();
		Sleep(nextKeepAliveTrigger);
	}
}

