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

using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Runtime.InteropServices;

namespace MASES.DataDistributionManager.Bindings
{
    public class SmartDataDistributionChannel : IDataDistributionChannelCallbackLow
    {
        public SmartDataDistributionChannel()
        {
            m_DataDistributionChannelCallbackLow = new DataDistributionChannelCallbackLow(IntPtr.Zero, this);
        }

        ~SmartDataDistributionChannel()
        {

        }

        public HRESULT StartChannel(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StartChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, dwMilliseconds);
        }

        public HRESULT StopChannel(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StopChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, dwMilliseconds);
        }

        public HRESULT Lock(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Lock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }

        public HRESULT Unlock()
        {
          return  DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Unlock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle);
        }

        public HRESULT SeekChannel(Int64 position)
        {
           return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SeekChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, position);
        }

        public HRESULT WriteOnChannel(string key, byte[] buffer, bool waitAll = false, Int64 timestamp = -1)
        {
            IntPtr unmanagedPointer = Marshal.AllocHGlobal(buffer.Length);
            try
            {
                Marshal.Copy(buffer, 0, unmanagedPointer, buffer.Length);
                // Call unmanaged code 
                return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_WriteOnChannel>().Invoke(
                    IDataDistributionSubsystemManager_ptr, channelHandle, key, (key != null) ? new IntPtr(key.Length) : IntPtr.Zero, unmanagedPointer, new IntPtr(buffer.Length), waitAll, timestamp);
            }
            finally
            {
                Marshal.FreeHGlobal(unmanagedPointer);
            }
        }

        public HRESULT ReadFromChannel(Int64 offset, long length, out byte[] buffer)
        {
            try
            {
                IntPtr dataBuffer = IntPtr.Zero;
                long dataLen = 0;
                buffer = null;
                // Call unmanaged code
                IntPtr len = new IntPtr(length);
                HRESULT status = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_ReadFromChannel>().Invoke(
                    IDataDistributionSubsystemManager_ptr, channelHandle, offset, dataBuffer, len);

                if (status.Failed) return status;

                buffer = new byte[dataLen];
                Marshal.Copy(dataBuffer, buffer, 0, len.ToInt32());
                return status;
            }
            finally
            {
            }
        }

        public HRESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_ChangeChannelDirection>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, direction);
        }

        public virtual void OnDataAvailable(string channelName, string key, byte[] buffer)
        {

        }

        public virtual void OnConditionOrError(string channelName, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, string subSystemReason)
        {

        }

        void IDataDistributionChannelCallbackLow.OnUnderlyingEvent(IntPtr opaque, IntPtr channelHandle, UnderlyingEvent uEvent)
        {
            if (uEvent.IsDataAvailable)
            {
                byte[] data = new byte[uEvent.BufferLength.ToInt64()];
                Marshal.Copy(uEvent.Buffer, data, 0, data.Length);

                byte[] keyData = new byte[uEvent.KeyLen.ToInt64()];
                Marshal.Copy(uEvent.Key, keyData, 0, keyData.Length);
                string key = string.Empty;
                try
                {
                    key = System.Text.Encoding.ASCII.GetString(keyData);
                }
                catch (Exception ex) { key = ex.Message; }
                OnDataAvailable(uEvent.ChannelName, key, data);
            }
            else
            {
                OnConditionOrError(uEvent.ChannelName, uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason);
            }
        }

        internal DDM_CHANNEL_DIRECTION m_direction = DDM_CHANNEL_DIRECTION.ALL;
        internal IntPtr channelHandle;
        internal IntPtr IDataDistributionSubsystemManager_ptr;
        internal DataDistributionChannelCallbackLow m_DataDistributionChannelCallbackLow;
    }
}
