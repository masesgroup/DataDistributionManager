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

#include "DataDistributionManager_Internal.h"
#include "DataDistributionMastershipManagerBase.h"

static const byte sString[] = "Copyright(c) 2012-2021 MASES s.r.l., All Rights Reserved";

ITimeMeasureWrapper* TimeMeasureWrapper::create()
{
	return new TimeMeasureWrapperImpl();
}

TimeMeasureWrapperImpl::TimeMeasureWrapperImpl()
{
#ifndef USE_CTIME
	timeStart = high_resolution_clock::now();
#else
	ResetTime();
#endif
}

TimeMeasureWrapperImpl::~TimeMeasureWrapperImpl() { }

void TimeMeasureWrapperImpl::ResetTime()
{
#ifndef USE_CTIME
	timeStart = high_resolution_clock::now();
#else
	timeStart = clock();
#endif
}

int64_t TimeMeasureWrapperImpl::ElapsedMilliseconds()
{
#ifndef USE_CTIME
	high_resolution_clock::time_point tnow = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(tnow - timeStart).count();
	return duration;
#else
	clock_t end = clock();
	double elapsed_secs = double(end - timeStart) / CLOCKS_PER_SEC;
	return elapsed_secs * 1000;
#endif
}

int64_t TimeMeasureWrapperImpl::ElapsedMicroseconds()
{
#ifndef USE_CTIME
	high_resolution_clock::time_point tnow = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(tnow - timeStart).count();
	return duration;
#else
	return ElapsedMilliseconds() * 1000;
#endif
}

int64_t TimeMeasureWrapperImpl::ElapsedNanoseconds()
{
#ifndef USE_CTIME
	high_resolution_clock::time_point tnow = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(tnow - timeStart).count();
	return duration;
#else
	return ElapsedMicroseconds() * 1000;
#endif
}

void TimeMeasureWrapperImpl::AddNanoseconds(unsigned int time)
{
#ifndef USE_CTIME
	auto start = timeStart.time_since_epoch();
	start = start + (std::chrono::nanoseconds)time;

	timeStart = high_resolution_clock::time_point::time_point(start);
#else
	timeStart += time / 1000000L;
#endif
}

IDataDistributionChannelCallback *DataDistributionChannelCallback::create(void* opaque, dataDistributionUnderlyingEvent uEvent)
{
	return new DataDistributionChannelCallbackImpl(opaque, uEvent);
}

DataDistributionChannelCallbackImpl::DataDistributionChannelCallbackImpl(const void* opaqueCallbackReceiver, dataDistributionUnderlyingEvent uEvent)
{
	m_DataDistributionManagerChannelCallbacks.OpaqueCallbackReceiver = opaqueCallbackReceiver;
	m_DataDistributionManagerChannelCallbacks.OnUnderlyingEvent = uEvent;
}

void DataDistributionChannelCallbackImpl::OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData* uEvent)
{
	if (m_DataDistributionManagerChannelCallbacks.OnUnderlyingEvent)
	{
		m_DataDistributionManagerChannelCallbacks.OnUnderlyingEvent(m_DataDistributionManagerChannelCallbacks.OpaqueCallbackReceiver, channelHandle, uEvent);
	}
}

IDataDistributionCallback *DataDistributionCallback::create(void* opaque,
	dataDistributionConfigurationCb configurationCb,
	dataDistributionLoggingCb loggingCb,
	dataDistributionCompletelyDisconnectedCb completelyDisconnectdCb)
{
	return new DataDistributionCallbackImpl(opaque, configurationCb, loggingCb, completelyDisconnectdCb);
}

DataDistributionCallbackImpl::DataDistributionCallbackImpl(const void* opaqueCallbackReceiver,
	dataDistributionConfigurationCb configurationCb,
	dataDistributionLoggingCb loggingCb,
	dataDistributionCompletelyDisconnectedCb completelyDisconnectedCb)
{
	m_DataDistributionManagerCallbacks.OpaqueCallbackReceiver = opaqueCallbackReceiver;
	m_DataDistributionManagerCallbacks.OnConfiguration = configurationCb;
	m_DataDistributionManagerCallbacks.OnLogging = loggingCb;
	m_DataDistributionManagerCallbacks.OnCompletelyDisconnected = completelyDisconnectedCb;
}

const char* DataDistributionCallbackImpl::OnConfiguration(const char * channelName, const char* key, const char* value)
{
	if (m_DataDistributionManagerCallbacks.OnConfiguration)
	{
		return m_DataDistributionManagerCallbacks.OnConfiguration(m_DataDistributionManagerCallbacks.OpaqueCallbackReceiver, channelName, key, value);
	}

	return value;
}

