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

public class EntityFactoryQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="Period"/>
    /// </summary>
    public static final String AutoenableCreatedEntitiesPropertyKey = "datadistributionmanager.opendds.qos.entityfactoryqospolicy.autoenable_created_entities";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="EntityFactoryQosPolicyConfiguration"/>
    /// </summary>
    public EntityFactoryQosPolicyConfiguration() {
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Boolean getAutoenableCreatedEntities() {
        String value = keyValuePair.get(AutoenableCreatedEntitiesPropertyKey);
        return Boolean.parseBoolean(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setAutoenableCreatedEntities(Boolean value) {
        keyValuePair.put(AutoenableCreatedEntitiesPropertyKey, value.toString());
    }
}
