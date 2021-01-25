// -*- C++ -*-
// *******************************************************************
//
// (c) Copyright 2006, Object Computing, Inc.
// All Rights Reserved.
//
// *******************************************************************


#include "DataDistributionManagerOpenDDS.h"

// constants for Stock Quoter domain Id, types, and topic
DDS::DomainId_t QUOTER_DOMAIN_ID = 1066;

int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) {

	DataDistributionManagerOpenDDS* manager = new DataDistributionManagerOpenDDS();

	manager->Initialize();

	return 0;
}
