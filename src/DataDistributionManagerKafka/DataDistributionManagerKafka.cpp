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

#include "DataDistributionManagerKafka.h"

extern "C" __declspec(dllexport) void* CreateObjectImplementation()
{
	return static_cast<void*> (new DataDistributionManagerKafka);
}

DataDistributionManagerKafka::DataDistributionManagerKafka()
{
	SetMaxMessageSize(1024 * 1024);
}

DataDistributionManagerKafka::~DataDistributionManagerKafka()
{
}

int DataDistributionManagerKafka::read_config_file(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len)
{
	// SHA512 of copyright calculated with https://www.fileformat.info/tool/hash.htm
	static const byte sStringHash[] = "c444f7fa5bdbdd738661edc4c528c82bb9ed6f4efce9da0db9403b65035a5a970f87d62362c1f9a4f9d083e5c926460292aba19e5b179b3dd68ab584ce866a35";

	for (int i = 0; i < len; i++)
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
			configuration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "DataDistributionManagerKafka", "read_config_file", "Conf file: malformed line: %s", line.c_str());
			return ERROR_INVALID_DATA;
		}

		std::string n = line.substr(0, f);
		std::string v = line.substr(f + 1);

		v = CheckConfigurationParameter(n, v);

		SetParameter(configuration, n.c_str(), v.c_str());
	}

	return NO_ERROR;
}

int DataDistributionManagerKafka::conf_init(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len)
{
	if (!configuration->pConnection_conf)
		configuration->pConnection_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	if (!configuration->pTopic)
		configuration->pTopic_conf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

	int result = read_config_file(configuration, arrayParams, len);
	if (result != NO_ERROR) return result;

	std::string confResVal;
	auto confRes = configuration->pConnection_conf->get("client.id", confResVal);
	if (confRes != RdKafka::Conf::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, configuration->GetChannelName(), "conf_init", "Error: client.id must be set in configuration: %d", confRes);
		return E_FAIL;
	}
	confRes = configuration->pConnection_conf->get("group.id", confResVal);
	if (confRes != RdKafka::Conf::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, configuration->GetChannelName(), "conf_init", "Error: group.id must be set in configuration: %d", confRes);
		return E_FAIL;
	}
	return NO_ERROR;
}

int DataDistributionManagerKafka::admin_create_topic(rd_kafka_t *use_rk,
	const char *channelname, int partition_cnt,
	int replication_factor, int timeout_ms)
{
	TRACESTART("DataDistributionManagerKafka", "admin_create_topic");

	int retVal = NO_ERROR;

	rd_kafka_NewTopic_t *newt[1];
	const size_t newt_cnt = 1;
	rd_kafka_AdminOptions_t *options;
	rd_kafka_queue_t *rkqu;
	rd_kafka_event_t *rkev;
	const rd_kafka_CreateTopics_result_t *res;
	const rd_kafka_topic_result_t **terr;
	size_t res_cnt;
	rd_kafka_resp_err_t err;
	char errstr[512];

	rkqu = rd_kafka_queue_new(use_rk);

	newt[0] = rd_kafka_NewTopic_new(channelname, partition_cnt,
		replication_factor,
		errstr, sizeof(errstr));

	options = rd_kafka_AdminOptions_new(use_rk, RD_KAFKA_ADMIN_OP_CREATETOPICS);
	err = rd_kafka_AdminOptions_set_operation_timeout(options, timeout_ms,
		errstr,
		sizeof(errstr));

	rd_kafka_CreateTopics(use_rk, newt, newt_cnt, options, rkqu);

	/* Wait for result */
	rkev = rd_kafka_queue_poll(rkqu, timeout_ms + 2000);

	if (rkev == NULL)
	{
		retVal = ERROR_TIMEOUT;
		LOG_ERROR("Timeout on create Topic %s", channelname);
		goto exitFun;
	}

	res = rd_kafka_event_CreateTopics_result(rkev);

	terr = rd_kafka_CreateTopics_result_topics(res, &res_cnt);
	if (terr != NULL)
	{
		auto res = rd_kafka_topic_result_error(terr[0]);
		if (RD_KAFKA_RESP_ERR_TOPIC_ALREADY_EXISTS == res)
		{
			const char* errorStr = rd_kafka_topic_result_error_string(terr[0]);
			LOG_ERROR("%s", errorStr);
			retVal = NO_ERROR;
		}
		else if (res != RD_KAFKA_RESP_ERR_NO_ERROR)
		{
			const char* errorStr = rd_kafka_topic_result_error_string(terr[0]);
			LOG_ERROR("%s", errorStr);
			retVal = E_FAIL;
		}
		else
		{
			LOG_ERROR("Topic %s created", rd_kafka_topic_result_name(terr[0]));
		}
	}
exitFun:
	if (rkev != NULL) rd_kafka_event_destroy(rkev);

	if (rkqu != NULL) rd_kafka_queue_destroy(rkqu);

	if (options != NULL) rd_kafka_AdminOptions_destroy(options);

	if (newt[0] != NULL) rd_kafka_NewTopic_destroy(newt[0]);

	return retVal;
}

