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

#include "DataDistributionManagerCommon.h"
#include "DataDistributionManager_Internal.h"

DataDistributionCommon::DataDistributionCommon()
{
	m_SubSystemStarted = FALSE;
	m_confFile = "";
	m_ChannelTrailer = "";
	m_ServerName = "";
	m_MaxMessageSize = 0;
	m_ServerLostTimeout = 10000;
	m_pDataDistributionManagerCallbacks = NULL;
}

DataDistributionCommon::~DataDistributionCommon()
{

}

HRESULT DataDistributionCommon::ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len)
{
	return DataDistributionManagerImpl::ConvertConfFile(conf_file, arrayParams, len);
}

HRESULT DataDistributionCommon::Initialize(IDataDistributionCallback* callbacks, const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	int len = 0;
	const char** arrayParams = NULL;

	if (!conf_file) conf_file = "datadistributionmanager.conf";
	m_confFile = _strdup(conf_file);
	HRESULT res = ConvertConfFile(m_confFile, &arrayParams, &len);
	if (res != NO_ERROR) return res;

	return Initialize(callbacks, arrayParams, len, szMyAddress, channelTrailer);
}

HRESULT DataDistributionCommon::Initialize(IDataDistributionCallback* callbacks, const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer)
{
	m_pDataDistributionManagerCallbacks = callbacks;
	m_ChannelTrailer = _strdup(channelTrailer);
	m_ServerName = _strdup(szMyAddress);
	m_arrayParams = arrayParams;
	m_arrayParamsLen = len;
	return Initialize();
}

HRESULT DataDistributionCommon::Initialize()
{
	return S_OK;
}

void DataDistributionCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...)
{
	if (NULL == m_pDataDistributionManagerCallbacks) return;

	va_list args = NULL;
	va_start(args, format);
	Log(level, sourceName, function, format, args);
	va_end(args);
}

#define LOG_LEN 1024
void DataDistributionCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args)
{
	if (NULL == m_pDataDistributionManagerCallbacks) return;

	char buffer[LOG_LEN];
	memset(buffer, 0, sizeof(buffer));
	int result = vsnprintf_s(buffer, _countof(buffer), _TRUNCATE, format, args);
	if (result > 0)
	{
		m_pDataDistributionManagerCallbacks->OnLogging(level, sourceName, function, buffer);
	}
	else
	{
		m_pDataDistributionManagerCallbacks->OnLogging(level, sourceName, function, "Failed to write log");
	}
}

size_t DataDistributionCommon::GetMaxMessageSize()
{
	return m_MaxMessageSize;
}

HANDLE DataDistributionCommon::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	TRACESTART("DataDistributionCommon", "CreateChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return NULL;
}

HRESULT DataDistributionCommon::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "StartChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return TRUE;
}

HRESULT DataDistributionCommon::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "StopChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return S_OK;
}

