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
	m_GlobalLogLevel = DDM_LOG_LEVEL::INFO_LEVEL;
	m_pDataDistributionManagerCallbacks = NULL;
}

DataDistributionCommon::~DataDistributionCommon()
{

}

OPERATION_RESULT DataDistributionCommon::ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len)
{
	return DataDistributionManagerImpl::ConvertConfFile(conf_file, arrayParams, len);
}

OPERATION_RESULT DataDistributionCommon::Initialize(IDataDistributionCallback* callbacks, const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	int len = 0;
	const char** arrayParams = NULL;

	if (!conf_file) conf_file = "datadistributionmanager.conf";
	m_confFile = _strdup(conf_file);
	OPERATION_RESULT res = ConvertConfFile(m_confFile, &arrayParams, &len);
	if (OPERATION_FAILED(res)) return res;

	return Initialize(callbacks, arrayParams, len, szMyAddress, channelTrailer);
}

OPERATION_RESULT DataDistributionCommon::Initialize(IDataDistributionCallback* callbacks, const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer)
{
	m_pDataDistributionManagerCallbacks = callbacks;
	m_ChannelTrailer = _strdup(channelTrailer);
	m_ServerName = _strdup(szMyAddress);
	m_arrayParams = arrayParams;
	m_arrayParamsLen = len;
	return Initialize();
}

OPERATION_RESULT DataDistributionCommon::Initialize()
{
	return DDM_NO_ERROR_CONDITION;
}

void DataDistributionCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...)
{
	if (NULL == m_pDataDistributionManagerCallbacks) return;
	if (m_GlobalLogLevel < level) return;

	va_list args = NULL;
	va_start(args, format);
	Log(level, sourceName, function, format, args);
	va_end(args);
}

