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

#include "DataDistributionManagerOpenDDS.h"
#include "CommonDataReaderListenerImpl.h"
#include "DataDistributionSchemaTypeSupportC.h"
#include "DataDistributionSchemaTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <ace/streams.h>
#include <ace/Init_ACE.h>
#include <ace/ARGV.h>

const char* DATADISTRIBUTION_SCHEMA_OPENDDSMSG_TYPE = "OpenDDS Msg Type";

extern "C" __declspec(dllexport) void* CreateObjectImplementation()
{
	return static_cast<void*> (new DataDistributionManagerOpenDDS);
}

DataDistributionManagerOpenDDS::DataDistributionManagerOpenDDS()
{
	ACE::init();
	ACE_Log_Msg::instance()->msg_callback(this);
	ACE_Log_Msg::instance()->enable_debug_messages();
	ACE_Log_Msg::instance()->open("DataDistributionManagerOpenDDS", ACE_Log_Msg::MSG_CALLBACK | ACE_Log_Msg::VERBOSE);

	m_dpf = DDS::DomainParticipantFactory::_nil();
	m_participant = DDS::DomainParticipant::_nil();
	m_domainId = 42;
	m_cmdLine = "";
	m_argc = 0;
	m_argv = NULL;
}

DataDistributionManagerOpenDDS::~DataDistributionManagerOpenDDS()
{
	ACE::fini();
}

void DataDistributionManagerOpenDDS::log(ACE_Log_Record &log_record)
{
	DDM_LOG_LEVEL level;

	switch (log_record.type())
	{
	case ACE_Log_Priority::LM_EMERGENCY:
	case ACE_Log_Priority::LM_CRITICAL:
		level = DDM_LOG_LEVEL::FATAL_LEVEL;
		break;
	case ACE_Log_Priority::LM_WARNING:level = DDM_LOG_LEVEL::WARNING_LEVEL; break;
	case ACE_Log_Priority::LM_ERROR:level = DDM_LOG_LEVEL::ERROR_LEVEL; break;
	case ACE_Log_Priority::LM_INFO:level = DDM_LOG_LEVEL::INFO_LEVEL; break;
	default:level = DDM_LOG_LEVEL::DEBUG_LEVEL; break;
	}

#define BUFFER_LEN 1024
	ACE_TCHAR buffer[BUFFER_LEN];

	log_record.format_msg(GetServerName().c_str(), ACE_Log_Msg::MSG_CALLBACK | ACE_Log_Msg::VERBOSE, buffer, BUFFER_LEN);

	Log(level, "ACE::Log", "Generic DataDistributionManagerOpenDDS::log", buffer);

	//	Log(level, "ACE::Log", (log_record.category()) ? log_record.category()->name() : "Generic DataDistributionManagerOpenDDS log", "Prio: %s, Message: %s", ACE_Log_Record::priority_name((ACE_Log_Priority)log_record.priority()), log_record.msg_data());
}

DDM_UNDERLYING_ERROR_CONDITION DataDistributionManagerOpenDDS::OpenDDSErrorMapper(CORBA::Long code)
{
	switch (code)
	{
	default:
		break;
	}
	return DDM_UNDERLYING_ERROR_CONDITION::DDM_NO_ERROR_CONDITION;
}

HRESULT DataDistributionManagerOpenDDS::conf_init(pChannelConfigurationOpenDDS configuration, const char* arrayParams[], int len)
{
	return 0;
}

HRESULT DataDistributionManagerOpenDDS::read_config_file(pChannelConfigurationOpenDDS configuration, const char* arrayParams[], int len)
{
	// SHA512 of copyright calculated with https://www.fileformat.info/tool/hash.htm
	static const byte sStringHash[] = "c444f7fa5bdbdd738661edc4c528c82bb9ed6f4efce9da0db9403b65035a5a970f87d62362c1f9a4f9d083e5c926460292aba19e5b179b3dd68ab584ce866a35";

	for (size_t i = 0; i < len; i++)
	{
		std::string line = arrayParams[i];

		/* Trim string */
		line.erase(0, line.find_first_not_of("\t "));
		line.erase(line.find_last_not_of("\t ") + 1);

		if (line.length() == 0 ||
			line.substr(0, 1) == "#")
			continue;

		size_t f = line.find("=");
		if (f == std::string::npos)
		{
			Log(DDM_LOG_LEVEL::ERROR_LEVEL, "read_config_file", "Conf file: malformed line: %s", line.c_str());
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
		}

		std::string n = line.substr(0, f);
		std::string v = line.substr(f + 1);

		v = CheckConfigurationParameter(n, v);

		SetParameter(configuration, n.c_str(), v.c_str());
	}

	return S_OK;
}

