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

void SmartDataDistributionChannel::SetInformation(const char* channelName, IDataDistributionChannelBase* pIDataDistributionChannelBase, CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction)
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

OPERATION_RESULT SmartDataDistributionChannel::StartChannel(unsigned long dwMilliseconds)
{
	return m_pIDataDistributionChannelBase->StartChannel(m_channelHandle, dwMilliseconds);
}

OPERATION_RESULT SmartDataDistributionChannel::StopChannel(unsigned long dwMilliseconds)
{
	return m_pIDataDistributionChannelBase->StopChannel(m_channelHandle, dwMilliseconds);
}

OPERATION_RESULT SmartDataDistributionChannel::Lock(unsigned long timeout)
{
	return m_pIDataDistributionChannelBase->Lock(m_channelHandle, timeout);
}

OPERATION_RESULT SmartDataDistributionChannel::Unlock()
{
	return m_pIDataDistributionChannelBase->Unlock(m_channelHandle);
}

OPERATION_RESULT SmartDataDistributionChannel::SeekChannel(int64_t position)
{
	return m_pIDataDistributionChannelBase->SeekChannel(m_channelHandle, position);
}

OPERATION_RESULT SmartDataDistributionChannel::WriteOnChannel(const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	return m_pIDataDistributionChannelBase->WriteOnChannel(m_channelHandle, key, keyLen, param, dataLen, waitAll, timestamp);
}

OPERATION_RESULT SmartDataDistributionChannel::ReadFromChannel(int64_t offset, size_t *dataLen, void **param)
{
	return m_pIDataDistributionChannelBase->ReadFromChannel(m_channelHandle, offset, dataLen, param);
}

OPERATION_RESULT SmartDataDistributionChannel::ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction)
{
	return m_pIDataDistributionChannelBase->ChangeChannelDirection(m_channelHandle, direction);
}

const char* SmartDataDistributionChannel::GetChannelName()
{
	return m_ChannelName;
}

int64_t SmartDataDistributionChannel::GetTimestamp()
{
	return m_timestamp;
}

int64_t SmartDataDistributionChannel::GetOffset()
{
	return m_offset;
}

void SmartDataDistributionChannel::OnDataAvailable(const char* key, size_t keyLen, const void* buffer, const size_t len)
{
	char* p = (char*)buffer;
	OnDataAvailable(std::string(key, keyLen),buffer, len);
}

void SmartDataDistributionChannel::OnDataAvailable(const std::string key, const void* buffer, const size_t len)
{

}

void SmartDataDistributionChannel::OnConditionOrError(const OPERATION_RESULT errorCode, const int nativeCode, const char* subSystemReason)
{
	OnConditionOrError(errorCode, nativeCode, std::string(subSystemReason));
}

void SmartDataDistributionChannel::OnConditionOrError(const OPERATION_RESULT errorCode, const int nativeCode, const std::string subSystemReason)
{

}

void SmartDataDistributionChannel::OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData* uEvent)
{
	if (uEvent->IsDataAvailable)
	{
		m_timestamp = uEvent->Timestamp;
		m_offset = uEvent->Offset;
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
	m_pInitializeHRESULT = DDM_POINTER_NOT_SET;
}

SmartDataDistributionBase::~SmartDataDistributionBase()
{
	// clear everything before call this
	delete m_pIDataDistribution;
}

OPERATION_RESULT SmartDataDistributionBase::Initialize(const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	m_pInitializeHRESULT = m_pIDataDistribution->Initialize(this, conf_file, szMyAddress, channelTrailer);
	return m_pInitializeHRESULT;
}

OPERATION_RESULT SmartDataDistributionBase::Initialize(const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer)
{
	m_pInitializeHRESULT = m_pIDataDistribution->Initialize(this, arrayParams, len, szMyAddress, channelTrailer);
	return m_pInitializeHRESULT;
}

OPERATION_RESULT SmartDataDistributionBase::RequestMastershipManager(const char* szMyAddress, const char* arrayParams[], int len)
{
	return m_pIDataDistribution->RequestMastershipManager(this, szMyAddress, arrayParams, len);
}

BOOL SmartDataDistributionBase::Start(unsigned long dwMilliseconds)
{
	if (OPERATION_FAILED(m_pInitializeHRESULT))
	{
		return FALSE;
	}
	return m_pIDataDistribution->GetSubsystemManager()->Start(dwMilliseconds);
}

BOOL SmartDataDistributionBase::Stop(unsigned long dwMilliseconds)
{
	if (OPERATION_FAILED(m_pInitializeHRESULT))
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
