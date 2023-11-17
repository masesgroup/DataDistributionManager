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

#include "DataDistributionManagerSupport.h"
#include "windows.h"

DataDistributionHandleWrapper::DataDistributionHandleWrapper()
{
	m_hValue = NULL;
}

DataDistributionHandleWrapper::DataDistributionHandleWrapper(GENERIC_HANDLE handle)
{
	SetHandle(handle);
}

DataDistributionHandleWrapper::~DataDistributionHandleWrapper()
{
	m_hValue = NULL;
}

GENERIC_HANDLE DataDistributionHandleWrapper::GetHandle()
{
	return m_hValue;
}

void DataDistributionHandleWrapper::SetHandle(GENERIC_HANDLE handle)
{
	m_hValue = handle;
}


DataDistributionLockWrapper::DataDistributionLockWrapper()
{
	CRITICAL_SECTION* handle = new CRITICAL_SECTION;
	InitializeCriticalSection(handle);
	SetHandle(handle);
}

DataDistributionLockWrapper::~DataDistributionLockWrapper()
{
	GENERIC_HANDLE handle = GetHandle();
	DeleteCriticalSection((CRITICAL_SECTION*)handle);
	delete handle;
	SetHandle(NULL);
}

OPERATION_RESULT DataDistributionLockWrapper::Lock()
{
	GENERIC_HANDLE handle = GetHandle();
	EnterCriticalSection((CRITICAL_SECTION*)handle);
	return DDM_NO_ERROR_CONDITION;
}

OPERATION_RESULT DataDistributionLockWrapper::Unlock()
{
	GENERIC_HANDLE handle = GetHandle();
	LeaveCriticalSection((CRITICAL_SECTION*)handle);
	return DDM_NO_ERROR_CONDITION;
}

DataDistributionAutoLockWrapper::DataDistributionAutoLockWrapper(DataDistributionLockWrapper* lock)
{
	m_pLock = lock;
	m_pLock->Lock();
}

DataDistributionAutoLockWrapper::~DataDistributionAutoLockWrapper()
{
	m_pLock->Unlock();
	m_pLock = NULL;
}

DataDistributionEventWrapper::DataDistributionEventWrapper()
{
	HANDLE handle = CreateEvent(0, true, false, NULL);
	SetHandle(handle);
}

DataDistributionEventWrapper::~DataDistributionEventWrapper()
{
	CloseHandle(GetHandle());
	SetHandle(NULL);
}

OPERATION_RESULT DataDistributionEventWrapper::Set()
{
	if (SetEvent(GetHandle())) return DDM_NO_ERROR_CONDITION;
	return HRESULT_FROM_WIN32(GetLastError());
}

OPERATION_RESULT DataDistributionEventWrapper::Reset()
{
	if (ResetEvent(GetHandle())) return DDM_NO_ERROR_CONDITION;
	return HRESULT_FROM_WIN32(GetLastError());
}

OPERATION_RESULT DataDistributionEventWrapper::Wait(unsigned long timeout)
{
	OPERATION_RESULT result = DDM_NO_ERROR_CONDITION;
	auto res = WaitForSingleObject(GetHandle(), timeout);
	switch (res)
	{
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		result = HRESULT_FROM_WIN32(res);
		break;
	case WAIT_OBJECT_0:
	default:
		break;
	}
	return result;
}

unsigned long __stdcall DataDistributionThreadWrapperHandler(void * argh)
{
	ThreadWrapperArg* pArg = (ThreadWrapperArg*)argh;
	while (pArg->bIsRunning)
	{
		pArg->pEntry(pArg);
	}
	return 0;
}

DataDistributionThreadWrapper::DataDistributionThreadWrapper(threadWrapperPrototype entry, void* arg)
{
	m_pThread = new ThreadWrapperArg;
	m_pThread->bIsRunning = FALSE;
	m_pThread->pEntry = entry;
	m_pThread->user_arg = arg;
	m_pThread->pEvent = new DataDistributionEventWrapper();
}

DataDistributionThreadWrapper::~DataDistributionThreadWrapper()
{
	delete m_pThread->pEvent;
}

OPERATION_RESULT DataDistributionThreadWrapper::Start(unsigned long timeout)
{
	OPERATION_RESULT result = DDM_NO_ERROR_CONDITION;
	m_pThread->bIsRunning = TRUE;
	m_phThread = CreateThread(0, 0, DataDistributionThreadWrapperHandler, m_pThread, 0, &m_ThrId);
	return m_pThread->pEvent->Wait(timeout);
}

OPERATION_RESULT DataDistributionThreadWrapper::Stop(unsigned long timeout)
{
	m_pThread->bIsRunning = FALSE;
	CloseHandle(m_phThread);
	return DDM_NO_ERROR_CONDITION;
}

BOOL DataDistributionThreadWrapper::IsRunning()
{
	return m_pThread->bIsRunning;
}

