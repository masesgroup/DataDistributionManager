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

#include "SmartDataDistributionManager.h"

SmartTimeMeasureWrapper::SmartTimeMeasureWrapper()
{
	m_pITimeMeasureWrapper = TimeMeasureWrapper::create();
}

SmartTimeMeasureWrapper::~SmartTimeMeasureWrapper()
{
	if (m_pITimeMeasureWrapper != NULL) delete m_pITimeMeasureWrapper;
}

void SmartTimeMeasureWrapper::ResetTime()
{
	m_pITimeMeasureWrapper->ResetTime();
}

int64_t SmartTimeMeasureWrapper::ElapsedMilliseconds()
{
	return m_pITimeMeasureWrapper->ElapsedMilliseconds();
}

int64_t SmartTimeMeasureWrapper::ElapsedMicroseconds()
{
	return m_pITimeMeasureWrapper->ElapsedMicroseconds();
}

int64_t SmartTimeMeasureWrapper::ElapsedNanoseconds()
{
	return m_pITimeMeasureWrapper->ElapsedNanoseconds();
}

void SmartTimeMeasureWrapper::AddNanoseconds(unsigned int time)
{
	m_pITimeMeasureWrapper->AddNanoseconds(time);
}

SmartDataDistributionChannel::SmartDataDistributionChannel()
{
	m_channelHandle = NULL;
	m_pIDataDistributionChannelBase = NULL;
}

SmartDataDistributionChannel::~SmartDataDistributionChannel()
{
	if (m_pIDataDistributionChannelBase != NULL) m_pIDataDistributionChannelBase->DeleteChannel(m_channelHandle);
}

void SmartDataDistributionChannel::SetInformation(const char* channelName, IDataDistributionChannelBase* pIDataDistributionChannelBase, HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	m_ChannelName = _strdup(channelName);
	m_Direction = direction;
	m_channelHandle = channelHandle;
	m_pIDataDistributionChannelBase = pIDataDistributionChannelBase;
}

DDM_CHANNEL_DIRECTION SmartDataDistributionChannel::GetDirection()
{
	return m_Direction;
}

HRESULT SmartDataDistributionChannel::StartChannel(DWORD dwMilliseconds)
{
	return m_pIDataDistributionChannelBase->StartChannel(m_channelHandle, dwMilliseconds);
}

HRESULT SmartDataDistributionChannel::StopChannel(DWORD dwMilliseconds)
{
	return m_pIDataDistributionChannelBase->StopChannel(m_channelHandle, dwMilliseconds);
}

HRESULT SmartDataDistributionChannel::Lock(DWORD timeout)
{
	return m_pIDataDistributionChannelBase->Lock(m_channelHandle, timeout);
}

HRESULT SmartDataDistributionChannel::Unlock()
{
	return m_pIDataDistributionChannelBase->Unlock(m_channelHandle);
}

HRESULT SmartDataDistributionChannel::SeekChannel(int64_t position)
{
	return m_pIDataDistributionChannelBase->SeekChannel(m_channelHandle, position);
}

HRESULT SmartDataDistributionChannel::WriteOnChannel(const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	return m_pIDataDistributionChannelBase->WriteOnChannel(m_channelHandle, key, keyLen, param, dataLen, waitAll, timestamp);
}

HRESULT SmartDataDistributionChannel::ReadFromChannel(int64_t offset, size_t *dataLen, void **param)
{
	return m_pIDataDistributionChannelBase->ReadFromChannel(m_channelHandle, offset, dataLen, param);
}

HRESULT SmartDataDistributionChannel::ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction)
{
	return m_pIDataDistributionChannelBase->ChangeChannelDirection(m_channelHandle, direction);
}

const char* SmartDataDistributionChannel::GetChannelName()
{
	return m_ChannelName;
}

void SmartDataDistributionChannel::OnDataAvailable(const char* key, size_t keyLen, const void* buffer, const size_t len)
{
	char* p = (char*)buffer;
	OnDataAvailable(std::string(key, keyLen),buffer, len);
}