static std::string metadata_print(const std::string &channel, const RdKafka::Metadata *metadata)
{
	std::stringstream stdStream;

	stdStream << "Metadata for " << (channel.empty() ? "" : "all channels")
		<< "(from broker " << metadata->orig_broker_id()
		<< ":" << metadata->orig_broker_name() << std::endl;

	/* Iterate brokers */
	stdStream << " " << metadata->brokers()->size() << " brokers:" << std::endl;
	RdKafka::Metadata::BrokerMetadataIterator ib;
	for (ib = metadata->brokers()->begin();
		ib != metadata->brokers()->end();
		++ib) {
		stdStream << "  broker " << (*ib)->id() << " at "
			<< (*ib)->host() << ":" << (*ib)->port() << std::endl;
	}
	/* Iterate channels */
	stdStream << metadata->topics()->size() << " topics:" << std::endl;
	RdKafka::Metadata::TopicMetadataIterator it;
	for (it = metadata->topics()->begin();
		it != metadata->topics()->end();
		++it) {
		stdStream << "  topic \"" << (*it)->topic() << "\" with "
			<< (*it)->partitions()->size() << " partitions:";

		if ((*it)->err() != RdKafka::ERR_NO_ERROR) {
			stdStream << " " << err2str((*it)->err());
			if ((*it)->err() == RdKafka::ERR_LEADER_NOT_AVAILABLE)
				stdStream << " (try again)";
		}
		stdStream << std::endl;

		/* Iterate topic's partitions */
		RdKafka::TopicMetadata::PartitionMetadataIterator ip;
		for (ip = (*it)->partitions()->begin();
			ip != (*it)->partitions()->end();
			++ip) {
			stdStream << "    partition " << (*ip)->id()
				<< ", leader " << (*ip)->leader()
				<< ", replicas: ";

			/* Iterate partition's replicas */
			RdKafka::PartitionMetadata::ReplicasIterator ir;
			for (ir = (*ip)->replicas()->begin();
				ir != (*ip)->replicas()->end();
				++ir) {
				stdStream << (ir == (*ip)->replicas()->begin() ? "" : ",") << *ir;
			}

			/* Iterate partition's ISRs */
			stdStream << ", isrs: ";
			RdKafka::PartitionMetadata::ISRSIterator iis;
			for (iis = (*ip)->isrs()->begin(); iis != (*ip)->isrs()->end(); ++iis)
				stdStream << (iis == (*ip)->isrs()->begin() ? "" : ",") << *iis;

			if ((*ip)->err() != RdKafka::ERR_NO_ERROR)
				stdStream << ", " << RdKafka::err2str((*ip)->err()) << std::endl;
			else
				stdStream << std::endl;
		}
	}

	return stdStream.str();
}

HRESULT DataDistributionManagerKafka::Initialize()
{
	TRACESTART("DataDistributionManagerKafka", "Initialize");
	if (read_config_file(NULL, GetArrayParams(), GetArrayParamsLen()) != NO_ERROR)
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::Lock(HANDLE channelHandle, DWORD timeout)
{
	TRACESTART("DataDistributionManagerKafka", "Lock");
	CAST_CHANNEL(ChannelConfigurationKafka);

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return pChannelConfiguration->SetLockState();
}

HRESULT DataDistributionManagerKafka::Unlock(HANDLE channelHandle)
{
	TRACESTART("DataDistributionManagerKafka", "Unlock");
	CAST_CHANNEL(ChannelConfigurationKafka);

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return pChannelConfiguration->ResetLockState();
}

HANDLE DataDistributionManagerKafka::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	TRACESTART("DataDistributionManagerKafka", "CreateChannel");
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("SubSystem not started. Channel: %s", channelName);
		return NULL;
	}

	std::string sTopicName = channelName;
#if !TEST_CONSOLE
	sTopicName = sTopicName.append(GetChannelTrailer());
