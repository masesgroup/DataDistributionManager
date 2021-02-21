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

/**
 * Class managing DurabilityQosPolicy
 */
public class DurabilityServiceQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="ServiceCleanupDelay"/>
    /// </summary>
    public static final String ServiceCleanupDelayPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.service_cleanup_delay";
    /// <summary>
    /// Configuration key of <see cref="HistoryQosPolicy"/>
    /// </summary>
    public static final String KindPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.kind";
    /// <summary>
    /// Configuration key of <see cref="HistoryDepth"/>
    /// </summary>
    public static final String HistoryDepthPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.history_depth";
    /// <summary>
    /// Configuration key of <see cref="MaxSamples"/>
    /// </summary>
    public static final String MaxSamplesPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_samples";
    /// <summary>
    /// Configuration key of <see cref="MaxInstances"/>
    /// </summary>
    public static final String MaxInstancesPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_instances";
    /// <summary>
    /// Configuration key of <see cref="MaxSamplesPerInstance"/>
    /// </summary>
    public static final String MaxSamplesPerInstancePropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_samples_per_instance";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="DurabilityServiceQosPolicyConfiguration"/>
    /// </summary>
    public DurabilityServiceQosPolicyConfiguration() {

    }

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="DurabilityServiceQosPolicyConfiguration"/>
    /// </summary>
    /// <param name="historyQosPolicy">Value to use for <see
    /// name="HistoryQosPolicyKind"/></param>
    public DurabilityServiceQosPolicyConfiguration(HistoryQosPolicyKind kind) {
        setKind(kind);
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getServiceCleanupDelay() {
        String value = keyValuePair.get(ServiceCleanupDelayPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setServiceCleanupDelay(Integer value) {
        keyValuePair.put(ServiceCleanupDelayPropertyKey, value.toString());
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
    public Integer getHistoryDepth() {
        String value = keyValuePair.get(HistoryDepthPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setHistoryDepth(Integer value) {
        keyValuePair.put(HistoryDepthPropertyKey, value.toString());
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