void DataDistributionCallbackImpl::OnLogging(const DDM_LOG_LEVEL level, const char* channelName, const char* function, const char* str)
{
	if (m_DataDistributionManagerCallbacks.OnLogging)
	{
		m_DataDistributionManagerCallbacks.OnLogging(m_DataDistributionManagerCallbacks.OpaqueCallbackReceiver, level, channelName, function, str);
	}
}

void DataDistributionCallbackImpl::OnCompletelyDisconnected(const char* channelName, const char* str)
{
	if (m_DataDistributionManagerCallbacks.OnCompletelyDisconnected)
	{
		m_DataDistributionManagerCallbacks.OnCompletelyDisconnected(m_DataDistributionManagerCallbacks.OpaqueCallbackReceiver, channelName, str);
	}
}

IDataDistributionMastershipCallback *DataDistributionMastershipCallback::create(void* opaque, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
	dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
	dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8)
{
	return new DataDistributionMastershipCallbackImpl(opaque, p1, p2, p3, p4, p5, p6, p7, p8);
}

DataDistributionMastershipCallbackImpl::DataDistributionMastershipCallbackImpl(void* opaqueCallbackReceiver, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
	dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
	dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8)
{
	m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver = opaqueCallbackReceiver;
	m_DataDistributionMastershipCallbacks.OnClusterStateChange = p1;
	m_DataDistributionMastershipCallbacks.OnStateChange = p2;
	m_DataDistributionMastershipCallbacks.OnStateReady = p3;
	m_DataDistributionMastershipCallbacks.OnRequestedState = p4;
	m_DataDistributionMastershipCallbacks.OnMultiplePrimary = p5;
	m_DataDistributionMastershipCallbacks.FirstStateChange = p6;
	m_DataDistributionMastershipCallbacks.ChangingState = p7;
	m_DataDistributionMastershipCallbacks.ChangedState = p8;
}

void DataDistributionMastershipCallbackImpl::OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid)
{
	if (m_DataDistributionMastershipCallbacks.OnClusterStateChange)
	{
		m_DataDistributionMastershipCallbacks.OnClusterStateChange(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, change, serverid);
	}
}

void DataDistributionMastershipCallbackImpl::OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
{
	if (m_DataDistributionMastershipCallbacks.OnStateChange)
	{
		m_DataDistributionMastershipCallbacks.OnStateChange(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, newState, oldState);
	}
}

void DataDistributionMastershipCallbackImpl::OnStateReady(void* pState, int64_t len)
{
	if (m_DataDistributionMastershipCallbacks.OnStateReady)
	{
		m_DataDistributionMastershipCallbacks.OnStateReady(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, pState, len);
	}
}

void DataDistributionMastershipCallbackImpl::OnRequestedState(void** pState, size_t* len)
{
	if (m_DataDistributionMastershipCallbacks.OnRequestedState)
	{
		m_DataDistributionMastershipCallbacks.OnRequestedState(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, pState, len);
	}
}

void DataDistributionMastershipCallbackImpl::OnMultiplePrimary(int64_t myId, int64_t otherId)
{
	if (m_DataDistributionMastershipCallbacks.OnMultiplePrimary)
	{
		m_DataDistributionMastershipCallbacks.OnMultiplePrimary(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, myId, otherId);
	}
}

void DataDistributionMastershipCallbackImpl::FirstStateChange(DDM_INSTANCE_STATE newState)
{
	if (m_DataDistributionMastershipCallbacks.FirstStateChange)
	{
		m_DataDistributionMastershipCallbacks.FirstStateChange(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, newState);
	}
}

void DataDistributionMastershipCallbackImpl::ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
{
	if (m_DataDistributionMastershipCallbacks.ChangingState)
	{
		m_DataDistributionMastershipCallbacks.ChangingState(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, oldState, newState);
	}
}

void DataDistributionMastershipCallbackImpl::ChangedState(DDM_INSTANCE_STATE newState)
{
	if (m_DataDistributionMastershipCallbacks.ChangedState)
	{
		m_DataDistributionMastershipCallbacks.ChangedState(m_DataDistributionMastershipCallbacks.OpaqueCallbackReceiver, newState);
	}
}

IDataDistribution *DataDistribution::create()
{
	return new DataDistributionManagerImpl();
}

DataDistributionManagerImpl::DataDistributionManagerImpl()
{
	m_InitializedResult = NULL;
	m_hlibModule = NULL;
	m_hMastershipManagerModule = NULL;
	pDataDistributionManagerSubsystem = NULL;
	pDataDistributionMastershipManagerCommon = NULL;
}

DataDistributionManagerImpl::~DataDistributionManagerImpl()
{

}

