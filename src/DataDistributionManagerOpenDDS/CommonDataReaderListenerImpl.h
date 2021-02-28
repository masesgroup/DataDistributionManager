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

#ifndef COMMON_DATAREADER_LISTENER_IMPL
#define COMMON_DATAREADER_LISTENER_IMPL

#include "DataDistributionSchemaTypeSupportImpl.h"
#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ChannelConfiguration;

class CommonDataReaderListenerImpl
	: public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
	CommonDataReaderListenerImpl(ChannelConfiguration*);

	virtual ~CommonDataReaderListenerImpl(void);

	virtual void on_requested_deadline_missed(
		DDS::DataReader_ptr reader,
		const DDS::RequestedDeadlineMissedStatus & status);

	virtual void on_requested_incompatible_qos(
		DDS::DataReader_ptr reader,
		const DDS::RequestedIncompatibleQosStatus & status);

	virtual void on_liveliness_changed(
		DDS::DataReader_ptr reader,
		const DDS::LivelinessChangedStatus & status);

	virtual void on_subscription_matched(
		DDS::DataReader_ptr reader,
		const DDS::SubscriptionMatchedStatus & status);

	virtual void on_sample_rejected(
		DDS::DataReader_ptr reader,
		const DDS::SampleRejectedStatus& status);

	virtual void on_data_available(
		DDS::DataReader_ptr reader);

	virtual void on_sample_lost(
		DDS::DataReader_ptr reader,
		const DDS::SampleLostStatus& status);

private:
	ChannelConfiguration* m_pChannelConfiguration;
};

#endif /* COMMON_DATAREADER_LISTENER_IMPL  */
