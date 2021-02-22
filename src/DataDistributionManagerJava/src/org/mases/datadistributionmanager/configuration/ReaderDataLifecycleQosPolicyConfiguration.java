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

public class ReaderDataLifecycleQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="Period"/>
    /// </summary>
    public static final String AutopurgeNowriterSamplesDelayPropertyKey = "datadistributionmanager.opendds.qos.readerdatalifecycleqospolicy.autopurge_nowriter_samples_delay";
    /// <summary>
    /// Configuration key of <see cref="Period"/>
    /// </summary>
    public static final String AutopurgeDisposedSamplesDelayPropertyKey = "datadistributionmanager.opendds.qos.readerdatalifecycleqospolicy.autopurge_disposed_samples_delay";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="ReaderDataLifecycleQosPolicyConfiguration"/>
    /// </summary>
    public ReaderDataLifecycleQosPolicyConfiguration() {
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getAutopurgeNowriterSamplesDelay() {
        String value = keyValuePair.get(AutopurgeNowriterSamplesDelayPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setAutopurgeNowriterSamplesDelay(Integer value) {
        keyValuePair.put(AutopurgeNowriterSamplesDelayPropertyKey, value.toString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getAutopurgeDisposedSamplesDelay() {
        String value = keyValuePair.get(AutopurgeDisposedSamplesDelayPropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setAutopurgeDisposedSamplesDelay(Integer value) {
        keyValuePair.put(AutopurgeDisposedSamplesDelayPropertyKey, value.toString());
    }
}
