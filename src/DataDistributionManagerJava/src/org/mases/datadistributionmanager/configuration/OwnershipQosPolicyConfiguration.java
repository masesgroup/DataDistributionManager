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
 * Class managing OwnershipQosPolicy
 */
public class OwnershipQosPolicyConfiguration extends BaseConfiguration {
    /**
     * Configuration key of OwnershipQosPolicy property
     */
    public static final String KindPropertyKey = "datadistributionmanager.opendds.qos.ownershipqospolicy.kind";

    /**
     * Configuration key of OwnershipQosPolicy property
     * 
     * @param kind {@link OwnershipQosPolicyKind}
     */
    public OwnershipQosPolicyConfiguration(OwnershipQosPolicyKind kind) {
        setKind(kind);
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public OwnershipQosPolicyKind getKind() {
        String value = keyValuePair.get(KindPropertyKey);
        return OwnershipQosPolicyKind.valueOfLabel(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setKind(OwnershipQosPolicyKind value) {
        keyValuePair.put(KindPropertyKey, value.toIntString());
    }
}
