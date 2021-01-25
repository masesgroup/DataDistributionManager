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

#include "stdafx.h"
#include "SmartDataDistributionManager.h"
#include "DataDistributionManager_c.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <process.h> 
#include <time.h>

#define THRESHOLD 1000

#define MasterChannelName "MasterChannel"
#define SlaveChannelName "SlaveChannel"
#define ControlChannelName "ControlChannel"

static DDM_INSTANCE_STATE lastState = DDM_INSTANCE_STATE::UNKNOWN;

void stateChangeCallback(void*, DDM_INSTANCE_STATE state)
{
	lastState = state;
	switch (state)
	{
	case DDM_INSTANCE_STATE::ALONE:
		printf("Changed state to ALONE\n");
		break;
	case DDM_INSTANCE_STATE::PRIMARY:
		printf("Changed state to PRIMARY\n");
		break;
	case DDM_INSTANCE_STATE::FOLLOWER:
		printf("Changed state to FOLLOWER\n");
		break;
	default:
		printf("Changed state to %d\n", state);
		break;
	}
}

void masterDataCallbackFun(void*, void* pPointer, size_t len)
{
	static unsigned int callCounter = 0;
	unsigned int counter;
	if ((callCounter % THRESHOLD) == 0) printf("masterDataCallbackFun Reached %d\n", callCounter);
	++callCounter;
}

void masterStateCallbackFun(void*, size_t masterId, void* pPointer, size_t len)
{
	printf("Received STATE from PRIMARY/ALONE\n");
}

BOOL slaveDataCallbackFun(void*, void* pPointer, size_t len)
{
	static unsigned int callCounter = 0;
	unsigned int counter;
	if ((callCounter % THRESHOLD) == 0) printf("slaveDataCallbackFun Reached %d\n", callCounter);
	++callCounter;
	return FALSE;
}

BOOL slaveStateCallbackFun(void*, void** pPointer, size_t* len, HRESULT*)
{
	printf("Requested STATE from SLAVE\n");
	return TRUE;
}

void loggingCallbackFun(const void*, const DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* stringa)
{
	char text[100];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	strftime(text, sizeof(text) - 1, "%Y/%m/%d %H:%M:%S", t);

	printf("Timestamp: %s Source: %s Function: %s - %s\n", text, sourceName, function, stringa);
}

void dataDistributionOnUnderlyingEventCb(const void* opaque, const HANDLE channelHandle, const UnderlyingEventData* uEvent)
{

}