#endif

	ChannelConfigurationKafka* pChannelConfiguration = new ChannelConfigurationKafka(sTopicName.c_str(), direction, this, dataCb);

	std::string errstr;

	int retVal = conf_init(pChannelConfiguration, (arrayParams == NULL) ? GetArrayParams() : arrayParams, (len == 0) ? GetArrayParamsLen() : len);
	if (retVal != NO_ERROR)
	{
		LOG_ERROR("Channel %s - conf_init error: %d", pChannelConfiguration->GetChannelName(), retVal);
		return NULL;
	}

	RdKafka::Conf::ConfResult result = pChannelConfiguration->pConnection_conf->set("default_topic_conf", pChannelConfiguration->pTopic_conf, errstr);
	if (result != RdKafka::Conf::CONF_OK)
	{
		LOG_ERROR("Channel %s - set default_topic_conf error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}
	pChannelConfiguration->pRebalance_cb = new KafkaMessageManagerRebalanceCb(pChannelConfiguration);
	if (pChannelConfiguration->pConnection_conf->set("rebalance_cb", pChannelConfiguration->pRebalance_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		LOG_ERROR("Channel %s - set rebalance_cb error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}
	pChannelConfiguration->pEvent_cb = new KafkaMessageManagerEventCb(pChannelConfiguration);
	if (pChannelConfiguration->pConnection_conf->set("event_cb", pChannelConfiguration->pEvent_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		LOG_ERROR("Channel %s - set event_cb error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}
	pChannelConfiguration->pConsume_cb = new KafkaMessageManagerConsumeCb(pChannelConfiguration);
	if (pChannelConfiguration->pConnection_conf->set("consume_cb", pChannelConfiguration->pConsume_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		LOG_ERROR("Channel %s - set consume_cb error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}

	pChannelConfiguration->pProducer = RdKafka::Producer::create(pChannelConfiguration->pConnection_conf, errstr);
	if (!pChannelConfiguration->pProducer)
	{
		LOG_ERROR("Channel %s - pProducer RdKafka::Producer::create error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}

	pChannelConfiguration->pConsumer = RdKafka::KafkaConsumer::create(pChannelConfiguration->pConnection_conf, errstr);
	if (!pChannelConfiguration->pConsumer)
	{
		delete pChannelConfiguration->pProducer;
		LOG_ERROR("Channel %s - pConsumer RdKafka::KafkaConsumer::create error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}

	if (pChannelConfiguration->m_CreateTopic && admin_create_topic(pChannelConfiguration->pProducer->c_ptr(), pChannelConfiguration->GetChannelName(), 1, pChannelConfiguration->m_TopicReplicationFactor, pChannelConfiguration->GetCreateChannelTimeout()) != NO_ERROR)
	{
		delete pChannelConfiguration->pConsumer;
		delete pChannelConfiguration->pProducer;
		LOG_ERROR("Channel %s - admin_create_topic failed", pChannelConfiguration->GetChannelName());
		return NULL;
	}

	pChannelConfiguration->pTopicPartition = RdKafka::TopicPartition::create(sTopicName, 0);
	pChannelConfiguration->pTopicPartitionVector = new std::vector<RdKafka::TopicPartition*>();
	pChannelConfiguration->pTopicPartitionVector->push_back(pChannelConfiguration->pTopicPartition);

	pChannelConfiguration->pTopic = RdKafka::Topic::create(pChannelConfiguration->pProducer, sTopicName, pChannelConfiguration->pTopic_conf, errstr);
	if (!pChannelConfiguration->pTopic)
	{
		delete pChannelConfiguration->pConsumer;
		delete pChannelConfiguration->pProducer;
		LOG_ERROR("Channel %s - pPrimaryTopic RdKafka::Topic::create error: %s", pChannelConfiguration->GetChannelName(), errstr.c_str());
		return NULL;
	}

	if (pChannelConfiguration->m_DumpMetadata)
	{
		class RdKafka::Metadata *metadata;
		auto err = pChannelConfiguration->pProducer->metadata(false, pChannelConfiguration->pTopic, &metadata, pChannelConfiguration->GetCreateChannelTimeout());

		if (err != RdKafka::ERR_NO_ERROR)
		{
			LOG_ERROR("Failed to acquire metadata for topic %s: %s", pChannelConfiguration->GetChannelName(), RdKafka::err2str(err).c_str());
		}
		else
		{
			auto resultStr = metadata_print(sTopicName, metadata);
			LOG_ERROR("Channel %s - %s", pChannelConfiguration->GetChannelName(), resultStr.c_str());
		}
	}

	topicVector.push_back(pChannelConfiguration);

	return pChannelConfiguration;
}

HRESULT DataDistributionManagerKafka::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerKafka", "StartChannel");

	CAST_CHANNEL(ChannelConfigurationKafka);

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	HRESULT status = StartConsumerAndWait(pChannelConfiguration, dwMilliseconds);
	if (FAILED(status)) return status;
	status = StartPoll(pChannelConfiguration, dwMilliseconds);

	return status;
}

HRESULT DataDistributionManagerKafka::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerKafka", "StopChannel");
	CAST_CHANNEL(ChannelConfigurationKafka);
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	StopConsumer(pChannelConfiguration);
	StopPoll(pChannelConfiguration);
	return S_OK;
}

void DataDistributionManagerKafka::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	TRACESTART("DataDistributionManagerKafka", "SetParameter");

	LOG_INFO("Channel %s - Name: %s - Value : %s", (paramName != NULL) ? paramName : "", (paramValue != NULL) ? paramValue : "");

	DataDistributionCommon::SetParameter(channelHandle, paramName, paramValue);

	std::string errstr;
	CAST_CHANNEL(ChannelConfigurationKafka);

	if (!strcmp(paramName, "datadistributionmanager.kafka.producer.msgflags"))
	{
		pChannelConfiguration->m_ProducerMsgFlags = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.replicationfactor"))
	{
		pChannelConfiguration->m_TopicReplicationFactor = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.create"))
	{
		if (!strcmp(paramValue, "true") ||
			!strcmp(paramValue, "1"))
			pChannelConfiguration->m_CreateTopic = TRUE;
		else
			pChannelConfiguration->m_CreateTopic = FALSE;
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.dumpmetadata"))
	{
		if (!strcmp(paramValue, "true") ||
			!strcmp(paramValue, "1"))
			pChannelConfiguration->m_DumpMetadata = TRUE;
		else
			pChannelConfiguration->m_DumpMetadata = FALSE;
		return;
	}

	std::string n(paramName);
	std::string v(paramValue);

	RdKafka::Conf::ConfResult r = RdKafka::Conf::CONF_UNKNOWN;
	std::string sTopicName = pChannelConfiguration->GetChannelName();
	std::string strToCheck = "datadistributionmanager.kafka.topic.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pChannelConfiguration->pTopic_conf->set(n.substr(strToCheck.length()), v, errstr);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka.";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pChannelConfiguration->pConnection_conf->set(n.substr(strToCheck.length()), v, errstr);
		}
	}

	strToCheck = "datadistributionmanager.kafka." + sTopicName + ".topic.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pChannelConfiguration->pTopic_conf->set(n.substr(strToCheck.length()), v, errstr);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka." + sTopicName + ".";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pChannelConfiguration->pConnection_conf->set(n.substr(strToCheck.length()), v, errstr);
		}
	}

	if (r != RdKafka::Conf::CONF_OK) {
		LOG_ERROR("Channel %s - Error set configuration: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", errstr.c_str());
	}
}

static const char* ConvertIToA(int value)
{
	return _strdup(_itoa(value, NULL, 0));
}

static const char* ConvertIToA(size_t value)
{
#ifdef _WIN64
	return _strdup(_ui64toa(value, NULL, 0));
#else
	return _strdup(_itoa(value, NULL, 0));
#endif
}

const char* DataDistributionManagerKafka::GetParameter(HANDLE channelHandle, const char* paramName)
{
	TRACESTART("DataDistributionManagerKafka", "GetParameter");

	CAST_CHANNEL(ChannelConfigurationKafka);

	LOG_INFO("Channel %s - Name: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", (paramName != NULL) ? paramName : "");

	if (!strcmp(paramName, "datadistributionmanager.kafka.producer.msgflags"))
	{
		return ConvertIToA(pChannelConfiguration->m_ProducerMsgFlags);
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.replicationfactor"))
	{
		return ConvertIToA(pChannelConfiguration->m_TopicReplicationFactor);
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.create"))
	{
		if (pChannelConfiguration->m_CreateTopic) return "true";
		else return "false";
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.dumpmetadata"))
	{
		if (pChannelConfiguration->m_DumpMetadata) return "true";
		else return "false";
	}

	std::string n(paramName);
	std::string v;

	RdKafka::Conf::ConfResult r = RdKafka::Conf::CONF_UNKNOWN;
	std::string sTopicName = pChannelConfiguration->GetChannelName();
	std::string strToCheck = "datadistributionmanager.kafka.channel.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pChannelConfiguration->pTopic_conf->get(n.substr(strToCheck.length()), v);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka.";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pChannelConfiguration->pConnection_conf->get(n.substr(strToCheck.length()), v);
		}
	}

	strToCheck = "datadistributionmanager.kafka." + sTopicName + ".channel.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pChannelConfiguration->pTopic_conf->get(n.substr(strToCheck.length()), v);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka." + sTopicName + ".";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pChannelConfiguration->pConnection_conf->get(n.substr(strToCheck.length()), v);
		}
	}

	if (r != RdKafka::Conf::CONF_OK) {
		pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "GetParameter", "Error get configuration");
	}
	else return _strdup(v.c_str());

	return DataDistributionCommon::GetParameter(channelHandle, paramName);
}

HRESULT DataDistributionManagerKafka::SeekChannel(HANDLE channelHandle, size_t position)
{
	TRACESTART("DataDistributionManagerKafka", "SeekChannel");

	RdKafka::ErrorCode code;
	CAST_CHANNEL(ChannelConfigurationKafka);
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	std::vector<RdKafka::TopicPartition*> partVector;
	std::string sTopicName = pChannelConfiguration->GetChannelName();
	partVector.push_back(RdKafka::TopicPartition::create(sTopicName, 0, position));
	code = pChannelConfiguration->pConsumer->offsetsForTimes(partVector, pChannelConfiguration->GetCreateChannelTimeout());
	for (unsigned int i = 0; i < partVector.size(); i++)
	{
#ifdef _WIN64
		LOG_ERROR("Channel %s - offsetsForTimes [channel: %s offset:" PRId64 "]: error: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", partVector[i]->topic().c_str(), partVector[i]->offset(), RdKafka::err2str(partVector[i]->err()).c_str());
#else
		LOG_ERROR("Channel %s - offsetsForTimes [channel: %s offset: %ld ]: error: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", partVector[i]->topic().c_str(), partVector[i]->offset(), RdKafka::err2str(partVector[i]->err()).c_str());
#endif
	}
	if (code != RdKafka::ERR_NO_ERROR)
	{
		LOG_ERROR("Channel %s - offsetsForTimes error: %s", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", RdKafka::err2str(code).c_str());
		pChannelConfiguration->bConsumerRun = FALSE;
		SetEvent(pChannelConfiguration->h_evtConsumer);
		return E_FAIL;
	}
	else
	{
#ifdef _WIN64
		LOG_ERROR("Channel %s - offsetsForTimes set offset to: " PRId64 "", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", partVector[0]->offset());
#else
		LOG_ERROR("Channel %s - pChannelConfiguration->pConsumer->offsetsForTimes set offset to: %ld", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", partVector[0]->offset());
#endif
		pChannelConfiguration->SetManagedOffset(partVector[0]->offset());
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::DeleteChannel(HANDLE channelHandle)
{
	TRACESTART("DataDistributionManagerKafka", "DeleteChannel");

	CAST_CHANNEL(ChannelConfigurationKafka);
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	TRACESTART("DataDistributionManagerKafka", "WriteOnChannel");
	CAST_CHANNEL(ChannelConfigurationKafka);

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	RdKafka::ErrorCode code;
	if (timestamp != -1)
	{
		std::string sTopicName = pChannelConfiguration->GetChannelName();
		code = pChannelConfiguration->pProducer->produce(sTopicName, 0, pChannelConfiguration->m_ProducerMsgFlags,
			param, dataLen, key, keyLen, timestamp, (void*)this);
	}
	else
	{
		code = pChannelConfiguration->pProducer->produce(pChannelConfiguration->pTopic, 0, pChannelConfiguration->m_ProducerMsgFlags,
			param, dataLen, key, keyLen, (void*)this);
	}

	if (code != RdKafka::ErrorCode::ERR_NO_ERROR)
	{
		LOG_ERROR("Channel %s - Write failed with reason %s.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel", RdKafka::err2str(code).c_str());
		DDM_UNDERLYING_ERROR_CONDITION thisCode = KafkaErrorMapper(code);

		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_WRITE_FAILED, code, RdKafka::err2str(code).c_str());
		return E_FAIL;
	}
	return S_OK;
}

HRESULT DataDistributionManagerKafka::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{
	TRACESTART("DataDistributionManagerKafka", "ReadFromChannel");

	CAST_CHANNEL(ChannelConfigurationKafka);
	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		return S_FALSE;
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	TRACESTART("DataDistributionManagerKafka", "ChangeChannelDirection");

	CAST_CHANNEL(ChannelConfigurationKafka);
	DDM_CHANNEL_DIRECTION oldDirection = pChannelConfiguration->GetDirection();

	if (!GetSubSystemStarted())
	{
		LOG_ERROR("Channel %s - SubSystem not started.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}
	pChannelConfiguration->SetDirection(direction);
	return S_OK;
}

HRESULT DataDistributionManagerKafka::StartConsumerAndWait(ChannelConfigurationKafka* pChannelConfiguration, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerKafka", "StartConsumerAndWait");

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

void DataDistributionManagerKafka::StopConsumer(ChannelConfigurationKafka* pChannelConfiguration)
{
	TRACESTART("DataDistributionManagerKafka", "StopConsumer");
	LOG_DEBUG("Channel %s - Enter.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
	pChannelConfiguration->bConsumerRun = FALSE;
	CloseHandle(pChannelConfiguration->hConsumerThread);
	LOG_DEBUG("Channel %s - Exit.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
}

HRESULT DataDistributionManagerKafka::StartPoll(ChannelConfigurationKafka* pChannelConfiguration, DWORD dwMilliseconds)
{
	TRACESTART("DataDistributionManagerKafka", "StartPoll");

	HRESULT result = S_OK;
	LOG_DEBUG("Channel %s - Enter.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
	pChannelConfiguration->bPollRun = TRUE;
	pChannelConfiguration->hPollThread = CreateThread(0, 0, pollHandler, pChannelConfiguration, 0, &pChannelConfiguration->dwPollThrId);
	auto res = WaitForSingleObject(pChannelConfiguration->h_evtPoll, dwMilliseconds);
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

void DataDistributionManagerKafka::StopPoll(ChannelConfigurationKafka* pChannelConfiguration)
{
	TRACESTART("DataDistributionManagerKafka", "StopPoll");

	LOG_DEBUG("Channel %s - Enter.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
	pChannelConfiguration->bPollRun = FALSE;
	CloseHandle(pChannelConfiguration->hPollThread);
	LOG_DEBUG("Channel %s - Exit.", (pChannelConfiguration) ? pChannelConfiguration->GetChannelName() : "No channel");
}

DDM_UNDERLYING_ERROR_CONDITION DataDistributionManagerKafka::KafkaErrorMapper(RdKafka::ErrorCode code)
{
	switch (code)
	{
	case RdKafka::ERR__BEGIN:
		break;
	case RdKafka::ERR__BAD_MSG:
		break;
	case RdKafka::ERR__BAD_COMPRESSION:
		break;
	case RdKafka::ERR__DESTROY:
		break;
	case RdKafka::ERR__FAIL:
		break;
	case RdKafka::ERR__TRANSPORT:
		break;
	case RdKafka::ERR__CRIT_SYS_RESOURCE:
		break;
	case RdKafka::ERR__RESOLVE:
		break;
	case RdKafka::ERR__MSG_TIMED_OUT:
		break;
	case RdKafka::ERR__PARTITION_EOF:
		break;
	case RdKafka::ERR__UNKNOWN_PARTITION:
		break;
	case RdKafka::ERR__FS:
		break;
	case RdKafka::ERR__UNKNOWN_TOPIC:
		break;
	case RdKafka::ERR__ALL_BROKERS_DOWN:
		break;
	case RdKafka::ERR__INVALID_ARG:
		break;
	case RdKafka::ERR__TIMED_OUT:
		break;
	case RdKafka::ERR__QUEUE_FULL:
		break;
	case RdKafka::ERR__ISR_INSUFF:
		break;
	case RdKafka::ERR__NODE_UPDATE:
		break;
	case RdKafka::ERR__SSL:
		break;
	case RdKafka::ERR__WAIT_COORD:
		break;
	case RdKafka::ERR__UNKNOWN_GROUP:
		break;
	case RdKafka::ERR__IN_PROGRESS:
		break;
	case RdKafka::ERR__PREV_IN_PROGRESS:
		break;
	case RdKafka::ERR__EXISTING_SUBSCRIPTION:
		break;
	case RdKafka::ERR__ASSIGN_PARTITIONS:
		break;
	case RdKafka::ERR__REVOKE_PARTITIONS:
		break;
	case RdKafka::ERR__CONFLICT:
		break;
	case RdKafka::ERR__STATE:
		break;
	case RdKafka::ERR__UNKNOWN_PROTOCOL:
		break;
	case RdKafka::ERR__NOT_IMPLEMENTED:
		break;
	case RdKafka::ERR__AUTHENTICATION:
		break;
	case RdKafka::ERR__NO_OFFSET:
		break;
	case RdKafka::ERR__OUTDATED:
		break;
	case RdKafka::ERR__TIMED_OUT_QUEUE:
		break;
	case RdKafka::ERR__UNSUPPORTED_FEATURE:
		break;
	case RdKafka::ERR__WAIT_CACHE:
		break;
	case RdKafka::ERR__INTR:
		break;
	case RdKafka::ERR__KEY_SERIALIZATION:
		break;
	case RdKafka::ERR__VALUE_SERIALIZATION:
		break;
	case RdKafka::ERR__KEY_DESERIALIZATION:
		break;
	case RdKafka::ERR__VALUE_DESERIALIZATION:
		break;
	case RdKafka::ERR__PARTIAL:
		break;
	case RdKafka::ERR__READ_ONLY:
		break;
	case RdKafka::ERR__NOENT:
		break;
	case RdKafka::ERR__UNDERFLOW:
		break;
	case RdKafka::ERR__INVALID_TYPE:
		break;
	case RdKafka::ERR__RETRY:
		break;
	case RdKafka::ERR__PURGE_QUEUE:
		break;
	case RdKafka::ERR__PURGE_INFLIGHT:
		break;
	case RdKafka::ERR__FATAL:
		break;
	case RdKafka::ERR__INCONSISTENT:
		break;
	case RdKafka::ERR__GAPLESS_GUARANTEE:
		break;
	case RdKafka::ERR__MAX_POLL_EXCEEDED:
		break;
	case RdKafka::ERR__END:
		break;
	case RdKafka::ERR_UNKNOWN:
		break;
	case RdKafka::ERR_NO_ERROR:
		break;
	case RdKafka::ERR_OFFSET_OUT_OF_RANGE:
		break;
	case RdKafka::ERR_INVALID_MSG:
		break;
	case RdKafka::ERR_UNKNOWN_TOPIC_OR_PART:
		break;
	case RdKafka::ERR_INVALID_MSG_SIZE:
		break;
	case RdKafka::ERR_LEADER_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_NOT_LEADER_FOR_PARTITION:
		break;
	case RdKafka::ERR_REQUEST_TIMED_OUT:
		break;
	case RdKafka::ERR_BROKER_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_REPLICA_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_MSG_SIZE_TOO_LARGE:
		break;
	case RdKafka::ERR_STALE_CTRL_EPOCH:
		break;
	case RdKafka::ERR_OFFSET_METADATA_TOO_LARGE:
		break;
	case RdKafka::ERR_NETWORK_EXCEPTION:
		break;
	case RdKafka::ERR_GROUP_LOAD_IN_PROGRESS:
		break;
	case RdKafka::ERR_GROUP_COORDINATOR_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_NOT_COORDINATOR_FOR_GROUP:
		break;
	case RdKafka::ERR_TOPIC_EXCEPTION:
		break;
	case RdKafka::ERR_RECORD_LIST_TOO_LARGE:
		break;
	case RdKafka::ERR_NOT_ENOUGH_REPLICAS:
		break;
	case RdKafka::ERR_NOT_ENOUGH_REPLICAS_AFTER_APPEND:
		break;
	case RdKafka::ERR_INVALID_REQUIRED_ACKS:
		break;
	case RdKafka::ERR_ILLEGAL_GENERATION:
		break;
	case RdKafka::ERR_INCONSISTENT_GROUP_PROTOCOL:
		break;
	case RdKafka::ERR_INVALID_GROUP_ID:
		break;
	case RdKafka::ERR_UNKNOWN_MEMBER_ID:
		break;
	case RdKafka::ERR_INVALID_SESSION_TIMEOUT:
		break;
	case RdKafka::ERR_REBALANCE_IN_PROGRESS:
		break;
	case RdKafka::ERR_INVALID_COMMIT_OFFSET_SIZE:
		break;
	case RdKafka::ERR_TOPIC_AUTHORIZATION_FAILED:
		break;
	case RdKafka::ERR_GROUP_AUTHORIZATION_FAILED:
		break;
	case RdKafka::ERR_CLUSTER_AUTHORIZATION_FAILED:
		break;
	case RdKafka::ERR_INVALID_TIMESTAMP:
		break;
	case RdKafka::ERR_UNSUPPORTED_SASL_MECHANISM:
		break;
	case RdKafka::ERR_ILLEGAL_SASL_STATE:
		break;
	case RdKafka::ERR_UNSUPPORTED_VERSION:
		break;
	case RdKafka::ERR_TOPIC_ALREADY_EXISTS:
		break;
	case RdKafka::ERR_INVALID_PARTITIONS:
		break;
	case RdKafka::ERR_INVALID_REPLICATION_FACTOR:
		break;
	case RdKafka::ERR_INVALID_REPLICA_ASSIGNMENT:
		break;
	case RdKafka::ERR_INVALID_CONFIG:
		break;
	case RdKafka::ERR_NOT_CONTROLLER:
		break;
	case RdKafka::ERR_INVALID_REQUEST:
		break;
	case RdKafka::ERR_UNSUPPORTED_FOR_MESSAGE_FORMAT:
		break;
	case RdKafka::ERR_POLICY_VIOLATION:
		break;
	case RdKafka::ERR_OUT_OF_ORDER_SEQUENCE_NUMBER:
		break;
	case RdKafka::ERR_DUPLICATE_SEQUENCE_NUMBER:
		break;
	case RdKafka::ERR_INVALID_PRODUCER_EPOCH:
		break;
	case RdKafka::ERR_INVALID_TXN_STATE:
		break;
	case RdKafka::ERR_INVALID_PRODUCER_ID_MAPPING:
		break;
	case RdKafka::ERR_INVALID_TRANSACTION_TIMEOUT:
		break;
	case RdKafka::ERR_CONCURRENT_TRANSACTIONS:
		break;
	case RdKafka::ERR_TRANSACTION_COORDINATOR_FENCED:
		break;
	case RdKafka::ERR_TRANSACTIONAL_ID_AUTHORIZATION_FAILED:
		break;
	case RdKafka::ERR_SECURITY_DISABLED:
		break;
	case RdKafka::ERR_OPERATION_NOT_ATTEMPTED:
		break;
	case RdKafka::ERR_KAFKA_STORAGE_ERROR:
		break;
	case RdKafka::ERR_LOG_DIR_NOT_FOUND:
		break;
	case RdKafka::ERR_SASL_AUTHENTICATION_FAILED:
		break;
	case RdKafka::ERR_UNKNOWN_PRODUCER_ID:
		break;
	case RdKafka::ERR_REASSIGNMENT_IN_PROGRESS:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_AUTH_DISABLED:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_NOT_FOUND:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_OWNER_MISMATCH:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_REQUEST_NOT_ALLOWED:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_AUTHORIZATION_FAILED:
		break;
	case RdKafka::ERR_DELEGATION_TOKEN_EXPIRED:
		break;
	case RdKafka::ERR_INVALID_PRINCIPAL_TYPE:
		break;
	case RdKafka::ERR_NON_EMPTY_GROUP:
		break;
	case RdKafka::ERR_GROUP_ID_NOT_FOUND:
		break;
	case RdKafka::ERR_FETCH_SESSION_ID_NOT_FOUND:
		break;
	case RdKafka::ERR_INVALID_FETCH_SESSION_EPOCH:
		break;
	case RdKafka::ERR_LISTENER_NOT_FOUND:
		break;
	case RdKafka::ERR_TOPIC_DELETION_DISABLED:
		break;
	case RdKafka::ERR_FENCED_LEADER_EPOCH:
		break;
	case RdKafka::ERR_UNKNOWN_LEADER_EPOCH:
		break;
	case RdKafka::ERR_UNSUPPORTED_COMPRESSION_TYPE:
		break;
	case RdKafka::ERR_STALE_BROKER_EPOCH:
		break;
	case RdKafka::ERR_OFFSET_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_MEMBER_ID_REQUIRED:
		break;
	case RdKafka::ERR_PREFERRED_LEADER_NOT_AVAILABLE:
		break;
	case RdKafka::ERR_GROUP_MAX_SIZE_REACHED:
		break;
	default:
		return DDM_UNDERLYING_ERROR_CONDITION::DDM_UNMAPPED_ERROR_CONDITION;
		break;
	}
	return DDM_UNDERLYING_ERROR_CONDITION::DDM_NO_ERROR_CONDITION;
}

DWORD __stdcall DataDistributionManagerKafka::consumerHandler(void * argh)
{
	ChannelConfigurationKafka* pChannelConfiguration = static_cast<ChannelConfigurationKafka*>(argh);

	pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "consumerHandler", "Entering ");

	SmartTimeMeasureWrapper timeStart;

	timeStart.ResetTime();

	pChannelConfiguration->bConsumerRun = TRUE;

	RdKafka::ErrorCode code;
	RdKafka::Message *p_Msg;

	std::vector<std::string> vector;
	std::string sTopicName = pChannelConfiguration->GetChannelName();
	vector.push_back(sTopicName);
	code = pChannelConfiguration->pConsumer->subscribe(vector);
	if (code != RdKafka::ERR_NO_ERROR)
	{
		pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pChannelConfiguration->pConsumer->subscribe error: %s", RdKafka::err2str(code).c_str());
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, code, "Failed to start subsystem: %s", RdKafka::err2str(code).c_str());
		pChannelConfiguration->bConsumerRun = FALSE;
		SetEvent(pChannelConfiguration->h_evtConsumer);
		return -1;
	}

	BOOL result = FALSE;
	do
	{
		p_Msg = pChannelConfiguration->pConsumer->consume(10); // waiting first message to know if we are connected
		code = p_Msg->err();
		if (code == RdKafka::ErrorCode::ERR_NO_ERROR) pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "primaryConsumerHandler", "Received Message in the loop");
		result = code == RdKafka::ErrorCode::ERR__TIMED_OUT && !pChannelConfiguration->IsStartupStatusSet();
		if (result) delete p_Msg;
	} while (result);

	pChannelConfiguration->WaitStartupStatus(INFINITE);

	if (pChannelConfiguration->GetStartupStatus() != CHANNEL_STARTUP_TYPE::STARTED)
	{
		pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pKafkaMessageManager->GetStartupStatus() is: %d", pChannelConfiguration->GetStartupStatus());
		pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "Failed to start subsystem");
		pChannelConfiguration->bConsumerRun = FALSE;
		SetEvent(pChannelConfiguration->h_evtConsumer);
		return -1;
	}

	BOOL timeoutEmitted = FALSE;
	SetEvent(pChannelConfiguration->h_evtConsumer);
	timeStart.ResetTime();
	do
	{
		pChannelConfiguration->WaitingFinishLockState(INFINITE);

		BOOL deleteMsgOnExit = TRUE;
		code = p_Msg->err();
		switch (code)
		{
		case RdKafka::ErrorCode::ERR__PARTITION_EOF:
		case RdKafka::ErrorCode::ERR__TIMED_OUT:
		{
			auto duration = timeStart.ElapsedMilliseconds();

			if (!timeoutEmitted && duration > pChannelConfiguration->GetMessageReceiveTimeout()) // no message within m_MessageReceiveTimeout
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN, 0, "Elapsed timeout receiving packets.");
				timeoutEmitted = TRUE;
			}
		}
		break;
		case RdKafka::ErrorCode::ERR_NO_ERROR:
		{
			if (timeoutEmitted)
			{
				pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END, 0, "End timeout receiving packets.");
			}
			timeoutEmitted = FALSE;
			timeStart.ResetTime();

			pChannelConfiguration->OnDataAvailable(p_Msg->key()->c_str(), p_Msg->key_len(), p_Msg->payload(), p_Msg->len());

			if (deleteMsgOnExit)
			{
				if (pChannelConfiguration->GetCommitSync())
				{
					code = pChannelConfiguration->pConsumer->commitSync(p_Msg);
				}
				else
				{
					code = pChannelConfiguration->pConsumer->commitAsync(p_Msg);
				}
				if (code != RdKafka::ERR_NO_ERROR)
				{
					pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "%s error: %s", pChannelConfiguration->GetCommitSync() ? "commitSync" : "commitAsync", RdKafka::err2str(code).c_str());
					pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_COMMIT_FAILED, code, "Failed to commit message: %s", RdKafka::err2str(code).c_str());
				}
				pChannelConfiguration->SetManagedOffset(p_Msg->offset());
			}
		}
		break;
		default:
			pChannelConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_UNMAPPED_ERROR_CONDITION, code, RdKafka::err2str(code).c_str());
			break;
		}
		if (deleteMsgOnExit) delete p_Msg;
	} while ((p_Msg = pChannelConfiguration->pConsumer->consume(pChannelConfiguration->GetConsumerTimeout())) != NULL && pChannelConfiguration->bConsumerRun);

	return S_OK;
}

DWORD __stdcall DataDistributionManagerKafka::pollHandler(void * argh)
{
	ChannelConfigurationKafka* pChannelConfiguration = static_cast<ChannelConfigurationKafka*>(argh);

	SetEvent(pChannelConfiguration->h_evtPoll);

	while (pChannelConfiguration->bPollRun)
	{
		int eventServed = pChannelConfiguration->pProducer->poll(pChannelConfiguration->GetProducerTimeout());
		if (eventServed != 0) pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, pChannelConfiguration->GetChannelName(), "primaryPollHandler", "served %d events", eventServed);
	}

	return S_OK;
}


