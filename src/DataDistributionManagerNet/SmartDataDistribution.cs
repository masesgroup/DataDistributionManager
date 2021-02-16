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
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace MASES.DataDistributionManager.Bindings
{
    #region ClusterHealthElement class
    /// <summary>
    /// Container of cluster instance information
    /// </summary>
    public class ClusterHealthElement
    {
        internal ClusterHealthElement(ClusterHealthElementStruct s)
        {
            this.Status = s.Status;
            this.Uptime = s.Uptime;
            this.LastContactTime = s.LastContactTime;
        }
        /// <summary>
        /// Instance status
        /// </summary>
        public DDM_INSTANCE_STATE Status { get; private set; }
        /// <summary>
        /// Instance uptime
        /// </summary>
        public Int64 Uptime { get; private set; }
        /// <summary>
        /// Last contact time of the instance
        /// </summary>
        public Int64 LastContactTime { get; private set; }
    };
    #endregion

    #region SmartDataDistribution class
    /// <summary>
    /// Main class to activate Data Distribution Manager
    /// </summary>
    public class SmartDataDistribution : IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow, ISmartDataDistributionMastership
    {
        /// <summary>
        /// Ctor
        /// </summary>
        public SmartDataDistribution()
        {
            IDataDistribution_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistribution_create>().Invoke();
        }
        /// <summary>
        /// Finalizer
        /// </summary>
        ~SmartDataDistribution()
        {

        }
        /// <summary>
        /// Initialize the instance using configuration file
        /// </summary>
        /// <param name="conf_file">Configuration file to use</param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Initialize(string conf_file, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            HRESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                conf_file, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }
        /// <summary>
        /// Initialize the instance using a configuration instance
        /// </summary>
        /// <param name="configuration">The configuration coming from an instance of <see cref="IConfiguration"/> </param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Initialize(IConfiguration configuration, string szMyAddress = null, string channelTrailer = null)
        {
            return Initialize(configuration.Configuration, szMyAddress, channelTrailer);
        }
        /// <summary>
        /// Initialize the instance using a set of key=value pairs
        /// </summary>
        /// <param name="arrayParams">array of key=value parameters</param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Initialize(string[] arrayParams, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            HRESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize2>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                arrayParams, arrayParams.Length, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }
        /// <summary>
        /// Request to allocate mastership manager
        /// </summary>
        /// <param name="serverName">The server name</param>
        /// <param name="parameters">Paramaters to send to underlying layer</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT RequestMastershipManager(string serverName, string[] parameters)
        {
            m_DataDistributionMastershipCallbackLow = new DataDistributionMastershipCallbackLow(IDataDistribution_ptr, this);

            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_RequestMastershipManager>().Invoke(
                    IDataDistribution_ptr, m_DataDistributionMastershipCallbackLow.Pointer, serverName, parameters, parameters.Length);
        }
        /// <summary>
        /// Returns <see cref="ISmartDataDistributionMastership"/> reference
        /// </summary>
        public ISmartDataDistributionMastership MastershipManager { get { return this; } }
        /// <summary>
        /// Starts the manager
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Start(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Start>().Invoke(IDataDistributionSubsystemManager_ptr, timeout);
        }
        /// <summary>
        /// Stops the manager
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="HRESULT"/></returns>
        public HRESULT Stop(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Stop>().Invoke(IDataDistributionSubsystemManager_ptr, timeout);
        }
        /// <summary>
        /// Return the protocol in use
        /// </summary>
        public string Protocol
        {
            get
            {
                IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetProtocol>().Invoke(IDataDistribution_ptr);
                if (ptr != IntPtr.Zero)
                {
                    return Marshal.PtrToStringAnsi(ptr);
                }
                return null;
            }
        }
        /// <summary>
        /// Return the protocol library in use
        /// </summary>
        public string ProtocolLib
        {
            get
            {
                IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetProtocolLib>().Invoke(IDataDistribution_ptr);
                if (ptr != IntPtr.Zero)
                {
                    return Marshal.PtrToStringAnsi(ptr);
                }
                return null;
            }
        }
        /// <summary>
        /// Return the mastership library in use
        /// </summary>
        public string MastershipLib
        {
            get
            {
                IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetMastershipLib>().Invoke(IDataDistribution_ptr);
                if (ptr != IntPtr.Zero)
                {
                    return Marshal.PtrToStringAnsi(ptr);
                }
                return null;
            }
        }
        /// <summary>
        /// Set global parameter
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        public void SetParameter(string paramName, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramName, paramValue);
        }
        /// <summary>
        /// Set global parameter
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        public void SetParameter(DDM_GENERAL_PARAMETER paramId, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramId, paramValue);
        }
        /// <summary>
        /// Get global parameter
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <returns>Parameter value</returns>
        public string GetParameter(string paramName)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramName);
        }
        /// <summary>
        /// Get global parameter
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to set</param>
        /// <returns>Parameter value</returns>
        public string GetParameter(DDM_GENERAL_PARAMETER paramId)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramId);
        }
        /// <summary>
        /// Creates a channel
        /// </summary>
        /// <typeparam name="T">A <see cref="Type"/> which inherits <see cref="SmartDataDistributionChannel"/></typeparam>
        /// <param name="channelName">The channel name</param>
        /// <param name="direction">The <see cref="DDM_CHANNEL_DIRECTION"/>. Default is <see cref="DDM_CHANNEL_DIRECTION.ALL"/></param>
        /// <param name="arrayParams">Specific parameters which override main parameters</param>
        /// <returns>An allocated instance of <typeparamref name="T"/></returns>
        public T CreateSmartChannel<T>(string channelName, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL, string[] arrayParams = null)
            where T : SmartDataDistributionChannel
        {
            T smartChannelReference = Activator.CreateInstance(typeof(T)) as T;

            IntPtr handle = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_CreateChannel>().Invoke(
                IDataDistributionSubsystemManager_ptr, channelName, smartChannelReference.m_DataDistributionChannelCallbackLow.Pointer, direction, arrayParams, (arrayParams != null) ? arrayParams.Length : 0);

            if (handle == IntPtr.Zero)
            {
                throw new InvalidOperationException("Failed to allocate a channel");
            }

            smartChannelReference.m_direction = direction;
            smartChannelReference.IDataDistributionSubsystemManager_ptr = IDataDistributionSubsystemManager_ptr;
            smartChannelReference.channelHandle = handle;

            return smartChannelReference;
        }

        /// <summary>
        /// Creates a channel with <see cref="IConfiguration"/>
        /// </summary>
        /// <typeparam name="T">A <see cref="Type"/> which inherits <see cref="SmartDataDistributionChannel"/></typeparam>
        /// <param name="channelName">The channel name</param>
        /// <param name="direction">The <see cref="DDM_CHANNEL_DIRECTION"/>. Default is <see cref="DDM_CHANNEL_DIRECTION.ALL"/></param>
        /// <param name="configuration">The configuration coming from an instance of <see cref="IConfiguration"/></param>
        /// <returns>An allocated instance of <typeparamref name="T"/></returns>
        public T CreateSmartChannel2<T>(string channelName, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL, IConfiguration configuration = null)
            where T : SmartDataDistributionChannel
        {
            return CreateSmartChannel<T>(channelName, direction, (configuration != null) ? configuration.Configuration : null);
        }

        #region IDataDistributionCallback
        string IDataDistributionCallbackLow.OnConfiguration(IntPtr IDataDistribution_nativePtr, string key, string value)
        {
            return this.OnConfiguration(IDataDistribution_nativePtr, key, value);
        }

        void IDataDistributionCallbackLow.OnLogging(IntPtr IDataDistribution_nativePtr, DDM_LOG_LEVEL level, string source, string function, string errStr)
        {
            this.OnLogging(IDataDistribution_nativePtr, level, source, function, errStr);
        }

        void IDataDistributionCallbackLow.OnCompletelyDisconnected(IntPtr IDataDistribution_nativePtr, string channelName, string reason)
        {
            this.OnCompletelyDisconnected(IDataDistribution_nativePtr, channelName, reason);
        }
        /// <summary>
        /// Called when a configuration parameter shall be checked
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="key">The parameter key</param>
        /// <param name="value">The parameter value</param>
        /// <returns>The new value associated to the <paramref name="key"/></returns>
        public virtual string OnConfiguration(IntPtr IDataDistribution_nativePtr, string key, string value)
        {
            return value;
        }
        /// <summary>
        /// Called when a log is emitted
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="level">Log level</param>
        /// <param name="source">The source of the log</param>
        /// <param name="function">The function</param>
        /// <param name="errStr">The log string</param>
        public virtual void OnLogging(IntPtr IDataDistribution_nativePtr, DDM_LOG_LEVEL level, string source, string function, string errStr)
        {

        }
        /// <summary>
        /// Called when a critical condition disconnects the instance from the server
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="channelName">The channel name</param>
        /// <param name="reason">Disconnection reason</param>
        public virtual void OnCompletelyDisconnected(IntPtr IDataDistribution_nativePtr, string channelName, string reason)
        {

        }
        #endregion

        #region IDataDistributionMastershipCallback
        /// <summary>
        /// The cluster has changed its state
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="change">New state</param>
        /// <param name="serverid">Server has changed state</param>
        public virtual void OnClusterStateChange(IntPtr IDataDistribution_nativePtr, DDM_CLUSTEREVENT change, Int64 serverid)
        {

        }
        /// <summary>
        /// My state has changed
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="newState">New state</param>
        /// <param name="oldState">Old state</param>
        public virtual void OnStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {

        }
        /// <summary>
        /// The state is ready to be used 
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="buffer">The buffer receiver</param>
        /// <param name="len">Length of the received buffer</param>
        public virtual void OnStateReady(IntPtr IDataDistribution_nativePtr, IntPtr buffer, Int64 len)
        {

        }
        /// <summary>
        /// Called when a state transfer was requested
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="buffer">Pointer to the buffer</param>
        /// <param name="len">Length of the buffer</param>
        public virtual void OnRequestedState(IntPtr IDataDistribution_nativePtr, IntPtr buffer, IntPtr len)
        {

        }
        /// <summary>
        /// There are multiple primary server in the cluster
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="myId">My identifier</param>
        /// <param name="otherId">Other identifier which is primary</param>
        public virtual void OnMultiplePrimary(IntPtr IDataDistribution_nativePtr, Int64 myId, Int64 otherId)
        {

        }
        /// <summary>
        /// Called the first time there is state change
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="newState">New state</param>
        public virtual void FirstStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState)
        {

        }
        /// <summary>
        /// The state is starting to change
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="oldState">Old state</param>
        /// <param name="newState">New state</param>
        public virtual void ChangingState(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {

        }
        /// <summary>
        /// The state has changed
        /// </summary>
        /// <param name="IDataDistribution_nativePtr">The pointer to native class pointer</param>
        /// <param name="newState">New state</param>
        public virtual void ChangedState(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState)
        {

        }

        #endregion

        #region ISmartDataDistributionMastership

        HRESULT ISmartDataDistributionMastership.Start(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_Start>().Invoke(IDataDistribution_ptr, dwMilliseconds);
        }

        HRESULT ISmartDataDistributionMastership.Stop(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_Stop>().Invoke(IDataDistribution_ptr, dwMilliseconds);
        }

        void ISmartDataDistributionMastership.ChangeMyState(DDM_INSTANCE_STATE newState)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_ChangeMyState>().Invoke(IDataDistribution_ptr, newState);
        }

        void ISmartDataDistributionMastership.ChangeState(Int64 instanceId, DDM_INSTANCE_STATE newState)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_ChangeState>().Invoke(IDataDistribution_ptr, instanceId, newState);
        }

        bool ISmartDataDistributionMastership.RequestIAmNextPrimary()
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_RequestIAmNextPrimary>().Invoke(IDataDistribution_ptr);
        }

        bool ISmartDataDistributionMastership.IamNextPrimary
        {
            get
            {
                return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetIamNextPrimary>().Invoke(IDataDistribution_ptr);
            }
        }

        IDictionary<Int64, DDM_INSTANCE_STATE> ISmartDataDistributionMastership.ClusterState
        {
            get
            {
                IDictionary<Int64, DDM_INSTANCE_STATE> result = new Dictionary<Int64, DDM_INSTANCE_STATE>();

                Int64[] arrayElements = null;
                IntPtr length = IntPtr.Zero;

                DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetClusterIndexes>().Invoke(IDataDistribution_ptr, arrayElements, length);

                if (length != IntPtr.Zero && arrayElements != null)
                {
                    foreach (var item in arrayElements)
                    {
                        var res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetStateOf>().Invoke(IDataDistribution_ptr, item);
                        result.Add(item, res);
                    }
                }

                return result;
            }
        }

        IDictionary<Int64, ClusterHealthElement> ISmartDataDistributionMastership.ClusterHealth
        {
            get
            {
                IDictionary<Int64, ClusterHealthElement> result = new Dictionary<Int64, ClusterHealthElement>();

                Int64[] arrayElements = null;
                IntPtr length = IntPtr.Zero;

                DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetClusterIndexes>().Invoke(IDataDistribution_ptr, arrayElements, length);

                if (length != IntPtr.Zero && arrayElements != null)
                {
                    foreach (var item in arrayElements)
                    {
                        var res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetClusterHealth>().Invoke(IDataDistribution_ptr, item);
                        var structure = Marshal.PtrToStructure<ClusterHealthElementStruct>(res);
                        result.Add(item, new ClusterHealthElement(structure));
                    }
                }

                return result;
            }
        }

        DDM_INSTANCE_STATE ISmartDataDistributionMastership.MyState
        {
            get
            {
                return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetMyState>().Invoke(IDataDistribution_ptr);
            }
        }

        Int64 ISmartDataDistributionMastership.LocalServerId
        {
            get
            {
                Int64 res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetLocalServerId>().Invoke(IDataDistribution_ptr);
                return res;
            }
        }

        Int64 ISmartDataDistributionMastership.PrimaryServerId
        {
            get
            {
                Int64 res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetPrimaryServerId>().Invoke(IDataDistribution_ptr);
                return res;
            }
        }

        Int64 ISmartDataDistributionMastership.MessageDelay
        {
            get
            {
                Int64 res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetMessageDelay>().Invoke(IDataDistribution_ptr);
                return res;
            }
        }

        Int64 ISmartDataDistributionMastership.UpTime
        {
            get
            {
                Int64 res = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetUpTime>().Invoke(IDataDistribution_ptr);
                return res;
            }
        }

        #endregion

        #region Private members
        DataDistributionCallbackLow m_DataDistributionCallbackLow;
        DataDistributionMastershipCallbackLow m_DataDistributionMastershipCallbackLow;
        IntPtr IDataDistribution_ptr;
        IntPtr IDataDistributionSubsystemManager_ptr;
        #endregion
    }
    #endregion
}
