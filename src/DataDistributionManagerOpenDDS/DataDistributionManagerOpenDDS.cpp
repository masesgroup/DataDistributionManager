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
	m_bStartDCPSInfoRepo = FALSE;
	m_bDCPSInfoRepoLogOnApplication = FALSE;
	m_DCPSInfoRepoCmdLine = "";
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
	case ACE_Log_Priority::LM_WARNING: level = DDM_LOG_LEVEL::WARNING_LEVEL; break;
	case ACE_Log_Priority::LM_ERROR: level = DDM_LOG_LEVEL::ERROR_LEVEL; break;
	case ACE_Log_Priority::LM_INFO: level = DDM_LOG_LEVEL::INFO_LEVEL; break;
	default: level = DDM_LOG_LEVEL::DEBUG_LEVEL; break;
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

HRESULT DataDistributionManagerOpenDDS::conf_init(ChannelConfigurationOpenDDS* configuration, const char* arrayParams[], int len)
{
	return read_config_file(configuration, arrayParams, len);
}

HRESULT DataDistributionManagerOpenDDS::read_config_file(ChannelConfigurationOpenDDS* configuration, const char* arrayParams[], int len)
{
	// SHA512 of copyright calculated with https://www.fileformat.info/tool/hash.htm
	static const byte sStringHash[] = "c444f7fa5bdbdd738661edc4c528c82bb9ed6f4efce9da0db9403b65035a5a970f87d62362c1f9a4f9d083e5c926460292aba19e5b179b3dd68ab584ce866a35";

	for (size_t i = 0; i < len; i++)
	{
		std::string line(arrayParams[i]);

		/* Trim string */
		line.erase(0, line.find_first_not_of("\t "));
		line.erase(line.find_last_not_of("\t ") + 1);

		if (line.length() == 0 ||
			line.substr(0, 1) == "#")
			continue;

		size_t f = line.find("=");
		if (f == std::string::npos)
		{
			Log(DDM_LOG_LEVEL::ERROR_LEVEL, "DataDistributionManagerOpenDDS", "read_config_file", "Conf file: malformed line: %s", line.c_str());
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
	TRACESTART("DataDistributionManagerOpenDDS", "SetCmdLine");

	LOG_INFO("Cmd line: %s", cmdLine.c_str());

	if (m_cmdLine.length() != 0) return;

	m_cmdLine = cmdLine;

	if (m_cmdLine.length() != 0)
	{
		m_myArgs = new ACE_ARGV_T<char>(strdup(m_cmdLine.c_str()));
		m_argc = m_myArgs->argc();
		m_argv = m_myArgs->argv();
	}
}

HRESULT DataDistributionManagerOpenDDS::InitializeInfoRepo()
{
	TRACESTART("DataDistributionManagerOpenDDS", "InitializeInfoRepo");

	m_hChildStd_OUT_Rd = NULL;
	m_hChildStd_OUT_Wr = NULL;
	m_hreadDataFromInfoRepo = NULL;

	if (m_bStartDCPSInfoRepo)
	{
		// get path of DataDistributionManagerOpenDDS.dll because DCPSInfoRepo is in the same folder
		TCHAR oldPath[64 * 1024];
		std::string moduleName("DataDistributionManagerOpenDDS.dll");
		TCHAR pathToDll[MAX_PATH];
		HMODULE ddm_Module = GetModuleHandle(moduleName.c_str());
		DWORD moduleNameLen = GetModuleFileName(ddm_Module, pathToDll, MAX_PATH);
		std::string newFullPath(pathToDll);
		std::string path = newFullPath.substr(0, newFullPath.size() - moduleName.size());

		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		if (m_bDCPSInfoRepoLogOnApplication)
		{
			SECURITY_ATTRIBUTES saAttr;

			ZeroMemory(&saAttr, sizeof(saAttr));
			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			// Create a pipe for the child process's STDOUT. 

			if (!CreatePipe(&m_hChildStd_OUT_Rd, &m_hChildStd_OUT_Wr, &saAttr, 0))
			{
				LOG_ERROR("StdoutRd CreatePipe with error %x", HRESULT_FROM_WIN32(GetLastError()));
				return HRESULT_FROM_WIN32(GetLastError());
			}

			// Ensure the read handle to the pipe for STDOUT is not inherited.

			if (!SetHandleInformation(m_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
			{
				LOG_ERROR("Stdout SetHandleInformation with error %x", HRESULT_FROM_WIN32(GetLastError()));
				return HRESULT_FROM_WIN32(GetLastError());
			}

			si.hStdError = m_hChildStd_OUT_Wr;
			si.hStdOutput = m_hChildStd_OUT_Wr;
			si.dwFlags |= STARTF_USESTDHANDLES;
		}

		std::string commandLine = path + "DCPSInfoRepo " + m_DCPSInfoRepoCmdLine;

		// Start the child process. 
		if (!CreateProcessA(NULL,				// No module name (use command line)
			(TCHAR*)commandLine.c_str(),		// Command line
			NULL,								// Process handle not inheritable
			NULL,								// Thread handle not inheritable
			m_bDCPSInfoRepoLogOnApplication,	// Set handle inheritance to FALSE
			0,									// No creation flags
			NULL,								// Use parent's environment block
			NULL,								// Use parent's starting directory 
			&si,								// Pointer to STARTUPINFO structure
			&pi)								// Pointer to PROCESS_INFORMATION structure
			)
		{
			LOG_ERROR("CreateProcessA with error %x", HRESULT_FROM_WIN32(GetLastError()));
			return HRESULT_FROM_WIN32(GetLastError());
		}
		else if (m_bDCPSInfoRepoLogOnApplication)
		{
			m_hreadDataFromInfoRepo = CreateThread(0, 0, readDataFromInfoRepo, this, 0, NULL);
		}
	}
	return S_OK;
}

DWORD __stdcall DataDistributionManagerOpenDDS::readDataFromInfoRepo(void * argh)
{
	DWORD dwRead;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	DataDistributionManagerOpenDDS* pDataDistributionManagerOpenDDS = static_cast<DataDistributionManagerOpenDDS*>(argh);

	for (;;)
	{
		bSuccess = ReadFile(pDataDistributionManagerOpenDDS->m_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) continue;

		pDataDistributionManagerOpenDDS->Log(DDM_LOG_LEVEL::INFO_LEVEL, "External source", "DCPSInfoRepo", chBuf);

		if (!bSuccess) break;
	}
	return 0;
}

HRESULT DataDistributionManagerOpenDDS::Initialize()
{
	TRACESTART("DataDistributionManagerOpenDDS", "Initialize");

	HRESULT hr = S_OK;
	if (read_config_file(NULL, GetArrayParams(), GetArrayParamsLen()) != NO_ERROR)
	{
		return E_FAIL;
	}

	hr = InitializeInfoRepo();

	if (FAILED(hr)) return hr;

	m_dpf = TheParticipantFactoryWithArgs(m_argc, m_argv);

	if (CORBA::is_nil(m_dpf.in()))
	{
		LOG_ERROR0("TheParticipantFactoryWithArgs failed.");
		return E_FAIL;
	}

	m_participant = m_dpf->create_participant(
		m_domainId,
		PARTICIPANT_QOS_DEFAULT,
		DDS::DomainParticipantListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);

	if (CORBA::is_nil(m_participant.in()))
	{
		LOG_ERROR0("create_participant failed.");
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
		LOG_ERROR("register_type for %s failed", DATADISTRIBUTION_SCHEMA_OPENDDSMSG_TYPE);
		return E_FAIL;
	}

	// Get QoS to use for our two channels
	// Could also use TOPIC_QOS_DEFAULT instead
	m_participant->get_default_topic_qos(m_default_channel_qos);

	DDS::Duration_t duration;
	duration.sec = 10;
	duration.nanosec = 0;

	m_default_channel_qos.deadline.period = duration;

	return hr;
}

HRESULT DataDistributionManagerOpenDDS::Lock(HANDLE channelHandle, DWORD timeout)
{
	TRACESTART("DataDistributionManagerOpenDDS", "Lock");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return pChannelConfiguration->SetLockState();
}

HRESULT DataDistributionManagerOpenDDS::Unlock(HANDLE channelHandle)
{
	TRACESTART("DataDistributionManagerOpenDDS", "Unlock");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return pChannelConfiguration->ResetLockState();
}

HANDLE DataDistributionManagerOpenDDS::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	TRACESTART("DataDistributionManagerOpenDDS", "CreateChannel");

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("SubSystem not started. Channel: %s", channelName);
		return NULL;
	}

	std::string sChannelName = channelName;
#if !TEST_CONSOLE
	sChannelName = sChannelName.append(GetChannelTrailer());
#endif

	ChannelConfigurationOpenDDS* pChannelConfiguration = new ChannelConfigurationOpenDDS(sChannelName.c_str(), direction, this, dataCb);

	std::string errstr;

	int retVal = conf_init(pChannelConfiguration, (arrayParams == NULL) ? GetArrayParams() : arrayParams, (len == 0) ? GetArrayParamsLen() : len);
	if (retVal != NO_ERROR)
	{
		LOG_ERROR("Channel %s - set conf_init error: %d", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", retVal);
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
		LOG_ERROR("Channel %s - create_channel failed", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
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
		LOG_ERROR("Channel %s - create_datawriter failed.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return NULL;
	}

	pChannelConfiguration->channel_dw = DataDistributionSchema::OpenDDSMsgDataWriter::_narrow(pChannelConfiguration->channel_base_dw.in());
	if (CORBA::is_nil(pChannelConfiguration->channel_dw.in())) {
		LOG_ERROR("Channel %s - Primary DataWriter could not be narrowed", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return NULL;
	}

	// Create a subscriber for the two channels
	// (SUBSCRIBER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
	pChannelConfiguration->subscriber = m_participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
		DDS::SubscriberListener::_nil(),
		::OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if (CORBA::is_nil(pChannelConfiguration->subscriber.in())) {
		LOG_ERROR("Channel %s - create_subscriber failed.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
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
		LOG_ERROR("Channel %s - create_datreader failed.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return NULL;
	}

	pChannelConfiguration->channel_dr = DataDistributionSchema::OpenDDSMsgDataReader::_narrow(pChannelConfiguration->channel_base_dr.in());
	if (CORBA::is_nil(pChannelConfiguration->channel_dr.in())) {
		LOG_ERROR("Channel %s - Primary DataReader could not be narrowed", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return NULL;
	}

	channelVector.push_back(pChannelConfiguration);

	return pChannelConfiguration;
}

HRESULT DataDistributionManagerOpenDDS::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerOpenDDS", "StartChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return StartConsumerAndWait(pChannelConfiguration, dwMilliseconds);
}

HRESULT DataDistributionManagerOpenDDS::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerOpenDDS", "StopChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	return S_OK;
}

void DataDistributionManagerOpenDDS::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	TRACESTART("DataDistributionManagerOpenDDS", "SetParameter");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	LOG_INFO("Channel %s - Name: %s - Value: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", (paramName != NULL) ? paramName : "", (paramValue != NULL) ? paramValue : "");

	DataDistributionCommon::SetParameter(channelHandle, paramName, paramValue);

	if (NULL != channelHandle)
	{
		// Non global params
		
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
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.autostart"))
		{
			if (!strcmp(paramValue, "true") ||
				!strcmp(paramValue, "1"))
				m_bStartDCPSInfoRepo = true;
			else
				m_bStartDCPSInfoRepo = false;
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.logonapplication"))
		{
			if (!strcmp(paramValue, "true") ||
				!strcmp(paramValue, "1"))
				m_bDCPSInfoRepoLogOnApplication = true;
			else
				m_bDCPSInfoRepoLogOnApplication = false;
			return;
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.cmdlineargs"))
		{
			m_DCPSInfoRepoCmdLine = paramValue;
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
	TRACESTART("DataDistributionManagerOpenDDS", "GetParameter");
	CAST_CHANNEL(ChannelConfigurationOpenDDS)
	LOG_INFO("Channel %s - Name: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", (paramName != NULL) ? paramName : "");

	if (NULL != channelHandle)
	{
		
	}
	else
	{
		if (!strcmp(paramName, "datadistributionmanager.opendds.cmdlineargs"))
		{
			return m_cmdLine.c_str();
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.domain_id"))
		{
			return itoa(m_domainId, NULL, 0);
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.autostart"))
		{
			if (m_bStartDCPSInfoRepo) return "true";
			else return "false";
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.logonapplication"))
		{
			if (m_bDCPSInfoRepoLogOnApplication) return "true";
			else return "false";
		}
		else if (!strcmp(paramName, "datadistributionmanager.opendds.dcpsinforepo.cmdlineargs"))
		{
			return m_DCPSInfoRepoCmdLine.c_str();
		}
	}
	return DataDistributionCommon::GetParameter(channelHandle, paramName);
}

HRESULT DataDistributionManagerOpenDDS::SeekChannel(HANDLE channelHandle, size_t position)
{
	TRACESTART("DataDistributionManagerOpenDDS", "SeekChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::DeleteChannel(HANDLE channelHandle)
{
	TRACESTART("DataDistributionManagerOpenDDS", "DeleteChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *buffer, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	TRACESTART("DataDistributionManagerOpenDDS", "WriteOnChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
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
		LOG_ERROR("Channel %s - writer not ready.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
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
			ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SPY write_w_timestamp returned %d.\n"), retCode));
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_WRITE_FAILED, retCode, "Failed write_w_timestamp.");
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
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_WRITE_FAILED, retCode, "Failed write.");
			return E_FAIL;
		}
	}

	if (waitAll)
	{
		DDS::Duration_t timeout = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->GetProducerTimeout());
		retCode = pChannelConfiguration->channel_dw->wait_for_acknowledgments(timeout);
		if (retCode != DDS::RETCODE_OK) {
			ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: SPY wait_for_acknowledgments returned %d.\n"), retCode));
#pragma warning "send callback"
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT, retCode, "Failed wait_for_acknowledgments.");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{
	TRACESTART("DataDistributionManagerOpenDDS", "ReadFromChannel");

	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return TRUE;
}

HRESULT DataDistributionManagerOpenDDS::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	TRACESTART("DataDistributionManagerOpenDDS", "ReadFromChannel");
	CAST_CHANNEL(ChannelConfigurationOpenDDS)

	DDM_CHANNEL_DIRECTION oldDirection = pChannelConfiguration->GetDirection();

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	pChannelConfiguration->SetDirection(direction);
	return TRUE;
}

HRESULT DataDistributionManagerOpenDDS::Stop(DWORD milliseconds)
{
	TRACESTART("DataDistributionManagerOpenDDS", "Stop");

	DataDistributionCommon::Stop(milliseconds);

	if (m_hChildStd_OUT_Rd) CloseHandle(m_hChildStd_OUT_Rd);
	if (m_hChildStd_OUT_Wr) CloseHandle(m_hChildStd_OUT_Wr);
	if (m_hreadDataFromInfoRepo) CloseHandle(m_hreadDataFromInfoRepo);

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
	TRACESTART("DataDistributionManagerOpenDDS", "shutdown");
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
		LOG_ERROR("Exception caught in cleanup. %s", e._info());
		return S_FALSE;
	}
	TheServiceParticipant->shutdown();
	return S_OK;
}

HRESULT DataDistributionManagerOpenDDS::StartConsumerAndWait(ChannelConfigurationOpenDDS* pChannelConfiguration, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerOpenDDS", "StartConsumerAndWait");

	HRESULT result = S_OK;
	LOG_DEBUG("Channel %s - Enter.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
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
	LOG_DEBUG("Channel %s - Exit.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
	return result;
}

void DataDistributionManagerOpenDDS::StopConsumer(ChannelConfigurationOpenDDS* pChannelConfiguration)
{
	TRACESTART("DataDistributionManagerOpenDDS", "StopConsumer");

	LOG_DEBUG("Channel %s - Enter.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
	pChannelConfiguration->bConsumerRun = FALSE;
	CloseHandle(pChannelConfiguration->hConsumerThread);
	LOG_DEBUG("Channel %s - Exit.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
}

DWORD __stdcall DataDistributionManagerOpenDDS::consumerHandler(void * argh)
{
	ChannelConfigurationOpenDDS* pChannelConfiguration = static_cast<ChannelConfigurationOpenDDS*>(argh);

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
	DDS::Duration_t ten_seconds = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->GetMessageReceiveTimeout());
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
	DDS::Duration_t timeout = DataDistributionManagerOpenDDS::DurationFromMs(pChannelConfiguration->GetConsumerTimeout());
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
			if (timeoutEmitted)
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END, 0, "End timeout receiving packets.");
			}
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

			if (!timeoutEmitted && duration > pChannelConfiguration->GetMessageReceiveTimeout()) // no message within m_MessageReceiveTimeout
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN, 0, "Elapsed timeout receiving packets.");
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
