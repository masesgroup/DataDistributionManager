/*
*  Copyright 2023 MASES s.r.l.
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

#if !defined(DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_)
#define DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionMastershipCommon.h"


typedef enum class DDM_KEEPALIVE_TYPE
{
	ALIVE = 0x1,
	HELLO = 0x2,
	WELCOME = 0x4,
	GOODBYE = 0x8,
	STATECHANGEREQUEST = 0x10,
	STATECHANGERESPONSE = 0x20,
} DDM_KEEPALIVE_TYPE;

struct BaseKeepAlive
{
	int MessageLength;
	DDM_KEEPALIVE_TYPE Type;
	int64_t ServerId;

	BaseKeepAlive(DDM_KEEPALIVE_TYPE type, int64_t serverId)
	{
		MessageLength = sizeof(BaseKeepAlive);
		Type = type;
		ServerId = serverId;
	}
};

struct HELLO_WELCOME : BaseKeepAlive
{
#define NAME_LENGTH_MAX 256

	int64_t Uptime;
	char ServerName[NAME_LENGTH_MAX];
	char HostName[NAME_LENGTH_MAX];
	HELLO_WELCOME(DDM_KEEPALIVE_TYPE type, size_t serverId, const char* serverName, const char* hostname) : BaseKeepAlive(type, serverId)
	{
		MessageLength = sizeof(HELLO_WELCOME);
		memset(ServerName, 0, NAME_LENGTH_MAX);
		memset(HostName, 0, NAME_LENGTH_MAX);
		if (serverName)
		{
			snprintf(ServerName, NAME_LENGTH_MAX, "%s", serverName);
		}
		if (hostname)
		{
			snprintf(HostName, NAME_LENGTH_MAX, "%s", hostname);
		}
	}
};

struct GOODBYE : BaseKeepAlive
{
	GOODBYE(int64_t serverId) : BaseKeepAlive(DDM_KEEPALIVE_TYPE::GOODBYE, serverId) {}
};

struct ALIVE : BaseKeepAlive
{
	int64_t Uptime;
	DDM_INSTANCE_STATE Status;
	ALIVE(int64_t serverId, int64_t uptime, DDM_INSTANCE_STATE status) : BaseKeepAlive(DDM_KEEPALIVE_TYPE::ALIVE, serverId)
	{
		Uptime = uptime;
		Status = status;
	}
};

struct STATECHANGEREQUEST : BaseKeepAlive
{
	int64_t RequestId;
	DDM_INSTANCE_STATE FutureStatus;
	int64_t Uptime;
	STATECHANGEREQUEST(int64_t serverId, int64_t requestId, DDM_INSTANCE_STATE futureStatus, int64_t uptime) :BaseKeepAlive(DDM_KEEPALIVE_TYPE::STATECHANGEREQUEST, serverId)
	{
		RequestId = requestId;
		FutureStatus = futureStatus;
		Uptime = uptime;
	}
};

struct STATECHANGERESPONSE : BaseKeepAlive
{
	int64_t RequestId;
	BOOL Accepted;
	int Reason;
	STATECHANGERESPONSE(int64_t serverId, int64_t requestId, BOOL accepted, int reason) :BaseKeepAlive(DDM_KEEPALIVE_TYPE::STATECHANGERESPONSE, serverId)
	{
		RequestId = requestId;
		Accepted = accepted;
		Reason = reason;
	}
};

class DataDistributionMastershipManager : public DataDistributionMastershipCommon, protected IDataDistributionChannelCallback
{
public:
	DataDistributionMastershipManager();
	virtual ~DataDistributionMastershipManager();
	OPERATION_RESULT Initialize(IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	void SetParameter(const char *paramName, const char *paramValue);
	const char *GetParameter(const char *paramName);
	OPERATION_RESULT Start(unsigned long dwMilliseconds);
	OPERATION_RESULT Stop(unsigned long dwMilliseconds);
	BOOL GetIamNextPrimary();
	BOOL RequestIAmNextPrimary();
	int64_t* GetClusterIndexes(size_t* length);
	DDM_INSTANCE_STATE GetStateOf(int64_t serverId);
	ClusterHealth GetClusterHealth();
	DDM_INSTANCE_STATE GetMyState();
	void ChangeMyState(DDM_INSTANCE_STATE newState);
	void ChangeState(int64_t instanceId, DDM_INSTANCE_STATE);
	int64_t GetLocalServerId();
	int64_t GetPrimaryServerId();
	int64_t GetMessageDelay();
	int64_t GetUpTime();
protected:
	virtual void SetLocalServerId(int64_t);
	virtual void SetPrimaryServerId(int64_t);
	void AddRandomToMyTime();

	virtual int SendKeepAlive();
	virtual void OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData* uEvent);
	virtual void OnCondition(const char* channelName, OPERATION_RESULT condition, int nativeCode, const char* subSystemReason);
	virtual void OnALIVE(ALIVE* pALIVE);
	virtual void OnHELLO(HELLO_WELCOME* pHELLO_WELCOME);
	virtual void OnWELCOME(HELLO_WELCOME* pHELLO_WELCOME);
	virtual void OnGOODBYE(GOODBYE* pGOODBYE);
	virtual void OnSTATECHANGEREQUEST(STATECHANGEREQUEST* pSTATECHANGEREQUEST);
	virtual void OnSTATECHANGERESPONSE(STATECHANGERESPONSE* pSTATECHANGERESPONSE);
protected:
	const char* m_szServerName;
	const char** m_arrayParams;
	int m_arrayParamsLen;
	DDM_INSTANCE_STATE m_myState;
	BOOL m_IamNextPrimary;
	IDataDistributionMastershipCallback* m_pMastershipCallback;
	long long m_PrimaryKeepAliveDelay;
	int64_t m_PrimaryIdentifier;
	int64_t m_MyIdentifier;
	CHANNEL_HANDLE m_hKeepAlive;
	int m_keepAliveInterval;
private:
	DataDistributionThreadWrapper* m_tKeepAlive;
	ClusterHealth clusterState;
	static void FUNCALL keepAliveHandler(ThreadWrapperArg *arg);
	DataDistributionLockWrapper* m_csState;
	SmartTimeMeasureWrapper m_startupTime;
	BOOL m_bSystemRunning;
};

#endif // !defined(DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_)