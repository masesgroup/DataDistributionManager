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

#include "DataDistributionMastershipCommon.h"

#include <winsock2.h>

DataDistributionMastershipCommon::DataDistributionMastershipCommon()
{
	m_pDataDistributionManagerSubsystem = NULL;
}

OPERATION_RESULT DataDistributionMastershipCommon::Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress, const char* arrayParams[], int length)
{
	m_pDataDistributionManagerSubsystem = transportManager;
	return Initialize(cbs, szMyAddress, arrayParams, length);
}

OPERATION_RESULT DataDistributionMastershipCommon::Initialize(IDataDistributionMastershipCallback* cbs, const char* szMyAddress, const char* arrayParams[], int length)
{
	TRACESTART("DataDistributionMastershipCommon", "Initialize");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

void DataDistributionMastershipCommon::SetParameter(const char *paramName, const char *paramValue)
{
	TRACESTART("DataDistributionMastershipCommon", "SetParameter");
	LOG_WARNING0("Not Implemented in subclass");
}

const char *DataDistributionMastershipCommon::GetParameter(const char *paramName)
{
	TRACESTART("DataDistributionMastershipCommon", "GetParameter");
	LOG_WARNING0("Not Implemented in subclass");
	return NULL;
}

OPERATION_RESULT DataDistributionMastershipCommon::Start(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionMastershipCommon", "Start");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

OPERATION_RESULT DataDistributionMastershipCommon::Stop(unsigned long dwMilliseconds)
{
	TRACESTART("DataDistributionMastershipCommon", "Stop");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}


BOOL DataDistributionMastershipCommon::GetIamNextPrimary()
{
	TRACESTART("DataDistributionMastershipCommon", "GetIamNextPrimary");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_NOT_IMPLEMENTED;
}

int64_t DataDistributionMastershipCommon::GetUpTime()
{
	TRACESTART("DataDistributionMastershipCommon", "GetUpTime");
	LOG_WARNING0("Not Implemented in subclass");
	return 0;
}

void DataDistributionMastershipCommon::ChangeMyState(DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionMastershipCommon", "ChangeMyState");
	LOG_WARNING0("Not Implemented in subclass");
}

void DataDistributionMastershipCommon::ChangeState(int64_t instanceId, DDM_INSTANCE_STATE newState)
{
	TRACESTART("DataDistributionMastershipCommon", "ChangeState");
	LOG_WARNING0("Not Implemented in subclass");
}

DDM_INSTANCE_STATE DataDistributionMastershipCommon::GetMyState()
{
	TRACESTART("DataDistributionMastershipCommon", "GetMyState");
	LOG_WARNING0("Not Implemented in subclass");
	return DDM_INSTANCE_STATE::UNKNOWN;
}

int64_t DataDistributionMastershipCommon::GetMessageDelay()
{
	TRACESTART("DataDistributionMastershipCommon", "GetMessageDelay");
	LOG_WARNING0("Not Implemented in subclass");
	return 0;
}

BOOL DataDistributionMastershipCommon::RequestIAmNextPrimary()
{
	TRACESTART("DataDistributionMastershipCommon", "RequestIAmNextPrimary");
	LOG_WARNING0("Not Implemented in subclass");
	return FALSE;
}

int64_t DataDistributionMastershipCommon::GetLocalServerId()
{
	TRACESTART("DataDistributionMastershipCommon", "GetLocalServerId");
	LOG_WARNING0("Not Implemented in subclass");
	return 0;
}

int64_t DataDistributionMastershipCommon::GetPrimaryServerId()
{
	TRACESTART("DataDistributionMastershipCommon", "GetPrimaryServerId");
	LOG_WARNING0("Not Implemented in subclass");
	return 0;
}

void DataDistributionMastershipCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...)
{
	if (NULL == m_pDataDistributionManagerSubsystem) return;

	va_list args = NULL;
	va_start(args, format);
	Log(level, sourceName, function, format, args);
	va_end(args);
}

void DataDistributionMastershipCommon::Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args)
{
	if (NULL == m_pDataDistributionManagerSubsystem) return;
	m_pDataDistributionManagerSubsystem->Log(level, sourceName, function, format, args);
}
