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

#if !defined(DATADISTRIBUTIONMANAGERKAFKA_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERKAFKA_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerCommon.h"

#include <librdkafka\rdkafka.h>
#include <librdkafka\rdkafkacpp.h>

class KafkaMessageManagerEventCb;
class KafkaMessageManagerConsumeCb;
class KafkaMessageManagerRebalanceCb;
class ChannelConfigurationKafka;

class DataDistributionManagerKafka : public DataDistributionCommon
{
public:
	DataDistributionManagerKafka();
	virtual ~DataDistributionManagerKafka();
	OPERATION_RESULT Initialize();
	CHANNEL_HANDLE CreateChannel(const char* channelName, IDataDistributionChannelCallback* dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char* arrayParams[] = NULL, int len = NULL);
	OPERATION_RESULT StartChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds);
	OPERATION_RESULT StopChannel(CHANNEL_HANDLE_PARAMETER, unsigned long dwMilliseconds);
	void SetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName, const char* paramValue);
	const char* GetParameter(CHANNEL_HANDLE_PARAMETER, const char* paramName);
	OPERATION_RESULT Lock(CHANNEL_HANDLE_PARAMETER, unsigned long timeout);
	OPERATION_RESULT Unlock(CHANNEL_HANDLE_PARAMETER);
	OPERATION_RESULT SeekChannel(CHANNEL_HANDLE_PARAMETER, int64_t position, DDM_SEEKCONTEXT context = DDM_SEEKCONTEXT::OFFSET, DDM_SEEKKIND kind = DDM_SEEKKIND::ABSOLUTE);
	OPERATION_RESULT DeleteChannel(CHANNEL_HANDLE_PARAMETER);
	OPERATION_RESULT WriteOnChannel(CHANNEL_HANDLE_PARAMETER, const char* key, size_t keyLen, void *param, size_t dataLen, const BOOL waitAll = FALSE, const int64_t timestamp = DDM_NO_TIMESTAMP);
	OPERATION_RESULT ReadFromChannel(CHANNEL_HANDLE_PARAMETER, int64_t offset, size_t *dataLen, void **param);
	OPERATION_RESULT ChangeChannelDirection(CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction);

	int GetServerLostTimeout() { return m_ServerLostTimeout; };
private:
	int  m_ServerLostTimeout;
	static OPERATION_RESULT KafkaErrorMapper(RdKafka::ErrorCode code);
	int read_config_file(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len);
	OPERATION_RESULT StartConsumerAndWait(ChannelConfigurationKafka* conf, unsigned long dwMilliseconds);
	void StopConsumer(ChannelConfigurationKafka* conf);
	OPERATION_RESULT StartPoll(ChannelConfigurationKafka* conf, unsigned long dwMilliseconds);
	void StopPoll(ChannelConfigurationKafka* conf);
	int conf_init(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len);
	int admin_create_topic(rd_kafka_t *use_rk, const char *channelname, int partition_cnt, int replication_factor, int timeout_ms);

	static void FUNCALL consumerHandler(ThreadWrapperArg *arg);
	static void FUNCALL pollHandler(ThreadWrapperArg *arg);

	std::vector<ChannelConfigurationKafka*> topicVector;
};

class ChannelConfigurationKafka : public ChannelConfiguration
{
public:
	ChannelConfigurationKafka(const char* channelName, DDM_CHANNEL_DIRECTION direction, DataDistributionManagerKafka* mainManager, IDataDistributionChannelCallback* Cb)
		: ChannelConfiguration(channelName, direction, mainManager, Cb)
	{
		pConnection_conf = NULL;
		pTopic_conf = NULL;
		pProducer = NULL;
		pConsumer = NULL;
		pRebalance_cb = NULL;
		pEvent_cb = NULL;
		pConsume_cb = NULL;
		pTopic = NULL;
		pTopicPartition = NULL;
		pTopicPartitionVector = NULL;
		m_ProducerMsgFlags = RdKafka::Producer::RK_MSG_COPY;
		m_TopicReplicationFactor = 1;
		m_CreateTopic = FALSE;
		m_DumpMetadata = FALSE;
		m_bTransactionsEnabled = FALSE;
		m_TransactionsTimeout = 10000;

		m_lastRoutedOffset = RD_KAFKA_OFFSET_END;
		m_actualOffset = RD_KAFKA_OFFSET_END;

		m_tConsumerThread = NULL;
		m_tPollThread = NULL;
	}

	int m_ProducerMsgFlags;
	int  m_TopicReplicationFactor;
	BOOL m_CreateTopic;
	BOOL m_DumpMetadata;
	BOOL m_bTransactionsEnabled;
	int m_TransactionsTimeout;

	std::string clientId;
	std::string groupId;

	RdKafka::Conf *pConnection_conf;
	RdKafka::Conf *pTopic_conf;
	RdKafka::Producer *pProducer;
	RdKafka::KafkaConsumer *pConsumer;
	KafkaMessageManagerRebalanceCb* pRebalance_cb;
	KafkaMessageManagerEventCb* pEvent_cb;
	KafkaMessageManagerConsumeCb* pConsume_cb;
	
	RdKafka::Topic *pTopic;
	RdKafka::TopicPartition *pTopicPartition;
	std::vector<RdKafka::TopicPartition*> *pTopicPartitionVector;

	DataDistributionThreadWrapper* m_tConsumerThread;
	DataDistributionThreadWrapper* m_tPollThread;
};

class KafkaMessageManagerEventCb : public RdKafka::EventCb
{
private:
	ChannelConfigurationKafka* pChannelConfiguration;
public:
	KafkaMessageManagerEventCb(ChannelConfigurationKafka* manager)
	{
		pChannelConfiguration = manager;
	}

