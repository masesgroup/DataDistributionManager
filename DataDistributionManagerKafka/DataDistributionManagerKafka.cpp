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
	// init
	SetMaxMessageSize(1024 * 1024);
}

DataDistributionManagerKafka::~DataDistributionManagerKafka()
{
}

int DataDistributionManagerKafka::read_config_file(pChannelConfigurationKafka configuration, const char* arrayParams[], int len)
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
			configuration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "read_config_file", "Conf file: malformed line: %s", line.c_str());
			return ERROR_INVALID_DATA;
		}

		std::string n = line.substr(0, f);
		std::string v = line.substr(f + 1);

		v = CheckConfigurationParameter(n, v);

		SetParameter(configuration, n.c_str(), v.c_str());
	}

	return NO_ERROR;
}

int DataDistributionManagerKafka::conf_init(pChannelConfigurationKafka configuration, const char* arrayParams[], int len)
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
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, "admin_create_channel", "Timeout on create Topic %s", channelname);
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
			Log(DDM_LOG_LEVEL::INFO_LEVEL, "admin_create_channel", "%s", errorStr);
			retVal = NO_ERROR;
		}
		else if (res != RD_KAFKA_RESP_ERR_NO_ERROR)
		{
			const char* errorStr = rd_kafka_topic_result_error_string(terr[0]);
			Log(DDM_LOG_LEVEL::ERROR_LEVEL, "admin_create_channel", "%s", errorStr);
			retVal = E_FAIL;
		}
		else
		{
			Log(DDM_LOG_LEVEL::ERROR_LEVEL, "admin_create_topic", "Topic %s created", rd_kafka_topic_result_name(terr[0]));
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

HRESULT DataDistributionManagerKafka::Lock(HANDLE channelHandle, DWORD timeout)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Lock", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return pTopicConfiguration->SetLockState();
}

HRESULT DataDistributionManagerKafka::Unlock(HANDLE channelHandle)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Unlock", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return pTopicConfiguration->ResetLockState();
}

HANDLE DataDistributionManagerKafka::CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction, const char* arrayParams[], int len)
{
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, channelName, "CreateTopic", "SubSystem not started.");
		return NULL;
	}

	std::string sTopicName = channelName;
#if !TEST_CONSOLE
	sTopicName = sTopicName.append(GetChannelTrailer());
