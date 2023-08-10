/*
*  Copyright 2023 MASES s.r.l
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

public interface ISmartDataDistributionChannelInfo {
    /**
     * The name of the channel
     * 
     * @return The name of the channel
     */
    String getChannelName();

    /**
     * The last available message timestamp associated to the data
     * 
     * @return The last available message timestamp associated to the data:
     *         Milliseconds since epoch (UTC).
     */
    long getTimestamp();

    /**
     * The last available message offset associated to the data
     * 
     * @return The last available message offset associated to the data
     */
    long getOffset();
}