void SmartDataDistributionChannel::OnDataAvailable(const std::string key, const void* buffer, const size_t len)
{

}

void SmartDataDistributionChannel::OnConditionOrError(const DDM_UNDERLYING_ERROR_CONDITION errorCode, const int nativeCode, const char* subSystemReason)
{
	OnConditionOrError(errorCode, nativeCode, std::string(subSystemReason));
}

void SmartDataDistributionChannel::OnConditionOrError(const DDM_UNDERLYING_ERROR_CONDITION errorCode, const int nativeCode, const std::string subSystemReason)
{

}

void SmartDataDistributionChannel::OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent)
{
	if (uEvent->IsDataAvailable)
	{
		OnDataAvailable(uEvent->Key, uEvent->KeyLen, uEvent->Buffer, uEvent->BufferLength);
	}
	else
	{
		OnConditionOrError(uEvent->Condition, uEvent->NativeCode, uEvent->SubSystemReason);
	}
}

SmartDataDistributionBase::SmartDataDistributionBase()
{
	m_pIDataDistribution = DataDistribution::create();
	m_pInitializeHRESULT = E_NOT_SET;
}

SmartDataDistributionBase::~SmartDataDistributionBase()
{
	// clear everything before call this
	delete m_pIDataDistribution;
}

HRESULT SmartDataDistributionBase::Initialize(const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	m_pInitializeHRESULT = m_pIDataDistribution->Initialize(this, conf_file, szMyAddress, channelTrailer);
	return m_pInitializeHRESULT;
}

HRESULT SmartDataDistributionBase::Initialize(const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer)
{
	m_pInitializeHRESULT = m_pIDataDistribution->Initialize(this, arrayParams, len, szMyAddress, channelTrailer);
	return m_pInitializeHRESULT;
}

HRESULT SmartDataDistributionBase::RequestMastershipManager(const char* szMyAddress, const char* arrayParams[], int len)
{
	return m_pIDataDistribution->RequestMastershipManager(this, szMyAddress, arrayParams, len);
}

BOOL SmartDataDistributionBase::Start(DWORD dwMilliseconds)
{
	if (m_pInitializeHRESULT != S_OK)
	{
		return FALSE;
	}
	return m_pIDataDistribution->GetSubsystemManager()->Start(dwMilliseconds);
}

BOOL SmartDataDistributionBase::Stop(DWORD dwMilliseconds)
{
	if (m_pInitializeHRESULT != S_OK)
	{
		return FALSE;
	}
	return m_pIDataDistribution->GetSubsystemManager()->Stop(dwMilliseconds);
}

std::string SmartDataDistributionBase::GetProtocol()
{
	return m_pIDataDistribution->GetProtocol();
}

std::string SmartDataDistributionBase::GetProtocolLib()
{
	return m_pIDataDistribution->GetProtocolLib();
}

std::string SmartDataDistributionBase::GetMastershipLib()
{
	return m_pIDataDistribution->GetMastershipLib();
}

// IDataDistributionCallback
const char * SmartDataDistributionBase::OnConfiguration(const char *channelName, const char * key, const char * value)
{
	return value;
}

void SmartDataDistributionBase::OnLogging(DDM_LOG_LEVEL, const char *, const char*, const char*)
{

}

void SmartDataDistributionBase::OnCompletelyDisconnected(const char * channelName, const char* reason)
{

}

// IDataDistributionMastershipCallback
void SmartDataDistributionBase::OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid)
{

}

void SmartDataDistributionBase::OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
{

}

void SmartDataDistributionBase::OnStateReady(void* pState, int64_t len)
{

}

void SmartDataDistributionBase::OnRequestedState(void** pState, size_t* len)
{

}

void SmartDataDistributionBase::OnMultiplePrimary(int64_t myId, int64_t otherId)
{

}

void SmartDataDistributionBase::FirstStateChange(DDM_INSTANCE_STATE newState)
{

}

void SmartDataDistributionBase::ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
{

}

void SmartDataDistributionBase::ChangedState(DDM_INSTANCE_STATE newState)
{

}
