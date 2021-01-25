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

#if !defined(SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_)
#define SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_

#include "DataDistributionManager.h"
#include <vector>

class __declspec(dllexport) SmartTimeMeasureWrapper : public ITimeMeasureWrapper
{
public:
	SmartTimeMeasureWrapper();
	~SmartTimeMeasureWrapper();
	void ResetTime();

	int64_t ElapsedMilliseconds();

	int64_t ElapsedMicroseconds();

	int64_t ElapsedNanoseconds();

	void AddNanoseconds(unsigned int time);
private:
	ITimeMeasureWrapper* m_pITimeMeasureWrapper;
};

class __declspec(dllexport) SmartDataDistributionChannel : protected IDataDistributionChannelCallback
{
public:
	SmartDataDistributionChannel();
protected:
	~SmartDataDistributionChannel();
public:
	DDM_CHANNEL_DIRECTION GetDirection();
	void SetInformation(const char* channelName, IDataDistributionChannelBase* pIDataDistributionChannelBase, HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction);
	HRESULT StartChannel(DWORD dwMilliseconds);
	HRESULT StopChannel(DWORD dwMilliseconds);
	HRESULT Lock(DWORD timeout);
	HRESULT Unlock();
	HRESULT SeekChannel(int64_t position);
	HRESULT WriteOnChannel(const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = -1);
	HRESULT ReadFromChannel(int64_t offset, size_t *dataLen, void **param);
	HRESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction);
	const char* GetChannelName();
	virtual void OnDataAvailable(const char* key, size_t keyLen, const void* buffer, const size_t len);
	virtual void OnDataAvailable(const std::string key, const void* buffer, const size_t len);
	virtual void OnConditionOrError(const DDM_UNDERLYING_ERROR_CONDITION errorCode, const int nativeCode, const char* subSystemReason);
	virtual void OnConditionOrError(const DDM_UNDERLYING_ERROR_CONDITION errorCode, const int nativeCode, const std::string subSystemReason);
protected:
	virtual void OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData* uEvent);
private:
	const char* m_ChannelName;
	HANDLE m_channelHandle;
	IDataDistributionChannelBase* m_pIDataDistributionChannelBase;
	DDM_CHANNEL_DIRECTION m_Direction;
};

class __declspec(dllexport) SmartDataDistributionBase : protected IDataDistributionCallback, protected IDataDistributionMastershipCallback
{
public:
	SmartDataDistributionBase();
	~SmartDataDistributionBase();
	HRESULT Initialize(const char* conf_file, const char* szMyAddress = 0, const char* channelTrailer = 0);
	HRESULT Initialize(const char* arrayParams[], int len, const char* szMyAddress = 0, const char* channelTrailer = 0);
	HRESULT RequestMastershipManager(const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	BOOL Start(DWORD dwMilliseconds);
	BOOL Stop(DWORD dwMilliseconds);
	std::string GetProtocol();
	std::string GetProtocolLib();
	std::string GetMastershipLib();
protected:
	// IDataDistributionCallback
	virtual const char * OnConfiguration(const char * key, const char * value);
	virtual void OnLogging(DDM_LOG_LEVEL, const char *, const char*, const char*);
	virtual void OnCompletelyDisconnected(const char * channelName, const char* reason);
	// IDataDistributionMastershipCallback
	virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, size_t serverid);
	virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
	virtual void OnStateReady(void* pState, size_t len);
	virtual void OnRequestedState(void** pState, size_t* len);
	virtual void OnMultiplePrimary(size_t, size_t);
	virtual void FirstStateChange(DDM_INSTANCE_STATE newState);
	virtual void ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);
	virtual void ChangedState(DDM_INSTANCE_STATE newState);
protected:
	IDataDistribution* m_pIDataDistribution;
	IDataDistributionMastershipCommon* m_pIDataDistributionMastershipCommon;
	HRESULT m_pInitializeHRESULT;
};

template <typename T>
class __declspec(dllexport) SmartDataDistribution : public SmartDataDistributionBase
{
public:
	SmartDataDistribution() : SmartDataDistributionBase() {}
	~SmartDataDistribution() {}
	T* CreateSmartChannel(const char* channelName, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = 0)
	{
		static_assert(std::is_base_of<SmartDataDistributionChannel, T>::value, "type parameter of this class must derive from SmartDataDistributionChannel");
		if (m_pInitializeHRESULT != S_OK) return NULL;
		T* pSmartChannelT = new T();
		SmartDataDistributionChannel* pSmartChannel = (SmartDataDistributionChannel*)pSmartChannelT;
		IDataDistributionChannelBase* pChannelBase = m_pIDataDistribution->GetSubsystemManager();
		HANDLE channelHandle = pChannelBase->CreateChannel(channelName, (IDataDistributionChannelCallback*)pSmartChannel, direction, arrayParams, len);
		pSmartChannel->SetInformation(channelName, pChannelBase, channelHandle, direction);

		return pSmartChannelT;
	}
};


#endif // End SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_
