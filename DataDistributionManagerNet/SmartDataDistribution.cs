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
    public class ClusterHealthElement
    {
        internal ClusterHealthElement(ClusterHealthElementStruct s)
        {
            this.Status = s.Status;
            this.Uptime = s.Uptime;
            this.LastContactTime = s.LastContactTime;
        }

        public DDM_INSTANCE_STATE Status { get; private set; }
        public Int64 Uptime { get; private set; }
        public Int64 LastContactTime { get; private set; }
    };


    public class SmartDataDistribution : IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow, ISmartDataDistributionMastership
    {
        public SmartDataDistribution()
        {
            IDataDistribution_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistribution_create>().Invoke();
        }

        ~SmartDataDistribution()
        {

        }

        public HRESULT Initialize(string conf_file, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            HRESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                conf_file, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }

        public HRESULT Initialize(string[] arrayParams, string szMyAddress = null, string channelTrailer = null)
        {
            m_DataDistributionCallbackLow = new DataDistributionCallbackLow(IDataDistribution_ptr, this);

            HRESULT result = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_Initialize2>().Invoke(
                IDataDistribution_ptr, m_DataDistributionCallbackLow.Pointer,
                arrayParams, arrayParams.Length, szMyAddress, channelTrailer);

            if (result.Succeeded) IDataDistributionSubsystemManager_ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetSubsystemManager>().Invoke(IDataDistribution_ptr);
            return result;
        }

        public HRESULT RequestMastershipManager(string serverName, string[] parameters)
        {
            m_DataDistributionMastershipCallbackLow = new DataDistributionMastershipCallbackLow(IDataDistribution_ptr, this);

            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_RequestMastershipManager>().Invoke(
                    IDataDistribution_ptr, m_DataDistributionMastershipCallbackLow.Pointer, serverName, parameters, parameters.Length);
        }

        public ISmartDataDistributionMastership MastershipManager { get { return this; } }

        public HRESULT Start(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Start>().Invoke(IDataDistributionSubsystemManager_ptr, dwMilliseconds);
        }

        public HRESULT Stop(uint dwMilliseconds)
        {
            return DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_Stop>().Invoke(IDataDistributionSubsystemManager_ptr, dwMilliseconds);
        }

        public string GetProtocol()
        {
            IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetProtocol>().Invoke(IDataDistribution_ptr);
            if (ptr != IntPtr.Zero)
            {
                return Marshal.PtrToStringAnsi(ptr);
            }
            return null;
        }

        public string GetProtocolLib()
        {
            IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetProtocolLib>().Invoke(IDataDistribution_ptr);
            if (ptr != IntPtr.Zero)
            {
                return Marshal.PtrToStringAnsi(ptr);
            }
            return null;
        }

        public string GetMastershipLib()
        {
            IntPtr ptr = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistribution_GetMastershipLib>().Invoke(IDataDistribution_ptr);
            if (ptr != IntPtr.Zero)
            {
                return Marshal.PtrToStringAnsi(ptr);
            }
            return null;
        }

        public T CreateSmartChannel<T>(string channelName, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.ALL, string[] arrayParams = null)
            where T : SmartDataDistributionChannel
        {
            T smartChannelReference = Activator.CreateInstance(typeof(T)) as T;

            IntPtr handle = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<IDataDistributionSubsystem_CreateChannel>().Invoke(
                IDataDistributionSubsystemManager_ptr, channelName, smartChannelReference.m_DataDistributionChannelCallbackLow.Pointer, direction, arrayParams, (arrayParams != null) ? arrayParams.Length : 0);

            smartChannelReference.m_direction = direction;
            smartChannelReference.IDataDistributionSubsystemManager_ptr = IDataDistributionSubsystemManager_ptr;
            smartChannelReference.channelHandle = handle;

            return smartChannelReference;
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

        public virtual string OnConfiguration(IntPtr IDataDistribution_nativePtr, string key, string value)
        {
            return value;
        }

        public virtual void OnLogging(IntPtr IDataDistribution_nativePtr, DDM_LOG_LEVEL level, string source, string function, string errStr)
        {

        }

        public virtual void OnCompletelyDisconnected(IntPtr IDataDistribution_nativePtr, string channelName, string reason)
        {

        }
        #endregion

        #region IDataDistributionMastershipCallback
        public virtual void OnClusterStateChange(IntPtr IDataDistribution_nativePtr, DDM_CLUSTEREVENT change, Int64 serverid)
        {

        }

        public virtual void OnStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {

        }

        public virtual void OnStateReady(IntPtr IDataDistribution_nativePtr, IntPtr buffer, Int64 len)
        {

        }

        public virtual void OnRequestedState(IntPtr IDataDistribution_nativePtr, IntPtr buffer, IntPtr len)
        {

        }

        public virtual void OnMultiplePrimary(IntPtr IDataDistribution_nativePtr, Int64 myId, Int64 otherId)
        {

        }

        public virtual void FirstStateChange(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE newState)
        {

        }

        public virtual void ChangingState(IntPtr IDataDistribution_nativePtr, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {

        }

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

        DataDistributionCallbackLow m_DataDistributionCallbackLow;
        DataDistributionMastershipCallbackLow m_DataDistributionMastershipCallbackLow;
        IntPtr IDataDistribution_ptr;
        IntPtr IDataDistributionSubsystemManager_ptr;
    }
}
