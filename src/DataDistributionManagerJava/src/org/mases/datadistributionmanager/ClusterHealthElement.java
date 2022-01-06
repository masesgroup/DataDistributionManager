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

package org.mases.datadistributionmanager;

/**
 * Container of cluster instance information
 */
public class ClusterHealthElement {
    final long ServerId;
    final DDM_INSTANCE_STATE Status;
    final long Uptime;
    final long LastContactTime;

    ClusterHealthElement(long serverId, int status, long uptime, long lastContactTime) {
        this.ServerId = serverId;
        this.Status = DDM_INSTANCE_STATE.valueOfAtomicNumber(status);
        this.Uptime = uptime;
        this.LastContactTime = lastContactTime;
    }

    /**
     * Server identifier
     * 
     * @return The server id
     */
    public long getServerId() {
        return ServerId;
    }

    /**
     * Instance status
     * 
     * @return The {@link DDM_INSTANCE_STATE} status
     */
    public DDM_INSTANCE_STATE getStatus() {
        return Status;
    }

    /**
     * Instance uptime
     * 
     * @return the uptime
     */
    public long getUptime() {
        return Uptime;
    }

    /**
     * Last contact time of the instance
     * 
     * @return last contact time
     */
    public long getLastContactTime() {
        return LastContactTime;
    }
};