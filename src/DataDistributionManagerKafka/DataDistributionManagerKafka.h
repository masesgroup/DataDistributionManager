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

	int GetServerLostTimeout() { return m_ServerLostTimeout; };
private:
	int  m_ServerLostTimeout;
	static DDM_UNDERLYING_ERROR_CONDITION KafkaErrorMapper(RdKafka::ErrorCode code);
	int read_config_file(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len);
	HRESULT StartConsumerAndWait(ChannelConfigurationKafka* conf, DWORD dwMilliseconds);
	void StopConsumer(ChannelConfigurationKafka* conf);
	HRESULT StartPoll(ChannelConfigurationKafka* conf, DWORD dwMilliseconds);
	void StopPoll(ChannelConfigurationKafka* conf);
	int conf_init(ChannelConfigurationKafka* configuration, const char* arrayParams[], int len);
	int admin_create_topic(rd_kafka_t *use_rk, const char *channelname, int partition_cnt, int replication_factor, int timeout_ms);

	static DWORD __stdcall consumerHandler(void * argh);
	static DWORD __stdcall pollHandler(void * argh);

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
		m_ProducerMsgFlags = 0;
		m_TopicReplicationFactor = 1;
		m_CreateTopic = FALSE;
		m_DumpMetadata = FALSE;

		m_lastRoutedOffset = RD_KAFKA_OFFSET_END;
		m_lastManagedOffset = RD_KAFKA_OFFSET_END;

		h_evtPoll = CreateEvent(0, true, false, NULL);
		h_evtConsumer = CreateEvent(0, true, false, NULL);
	}

	int m_ProducerMsgFlags;
	int  m_TopicReplicationFactor;
	BOOL m_CreateTopic;
	BOOL m_DumpMetadata;

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

	HANDLE  h_evtConsumer;
	BOOL	bConsumerRun;
	DWORD	dwConsumerThrId;
	HANDLE	hConsumerThread;

	HANDLE  h_evtPoll;
	BOOL	bPollRun;
	DWORD	dwPollThrId;
	HANDLE	hPollThread;
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
				int64_t nextOffset = pChannelConfiguration->GetManagedOffset();
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