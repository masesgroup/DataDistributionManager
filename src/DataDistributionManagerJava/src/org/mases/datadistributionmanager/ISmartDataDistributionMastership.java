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

package org.mases.datadistributionmanager;

import java.util.HashMap;

/**
 * Interface to manage undrlying mastersip manager
 */
public interface ISmartDataDistributionMastership {
    /**
     * Starts the manager
     * 
     * @param dwMilliseconds Timeout in ms
     * @return {@link HRESULT}
     */
    HRESULT Start(int dwMilliseconds);

    /**
     * Stops the manager
     * 
     * @param dwMilliseconds Timeout in ms
     * @return {@link HRESULT}
     */
    HRESULT Stop(int dwMilliseconds);

    /**
     * Change the state of this instance
     * 
     * @param newState The new {@link DDM_INSTANCE_STATE} state
     */
    void ChangeMyState(DDM_INSTANCE_STATE newState);

    /**
     * Change the state of the instance with instanceId
     * 
     * @param instanceId Instance to change state
     * @param newState   Expected {@link DDM_INSTANCE_STATE} state
     */
    void ChangeState(long instanceId, DDM_INSTANCE_STATE newState);

    /**
     * Request to be the next primary server
     * 
     * @return True in case of success
     */
    boolean RequestIAmNextPrimary();

    /**
     * Verify if this server will be elected as next primary
     * 
     * @return True if this server is the next primary
     */
    boolean getIamNextPrimary();

    /**
     * State of the cluster
     * 
     * @return an {@link HashMap} between server id and {@link DDM_INSTANCE_STATE}
     */
    HashMap<Long, DDM_INSTANCE_STATE> getClusterState();

    /**
     * Health of the cluster
     * 
     * @return an {@link HashMap} between server id and {@link ClusterHealthElement}
     */
    HashMap<Long, ClusterHealthElement> getClusterHealth();

    /**
     * Get my {@link DDM_INSTANCE_STATE} state
     * 
     * @return this {@link DDM_INSTANCE_STATE} state
     */
    DDM_INSTANCE_STATE getMyState();

    /**
     * Get my server identifier
     * 
     * @return local identifier
     */
    long getLocalServerId();

    /**
     * Gets identifier of the primary server
     * 
     * @return primary identifier
     */
    long getPrimaryServerId();

    /**
     * Gets actual message delay in the cluster
     * 
     * @return message delay in the cluster
     */
    long getMessageDelay();

    /**
     * Gets this instance uptime
     * 
     * @return the uptime
     */
    long getUpTime();
}