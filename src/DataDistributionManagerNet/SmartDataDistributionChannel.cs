﻿/*
*  Copyright 2023 MASES s.r.l.
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
using System.Text;

namespace MASES.DataDistributionManager.Bindings
{
    #region DataAvailableEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistributionChannel.DataAvailable"/> event
    /// </summary>
    public class DataAvailableEventArgs : EventArgs
    {
        internal DataAvailableEventArgs(ISmartDataDistributionChannelInfo channelInfo, string key, byte[] buffer)
        {
            ChannelInfo = channelInfo;
            Key = key;
            Buffer = buffer;
            try
            {
                DecodedString = Encoding.UTF8.GetString(Buffer);
            }
            catch { }
        }
        /// <summary>
        /// The <see cref="ISmartDataDistributionChannelInfo"/> reference emitting data
        /// </summary>
        public ISmartDataDistributionChannelInfo ChannelInfo { get; private set; }
        /// <summary>
        /// The key associated to the message
        /// </summary>
        public string Key { get; private set; }
        /// <summary>
        /// The data buffer received
        /// </summary>
        public byte[] Buffer { get; private set; }
        /// <summary>
        /// The decoded string if the buffer contains an UTF8 encoded string
        /// </summary>
        public string DecodedString { get; private set; }
    }
    #endregion

    #region ConditionOrErrorEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistributionChannel.ConditionOrError"/> event
    /// </summary>
    public class ConditionOrErrorEventArgs : EventArgs
    {
        internal ConditionOrErrorEventArgs(ISmartDataDistributionChannelInfo channelInfo, OPERATION_RESULT errorCode, int nativeCode, string subSystemReason)
        {
            ChannelInfo = channelInfo;
            ErrorCode = errorCode;
            NativeCode = nativeCode;
            SubSystemReason = subSystemReason;
        }
        /// <summary>
        /// The <see cref="ISmartDataDistributionChannelInfo"/> reference emitting data
        /// </summary>
        public ISmartDataDistributionChannelInfo ChannelInfo { get; private set; }
        /// <summary>
        /// The <see cref="OPERATION_RESULT"/>
        /// </summary>
        public OPERATION_RESULT ErrorCode { get; private set; }
        /// <summary>
        /// The native code from transport layer
        /// </summary>
        public int NativeCode { get; private set; }
        /// <summary>
        /// The decoded reason from subsystem
        /// </summary>
        public string SubSystemReason { get; private set; }
    }
    #endregion

    #region ISmartDataDistributionChannelInfo

    /// <summary>
    /// Interface containing information related to a channel
    /// </summary>
    public interface ISmartDataDistributionChannelInfo
    {
        /// <summary>
        /// The name of the channel
        /// </summary>
        string ChannelName { get; }
        /// <summary>
        /// The last available message timestamp associated to the data: Milliseconds since epoch (UTC).
        /// </summary>
        Int64 Timestamp { get; }
        /// <summary>
        /// The last available message <see cref="DateTime"/> associated to the data (UTC).
        /// </summary>
        DateTime DateTime { get; }
        /// <summary>
        /// The last available message offset associated to the data
        /// </summary>
        Int64 Offset { get; }
    }

    #endregion

    #region ISmartDataDistributionChannel
    /// <summary>
    /// Interface to interact with channel
    /// </summary>
    public interface ISmartDataDistributionChannel : ISmartDataDistributionChannelInfo
    {
        /// <summary>
        /// Starts the channel
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT StartChannel(uint timeout);
        /// <summary>
        /// Stops the channel
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT StopChannel(uint timeout);
        /// <summary>
        /// Set parameter on channel
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        void SetParameter(string paramName, string paramValue);
        /// <summary>
        /// Set parameter on channel
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        void SetParameter(DDM_GENERAL_PARAMETER paramId, string paramValue);
        /// <summary>
        /// Set parameter on channel
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <returns>Parameter value</returns>
        string GetParameter(string paramName);
        /// <summary>
        /// Set parameter on channel
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to set</param>
        /// <returns>Parameter value</returns>
        string GetParameter(DDM_GENERAL_PARAMETER paramId);
        /// <summary>
        /// Lock the channel
        /// </summary>
        /// <param name="timeout">Timeout to acuire lock in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Lock(uint timeout);
        /// <summary>
        /// Unlock the channel
        /// </summary>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Unlock();
        /// <summary>
        /// Seek the channel using absolute or relative offset
        /// </summary>
        /// <param name="position">Seek offset poisition</param>
        /// <param name="kind"><see cref="DDM_SEEKKIND"/> to use. Default is <see cref="DDM_SEEKKIND.ABSOLUTE"/></param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT SeekChannel(Int64 position, DDM_SEEKKIND kind = DDM_SEEKKIND.ABSOLUTE);
        /// <summary>
        /// Seek the channel to an absolute timestamp
        /// </summary>
        /// <param name="position">Seek timestamp to an absolute poisition</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT SeekChannel(DateTime position);
        /// <summary>
        /// Seek the channel by a relative time expressed as <see cref="TimeSpan"/> starting from latest known timestamp
        /// </summary>
        /// <param name="position">Seek timestamp relative poisition</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT SeekChannel(TimeSpan position);
        /// <summary>
        /// Seek the channel using absolute or relative offset
        /// </summary>
        /// <param name="position">Seek offset poisition</param>
        /// <param name="context"><see cref="DDM_SEEKCONTEXT"/> to use. Default is <see cref="DDM_SEEKCONTEXT.OFFSET"/></param>
        /// <param name="kind"><see cref="DDM_SEEKKIND"/> to use. Default is <see cref="DDM_SEEKKIND.ABSOLUTE"/></param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT SeekChannel(Int64 position, DDM_SEEKCONTEXT context = DDM_SEEKCONTEXT.OFFSET, DDM_SEEKKIND kind = DDM_SEEKKIND.ABSOLUTE);
        /// <summary>
        /// Writes in the channel
        /// </summary>
        /// <param name="value">The <see cref="string"/> to write in the channel</param>
        /// <param name="waitAll">Wait a complete acknowledge from the peers</param>
        /// <param name="timestamp">timestamp to associated to the message</param>
        /// <returns></returns>
        OPERATION_RESULT WriteOnChannel(string value, bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        /// <summary>
        /// Writes in the channel
        /// </summary>
        /// <param name="key">Key to use in the channel message</param>
        /// <param name="value">The <see cref="string"/> to write in the channel</param>
        /// <param name="waitAll">Wait a complete acknowledge from the peers</param>
        /// <param name="timestamp">timestamp to associated to the message</param>
        /// <returns></returns>
        OPERATION_RESULT WriteOnChannel(string key, string value, bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        /// <summary>
        /// Writes in the channel
        /// </summary>
        /// <param name="buffer">The data buffer in the channel message</param>
        /// <param name="waitAll">Wait a complete acknowledge from the peers</param>
        /// <param name="timestamp">timestamp to associated to the message</param>
        /// <returns></returns>
        OPERATION_RESULT WriteOnChannel(byte[] buffer, bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        /// <summary>
        /// Writes in the channel
        /// </summary>
        /// <param name="key">Key to use in the channel message</param>
        /// <param name="buffer">The data buffer in the channel message</param>
        /// <param name="waitAll">Wait a complete acknowledge from the peers</param>
        /// <param name="timestamp">timestamp to associated to the message</param>
        /// <returns></returns>
        OPERATION_RESULT WriteOnChannel(string key, byte[] buffer, bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        /// <summary>
        /// Reads data from channel
        /// </summary>
        /// <param name="offset">Position where read begins</param>
        /// <param name="length">Number of elements to read</param>
        /// <param name="buffer">Result buffer</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT ReadFromChannel(Int64 offset, long length, out byte[] buffer);
        /// <summary>
        /// Changes channel direction
        /// </summary>
        /// <param name="direction">New <see cref="DDM_CHANNEL_DIRECTION"/></param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction);
        /// <summary>
        /// Called when a data is available
        /// </summary>
        /// <param name="key">Message key</param>
        /// <param name="buffer">Message buffer</param>
        void OnDataAvailable(string key, byte[] buffer);
        /// <summary>
        /// Called when an event condition is raised from subsystem
        /// </summary>
        /// <param name="errorCode">The error code reported</param>
        /// <param name="nativeCode">The native code associated to the error if available</param>
        /// <param name="subSystemReason">A string with a reason from subsystem</param>
        void OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode, string subSystemReason);
        /// <summary>
        /// Event to receive messages when data are available
        /// </summary>
        event EventHandler<DataAvailableEventArgs> DataAvailable;
        /// <summary>
        /// Event to receive condition or errors
        /// </summary>
        event EventHandler<ConditionOrErrorEventArgs> ConditionOrError;
    }
    #endregion

    #region SmartDataDistributionChannel
    /// <summary>
    /// Main class managing channel
    /// </summary>
    public class SmartDataDistributionChannel : ISmartDataDistributionChannel, IDataDistributionChannelCallbackLow
    {
        /// <summary>
        /// No timestamp value
        /// </summary>
        public const Int64 DDM_NO_TIMESTAMP = -1;

        /// <summary>
        /// Ctor
        /// </summary>
        public SmartDataDistributionChannel()
        {
            m_DataDistributionChannelCallbackLow = new DataDistributionChannelCallbackLow(IntPtr.Zero, this);
        }
        /// <summary>
        /// Finalizer
        /// </summary>
        ~SmartDataDistributionChannel()
        {

        }
        /// <inheritdoc/>
        public OPERATION_RESULT StartChannel(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StartChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT StopChannel(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_StopChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <inheritdoc/>
        public void SetParameter(string paramName, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, paramName, paramValue);
        }
        /// <inheritdoc/>
        public void SetParameter(DDM_GENERAL_PARAMETER paramId, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, paramId, paramValue);
        }
        /// <inheritdoc/>
        public string GetParameter(string paramName)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, paramName);
        }
        /// <inheritdoc/>
        public string GetParameter(DDM_GENERAL_PARAMETER paramId)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, paramId);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT Lock(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Lock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, timeout);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT Unlock()
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Unlock>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT SeekChannel(Int64 position, DDM_SEEKKIND kind = DDM_SEEKKIND.ABSOLUTE)
        {
            return SeekChannel(position, DDM_SEEKCONTEXT.OFFSET, kind);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT SeekChannel(DateTime position)
        {
            long unixTime = ((DateTimeOffset)position).ToUnixTimeMilliseconds();
            return SeekChannel(unixTime, DDM_SEEKCONTEXT.TIMESTAMP, DDM_SEEKKIND.ABSOLUTE);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT SeekChannel(TimeSpan position)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SeekChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, (Int64)position.TotalMilliseconds, DDM_SEEKCONTEXT.TIMESTAMP, DDM_SEEKKIND.RELATIVE);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT SeekChannel(Int64 position, DDM_SEEKCONTEXT context = DDM_SEEKCONTEXT.OFFSET, DDM_SEEKKIND kind = DDM_SEEKKIND.ABSOLUTE)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SeekChannel>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, position, context, kind);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT WriteOnChannel(string value, bool waitAll = false, Int64 timestamp = DDM_NO_TIMESTAMP)
        {
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(value);
            return WriteOnChannel(null, buffer, waitAll, timestamp);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT WriteOnChannel(string key, string value, bool waitAll = false, Int64 timestamp = DDM_NO_TIMESTAMP)
        {
            byte[] buffer = System.Text.Encoding.UTF8.GetBytes(value);
            return WriteOnChannel(key, buffer, waitAll, timestamp);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT WriteOnChannel(byte[] buffer, bool waitAll = false, Int64 timestamp = DDM_NO_TIMESTAMP)
        {
            IntPtr unmanagedPointer = Marshal.AllocHGlobal(buffer.Length);
            try
            {
                Marshal.Copy(buffer, 0, unmanagedPointer, buffer.Length);
                // Call unmanaged code 
                return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_WriteOnChannel>().Invoke(
                    IDataDistributionSubsystemManager_ptr, channelHandle, null, IntPtr.Zero, unmanagedPointer, new IntPtr(buffer.Length), waitAll, timestamp);
            }
            finally
            {
                Marshal.FreeHGlobal(unmanagedPointer);
            }
        }
        /// <inheritdoc/>
        public OPERATION_RESULT WriteOnChannel(string key, byte[] buffer, bool waitAll = false, Int64 timestamp = DDM_NO_TIMESTAMP)
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
        /// <inheritdoc/>
        public OPERATION_RESULT ReadFromChannel(Int64 offset, long length, out byte[] buffer)
        {
            try
            {
                IntPtr dataBuffer = IntPtr.Zero;
                long dataLen = 0;
                buffer = null;
                // Call unmanaged code
                IntPtr len = new IntPtr(length);
                OPERATION_RESULT status = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_ReadFromChannel>().Invoke(
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
        /// <inheritdoc/>
        public OPERATION_RESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_ChangeChannelDirection>().Invoke(IDataDistributionSubsystemManager_ptr, channelHandle, direction);
        }
        /// <inheritdoc/>
        public virtual void OnDataAvailable(string key, byte[] buffer)
        {

        }
        /// <inheritdoc/>
        public virtual void OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode, string subSystemReason)
        {

        }

        /// <inheritdoc/>
        public string ChannelName { get { return channelName; } }
        /// <inheritdoc/>
        public Int64 Timestamp { get; private set; }
        /// <inheritdoc/>
        public DateTime DateTime { get { return DateTimeOffset.FromUnixTimeMilliseconds(Timestamp).DateTime; } }
        /// <inheritdoc/>
        public Int64 Offset { get; private set; }

        /// <inheritdoc/>
        public event EventHandler<DataAvailableEventArgs> DataAvailable;
        /// <inheritdoc/>
        public event EventHandler<ConditionOrErrorEventArgs> ConditionOrError;

        void IDataDistributionChannelCallbackLow.OnUnderlyingEvent(IntPtr opaque, IntPtr channelHandle, UnderlyingEvent uEvent)
        {
            if (uEvent.IsDataAvailable)
            {
                Timestamp = uEvent.Timestamp;
                Offset = uEvent.Offset;

                byte[] data = null;
                if (uEvent.Buffer != IntPtr.Zero)
                {
                    data = new byte[uEvent.BufferLength.ToInt64()];
                    Marshal.Copy(uEvent.Buffer, data, 0, data.Length);
                }

                string key = null;
                byte[] keyData = null;
                if (uEvent.Key != IntPtr.Zero)
                {
                    keyData = new byte[uEvent.KeyLen.ToInt64()];
                    Marshal.Copy(uEvent.Key, keyData, 0, keyData.Length);
                    try
                    {
                        key = Encoding.ASCII.GetString(keyData);
                    }
                    catch (Exception ex)
                    {
                        OnConditionOrError(OPERATION_RESULT.DDM_INVALID_DATA, 0, ex.Message);
                        key = null;
                    }
                }

                OnDataAvailable(key, data);
                DataAvailable?.Invoke(this, new DataAvailableEventArgs(this, key, data));
            }
            else
            {
                OnConditionOrError(uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason);
                ConditionOrError?.Invoke(this, new ConditionOrErrorEventArgs(this, uEvent.Condition, uEvent.NativeCode, uEvent.SubSystemReason));
            }
        }

        internal DDM_CHANNEL_DIRECTION m_direction = DDM_CHANNEL_DIRECTION.ALL;
        internal IntPtr channelHandle;
        internal string channelName;
        internal IntPtr IDataDistributionSubsystemManager_ptr;
        internal DataDistributionChannelCallbackLow m_DataDistributionChannelCallbackLow;
    }
    #endregion
}
