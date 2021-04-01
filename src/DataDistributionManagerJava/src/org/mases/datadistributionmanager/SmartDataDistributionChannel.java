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

import java.nio.charset.Charset;

/**
 * Main class managing channel
 */
public class SmartDataDistributionChannel implements IDataDistributionChannelCallbackLow {
    /**
     * No timestamp value
     */
    public final long DDM_NO_TIMESTAMP = -1;

    SynchronizedEventsManager<IDataAvailableListener> m_IDataAvailable_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IConditionOrErrorListener> m_IConditionOrError_listeners = new SynchronizedEventsManager<>();

    /**
     * Ctor
     */
    public SmartDataDistributionChannel() {
        m_DataDistributionChannelCallbackLow = NativeCallbackManager.RegisterCallback(this);
    }

    /**
     * Starts the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT StartChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StartChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, timeout);
        return new OPERATION_RESULT(res);
    }

    /**
     * Stops the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT StopChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StopChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, timeout);
        return new OPERATION_RESULT(res);
    }

    /**
     * Set parameter on channel
     * 
     * @param paramName  Parameter name to set
     * @param paramValue Parameter value to set
     */
    public void SetParameter(String paramName, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, channelHandle,
                paramName, paramValue);
    }

    /**
     * Set parameter on channel
     * 
     * @param paramId    Parameter {@link DDM_GENERAL_PARAMETER} to set
     * @param paramValue Parameter value to set
     */
    public void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, channelHandle,
                paramId.atomicNumber, paramValue);
    }

    /**
     * Get parameter from channel
     * 
     * @param paramName Parameter name to get
     * @return Parameter value
     */
    public String GetParameter(String paramName) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr,
                channelHandle, paramName);
    }

    /**
     * Get parameter from channel
     * 
     * @param paramId Parameter {@link DDM_GENERAL_PARAMETER} to get
     * @return Parameter value
     */
    public String GetParameter(DDM_GENERAL_PARAMETER paramId) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr,
                channelHandle, paramId.atomicNumber);
    }

    /**
     * Locks the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT Lock(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Lock(IDataDistributionSubsystemManager_ptr, channelHandle,
                timeout);
        return new OPERATION_RESULT(res);
    }

    /**
     * Unlocks the channel
     * 
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT Unlock() {
        long res = NativeInterface.IDataDistributionSubsystem_Unlock(IDataDistributionSubsystemManager_ptr,
                channelHandle);
        return new OPERATION_RESULT(res);
    }

    /**
     * Seeks the channel
     * 
     * @param position position
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT SeekChannel(long position) {
        long res = NativeInterface.IDataDistributionSubsystem_SeekChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, position);
        return new OPERATION_RESULT(res);
    }

    /**
     * Writes on the channel
     * 
     * @param value The {@link String} to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String value) {
        return WriteOnChannel(null, value, false, DDM_NO_TIMESTAMP);
    }

    /**
     * Writes on the channel
     * 
     * @param key   The key to use
     * @param value The {@link String} to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, String value) {
        return WriteOnChannel(key, value, false, DDM_NO_TIMESTAMP);
    }

    /**
     * Writes on the channel
     * 
     * @param key     The key to use
     * @param value   The {@link String} to write in the channel
     * @param waitAll waits all write in the distributed environment
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll) {
        return WriteOnChannel(key, value, waitAll, DDM_NO_TIMESTAMP);
    }

    /**
     * Writes on the channel
     * 
     * @param key       The key to use
     * @param value     The {@link String} to write in the channel
     * @param waitAll   waits all write in the distributed environment
     * @param timestamp timestamp to apply
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll, long timestamp) {
        byte[] buffer = value.getBytes(Charset.forName("UTF8"));
        return WriteOnChannel(key, buffer, waitAll, timestamp);
    }

    /**
     * Writes on the channel
     * 
     * @param buffer The buffer to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(byte[] buffer) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, null, buffer, false, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    /**
     * Writes on the channel
     * 
     * @param key    The key to use
     * @param buffer The buffer to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, key, buffer, false, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    /**
     * Writes on the channel
     * 
     * @param key     The key to use
     * @param buffer  The buffer to write in the channel
     * @param waitAll waits all write in the distributed environment
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, key, buffer, waitAll, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    /**
     * Writes on the channel
     * 
     * @param key       The key to use
     * @param buffer    The buffer to write in the channel
     * @param waitAll   waits all write in the distributed environment
     * @param timestamp timestamp to apply
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll, long timestamp) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                channelHandle, key, buffer, waitAll, timestamp);
        return new OPERATION_RESULT(res);
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
                channelHandle, offset, length);
    }

    /**
     * Change direction of the channel
     * 
     * @param direction {@link DDM_CHANNEL_DIRECTION}
     * @return {@link OPERATION_RESULT}
     */
    public OPERATION_RESULT ChangeDirectionOnChannel(DDM_CHANNEL_DIRECTION direction) {
        long res = NativeInterface.IDataDistributionSubsystem_ChangeDirectionOnChannel(
                IDataDistributionSubsystemManager_ptr, channelHandle, direction.atomicNumber);
        return new OPERATION_RESULT(res);
    }

    /**
     * Adds a {@link IDataAvailableListener} listener
     * 
     * @param listener {@link IDataAvailableListener} listener to add
     */
    public void addListener(IDataAvailableListener listener) {
        m_IDataAvailable_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IDataAvailableListener} listener
     * 
     * @param listener {@link IDataAvailableListener} listener to remove
     */
    public void removeListener(IDataAvailableListener listener) {
        m_IDataAvailable_listeners.removeListener(listener);
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
     * Adds a {@link IConditionOrErrorListener} listener
     * 
     * @param listener {@link IConditionOrErrorListener} listener to add
     */
    public synchronized void addListener(IConditionOrErrorListener listener) {
        m_IConditionOrError_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IConditionOrErrorListener} listener
     * 
     * @param listener {@link IConditionOrErrorListener} listener to remove
     */
    public synchronized void removeListener(IConditionOrErrorListener listener) {
        m_IConditionOrError_listeners.removeListener(listener);
    }

    /**
     * Called when an event condition is raised from subsystem
     * 
     * @param channelName     The channel name
     * @param errorCode       The error code reported
     * @param nativeCode      The native code associated to the error if available
     * @param subSystemReason A String with a reason from subsystem
     */
    public void OnConditionOrError(String channelName, OPERATION_RESULT errorCode, int nativeCode,
            String subSystemReason) {

    }

    public void OnUnderlyingEvent(long opaque, long channelHandle, UnderlyingEvent uEvent) {
        if (uEvent.IsDataAvailable) {
            OnDataAvailable(uEvent.ChannelName, uEvent.Key, uEvent.Buffer);
            for (IDataAvailableListener iDataAvailable : m_IDataAvailable_listeners) {
                try {
                    iDataAvailable.OnDataAvailable(uEvent.ChannelName, uEvent.Key, uEvent.Buffer);
                } catch (Throwable throwable) {
                    OnConditionOrError(uEvent.ChannelName, OPERATION_RESULT.E_FAIL, 0, throwable.getMessage());
                }
            }
        } else {
            OnConditionOrError(uEvent.ChannelName, uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason);
            for (IConditionOrErrorListener iConditionOrError : m_IConditionOrError_listeners) {
                try {
                    iConditionOrError.OnConditionOrError(uEvent.ChannelName, uEvent.Condition, uEvent.NativeCode,
                            uEvent.SubSystemReason);
                } catch (Throwable throwable) {
                    OnConditionOrError(uEvent.ChannelName, OPERATION_RESULT.E_FAIL, 0, throwable.getMessage());
                }
            }
        }
    }

    public final void OnUnderlyingEvent(long opaque, long channelHandle, String topicName, int condition,
            boolean isDataAvailable, String key, byte[] buffer, int nativeCode, String subSystemReason) {
        OnUnderlyingEvent(opaque, channelHandle,
                new UnderlyingEvent(topicName, OPERATION_RESULT.fromCondition(condition),
                        isDataAvailable, key, buffer, nativeCode, subSystemReason));
    }

    DDM_CHANNEL_DIRECTION m_direction;
    long channelHandle;
    long IDataDistributionSubsystemManager_ptr;
    long m_DataDistributionChannelCallbackLow;
}
