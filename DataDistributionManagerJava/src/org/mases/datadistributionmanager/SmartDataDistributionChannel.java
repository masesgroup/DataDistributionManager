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

/**
 * Main class managing channel
 */
public class SmartDataDistributionChannel implements IDataDistributionChannelCallbackLow {
    SmartDataDistributionChannel() {
        m_DataDistributionChannelCallbackLow = NativeCallbackManager.RegisterCallback(this);
    }

    /**
     * Starts the channel
     * 
     * @param timeout Timeout
     * @return {@link HRESULT}
     */
    public HRESULT StartChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StartChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, timeout);
        return new HRESULT(res);
    }

    /**
     * Stops the channel
     * 
     * @param timeout Timeout
     * @return {@link HRESULT}
     */
    public HRESULT StopChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StopChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, timeout);
        return new HRESULT(res);
    }

    /**
     * Locks the channel
     * 
     * @param timeout Timeout
     * @return {@link HRESULT}
     */
    public HRESULT Lock(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Lock(IDataDistributionSubsystemManager_ptr, topicHandle,
                timeout);
        return new HRESULT(res);
    }

    /**
     * Unlocks the channel
     * 
     * @return {@link HRESULT}
     */
    public HRESULT Unlock() {
        long res = NativeInterface.IDataDistributionSubsystem_Unlock(IDataDistributionSubsystemManager_ptr,
                topicHandle);
        return new HRESULT(res);
    }

    /**
     * Seeks the channel
     * 
     * @param position position
     * @return {@link HRESULT}
     */
    public HRESULT SeekChannel(long position) {
        long res = NativeInterface.IDataDistributionSubsystem_SeekChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, position);
        return new HRESULT(res);
    }

    /**
     * Writes on the channel
     * 
     * @param key       The key to use
     * @param buffer    The buffer to write
     * @param waitAll   waits all write in the distributed environment
     * @param timestamp timestamp to apply
     * @return {@link HRESULT}
     */
    public HRESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll, long timestamp) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, key, buffer, waitAll, timestamp);
        return new HRESULT(res);
    }

    /**
     * Reads from the channel
     * 
     * @param offset offset in the stream
     * @param length number of elements
     * @return an array of {@link byte}
     */
    public byte[] ReadFromChannel(long offset, long length) {
        return NativeInterface.IDataDistributionSubsystem_ReadFromChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, offset, length);
    }

    /**
     * Change direction of the channel
     * 
     * @param direction {@link DDM_CHANNEL_DIRECTION}
     * @return {@link HRESULT}
     */
    public HRESULT ChangeDirectionOnChannel(DDM_CHANNEL_DIRECTION direction) {
        long res = NativeInterface.IDataDistributionSubsystem_ChangeDirectionOnChannel(
                IDataDistributionSubsystemManager_ptr, topicHandle, direction.atomicNumber);
        return new HRESULT(res);
    }

    /**
     * Called when a data is available
     * 
     * @param channelName The channel with data
     * @param key         Message key
     * @param buffer      Message buffer
     */
    public void OnDataAvailable(String channelName, String key, byte[] buffer) {

    }

    /**
     * Called when an event condition is raised from subsystem
     * 
     * @param channelName     The channel name
     * @param errorCode       The error code reported
     * @param nativeCode      The native code associated to the error if available
     * @param subSystemReason A string with a reason from subsystem
     */
    public void OnConditionOrError(String channelName, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode,
            String subSystemReason) {

    }

    public void OnUnderlyingEvent(long opaque, long topicHandle, UnderlyingEvent uEvent) {
        if (uEvent.IsDataAvailable) {
            OnDataAvailable(uEvent.ChannelName, uEvent.Key, uEvent.Buffer);
        } else {
            OnConditionOrError(uEvent.ChannelName, uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason);
        }
    }

    public final void OnUnderlyingEvent(long opaque, long topicHandle, String topicName, int condition,
            boolean isDataAvailable, String key, byte[] buffer, int nativeCode, String subSystemReason) {
        OnUnderlyingEvent(opaque, topicHandle,
                new UnderlyingEvent(topicName, DDM_UNDERLYING_ERROR_CONDITION.valueOfAtomicNumber(condition),
                        isDataAvailable, key, buffer, nativeCode, subSystemReason));
    }

    DDM_CHANNEL_DIRECTION m_direction;
    long topicHandle;
    long IDataDistributionSubsystemManager_ptr;
    long m_DataDistributionChannelCallbackLow;
}
