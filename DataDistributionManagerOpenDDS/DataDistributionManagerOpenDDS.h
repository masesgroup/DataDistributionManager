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

#ifndef DATADISTRIBUTIONMANAGEROPENDDS_H
#define DATADISTRIBUTIONMANAGEROPENDDS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerCommon.h"
#include "DataDistributionSchemaTypeSupportImpl.h"
#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>
#include <orbsvcs/Time_Utilities.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/Log_Record.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ChannelConfigurationOpenDDS;

typedef ChannelConfigurationOpenDDS *pChannelConfigurationOpenDDS;

class __declspec(dllexport) DataDistributionManagerOpenDDS : public DataDistributionCommon, public ACE_Log_Msg_Callback
{
public:
	DataDistributionManagerOpenDDS();
	virtual ~DataDistributionManagerOpenDDS(void);
	size_t GetMaxStateSize();
	HRESULT Initialize();
	HANDLE CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = NULL);
	HRESULT StartChannel(HANDLE channelHandle, DWORD dwMilliseconds);
	HRESULT StopChannel(HANDLE channelHandle, DWORD dwMilliseconds);
	void SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue);
	const char* GetParameter(HANDLE channelHandle, const char* paramName);
	HRESULT Lock(HANDLE channelHandle, DWORD timeout);
	HRESULT Unlock(HANDLE channelHandle);
	HRESULT SeekChannel(HANDLE channelHandle, size_t position);
	HRESULT DeleteChannel(HANDLE channelHandle);
	HRESULT WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = -1);
	HRESULT ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param);
	HRESULT ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction);

	HRESULT Stop(DWORD milliseconds);

	int GetServerLostTimeout() { return m_ServerLostTimeout; };
	static DDM_UNDERLYING_ERROR_CONDITION OpenDDSErrorMapper(CORBA::Long code);
private:
	// ACE_Log_Msg_Callback api
	void log(ACE_Log_Record &log_record);
	// internal api
	HRESULT shutdown();
	TimeBase::TimeT get_timestamp();
	static DDS::Duration_t DurationFromMs(int ms);
	HRESULT conf_init(pChannelConfigurationOpenDDS configuration, const char* arrayParams[], int len);
	HRESULT read_config_file(pChannelConfigurationOpenDDS configuration, const char* arrayParams[], int len);
	void SetCmdLine(std::string cmdLine);
	HRESULT StartConsumerAndWait(pChannelConfigurationOpenDDS pChannelConfiguration, DWORD dwMilliseconds);
	void StopConsumer(pChannelConfigurationOpenDDS pChannelConfiguration);
	static DWORD __stdcall consumerHandler(void * argh);
private:
	::DDS::DomainId_t m_domainId;
	DataDistributionSchema::OpenDDSMsgTypeSupport_var openddsmsg_servant;
	DDS::DomainParticipantFactory_var m_dpf;
	DDS::DomainParticipant_var m_participant;
	DDS::TopicQos m_default_channel_qos;

	std::string	m_cmdLine;
	ACE_ARGV_T<char>* m_myArgs;
	int m_argc;
	char** m_argv;

	int  m_ServerLostTimeout;

	std::vector<pChannelConfigurationOpenDDS> channelVector;
};

class ChannelConfigurationOpenDDS : public ChannelConfiguration
{
public:
	ChannelConfigurationOpenDDS(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionManagerOpenDDS* mainManager, IDataDistributionChannelCallback* Cb)
		: ChannelConfiguration(channelName,direction, mainManager, Cb)
	{
		m_CreateChannelTimeout = 10000;
		m_ChannelSeekTimeout = 10000;
		m_ServerLostTimeout = 10000;
		m_KeepAliveTimeout = 1000;
		m_FirstConnectionTimeout = 10000;
		m_CommitSync = FALSE;
		m_ConsumerTimeout = 10;
		m_ProducerTimeout = 1;

		m_lastRoutedOffset = -1;
		m_lastManagedOffset = -1;

		h_evtConsumer = CreateEvent(0, true, false, NULL);
	}

	BOOL m_CommitSync;

	int  m_CreateChannelTimeout;
	int  m_ChannelSeekTimeout;
	int  m_ServerLostTimeout;
	int  m_KeepAliveTimeout;
	int  m_FirstConnectionTimeout;
	int  m_ConsumerTimeout;
	int  m_ProducerTimeout;

	DDS::Topic_var channel_channel;

	DDS::Publisher_var publisher;
	DDS::DataWriterQos dw_default_qos;

	DDS::DataWriter_var channel_base_dw;
	DataDistributionSchema::OpenDDSMsgDataWriter_var channel_dw;

	DDS::Subscriber_var subscriber;
	DDS::DataReaderQos dr_default_qos;

	DDS::DataReaderListener_var channel_listener;
	DDS::DataReader_var channel_base_dr;
	DataDistributionSchema::OpenDDSMsgDataReader_var channel_dr;

	HANDLE  h_evtConsumer;
	BOOL	bConsumerRun;
	DWORD	dwConsumerThrId;
	HANDLE	hConsumerThread;
};

#endif /* DATADISTRIBUTIONMANAGEROPENDDS_H  */
