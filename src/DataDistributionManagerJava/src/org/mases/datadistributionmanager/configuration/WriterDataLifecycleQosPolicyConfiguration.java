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

package org.mases.datadistributionmanager.configuration;

import org.mases.datadistributionmanager.BaseConfiguration;

/**
 * The configuration class for WriterDataLifecycleQosPolicy. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class WriterDataLifecycleQosPolicyConfiguration extends BaseConfiguration {
    public static final String AutodisposeUnregisteredInstancesPropertyKey = "datadistributionmanager.opendds.qos.writerdatalifecycleqospolicy.autodispose_unregistered_instances";

    /**
     * Initialize a new {@link WriterDataLifecycleQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public WriterDataLifecycleQosPolicyConfiguration() {
    }

    /**
     * See OpenDDS documentation at
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     * 
     * @return See OpenDDS documentation at <a href=
     *         "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     *         target=
     *         "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public Boolean getAutodisposeUnregisteredInstances() {
        String value = keyValuePair.get(AutodisposeUnregisteredInstancesPropertyKey);
        return Boolean.parseBoolean(value);
    }

    /**
     * See OpenDDS documentation at
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     * 
     * @param value See OpenDDS documentation at <a href=
     *              "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     *              target=
     *              "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public void setAutodisposeUnregisteredInstances(Boolean value) {
        keyValuePair.put(AutodisposeUnregisteredInstancesPropertyKey, value.toString());
    }
}