#define LOG_LEN 1024
void DataDistributionCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args)
{
	if (NULL == m_pDataDistributionManagerCallbacks) return;
	if (m_GlobalLogLevel < level) return;

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

CHANNEL_HANDLE DataDistributionCommon::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	TRACESTART("DataDistributionCommon", "CreateChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return NULL;
}

OPERATION_RESULT DataDistributionCommon::StartChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "StartChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::StopChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "StopChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

void DataDistributionCommon::SetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName, const char* paramValue)
{
	TRACESTART("DataDistributionCommon", "SetParameter");
	CAST_CHANNEL(ChannelConfiguration);

	if (paramName == NULL || paramValue == NULL)
	{
		LOG_ERROR("Channel %s - INPUT PARAMETERS CANNOT BE NULL", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return;
	}

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
		else if (!strcmp(paramName, "datadistributionmanager.timeout.commit"))
		{
			pChannelConfiguration->SetCommitTimeout(atoi(paramValue));
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
		else if (!strcmp(paramName, "datadistributionmanager.event.sync"))
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
		else if (!strcmp(paramName, "datadistributionmanager.loglevel.global"))
		{
			SetGlobalLogLevel((DDM_LOG_LEVEL)atoi(paramValue));
			return;
		}
	}
}

void DataDistributionCommon::SetParameter(CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId, const char* paramValue)
{
	TRACESTART("DataDistributionCommon", "SetParameter");
	LOG_WARNING0("Not Implemented in subclass");
	CAST_CHANNEL(ChannelConfiguration);

	if (paramValue == NULL)
	{
		LOG_ERROR("Channel %s - INPUT PARAMETER CANNOT BE NULL", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return;
	}
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

const char* DataDistributionCommon::GetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName)
{
	TRACESTART("DataDistributionCommon", "GetParameter");
	CAST_CHANNEL(ChannelConfiguration);

	if (paramName == NULL)
	{
		LOG_ERROR("Channel %s - INPUT PARAMETER CANNOT BE NULL", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return NULL;
	}

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
		else if (!strcmp(paramName, "datadistributionmanager.timeout.commit"))
		{
			return ConvertIToA(pChannelConfiguration->GetCommitTimeout());
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
		else if (!strcmp(paramName, "datadistributionmanager.loglevel.global"))
		{
			return ConvertIToA((int)GetGlobalLogLevel());
		}
	}

	return NULL;
}

const char* DataDistributionCommon::GetParameter(CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId)
{
	TRACESTART("DataDistributionCommon", "GetParameter");
	LOG_WARNING0("Not Implemented in subclass");
	return NULL;
}

OPERATION_RESULT DataDistributionCommon::Lock(CHANNEL_HANDLE_PARAMETER, unsigned long timeout)
{
	TRACESTART("DataDistributionCommon", "Lock");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::Unlock(CHANNEL_HANDLE_PARAMETER)
{
	TRACESTART("DataDistributionCommon", "Unlock");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::SeekChannel(CHANNEL_HANDLE_PARAMETER, int64_t position)
{
	TRACESTART("DataDistributionCommon", "SeekChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::DeleteChannel(CHANNEL_HANDLE_PARAMETER)
{
	TRACESTART("DataDistributionCommon", "DeleteChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::WriteOnChannel(CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	TRACESTART("DataDistributionCommon", "WriteOnChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::ReadFromChannel(CHANNEL_HANDLE_PARAMETER, int64_t offset, size_t *dataLen, void **param)
{
	TRACESTART("DataDistributionCommon", "ReadFromChannel");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::ChangeChannelDirection(CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction)
{
	TRACESTART("DataDistributionCommon", "ChangeChannelDirection");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionCommon::Start(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");
	OPERATION_RESULT status = DDM_NO_ERROR_CONDITION;
	SetSubSystemStarted(TRUE);
	return status;
}

OPERATION_RESULT DataDistributionCommon::Stop(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionCommon", "Start");
	OPERATION_RESULT status = DDM_NO_ERROR_CONDITION;
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

std::string DataDistributionCommon::CheckConfigurationParameter(const char * channelName, std::string key, std::string value)
{
	if (m_pDataDistributionManagerCallbacks) return m_pDataDistributionManagerCallbacks->OnConfiguration(channelName, key.c_str(), value.c_str());
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

DDM_LOG_LEVEL DataDistributionCommon::GetGlobalLogLevel()
{
	return m_GlobalLogLevel;
}

void DataDistributionCommon::SetGlobalLogLevel(DDM_LOG_LEVEL level)
{
	m_GlobalLogLevel = level;
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
	m_EventSync = TRUE;
	m_CreateChannelTimeout = 10000;
	m_ChannelSeekTimeout = 10000;
	m_KeepAliveTimeout = 1000;
	m_MessageReceiveTimeout = 10000;
	m_ConsumerTimeout = 10;
	m_ProducerTimeout = 1;
	m_CommitTimeout = 10000;

	m_lastRoutedOffset = -1;
	m_lastManagedOffset = -1;

	m_pEvtStartupStatus = new DataDistributionEventWrapper();
	m_pEvtLockState = new DataDistributionEventWrapper();
	bLockState = FALSE;

	m_csFlags = new DataDistributionLockWrapper();
	m_csState = new DataDistributionLockWrapper();
	m_csOffsets = new DataDistributionLockWrapper();
}

const char* ChannelConfiguration::GetChannelName()
{
	return m_pChannelName;
}

GENERIC_HANDLE ChannelConfiguration::GetOpaqueHandle()
{
	return this;
}

DDM_CHANNEL_DIRECTION ChannelConfiguration::GetDirection()
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	return m_Direction;
}

void ChannelConfiguration::SetDirection(DDM_CHANNEL_DIRECTION direction)
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	m_Direction = direction;
}

DataDistributionCommon* ChannelConfiguration::GetManager()
{
	return m_pMainManager;
}

void ChannelConfiguration::OnDataAvailable(const char* key, size_t keyLen, void* buffer, size_t len)
{
	OnDataAvailable(this, key, keyLen, buffer, len);
}

void ChannelConfiguration::OnDataAvailable(const CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void* buffer, size_t len)
{
	UnderlyingEventData pData(m_pChannelName, key, keyLen, buffer, len);
	if (dataCb != NULL)
	{
		dataCb->OnUnderlyingEvent(this, &pData);
	}
}

void ChannelConfiguration::OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode, const char* subSystemReason, ...)
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

void ChannelConfiguration::OnConditionOrError(const CHANNEL_HANDLE_PARAMETER, OPERATION_RESULT errorCode, int nativeCode, const char* subSystemReason, ...)
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
	DataDistributionAutoLockWrapper lock(m_csOffsets);
	return m_lastManagedOffset;
}

void ChannelConfiguration::SetManagedOffset(int64_t val)
{
	DataDistributionAutoLockWrapper lock(m_csOffsets);
	m_lastManagedOffset = val;
}

OPERATION_RESULT ChannelConfiguration::WaitStartupStatus(unsigned long dwMilliseconds)
{
	return m_pEvtStartupStatus->Wait(dwMilliseconds);
}

void ChannelConfiguration::SetStartupStatus(CHANNEL_STARTUP_TYPE status)
{
	DataDistributionAutoLockWrapper lock(m_csState);
	Log(DDM_LOG_LEVEL::INFO_LEVEL, "SetStartupStatus", "Setting StartupStatus %d", status);
	if (!m_StartupStatusSet)
	{
		m_StartupStatus = status;
		DataDistributionAutoLockWrapper lock2(m_csFlags);
		m_StartupStatusSet = TRUE;
		m_pEvtStartupStatus->Set();
	}
}

CHANNEL_STARTUP_TYPE ChannelConfiguration::GetStartupStatus()
{
	DataDistributionAutoLockWrapper lock(m_csState);
	return m_StartupStatus;
}

BOOL ChannelConfiguration::IsStartupStatusSet()
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	return m_StartupStatusSet;
}

BOOL ChannelConfiguration::GetLockState()
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	return bLockState;
}

OPERATION_RESULT ChannelConfiguration::SetLockState()
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	bLockState = TRUE;
	m_pEvtLockState->Reset();
	return DDM_NO_ERROR_CONDITION;
}

OPERATION_RESULT ChannelConfiguration::ResetLockState()
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	bLockState = FALSE;
	m_pEvtLockState->Set();
	return DDM_NO_ERROR_CONDITION;
}

void ChannelConfiguration::WaitingFinishLockState(unsigned long dwMilliseconds)
{
	DataDistributionAutoLockWrapper lock(m_csFlags);
	if (bLockState)
	{
		Log(DDM_LOG_LEVEL::INFO_LEVEL, "WaitingFinishLockState", "Enter");
		m_pEvtLockState->Wait(dwMilliseconds);
		return;
	}
}

BOOL ChannelConfiguration::GetCommitSync() { return m_CommitSync; }
void ChannelConfiguration::SetCommitSync(BOOL val) { m_CommitSync = val; }

BOOL ChannelConfiguration::GetEventSync() { return m_EventSync; }
void ChannelConfiguration::SetEventSync(BOOL val) { m_EventSync = val; }

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

int  ChannelConfiguration::GetCommitTimeout() { return m_CommitTimeout; }
void ChannelConfiguration::SetCommitTimeout(int val) { m_CommitTimeout = val; }
