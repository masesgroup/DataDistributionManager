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

public class ClusterHealthElement {
    final DDM_INSTANCE_STATE Status;
    final long Uptime;
    final long LastContactTime;

    public ClusterHealthElement(int status, long uptime, long lastContactTime) {
        this.Status = DDM_INSTANCE_STATE.valueOfAtomicNumber(status);
        this.Uptime = uptime;
        this.LastContactTime = lastContactTime;
    }

    public DDM_INSTANCE_STATE getStatus() {
        return Status;
    }

    public long getUptime() {
        return Uptime;
    }

    public long getLastContactTime() {
        return LastContactTime;
    }
};