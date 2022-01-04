/*
*  Copyright 2022 MASES s.r.l.
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

#if !defined(DATADISTRIBUTIONMANAGERLOG_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERLOG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SmartDataDistributionManager.h"

class DDM_EXPORT DataDistributionLog
{
public:
	DataDistributionLog(IDataDistributionLog* pLog, const char* source, const char* function);
	DataDistributionLog(IDataDistributionLog* pLog, IDataDistributionChannel* pChannel, const char* source, const char* function);
	~DataDistributionLog();

	void Log(const DDM_LOG_LEVEL level, const char* format, ...);
private:
	IDataDistributionLog* m_pLog;
	IDataDistributionChannel* m_pChannel;
	const char* m_source;
	const char* m_function;
};

#define TRACESTART(A, B) DataDistributionLog dataDistributionLog(this, A, B);
#define TRACECHANNELSTART(A, B, C) DataDistributionLog dataDistributionLog(this, A, B, C);

#define LOG_FATAL0(value) dataDistributionLog.Log(DDM_LOG_LEVEL::FATAL_LEVEL, value);
#define LOG_ERROR0(value) dataDistributionLog.Log(DDM_LOG_LEVEL::ERROR_LEVEL, value);
#define LOG_WARNING0(value) dataDistributionLog.Log(DDM_LOG_LEVEL::WARNING_LEVEL, value);
#define LOG_INFO0(value) dataDistributionLog.Log(DDM_LOG_LEVEL::INFO_LEVEL, value);
#define LOG_DEBUG0(value) dataDistributionLog.Log(DDM_LOG_LEVEL::DEBUG_LEVEL, value);

#define LOG_FATAL(format, ...) dataDistributionLog.Log(DDM_LOG_LEVEL::FATAL_LEVEL, format, __VA_ARGS__);
#define LOG_ERROR(format, ...) dataDistributionLog.Log(DDM_LOG_LEVEL::ERROR_LEVEL, format, __VA_ARGS__);
#define LOG_WARNING(format, ...) dataDistributionLog.Log(DDM_LOG_LEVEL::WARNING_LEVEL, format, __VA_ARGS__);
#define LOG_INFO(format, ...) dataDistributionLog.Log(DDM_LOG_LEVEL::INFO_LEVEL, format, __VA_ARGS__);
#define LOG_DEBUG(format, ...) dataDistributionLog.Log(DDM_LOG_LEVEL::DEBUG_LEVEL, format, __VA_ARGS__);

#endif // !defined(DATADISTRIBUTIONMANAGERLOG_H__INCLUDED_)