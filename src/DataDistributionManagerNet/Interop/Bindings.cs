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

using System;
using System.Runtime.InteropServices;

namespace MASES.DataDistributionManager.Bindings.Interop
{
    #region interface
    interface IUnderlyingPointerLow
    {
        IntPtr Pointer { get; }
    }

    interface IDataDistributionCallbackLow
    {
        string OnConfiguration(IntPtr opaque, string channelName, string key, string value);
        void OnLogging(IntPtr opaque, DDM_LOG_LEVEL logLevel, string source, string function, string errorStr);
        void OnCompletelyDisconnected(IntPtr opaque, string channelName, string reason);
    }

    interface IDataDistributionChannelCallbackLow
    {
        void OnUnderlyingEvent(IntPtr opaque, IntPtr channelHandle, UnderlyingEvent uEvent);
    }

    interface IDataDistributionMastershipCallbackLow
    {
        void OnClusterStateChange(IntPtr opaque, DDM_CLUSTEREVENT change, Int64 serverid);
        void OnStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
        void OnStateReady(IntPtr opaque, IntPtr pState, Int64 len);
        void OnRequestedState(IntPtr opaque, IntPtr pState, IntPtr len);
        void OnMultiplePrimary(IntPtr opaque, Int64 myId, Int64 otherId);
        void FirstStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState);
        void ChangingState(IntPtr opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);
        void ChangedState(IntPtr opaque, DDM_INSTANCE_STATE newState);
    };

    interface IDataDistributionLow
    {
        uint Initialize(IntPtr iddcb, IntPtr iddmcb,
                                      [MarshalAs(UnmanagedType.LPStr)] string conf_file,
                                      [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);


        uint IDataDistribution_Initialize(IntPtr iddcb, IntPtr iddmcb,
                                                          [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams, int len,
                                                          [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);

        IntPtr IDataDistribution_GetSubsystemManager();
        IntPtr IDataDistribution_GetMastershipManager();

        IntPtr IDataDistribution_GetProtocol();
        IntPtr IDataDistribution_GetProtocolLib();
        IntPtr IDataDistribution_GetMastershipLib();
    }

    interface IDataDistributionChannelBaseLow
    {
        IntPtr CreateChannel(string channelName, IntPtr dataCb, [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams = null, int len = 0);
        bool StartChannel(IntPtr channelHandle, uint dwMilliseconds);
        bool StopChannel(IntPtr channelHandle, uint dwMilliseconds);
        void Lock(IntPtr channelHandle, uint timeout);
        void Unlock(IntPtr channelHandle);
        void SeekChannel(IntPtr channelHandle, Int64 position);
        void DeleteChannel(IntPtr channelHandle);
        bool WriteOnChannel(IntPtr channelHandle, [MarshalAs(UnmanagedType.LPStr)] string key, Int64 keyLen, IntPtr param, Int64 dataLen, bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        bool ReadFromChannel(IntPtr channelHandle, IntPtr param, IntPtr dataLen, Int64 offset);
    };

    interface IDataDistributionTransportLow : IDataDistributionChannelBaseLow
    {
        int GetServerLostTimeout();
        Int64 GetMaxMessageSize();
        bool Start(uint dwMilliseconds);
        bool Stop(uint dwMilliseconds);
    };

    #endregion

    #region internal classes

    class DataDistributionCallbackLow : IUnderlyingPointerLow
    {
        public DataDistributionCallbackLow(IntPtr internalRef, IDataDistributionCallbackLow cbs)
        {
            IDataDistributionCallback_cbs = cbs;
            p1 = new Interop.dataDistributionConfigurationCb(dataDistributionConfigurationCb);
            p2 = new Interop.dataDistributionLoggingCb(dataDistributionLoggingCb);
            p3 = new Interop.dataDistributionCompletelyDisconnectedCb(dataDistributionCompletelyDisconnectedCb);
            Pointer = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionCallback_create>().Invoke(internalRef, p1, p2, p3);
        }

        ~DataDistributionCallbackLow()
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionCallback_delete>().Invoke(Pointer);
        }

        [return: MarshalAs(UnmanagedType.LPStr)]
        string dataDistributionConfigurationCb([In]IntPtr opaque, [In][MarshalAs(UnmanagedType.LPStr)] string channelName, [In][MarshalAs(UnmanagedType.LPStr)] string key, [In][MarshalAs(UnmanagedType.LPStr)] string value)
        {
            return IDataDistributionCallback_cbs.OnConfiguration(opaque, channelName, key, value);
        }

        void dataDistributionLoggingCb(IntPtr opaque, DDM_LOG_LEVEL level, [In][MarshalAs(UnmanagedType.LPStr)] string source, [In][MarshalAs(UnmanagedType.LPStr)] string function, [In][MarshalAs(UnmanagedType.LPStr)] string errorStr)
        {
            IDataDistributionCallback_cbs.OnLogging(opaque, level, source, function, errorStr);
        }

        void dataDistributionCompletelyDisconnectedCb([In]IntPtr opaque, [In][MarshalAs(UnmanagedType.LPStr)] string source, [In][MarshalAs(UnmanagedType.LPStr)] string errorStr)
        {
            IDataDistributionCallback_cbs.OnCompletelyDisconnected(opaque, source, errorStr);
        }

        public IntPtr Pointer { get; private set; }
        #region Private members
        IDataDistributionCallbackLow IDataDistributionCallback_cbs;
        dataDistributionConfigurationCb p1;
        dataDistributionLoggingCb p2;
        dataDistributionCompletelyDisconnectedCb p3;
        #endregion
    }

    class DataDistributionChannelCallbackLow : IUnderlyingPointerLow
    {
        public DataDistributionChannelCallbackLow(IntPtr internalRef, IDataDistributionChannelCallbackLow cbs)
        {
            IDataDistributionChannelCallbackLow_cbs = cbs;
            p1 = new Interop.dataDistributionUnderlyingEvent(dataDistributionUnderlyingEvent);
            Pointer = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionChannelCallback_create>().Invoke(internalRef, p1);
        }

        ~DataDistributionChannelCallbackLow()
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionChannelCallback_delete>().Invoke(Pointer);
        }

        void dataDistributionUnderlyingEvent(IntPtr opaque, IntPtr channelHandle, IntPtr uEvent)
        {
            var eventData = Marshal.PtrToStructure<UnderlyingEvent>(uEvent);
            IDataDistributionChannelCallbackLow_cbs.OnUnderlyingEvent(opaque, channelHandle, eventData);
        }

        public IntPtr Pointer { get; private set; }
        #region Private members
        IDataDistributionChannelCallbackLow IDataDistributionChannelCallbackLow_cbs;
        dataDistributionUnderlyingEvent p1;
        #endregion
    }

    class DataDistributionMastershipCallbackLow : IUnderlyingPointerLow
    {
        public DataDistributionMastershipCallbackLow(IntPtr internalRef, IDataDistributionMastershipCallbackLow cbs)
        {
            IDataDistributionMastershipCallbackLow_cbs = cbs;
            p1 = new Interop.dataDistributionOnClusterStateChange(dataDistributionOnClusterStateChange);
            p2 = new Interop.dataDistributionOnStateChange(dataDistributionOnStateChange);
            p3 = new Interop.dataDistributionOnStateReady(dataDistributionOnStateReady);
            p4 = new Interop.dataDistributionOnRequestedState(dataDistributionOnRequestedState);
            p5 = new Interop.dataDistributionOnMultiplePrimary(dataDistributionOnMultiplePrimary);
            p6 = new Interop.dataDistributionFirstStateChange(dataDistributionFirstStateChange);
            p7 = new Interop.dataDistributionChangingState(dataDistributionChangingState);
            p8 = new Interop.dataDistributionChangedState(dataDistributionChangedState);
            Pointer = DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionMastershipCallback_create>().Invoke(internalRef, p1, p2, p3, p4, p5, p6, p7, p8);
        }

        ~DataDistributionMastershipCallbackLow()
        {
            DataDistributionManagerInvokeWrapper.DataDistributionEnv.GetDelegate<DataDistributionMastershipCallback_delete>().Invoke(Pointer);
        }

        void dataDistributionOnClusterStateChange(IntPtr opaque, DDM_CLUSTEREVENT change, Int64 serverid)
        {
            IDataDistributionMastershipCallbackLow_cbs.OnClusterStateChange(opaque, change, serverid);
        }

        void dataDistributionOnStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
        {
            IDataDistributionMastershipCallbackLow_cbs.OnStateChange(opaque, newState, oldState);
        }

        void dataDistributionOnStateReady(IntPtr opaque, IntPtr pState, Int64 len)
        {
            IDataDistributionMastershipCallbackLow_cbs.OnStateReady(opaque, pState, len);
        }

        void dataDistributionOnRequestedState(IntPtr opaque, IntPtr pState, IntPtr len)
        {
            IDataDistributionMastershipCallbackLow_cbs.OnRequestedState(opaque, pState, len);
        }

        void dataDistributionOnMultiplePrimary(IntPtr opaque, Int64 myId, Int64 otherId)
        {
            IDataDistributionMastershipCallbackLow_cbs.OnMultiplePrimary(opaque, myId, otherId);
        }

        void dataDistributionFirstStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState)
        {
            IDataDistributionMastershipCallbackLow_cbs.FirstStateChange(opaque, newState);
        }

        void dataDistributionChangingState(IntPtr opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
        {
            IDataDistributionMastershipCallbackLow_cbs.ChangingState(opaque, oldState, newState);
        }

        void dataDistributionChangedState(IntPtr opaque, DDM_INSTANCE_STATE newState)
        {
            IDataDistributionMastershipCallbackLow_cbs.ChangedState(opaque, newState);
        }

        public IntPtr Pointer { get; private set; }
        #region Private members
        IDataDistributionMastershipCallbackLow IDataDistributionMastershipCallbackLow_cbs;
        dataDistributionOnClusterStateChange p1;
        dataDistributionOnStateChange p2;
        dataDistributionOnStateReady p3;
        dataDistributionOnRequestedState p4;
        dataDistributionOnMultiplePrimary p5;
        dataDistributionFirstStateChange p6;
        dataDistributionChangingState p7;
        dataDistributionChangedState p8;
        #endregion
    }

    #endregion
}
