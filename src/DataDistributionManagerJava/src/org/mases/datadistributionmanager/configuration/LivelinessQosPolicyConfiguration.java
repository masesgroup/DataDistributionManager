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
 * The configuration class for LivelinessQosPolicy. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class LivelinessQosPolicyConfiguration extends BaseConfiguration {
    public static final String KindPropertyKey = "datadistributionmanager.opendds.qos.livelinessqospolicy.kind";
    public static final String LeaseDurationPropertyKey = "datadistributionmanager.opendds.qos.livelinessqospolicy.lease_duration";

    /**
     * Initialize a new {@link LivelinessQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public LivelinessQosPolicyConfiguration() {

    }

    /**
     * Initialize a new {@link LivelinessQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     * 
     * @param kind   See OpenDDS documentation at <a href=
     *               "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     *               target=
     *               "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     * @param period See OpenDDS documentation at <a href=
     *               "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     *               target=
     *               "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public LivelinessQosPolicyConfiguration(LivelinessQosPolicyKind kind, Integer period) {
        setKind(kind);
        setLeaseDuration(period);
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
    public LivelinessQosPolicyKind getKind() {
        String value = keyValuePair.get(KindPropertyKey);
        return LivelinessQosPolicyKind.valueOfLabel(value);
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
    public void setKind(LivelinessQosPolicyKind value) {
        keyValuePair.put(KindPropertyKey, value.toIntString());
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
    public Integer getLeaseDuration() {
        String value = keyValuePair.get(LeaseDurationPropertyKey);
        return Integer.parseInt(value);
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
    public void setLeaseDuration(Integer value) {
        keyValuePair.put(LeaseDurationPropertyKey, value.toString());
    }
}
