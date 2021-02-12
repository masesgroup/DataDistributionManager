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

#if !defined(DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_)
#define DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerCommon.h"

struct BaseKeepAlive
{
	int MessageLength;
	DDM_KEEPALIVE_TYPE Type;
	size_t ServerId;

	BaseKeepAlive(DDM_KEEPALIVE_TYPE type, size_t serverId)
	{
		MessageLength = sizeof(BaseKeepAlive);
		Type = type;
		ServerId = serverId;
	}
};

struct HELLO_WELCOME : BaseKeepAlive
{
	int64_t Uptime;
	char ServerName[256];
	char HostName[256];
	HELLO_WELCOME(DDM_KEEPALIVE_TYPE type, size_t serverId, const char* serverName, const char* hostname) : BaseKeepAlive(type, serverId)
	{
		MessageLength = sizeof(HELLO_WELCOME);
		memset(ServerName, 0, 256);
		memset(HostName, 0, 256);
		if (serverName)
		{
			strncpy(ServerName, serverName, min(256, strlen(serverName)));
		}
		if (hostname)
		{
			strncpy(HostName, hostname, min(256, strlen(hostname)));
		}
	}
};

struct GOODBYE : BaseKeepAlive
{
	GOODBYE(size_t serverId) : BaseKeepAlive(DDM_KEEPALIVE_TYPE::GOODBYE, serverId) {}
};

struct ALIVE : BaseKeepAlive
{
	int64_t Uptime;
	DDM_INSTANCE_STATE Status;
	ALIVE(size_t serverId, int64_t uptime, DDM_INSTANCE_STATE status) : BaseKeepAlive(DDM_KEEPALIVE_TYPE::ALIVE, serverId)
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
	STATECHANGEREQUEST(size_t serverId, int64_t requestId, DDM_INSTANCE_STATE futureStatus, int64_t uptime) :BaseKeepAlive(DDM_KEEPALIVE_TYPE::STATECHANGEREQUEST, serverId)
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
	STATECHANGERESPONSE(size_t serverId, int64_t requestId, BOOL accepted, int reason) :BaseKeepAlive(DDM_KEEPALIVE_TYPE::STATECHANGERESPONSE, serverId)
	{
		RequestId = requestId;
		Accepted = accepted;
		Reason = reason;
	}
};

class __declspec(dllexport) DataDistributionMastershipCommon : public IDataDistributionMastershipCommon, protected IDataDistributionChannelCallback, protected IDataDistributionLog
{
public:
	DataDistributionMastershipCommon();
	virtual ~DataDistributionMastershipCommon() {}
	HRESULT Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	virtual HRESULT Initialize();
	virtual HRESULT Start(DWORD dwMilliseconds);
	virtual HRESULT Stop(DWORD dwMilliseconds);
	virtual BOOL GetIamNextPrimary();
	virtual BOOL RequestIAmNextPrimary();
	virtual void GetClusterIndexes(int64_t arraElements[], size_t* length) = 0;
	virtual DDM_INSTANCE_STATE GetStateOf(int64_t serverId) = 0;
	virtual ClusterHealth GetClusterHealth() = 0;
	virtual DDM_INSTANCE_STATE GetMyState();
	virtual void ChangeMyState(DDM_INSTANCE_STATE);
	virtual void ChangeState(int64_t instanceId, DDM_INSTANCE_STATE);
	virtual int64_t GetLocalServerId();
	virtual int64_t GetPrimaryServerId();
	virtual int64_t GetMessageDelay();
	int64_t GetUpTime();
protected:
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...);
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args);
	virtual void SetLocalServerId(int64_t);
	virtual void SetPrimaryServerId(int64_t);
	void AddRandomToMyTime();

	virtual int SendKeepAlive();
	virtual void OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent);
	virtual void OnCondition(const char* channelName, DDM_UNDERLYING_ERROR_CONDITION condition, int nativeCode, const char* subSystemReason);
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
	IDataDistributionSubsystem* m_pDataDistributionManagerSubsystem;
	IDataDistributionMastershipCallback* m_pMastershipCallback;
	long long m_PrimaryKeepAliveDelay;
	int64_t m_PrimaryIdentifier;
	int64_t m_MyIdentifier;
	HANDLE m_hKeepAlive;
	int m_keepAliveInterval;
private:
	HANDLE  h_evtKeepAlive;
	BOOL	bKeepAliveRun;
	DWORD	dwKeepAliveThrId;
	HANDLE	hKeepAliveThread;
	static DWORD __stdcall keepAliveHandler(void * argh);
	CRITICAL_SECTION m_csState;
	SmartTimeMeasureWrapper m_startupTime;
	BOOL m_bSystemRunning;
};

#endif // end DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_