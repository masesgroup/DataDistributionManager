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

namespace MASES.DataDistributionManager.Bindings
{
    public interface ISmartDataDistributionMastership
    {
        HRESULT Start(uint dwMilliseconds);

        HRESULT Stop(uint dwMilliseconds);

        void ChangeMyState(DDM_INSTANCE_STATE newState);

        void ChangeState(Int64 instanceId, DDM_INSTANCE_STATE newState);

        bool RequestIAmNextPrimary();

        bool IamNextPrimary { get; }

        IDictionary<Int64, DDM_INSTANCE_STATE> ClusterState { get; }

        IDictionary<Int64, ClusterHealthElement> ClusterHealth { get; }

        DDM_INSTANCE_STATE MyState { get; }

        Int64 LocalServerId { get; }

        Int64 PrimaryServerId { get; }

        Int64 MessageDelay { get; }

        Int64 UpTime { get; }
    }
}
