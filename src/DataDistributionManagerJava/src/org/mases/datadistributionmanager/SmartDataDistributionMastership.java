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

package org.mases.datadistributionmanager;

import java.util.HashMap;

class SmartDataDistributionMastership implements ISmartDataDistributionMastership {
    long IDataDistribution_instance = 0;

    SmartDataDistributionMastership(long ptr) {
        IDataDistribution_instance = ptr;
    }

    public void SetParameter(String paramName, String paramValue) {
        NativeInterface.IDataDistributionMastershipCommon_SetParameter(IDataDistribution_instance, paramName,
                paramValue);
    }

    public String GetParameter(String paramName) {
        return NativeInterface.IDataDistributionMastershipCommon_GetParameter(IDataDistribution_instance, paramName);
    }

    public OPERATION_RESULT Start(int dwMilliseconds) {
        long res = -1;
        res = NativeInterface.IDataDistributionMastershipCommon_Start(IDataDistribution_instance, dwMilliseconds);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT Stop(int dwMilliseconds) {
        long res = -1;
        res = NativeInterface.IDataDistributionMastershipCommon_Stop(IDataDistribution_instance, dwMilliseconds);
        return new OPERATION_RESULT(res);
    }

    public void ChangeMyState(DDM_INSTANCE_STATE newState) {
        NativeInterface.IDataDistributionMastershipCommon_ChangeMyState(IDataDistribution_instance,
                newState.atomicNumber);
    }

    public void ChangeState(long instanceId, DDM_INSTANCE_STATE newState) {
        NativeInterface.IDataDistributionMastershipCommon_ChangeState(IDataDistribution_instance, instanceId,
                newState.atomicNumber);
    }

    public boolean RequestIAmNextPrimary() {
        return NativeInterface.IDataDistributionMastershipCommon_RequestIAmNextPrimary(IDataDistribution_instance);
    }

    public boolean getIamNextPrimary() {
        return NativeInterface.IDataDistributionMastershipCommon_GetIamNextPrimary(IDataDistribution_instance);
    }

    public HashMap<Long, DDM_INSTANCE_STATE> getClusterState() {
        HashMap<Long, DDM_INSTANCE_STATE> map = new HashMap<>();
        long[] indexes = NativeInterface
                .IDataDistributionMastershipCommon_GetClusterIndexes(IDataDistribution_instance);
        for (long l : indexes) {
            int status = NativeInterface.IDataDistributionMastershipCommon_GetStateOf(IDataDistribution_instance, l);
            map.put(l, DDM_INSTANCE_STATE.valueOfAtomicNumber(status));
        }
        return map;
    }

    public HashMap<Long, ClusterHealthElement> getClusterHealth() {
        HashMap<Long, ClusterHealthElement> map = new HashMap<>();
        long[] indexes = NativeInterface
                .IDataDistributionMastershipCommon_GetClusterIndexes(IDataDistribution_instance);
        for (long l : indexes) {
            ClusterHealthElement elem = NativeInterface
                    .IDataDistributionMastershipCommon_GetClusterHealth(IDataDistribution_instance, l);
            map.put(l, elem);
        }
        return map;
    }

    public DDM_INSTANCE_STATE getMyState() {
        int state = NativeInterface.IDataDistributionMastershipCommon_GetMyState(IDataDistribution_instance);
        return DDM_INSTANCE_STATE.valueOfAtomicNumber(state);
    }

    public long getLocalServerId() {
        return NativeInterface.IDataDistributionMastershipCommon_GetLocalServerId(IDataDistribution_instance);
    }

    public long getPrimaryServerId() {
        return NativeInterface.IDataDistributionMastershipCommon_GetPrimaryServerId(IDataDistribution_instance);
    }

    public long getMessageDelay() {
        return NativeInterface.IDataDistributionMastershipCommon_GetMessageDelay(IDataDistribution_instance);
    }

    public long getUpTime() {
        return NativeInterface.IDataDistributionMastershipCommon_GetUpTime(IDataDistribution_instance);
    }
}