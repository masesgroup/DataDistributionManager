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

public class OwnershipStrengthQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="Period"/>
    /// </summary>
    public static final String ValuePropertyKey = "datadistributionmanager.opendds.qos.ownershipstrengthqospolicy.value";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="OwnershipStrengthQosPolicyConfiguration"/>
    /// </summary>
    public OwnershipStrengthQosPolicyConfiguration() {
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Integer getValue() {
        String value = keyValuePair.get(ValuePropertyKey);
        return Integer.parseInt(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setValue(Integer value) {
        keyValuePair.put(ValuePropertyKey, value.toString());
    }
}
