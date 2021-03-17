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

#if !defined(DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_)
#define DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerCommon.h"

class DDM_EXPORT DataDistributionMastershipCommon : public IDataDistributionMastershipCommon, protected IDataDistributionLog
{
public:
	DataDistributionMastershipCommon();
	virtual ~DataDistributionMastershipCommon() {}
	OPERATION_RESULT Initialize(IDataDistributionSubsystem* transportManager, IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	virtual OPERATION_RESULT Initialize(IDataDistributionMastershipCallback* cbs, const char* szMyAddress = NULL, const char* arrayParams[] = NULL, int len = 0);
	virtual void SetParameter(const char *paramName, const char *paramValue);
	virtual const char *GetParameter(const char *paramName);
	virtual OPERATION_RESULT Start(unsigned long dwMilliseconds);
	virtual OPERATION_RESULT Stop(unsigned long dwMilliseconds);
	virtual BOOL GetIamNextPrimary();
	virtual BOOL RequestIAmNextPrimary();
	virtual int64_t* GetClusterIndexes(size_t* length) = 0;
	virtual DDM_INSTANCE_STATE GetStateOf(int64_t serverId) = 0;
	virtual ClusterHealth GetClusterHealth() = 0;
	virtual DDM_INSTANCE_STATE GetMyState();
	virtual void ChangeMyState(DDM_INSTANCE_STATE);
	virtual void ChangeState(int64_t instanceId, DDM_INSTANCE_STATE);
	virtual int64_t GetLocalServerId();
	virtual int64_t GetPrimaryServerId();
	virtual int64_t GetMessageDelay();
	virtual int64_t GetUpTime();
protected:
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, ...);
	void Log(const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* format, va_list args);

protected:
	IDataDistributionSubsystem* m_pDataDistributionManagerSubsystem;
};

#endif // end DATADISTRIBUTIONMASTERSHIPMANAGERCOMMON_H__INCLUDED_