OPERATION_RESULT DataDistributionManagerImpl::ConvertConfFile(const char* conf_file, const char*** arrayParams, int* len)
{
	// SHA512 of copyright calculated with https://www.fileformat.info/tool/hash.htm
	static const byte sStringHash[] = "c444f7fa5bdbdd738661edc4c528c82bb9ed6f4efce9da0db9403b65035a5a970f87d62362c1f9a4f9d083e5c926460292aba19e5b179b3dd68ab584ce866a35";
	int index = 0;
	if (conf_file != NULL && strlen(conf_file) != 0)
	{
		std::ifstream input(conf_file, std::ifstream::in);

		if (input.bad())
		{
			return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		}

		std::string line;
		while (std::getline(input, line))
		{
			/* Trim string */
			line.erase(0, line.find_first_not_of("\t "));
			line.erase(line.find_last_not_of("\t ") + 1);

			if (line.length() == 0 ||
				line.substr(0, 1) == "#")
				continue;
			index++;
		}
	}

	const char **array = NULL;

	if (index != 0 && conf_file != NULL && strlen(conf_file) != 0)
	{
		array = (const char**)malloc(index * sizeof(const char *));

		std::ifstream input(conf_file, std::ifstream::in);
		int index = 0;
		std::string line;
		while (std::getline(input, line))
		{
			/* Trim string */
			line.erase(0, line.find_first_not_of("\t "));
			line.erase(line.find_last_not_of("\t ") + 1);

			if (line.length() == 0 ||
				line.substr(0, 1) == "#")
				continue;

			size_t f = line.find("=");
			if (f == std::string::npos)
			{
				std::string malformed = MALFORMED_STRING;
				malformed += line;
				array[index] = _strdup(malformed.c_str());
			}
			else
			{
				array[index] = _strdup(line.c_str());
			}
			index++;
		}
	}
	*arrayParams = array;
	*len = index;
	return DDM_NO_ERROR_CONDITION;
}

OPERATION_RESULT DataDistributionManagerImpl::read_config_file(const char* arrayParams[], int len)
{
	// SHA512 of copyright calculated with https://www.fileformat.info/tool/hash.htm
	static const byte sStringHash[] = "c444f7fa5bdbdd738661edc4c528c82bb9ed6f4efce9da0db9403b65035a5a970f87d62362c1f9a4f9d083e5c926460292aba19e5b179b3dd68ab584ce866a35";

	for (int i = 0; i < len; i++)
	{
		std::string line = arrayParams[i];

		/* Trim string */
		line.erase(0, line.find_first_not_of("\t "));
		line.erase(line.find_last_not_of("\t ") + 1);

		if (line.length() == 0 ||
			line.substr(0, 1) == "#")
			continue;

		size_t f = line.find(MALFORMED_STRING);
		if (f != std::string::npos)
		{
			return DDM_PARAMETER_ERROR;
		}

		f = line.find("=");

		std::string n = line.substr(0, f);
		std::string v = line.substr(f + 1);
		std::string errstr;

		if (!strcmp(n.c_str(), "datadistributionmanager.common.protocol"))
		{
			m_Protocol = v.c_str();
			continue;
		}
		else if (!strcmp(n.c_str(), "datadistributionmanager.common.protolib"))
		{
			m_ProtocolLib = v.c_str();
			continue;
		}
		else if (!strcmp(n.c_str(), "datadistributionmanager.common.mastershiplib"))
		{
			m_MastershipLib = v.c_str();
			continue;
		}
	}

	return DDM_NO_ERROR_CONDITION;
}

OPERATION_RESULT DataDistributionManagerImpl::Initialize(IDataDistributionCallback* callbacks, const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	int len = 0;
	const char** arrayParams = NULL;

	if (conf_file == NULL) conf_file = "datadistributionmanager.conf";
	OPERATION_RESULT res = ConvertConfFile(conf_file, &arrayParams, &len);
	if (OPERATION_FAILED(res)) return res;
	return Initialize(callbacks, arrayParams, len, szMyAddress, channelTrailer);
}

