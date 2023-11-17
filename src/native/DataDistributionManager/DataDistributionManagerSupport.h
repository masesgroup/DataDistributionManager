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

#if !defined(DATADISTRIBUTIONMANAGERSUPPORT_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERSUPPORT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerTypes.h"

/**
* @brief Basic class to manage all handle types
*
* Main entry point to activate an instance of IDataDistribution
*/
class DDM_EXPORT DataDistributionHandleWrapper
{
public:
	DataDistributionHandleWrapper();
	DataDistributionHandleWrapper(GENERIC_HANDLE handle);
	~DataDistributionHandleWrapper();
	GENERIC_HANDLE GetHandle();
protected:
	void SetHandle(GENERIC_HANDLE handle);
private:
	GENERIC_HANDLE m_hValue;
};

class DDM_EXPORT DataDistributionLockWrapper : public DataDistributionHandleWrapper
{
public:
	DataDistributionLockWrapper();
	~DataDistributionLockWrapper();
	OPERATION_RESULT Lock();
	OPERATION_RESULT Unlock();
};

class DDM_EXPORT DataDistributionAutoLockWrapper
{
public:
	DataDistributionAutoLockWrapper() {}
	DataDistributionAutoLockWrapper(DataDistributionLockWrapper*);
	~DataDistributionAutoLockWrapper();
private:
	DataDistributionLockWrapper* m_pLock;
};

class DDM_EXPORT DataDistributionEventWrapper : public DataDistributionHandleWrapper
{
public:
	DataDistributionEventWrapper();
	~DataDistributionEventWrapper();
	OPERATION_RESULT Set();
	OPERATION_RESULT Reset();
	OPERATION_RESULT Wait(unsigned long timeout);
};

struct ThreadWrapperArg;

typedef void(FUNCALL *threadWrapperPrototype)(ThreadWrapperArg *arg);

typedef struct ThreadWrapperArg
{
	threadWrapperPrototype			pEntry;
	BOOL							bIsRunning;
	void*							user_arg;
	DataDistributionEventWrapper*	pEvent;
} ThreadWrapperArg;

class DDM_EXPORT DataDistributionThreadWrapper : public DataDistributionHandleWrapper
{
public:
	DataDistributionThreadWrapper(threadWrapperPrototype entry, void* arg);
	~DataDistributionThreadWrapper();
	OPERATION_RESULT Start(unsigned long timeout);
	OPERATION_RESULT Stop(unsigned long timeout);
	BOOL IsRunning();
private:
	ThreadWrapperArg* m_pThread;
	unsigned long m_ThrId;
	GENERIC_HANDLE m_phThread;
};

#endif // !defined(DATADISTRIBUTIONMANAGERSUPPORT_H__INCLUDED_)
