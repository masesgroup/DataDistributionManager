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
 * Class managing HistoryQosPolicy
 */
public class HistoryQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="Kind"/>
    /// </summary>
    public static final String KindPropertyKey = "datadistributionmanager.opendds.qos.historyqospolicy.kind";
    /// <summary>
    /// Configuration key of <see cref="Depth"/>
    /// </summary>
    public static final String DepthPropertyKey = "datadistributionmanager.opendds.qos.historyqospolicy.depth";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="HistoryQosPolicyConfiguration"/>
    /// </summary>
    public HistoryQosPolicyConfiguration() {

    }

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="HistoryQosPolicyConfiguration"/>
    /// </summary>
    /// <param name="historyQosPolicy">Value to use for <see
    /// name="HistoryQosPolicyKind"/></param>
    public HistoryQosPolicyConfiguration(HistoryQosPolicyKind kind, Integer period) {
        setKind(kind);
        setDepth(period);
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public HistoryQosPolicyKind getKind() {
        String value = keyValuePair.get(KindPropertyKey);
        return HistoryQosPolicyKind.valueOfLabel(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setKind(HistoryQosPolicyKind value) {
        keyValuePair.put(KindPropertyKey, value.toIntString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getDepth() {
        String value = keyValuePair.get(DepthPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setDepth(Integer value) {
        keyValuePair.put(DepthPropertyKey, value.toString());
    }
}