	void event_cb(RdKafka::Event &event)
	{
		switch (event.type())
		{
		case RdKafka::Event::EVENT_ERROR:
		{
			RdKafka::ErrorCode code = event.err();
			switch (code)
			{
			case RdKafka::ERR__ALL_BROKERS_DOWN:
				if (!pChannelConfiguration->IsStartupStatusSet())
				{
					pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "KafkaMessageManagerEventCb", "ERROR: %s %s", RdKafka::err2str(code).c_str(), event.str().c_str());
					pChannelConfiguration->SetStartupStatus(CHANNEL_STARTUP_TYPE::DISCONNECTED);
				}
				else
				{
					pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "KafkaMessageManagerEventCb", "ERROR: %s %s", RdKafka::err2str(code).c_str(), event.str().c_str());
					pChannelConfiguration->CompletelyDisconnected();
				}
				break;
			default:
				pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerEventCb", "ERROR: %s %s", RdKafka::err2str(code).c_str(), event.str().c_str());
				break;
			}
		}
		break;
		case RdKafka::Event::EVENT_STATS:
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerEventCb", "STATS: %s", event.str().c_str());
			break;
		case RdKafka::Event::EVENT_LOG:
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerEventCb", "LOG-%i-%s: %s", event.severity(), event.fac().c_str(), event.str().c_str());
			break;
		case RdKafka::Event::EVENT_THROTTLE:
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerEventCb", "THROTTLE: %s %s", RdKafka::err2str(event.err()).c_str(), event.str().c_str());
			break;
		default:
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerEventCb", "EVENT: %s %s", RdKafka::err2str(event.err()).c_str(), event.str().c_str());
			break;
		}
	}
};

class KafkaMessageManagerRebalanceCb : public RdKafka::RebalanceCb
{
private:
	ChannelConfigurationKafka* pChannelConfiguration;
public:
	KafkaMessageManagerRebalanceCb(ChannelConfigurationKafka* manager)
	{
		pChannelConfiguration = manager;
	}
	void rebalance_cb(RdKafka::KafkaConsumer *consumer,
		RdKafka::ErrorCode err,
		std::vector<RdKafka::TopicPartition*> &partitions)
	{
		switch (err)
		{
		case RdKafka::ERR__ASSIGN_PARTITIONS:
		{
			size_t partition_cnt = partitions.size();
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "partitions: %d - %s", partition_cnt, RdKafka::err2str(err).c_str());
			for (size_t i = 0; i < partition_cnt; i++)
			{
				RdKafka::TopicPartition* pTopicPart = partitions.at(i);
				int64_t nextOffset = pChannelConfiguration->GetActualOffset();
				if (nextOffset >= 0)
				{
					pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "move forward partition offset: %" PRId64 "", pTopicPart->offset());
					nextOffset += 1; // move forward the index
				}
				pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "assigned channel %s - partition: %d - offset: %" PRId64 "", pTopicPart->topic().c_str(), pTopicPart->partition(), pTopicPart->offset());
				if (pTopicPart->offset() == RD_KAFKA_OFFSET_INVALID)
				{
					pTopicPart->set_offset(nextOffset);
					int partition = pTopicPart->partition();
					int64_t offset = pTopicPart->offset();
					pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "channel %s - partition: %d - changed offset: %" PRId64 "", pTopicPart->topic().c_str(), pTopicPart->partition(), pTopicPart->offset());
				}
			}

			RdKafka::ErrorCode errorCode = consumer->assign(partitions);
			if (errorCode != RdKafka::ERR_NO_ERROR)
			{
				pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "consumer->assign error: %s", RdKafka::err2str(errorCode).c_str());
			}

			if (!pChannelConfiguration->IsStartupStatusSet())
			{
				pChannelConfiguration->SetStartupStatus(CHANNEL_STARTUP_TYPE::STARTED);
			}
		}
		break;
		case RdKafka::ERR__REVOKE_PARTITIONS:
		{
			pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerRebalanceCb", "%s", RdKafka::err2str(err).c_str());
			RdKafka::ErrorCode errorCode = consumer->unassign();
			if (errorCode != RdKafka::ERR_NO_ERROR)
			{
				pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "KafkaMessageManagerRebalanceCb", "consumer->unassign error: %s", RdKafka::err2str(errorCode).c_str());
			}
		}
		break;
		default:
			pChannelConfiguration->Log(DDM_LOG_LEVEL::ERROR_LEVEL, "KafkaMessageManagerRebalanceCb", "Error: %s", RdKafka::err2str(err).c_str());
			break;
		}
	}
};

class KafkaMessageManagerConsumeCb : public RdKafka::ConsumeCb
{
private:
	ChannelConfigurationKafka* pChannelConfiguration;
public:
	KafkaMessageManagerConsumeCb(ChannelConfigurationKafka* manager)
	{
		pChannelConfiguration = manager;
	}

	void consume_cb(RdKafka::Message &msg, void *opaque)
	{
		RdKafka::ErrorCode err = msg.err();
		pChannelConfiguration->Log(DDM_LOG_LEVEL::INFO_LEVEL, "KafkaMessageManagerConsumeCb", "%s", RdKafka::err2str(err).c_str());

		switch (msg.err())
		{
		case RdKafka::ERR__TIMED_OUT:
			break;

		case RdKafka::ERR_NO_ERROR:
			/* Real message */
			break;

		case RdKafka::ERR__PARTITION_EOF:
			/* Last message */

			break;

		case RdKafka::ERR__UNKNOWN_TOPIC:
		case RdKafka::ERR__UNKNOWN_PARTITION:

			break;

		default:
			/* Errors */
			break;
		}
	}
};

#endif // !defined(DATADISTRIBUTIONMANAGERKAFKA_H__INCLUDED_)