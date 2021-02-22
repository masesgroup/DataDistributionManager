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
 * Class managing DurabilityQosPolicy
 */
public class ResourceLimitsQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="MaxSamples"/>
    /// </summary>
    public static final String MaxSamplesPropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_samples";
    /// <summary>
    /// Configuration key of <see cref="MaxInstances"/>
    /// </summary>
    public static final String MaxInstancesPropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_instances";
    /// <summary>
    /// Configuration key of <see cref="MaxSamplesPerInstance"/>
    /// </summary>
    public static final String MaxSamplesPerInstancePropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_samples_per_instance";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="ResourceLimitsQosPolicyConfiguration"/>
    /// </summary>
    public ResourceLimitsQosPolicyConfiguration() {

    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getMaxSamples() {
        String value = keyValuePair.get(MaxSamplesPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setMaxSamples(Integer value) {
        keyValuePair.put(MaxSamplesPropertyKey, value.toString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getMaxInstances() {
        String value = keyValuePair.get(MaxInstancesPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setMaxInstances(Integer value) {
        keyValuePair.put(MaxInstancesPropertyKey, value.toString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getMaxSamplesPerInstance() {
        String value = keyValuePair.get(MaxSamplesPerInstancePropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setMaxSamplesPerInstance(Integer value) {
        keyValuePair.put(MaxSamplesPerInstancePropertyKey, value.toString());
    }
}
