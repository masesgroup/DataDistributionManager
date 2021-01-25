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

public class SmartDataDistributionChannel implements IDataDistributionChannelCallbackLow {
    public SmartDataDistributionChannel() {
        m_DataDistributionChannelCallbackLow = NativeCallbackManager.RegisterCallback(this);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
    }

    public HRESULT StartChannel(int dwMilliseconds) {
        long res = NativeInterface.IDataDistributionSubsystem_StartChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, dwMilliseconds);
        return new HRESULT(res);
    }

    public HRESULT StopChannel(int dwMilliseconds) {
        long res = NativeInterface.IDataDistributionSubsystem_StopChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, dwMilliseconds);
        return new HRESULT(res);
    }

    public HRESULT Lock(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Lock(IDataDistributionSubsystemManager_ptr, topicHandle,
                timeout);
        return new HRESULT(res);
    }

    public HRESULT Unlock() {
        long res = NativeInterface.IDataDistributionSubsystem_Unlock(IDataDistributionSubsystemManager_ptr,
                topicHandle);
        return new HRESULT(res);
    }

    public HRESULT SeekChannel(long position) {
        long res = NativeInterface.IDataDistributionSubsystem_SeekChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, position);
        return new HRESULT(res);
    }

    public HRESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll, long timestamp) {
        // Call unmanaged code
        long res = NativeInterface.IDataDistributionSubsystem_WriteOnChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, key, buffer, waitAll, timestamp);
        return new HRESULT(res);
    }

    public byte[] ReadFromChannel(long offset, long length) {
        return NativeInterface.IDataDistributionSubsystem_ReadFromChannel(IDataDistributionSubsystemManager_ptr,
                topicHandle, offset, length);
    }

    public HRESULT ChangeDirectionOnChannel(DDM_CHANNEL_DIRECTION direction) {
        long res = NativeInterface.IDataDistributionSubsystem_ChangeDirectionOnChannel(
                IDataDistributionSubsystemManager_ptr, topicHandle, direction.atomicNumber);
        return new HRESULT(res);
    }

    public void OnDataAvailable(String topicName, String key, byte[] buffer) {

    }

    public void OnConditionOrError(String topicName, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode,
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
