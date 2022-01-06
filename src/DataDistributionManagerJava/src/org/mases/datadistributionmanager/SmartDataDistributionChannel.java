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

import java.nio.charset.Charset;
import java.time.Duration;
import java.util.Date;

/**
 * Main class managing channel
 */
public class SmartDataDistributionChannel
        implements ISmartDataDistributionChannel, IDataDistributionChannelCallbackLow {
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

    public OPERATION_RESULT StartChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StartChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, timeout);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT StopChannel(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_StopChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, timeout);
        return new OPERATION_RESULT(res);
    }

    public void SetParameter(String paramName, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, m_channelHandle,
                paramName, paramValue);
    }

    public void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, m_channelHandle,
                paramId.atomicNumber, paramValue);
    }

    public String GetParameter(String paramName) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, paramName);
    }

    public String GetParameter(DDM_GENERAL_PARAMETER paramId) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, paramId.atomicNumber);
    }

    public OPERATION_RESULT Lock(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Lock(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, timeout);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT Unlock() {
        long res = NativeInterface.IDataDistributionSubsystem_Unlock(IDataDistributionSubsystemManager_ptr,
                m_channelHandle);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT SeekChannel(long position) {
        return SeekChannel(position, DDM_SEEKCONTEXT.OFFSET, DDM_SEEKKIND.ABSOLUTE);
    }

    public OPERATION_RESULT SeekChannel(long position, DDM_SEEKKIND kind) {
        return SeekChannel(position, DDM_SEEKCONTEXT.OFFSET, kind);
    }

    public OPERATION_RESULT SeekChannel(Date position) {
        return SeekChannel(position.getTime(), DDM_SEEKCONTEXT.TIMESTAMP, DDM_SEEKKIND.ABSOLUTE);
    }

    public OPERATION_RESULT SeekChannel(Duration position) {
        return SeekChannel(position.toMillis(), DDM_SEEKCONTEXT.TIMESTAMP, DDM_SEEKKIND.RELATIVE);
    }

    public OPERATION_RESULT SeekChannel(long position, DDM_SEEKCONTEXT context, DDM_SEEKKIND kind) {
        long res = NativeInterface.IDataDistributionSubsystem_SeekChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, position, context.atomicNumber, kind.atomicNumber);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT WriteOnChannel(String value) {
        return WriteOnChannel(null, value, false, DDM_NO_TIMESTAMP);
    }

    public OPERATION_RESULT WriteOnChannel(String key, String value) {
        return WriteOnChannel(key, value, false, DDM_NO_TIMESTAMP);
    }

    public OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll) {
        return WriteOnChannel(key, value, waitAll, DDM_NO_TIMESTAMP);
    }

    public OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll, long timestamp) {
        byte[] buffer = value.getBytes(Charset.forName("UTF8"));
        return WriteOnChannel(key, buffer, waitAll, timestamp);
    }

    public OPERATION_RESULT WriteOnChannel(byte[] buffer) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, null, buffer, false, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, key, buffer, false, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, key, buffer, waitAll, DDM_NO_TIMESTAMP);
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll, long timestamp) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, key, buffer, waitAll, timestamp);
        return new OPERATION_RESULT(res);
    }

    public byte[] ReadFromChannel(long offset, long length) {
        return NativeInterface.IDataDistributionSubsystem_ReadFromChannel(IDataDistributionSubsystemManager_ptr,
                m_channelHandle, offset, length);
    }

    public OPERATION_RESULT ChangeDirectionOnChannel(DDM_CHANNEL_DIRECTION direction) {
        long res = NativeInterface.IDataDistributionSubsystem_ChangeDirectionOnChannel(
                IDataDistributionSubsystemManager_ptr, m_channelHandle, direction.atomicNumber);
        return new OPERATION_RESULT(res);
    }

    public void addListener(IDataAvailableListener listener) {
        m_IDataAvailable_listeners.addListener(listener);
    }

    public void removeListener(IDataAvailableListener listener) {
        m_IDataAvailable_listeners.removeListener(listener);
    }

    /**
     * Called when a data is available
     * 
     * @param key    Message key
     * @param buffer Message buffer
     */
    public void OnDataAvailable(String key, byte[] buffer) {

    }

    public void addListener(IConditionOrErrorListener listener) {
        m_IConditionOrError_listeners.addListener(listener);
    }

    public void removeListener(IConditionOrErrorListener listener) {
        m_IConditionOrError_listeners.removeListener(listener);
    }

    /**
     * Called when an event condition is raised from subsystem
     * 
     * @param errorCode       The error code reported
     * @param nativeCode      The native code associated to the error if available
     * @param subSystemReason A String with a reason from subsystem
     */
    public void OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode, String subSystemReason) {

    }

    public String getChannelName() {
        return m_channelName;
    }

    public long getTimestamp() {
        return m_timestamp;
    }

    public long getOffset() {
        return m_offset;
    }

    public void OnUnderlyingEvent(long opaque, long channelHandle, UnderlyingEvent uEvent) {
        if (uEvent.IsDataAvailable) {
            m_timestamp = uEvent.Timestamp;
            m_offset = uEvent.Offset;
            OnDataAvailable(uEvent.Key, uEvent.Buffer);
            for (IDataAvailableListener iDataAvailable : m_IDataAvailable_listeners) {
                try {
                    iDataAvailable.OnDataAvailable(this, uEvent.Key, uEvent.Buffer);
                } catch (Throwable throwable) {
                    OnConditionOrError(OPERATION_RESULT.E_FAIL, 0, throwable.getMessage());
                }
            }
        } else {
            OnConditionOrError(uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason);
            for (IConditionOrErrorListener iConditionOrError : m_IConditionOrError_listeners) {
                try {
                    iConditionOrError.OnConditionOrError(this, uEvent.Condition, uEvent.NativeCode,
                            uEvent.SubSystemReason);
                } catch (Throwable throwable) {
                    OnConditionOrError(OPERATION_RESULT.E_FAIL, 0, throwable.getMessage());
                }
            }
        }
    }

    public final void OnUnderlyingEvent(long opaque, long channelHandle, String topicName, int condition,
            boolean isDataAvailable, String key, byte[] buffer, int nativeCode, String subSystemReason, long timestamp,
            long offset) {
        OnUnderlyingEvent(opaque, channelHandle,
                new UnderlyingEvent(topicName, OPERATION_RESULT.fromCondition(condition), isDataAvailable, key, buffer,
                        nativeCode, subSystemReason, timestamp, offset));
    }

    DDM_CHANNEL_DIRECTION m_direction;
    long m_channelHandle;
    String m_channelName;
    long m_timestamp;
    long m_offset;
    long IDataDistributionSubsystemManager_ptr;
    long m_DataDistributionChannelCallbackLow;
}
