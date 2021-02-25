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

#if !defined(DATADISTRIBUTIONMANAGER_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdint.h>
#include <map>

#ifndef _INC_WINDOWS
#include <windows.h>
#endif /* _INC_WINDOWS */

#include "DataDistributionManagerEnums.h"
/**
 * @brief 
 * 
 */
typedef const char*(__cdecl *dataDistributionConfigurationCb) (const void* opaque, const char * channelName, const char* key, const char* value);
typedef void(__cdecl *dataDistributionLoggingCb) (const void* opaque, const DDM_LOG_LEVEL level, const char* source, const char* function, const char* errorStr);
typedef void(__cdecl *dataDistributionCompletelyDisconnectedCb) (const void* opaque, const char* source, const char* errorStr);

struct ClusterHealthElement
{
	DDM_INSTANCE_STATE Status;
	int64_t Uptime;
	int64_t LastContactTime;
};
///*! \class ClusterStateElement
//    \brief A class con.
//
//    A more detailed class description.
//*/
//struct ClusterStateElement
//{
//	int64_t ServerId; /*!< Server identifier */
//	DDM_INSTANCE_STATE Status; /*!< Status of server  */
//
//	ClusterStateElement(int64_t serverId, DDM_INSTANCE_STATE status)
//	{
//		ServerId = serverId;
//		Status = status;
//	}
//};

class IDataDistributionMastershipCallback;
class IDataDistributionMastershipCommon;
class IDataDistributionSubsystem;
/**
 * @brief Construct a new declspec object
 * 
 */
class __declspec(dllexport) ITimeMeasureWrapper
{
public:
	virtual void ResetTime() = 0;

	virtual int64_t ElapsedMilliseconds() = 0;

	virtual	int64_t ElapsedMicroseconds() = 0;

	virtual	int64_t ElapsedNanoseconds() = 0;

	virtual void AddNanoseconds(unsigned int time) = 0;
};

class __declspec(dllexport) TimeMeasureWrapper
{
public:
	static ITimeMeasureWrapper* create();
};

class __declspec(dllexport) IDataDistributionLog
{
public:
	virtual void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...) = 0;
	virtual void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args) = 0;
};

class __declspec(dllexport) IDataDistributionCallback
{
public:
	virtual const char * OnConfiguration(const char * channelName, const char * key, const char * value) = 0;
	virtual void OnLogging(DDM_LOG_LEVEL, const char *, const char*, const char*) = 0;
	virtual void OnCompletelyDisconnected(const char * channelName, const char* reason) = 0;
};

class __declspec(dllexport) DataDistributionCallback : public IDataDistributionCallback
{
public:
	static IDataDistributionCallback *create(void*, dataDistributionConfigurationCb, dataDistributionLoggingCb, dataDistributionCompletelyDisconnectedCb);
};

typedef struct __declspec(dllexport) underlyingEventData
{
	const char* ChannelName;
	DDM_UNDERLYING_ERROR_CONDITION Condition;
	BOOL IsDataAvailable;
	const char* Key;
	size_t KeyLen;
	const void* Buffer;
	size_t BufferLength;
	int NativeCode;
	const char* SubSystemReason;

	underlyingEventData(const char* channelName)
	{
		ChannelName = channelName;
		Condition = DDM_UNDERLYING_ERROR_CONDITION::DDM_NO_ERROR_CONDITION;
		IsDataAvailable = FALSE;
		Key = NULL;
		KeyLen = 0;
		Buffer = NULL;
		BufferLength = 0;
		NativeCode = 0;
		SubSystemReason = NULL;
	}

	underlyingEventData(const char* channelName, const char* key, size_t keyLen, const void* buffer, size_t bufferLength) : underlyingEventData(channelName)
	{
		Key = key;
		KeyLen = keyLen;
		Buffer = buffer;
		BufferLength = bufferLength;
		IsDataAvailable = TRUE;
	}

	underlyingEventData(const char* channelName, DDM_UNDERLYING_ERROR_CONDITION condition, int nativeCode, const char* subSystemReason) : underlyingEventData(channelName)
	{
		Condition = condition;
		NativeCode = nativeCode;
		SubSystemReason = subSystemReason;
	}
} UnderlyingEventData, *pUnderlyingEventData;

typedef void(__cdecl *dataDistributionUnderlyingEvent)(const void* opaque, const HANDLE channelHandle, const UnderlyingEventData* uEvent);

class __declspec(dllexport) IDataDistributionChannelCallback
{
public:
	virtual void OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent) = 0;
};

class __declspec(dllexport) DataDistributionChannelCallback
{
public:
	static IDataDistributionChannelCallback *create(void* opaque, dataDistributionUnderlyingEvent uEventCb);
};

typedef void(__cdecl * dataDistributionOnClusterStateChange)(void* opaque, DDM_CLUSTEREVENT change, int64_t serverid);
typedef void(__cdecl * dataDistributionOnStateChange)(void* opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
typedef void(__cdecl * dataDistributionOnStateReady)(void* opaque, void* pState, int64_t len);
typedef void(__cdecl * dataDistributionOnRequestedState)(void* opaque, void** pState, size_t* len);
typedef void(__cdecl * dataDistributionOnMultiplePrimary)(void* opaque, int64_t myId, int64_t otherId);
typedef void(__cdecl * dataDistributionFirstStateChange)(void* opaque, DDM_INSTANCE_STATE newState);
typedef void(__cdecl * dataDistributionChangingState)(void* opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);
typedef void(__cdecl * dataDistributionChangedState)(void* opaque, DDM_INSTANCE_STATE newState);

class __declspec(dllexport) IDataDistributionMastershipCallback
{
public:
	virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid) = 0;
	virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) = 0;
	virtual void OnStateReady(void* pState, int64_t len) = 0;
	virtual void OnRequestedState(void** pState, size_t* len) = 0;
	virtual void OnMultiplePrimary(int64_t myId, int64_t otherId) = 0;
	virtual void FirstStateChange(DDM_INSTANCE_STATE newState) = 0;
	virtual void ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) = 0;
	virtual void ChangedState(DDM_INSTANCE_STATE newState) = 0;
};

