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

#if !defined(DATADISTRIBUTIONMANAGERCOMMON_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERCOMMON_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_RAND_S  

#include <winsock2.h>
#include <process.h> 

#include <iostream>
#include <fstream>
#include <cstring>
#if _MSC_VER > 1600
#include <chrono>
#else
#define USE_CTIME
#include <ctime>
#endif
#include <sstream> 
#include <list>
#include <map>

#include "SmartDataDistributionManager.h"
#include "DataDistributionManagerLog.h"

#ifndef USE_CTIME
using namespace std::chrono;
#endif

class __declspec(dllexport) DataDistributionCommon : public IDataDistributionSubsystem
{
public:
	static HRESULT ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len);

public:
	DataDistributionCommon();
	~DataDistributionCommon();
	HRESULT Initialize(IDataDistributionCallback*, const char* conf_file, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	HRESULT Initialize(IDataDistributionCallback*, const char* arrayParams[], int len, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	virtual HRESULT Initialize();
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...);
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args);

	virtual HANDLE CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = 0);
	virtual	HRESULT StartChannel(HANDLE channelHandle, DWORD dwMilliseconds);
	virtual	HRESULT StopChannel(HANDLE channelHandle, DWORD dwMilliseconds);
	virtual void SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue);
	virtual void SetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char* paramValue);
	virtual const char* GetParameter(HANDLE channelHandle, const char* paramName);
	virtual const char* GetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId);
	virtual HRESULT Lock(HANDLE channelHandle, DWORD timeout);
	virtual HRESULT Unlock(HANDLE channelHandle);
	virtual HRESULT SeekChannel(HANDLE channelHandle, int64_t position);
	virtual HRESULT DeleteChannel(HANDLE channelHandle);
	virtual HRESULT WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = -1);
	virtual HRESULT ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param);
	virtual HRESULT ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction);

	virtual size_t GetMaxMessageSize();
	virtual int GetServerLostTimeout();
	virtual	HRESULT Start(DWORD dwMilliseconds);
	virtual	HRESULT Stop(DWORD dwMilliseconds);
	IDataDistributionCallback* GetCallbacks();
	BOOL GetSubSystemStarted();
protected:
	std::string CheckConfigurationParameter(std::string key, std::string value);
	void SetSubSystemStarted(BOOL started);
	void SetMaxMessageSize(size_t maxMessageSize);
	void SetServerLostTimeout(int timeout);
	std::string GetConfigFile();
	std::string GetChannelTrailer();
	std::string GetServerName();
	const char** GetArrayParams();
	int GetArrayParamsLen();
private:
	const char** m_arrayParams;
	int m_arrayParamsLen;
	size_t m_MaxMessageSize;
	int  m_ServerLostTimeout;
	IDataDistributionCallback* m_pDataDistributionManagerCallbacks;
	const char* m_confFile;
	const char* m_ChannelTrailer;
	const char* m_ServerName;
	BOOL m_SubSystemStarted;
};

class __declspec(dllexport) ChannelConfiguration
{
public:
	ChannelConfiguration(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionCommon* mainManager, IDataDistributionChannelCallback* Cb);
	const char* GetChannelName();
	DDM_CHANNEL_DIRECTION GetDirection();
	void SetDirection(DDM_CHANNEL_DIRECTION direction);
	DataDistributionCommon* GetManager();
	void OnDataAvailable(const char* key, size_t keyLen, void* buffer, size_t len);
	void OnDataAvailable(const HANDLE channelHandle, const char* key, size_t keyLen, void* buffer, size_t len);
	void OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason, ...);
	void OnConditionOrError(const HANDLE channelHandle, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason, ...);
	void Log(const DDM_LOG_LEVEL level, const char* function, const char* format, ...);
	void CompletelyDisconnected();
	int64_t GetManagedOffset();
	void SetManagedOffset(int64_t val);
	DWORD WaitStartupStatus(DWORD dwMilliseconds);
	void SetStartupStatus(CHANNEL_STARTUP_TYPE status);
	CHANNEL_STARTUP_TYPE GetStartupStatus();
	BOOL IsStartupStatusSet();
	BOOL GetLockState();
	HRESULT SetLockState();
	HRESULT ResetLockState();
	void WaitingFinishLockState(DWORD dwMilliseconds);

	BOOL GetCommitSync();
	void SetCommitSync(BOOL);
	int  GetCreateChannelTimeout();
	void SetCreateChannelTimeout(int);
	int  GetChannelSeekTimeout();
	void SetChannelSeekTimeout(int);
	int  GetKeepAliveTimeout();
	void SetKeepAliveTimeout(int);
	int GetMessageReceiveTimeout();
	void SetMessageReceiveTimeout(int);
	int  GetConsumerTimeout();
	void SetConsumerTimeout(int);
	int  GetProducerTimeout();
	void SetProducerTimeout(int);
protected:
	CRITICAL_SECTION m_csFlags;
	CRITICAL_SECTION m_csState;
	CRITICAL_SECTION m_csOffsets;
	int64_t m_lastRoutedOffset;
	int64_t m_lastManagedOffset;

private:
	BOOL m_CommitSync;
	int  m_CreateChannelTimeout;
	int  m_ChannelSeekTimeout;
	int  m_KeepAliveTimeout;
	int  m_MessageReceiveTimeout;
	int  m_ConsumerTimeout;
	int  m_ProducerTimeout;

	BOOL bLockState;

	HANDLE h_evtStartupStatus;
	HANDLE h_evtLockState;

	BOOL m_StartupStatusSet;

	CHANNEL_STARTUP_TYPE m_StartupStatus;
	DataDistributionCommon* m_pMainManager;
	IDataDistributionChannelCallback* dataCb;
	const char* m_pChannelName;
	DDM_CHANNEL_DIRECTION m_Direction;
};

#define CAST_CHANNEL_NAME(TYPE, VAR_NAME) TYPE* pChannelConfiguration = static_cast<TYPE*>(VAR_NAME);
#define CAST_CHANNEL(TYPE) CAST_CHANNEL_NAME(TYPE, channelHandle)

#endif // !defined(DATADISTRIBUTIONMANAGERCOMMON_H__INCLUDED_)