DDS::Duration_t DataDistributionManagerOpenDDS::DurationFromMs(int milliseconds)
{
	DDS::Duration_t timeout = { CORBA::Long(milliseconds / 1000), CORBA::ULong((milliseconds * 1000000) % 1000000000) };

	return timeout;
}

void DataDistributionManagerOpenDDS::SetCmdLine(std::string cmdLine)
{
	if (m_cmdLine.length() != 0) return;

	m_cmdLine = cmdLine;

	if (m_cmdLine.length() != 0)
	{
		m_myArgs = new ACE_ARGV_T<char>(strdup(m_cmdLine.c_str()));
		m_argc = m_myArgs->argc();
		m_argv = m_myArgs->argv();
	}
}

HRESULT DataDistributionManagerOpenDDS::Initialize()
{
	if (read_config_file(NULL, GetArrayParams(), GetArrayParamsLen()) != NO_ERROR)
	{
		return E_FAIL;
	}

	m_dpf = TheParticipantFactoryWithArgs(m_argc, m_argv);

	if (CORBA::is_nil(m_dpf.in()))
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, "General", "Initialize", "TheParticipantFactoryWithArgs failed.");
		return E_FAIL;
	}

	m_participant = m_dpf->create_participant(
		m_domainId,
		PARTICIPANT_QOS_DEFAULT,
		DDS::DomainParticipantListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(m_participant.in()))
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, "General", "Initialize", "create_participant failed.");
		return E_FAIL;
	}

	DDS::InstanceHandleSeq participantsInstanceHandleSeq;
	DDS::ReturnCode_t retCode = m_participant->get_discovered_participants(participantsInstanceHandleSeq);
	if (retCode == DDS::RETCODE_OK)
	{
		for (size_t i = 0; i < participantsInstanceHandleSeq.length(); i++)
		{
			DDS::InstanceHandle_t handle = participantsInstanceHandleSeq[i];
			DDS::ParticipantBuiltinTopicData data;
			retCode = m_participant->get_discovered_participant_data(data, handle);
			if (retCode == DDS::RETCODE_OK)
			{

			}
		}
	}

	DDS::InstanceHandleSeq channelsInstanceHandleSeq;
	retCode = m_participant->get_discovered_topics(channelsInstanceHandleSeq);
	if (retCode == DDS::RETCODE_OK)
	{
		for (size_t i = 0; i < channelsInstanceHandleSeq.length(); i++)
		{
			DDS::InstanceHandle_t handle = channelsInstanceHandleSeq[i];
			DDS::TopicBuiltinTopicData data;
			retCode = m_participant->get_discovered_topic_data(data, handle);
			if (retCode == DDS::RETCODE_OK)
			{

			}
		}
	}

	// Register the OpenDDSMsg type
	openddsmsg_servant = new DataDistributionSchema::OpenDDSMsgTypeSupportImpl();
	if (DDS::RETCODE_OK != openddsmsg_servant->register_type(m_participant.in(),
		DATADISTRIBUTION_SCHEMA_OPENDDSMSG_TYPE)) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, "Initialize", "register_type for %s failed", DATADISTRIBUTION_SCHEMA_OPENDDSMSG_TYPE);
		return E_FAIL;
	}

	// Get QoS to use for our two channels
	// Could also use TOPIC_QOS_DEFAULT instead
	m_participant->get_default_topic_qos(m_default_channel_qos);

	DDS::Duration_t duration;
	duration.sec = 10;
	duration.nanosec = 0;

	m_default_channel_qos.deadline.period = duration;

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::Lock(HANDLE channelHandle, DWORD timeout)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "Lock", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return pChannelConfiguration->SetLockState();
}

HRESULT DataDistributionManagerOpenDDS::Unlock(HANDLE channelHandle)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "Unlock", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return pChannelConfiguration->ResetLockState();
}

HANDLE DataDistributionManagerOpenDDS::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, channelName, "CreateChannel", "SubSystem not started.");
		return NULL;
	}

	std::string sChannelName = channelName;
