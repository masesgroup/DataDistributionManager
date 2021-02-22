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

package org.mases.datadistributionmanager.configuration;

import org.mases.datadistributionmanager.BaseConfiguration;

/**
 * Class managing LivelinessQosPolicy
 */
public class ReliabilityQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="LeaseDuration"/>
    /// </summary>
    public static final String KindPropertyKey = "datadistributionmanager.opendds.qos.reliabilityqospolicy.kind";
    /// <summary>
    /// Configuration key of <see cref="LeaseDuration"/>
    /// </summary>
    public static final String MaxBlockingTimePropertyKey = "datadistributionmanager.opendds.qos.reliabilityqospolicy.max_blocking_time";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="ReliabilityQosPolicyConfiguration"/>
    /// </summary>
    public ReliabilityQosPolicyConfiguration() {

    }

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="ReliabilityQosPolicyConfiguration"/>
    /// </summary>
    /// <param name="historyQosPolicy">Value to use for <see
    /// name="HistoryQosPolicyKind"/></param>
    public ReliabilityQosPolicyConfiguration(ReliabilityQosPolicyKind kind, Integer period) {
        setKind(kind);
        setMaxBlockingTime(period);
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public ReliabilityQosPolicyKind getKind() {
        String value = keyValuePair.get(KindPropertyKey);
        return ReliabilityQosPolicyKind.valueOfLabel(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setKind(ReliabilityQosPolicyKind value) {
        keyValuePair.put(KindPropertyKey, value.toIntString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getMaxBlockingTime() {
        String value = keyValuePair.get(MaxBlockingTimePropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setMaxBlockingTime(Integer value) {
        keyValuePair.put(MaxBlockingTimePropertyKey, value.toString());
    }
}
