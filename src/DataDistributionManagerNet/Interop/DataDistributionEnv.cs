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
    [StructLayout(LayoutKind.Sequential)]
    partial struct DataDistributionEnv
    {
        public DataDistributionEnv(IntPtr moduleEntry)
        {
            _functions = moduleEntry;
        }

        IntPtr _functions;

        public T GetDelegate<T>()
            where T : class
        {
            return Marshal.GetDelegateForFunctionPointer<T>(DataDistributionManagerInvokeWrapper.WrapperGetProcAddress(_functions, typeof(T).Name));
        }

        /// <summary>
        /// The interface table for all the function pointers in DataDistributionManager.dll.
        /// </summary>
        enum DataDistributionInterfaceTable
        {
            DataDistribution_create,
            DataDistributionCallback_create,
            DataDistributionCallback_delete,
            DataDistributionChannelCallback_create,
            DataDistributionChannelCallback_delete,
            DataDistributionMastershipCallback_create,
            DataDistributionMastershipCallback_delete,

            // IDataDistribution interface
            IDataDistribution_Initialize,
            IDataDistribution_Initialize2,
            IDataDistribution_RequestMastershipManager,
            IDataDistribution_GetSubsystemManager,
            IDataDistribution_GetMastershipManager,
            IDataDistribution_GetProtocol,
            IDataDistribution_GetProtocolLib,
            IDataDistribution_GetMastershipLib,

            // IDataDistributionSubsystem interface

            // DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Initialize,
            // DLLEXPORT OPERATION_RESULT DLLCALL IDataDistributionSubsystem_Initialize2,
            // DLLEXPORT void* DLLCALL IDataDistributionSubsystem_GetCallbacks,
            IDataDistributionSubsystem_GetServerLostTimeout,
            IDataDistributionSubsystem_GetMaxMessageSize,
            IDataDistributionSubsystem_Start,
            IDataDistributionSubsystem_Stop,
            IDataDistributionSubsystem_CreateChannel,
            IDataDistributionSubsystem_StartChannel,
            IDataDistributionSubsystem_StopChannel,
            IDataDistributionSubsystem_SetParameter,
            IDataDistributionSubsystem_SetParameter2,
            IDataDistributionSubsystem_GetParameter,
            IDataDistributionSubsystem_GetParameter2,
            IDataDistributionSubsystem_Lock,
            IDataDistributionSubsystem_Unlock,
            IDataDistributionSubsystem_SeekChannel,
            IDataDistributionSubsystem_DeleteChannel,
            IDataDistributionSubsystem_WriteOnChannel,
            IDataDistributionSubsystem_ReadFromChannel,
            IDataDistributionSubsystem_ChangeChannelDirection,

            // IDataDistributionMastershipCommon interface
            IDataDistributionMastershipCommon_Initialize,
            IDataDistributionMastershipCommon_SetParameter,
            IDataDistributionMastershipCommon_GetParameter,
            IDataDistributionMastershipCommon_Start,
            IDataDistributionMastershipCommon_Stop,
            IDataDistributionMastershipCommon_GetIamNextPrimary,
            IDataDistributionMastershipCommon_RequestIAmNextPrimary,
            IDataDistributionMastershipCommon_GetClusterIndexes,
            IDataDistributionMastershipCommon_GetStateOf,
            IDataDistributionMastershipCommon_GetClusterHealth,
            IDataDistributionMastershipCommon_GetMyState,
            IDataDistributionMastershipCommon_ChangeMyState,
            IDataDistributionMastershipCommon_ChangeState,
            IDataDistributionMastershipCommon_GetLocalServerId,
            IDataDistributionMastershipCommon_GetPrimaryServerId,
            IDataDistributionMastershipCommon_GetMessageDelay,
            IDataDistributionMastershipCommon_GetUpTime,

        }
    }
}
