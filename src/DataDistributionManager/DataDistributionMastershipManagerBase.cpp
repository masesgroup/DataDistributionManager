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

#include "DataDistributionMastershipManagerBase.h"

DataDistributionMastershipManagerBase::DataDistributionMastershipManagerBase()
{
	InitializeCriticalSection(&m_csFlags);
}

DataDistributionMastershipManagerBase::~DataDistributionMastershipManagerBase()
{

}

void DataDistributionMastershipManagerBase::OnALIVE(ALIVE* pALIVE)
{
	if (pALIVE->ServerId == GetLocalServerId())
	{
		// discard without log enything
#if _DEBUG
		Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "DataDistributionMastershipManagerBase", "OnALIVE", "Discard my echo message");
#endif
		return;
	}

	if (clusterState.count(pALIVE->ServerId))
	{
		clusterState.at(pALIVE->ServerId)->Status = pALIVE->Status;
		clusterState.at(pALIVE->ServerId)->Uptime = pALIVE->Uptime;
		clusterState.at(pALIVE->ServerId)->LastContactTime = GetUpTime();
	}
	else
	{
		ClusterHealthElement* data = new ClusterHealthElement();
		data->Status = pALIVE->Status;
		data->Uptime = pALIVE->Uptime;
		data->LastContactTime = GetUpTime();
		clusterState.insert(std::pair<int64_t, ClusterHealthElement*>(pALIVE->ServerId, data));
		m_pMastershipCallback->OnClusterStateChange(DDM_CLUSTEREVENT::ADDSERVER, pALIVE->ServerId);
	}
	bool addRandomTime = false;
	long long myTime = GetUpTime();

	ClusterHealthIterator it;
	EnterCriticalSection(&m_csFlags);
	m_IamNextPrimary = TRUE;
	std::list<int64_t> listToRemove;
	for (it = clusterState.begin(); it != clusterState.end(); ++it)
	{
		if ((myTime - it->second->Uptime) > m_pDataDistributionManagerSubsystem->GetServerLostTimeout())
		{
			listToRemove.push_back(it->first);
		}
		else
		{
			m_IamNextPrimary &= myTime > it->second->Uptime;
			if (myTime == it->second->Uptime)
			{
				addRandomTime = true;
			}
		}
	}
	std::list<int64_t>::iterator it2;
	for (it2 = listToRemove.begin(); it2 != listToRemove.end(); ++it2)
	{
		delete clusterState.at(*it2);
		clusterState.erase(*it2);
		m_pMastershipCallback->OnClusterStateChange(DDM_CLUSTEREVENT::LOSTSERVER, *it2);
	}
	if (addRandomTime)
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionMastershipManagerBase", "OnALIVE", "Adding random time to my time");
		AddRandomToMyTime();
	}
	LeaveCriticalSection(&m_csFlags);
}

void DataDistributionMastershipManagerBase::OnHELLO_WELCOME(HELLO_WELCOME* pHELLO_WELCOME)
{
	if (pHELLO_WELCOME->ServerId == GetLocalServerId())
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionMastershipManagerBase", "OnHELLO_WELCOME", "Discard my echo message");
		return;
	}


}

void DataDistributionMastershipManagerBase::OnGOODBYE(GOODBYE* pGOODBYE)
{
	if (pGOODBYE->ServerId == GetLocalServerId())
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionMastershipManagerBase", "OnGOODBYE", "Discard my echo message");
		return;
	}


}

void DataDistributionMastershipManagerBase::OnSTATECHANGEREQUEST(STATECHANGEREQUEST* pSTATECHANGEREQUEST)
{
	if (pSTATECHANGEREQUEST->ServerId == GetLocalServerId())
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionMastershipManagerBase", "OnSTATECHANGEREQUEST", "Discard my echo request");
		return;
	}


}

void DataDistributionMastershipManagerBase::OnSTATECHANGERESPONSE(STATECHANGERESPONSE* pSTATECHANGERESPONSE)
{
	if (pSTATECHANGERESPONSE->ServerId == GetLocalServerId())
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionMastershipManagerBase", "OnSTATECHANGERESPONSE", "Discard my echo response");
		return;
	}


}

int64_t* DataDistributionMastershipManagerBase::GetClusterIndexes(size_t* length)
{
	ClusterHealthIterator it;

	EnterCriticalSection(&m_csFlags);
	*length = clusterState.size();
	int64_t* arraElements = (int64_t*)malloc(sizeof(int64_t) * (*length));
	size_t counter = 0;
	for (it = clusterState.begin(); it != clusterState.end(); ++it)
	{
		arraElements[counter] = it->first;
		counter++;
	}
	LeaveCriticalSection(&m_csFlags);
	return arraElements;
}

DDM_INSTANCE_STATE DataDistributionMastershipManagerBase::GetStateOf(int64_t serverId)
{
	DDM_INSTANCE_STATE state = DDM_INSTANCE_STATE::UNKNOWN;
	ClusterHealthIterator it;
	EnterCriticalSection(&m_csFlags);
	auto elem = clusterState.at(serverId);
	if (elem != NULL)
	{
		state = elem->Status;
	}
	LeaveCriticalSection(&m_csFlags);

	return state;
}

ClusterHealth DataDistributionMastershipManagerBase::GetClusterHealth()
{
	return clusterState;
}