int interfaceCaller(BOOL sendData, const char* confFilePath)
{
	char buffer[10];
	unsigned int counter = 100;

	int pid = _getpid();

	sprintf(buffer, "%10d", pid);

	IDataDistributionCallback* cbs = DataDistributionCallback::create(NULL, NULL, loggingCallbackFun, NULL);
	IDataDistribution *manager = DataDistribution::create();
	if (manager->Initialize(cbs, confFilePath, buffer, "KafkaManager") != S_OK)
	{
		printf("Error in configuration.");
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	IDataDistributionSubsystem* subsystem = manager->GetSubsystemManager();

	if (subsystem->Start(INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}
	printf("After StartMasterConsumerAndWait...\n");

	auto tcbs = DataDistributionChannelCallback::create(NULL, dataDistributionOnUnderlyingEventCb);

	HANDLE provaHandle = subsystem->CreateChannel("prova", tcbs);

	if (subsystem->StartChannel(provaHandle, INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	Sleep(10000);

	printf("Starting sending...\n");
	while (true)
	{
		if ((sendData ? subsystem->WriteOnChannel(provaHandle, NULL, 0, buffer, 10) : S_OK) == S_OK)
		{
			sprintf(buffer, "%10d", counter++);
			if ((counter % THRESHOLD) == 0) printf("SendData Reached %d\n", counter);
		}
		Sleep(1000);
	}
}

int cstubCaller(BOOL sendData, const char* confFilePath)
{
	char buffer[10];
	unsigned int counter = 100;

	int pid = _getpid();

	sprintf(buffer, "%10d", pid);

	void* cbs_c = DataDistributionCallback_create(NULL, NULL, loggingCallbackFun, NULL);
	void* manager_c = DataDistribution_create();

	if (IDataDistribution_Initialize(manager_c, cbs_c, confFilePath, buffer, "KafkaManager") != S_OK)
	{
		printf("Error in configuration.");
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}
	void* subsystem = IDataDistribution_GetSubsystemManager(manager_c);

	if (IDataDistributionSubsystem_Start(subsystem, INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	printf("After StartMasterConsumerAndWait...\n");

	auto tcbs = DataDistributionChannelCallback_create(NULL, dataDistributionOnUnderlyingEventCb);

	HANDLE provaHandle = IDataDistributionSubsystem_CreateChannel(subsystem, "prova", tcbs);

	if (IDataDistributionSubsystem_StartChannel(subsystem, provaHandle, INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	printf("Starting sending...\n");
	while (true)
	{
		if ((sendData ? IDataDistributionSubsystem_WriteOnChannel(subsystem, provaHandle, NULL, 0, buffer, 10) : S_OK) == S_OK)
		{
			sprintf(buffer, "%10d", counter++);
			if ((counter % THRESHOLD) == 0) printf("SendData Reached %d\n", counter);
		}
		Sleep(1000);
	}
}

class MySmartDataDistributionChannel : public SmartDataDistributionChannel
{
	void OnDataAvailable(const char* key, size_t keyLen, const void* buffer, const size_t len)
	{
		printf("Data from %s with key %s and buffer is %s", GetChannelName(), key, buffer);
	}

	void OnConditionOrError(const DDM_UNDERLYING_ERROR_CONDITION errorCode, const int nativeCode, const char* subSystemReason)
	{

	}
};

class MySmartDataDistribution : public SmartDataDistribution<MySmartDataDistributionChannel>
{
	void OnLogging(DDM_LOG_LEVEL level, const char* sourceName, const char* function, const char* stringa)
	{
		char text[100];
		time_t now = time(NULL);
		struct tm *t = localtime(&now);

		strftime(text, sizeof(text) - 1, "%Y/%m/%d %H:%M:%S", t);

		printf("Timestamp: %s Source: %s Function: %s - %s\n", text, sourceName, function, stringa);
	}

};

int smartCaller(BOOL sendData, const char* confFilePath)
{
	char buffer[10];
	unsigned int counter = 100;

	int pid = _getpid();

	sprintf(buffer, "%10d", pid);

	MySmartDataDistribution dataDistribution;
	MySmartDataDistributionChannel* provaChannel;

	if (dataDistribution.Initialize(confFilePath, buffer, "KafkaManager") != S_OK)
	{
		printf("Error in configuration.");
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	if (dataDistribution.Start(INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	printf("After StartMasterConsumerAndWait...\n");

	provaChannel = dataDistribution.CreateSmartChannel("prova");

	if (provaChannel->StartChannel(INFINITE) != S_OK)
	{
		std::string str;
		std::getline(std::cin, str);
		return -1;
	}

	printf("Starting sending...\n");
	while (true)
	{
		if ((sendData ? provaChannel->WriteOnChannel(NULL, 0, buffer, 10) : S_OK) == S_OK)
		{
			sprintf(buffer, "%10d", counter++);
			if ((counter % THRESHOLD) == 0) printf("SendData Reached %d\n", counter);
		}
		Sleep(1000);
	}
}

int main(int argc, char *argv[])
{
	BOOL isPublisher = FALSE;
	if (argc < 4)
	{
		printf("ManagerTest --publisher/--receiver type (0, 1, 2) <conf-file-full-path>\n");
		return -1;
	}

	if (strcmp(argv[1], "--publisher") == 0)
	{
		isPublisher = TRUE;
	}

	int type = atoi(argv[2]);

	switch (type)
	{
	case 0:
		return interfaceCaller(isPublisher, argv[3]);
	case 1:
		return cstubCaller(isPublisher, argv[3]);
	case 2:
		return smartCaller(isPublisher, argv[3]);
	default:
		printf("ManagerTest --publisher/--receiver type (0, 1, 2) <conf-file-full-path>\n");
		break;
	}

	return 0;
}