class __declspec(dllexport) DataDistributionMastershipCallback
{
public:
	static IDataDistributionMastershipCallback *create(void* opaque, dataDistributionOnClusterStateChange, dataDistributionOnStateChange,
		dataDistributionOnStateReady, dataDistributionOnRequestedState, dataDistributionOnMultiplePrimary,
		dataDistributionFirstStateChange, dataDistributionChangingState, dataDistributionChangedState);
};

class __declspec(dllexport) IDataDistributionChannelBase
{
public:
	virtual HANDLE CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = 0) = 0;
	virtual	HRESULT StartChannel(HANDLE channelHandle, DWORD dwMilliseconds) = 0;
	virtual	HRESULT StopChannel(HANDLE channelHandle, DWORD dwMilliseconds) = 0;
	virtual void SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue) = 0;
	virtual void SetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char* paramValue) = 0;
	virtual const char* GetParameter(HANDLE channelHandle, const char* paramName) = 0;
	virtual const char* GetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId) = 0;
	virtual HRESULT Lock(HANDLE channelHandle, DWORD timeout) = 0;
	virtual HRESULT Unlock(HANDLE channelHandle) = 0;
	virtual HRESULT SeekChannel(HANDLE channelHandle, int64_t position) = 0;
	virtual HRESULT DeleteChannel(HANDLE channelHandle) = 0;
	virtual HRESULT WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = -1) = 0;
	virtual HRESULT ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param) = 0;
	virtual HRESULT ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction) = 0;
};

class __declspec(dllexport) IDataDistributionTransport : public IDataDistributionChannelBase
{
public:
	virtual int GetServerLostTimeout() = 0;
	virtual size_t GetMaxMessageSize() = 0;
	virtual	HRESULT Start(DWORD dwMilliseconds) = 0;
	virtual	HRESULT Stop(DWORD dwMilliseconds) = 0;
};

class __declspec(dllexport) IDataDistributionSubsystem : public IDataDistributionTransport, public IDataDistributionLog
{
public:
	virtual HRESULT Initialize(IDataDistributionCallback*, const char* conf_file, const char* szMyAddress = NULL, const char* channelTrailer = NULL) = 0;
	virtual HRESULT Initialize(IDataDistributionCallback*, const char* arrayParams[], int len, const char* szMyAddress = NULL, const char* channelTrailer = NULL) = 0;
	virtual	IDataDistributionCallback* GetCallbacks() = 0;
};

typedef std::map<int64_t, ClusterHealthElement*> ClusterHealth;
typedef std::map<int64_t, ClusterHealthElement*>::const_iterator ClusterHealthIterator;

class __declspec(dllexport) IDataDistributionMastershipCommon
{
public:
	virtual HRESULT Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0) = 0;
	virtual HRESULT Start(DWORD dwMilliseconds) = 0;
	virtual HRESULT Stop(DWORD dwMilliseconds) = 0;
	virtual BOOL GetIamNextPrimary() = 0;
	virtual BOOL RequestIAmNextPrimary() = 0;
	virtual int64_t* GetClusterIndexes(size_t* length) = 0;
	virtual DDM_INSTANCE_STATE GetStateOf(int64_t serverId) = 0;
	virtual ClusterHealth GetClusterHealth() = 0;
	virtual DDM_INSTANCE_STATE GetMyState() = 0;
	virtual void ChangeMyState(DDM_INSTANCE_STATE) = 0;
	virtual void ChangeState(int64_t instanceId, DDM_INSTANCE_STATE) = 0;
	virtual int64_t GetLocalServerId() = 0;
	virtual int64_t GetPrimaryServerId() = 0;
	virtual int64_t GetMessageDelay() = 0;
	virtual int64_t GetUpTime() = 0;
};

class __declspec(dllexport) IDataDistribution
{
public:
	virtual HRESULT Initialize(IDataDistributionCallback* iddcb, const char* conf_file = NULL, const char* szMyAddress = NULL, const char* channelTrailer = NULL) = 0;
	virtual HRESULT Initialize(IDataDistributionCallback* iddcb, const char* arrayParams[], int len, const char* szMyAddress = NULL, const char* channelTrailer = NULL) = 0;
	virtual HRESULT RequestMastershipManager(IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0) = 0;
	virtual	BOOL Start(DWORD dwMilliseconds) = 0;
	virtual	BOOL Stop(DWORD dwMilliseconds) = 0;
	virtual IDataDistributionSubsystem* GetSubsystemManager() = 0;
	virtual IDataDistributionMastershipCommon* GetMastershipManager() = 0;
	virtual std::string GetProtocol() = 0;
	virtual std::string GetProtocolLib() = 0;
	virtual std::string GetMastershipLib() = 0;
};

class __declspec(dllexport) DataDistribution : public IDataDistribution
{
public:
	static IDataDistribution *create();
};

#endif // !defined(DATADISTRIBUTIONMANAGER_H__INCLUDED_)
