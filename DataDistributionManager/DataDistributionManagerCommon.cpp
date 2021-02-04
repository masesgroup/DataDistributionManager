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
	m_confFile = "";
	m_ChannelTrailer = "";
	m_ServerName = "";
	m_MaxMessageSize = 0;
	m_pDataDistributionManagerCallbacks = NULL;
}

DataDistributionCommon::~DataDistributionCommon()
{

}

HRESULT DataDistributionCommon::ConvertConfFile(const char* conf_file,  const char*** arrayParams, int* len)
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
	va_list args = NULL;
	va_start(args, format);
	Log(level, sourceName, function, format, args);
	va_end(args);
}

#define LOG_LEN 1024
void DataDistributionCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args)
{
	char buffer[LOG_LEN];
	vsnprintf_s(buffer, LOG_LEN, format, args);
	if (m_pDataDistributionManagerCallbacks) m_pDataDistributionManagerCallbacks->OnLogging(level, sourceName, function, buffer);
}

size_t DataDistributionCommon::GetMaxMessageSize() 
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "GetMaxMessageSize", "Not Implemented in subclass"); 
	return 0; 
}

HANDLE DataDistributionCommon::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len) 
{
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "CreateChannel", "Not Implemented in subclass");
	return NULL;
}

HRESULT DataDistributionCommon::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds) 
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "StartChannel", "Not Implemented in subclass"); 
	return TRUE; 
}

HRESULT DataDistributionCommon::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds) 
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "StopChannel", "Not Implemented in subclass");
	return S_OK; 
}

void DataDistributionCommon::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "SetParameter", "Not Implemented in subclass");
}

void DataDistributionCommon::SetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char* paramValue)
{
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "SetParameter", "Not Implemented in subclass");
}

const char* DataDistributionCommon::GetParameter(HANDLE channelHandle, const char* paramName)
{
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "GetParameter", "Not Implemented in subclass");
	return NULL;
}

const char* DataDistributionCommon::GetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId)
{
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "GetParameter", "Not Implemented in subclass");
	return NULL;
}
HRESULT DataDistributionCommon::Lock(HANDLE channelHandle, DWORD timeout) { Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "Lock", "Not Implemented in subclass"); return E_FAIL; }
HRESULT DataDistributionCommon::Unlock(HANDLE channelHandle) { Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "Unlock", "Not Implemented in subclass"); return E_FAIL; }
HRESULT DataDistributionCommon::SeekChannel(HANDLE channelHandle, int64_t position) { Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "SeekChannel", "Not Implemented in subclass"); return E_FAIL; }
HRESULT DataDistributionCommon::DeleteChannel(HANDLE channelHandle) { Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "DeleteChannel", "Not Implemented in subclass"); return E_FAIL; }
HRESULT DataDistributionCommon::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "WriteOnChannel", "Not Implemented in subclass");
	return E_FAIL; 
}

HRESULT DataDistributionCommon::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "ReadFromChannel", "Not Implemented in subclass"); 
	return FALSE; 
}

HRESULT DataDistributionCommon::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{ 
	Log(DDM_LOG_LEVEL::WARNING_LEVEL, "DataDistributionCommon", "ChangeChannelDirection", "Not Implemented in subclass");
	return FALSE; 
}

HRESULT DataDistributionCommon::Start(DWORD dwMilliseconds)
{
	HRESULT status = S_OK;
	Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "NoChannel", "Start", "Enter");
	SetSubSystemStarted(TRUE);
	Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "NoChannel", "Start", "Exit");
	return status;
}

HRESULT DataDistributionCommon::Stop(DWORD dwMilliseconds)
{
	HRESULT status = S_OK;
	Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "NoChannel", "Stop", "Enter");
	SetSubSystemStarted(FALSE);
	Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "NoChannel", "Stop", "Exit");
	return status;
}

IDataDistributionCallback* DataDistributionCommon::GetCallbacks()
{
	return m_pDataDistributionManagerCallbacks;
}

int DataDistributionCommon::GetServerLostTimeout()
{
	return 0;
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

ChannelConfiguration::ChannelConfiguration(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionCommon* mainManager, IDataDistributionChannelCallback* Cb)
{
	m_pChannelName = _strdup(channelName);
	m_Direction = direction;
	pMainManager = mainManager;
	dataCb = Cb;

	m_StartupStatusSet = FALSE;
	m_StartupStatus = CHANNEL_STARTUP_TYPE::UNDEFINED;

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

void ChannelConfiguration::OnDataAvailable(const char* key, size_t keyLen,  void* buffer, size_t len)
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

void ChannelConfiguration::OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason)
{
	OnConditionOrError(this, errorCode, nativeCode, subSystemReason);
}

void ChannelConfiguration::OnConditionOrError(const HANDLE channelHandle, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason)
{
	UnderlyingEventData pData(m_pChannelName, errorCode, nativeCode, subSystemReason);
	if (dataCb != NULL)
	{
		dataCb->OnUnderlyingEvent(this, &pData);
	}
}

void ChannelConfiguration::Log(DDM_LOG_LEVEL level, const char* function, const char* format, ...)
{
	va_list args = NULL;
	va_start(args, format);
	static_cast<DataDistributionCommon*>(pMainManager)->Log(level, m_pChannelName, function, format, args);
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

BOOL ChannelConfiguration::GetStartupStatusSet()
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


