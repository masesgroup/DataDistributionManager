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

#include "DataDistributionManager_c.h"

DLLEXPORT void* DLLCALL DataDistribution_create()
{
	return DataDistribution::create();
}

DLLEXPORT void* DLLCALL DataDistributionCallback_create(void* opaque, dataDistributionConfigurationCb conf, dataDistributionLoggingCb log, dataDistributionCompletelyDisconnectedCb disconnected)
{
	return DataDistributionCallback::create(opaque, conf, log, disconnected);
}

DLLEXPORT void DLLCALL DataDistributionCallback_delete(void* ddcb)
{
	delete ddcb;
}

DLLEXPORT void* DLLCALL DataDistributionChannelCallback_create(void* opaque, dataDistributionUnderlyingEvent uEventCb)
{
	return DataDistributionChannelCallback::create(opaque, uEventCb);
}

DLLEXPORT void DLLCALL DataDistributionChannelCallback_delete(void* ddtcb)
{
	delete ddtcb;
}

DLLEXPORT void* DLLCALL DataDistributionMastershipCallback_create(void* opaque, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
	dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
	dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8)
{
	return DataDistributionMastershipCallback::create(opaque, p1, p2, p3, p4, p5, p6, p7, p8);
}

DLLEXPORT void DLLCALL DataDistributionMastershipCallback_delete(void* ddmcb)
{
	delete ddmcb;
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistribution_Initialize(void* IDataDistribution_instance, void* iddc, const char* conf_file, const char* szMyAddress, const char* channelTrailer)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->Initialize((IDataDistributionCallback*)iddc, conf_file, szMyAddress, channelTrailer);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistribution_Initialize2(void* IDataDistribution_instance, void* iddc, const char* arrayParams[], int len, const char* szMyAddress, const char* channelTrailer)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->Initialize((IDataDistributionCallback*)iddc, arrayParams, len, szMyAddress, channelTrailer);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistribution_RequestMastershipManager(void* IDataDistribution_instance, void* cbs, const char* szMyAddress , const char* arrayParams[], int len)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->RequestMastershipManager((IDataDistributionMastershipCallback*)cbs, szMyAddress, arrayParams, len);
}

DLLEXPORT void* DLLCALL IDataDistribution_GetSubsystemManager(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->GetSubsystemManager();
}

DLLEXPORT void* DLLCALL IDataDistribution_GetMastershipManager(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->GetMastershipManager();
}

DLLEXPORT const char* DLLCALL IDataDistribution_GetProtocol(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	std::string prot = pIDataDistribution_instance->GetProtocol();
	return prot.c_str();
}

DLLEXPORT const char* DLLCALL IDataDistribution_GetProtocolLib(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	std::string prot = pIDataDistribution_instance->GetProtocolLib();
	return prot.c_str();
}

DLLEXPORT const char* DLLCALL IDataDistribution_GetMastershipLib(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	std::string prot = pIDataDistribution_instance->GetMastershipLib();
	return prot.c_str();
}

DLLEXPORT int DLLCALL IDataDistributionSubsystem_GetServerLostTimeout(void* IDataDistributionSubsystem_instance)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->GetServerLostTimeout();
}

DLLEXPORT size_t DLLCALL IDataDistributionSubsystem_GetMaxMessageSize(void* IDataDistributionSubsystem_instance)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->GetMaxMessageSize();
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Start(void* IDataDistributionSubsystem_instance, unsigned long dwMilliseconds)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->Start(dwMilliseconds);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Stop(void* IDataDistributionSubsystem_instance, unsigned long dwMilliseconds)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->Stop(dwMilliseconds);
}

DLLEXPORT CHANNEL_HANDLE DLLCALL IDataDistributionSubsystem_CreateChannel(void* IDataDistributionSubsystem_instance, const char* channelName, void* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->CreateChannel(channelName, (IDataDistributionChannelCallback*)dataCb, direction, arrayParams, len);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_StartChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->StartChannel(channelHandle, dwMilliseconds);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_StopChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->StopChannel(channelHandle, dwMilliseconds);
}

DLLEXPORT void DLLCALL IDataDistributionSubsystem_SetParameter(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, const char* paramName, const char* paramValue)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	pIDataDistributionSubsystem_instance->SetParameter(channelHandle, paramName, paramValue);
}

DLLEXPORT void DLLCALL IDataDistributionSubsystem_SetParameter2(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId, const char* paramValue)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	pIDataDistributionSubsystem_instance->SetParameter(channelHandle, paramId, paramValue);
}

DLLEXPORT const char* DLLCALL IDataDistributionSubsystem_GetParameter(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, const char* paramName)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->GetParameter(channelHandle, paramName);
}

