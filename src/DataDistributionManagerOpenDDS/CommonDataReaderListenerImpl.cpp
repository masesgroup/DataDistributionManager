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
#include <ace/streams.h>

CommonDataReaderListenerImpl::CommonDataReaderListenerImpl(ChannelConfiguration* pChannelConfiguration)
{
	m_pChannelConfiguration = pChannelConfiguration;
}

CommonDataReaderListenerImpl::~CommonDataReaderListenerImpl()
{
}

void CommonDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
	try
	{
		DataDistributionSchema::OpenDDSMsgDataReader_var quote_dr
			= DataDistributionSchema::OpenDDSMsgDataReader::_narrow(reader);
		if (CORBA::is_nil(quote_dr.in()))
		{
			m_pChannelConfiguration->OnConditionOrError(DDM_FATAL_ERROR, 0, "CommonDataReaderListenerImpl::on_data_available: _narrow failed.");
			return;
		}

		int count = 0;
		while (true)
		{
			DataDistributionSchema::OpenDDSMsg quote;
			DDS::SampleInfo si;
			DDS::ReturnCode_t status = quote_dr->take_next_sample(quote, si);

			if (status == DDS::RETCODE_OK && si.valid_data)
			{
				++count;
				const char* key = quote.key;
				size_t keyLen = (key != NULL) ? strlen(key) : 0;
				m_pChannelConfiguration->OnDataAvailable(quote.key, keyLen, quote.buffer.get_buffer(), quote.msgSize);
			}
			else if (status == DDS::RETCODE_NO_DATA)
			{
				m_pChannelConfiguration->OnConditionOrError(DDM_NO_DATA_RETURNED, status, "status is OK");
				break;
			}
			else
			{
				m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, status, "See native code error.");
			}
		}

	}
	catch (CORBA::Exception& e)
	{
		m_pChannelConfiguration->OnConditionOrError(DDM_FATAL_ERROR, 0, e._info().c_str());
		return;
	}
}

void CommonDataReaderListenerImpl::on_requested_deadline_missed(
	DDS::DataReader_ptr,
	const DDS::RequestedDeadlineMissedStatus & status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_requested_deadline_missed");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_requested_deadline_missed", "total_count: %d. total_count_change: %d", status.total_count, status.total_count_change);
}

void CommonDataReaderListenerImpl::on_requested_incompatible_qos(
	DDS::DataReader_ptr,
	const DDS::RequestedIncompatibleQosStatus & status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_requested_incompatible_qos");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_requested_incompatible_qos", "total_count: %d. total_count_change: %d", status.total_count, status.total_count_change);
}

void CommonDataReaderListenerImpl::on_liveliness_changed(
	DDS::DataReader_ptr,
	const DDS::LivelinessChangedStatus & status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_liveliness_changed");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_liveliness_changed", "alive_count: %d. alive_count_change: %d", status.alive_count, status.alive_count_change);
}

void CommonDataReaderListenerImpl::on_subscription_matched(
	DDS::DataReader_ptr,
	const DDS::SubscriptionMatchedStatus & status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_subscription_matched");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_subscription_matched", "current_count: %d. current_count_change: %d", status.current_count, status.current_count_change);
}

void CommonDataReaderListenerImpl::on_sample_rejected(
	DDS::DataReader_ptr,
	const DDS::SampleRejectedStatus& status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_sample_rejected");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_sample_rejected", "total_count: %d. total_count_change: %d last_reason: %d", status.total_count, status.total_count_change, status.last_reason);
}

void CommonDataReaderListenerImpl::on_sample_lost(
	DDS::DataReader_ptr,
	const DDS::SampleLostStatus& status)
{
	m_pChannelConfiguration->OnConditionOrError(DDM_UNMAPPED_ERROR_CONDITION, 0, "on_sample_lost");
	m_pChannelConfiguration->Log(DDM_LOG_LEVEL::DEBUG_LEVEL, "CommonDataReaderListenerImpl::on_sample_lost", "total_count: %d. total_count_change: %d", status.total_count, status.total_count_change);
}
