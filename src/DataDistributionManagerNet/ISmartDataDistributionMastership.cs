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
using System.Collections.Generic;

namespace MASES.DataDistributionManager.Bindings
{
    #region ISmartDataDistributionMastership
    /// <summary>
    /// Interface to manage underlying mastersip manager
    /// </summary>
    public interface ISmartDataDistributionMastership
    {
        /// <summary>
        /// Set mastership parameter
        /// </summary>
        /// <param name="paramName">Parameter to set</param>
        /// <param name="paramValue">Value to set</param>
        void SetParameter(string paramName, string paramValue);
        /// <summary>
        /// Get mastership parameter
        /// </summary>
        /// <param name="paramName">Parameter to get</param>
        /// <returns>Parameter value</returns>
        string GetParameter(string paramName);
        /// <summary>
        /// Starts the manager
        /// </summary>
        /// <param name="dwMilliseconds">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Start(uint dwMilliseconds);
        /// <summary>
        /// Stops the manager
        /// </summary>
        /// <param name="dwMilliseconds">Timeout in ms</param>
        /// <returns><see cref="OPERATION_RESULT"/></returns>
        OPERATION_RESULT Stop(uint dwMilliseconds);
        /// <summary>
        /// Change the state of this instance
        /// </summary>
        /// <param name="newState">Expected <see cref="DDM_INSTANCE_STATE"/></param>
        void ChangeMyState(DDM_INSTANCE_STATE newState);
        /// <summary>
        /// Change the state of the instance <paramref name="instanceId"/>
        /// </summary>
        /// <param name="instanceId">Instance to change state to <paramref name="newState"/></param>
        /// <param name="newState">Expected <see cref="DDM_INSTANCE_STATE"/></param>
        void ChangeState(Int64 instanceId, DDM_INSTANCE_STATE newState);
        /// <summary>
        /// Request to be the next primary server
        /// </summary>
        /// <returns>True in case of success</returns>
        bool RequestIAmNextPrimary();
        /// <summary>
        /// Verify if this server will be elected as next primary
        /// </summary>
        /// <value>True if this server is the next primary</value>
        bool IamNextPrimary { get; }
        /// <summary>
        /// State of the cluster
        /// </summary>
        IDictionary<Int64, DDM_INSTANCE_STATE> ClusterState { get; }
        /// <summary>
        /// Health of the cluster
        /// </summary>
        IDictionary<Int64, ClusterHealthElement> ClusterHealth { get; }
        /// <summary>
        /// Get my <see cref="DDM_INSTANCE_STATE"/>
        /// </summary>
        DDM_INSTANCE_STATE MyState { get; }
        /// <summary>
        /// Get my server identifier
        /// </summary>
        Int64 LocalServerId { get; }
        /// <summary>
        /// Gets identifier of the primary server
        /// </summary>
        Int64 PrimaryServerId { get; }
        /// <summary>
        /// Gets actual message delay in the cluster
        /// </summary>
        Int64 MessageDelay { get; }
        /// <summary>
        /// Gets this instance uptime
        /// </summary>
        Int64 UpTime { get; }
    }
    #endregion
}
