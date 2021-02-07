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
    /// <summary>
    /// Main class managing channel
    /// </summary>
    public class SmartDataDistributionChannel : IDataDistributionChannelCallbackLow
    {
        /// <summary>
        /// Ctor
        /// </summary>
        SmartDataDistributionChannel()
        {
            m_DataDistributionChannelCallbackLow = new DataDistributionChannelCallbackLow(IntPtr.Zero, this);
        }
        /// <summary>
        /// Finalizer
        /// </summary>
        ~SmartDataDistributionChannel()
        {

        }
        /// <summary>
        /// Starts the channel
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT StartChannel(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StartChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <summary>
        /// Stops the channel
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT StopChannel(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StopChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <summary>
        /// Lock the channel
        /// </summary>
        /// <param name="timeout">Timeout to acuire lock</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Lock(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Lock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <summary>
        /// Unlock the channel
        /// </summary>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Unlock()
        {
          return  DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Unlock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle);
        }
        /// <summary>
        /// Seek the channel
        /// </summary>
        /// <param name="position">Seek poisition</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT SeekChannel(Int64 position)
        {
           return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SeekChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, position);
        }
        /// <summary>
        /// Writes in the channel
        /// </summary>
        /// <param name="key">Key to use in the channel message</param>
        /// <param name="buffer">The data buffer in the channel message</param>
        /// <param name="waitAll">Wait a complete acknowledge from the peers</param>
        /// <param name="timestamp">timestamp to associated to the message</param>
        /// <returns></returns>
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
        /// <summary>
        /// Reads data from channel
        /// </summary>
        /// <param name="offset">Position where read begins</param>
        /// <param name="length">Number of elements to read</param>
        /// <param name="buffer">Result buffer</param>
        /// <returns><see cref="HRESULT"/></returns>
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
        /// <summary>
        /// Changes channel direction
        /// </summary>
        /// <param name="direction">New <see cref="DDM_CHANNEL_DIRECTION"/></param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_ChangeChannelDirection>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, direction);
        }
        /// <summary>
        /// Called when a data is available
        /// </summary>
        /// <param name="channelName">The channel with data</param>
        /// <param name="key">Message key</param>
        /// <param name="buffer">Message buffer</param>
        public virtual void OnDataAvailable(string channelName, string key, byte[] buffer)
        {

        }
        /// <summary>
        /// Called when an event condition is raised from subsystem
        /// </summary>
        /// <param name="channelName">The channel name</param>
        /// <param name="errorCode">The error code reported</param>
        /// <param name="nativeCode">The native code associated to the error if available</param>
        /// <param name="subSystemReason">A string with a reason from subsystem</param>
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
