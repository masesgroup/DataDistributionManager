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

class NativeInterface {
        static native long DataDistribution_create();

        static native long DataDistributionCallback_create(Object opaque);

        static native void DataDistributionCallback_delete(long ddcb);

        static native long DataDistributionChannelCallback_create(Object opaque);

        static native void DataDistributionChannelCallback_delete(long ddtcb);

        static native long DataDistributionMastershipCallback_create(Object opaque);

        static native void DataDistributionMastershipCallback_delete(long ddmcb);

        // IDataDistribution interface
        static native long IDataDistribution_Initialize(long IDataDistribution_instance, long iddcb, String conf_file,
                        String szMyAddress, String topicTrailer);

        static native long IDataDistribution_Initialize2(long IDataDistribution_instance, long iddcb,
                        String[] arrayParams, String szMyAddress, String topicTrailer);

        static native long IDataDistribution_RequestMastershipManager(long IDataDistribution_instance, long cbs,
                        String szMyAddress, String[] arrayParams);

        static native long IDataDistribution_GetSubsystemManager(long IDataDistribution_instance);

        static native long IDataDistribution_GetMastershipManager(long IDataDistribution_instance);

        static native String IDataDistribution_GetProtocol(long IDataDistribution_instance);

        static native String IDataDistribution_GetProtocolLib(long IDataDistribution_instance);

        static native String IDataDistribution_GetMastershipLib(long IDataDistribution_instance);

        // IDataDistributionSubsystem interface

        static native int IDataDistributionSubsystem_GetServerLostTimeout(long IDataDistributionSubsystem_instance);

        static native long IDataDistributionSubsystem_GetMaxMessageSize(long IDataDistributionSubsystem_instance);

        static native long IDataDistributionSubsystem_Start(long IDataDistributionSubsystem_instance,
                        long dwMilliseconds);

        static native long IDataDistributionSubsystem_Stop(long IDataDistributionSubsystem_instance,
                        long dwMilliseconds);

        static native long IDataDistributionSubsystem_CreateChannel(long IDataDistributionSubsystem_instance,
                        String topicName, long dataCb, int direction, String[] arrayParams);

        static native long IDataDistributionSubsystem_StartChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, long dwMilliseconds);

        static native long IDataDistributionSubsystem_StopChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, long dwMilliseconds);

        static native long IDataDistributionSubsystem_Lock(long IDataDistributionSubsystem_instance, long topicHandle,
                        long timeout);

        static native long IDataDistributionSubsystem_Unlock(long IDataDistributionSubsystem_instance,
                        long topicHandle);

        static native long IDataDistributionSubsystem_SeekChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, long position);

        static native long IDataDistributionSubsystem_DeleteChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle);

        static native long IDataDistributionSubsystem_WriteOnChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, String key, byte[] param, boolean waitAll, long timestamp);

        static native byte[] IDataDistributionSubsystem_ReadFromChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, long offset, long length);

        static native long IDataDistributionSubsystem_ChangeDirectionOnChannel(long IDataDistributionSubsystem_instance,
                        long topicHandle, int direction);

        static native long IDataDistributionMastershipCommon_Start(long IDataDistribution_instance, int dwMilliseconds);

        static native long IDataDistributionMastershipCommon_Stop(long IDataDistribution_instance, int dwMilliseconds);

        static native boolean IDataDistributionMastershipCommon_GetIamNextPrimary(long IDataDistribution_instance);

        static native boolean IDataDistributionMastershipCommon_RequestIAmNextPrimary(long IDataDistribution_instance);

        static native long[] IDataDistributionMastershipCommon_GetClusterIndexes(long IDataDistribution_instance);

        static native int IDataDistributionMastershipCommon_GetStateOf(long IDataDistribution_instance, long index);

        static native ClusterHealthElement IDataDistributionMastershipCommon_GetClusterHealth(
                        long IDataDistribution_instance, long index);

        static native int IDataDistributionMastershipCommon_GetMyState(long IDataDistribution_instance);

        static native void IDataDistributionMastershipCommon_ChangeMyState(long IDataDistribution_instance,
                        int newState);

        static native void IDataDistributionMastershipCommon_ChangeState(long IDataDistribution_instance,
                        long instanceId, int newState);

        static native long IDataDistributionMastershipCommon_GetLocalServerId(long IDataDistribution_instance);

        static native long IDataDistributionMastershipCommon_GetPrimaryServerId(long IDataDistribution_instance);

        static native long IDataDistributionMastershipCommon_GetMessageDelay(long IDataDistribution_instance);

        static native long IDataDistributionMastershipCommon_GetUpTime(long IDataDistribution_instance);

}