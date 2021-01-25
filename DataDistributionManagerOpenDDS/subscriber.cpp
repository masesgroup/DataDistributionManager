// -*- C++ -*-
// *******************************************************************
//
// (c) Copyright 2006, Object Computing, Inc.
// All Rights Reserved.
//
// *******************************************************************

#include "DataDistributionSchemaTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/BuiltInTopicUtils.h>
#include <ace/streams.h>
#include "ace/OS_NS_unistd.h"
#include <orbsvcs/Time_Utilities.h>

#include "dds/DCPS/StaticIncludes.h"

#include "DataDistributionManagerOpenDDS.h"

// constants for Stock Quoter domain Id, types, and topic
// (same as publisher)
DDS::DomainId_t QUOTER_DOMAIN_ID = 1066;


int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) {

	DataDistributionManagerOpenDDS* manager = new DataDistributionManagerOpenDDS();

	manager->Initialize();


	while (true) {
		ACE_OS::sleep(1);
	}

	return 0;
}