OPERATION_RESULT DataDistributionManagerImpl::Initialize(IDataDistributionCallback* callbacks, const char* arrayParams[], int length, const char* szMyAddress, const char* channelTrailer)
{
	OPERATION_RESULT res = read_config_file(arrayParams, length);
	if (OPERATION_FAILED(res)) return res;

	m_arrayParamsLen = (arrayParams != NULL) ? length : 0;
	if (arrayParams != NULL)
	{
		m_arrayParams = (const char**)malloc(m_arrayParamsLen * sizeof(const char*));
		for (int i = 0; i < m_arrayParamsLen; i++)
		{
			m_arrayParams[i] = _strdup(arrayParams[i]);
		}
	}

	if (m_ProtocolLib.length() == 0)
	{
		if (m_Protocol.length() == 0) return DDM_PARAMETER_ERROR;
		m_ProtocolLib = "DataDistributionManager";
		m_ProtocolLib += m_Protocol;
#if _DEBUG
		m_ProtocolLib += "d";
#endif
	}

	// add DataDistributionManager.dll folder in the PATH variable to load DLLs
	TCHAR oldPath[64 * 1024];
	GetEnvironmentVariable("PATH", oldPath, 64 * 1024);
#if _DEBUG
	std::string moduleName("DataDistributionManagerd.dll");
#else
	std::string moduleName("DataDistributionManager.dll");
#endif
	TCHAR pathToDll[MAX_PATH];
	HMODULE ddm_Module = GetModuleHandle(moduleName.c_str());
	unsigned long moduleNameLen = GetModuleFileName(ddm_Module, pathToDll, MAX_PATH);
	std::string newFullPath(pathToDll);
	std::string path = newFullPath.substr(0, newFullPath.size() - moduleName.size());
	std::string newPath = std::string(oldPath) + std::string(";") + path;
	SetEnvironmentVariable("PATH", newPath.c_str());

	m_hlibModule = LoadLibraryA(m_ProtocolLib.c_str());

	if (!m_hlibModule) return HRESULT_FROM_WIN32(GetLastError());

	auto createProc = (pvInstantiatorFun)GetProcAddress(m_hlibModule, "CreateObjectImplementation");

	if (!createProc) return HRESULT_FROM_WIN32(GetLastError());

	pDataDistributionManagerSubsystem = static_cast<IDataDistributionSubsystem*>(createProc());

	m_InitializedResult = pDataDistributionManagerSubsystem->Initialize(callbacks, m_arrayParams, m_arrayParamsLen, szMyAddress, channelTrailer);

	if (OPERATION_FAILED(m_InitializedResult)) return m_InitializedResult;

	return m_InitializedResult;
}

OPERATION_RESULT DataDistributionManagerImpl::RequestMastershipManager(IDataDistributionMastershipCallback* mastershipCallback, const char* szMyAddress, const char* arrayParams[], int len)
{
	if (pDataDistributionMastershipManagerCommon == NULL)
	{
		if (m_MastershipLib.length() != 0)
		{
#if _DEBUG
			m_MastershipLib += "d";
#endif
			m_hMastershipManagerModule = LoadLibraryA(m_MastershipLib.c_str());
		}

		if (m_hMastershipManagerModule)
		{
			auto createMastershipProc = (pvInstantiatorFun)GetProcAddress(m_hMastershipManagerModule, "CreateObjectImplementation");
			if (!createMastershipProc)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
			pDataDistributionMastershipManagerCommon = static_cast<DataDistributionMastershipCommon*>(createMastershipProc());
		}
		else pDataDistributionMastershipManagerCommon = new DataDistributionMastershipManagerBase();

		m_InitializedMastershipResult = pDataDistributionMastershipManagerCommon->Initialize(pDataDistributionManagerSubsystem, mastershipCallback, szMyAddress,
			(arrayParams == NULL) ? m_arrayParams : arrayParams,
			(arrayParams == NULL) ? m_arrayParamsLen : len);

		if (OPERATION_FAILED(m_InitializedMastershipResult)) return m_InitializedMastershipResult;
	}
	return DDM_NO_ERROR_CONDITION;
}

BOOL DataDistributionManagerImpl::Start(unsigned long dwMilliseconds)
{
	if (OPERATION_SUCCEEDED(m_InitializedResult))
	{
		BOOL res = pDataDistributionManagerSubsystem->Start(dwMilliseconds);
		if (res)
		{
			res = pDataDistributionMastershipManagerCommon->Start(dwMilliseconds);
		}
		return res;
	}
	return FALSE;
}

BOOL DataDistributionManagerImpl::Stop(unsigned long dwMilliseconds)
{
	if (OPERATION_SUCCEEDED(m_InitializedResult))
	{
		BOOL res = pDataDistributionMastershipManagerCommon->Stop(dwMilliseconds);
		if (res)
		{
			res = pDataDistributionManagerSubsystem->Stop(dwMilliseconds);
		}
		return res;
	}
	return FALSE;
}

IDataDistributionSubsystem* DataDistributionManagerImpl::GetSubsystemManager()
{
	return pDataDistributionManagerSubsystem;
}

IDataDistributionMastershipCommon* DataDistributionManagerImpl::GetMastershipManager()
{
	return pDataDistributionMastershipManagerCommon;
}

std::string DataDistributionManagerImpl::GetProtocol() {
	return m_Protocol;
}

std::string DataDistributionManagerImpl::GetProtocolLib() {
	return m_ProtocolLib;
}

std::string DataDistributionManagerImpl::GetMastershipLib() {
	return m_MastershipLib;
}