void DataDistributionCommon::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	TRACESTART("DataDistributionCommon", "SetParameter");
	CAST_CHANNEL(ChannelConfiguration)
	LOG_DEBUG("Channel %s - Writing %s with value %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", (paramName != NULL) ? paramName : "", (paramValue != NULL) ? paramValue : "");

	if (NULL != channelHandle)
	{
		// Non global params
		if (!strcmp(paramName, "datadistributionmanager.timeout.createchannel"))
		{
			pChannelConfiguration->SetCreateChannelTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
		{
			pChannelConfiguration->SetChannelSeekTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.receive"))
		{
			pChannelConfiguration->SetMessageReceiveTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
		{
			pChannelConfiguration->SetKeepAliveTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
		{
			pChannelConfiguration->SetConsumerTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
		{
			pChannelConfiguration->SetProducerTimeout(atoi(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
		{
			if (!strcmp(paramValue, "true") ||
				!strcmp(paramValue, "1"))
				pChannelConfiguration->SetCommitSync(TRUE);
			else
				pChannelConfiguration->SetCommitSync(FALSE);
			return;
		}
	}
	else
	{
		// if channel handle is NULL we are in Initialize and we need to get only the following parameters
		if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
		{
#if _WIN64
			SetMaxMessageSize(_atoi64(paramValue));
#else
			SetMaxMessageSize(atoi(paramValue));
#endif
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
		{
			SetServerLostTimeout(atoi(paramValue));
			return;
		}
	}
}

void DataDistributionCommon::SetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char* paramValue)
{
	TRACESTART("DataDistributionCommon", "SetParameter");
	LOG_WARNING0("Not Implemented in subclass");
}

static const char* ConvertIToA(int value)
{
	return _strdup(_itoa(value, NULL, 0));
}

static const char* ConvertIToA(size_t value)
{
#ifdef _WIN64
	return _strdup(_ui64toa(value, NULL, 0));
#else
	return _strdup(_itoa(value, NULL, 0));
#endif
}

const char* DataDistributionCommon::GetParameter(HANDLE channelHandle, const char* paramName)
{
	TRACESTART("DataDistributionCommon", "GetParameter");
	CAST_CHANNEL(ChannelConfiguration)
	LOG_DEBUG("Channel %s - Reading %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", (paramName != NULL) ? paramName : "");

	if (channelHandle != NULL)
	{
		if (!strcmp(paramName, "datadistributionmanager.CreateChannel"))
		{
			return ConvertIToA(pChannelConfiguration->GetCreateChannelTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
		{
			return ConvertIToA(pChannelConfiguration->GetChannelSeekTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.firstconnection"))
		{
			return ConvertIToA(pChannelConfiguration->GetMessageReceiveTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
		{
			return ConvertIToA(pChannelConfiguration->GetKeepAliveTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
		{
			return ConvertIToA(pChannelConfiguration->GetConsumerTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
		{
			return ConvertIToA(pChannelConfiguration->GetProducerTimeout());
		}
		else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
		{
			if (pChannelConfiguration->GetCommitSync()) return "true";
			else return "false";
		}
	}
	else
	{
		if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
		{
			return ConvertIToA(GetMaxMessageSize());
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
		{
			return ConvertIToA(GetServerLostTimeout());
		}
	}

	return NULL;
}

const char* DataDistributionCommon::GetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId)
{
	TRACESTART("DataDistributionCommon", "GetParameter");
	LOG_WARNING0("Not Implemented in subclass");
	return NULL;
}

HRESULT DataDistributionCommon::Lock(HANDLE channelHandle, DWORD timeout)
{
	TRACESTART("DataDistributionCommon", "Lock");
	LOG_WARNING0("Not Implemented in subclass");
	return E_FAIL;
}

HRESULT DataDistributionCommon::Unlock(HANDLE channelHandle)
{
	TRACESTART("DataDistributionCommon", "Unlock");
	LOG_WARNING0("Not Implemented in subclass");
	return E_FAIL;
}

HRESULT DataDistributionCommon::SeekChannel(HANDLE channelHandle, int64_t position)
{
	TRACESTART("DataDistributionCommon", "SeekChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return E_FAIL;
}

HRESULT DataDistributionCommon::DeleteChannel(HANDLE channelHandle)
{
	TRACESTART("DataDistributionCommon", "DeleteChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return E_FAIL;
}

HRESULT DataDistributionCommon::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	TRACESTART("DataDistributionCommon", "WriteOnChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return E_FAIL;
}

HRESULT DataDistributionCommon::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{
	TRACESTART("DataDistributionCommon", "ReadFromChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return S_FALSE;
}

HRESULT DataDistributionCommon::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	TRACESTART("DataDistributionCommon", "ChangeChannelDirection");
	LOG_WARNING0("Not Implemented in subclass");
	return S_FALSE;
}

HRESULT DataDistributionCommon::Start(DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");
	HRESULT status = S_OK;
	SetSubSystemStarted(TRUE);
	return status;
}

HRESULT DataDistributionCommon::Stop(DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");
	HRESULT status = S_OK;
	SetSubSystemStarted(FALSE);
	return status;
}

IDataDistributionCallback* DataDistributionCommon::GetCallbacks()
{
	return m_pDataDistributionManagerCallbacks;
}

int DataDistributionCommon::GetServerLostTimeout()
{
	return m_ServerLostTimeout;
}

std::string DataDistributionCommon::CheckConfigurationParameter(std::string key, std::string value)
{
	if (m_pDataDistributionManagerCallbacks) return m_pDataDistributionManagerCallbacks->OnConfiguration(key.c_str(), value.c_str());
	return value;
}

void DataDistributionCommon::SetSubSystemStarted(BOOL started)
{
	m_SubSystemStarted = started;
}

BOOL DataDistributionCommon::GetSubSystemStarted()
{
	return m_SubSystemStarted;
}

void DataDistributionCommon::SetMaxMessageSize(size_t maxMessageSize)
{
	m_MaxMessageSize = maxMessageSize;
}

void DataDistributionCommon::SetServerLostTimeout(int timeout)
{
	m_ServerLostTimeout = timeout;
}

std::string DataDistributionCommon::GetConfigFile() { return m_confFile; }
std::string DataDistributionCommon::GetChannelTrailer() { return (m_ChannelTrailer != NULL) ? m_ChannelTrailer : ""; }
std::string DataDistributionCommon::GetServerName() { return (m_ServerName != NULL) ? m_ServerName : ""; }
const char** DataDistributionCommon::GetArrayParams() { return m_arrayParams; }
int DataDistributionCommon::GetArrayParamsLen() { return m_arrayParamsLen; }

ChannelConfiguration::ChannelConfiguration(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionCommon* mainManager, IDataDistributionChannelCallback* Cb)
{
	m_pChannelName = _strdup(channelName);
	m_Direction = direction;
	m_pMainManager = mainManager;
	dataCb = Cb;

	m_StartupStatusSet = FALSE;
	m_StartupStatus = CHANNEL_STARTUP_TYPE::UNDEFINED;

	m_CommitSync = TRUE;
	m_CreateChannelTimeout = 10000;
	m_ChannelSeekTimeout = 10000;
	m_KeepAliveTimeout = 1000;
	m_MessageReceiveTimeout = 10000;
	m_ConsumerTimeout = 10;
	m_ProducerTimeout = 1;

	m_lastRoutedOffset = -1;
	m_lastManagedOffset = -1;

	h_evtStartupStatus = CreateEvent(0, true, false, NULL);
	h_evtLockState = CreateEvent(0, true, false, NULL);
	bLockState = FALSE;

	InitializeCriticalSection(&m_csFlags);
	InitializeCriticalSection(&m_csState);
	InitializeCriticalSection(&m_csOffsets);
}

const char* ChannelConfiguration::GetChannelName()
{
	return m_pChannelName;
}

DDM_CHANNEL_DIRECTION ChannelConfiguration::GetDirection()
{
	DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL;
	EnterCriticalSection(&m_csFlags);
	direction = m_Direction;
	LeaveCriticalSection(&m_csFlags);

	return direction;
}

void ChannelConfiguration::SetDirection(DDM_CHANNEL_DIRECTION direction)
{
	EnterCriticalSection(&m_csFlags);
	m_Direction = direction;
	LeaveCriticalSection(&m_csFlags);
}

DataDistributionCommon* ChannelConfiguration::GetManager()
{
	return m_pMainManager;
}

void ChannelConfiguration::OnDataAvailable(const char* key, size_t keyLen, void* buffer, size_t len)
{
	OnDataAvailable(this, key, keyLen, buffer, len);
}

void ChannelConfiguration::OnDataAvailable(const HANDLE channelHandle, const char* key, size_t keyLen, void* buffer, size_t len)
{
	UnderlyingEventData pData(m_pChannelName, key, keyLen, buffer, len);
	if (dataCb != NULL)
	{
		dataCb->OnUnderlyingEvent(this, &pData);
	}
}

void ChannelConfiguration::OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason, ...)
{
	va_list args = NULL;
	va_start(args, subSystemReason);
	UnderlyingEventData pData(m_pChannelName, errorCode, nativeCode, subSystemReason);
	if (dataCb != NULL)
	{
		dataCb->OnUnderlyingEvent(this, &pData);
	}
	va_end(args);
}

void ChannelConfiguration::OnConditionOrError(const HANDLE channelHandle, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason, ...)
{
	va_list args = NULL;
	va_start(args, subSystemReason);
	UnderlyingEventData pData(m_pChannelName, errorCode, nativeCode, subSystemReason);
	if (dataCb != NULL)
	{
		dataCb->OnUnderlyingEvent(this, &pData);
	}
	va_end(args);
}

void ChannelConfiguration::Log(DDM_LOG_LEVEL level, const char* function, const char* format, ...)
{
	va_list args = NULL;
	va_start(args, format);
	static_cast<DataDistributionCommon*>(m_pMainManager)->Log(level, m_pChannelName, function, format, args);
	va_end(args);
}

void ChannelConfiguration::CompletelyDisconnected()
{

}

int64_t ChannelConfiguration::GetManagedOffset()
{
	int64_t val;
	EnterCriticalSection(&m_csOffsets);
	val = m_lastManagedOffset;
	LeaveCriticalSection(&m_csOffsets);
	return val;
}

void ChannelConfiguration::SetManagedOffset(int64_t val)
{
	EnterCriticalSection(&m_csOffsets);
	m_lastManagedOffset = val;
	LeaveCriticalSection(&m_csOffsets);
}

DWORD ChannelConfiguration::WaitStartupStatus(DWORD dwMilliseconds)
{
	return WaitForSingleObject(h_evtStartupStatus, dwMilliseconds);
}

void ChannelConfiguration::SetStartupStatus(CHANNEL_STARTUP_TYPE status)
{
	EnterCriticalSection(&m_csState);
	Log(DDM_LOG_LEVEL::INFO_LEVEL, "SetStartupStatus", "Setting StartupStatus %d", status);
	if (!m_StartupStatusSet)
	{
		m_StartupStatus = status;
		EnterCriticalSection(&m_csFlags);
		m_StartupStatusSet = TRUE;
		LeaveCriticalSection(&m_csFlags);
		SetEvent(h_evtStartupStatus);
	}
	LeaveCriticalSection(&m_csState);
}

CHANNEL_STARTUP_TYPE ChannelConfiguration::GetStartupStatus()
{
	CHANNEL_STARTUP_TYPE status = CHANNEL_STARTUP_TYPE::UNDEFINED;
	EnterCriticalSection(&m_csState);
	status = m_StartupStatus;
	LeaveCriticalSection(&m_csState);
	return status;
}

BOOL ChannelConfiguration::IsStartupStatusSet()
{
	BOOL status = FALSE;
	EnterCriticalSection(&m_csFlags);
	status = m_StartupStatusSet;
	LeaveCriticalSection(&m_csFlags);
	return status;
}

BOOL ChannelConfiguration::GetLockState()
{
	BOOL status;
	EnterCriticalSection(&m_csFlags);
	status = bLockState;
	LeaveCriticalSection(&m_csFlags);
	return status;
}

HRESULT ChannelConfiguration::SetLockState()
{
	EnterCriticalSection(&m_csFlags);
	bLockState = TRUE;
	ResetEvent(h_evtLockState);
	LeaveCriticalSection(&m_csFlags);

	return S_OK;
}

HRESULT ChannelConfiguration::ResetLockState()
{
	EnterCriticalSection(&m_csFlags);
	bLockState = FALSE;
	SetEvent(h_evtLockState);
	LeaveCriticalSection(&m_csFlags);

	return S_OK;
}

void ChannelConfiguration::WaitingFinishLockState(DWORD dwMilliseconds)
{
	EnterCriticalSection(&m_csFlags);
	if (bLockState)
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "WaitingFinishLockState", "Enter");
		LeaveCriticalSection(&m_csFlags);
		WaitForSingleObject(h_evtLockState, dwMilliseconds);
		return;
	}
	LeaveCriticalSection(&m_csFlags);
}

BOOL ChannelConfiguration::GetCommitSync() { return m_CommitSync; }
void ChannelConfiguration::SetCommitSync(BOOL val) { m_CommitSync = val; }

int  ChannelConfiguration::GetCreateChannelTimeout() { return m_CreateChannelTimeout; }
void ChannelConfiguration::SetCreateChannelTimeout(int val) { m_CreateChannelTimeout = val; }

int  ChannelConfiguration::GetChannelSeekTimeout() { return m_ChannelSeekTimeout; }
void ChannelConfiguration::SetChannelSeekTimeout(int val) { m_ChannelSeekTimeout = val; }

int  ChannelConfiguration::GetKeepAliveTimeout() { return m_KeepAliveTimeout; }
void ChannelConfiguration::SetKeepAliveTimeout(int val) { m_KeepAliveTimeout = val; }

int  ChannelConfiguration::GetMessageReceiveTimeout() { return m_MessageReceiveTimeout; }
void ChannelConfiguration::SetMessageReceiveTimeout(int val) { m_MessageReceiveTimeout = val; }

int  ChannelConfiguration::GetConsumerTimeout() { return m_ConsumerTimeout; }
void ChannelConfiguration::SetConsumerTimeout(int val) { m_ConsumerTimeout = val; }

int  ChannelConfiguration::GetProducerTimeout() { return m_ProducerTimeout; }
void ChannelConfiguration::SetProducerTimeout(int val) { m_ProducerTimeout = val; }
