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

#ifndef _Included_DataDistributionManager_c
#define _Included_DataDistributionManager_c

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define DLLCALL __cdecl

#include "DataDistributionManager.h"

#ifdef __cplusplus
extern "C" {
#endif
	DLLEXPORT void* DLLCALL DataDistribution_create();
	DLLEXPORT void* DLLCALL DataDistributionCallback_create(void* opaque, dataDistributionConfigurationCb, dataDistributionLoggingCb, dataDistributionCompletelyDisconnectedCb);
	DLLEXPORT void DLLCALL DataDistributionCallback_delete(void* ddcb);
	DLLEXPORT void* DLLCALL DataDistributionChannelCallback_create(void* opaque, dataDistributionUnderlyingEvent uEventCb);
	DLLEXPORT void DLLCALL DataDistributionChannelCallback_delete(void* ddtcb);
	DLLEXPORT void* DLLCALL DataDistributionMastershipCallback_create(void* opaque, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
		dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
		dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8);
	DLLEXPORT void DLLCALL DataDistributionMastershipCallback_delete(void* ddmcb);

	// IDataDistribution interface
	DLLEXPORT HRESULT DLLCALL IDataDistribution_Initialize(void* IDataDistribution_instance, void* iddcb, const char* conf_file, const char* szMyAddress, const char* channelTrailer);
	DLLEXPORT HRESULT DLLCALL  IDataDistribution_Initialize2(void* IDataDistribution_instance, void* iddcb, const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer);
	DLLEXPORT HRESULT DLLCALL IDataDistribution_RequestMastershipManager(void* IDataDistribution_instance, void* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	DLLEXPORT void* DLLCALL IDataDistribution_GetSubsystemManager(void* IDataDistribution_instance);
	DLLEXPORT void* DLLCALL IDataDistribution_GetMastershipManager(void* IDataDistribution_instance);
	DLLEXPORT const char* DLLCALL IDataDistribution_GetProtocol(void* IDataDistribution_instance);
	DLLEXPORT const char* DLLCALL IDataDistribution_GetProtocolLib(void* IDataDistribution_instance);
	DLLEXPORT const char* DLLCALL IDataDistribution_GetMastershipLib(void* IDataDistribution_instance);

	// IDataDistributionSubsystem interface

	// DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Initialize(void* IDataDistributionSubsystem_instance, void*, const char* szMyAddress, const char* conf_file, const char* channelTrailer);
	// DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Initialize2(void* IDataDistributionSubsystem_instance, void*, const char* szMyAddress, const char* arrayParams[], int len, const char* channelTrailer);
	// DLLEXPORT void* DLLCALL IDataDistributionSubsystem_GetCallbacks(void* IDataDistributionSubsystem_instance);
	DLLEXPORT int DLLCALL IDataDistributionSubsystem_GetServerLostTimeout(void* IDataDistributionSubsystem_instance);
	DLLEXPORT size_t DLLCALL IDataDistributionSubsystem_GetMaxMessageSize(void* IDataDistributionSubsystem_instance);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Start(void* IDataDistributionSubsystem_instance, DWORD dwMilliseconds);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Stop(void* IDataDistributionSubsystem_instance, DWORD dwMilliseconds);
	DLLEXPORT HANDLE DLLCALL IDataDistributionSubsystem_CreateChannel(void* IDataDistributionSubsystem_instance, const char* channelName, void* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = 0);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_StartChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DWORD dwMilliseconds);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_StopChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DWORD dwMilliseconds);
	DLLEXPORT void DLLCALL IDataDistributionSubsystem_SetParameter(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, const char* paramName, const char* paramValue);
	DLLEXPORT void DLLCALL IDataDistributionSubsystem_SetParameter2(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char* paramValue);
	DLLEXPORT const char* DLLCALL IDataDistributionSubsystem_GetParameter(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, const char* paramName);
	DLLEXPORT const char* DLLCALL IDataDistributionSubsystem_GetParameter2(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Lock(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DWORD timeout);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_Unlock(void* IDataDistributionSubsystem_instance, HANDLE channelHandle);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_SeekChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, int64_t position);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_DeleteChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_WriteOnChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = -1);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_ReadFromChannel(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, int64_t offset, size_t* dataLen, void** param);
	DLLEXPORT HRESULT DLLCALL IDataDistributionSubsystem_ChangeChannelDirection(void* IDataDistributionSubsystem_instance, HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction);

	// IDataDistributionMastershipCommon
	DLLEXPORT HRESULT DLLCALL IDataDistributionMastershipCommon_Initialize(void* IDataDistribution_instance, void* iddmcb, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	DLLEXPORT HRESULT DLLCALL IDataDistributionMastershipCommon_Start(void* IDataDistribution_instance, DWORD dwMilliseconds);
	DLLEXPORT HRESULT DLLCALL IDataDistributionMastershipCommon_Stop(void* IDataDistribution_instance, DWORD dwMilliseconds);
	DLLEXPORT BOOL DLLCALL IDataDistributionMastershipCommon_GetIamNextPrimary(void* IDataDistribution_instance);
	DLLEXPORT BOOL DLLCALL IDataDistributionMastershipCommon_RequestIAmNextPrimary(void* IDataDistribution_instance);
	DLLEXPORT int64_t* DLLCALL IDataDistributionMastershipCommon_GetClusterIndexes(void* IDataDistribution_instance, size_t* length);
	DLLEXPORT DDM_INSTANCE_STATE DLLCALL IDataDistributionMastershipCommon_GetStateOf(void* IDataDistribution_instance, int64_t index);
	DLLEXPORT void* DLLCALL IDataDistributionMastershipCommon_GetClusterHealth(void* IDataDistribution_instance, int64_t index);
	DLLEXPORT DDM_INSTANCE_STATE DLLCALL IDataDistributionMastershipCommon_GetMyState(void* IDataDistribution_instance);
	DLLEXPORT void DLLCALL IDataDistributionMastershipCommon_ChangeMyState(void* IDataDistribution_instance, DDM_INSTANCE_STATE newState);
	DLLEXPORT void DLLCALL IDataDistributionMastershipCommon_ChangeState(void* IDataDistribution_instance, size_t instanceId, DDM_INSTANCE_STATE newState);
	DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetLocalServerId(void* IDataDistribution_instance);
	DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetPrimaryServerId(void* IDataDistribution_instance);
	DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetMessageDelay(void* IDataDistribution_instance);
	DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetUpTime(void* IDataDistribution_instance);

#ifdef __cplusplus
}
#endif
#endif // end DataDistributionManager_c