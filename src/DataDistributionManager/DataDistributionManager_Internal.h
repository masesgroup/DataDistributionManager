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

#if !defined(DATADISTRIBUTIONMANAGER_INTERNAL_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGER_INTERNAL_H__INCLUDED_

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

#include "DataDistributionManager.h"
#include "DataDistributionManagerCommon.h"

#ifndef USE_CTIME
using namespace std::chrono;
#endif

#define MALFORMED_STRING "Malformed: "

class DataDistributionMastershipCommon;

class TimeMeasureWrapperImpl : public ITimeMeasureWrapper
{
private:
#ifndef USE_CTIME
	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
#else
	clock_t timeStart;
#endif
public:
	TimeMeasureWrapperImpl();

	virtual ~TimeMeasureWrapperImpl();

	void ResetTime();

	int64_t ElapsedMilliseconds();

	int64_t ElapsedMicroseconds();

	int64_t ElapsedNanoseconds();

	void AddNanoseconds(unsigned int time);
};

typedef void* (*pvInstantiatorFun)();

typedef struct
{
	const void* OpaqueCallbackReceiver;
	dataDistributionConfigurationCb OnConfiguration;
	dataDistributionLoggingCb OnLogging;
	dataDistributionCompletelyDisconnectedCb OnCompletelyDisconnected;
} DataDistributionManagerCallbacks, *pDataDistributionManagerCallbacks;

class DataDistributionCallbackImpl : public DataDistributionCallback
{
public:
	DataDistributionCallbackImpl(const void*, dataDistributionConfigurationCb, dataDistributionLoggingCb, dataDistributionCompletelyDisconnectedCb);
	virtual const char* OnConfiguration(const char* key, const char* value);
	virtual void OnLogging(DDM_LOG_LEVEL, const char*, const char*, const char*);
	virtual void OnCompletelyDisconnected(const char*, const char*);
private:
	DataDistributionManagerCallbacks m_DataDistributionManagerCallbacks;
};

typedef struct
{
	const void* OpaqueCallbackReceiver;
	dataDistributionUnderlyingEvent OnUnderlyingEvent;
	//dataDistributionOnConditionOrError OnConditionOrError;
} DataDistributionManagerChannelCallbacks, *pDataDistributionManagerChannelCallbacks;

class DataDistributionChannelCallbackImpl : public IDataDistributionChannelCallback
{
public:
	DataDistributionChannelCallbackImpl(const void*, dataDistributionUnderlyingEvent); // , dataDistributionOnConditionOrError);
	virtual void OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent);
	//virtual void OnConditionOrError(HANDLE channelHandle, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, const char* subSystemReason);
private:
	DataDistributionManagerChannelCallbacks m_DataDistributionManagerChannelCallbacks;
};

typedef struct
{
	void* OpaqueCallbackReceiver;
	dataDistributionOnClusterStateChange OnClusterStateChange;
	dataDistributionOnStateChange OnStateChange;
	dataDistributionOnStateReady OnStateReady;
	dataDistributionOnRequestedState OnRequestedState;
	dataDistributionOnMultiplePrimary OnMultiplePrimary;
	dataDistributionFirstStateChange FirstStateChange;
	dataDistributionChangingState ChangingState;
	dataDistributionChangedState ChangedState;
} DataDistributionMastershipCallbacks, *pDataDistributionMastershipCallbacks;

class DataDistributionMastershipCallbackImpl : public IDataDistributionMastershipCallback
{
public:
	DataDistributionMastershipCallbackImpl(void* opaque, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
		dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
		dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8);
	virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid) ;
	virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) ;
	virtual void OnStateReady(void* pState, int64_t len) ;
	virtual void OnRequestedState(void** pState, size_t* len) ;
	virtual void OnMultiplePrimary(int64_t myId, int64_t otherId) ;
	virtual void FirstStateChange(DDM_INSTANCE_STATE newState);
	virtual void ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) ;
	virtual void ChangedState(DDM_INSTANCE_STATE newState);
private:
	DataDistributionMastershipCallbacks m_DataDistributionMastershipCallbacks;
};

class DataDistributionManagerImpl : public DataDistribution
{
public:
	static IDataDistribution *create();
	static HRESULT ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len);
public:
	DataDistributionManagerImpl();
	~DataDistributionManagerImpl();
	HRESULT Initialize(IDataDistributionCallback*, const char* conf_file = NULL, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	HRESULT Initialize(IDataDistributionCallback*, const char* arrayParams[], int len, const char* szMyAddress = NULL, const char* channelTrailer = NULL);
	HRESULT RequestMastershipManager(IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	BOOL Start(DWORD dwMilliseconds);
	BOOL Stop(DWORD dwMilliseconds);
	IDataDistributionSubsystem* GetSubsystemManager();
	IDataDistributionMastershipCommon* GetMastershipManager();
	std::string GetProtocol();
	std::string GetProtocolLib();
	std::string GetMastershipLib();
private:
	const char** m_arrayParams;
	int m_arrayParamsLen;
	HRESULT m_InitializedResult;
	HRESULT m_InitializedMastershipResult;
	HRESULT read_config_file(const char* arrayParams[], int len);
	IDataDistributionSubsystem* pDataDistributionManagerSubsystem;
	IDataDistributionMastershipCommon* pDataDistributionMastershipManagerCommon;
	HMODULE m_hlibModule;
	HMODULE m_hMastershipManagerModule;
	std::string m_Protocol;
	std::string m_ProtocolLib;
	std::string m_MastershipLib;
};

#endif // !defined(DATADISTRIBUTIONMANAGER_INTERNAL_H__INCLUDED_)