DLLEXPORT const char* DLLCALL IDataDistributionSubsystem_GetParameter2(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, DDM_GENERAL_PARAMETER paramId)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->GetParameter(channelHandle, paramId);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Lock(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, unsigned long timeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->Lock(channelHandle, timeout);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Unlock(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->Unlock(channelHandle);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_SeekChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, int64_t position, DDM_SEEKCONTEXT context, DDM_SEEKKIND kind)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->SeekChannel(channelHandle, position, context, kind);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_DeleteChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->DeleteChannel(channelHandle);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_WriteOnChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->WriteOnChannel(channelHandle, key, keyLen, param, dataLen, waitAll, timestamp);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_ReadFromChannel(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, int64_t offset, size_t *dataLen, void **param)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->ReadFromChannel(channelHandle, offset, dataLen, param);
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_ChangeChannelDirection(void* IDataDistributionSubsystem_instance, CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem_instance = static_cast<IDataDistributionSubsystem*>(IDataDistributionSubsystem_instance);
	return pIDataDistributionSubsystem_instance->ChangeChannelDirection(channelHandle, direction);
}

// IDataDistributionMastershipCommon
DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionMastershipCommon_Initialize(void* IDataDistribution_instance, void* iddmcb, const char* szMyAddress, const char* arrayParams[], int len)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	return pIDataDistribution_instance->RequestMastershipManager((IDataDistributionMastershipCallback*)iddmcb, szMyAddress, arrayParams, len);
}

DLLEXPORT void DLLCALL IDataDistributionMastershipCommon_SetParameter(void* IDataDistribution_instance, const char *paramName, const char *paramValue)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->SetParameter(paramName, paramValue);
	}
}

DLLEXPORT const char* DLLCALL IDataDistributionMastershipCommon_GetParameter(void* IDataDistribution_instance, const char *paramName)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->GetParameter(paramName);
	}
	return NULL;
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionMastershipCommon_Start(void* IDataDistribution_instance, unsigned long dwMilliseconds)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->Start(dwMilliseconds);
	}

	return DDM_POINTER_NOT_SET;
}

DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionMastershipCommon_Stop(void* IDataDistribution_instance, unsigned long dwMilliseconds)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->Stop(dwMilliseconds);
	}

	return DDM_POINTER_NOT_SET;
}

DLLEXPORT BOOL DLLCALL IDataDistributionMastershipCommon_GetIamNextPrimary(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->GetIamNextPrimary();
	}

	return FALSE;
}

DLLEXPORT BOOL DLLCALL IDataDistributionMastershipCommon_RequestIAmNextPrimary(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->RequestIAmNextPrimary();
	}

	return FALSE;
}

DLLEXPORT int64_t* DLLCALL IDataDistributionMastershipCommon_GetClusterIndexes(void* IDataDistribution_instance, size_t* length)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->GetClusterIndexes(length);
	}
	*length = 0;
	return NULL;
}

DLLEXPORT DDM_INSTANCE_STATE DLLCALL IDataDistributionMastershipCommon_GetStateOf(void* IDataDistribution_instance, int64_t index)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->GetStateOf(index);
	}
	return DDM_INSTANCE_STATE::UNKNOWN;
}

DLLEXPORT void* DLLCALL IDataDistributionMastershipCommon_GetClusterHealth(void* IDataDistribution_instance, int64_t index)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		auto state = pIDataDistribution_instance->GetMastershipManager()->GetClusterHealth();
		return state.at(index);
	}

	return NULL;
}

DLLEXPORT DDM_INSTANCE_STATE DLLCALL IDataDistributionMastershipCommon_GetMyState(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		return pIDataDistribution_instance->GetMastershipManager()->GetMyState();
	}

	return DDM_INSTANCE_STATE::UNKNOWN;
}

DLLEXPORT void DLLCALL IDataDistributionMastershipCommon_ChangeMyState(void* IDataDistribution_instance, DDM_INSTANCE_STATE newState)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->ChangeMyState(newState);
	}
}

DLLEXPORT void DLLCALL IDataDistributionMastershipCommon_ChangeState(void* IDataDistribution_instance, size_t instanceId, DDM_INSTANCE_STATE newState)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->ChangeState(instanceId, newState);
	}
}

DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetLocalServerId(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->GetLocalServerId();
	}

	return 0;
}

DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetPrimaryServerId(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->GetPrimaryServerId();
	}

	return 0;
}

DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetMessageDelay(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->GetMessageDelay();
	}

	return 0;
}

DLLEXPORT int64_t DLLCALL IDataDistributionMastershipCommon_GetUpTime(void* IDataDistribution_instance)
{
	IDataDistribution* pIDataDistribution_instance = static_cast<IDataDistribution*>(IDataDistribution_instance);
	if (pIDataDistribution_instance->GetMastershipManager() != NULL)
	{
		pIDataDistribution_instance->GetMastershipManager()->GetUpTime();
	}

	return 0;
}
