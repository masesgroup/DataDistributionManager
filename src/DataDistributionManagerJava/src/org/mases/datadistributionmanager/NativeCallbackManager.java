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

import java.util.*;

class NativeCallbackManager {
    private static final Map<IDataDistributionCallbackLow, Long> BY_IDataDistributionCallbackLow = new HashMap<>();
    private static final Map<IDataDistributionChannelCallbackLow, Long> BY_IDataDistributionChannelCallbackLow = new HashMap<>();
    private static final Map<IDataDistributionMastershipCallbackLow, Long> BY_IDataDistributionMastershipCallbackLow = new HashMap<>();

    public static long RegisterCallback(IDataDistributionCallbackLow callbacks)
    {
        long remoteReference = NativeInterface.DataDistributionCallback_create(callbacks);
        BY_IDataDistributionCallbackLow.put(callbacks, remoteReference);
        return remoteReference;
    }

    public static void UnregisterCallback(IDataDistributionCallbackLow callbacks)
    {
       Long result = BY_IDataDistributionCallbackLow.get(callbacks);
        NativeInterface.DataDistributionCallback_delete(result.longValue());
    }

    public static long RegisterCallback(IDataDistributionChannelCallbackLow callbacks)
    {
        long remoteReference = NativeInterface.DataDistributionChannelCallback_create(callbacks);
        BY_IDataDistributionChannelCallbackLow.put(callbacks, remoteReference);
        return remoteReference;
    }

    public static void UnregisterCallback(IDataDistributionChannelCallbackLow callbacks)
    {
       Long result = BY_IDataDistributionChannelCallbackLow.get(callbacks);
        NativeInterface.DataDistributionChannelCallback_delete(result.longValue());
    }

    public static long RegisterCallback(IDataDistributionMastershipCallbackLow callbacks)
    {
        long remoteReference = NativeInterface.DataDistributionMastershipCallback_create(callbacks);
        BY_IDataDistributionMastershipCallbackLow.put(callbacks, remoteReference);
        return remoteReference;
    }

    public static void UnregisterCallback(IDataDistributionMastershipCallbackLow callbacks)
    {
       Long result = BY_IDataDistributionMastershipCallbackLow.get(callbacks);
        NativeInterface.DataDistributionMastershipCallback_delete(result.longValue());
    }

    void dataDistributionLoggingCb(Object opaque, DDM_LOG_LEVEL level, String source, String function, String errorStr) {

    }

    void dataDistributionCompletelyDisconnectedCb(long opaque, String source, String errorStr) {

    }

    void dataDistributionUnderlyingEvent(long opaque, long topicHandle, long uEvent) {

    }

    void dataDistributionOnClusterStateChange(long opaque, DDM_CLUSTEREVENT change, long serverid) {

    }

    void dataDistributionOnStateChange(long opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) {

    }

    void dataDistributionOnStateReady(long opaque, byte[] state) {

    }

    void dataDistributionOnRequestedState(long opaque, byte[] state) {

    }

    void dataDistributionOnMultiplePrimary(long opaque, long myId, long otherId) {

    }

    void dataDistributionFirstStateChange(long opaque, DDM_INSTANCE_STATE newState) {

    }

    void dataDistributionChangingState(long opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) {

    }

    void dataDistributionChangedState(long opaque, DDM_INSTANCE_STATE newState) {

    }
}