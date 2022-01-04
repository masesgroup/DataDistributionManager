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
    #region generic callback

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    delegate string dataDistributionConfigurationCb([In]IntPtr opaque, [In][MarshalAs(UnmanagedType.LPStr)] string channelName, [In][MarshalAs(UnmanagedType.LPStr)] string key, [In][MarshalAs(UnmanagedType.LPStr)] string value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionLoggingCb([In]IntPtr opaque, [In]DDM_LOG_LEVEL level, [In][MarshalAs(UnmanagedType.LPStr)] string source, [In][MarshalAs(UnmanagedType.LPStr)] string function, [In][MarshalAs(UnmanagedType.LPStr)] string errorStr);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionCompletelyDisconnectedCb([In]IntPtr opaque, [In][MarshalAs(UnmanagedType.LPStr)] string source, [In][MarshalAs(UnmanagedType.LPStr)] string errorStr);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionUnderlyingEvent(IntPtr opaque, IntPtr channelHandle, IntPtr uEvent);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionOnClusterStateChange(IntPtr opaque, DDM_CLUSTEREVENT change, Int64 serverid);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionOnStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionOnStateReady(IntPtr opaque, IntPtr pState, Int64 len);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionOnRequestedState(IntPtr opaque, IntPtr pState, IntPtr len);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionOnMultiplePrimary(IntPtr opaque, Int64 myId, Int64 otherId);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionFirstStateChange(IntPtr opaque, DDM_INSTANCE_STATE newState);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionChangingState(IntPtr opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void dataDistributionChangedState(IntPtr opaque, DDM_INSTANCE_STATE newState);

    #endregion

    #region DataDistribution

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr DataDistribution_create();
    #endregion

    #region DataDistributionCallback
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr DataDistributionCallback_create(IntPtr opaque, dataDistributionConfigurationCb configurationCb, dataDistributionLoggingCb logCb, dataDistributionCompletelyDisconnectedCb disconnectedCb);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void DataDistributionCallback_delete(IntPtr ddcb);
    #endregion

    #region DataDistributionChannelCallback
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr DataDistributionChannelCallback_create(IntPtr opaque, dataDistributionUnderlyingEvent uEventCb);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void DataDistributionChannelCallback_delete(IntPtr ddtcb);
    #endregion

    #region DataDistributionMastershipCallback
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr DataDistributionMastershipCallback_create(IntPtr opaque, dataDistributionOnClusterStateChange p1, dataDistributionOnStateChange p2,
                                                              dataDistributionOnStateReady p3, dataDistributionOnRequestedState p4, dataDistributionOnMultiplePrimary p5,
                                                              dataDistributionFirstStateChange p6, dataDistributionChangingState p7, dataDistributionChangedState p8);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void DataDistributionMastershipCallback_delete(IntPtr ddmcb);
    #endregion

    #region IDataDistribution interface
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistribution_Initialize(IntPtr IDataDistribution_instance, IntPtr iddcb,
                                                     [MarshalAs(UnmanagedType.LPStr)] string conf_file,
                                                     [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistribution_Initialize2(IntPtr IDataDistribution_instance, IntPtr iddcb,
                                                      [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams, int len,
                                                      [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistribution_RequestMastershipManager(IntPtr IDataDistribution_instance, IntPtr iddmcb, [MarshalAs(UnmanagedType.LPStr)] string szMyAddress,
                                                               [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams, int len);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistribution_GetSubsystemManager(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistribution_GetMastershipManager(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistribution_GetProtocol(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistribution_GetProtocolLib(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistribution_GetMastershipLib(IntPtr IDataDistribution_instance);
    #endregion

    #region IDataDistributionSubsystem interface
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate int IDataDistributionSubsystem_GetServerLostTimeout(IntPtr IDataDistributionSubsystem_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate long IDataDistributionSubsystem_GetMaxMessageSize(IntPtr IDataDistributionSubsystem_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_Start(IntPtr IDataDistributionSubsystem_instance, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_Stop(IntPtr IDataDistributionSubsystem_instance, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistributionSubsystem_CreateChannel(IntPtr IDataDistributionSubsystem_instance, [MarshalAs(UnmanagedType.LPStr)] string channelName, IntPtr dataCb, DDM_CHANNEL_DIRECTION direction,
                                                                  [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams = null, int len = 0);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_StartChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_StopChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void IDataDistributionSubsystem_SetParameter(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, [MarshalAs(UnmanagedType.LPStr)] string paramName, [MarshalAs(UnmanagedType.LPStr)] string paramValue);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void IDataDistributionSubsystem_SetParameter2(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, DDM_GENERAL_PARAMETER paramId, [MarshalAs(UnmanagedType.LPStr)] string paramValue);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    delegate string IDataDistributionSubsystem_GetParameter(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, [MarshalAs(UnmanagedType.LPStr)] string paramName);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    delegate string IDataDistributionSubsystem_GetParameter2(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, DDM_GENERAL_PARAMETER paramId);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_Lock(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint timeout);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_Unlock(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_SeekChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, long position, DDM_SEEKCONTEXT context, DDM_SEEKKIND kind);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_DeleteChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_WriteOnChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle,
                                                                 [MarshalAs(UnmanagedType.LPStr)] string key, IntPtr keyLen,
                                                                 IntPtr param, IntPtr dataLen,
                                                                 bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_ReadFromChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle,
                                                                  long offset, [In, Out] IntPtr dataLen, [In, Out]IntPtr buffer);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionSubsystem_ChangeChannelDirection(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, DDM_CHANNEL_DIRECTION direction);

    #endregion

    #region IDataDistributionMastershipCommon
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionMastershipCommon_Initialize(IntPtr IDataDistribution_instance, IntPtr iddmcb,
                                                                   [MarshalAs(UnmanagedType.LPStr)] string szMyAddress = null,
                                                                   [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams = null, int len = 0);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void IDataDistributionMastershipCommon_SetParameter(IntPtr IDataDistribution_instance, [MarshalAs(UnmanagedType.LPStr)] string paramName, [MarshalAs(UnmanagedType.LPStr)] string paramValue);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    delegate string IDataDistributionMastershipCommon_GetParameter(IntPtr IDataDistribution_instance, [MarshalAs(UnmanagedType.LPStr)] string paramName);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionMastershipCommon_Start(IntPtr IDataDistribution_instance, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate OPERATION_RESULT IDataDistributionMastershipCommon_Stop(IntPtr IDataDistribution_instance, uint dwMilliseconds);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate bool IDataDistributionMastershipCommon_GetIamNextPrimary(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate bool IDataDistributionMastershipCommon_RequestIAmNextPrimary(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I8)]
    delegate Int64[] IDataDistributionMastershipCommon_GetClusterIndexes(IntPtr IDataDistribution_instance, [In, Out] IntPtr dataLen);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate DDM_INSTANCE_STATE IDataDistributionMastershipCommon_GetStateOf(IntPtr IDataDistribution_instance, [In] Int64 index);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate IntPtr IDataDistributionMastershipCommon_GetClusterHealth(IntPtr IDataDistribution_instance, [In] Int64 index);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate DDM_INSTANCE_STATE IDataDistributionMastershipCommon_GetMyState(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void IDataDistributionMastershipCommon_ChangeMyState(IntPtr IDataDistribution_instance, DDM_INSTANCE_STATE newState);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate void IDataDistributionMastershipCommon_ChangeState(IntPtr IDataDistribution_instance, Int64 instanceId, DDM_INSTANCE_STATE newState);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate Int64 IDataDistributionMastershipCommon_GetLocalServerId(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate Int64 IDataDistributionMastershipCommon_GetPrimaryServerId(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate Int64 IDataDistributionMastershipCommon_GetMessageDelay(IntPtr IDataDistribution_instance);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi, SetLastError = true)]
    delegate Int64 IDataDistributionMastershipCommon_GetUpTime(IntPtr IDataDistribution_instance);
    #endregion
}