#endif

	pChannelConfigurationKafka pTopicConfiguration = new ChannelConfigurationKafka(sTopicName.c_str(), direction, this, dataCb);

	std::string errstr;

	int retVal = conf_init(pTopicConfiguration, (arrayParams == NULL) ? GetArrayParams() : arrayParams, (len == 0) ? GetArrayParamsLen() : len);
	if (retVal != NO_ERROR)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "set conf_init error: %d", retVal);
		return NULL;
	}

	RdKafka::Conf::ConfResult result = pTopicConfiguration->pConnection_conf->set("default_topic_conf", pTopicConfiguration->pTopic_conf, errstr);
	if (result != RdKafka::Conf::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "set default_topic_conf error: %s", errstr.c_str());
		// TODO: log
		return NULL;
	}
	pTopicConfiguration->pRebalance_cb = new KafkaMessageManagerRebalanceCb(pTopicConfiguration);
	if (pTopicConfiguration->pConnection_conf->set("rebalance_cb", pTopicConfiguration->pRebalance_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "set rebalance_cb error: %s", errstr.c_str());
		return NULL;
	}
	pTopicConfiguration->pEvent_cb = new KafkaMessageManagerEventCb(pTopicConfiguration);
	if (pTopicConfiguration->pConnection_conf->set("event_cb", pTopicConfiguration->pEvent_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "set event_cb error: %s", errstr.c_str());
		return NULL;
	}
	pTopicConfiguration->pConsume_cb = new KafkaMessageManagerConsumeCb(pTopicConfiguration);
	if (pTopicConfiguration->pConnection_conf->set("consume_cb", pTopicConfiguration->pConsume_cb, errstr) != RdKafka::Conf::ConfResult::CONF_OK)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "set consume_cb error: %s", errstr.c_str());
		return NULL;
	}

	pTopicConfiguration->pProducer = RdKafka::Producer::create(pTopicConfiguration->pConnection_conf, errstr);
	if (!pTopicConfiguration->pProducer)
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "pProducer RdKafka::Producer::create error: %s", errstr.c_str());
		return NULL;
	}

	pTopicConfiguration->pConsumer = RdKafka::KafkaConsumer::create(pTopicConfiguration->pConnection_conf, errstr);
	if (!pTopicConfiguration->pConsumer)
	{
		delete pTopicConfiguration->pProducer;
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "pConsumer RdKafka::KafkaConsumer::create error: %s", errstr.c_str());
		return NULL;
	}

	if (pTopicConfiguration->m_CreateTopic && admin_create_topic(pTopicConfiguration->pProducer->c_ptr(), pTopicConfiguration->GetChannelName(), 1, pTopicConfiguration->m_TopicReplicationFactor, pTopicConfiguration->m_CreateChannelTimeout) != NO_ERROR)
	{
		delete pTopicConfiguration->pConsumer;
		delete pTopicConfiguration->pProducer;
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "CreateTopic", "admin_create_topic %s failed", pTopicConfiguration->GetChannelName());
		return NULL;
	}

	pTopicConfiguration->pTopicPartition = RdKafka::TopicPartition::create(sTopicName, 0);
	pTopicConfiguration->pTopicPartitionVector = new std::vector<RdKafka::TopicPartition*>();
	pTopicConfiguration->pTopicPartitionVector->push_back(pTopicConfiguration->pTopicPartition);

	pTopicConfiguration->pTopic = RdKafka::Topic::create(pTopicConfiguration->pProducer, sTopicName, pTopicConfiguration->pTopic_conf, errstr);
	if (!pTopicConfiguration->pTopic)
	{
		delete pTopicConfiguration->pConsumer;
		delete pTopicConfiguration->pProducer;
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Initialize", "pPrimaryTopic RdKafka::Topic::create error: %s", errstr.c_str());
		return NULL;
	}

	if (pTopicConfiguration->m_DumpMetadata)
	{
		class RdKafka::Metadata *metadata;
		auto err = pTopicConfiguration->pProducer->metadata(false, pTopicConfiguration->pTopic, &metadata, pTopicConfiguration->m_CreateChannelTimeout);

		if (err != RdKafka::ERR_NO_ERROR)
		{
			Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Initialize", "Failed to acquire metadata for topic %s: %s", pTopicConfiguration->GetChannelName(), RdKafka::err2str(err).c_str());
		}
		else
		{
			auto resultStr = metadata_print(sTopicName, metadata);
			Log(DDM_LOG_LEVEL::DEBUG_LEVEL, pTopicConfiguration->GetChannelName(), "Initialize", "%s", resultStr.c_str());
		}
	}

	topicVector.push_back(pTopicConfiguration);

	return pTopicConfiguration;
}

HRESULT DataDistributionManagerKafka::StartChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);

	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Start", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	HRESULT status = StartConsumerAndWait(pTopicConfiguration, dwMilliseconds);
	if (FAILED(status)) return status;
	status = StartPoll(pTopicConfiguration, dwMilliseconds);

	return status;
}

HRESULT DataDistributionManagerKafka::StopChannel(HANDLE channelHandle, DWORD dwMilliseconds)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "Stop", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	StopConsumer(pTopicConfiguration);
	StopPoll(pTopicConfiguration);
	return S_OK;
}

