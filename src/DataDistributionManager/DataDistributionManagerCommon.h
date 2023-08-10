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

#if !defined(DATADISTRIBUTIONMANAGERCOMMON_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERCOMMON_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>

#include "SmartDataDistributionManager.h"
#include "DataDistributionManagerLog.h"
#include "DataDistributionManagerSupport.h"

/**
* @enum CHANNEL_STARTUP_TYPE
*
* @brief CHANNEL_STARTUP_TYPE type.
*
* Startup status
*
* @sa dataDistributionOnClusterStateChange
* @sa IDataDistributionMastershipCallback::OnClusterStateChange()
*/
typedef enum class CHANNEL_STARTUP_TYPE
{
	UNDEFINED = 0x1, /**< Undefined */
	CREATED = 0x2, /**< Created */
	STARTED = 0X4, /**< Started */
	STOPPED = 0X8, /**< Stopped */
	DISCONNECTED = 0x10 /**< Disconnected */
} CHANNEL_STARTUP_TYPE;

class DDM_EXPORT DataDistributionCommon : public IDataDistributionSubsystem
{
public:
	static OPERATION_RESULT ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len);

public:
	DataDistributionCommon();
	~DataDistributionCommon();
	OPERATION_RESULT Initialize(IDataDistributionCallback*, const char* conf_file, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	OPERATION_RESULT Initialize(IDataDistributionCallback*, const char* arrayParams[], int len, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	virtual OPERATION_RESULT Initialize();
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...);
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args);

	virtual CHANNEL_HANDLE CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = 0);
	virtual	OPERATION_RESULT StartChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds);
	virtual	OPERATION_RESULT StopChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds);
	virtual void SetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName, const char* paramValue);
	virtual void SetParameter(CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId, const char* paramValue);
	virtual const char* GetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName);
	virtual const char* GetParameter(CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId);
	virtual OPERATION_RESULT Lock(CHANNEL_HANDLE_PARAMETER, unsigned long timeout);
	virtual OPERATION_RESULT Unlock(CHANNEL_HANDLE_PARAMETER);
	virtual OPERATION_RESULT SeekChannel(CHANNEL_HANDLE_PARAMETER, int64_t position, DDM_SEEKCONTEXT context = DDM_SEEKCONTEXT::OFFSET, DDM_SEEKKIND kind = DDM_SEEKKIND::ABSOLUTE);
	virtual OPERATION_RESULT DeleteChannel(CHANNEL_HANDLE_PARAMETER);
	virtual OPERATION_RESULT WriteOnChannel(CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = DDM_NO_TIMESTAMP);
	virtual OPERATION_RESULT ReadFromChannel(CHANNEL_HANDLE_PARAMETER, int64_t offset, size_t *dataLen, void **param);
	virtual OPERATION_RESULT ChangeChannelDirection(CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction);

	virtual size_t GetMaxMessageSize();
	virtual int GetServerLostTimeout();
	virtual	OPERATION_RESULT Start(unsigned long dwMilliseconds);
	virtual	OPERATION_RESULT Stop(unsigned long dwMilliseconds);
	IDataDistributionCallback* GetCallbacks();
	BOOL GetSubSystemStarted();
protected:
	std::string CheckConfigurationParameter(const char * channelName, std::string key, std::string value);
	void SetSubSystemStarted(BOOL started);
	void SetMaxMessageSize(size_t maxMessageSize);
	void SetServerLostTimeout(int timeout);
	DDM_LOG_LEVEL GetGlobalLogLevel();
	void SetGlobalLogLevel(DDM_LOG_LEVEL level);
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
	DDM_LOG_LEVEL m_GlobalLogLevel;
	IDataDistributionCallback* m_pDataDistributionManagerCallbacks;
	const char* m_confFile;
	const char* m_ChannelTrailer;
	const char* m_ServerName;
	BOOL m_SubSystemStarted;
};

class DDM_EXPORT ChannelConfiguration : public IDataDistributionChannel
{
public:
	ChannelConfiguration(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionCommon* mainManager, IDataDistributionChannelCallback* Cb);
	const char* GetChannelName();
	virtual GENERIC_HANDLE GetOpaqueHandle();
	DDM_CHANNEL_DIRECTION GetDirection();
	void SetDirection(DDM_CHANNEL_DIRECTION direction);
	DataDistributionCommon* GetManager();
	void OnDataAvailable(const char* key, size_t keyLen, void* buffer, size_t len, int64_t timestamp = -1, int64_t offset = -1);
	void OnDataAvailable(const CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void* buffer, size_t len, int64_t timestamp = -1, int64_t offset = -1);
	void OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode, const char* subSystemReason, ...);
	void OnConditionOrError(const CHANNEL_HANDLE_PARAMETER, OPERATION_RESULT errorCode, int nativeCode, const char* subSystemReason, ...);
	void Log(const DDM_LOG_LEVEL level, const char* function, const char* format, ...);
	void CompletelyDisconnected();
	int64_t GetActualOffset();
	void SetActualOffset(int64_t val);
	int64_t GetActualTimestamp();
	void SetActualTimestamp(int64_t val);
	OPERATION_RESULT WaitStartupStatus(unsigned long dwMilliseconds);
	void SetStartupStatus(CHANNEL_STARTUP_TYPE status);
	CHANNEL_STARTUP_TYPE GetStartupStatus();
	BOOL IsStartupStatusSet();
	BOOL GetLockState();
	OPERATION_RESULT SetLockState();
	OPERATION_RESULT ResetLockState();
	void WaitingFinishLockState(unsigned long dwMilliseconds);

	BOOL GetCommitSync();
	void SetCommitSync(BOOL);
	BOOL GetEventSync();
	void SetEventSync(BOOL val);
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
	int  GetCommitTimeout();
	void SetCommitTimeout(int);
protected:
	DataDistributionLockWrapper* m_csFlags;
	DataDistributionLockWrapper* m_csState;
	DataDistributionLockWrapper* m_csOffsets;
	int64_t m_lastRoutedOffset;
	int64_t m_actualOffset;
	int64_t m_actualTimestamp;

private:
	BOOL m_CommitSync;
	BOOL m_EventSync;
	int  m_CreateChannelTimeout;
	int  m_ChannelSeekTimeout;
	int  m_KeepAliveTimeout;
	int  m_MessageReceiveTimeout;
	int  m_ConsumerTimeout;
	int  m_ProducerTimeout;
	int  m_CommitTimeout;

	BOOL bLockState;

	DataDistributionEventWrapper* m_pEvtStartupStatus;
	DataDistributionEventWrapper* m_pEvtLockState;

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