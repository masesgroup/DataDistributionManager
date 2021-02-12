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

#include "DataDistributionManagerLog.h"

DataDistributionLog::DataDistributionLog(IDataDistributionLog* pLog, const char* source, const char* function)
{
	 m_pLog = pLog;
	 m_source = source;
	 m_function = function;
	 Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "Enter");
}

DataDistributionLog::~DataDistributionLog()
{
	Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "Exit");
}

void DataDistributionLog::Log(const DDM_LOG_LEVEL level, const char* format, ...)
{
	va_list args = NULL;
	va_start(args, format);
	if (m_pLog)
	{
		m_pLog->Log(level, m_source, m_function, format, args);
	}

	va_end(args);
}
