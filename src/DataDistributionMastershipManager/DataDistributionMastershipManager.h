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

#if !defined(DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_)
#define DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionMastershipCommon.h"

class DataDistributionMastershipManager : public DataDistributionMastershipCommon
{
public:
	DataDistributionMastershipManager();
	virtual ~DataDistributionMastershipManager();
	int64_t* GetClusterIndexes(size_t* length);
	DDM_INSTANCE_STATE GetStateOf(int64_t serverId);
	ClusterHealth GetClusterHealth();
private:
	ClusterHealth clusterState;
	DataDistributionLockWrapper* m_csFlags;
};

#endif // !defined(DATADISTRIBUTIONMASTERSHIPMANAGER_H__INCLUDED_)