#if !TEST_CONSOLE
	sChannelName = sChannelName.append(GetChannelTrailer());
#endif

	pChannelConfigurationOpenDDS pChannelConfiguration = new ChannelConfigurationOpenDDS(sChannelName.c_str(), direction, this, dataCb);

	std::string errstr;

	int retVal = conf_init(pChannelConfiguration, (arrayParams == NULL) ? GetArrayParams() : arrayParams, (len == 0) ? GetArrayParamsLen() : len);
	if (retVal != NO_ERROR)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "set conf_init error: %d", retVal);
		return NULL;
	}

	// Get QoS to use for our two channels
	// Could also use TOPIC_QOS_DEFAULT instead
	m_participant->get_default_topic_qos(m_default_channel_qos);

	DDS::Duration_t duration;
	duration.sec = 10;
	duration.nanosec = 0;

	m_default_channel_qos.deadline.period = duration;

	// Create a channel for the Quote type...
	pChannelConfiguration->channel_channel = m_participant->create_topic(pChannelConfiguration->GetChannelName(),
		DATADISTRIBUTION_SCHEMA_OPENDDSMSG_TYPE,
		m_default_channel_qos,
		DDS::TopicListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(pChannelConfiguration->channel_channel.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "create_channel for %s failed", pChannelConfiguration->GetChannelName());
		return NULL;
	}

	pChannelConfiguration->publisher = m_participant->create_publisher(PUBLISHER_QOS_DEFAULT,
		DDS::PublisherListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	// Get the default QoS for our Data Writers
	// Could also use DATAWRITER_QOS_DEFAULT
	pChannelConfiguration->publisher->get_default_datawriter_qos(pChannelConfiguration->dw_default_qos);

	duration.sec = 10;
	duration.nanosec = 0;

	pChannelConfiguration->dw_default_qos.deadline.period = duration;

	// Create a Primary DataWriter for the Quote channel
	pChannelConfiguration->channel_base_dw = pChannelConfiguration->publisher->create_datawriter(pChannelConfiguration->channel_channel.in(),
		pChannelConfiguration->dw_default_qos,
		DDS::DataWriterListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (CORBA::is_nil(pChannelConfiguration->channel_base_dw.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "create_datawriter for %s failed.", pChannelConfiguration->GetChannelName());
		return NULL;
	}

	pChannelConfiguration->channel_dw = DataDistributionSchema::OpenDDSMsgDataWriter::_narrow(pChannelConfiguration->channel_base_dw.in());
	if (CORBA::is_nil(pChannelConfiguration->channel_dw.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "Primary DataWriter could not be narrowed");
		return NULL;
	}

	// Create a subscriber for the two channels
	// (SUBSCRIBER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
	pChannelConfiguration->subscriber = m_participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
		DDS::SubscriberListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (CORBA::is_nil(pChannelConfiguration->subscriber.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "create_subscriber failed.");
		return NULL;
	}

	// Create DataReaders and DataReaderListeners for the

	pChannelConfiguration->channel_listener = DDS::DataReaderListener::_nil(); //  new CommonDataReaderListenerImpl(pChannelConfiguration);
	pChannelConfiguration->subscriber->get_default_datareader_qos(pChannelConfiguration->dr_default_qos);

	duration.sec = 10;
	duration.nanosec = 0;

	pChannelConfiguration->dr_default_qos.deadline.period = duration;

	pChannelConfiguration->channel_base_dr = pChannelConfiguration->subscriber->create_datareader(pChannelConfiguration->channel_channel.in(),
		pChannelConfiguration->dr_default_qos,
		pChannelConfiguration->channel_listener.in(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(pChannelConfiguration->channel_base_dr.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "create_datreader for %s failed.", pChannelConfiguration->GetChannelName());
		return NULL;
	}

	pChannelConfiguration->channel_dr = DataDistributionSchema::OpenDDSMsgDataReader::_narrow(pChannelConfiguration->channel_base_dr.in());
	if (CORBA::is_nil(pChannelConfiguration->channel_dr.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "CreateChannel", "Primary DataReader could not be narrowed");;
		return NULL;
	}

	channelVector.push_back(pChannelConfiguration);

	return pChannelConfiguration;
}

HRESULT DataDistributionManagerOpenDDS::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "Start", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return StartConsumerAndWait(pChannelConfiguration, dwMilliseconds);
}

HRESULT DataDistributionManagerOpenDDS::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "Start", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return S_OK;
}

