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
public class PresentationQosPolicyConfiguration extends BaseConfiguration {
    /// <summary>
    /// Configuration key of <see cref="LeaseDuration"/>
    /// </summary>
    public static final String AccessScopePropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.access_scope";
    /// <summary>
    /// Configuration key of <see cref="LeaseDuration"/>
    /// </summary>
    public static final String CoherentAccessPropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.coherent_access";
    /// <summary>
    /// Configuration key of <see cref="LeaseDuration"/>
    /// </summary>
    public static final String OrderedAccessPropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.ordered_access";

    /// <summary>
    /// Initialize a new instance of <see
    /// cref="PresentationQosPolicyConfiguration"/>
    /// </summary>
    public PresentationQosPolicyConfiguration() {

    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public PresentationQosPolicyAccessScopeKind getAccessScope() {
        String value = keyValuePair.get(AccessScopePropertyKey);
        return PresentationQosPolicyAccessScopeKind.valueOfLabel(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setAccessScope(PresentationQosPolicyAccessScopeKind value) {
        keyValuePair.put(AccessScopePropertyKey, value.toIntString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Boolean getCoherentAccess() {
        String value = keyValuePair.get(CoherentAccessPropertyKey);
        return Boolean.parseBoolean(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setCoherentAccess(Boolean value) {
        keyValuePair.put(CoherentAccessPropertyKey, value.toString());
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Boolean getOrderedAccess() {
        String value = keyValuePair.get(OrderedAccessPropertyKey);
        return Boolean.parseBoolean(value);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setOrderedAccess(Boolean value) {
        keyValuePair.put(OrderedAccessPropertyKey, value.toString());
    }
}
