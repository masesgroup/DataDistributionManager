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
            this.ServerId = s.ServerId;
            this.Status = s.Status;
            this.Uptime = s.Uptime;
            this.LastContactTime = s.LastContactTime;
        }
        /// <summary>
        /// Instance status
        /// </summary>
        public Int64 ServerId { get; private set; }
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

    #region ConfigurationEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.ConfigurationEvent"/> event
    /// </summary>
    public class ConfigurationEventArgs : EventArgs
    {
        internal ConfigurationEventArgs(string channelName, string key, string value)
        {
            ChannelName = channelName;
            Key = key;
            Value = value;
        }
        /// <summary>
        /// The configuration key
        /// </summary>
        public string ChannelName { get; private set; }
        /// <summary>
        /// The configuration key
        /// </summary>
        public string Key { get; private set; }
        /// <summary>
        /// The value received, update it to send back the new value
        /// </summary>
        public string Value { get; set; }
    }
    #endregion

    #region LoggingEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.LoggingEvent"/> event
    /// </summary>
    public class LoggingEventArgs : EventArgs
    {
        internal LoggingEventArgs(DDM_LOG_LEVEL level, string source, string function, string logStr)
        {
            Level = level;
            Source = source;
            Function = function;
            LogString = logStr;
        }
        /// <summary>
        /// Log level <see cref="DDM_LOG_LEVEL"/>
        /// </summary>
        public DDM_LOG_LEVEL Level { get; private set; }
        /// <summary>
        /// Source of log
        /// </summary>
        public string Source { get; private set; }
        /// <summary>
        /// Function source of log
        /// </summary>
        public string Function { get; private set; }
        /// <summary>
        /// Log string reported
        /// </summary>
        public string LogString { get; private set; }
    }
    #endregion 

    #region CompletelyDisconnectedEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.CompletelyDisconnectedEvent"/> event
    /// </summary>
    public class CompletelyDisconnectedEventArgs : EventArgs
    {
        internal CompletelyDisconnectedEventArgs(string channelName, string reason)
        {
            ChannelName = channelName;
            Reason = reason;
        }
        /// <summary>
        /// The disconnected channel
        /// </summary>
        public string ChannelName { get; private set; }
        /// <summary>
        /// The disconnection reason
        /// </summary>
        public string Reason { get; private set; }
    }
    #endregion

    #region ClusterStateChangeEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.ClusterStateChangeEvent"/> event
    /// </summary>
    public class ClusterStateChangeEventArgs : EventArgs
    {
        internal ClusterStateChangeEventArgs(DDM_CLUSTEREVENT state, Int64 serverid)
        {
            State = state;
            Serverid = serverid;
        }
        /// <summary>
        /// The state <see cref="DDM_CLUSTEREVENT"/> of server <see cref="Serverid"/>
        /// </summary>
        public DDM_CLUSTEREVENT State { get; private set; }
        /// <summary>
        /// The is of the server changed its state
        /// </summary>
        public Int64 Serverid { get; private set; }
    }
    #endregion

    #region StateChangeEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.StateChangeEvent"/> event
    /// </summary>
    public class StateChangeEventArgs : EventArgs
    {
        internal StateChangeEventArgs(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {
            NewState = newState;
            OldState = oldState;
        }
        /// <summary>
        /// The new state <see cref="DDM_INSTANCE_STATE"/>
        /// </summary>
        public DDM_INSTANCE_STATE NewState { get; private set; }
        /// <summary>
        /// The old state <see cref="DDM_INSTANCE_STATE"/>
        /// </summary>
        public DDM_INSTANCE_STATE OldState { get; private set; }
    }
    #endregion

    #region StateReadyEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.StateReadyEvent"/> event
    /// </summary>
    public class StateReadyEventArgs : EventArgs
    {
        internal StateReadyEventArgs(IntPtr buffer, Int64 len)
        {
            NativeBuffer = buffer;
            Length = len;
        }
        /// <summary>
        /// The native pointer of the buffer
        /// </summary>
        public IntPtr NativeBuffer { get; private set; }
        /// <summary>
        /// The length of the received buffer
        /// </summary>
        public Int64 Length { get; private set; }
        /// <summary>
        /// The managed buffer received
        /// </summary>
        public byte[] Buffer
        {
            get
            {
                throw new NotImplementedException();
            }
        }
    }
    #endregion

    #region RequestedStateEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.RequestedStateEvent"/> event
    /// </summary>
    public class RequestedStateEventArgs : EventArgs
    {
        internal RequestedStateEventArgs(IntPtr buffer, IntPtr len)
        {
            NativeBuffer = buffer;
            Length = len;
        }
        /// <summary>
        /// The native pointer of the buffer
        /// </summary>
        public IntPtr NativeBuffer { get; set; }
        /// <summary>
        /// The length of the received buffer
        /// </summary>
        public IntPtr Length { get; set; }
    }
    #endregion

    #region MultiplePrimaryEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.MultiplePrimaryEvent"/> event
    /// </summary>
    public class MultiplePrimaryEventArgs : EventArgs
    {
        internal MultiplePrimaryEventArgs(Int64 myId, Int64 otherId)
        {
            MyServerId = myId;
            OtherServerId = otherId;
        }
        /// <summary>
        /// My server id
        /// </summary>
        public Int64 MyServerId { get; private set; }
        /// <summary>
        /// The id of the other server in primary state
        /// </summary>
        public Int64 OtherServerId { get; private set; }
    }
    #endregion

    #region FirstStateChangeEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.FirstStateChangeEvent"/> event
    /// </summary>
    public class FirstStateChangeEventArgs : EventArgs
    {
        internal FirstStateChangeEventArgs(DDM_INSTANCE_STATE state)
        {
            State = state;
        }
        /// <summary>
        /// The state on start-up
        /// </summary>
        public DDM_INSTANCE_STATE State { get; private set; }
    }
    #endregion

    #region ChangingStateEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.ChangingStateEvent"/> event
    /// </summary>
    public class ChangingStateEventArgs : EventArgs
    {
        internal ChangingStateEventArgs(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {
            ActualState = oldState;
            FutureState = newState;
        }
        /// <summary>
        /// The actual <see cref="DDM_INSTANCE_STATE"/> state
        /// </summary>
        public DDM_INSTANCE_STATE ActualState { get; private set; }
        /// <summary>
        /// The future <see cref="DDM_INSTANCE_STATE"/> state
        /// </summary>
        public DDM_INSTANCE_STATE FutureState { get; private set; }
    }
    #endregion

    #region ChangedStateEventArgs
    /// <summary>
    /// Event args for <see cref="ISmartDataDistribution.ChangedStateEvent"/> event
    /// </summary>
    public class ChangedStateEventArgs : EventArgs
    {
        internal ChangedStateEventArgs(DDM_INSTANCE_STATE state)
        {
            State = state;
        }
        /// <summary>
        /// The final <see cref="DDM_INSTANCE_STATE"/> state
        /// </summary>
        public DDM_INSTANCE_STATE State { get; private set; }
    }
    #endregion

    #region ISmartDataDistribution
    /// <summary>
    /// Interface to interact with underlying system
    /// </summary>
    public interface ISmartDataDistribution
    {
        #region Methods
        /// <summary>
        /// Initialize the instance using configuration file
        /// </summary>
        /// <param name="conf_file">Configuration file to use</param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Initialize(string conf_file, string szMyAddress = null, string channelTrailer = null);
        /// <summary>
        /// Initialize the instance using a configuration instance
        /// </summary>
        /// <param name="configuration">The configuration coming from an instance of <see cref="IConfiguration"/> </param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Initialize(IConfiguration configuration, string szMyAddress = null, string channelTrailer = null);
        /// <summary>
        /// Initialize the instance using a set of key=value pairs
        /// </summary>
        /// <param name="arrayParams">array of key=value parameters</param>
        /// <param name="szMyAddress">The name of the server hosting the process</param>
        /// <param name="channelTrailer">Trailer string to append on channel names</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Initialize(string[] arrayParams, string szMyAddress = null, string channelTrailer = null);
        /// <summary>
        /// Request to allocate mastership manager
        /// </summary>
        /// <param name="serverName">The server name</param>
        /// <param name="parameters">The configuration coming from an instance of <see cref="IConfiguration"/></param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT RequestMastershipManager(IConfiguration parameters = null, string serverName = null );
        /// <summary>
        /// Request to allocate mastership manager
        /// </summary>
        /// <param name="serverName">The server name</param>
        /// <param name="parameters">Paramaters to send to underlying layer</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT RequestMastershipManager(string[] parameters, string serverName = null);
        /// <summary>
        /// Returns <see cref="ISmartDataDistributionMastership"/> reference
        /// </summary>
        ISmartDataDistributionMastership MastershipManager { get; }
        /// <summary>
        /// Starts the manager
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Start(uint timeout);
        /// <summary>
        /// Stops the manager
        /// </summary>
        /// <param name="timeout">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Stop(uint timeout);
        /// <summary>
        /// Return the protocol in use
        /// </summary>
        string Protocol { get; }
        /// <summary>
        /// Return the protocol library in use
        /// </summary>
        string ProtocolLib { get; }
        /// <summary>
        /// Return the mastership library in use
        /// </summary>
        string MastershipLib { get; }
        /// <summary>
        /// Set global parameter
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        void SetParameter(string paramName, string paramValue);
        /// <summary>
        /// Set global parameter
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        void SetParameter(DDM_GENERAL_PARAMETER paramId, string paramValue);
        /// <summary>
        /// Get global parameter
        /// </summary>
        /// <param name="paramName">Parameter to get</param>
        /// <returns>Parameter value</returns>
        string GetParameter(string paramName);
        /// <summary>
        /// Get global parameter
        /// </summary>
        /// <param name="paramId"><see cref="DDM_GENERAL_PARAMETER"/> parameter to get</param>
        /// <returns>Parameter value</returns>
        string GetParameter(DDM_GENERAL_PARAMETER paramId);
        /// <summary>
        /// Creates a channel
        /// </summary>
        /// <typeparam name="T">A <see cref="Type"/> which inherits <see cref="SmartDataDistributionChannel"/></typeparam>
        /// <param name="channelName">The channel name</param>
        /// <param name="direction">The <see cref="DDM_CHANNEL_DIRECTION"/>. Default is <see cref="DDM_CHANNEL_DIRECTION.ALL"/></param>
        /// <param name="arrayParams">Specific parameters which override main parameters</param>
        /// <returns>An allocated instance of <typeparamref name="T"/></returns>
        T CreateSmartChannel<T>(string channelName, string[] arrayParams, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL)
            where T : SmartDataDistributionChannel;
        /// <summary>
        /// Creates a channel with <see cref="IConfiguration"/>
        /// </summary>
        /// <typeparam name="T">A <see cref="Type"/> which inherits <see cref="SmartDataDistributionChannel"/></typeparam>
        /// <param name="channelName">The channel name</param>
        /// <param name="direction">The <see cref="DDM_CHANNEL_DIRECTION"/>. Default is <see cref="DDM_CHANNEL_DIRECTION.ALL"/></param>
        /// <param name="configuration">The configuration coming from an instance of <see cref="IConfiguration"/></param>
        /// <returns>An allocated instance of <typeparamref name="T"/></returns>
        T CreateSmartChannel<T>(string channelName, IConfiguration configuration = null, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL)
            where T : SmartDataDistributionChannel;

        #endregion

        #region IDataDistributionCallback
        /// <summary>
        /// Called when a configuration parameter shall be checked
        /// </summary>
        /// <param name="channelName">The channel requesting. null for global parameters</param>
        /// <param name="key">The parameter key</param>
        /// <param name="value">The parameter value</param>
        /// <returns>The new value associated to the <paramref name="key"/></returns>
        string OnConfiguration(string channelName, string key, string value);
        /// <summary>
        /// Called when a log is emitted
        /// </summary>
        /// <param name="level">Log level</param>
        /// <param name="source">The source of the log</param>
        /// <param name="function">The function</param>
        /// <param name="errStr">The log string</param>
        void OnLogging(DDM_LOG_LEVEL level, string source, string function, string errStr);
        /// <summary>
        /// Called when a critical condition disconnects the instance from the server
        /// </summary>
        /// <param name="channelName">The channel name</param>
        /// <param name="reason">Disconnection reason</param>
        void OnCompletelyDisconnected(string channelName, string reason);

        #region Events
        /// <summary>
        /// Event received during configuration parameter load on subsystem
        /// </summary>
        event EventHandler<ConfigurationEventArgs> ConfigurationEvent;
        /// <summary>
        /// Event received when a log is received from subsystem
        /// </summary>
        event EventHandler<LoggingEventArgs> LoggingEvent;
        /// <summary>
        /// Event received when subsystem detects a complete disconnection from central server or other peers
        /// </summary>
        event EventHandler<CompletelyDisconnectedEventArgs> CompletelyDisconnectedEvent;
        #endregion

        #endregion

        #region IDataDistributionMastershipCallback
        /// <summary>
        /// The cluster has changed its state
        /// </summary>
        /// <param name="change">New cluster event</param>
        /// <param name="serverid">Server has emitted event</param>
        void OnClusterStateChange(DDM_CLUSTEREVENT change, Int64 serverid);
        /// <summary>
        /// My state has changed
        /// </summary>
        /// <param name="newState">New state</param>
        /// <param name="oldState">Old state</param>
        void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
        /// <summary>
        /// The state is ready to be used 
        /// </summary>
        /// <param name="buffer">The buffer receiver</param>
        /// <param name="len">Length of the received buffer</param>
        void OnStateReady(IntPtr buffer, Int64 len);
        /// <summary>
        /// Called when a state transfer was requested
        /// </summary>
        /// <param name="buffer">Pointer to the buffer</param>
        /// <param name="len">Length of the buffer</param>
        void OnRequestedState(IntPtr buffer, IntPtr len);
        /// <summary>
        /// There are multiple primary server in the cluster
        /// </summary>
        /// <param name="myId">My identifier</param>
        /// <param name="otherId">Other identifier which is primary</param>
        void OnMultiplePrimary(Int64 myId, Int64 otherId);
        /// <summary>
        /// Called the first time there is state change
        /// </summary>
        /// <param name="newState">New state</param>
        void OnFirstStateChange(DDM_INSTANCE_STATE newState);
        /// <summary>
        /// The state is starting to change
        /// </summary>
        /// <param name="oldState">Old state</param>
        /// <param name="newState">New state</param>
        void OnChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);
        /// <summary>
        /// The state has changed
        /// </summary>
        /// <param name="newState">New state</param>
        void OnChangedState(DDM_INSTANCE_STATE newState);

        #region Events
        /// <summary>
        /// The cluster has changed its state
        /// </summary>
        event EventHandler<ClusterStateChangeEventArgs> ClusterStateChangeEvent;
        /// <summary>
        /// My state has changed
        /// </summary>
        event EventHandler<StateChangeEventArgs> StateChangeEvent;
        /// <summary>
        /// The state is ready to be used 
        /// </summary>
        event EventHandler<StateReadyEventArgs> StateReadyEvent;
        /// <summary>
        /// Called when a state transfer was requested
        /// </summary>
        event EventHandler<RequestedStateEventArgs> RequestedStateEvent;
        /// <summary>
        /// There are multiple primary server in the cluster
        /// </summary>
        event EventHandler<MultiplePrimaryEventArgs> MultiplePrimaryEvent;
        /// <summary>
        /// Called the first time there is state change
        /// </summary>
        event EventHandler<FirstStateChangeEventArgs> FirstStateChangeEvent;
        /// <summary>
        /// The state is starting to change
        /// </summary>
        event EventHandler<ChangingStateEventArgs> ChangingStateEvent;
        /// <summary>
        /// The state has changed
        /// </summary>
        event EventHandler<ChangedStateEventArgs> ChangedStateEvent;
        #endregion

        #endregion
    }
    #endregion

    #region SmartDataDistribution class
    /// <summary>
    /// Main class to activate Data Distribution Manager
    /// </summary>
    public class SmartDataDistribution : ISmartDataDistribution, IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow, ISmartDataDistributionMastership
    {
        /// <summary>
        /// Initialize a new <see cref="SmartDataDistribution"/>
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
        /// <inheritdoc/>
        public OPERATION_RESULT Initialize(string conf_file, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            OPERATION_RESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                conf_file, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }
        /// <inheritdoc/>
        public OPERATION_RESULT Initialize(IConfiguration configuration, string szMyAddress = null, string channelTrailer = null)
        {
            return Initialize(configuration.Configuration, szMyAddress, channelTrailer);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT Initialize(string[] arrayParams, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            OPERATION_RESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize2>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                arrayParams, arrayParams.Length, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }
        /// <inheritdoc/>
        public OPERATION_RESULT RequestMastershipManager(IConfiguration parameters = null, string serverName = null)
        {
            return RequestMastershipManager(parameters != null ? parameters.Configuration : null, serverName);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT RequestMastershipManager(string[] parameters, string serverName = null)
        {
            m_DataDistributionMastershipCallbackLow = new DataDistributionMastershipCallbackLow(IDataDistribution_ptr, this);

            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_RequestMastershipManager>().Invoke(
                    IDataDistribution_ptr, m_DataDistributionMastershipCallbackLow.Pointer, serverName, parameters, parameters.Length);
        }
        /// <inheritdoc/>
        public ISmartDataDistributionMastership MastershipManager { get { return this; } }
        /// <inheritdoc/>
        public OPERATION_RESULT Start(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Start>().Invoke(IDataDistributionSubsystemManager_ptr, timeout);
        }
        /// <inheritdoc/>
        public OPERATION_RESULT Stop(uint timeout)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Stop>().Invoke(IDataDistributionSubsystemManager_ptr, timeout);
        }
        /// <inheritdoc/>
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
        /// <inheritdoc/>
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
        /// <inheritdoc/>
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
        /// <inheritdoc/>
        public void SetParameter(string paramName, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramName, paramValue);
        }
        /// <inheritdoc/>
        public void SetParameter(DDM_GENERAL_PARAMETER paramId, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_SetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramId, paramValue);
        }
        /// <inheritdoc/>
        public string GetParameter(string paramName)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramName);
        }
        /// <inheritdoc/>
        public string GetParameter(DDM_GENERAL_PARAMETER paramId)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_GetParameter2>().Invoke(IDataDistributionSubsystemManager_ptr, IntPtr.Zero, paramId);
        }
        /// <inheritdoc/>
        public T CreateSmartChannel<T>(string channelName, string[] arrayParams, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL)
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

        /// <inheritdoc/>
        public T CreateSmartChannel<T>(string channelName, IConfiguration configuration = null, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL)
            where T : SmartDataDistributionChannel
        {
            return CreateSmartChannel<T>(channelName, (configuration != null) ? configuration.Configuration : null, direction);
        }

        #region IDataDistributionCallback
        string IDataDistributionCallbackLow.OnConfiguration(IntPtr IDataDistribution_nativePtr, string channelName, string key, string value)
        {
            if (ConfigurationEvent != null)
            {
                var args = new ConfigurationEventArgs(channelName, key, value);
                ConfigurationEvent(this, args);
                return args.Value;
            }
            else return this.OnConfiguration(channelName, key, value);
        }

        void IDataDistributionCallbackLow.OnLogging(IntPtr IDataDistribution_nativePtr, DDM_LOG_LEVEL level, string source, string function, string errStr)
        {
            this.OnLogging(level, source, function, errStr);
            LoggingEvent?.Invoke(this, new LoggingEventArgs(level, source, function, errStr));
        }

        void IDataDistributionCallbackLow.OnCompletelyDisconnected(IntPtr IDataDistribution_nativePtr, string channelName, string reason)
        {
            this.OnCompletelyDisconnected(channelName, reason);
            CompletelyDisconnectedEvent?.Invoke(this, new CompletelyDisconnectedEventArgs(channelName, reason));
        }
        /// <inheritdoc/>
        public virtual string OnConfiguration(string channelName, string key, string value)
        {
            return value;
        }
        /// <inheritdoc/>
        public virtual void OnLogging(DDM_LOG_LEVEL level, string source, string function, string errStr)
        {

        }
        /// <inheritdoc/>
        public virtual void OnCompletelyDisconnected(string channelName, string reason)
        {

        }

        #region Events
        /// <inheritdoc/>
        public event EventHandler<ConfigurationEventArgs> ConfigurationEvent;
        /// <inheritdoc/>
        public event EventHandler<LoggingEventArgs> LoggingEvent;
        /// <inheritdoc/>
        public event EventHandler<CompletelyDisconnectedEventArgs> CompletelyDisconnectedEvent;
        #endregion

        #endregion

        #region IDataDistributionMastershipCallback

        void IDataDistributionMastershipCallbackLow.OnClusterStateChange(IntPtr IDataDistribution_nativePtr, DDM_CLUSTEREVENT change, Int64 serverid)
        {
            OnClusterStateChange(change, serverid);
            ClusterStateChangeEvent?.Invoke(this, new ClusterStateChangeEventArgs(change, serverid));
        }

        void IDataDistributionMastershipCallbackLow.OnStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {
            OnStateChange(newState, oldState);
            StateChangeEvent?.Invoke(this, new StateChangeEventArgs(newState, oldState));
        }

        void IDataDistributionMastershipCallbackLow.OnStateReady(IntPtr IDataDistribution_nativePtr, IntPtr buffer, Int64 len)
        {
            OnStateReady(buffer, len);
            StateReadyEvent?.Invoke(this, new StateReadyEventArgs(buffer, len));
        }

        void IDataDistributionMastershipCallbackLow.OnRequestedState(IntPtr IDataDistribution_nativePtr, IntPtr buffer, IntPtr len)
        {
            OnRequestedState(buffer, len);
            RequestedStateEvent?.Invoke(this, new RequestedStateEventArgs(buffer, len));
        }

        void IDataDistributionMastershipCallbackLow.OnMultiplePrimary(IntPtr IDataDistribution_nativePtr, Int64 myId, Int64 otherId)
        {
            OnMultiplePrimary(myId, otherId);
            MultiplePrimaryEvent?.Invoke(this, new MultiplePrimaryEventArgs(myId, otherId));
        }

        void IDataDistributionMastershipCallbackLow.FirstStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState)
        {
            OnFirstStateChange(newState);
            FirstStateChangeEvent?.Invoke(this, new FirstStateChangeEventArgs(newState));
        }

        void IDataDistributionMastershipCallbackLow.ChangingState(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {
            OnChangingState(oldState, newState);
            ChangingStateEvent?.Invoke(this, new ChangingStateEventArgs(oldState, newState));
        }

        void IDataDistributionMastershipCallbackLow.ChangedState(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState)
        {
            OnChangedState(newState);
            ChangedStateEvent?.Invoke(this, new ChangedStateEventArgs(newState));
        }

        /// <summary>
        /// The cluster has changed its state
        /// </summary>
        /// <param name="change">New state</param>
        /// <param name="serverid">Server has changed state</param>
        public virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, Int64 serverid)
        {

        }
        /// <summary>
        /// My state has changed
        /// </summary>
        /// <param name="newState">New state</param>
        /// <param name="oldState">Old state</param>
        public virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {

        }
        /// <summary>
        /// The state is ready to be used 
        /// </summary>
        /// <param name="buffer">The buffer receiver</param>
        /// <param name="len">Length of the received buffer</param>
        public virtual void OnStateReady(IntPtr buffer, Int64 len)
        {

        }
        /// <summary>
        /// Called when a state transfer was requested
        /// </summary>
        /// <param name="buffer">Pointer to the buffer</param>
        /// <param name="len">Length of the buffer</param>
        public virtual void OnRequestedState(IntPtr buffer, IntPtr len)
        {

        }
        /// <summary>
        /// There are multiple primary server in the cluster
        /// </summary>
        /// <param name="myId">My identifier</param>
        /// <param name="otherId">Other identifier which is primary</param>
        public virtual void OnMultiplePrimary(Int64 myId, Int64 otherId)
        {

        }
        /// <summary>
        /// Called the first time there is state change
        /// </summary>
        /// <param name="newState">New state</param>
        public virtual void OnFirstStateChange(DDM_INSTANCE_STATE newState)
        {

        }
        /// <summary>
        /// The state is starting to change
        /// </summary>
        /// <param name="oldState">Old state</param>
        /// <param name="newState">New state</param>
        public virtual void OnChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {

        }
        /// <summary>
        /// The state has changed
        /// </summary>
        /// <param name="newState">New state</param>
        public virtual void OnChangedState(DDM_INSTANCE_STATE newState)
        {

        }

        #region Events
        /// <summary>
        /// The cluster has changed its state
        /// </summary>
        public event EventHandler<ClusterStateChangeEventArgs> ClusterStateChangeEvent;
        /// <summary>
        /// My state has changed
        /// </summary>
        public event EventHandler<StateChangeEventArgs> StateChangeEvent;
        /// <summary>
        /// The state is ready to be used 
        /// </summary>
        public event EventHandler<StateReadyEventArgs> StateReadyEvent;
        /// <summary>
        /// Called when a state transfer was requested
        /// </summary>
        public event EventHandler<RequestedStateEventArgs> RequestedStateEvent;
        /// <summary>
        /// There are multiple primary server in the cluster
        /// </summary>
        public event EventHandler<MultiplePrimaryEventArgs> MultiplePrimaryEvent;
        /// <summary>
        /// Called the first time there is state change
        /// </summary>
        public event EventHandler<FirstStateChangeEventArgs> FirstStateChangeEvent;
        /// <summary>
        /// The state is starting to change
        /// </summary>
        public event EventHandler<ChangingStateEventArgs> ChangingStateEvent;
        /// <summary>
        /// The state has changed
        /// </summary>
        public event EventHandler<ChangedStateEventArgs> ChangedStateEvent;
        #endregion

        #endregion

        #region ISmartDataDistributionMastership

        void ISmartDataDistributionMastership.SetParameter(string paramName, string paramValue)
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_SetParameter>().Invoke(IDataDistribution_ptr, paramName, paramValue);
        }

        string ISmartDataDistributionMastership.GetParameter(string paramName)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetParameter>().Invoke(IDataDistribution_ptr, paramName);
        }

        OPERATION_RESULT ISmartDataDistributionMastership.Start(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_Start>().Invoke(IDataDistribution_ptr, dwMilliseconds);
        }

        OPERATION_RESULT ISmartDataDistributionMastership.Stop(uint dwMilliseconds)
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

                IntPtr length = IntPtr.Zero;

                Int64[] arrayElements = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetClusterIndexes>().Invoke(IDataDistribution_ptr, length);

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

                IntPtr length = IntPtr.Zero;

                Int64[] arrayElements = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionMastershipCommon_GetClusterIndexes>().Invoke(IDataDistribution_ptr, length);

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