void DataDistributionManagerKafka::SetParameter(HANDLE channelHandle, const char* paramName, const char* paramValue)
{
	Log(DDM_LOG_LEVEL::INFO_LEVEL, "DataDistributionManagerKafka", "SetParameter", "Name: %s - Value: %s", (paramName != NULL) ? paramName : "", (paramValue != NULL) ? paramValue : "");
	std::string errstr;
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);

	if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
	{
		SetMaxMessageSize(_atoi64(paramValue));
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
	{
		pTopicConfiguration->m_ServerLostTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.createchannel"))
	{
		pTopicConfiguration->m_CreateChannelTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
	{
		pTopicConfiguration->m_TopicSeekTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.receive"))
	{
		pTopicConfiguration->m_MessageReceiveTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
	{
		pTopicConfiguration->m_KeepAliveTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
	{
		pTopicConfiguration->m_ConsumerTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
	{
		pTopicConfiguration->m_ProducerTimeout = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
	{
		if (!strcmp(paramValue, "true") ||
			!strcmp(paramValue, "1"))
			pTopicConfiguration->m_CommitSync = TRUE;
		else
			pTopicConfiguration->m_CommitSync = FALSE;
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.producer.msgflags"))
	{
		pTopicConfiguration->m_ProducerMsgFlags = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.replicationfactor"))
	{
		pTopicConfiguration->m_TopicReplicationFactor = atoi(paramValue);
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.create"))
	{
		if (!strcmp(paramValue, "true") ||
			!strcmp(paramValue, "1"))
			pTopicConfiguration->m_CreateTopic = TRUE;
		else
			pTopicConfiguration->m_CreateTopic = FALSE;
		return;
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.dumpmetadata"))
	{
		if (!strcmp(paramValue, "true") ||
			!strcmp(paramValue, "1"))
			pTopicConfiguration->m_DumpMetadata = TRUE;
		else
			pTopicConfiguration->m_DumpMetadata = FALSE;
		return;
	}

	std::string n(paramName);
	std::string v(paramValue);

	RdKafka::Conf::ConfResult r = RdKafka::Conf::CONF_UNKNOWN;
	std::string sTopicName = pTopicConfiguration->GetChannelName();
	std::string strToCheck = "datadistributionmanager.kafka.topic.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pTopicConfiguration->pTopic_conf->set(n.substr(strToCheck.length()), v, errstr);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka.";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pTopicConfiguration->pConnection_conf->set(n.substr(strToCheck.length()), v, errstr);
		}
	}

	strToCheck = "datadistributionmanager.kafka." + sTopicName + ".topic.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pTopicConfiguration->pTopic_conf->set(n.substr(strToCheck.length()), v, errstr);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka." + sTopicName + ".";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pTopicConfiguration->pConnection_conf->set(n.substr(strToCheck.length()), v, errstr);
		}
	}

	if (r != RdKafka::Conf::CONF_OK) {
		pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "read_config_file", "Error set configuration: %s", errstr.c_str());
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

const char* DataDistributionManagerKafka::GetParameter(HANDLE channelHandle, const char* paramName)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);

	if (!strcmp(paramName, "datadistributionmanager.maxmessagesize"))
	{
		return ConvertIToA(GetMaxMessageSize());
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.serverlost"))
	{
		return ConvertIToA(pTopicConfiguration->m_ServerLostTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.createchannel"))
	{
		return ConvertIToA(pTopicConfiguration->m_CreateChannelTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.channelseek"))
	{
		return ConvertIToA(pTopicConfiguration->m_TopicSeekTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.firstconnection"))
	{
		return ConvertIToA(pTopicConfiguration->m_MessageReceiveTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.keepalive"))
	{
		return ConvertIToA(pTopicConfiguration->m_KeepAliveTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.consumer"))
	{
		return ConvertIToA(pTopicConfiguration->m_ConsumerTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.timeout.producer"))
	{
		return ConvertIToA(pTopicConfiguration->m_ProducerTimeout);
	}
	else if (!strcmp(paramName, "datadistributionmanager.commit.sync"))
	{
		if (pTopicConfiguration->m_CommitSync) return "true";
		else return "false";
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.producer.msgflags"))
	{
		return ConvertIToA(pTopicConfiguration->m_ProducerMsgFlags);
	}
	else if (!strcmp(paramName, "datadistributionmanager.kafka.topic.replicationfactor"))
	{
		return ConvertIToA(pTopicConfiguration->m_TopicReplicationFactor);
	}

	std::string n(paramName);
	std::string v;

	RdKafka::Conf::ConfResult r = RdKafka::Conf::CONF_UNKNOWN;
	std::string sTopicName = pTopicConfiguration->GetChannelName();
	std::string strToCheck = "datadistributionmanager.kafka.channel.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pTopicConfiguration->pTopic_conf->get(n.substr(strToCheck.length()), v);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka.";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pTopicConfiguration->pConnection_conf->get(n.substr(strToCheck.length()), v);
		}
	}

	strToCheck = "datadistributionmanager.kafka." + sTopicName + ".channel.";
	if (n.substr(0, strToCheck.length()) == strToCheck)
	{
		r = pTopicConfiguration->pTopic_conf->get(n.substr(strToCheck.length()), v);
	}
	if (r == RdKafka::Conf::CONF_UNKNOWN)
	{
		strToCheck = "datadistributionmanager.kafka." + sTopicName + ".";
		if (n.substr(0, strToCheck.length()) == strToCheck)
		{
			r = pTopicConfiguration->pConnection_conf->get(n.substr(strToCheck.length()), v);
		}
	}

	if (r != RdKafka::Conf::CONF_OK) {
		pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "GetParameter", "Error get configuration");
	}
	else return _strdup(v.c_str());

	return NULL;
}

HRESULT DataDistributionManagerKafka::SeekChannel(HANDLE channelHandle, size_t position)
{
	RdKafka::ErrorCode code;
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "SeekTopic", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	std::vector<RdKafka::TopicPartition*> partVector;
	std::string sTopicName = pTopicConfiguration->GetChannelName();
	partVector.push_back(RdKafka::TopicPartition::create(sTopicName, 0, position));
	code = pTopicConfiguration->pConsumer->offsetsForTimes(partVector, pTopicConfiguration->m_CreateChannelTimeout);
	for (unsigned int i = 0; i < partVector.size(); i++)
	{
#ifdef _WIN64
		pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "SeekTopic", "pTopicConfiguration->pConsumer->offsetsForTimes [channel: %s offset:" PRId64 "]: error: %s", partVector[i]->topic().c_str(), partVector[i]->offset(), RdKafka::err2str(partVector[i]->err()).c_str());
#else
		pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "SeekTopic", "pTopicConfiguration->pConsumer->offsetsForTimes [channel: %s offset: %ld ]: error: %s", partVector[i]->topic().c_str(), partVector[i]->offset(), RdKafka::err2str(partVector[i]->err()).c_str());
#endif
	}
	if (code != RdKafka::ERR_NO_ERROR)
	{
		pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "SeekTopic", "pTopicConfiguration->pConsumer->offsetsForTimes error: %s", RdKafka::err2str(code).c_str());
		pTopicConfiguration->bConsumerRun = FALSE;
		SetEvent(pTopicConfiguration->h_evtConsumer);
		return E_FAIL;
	}
	else
	{
#ifdef _WIN64
		pTopicConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "SeekTopic", "pTopicConfiguration->pConsumer->offsetsForTimes set offset to: " PRId64 "", partVector[0]->offset());
#else
		pTopicConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "SeekTopic", "pTopicConfiguration->pConsumer->offsetsForTimes set offset to: %ld", partVector[0]->offset());
#endif
		pTopicConfiguration->SetManagedOffset(partVector[0]->offset());
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::DeleteChannel(HANDLE channelHandle)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "DeleteTopic", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::WriteOnChannel(HANDLE channelHandle, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll, const int64_t timestamp)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "WriteOnTopic", "SubSystem not started.");
		pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_SUBSYSTEM_NOT_STARTED, 0, "SubSystem not started.");
		return E_FAIL;
	}

	RdKafka::ErrorCode code;
	if (timestamp != -1)
	{
		std::string sTopicName = pTopicConfiguration->GetChannelName();
		code = pTopicConfiguration->pProducer->produce(sTopicName, 0, pTopicConfiguration->m_ProducerMsgFlags,
			param, dataLen, key, keyLen, timestamp, (void*)this);
	}
	else
	{
		code = pTopicConfiguration->pProducer->produce(pTopicConfiguration->pTopic, 0, pTopicConfiguration->m_ProducerMsgFlags,
			param, dataLen, key, keyLen, (void*)this);
	}

	if (code != RdKafka::ErrorCode::ERR_NO_ERROR)
	{
		DDM_UNDERLYING_ERROR_CONDITION thisCode = KafkaErrorMapper(code);

		pTopicConfiguration->OnConditionOrError(thisCode, code, RdKafka::err2str(code).c_str());
		return E_FAIL;
	}
	return S_OK;
}

HRESULT DataDistributionManagerKafka::ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *dataLen, void **param)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	if (!GetSubSystemStarted())
	{
		Log(DDM_LOG_LEVEL::ERROR_LEVEL, pTopicConfiguration->GetChannelName(), "ReadFromTopic", "SubSystem not started.");
		return FALSE;
	}

	return S_OK;
}

HRESULT DataDistributionManagerKafka::ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(channelHandle);
	DDM_CHANNEL_DIRECTION oldDirection = pTopicConfiguration->GetDirection();

	if (GetSubSystemStarted())
	{
		// update internal state following the direction


	}
	pTopicConfiguration->SetDirection(direction);
	return S_OK;
}

HRESULT DataDistributionManagerKafka::StartConsumerAndWait(pChannelConfigurationKafka pTopicConfiguration, DWORD dwMilliseconds)
{
	HRESULT result = S_OK;
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartConsumerAndWait", "Enter");
	pTopicConfiguration->bConsumerRun = TRUE;
	pTopicConfiguration->hConsumerThread = CreateThread(0, 0, consumerHandler, pTopicConfiguration, 0, &pTopicConfiguration->dwConsumerThrId);
	auto res = WaitForSingleObject(pTopicConfiguration->h_evtConsumer, dwMilliseconds);
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
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartConsumerAndWait", "Exit");
	return result;
}

void DataDistributionManagerKafka::StopConsumer(pChannelConfigurationKafka pTopicConfiguration)
{
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopConsumer", "Enter");
	pTopicConfiguration->bConsumerRun = FALSE;
	CloseHandle(pTopicConfiguration->hConsumerThread);
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopConsumer", "Exit");
}

HRESULT DataDistributionManagerKafka::StartPoll(pChannelConfigurationKafka pTopicConfiguration, DWORD dwMilliseconds)
{
	HRESULT result = S_OK;
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartPoll", "Enter");
	pTopicConfiguration->bPollRun = TRUE;
	pTopicConfiguration->hPollThread = CreateThread(0, 0, pollHandler, pTopicConfiguration, 0, &pTopicConfiguration->dwPollThrId);
	auto res = WaitForSingleObject(pTopicConfiguration->h_evtPoll, dwMilliseconds);
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
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StartPoll", "Exit");
	return result;
}

void DataDistributionManagerKafka::StopPoll(pChannelConfigurationKafka pTopicConfiguration)
{
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopPoll", "Enter");
	pTopicConfiguration->bPollRun = FALSE;
	CloseHandle(pTopicConfiguration->hPollThread);
	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "StopPoll", "Exit");
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
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(argh);

	pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "consumerHandler", "Entering ");

	SmartTimeMeasureWrapper timeStart;

	timeStart.ResetTime();

	pTopicConfiguration->bConsumerRun = TRUE;

	RdKafka::ErrorCode code;
	RdKafka::Message *p_Msg;

	std::vector<std::string> vector;
	std::string sTopicName = pTopicConfiguration->GetChannelName();
	vector.push_back(sTopicName);
	code = pTopicConfiguration->pConsumer->subscribe(vector);
	if (code != RdKafka::ERR_NO_ERROR)
	{
		pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pTopicConfiguration->pConsumer->subscribe error: %s", RdKafka::err2str(code).c_str());
		pTopicConfiguration->bConsumerRun = FALSE;
		SetEvent(pTopicConfiguration->h_evtConsumer);
		return -1;
	}

	BOOL result = FALSE;
	do
	{
		p_Msg = pTopicConfiguration->pConsumer->consume(10); // waiting first message to know if we are connected
		code = p_Msg->err();
		if (code == RdKafka::ErrorCode::ERR_NO_ERROR) pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "primaryConsumerHandler", "Received Message in the loop");
		result = code == RdKafka::ErrorCode::ERR__TIMED_OUT && !pTopicConfiguration->GetStartupStatusSet();
		if (result) delete p_Msg;
	} while (result);

	pTopicConfiguration->WaitStartupStatus(INFINITE);

	if (pTopicConfiguration->GetStartupStatus() != CHANNEL_STARTUP_TYPE::STARTED)
	{
		pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pKafkaMessageManager->GetStartupStatus() is: %d", pTopicConfiguration->GetStartupStatus());
		pTopicConfiguration->bConsumerRun = FALSE;
		SetEvent(pTopicConfiguration->h_evtConsumer);
		return -1;
	}

	BOOL timeoutEmitted = FALSE;
	SetEvent(pTopicConfiguration->h_evtConsumer);
	timeStart.ResetTime();
	do
	{
		pTopicConfiguration->WaitingFinishLockState(INFINITE);

		BOOL deleteMsgOnExit = TRUE;
		code = p_Msg->err();
		switch (code)
		{
		case RdKafka::ErrorCode::ERR__PARTITION_EOF:
		case RdKafka::ErrorCode::ERR__TIMED_OUT:
		{
			auto duration = timeStart.ElapsedMilliseconds();

			if (!timeoutEmitted && duration > pTopicConfiguration->m_MessageReceiveTimeout) // no message within m_MessageReceiveTimeout
			{
				pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN, 0, "Elapsed timeout receiving packets.");
				timeoutEmitted = TRUE;
			}
		}
#ifdef TRIAL_VERSION_DEPLOY
		continue;
#else
		break;
#endif
		case RdKafka::ErrorCode::ERR_NO_ERROR:
		{
			if (timeoutEmitted)
			{
				pTopicConfiguration->OnConditionOrError(DDM_UNDERLYING_ERROR_CONDITION::DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END, 0, "End timeout receiving packets.");
			}
			timeoutEmitted = FALSE;
			timeStart.ResetTime();

			pTopicConfiguration->OnDataAvailable(p_Msg->key()->c_str(), p_Msg->key_len(), p_Msg->payload(), p_Msg->len());

			if (deleteMsgOnExit)
			{
				if (pTopicConfiguration->m_CommitSync)
				{
					code = pTopicConfiguration->pConsumer->commitSync(p_Msg);
				}
				else
				{
					code = pTopicConfiguration->pConsumer->commitAsync(p_Msg);
				}
				if (code != RdKafka::ERR_NO_ERROR)
				{
					pTopicConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "consumerHandler", "pTopicConfiguration->pConsumer->commitSync/commitAsync error: %s", RdKafka::err2str(code).c_str());
				}
				pTopicConfiguration->SetManagedOffset(p_Msg->offset());
			}
		}
#ifdef TRIAL_VERSION_DEPLOY
		continue;
#else
		break;
#endif
		default:
			DDM_UNDERLYING_ERROR_CONDITION errCondCode = KafkaErrorMapper(code);
			pTopicConfiguration->OnConditionOrError(errCondCode, code, RdKafka::err2str(code).c_str());
			break;
		}
		if (deleteMsgOnExit) delete p_Msg;
	} while ((p_Msg = pTopicConfiguration->pConsumer->consume(pTopicConfiguration->m_ConsumerTimeout)) != NULL && pTopicConfiguration->bConsumerRun);

	return S_OK;
}

DWORD __stdcall DataDistributionManagerKafka::pollHandler(void * argh)
{
	pChannelConfigurationKafka pTopicConfiguration = static_cast<ChannelConfigurationKafka*>(argh);

	SetEvent(pTopicConfiguration->h_evtPoll);

	while (pTopicConfiguration->bPollRun)
	{
		int eventServed = pTopicConfiguration->pProducer->poll(pTopicConfiguration->m_ProducerTimeout);
		if (eventServed != 0) pTopicConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, pTopicConfiguration->GetChannelName(), "primaryPollHandler", "served %d events", eventServed);
	}

	return S_OK;
}


