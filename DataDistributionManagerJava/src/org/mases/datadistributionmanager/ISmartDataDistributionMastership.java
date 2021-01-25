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

package org.mases.datadistributionmanager;

import java.util.HashMap;

public interface ISmartDataDistributionMastership {
    long Start(int dwMilliseconds);

    long Stop(int dwMilliseconds);

    void ChangeMyState(DDM_INSTANCE_STATE newState);

    void ChangeState(long instanceId, DDM_INSTANCE_STATE newState);

    boolean RequestIAmNextPrimary();

    boolean getIamNextPrimary();

    HashMap<Long, DDM_INSTANCE_STATE> getClusterState();

    HashMap<Long, ClusterHealthElement> getClusterHealth();

    DDM_INSTANCE_STATE getMyState();

    long getLocalServerId();

    long getPrimaryServerId();

    long getMessageDelay();

    long getUpTime();
}