void DataDistributionManagerOpenDDS::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionManagerOpenDDS", "SetParameter", "Name: %s - Value: %s", (paramName != NULL) ? paramName : "", (paramValue != NULL) ? paramValue : "");

	if (NULL != channelHandle)
	{
		pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

		if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
		{
			SetMaxMessageSize(_atoi64(paramValue));
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
		{
			pChannelConfiguration->m_ServerLostTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.createchannel"))
		{
			pChannelConfiguration->m_CreateChannelTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
		{
			pChannelConfiguration->m_ChannelSeekTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.receive"))
		{
			pChannelConfiguration->m_MessageReceiveTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
		{
			pChannelConfiguration->m_KeepAliveTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
		{
			pChannelConfiguration->m_ConsumerTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
		{
			pChannelConfiguration->m_ProducerTimeout = atoi(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
		{
			if (!strcmp(paramValue, "true") ||
				!strcmp(paramValue, "1"))
				pChannelConfiguration->m_CommitSync = TRUE;
			else
				pChannelConfiguration->m_CommitSync = FALSE;
			return;
		}
	}
	else
	{
		// if channel handle is NULL we are in Initialize and we need to get only the following parameters
		if (!strcmp(paramName, "datadistributionmanager.opendds.cmdlineargs"))
		{
			SetCmdLine(paramValue);
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.domain_id"))
		{
			m_domainId = atoi(paramValue);
			return;
		}
	}
}

static const char* ConvertIToA(int value)
{
	return _strdup(itoa(value, NULL, 0));
}

static const char* ConvertIToA(size_t value)
{
#ifdef _WIN64
	return _strdup(_ui64toa(value, NULL, 0));
#else
	return _strdup(itoa(value, NULL, 0));
#endif
}

const char* DataDistributionManagerOpenDDS::GetParameter(HANDLE channelHandle, const char* paramName)
{
	Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionManagerOpenDDS", "GetParameter", "Name: %s", (paramName != NULL) ? paramName : "");

	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
	{
		return ConvertIToA(GetMaxMessageSize());
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
	{
		return ConvertIToA(pChannelConfiguration->m_ServerLostTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.CreateChannel"))
	{
		return ConvertIToA(pChannelConfiguration->m_CreateChannelTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
	{
		return ConvertIToA(pChannelConfiguration->m_ChannelSeekTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.firstconnection"))
	{
		return ConvertIToA(pChannelConfiguration->m_MessageReceiveTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
	{
		return ConvertIToA(pChannelConfiguration->m_KeepAliveTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
	{
		return ConvertIToA(pChannelConfiguration->m_ConsumerTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
	{
		return ConvertIToA(pChannelConfiguration->m_ProducerTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
	{
		if (pChannelConfiguration->m_CommitSync) return "true";
		else return "false";
	}
	else if (!strcmp(paramName, "datadistributionmanager.opendds.cmdlineargs"))
	{
		return m_cmdLine.c_str();
	}
	else if (!strcmp(paramName, "datadistributionmanager.opendds.domain_id"))
	{
		return itoa(m_domainId, NULL, 0);
	}

	return NULL;
}

HRESULT DataDistributionManagerOpenDDS::SeekChannel(HANDLE channelHandle, size_t position)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "SeekChannel", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::DeleteChannel(HANDLE channelHandle)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "DeleteChannel", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *buffer, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "WriteOnChannel", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	DDS::ReturnCode_t retCode;

	DataDistributionSchema::OpenDDSMsg msg;
	msg.key = (key == NULL) ? "DefaultKey" : key;
	msg.msgSize = dataLen;
	msg.buffer = (dataLen != 0) ? DataDistributionSchema::OctetSeq(dataLen, dataLen, (::CORBA::Octet*)buffer, false) : NULL;

	DataDistributionSchema::OpenDDSMsgDataWriter_var generic_dw;
	DDS::InstanceHandle_t msg_handle;

	if (CORBA::is_nil(pChannelConfiguration->channel_dw.in())) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "WriteOnChannel", "writer not ready.");
		return E_FAIL;
	}

	msg_handle = pChannelConfiguration->channel_dw->register_instance(msg);
	if (timestamp != -1)
	{
#pragma warning "create time"
		::DDS::Time_t time;
		//TimeBase::TimeT retval;

		//ORBSVCS_Time::hrtime_to_TimeT(time, timestamp);
		retCode = pChannelConfiguration->channel_dw->write_w_timestamp(msg, msg_handle, time);
		if (retCode != DDS::RETCODE_OK) {
			ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SPY write returned %d.\n"), retCode));
#pragma warning "send callback"
			return E_FAIL;
		}
	}
	else
	{
		retCode = pChannelConfiguration->channel_dw->write(msg, msg_handle);
		if (retCode != DDS::RETCODE_OK) {
			ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SPY write returned %d.\n"), retCode));
#pragma warning "send callback"
			return E_FAIL;
		}
	}

	if (pChannelConfiguration->m_CommitSync)
	{
		DDS::Duration_t timeout = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->m_ProducerTimeout);
		retCode = pChannelConfiguration->channel_dw->wait_for_acknowledgments(timeout);
		if (retCode != DDS::RETCODE_OK) {
			ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SPY write returned %d.\n"), retCode));
#pragma warning "send callback"
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pChannelConfiguration->GetChannelName(), "ReadFromChannel", "SubSystem not started.");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return TRUE;
}

HRESULT DataDistributionManagerOpenDDS::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(channelHandle);
	DDM_CHANNEL_DIRECTION oldDirection = pChannelConfiguration->GetDirection();

	if (GetSubSystemStarted())
	{
		// update internal state following the direction


	}
	pChannelConfiguration->SetDirection(direction);
	return TRUE;
}

HRESULT DataDistributionManagerOpenDDS::Stop(DWORD milliseconds)
{
	return shutdown();
}

TimeBase::TimeT DataDistributionManagerOpenDDS::get_timestamp() {
	TimeBase::TimeT retval;
	ACE_hrtime_t t = ACE_OS::gethrtime();
	ORBSVCS_Time::hrtime_to_TimeT(retval, t);
	return retval;
}

HRESULT DataDistributionManagerOpenDDS::shutdown()
{
	// Cleanup
	try {
		if (!CORBA::is_nil(m_participant.in())) {
			m_participant->delete_contained_entities();
		}
		if (!CORBA::is_nil(m_dpf.in())) {
			m_dpf->delete_participant(m_participant.in());
		}
	}
	catch (CORBA::Exception& e) {
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, "General", "shutdown", "Exception caught in cleanup. %s", e._info());
		return FALSE;
	}
	TheServiceParticipant->shutdown();
	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::StartConsumerAndWait(pChannelConfigurationOpenDDS pChannelConfiguration, DWORD dwMilliseconds)
{
	HRESULT result = S_OK;
	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartConsumerAndWait", "Enter");
	pChannelConfiguration->bConsumerRun = TRUE;
	pChannelConfiguration->hConsumerThread = CreateThread(0, 0, consumerHandler, pChannelConfiguration, 0, &pChannelConfiguration->dwConsumerThrId);
	auto res = WaitForSingleObject(pChannelConfiguration->h_evtConsumer, dwMilliseconds);
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
	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartConsumerAndWait", "Exit");
	return result;
}

void DataDistributionManagerOpenDDS::StopConsumer(pChannelConfigurationOpenDDS pChannelConfiguration)
{
	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopConsumer", "Enter");
	pChannelConfiguration->bConsumerRun = FALSE;
	CloseHandle(pChannelConfiguration->hConsumerThread);
	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopConsumer", "Exit");
}

DWORD __stdcall DataDistributionManagerOpenDDS::consumerHandler(void * argh)
{
	pChannelConfigurationOpenDDS pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(argh);

	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "consumerHandler", "Entering ");

	SmartTimeMeasureWrapper timeStart;

	timeStart.ResetTime();

	pChannelConfiguration->bConsumerRun = TRUE;

	DDS::ReturnCode_t retCode;

	DDS::StatusCondition_var cond = pChannelConfiguration->channel_dr->get_statuscondition();
	retCode = cond->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);
	DDS::WaitSet_var ws = new DDS::WaitSet;
	ws->attach_condition(cond);

	DDS::ConditionSeq active;
	DDS::Duration_t ten_seconds = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->m_MessageReceiveTimeout);
	retCode = ws->wait(active, ten_seconds);

	if (retCode == DDS::RETCODE_OK)
	{
		pChannelConfiguration->SetStartupStatus(CHANNEL_STARTUP_TYPE::STARTED);
		retCode = ws->detach_condition(cond);
		if (retCode != DDS::RETCODE_OK)
		{
			pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "Failed to detach DDS::SUBSCRIPTION_MATCHED_STATUS condition");
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_UNMAPPED_ERROR_CONDITION, retCode, "Failed to detach DDS::SUBSCRIPTION_MATCHED_STATUS condition");
		}
	}
	else
	{
		pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "Error/Timeout waiting for a matched subscription status");
		pChannelConfiguration->SetStartupStatus(CHANNEL_STARTUP_TYPE::DISCONNECTED);
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_TIMEOUT, retCode, "Subscription not matched status");
		retCode = ws->detach_condition(cond);
		if (retCode != DDS::RETCODE_OK)
		{
			pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "Failed to detach DDS::SUBSCRIPTION_MATCHED_STATUS condition");
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_UNMAPPED_ERROR_CONDITION, retCode, "Failed to detach DDS::SUBSCRIPTION_MATCHED_STATUS condition");
		}
		delete ws;
		return -1;
	}

	pChannelConfiguration->WaitStartupStatus(INFINITE);

	if (pChannelConfiguration->GetStartupStatus() != CHANNEL_STARTUP_TYPE::STARTED)
	{
		pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pKafkaMessageManager->GetStartupStatus() is: %d", pChannelConfiguration->GetStartupStatus());
		pChannelConfiguration->bConsumerRun = FALSE;
		SetEvent(pChannelConfiguration->h_evtConsumer);
		return -1;
	}

	SetEvent(pChannelConfiguration->h_evtConsumer);
	timeStart.ResetTime();

	BOOL timeoutEmitted = FALSE;
	DDS::ReadCondition_var rc = pChannelConfiguration->channel_dr->create_readcondition(DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);
	DDS::Duration_t timeout = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->m_ConsumerTimeout);
	retCode = ws->attach_condition(rc);
	do
	{
		pChannelConfiguration->WaitingFinishLockState(INFINITE);

		DDS::ConditionSeq active;
		retCode = ws->wait(active, timeout);
		switch (retCode)
		{
		case DDS::RETCODE_OK:
		{
			timeoutEmitted = FALSE;
			timeStart.ResetTime();
			DDS::ReturnCode_t retCodeInner;
			DataDistributionSchema::OpenDDSMsgDataReader_var res_dr = DataDistributionSchema::OpenDDSMsgDataReader::_narrow(pChannelConfiguration->channel_dr);
			DataDistributionSchema::OpenDDSMsgSeq data;
			DDS::SampleInfoSeq info;
			retCodeInner = res_dr->take_w_condition(data, info, 1, rc);

			if (retCodeInner == DDS::RETCODE_OK && info[0].valid_data)
			{
				const char* key = data[0].key;
				size_t keyLen = (key != NULL) ? strlen(key) : 0;
				pChannelConfiguration->OnDataAvailable(key, keyLen, data[0].buffer.get_buffer(), data[0].msgSize);
			}
			else if (retCodeInner == DDS::RETCODE_NO_DATA)
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_NO_DATA_RETURNED, retCodeInner, "No data available even if status is OK");
			}
			else
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_UNMAPPED_ERROR_CONDITION, retCodeInner, "See native code error.");
			}

			pChannelConfiguration->SetManagedOffset(info[0].absolute_generation_rank); // this value shall be revised
		}
		break;
		case DDS::RETCODE_TIMEOUT:
		{
			auto duration = timeStart.ElapsedMilliseconds();

			if (!timeoutEmitted && duration > pChannelConfiguration->m_MessageReceiveTimeout) // no message within m_MessageReceiveTimeout
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT, 0, "Elapsed timeout receiving packets.");
				timeoutEmitted = TRUE;
			}
		}
		break;
		default:
			DDM_UNDERLYING_ERROR_CONDITION errCondCode = OpenDDSErrorMapper(retCode);
			pChannelConfiguration->OnConditionOrError(errCondCode, retCode, "Underlying error code from wait");
			break;
		}
	} while (pChannelConfiguration->bConsumerRun);

	return S_OK;
}

