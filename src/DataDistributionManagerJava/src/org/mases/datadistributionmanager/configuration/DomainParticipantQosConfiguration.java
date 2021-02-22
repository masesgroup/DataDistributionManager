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

import java.util.ArrayList;

/// <summary>
/// The configuration class for DataWriterQos
/// </summary>
public class DomainParticipantQosConfiguration extends BaseConfiguration {
    /// <summary>
    /// Base property name of all specific configuration key of <see
    /// cref="DomainParticipantQosConfiguration"/>
    /// </summary>
    public static final String DomainParticipantQosBasePropertyKey = "datadistributionmanager.opendds.domainparticipantqos";

    /// <summary>
    /// Initialize a <see cref="DomainParticipantQosConfiguration"/>
    /// </summary>
    public DomainParticipantQosConfiguration() {
    }

    /// <summary>
    /// The configuration of <see cref="UserDataQosPolicyConfiguration"/>
    /// </summary>
    public UserDataQosPolicyConfiguration UserDataQosPolicy;

    /// <summary>
    /// The configuration of <see cref="EntityFactoryQosPolicyConfiguration"/>
    /// </summary>
    public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy;

    /// <summary>
    /// The configuration of <see cref="PropertyQosPolicyConfiguration"/>
    /// </summary>
    public PropertyQosPolicyConfiguration PropertyQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos",
                    DomainParticipantQosBasePropertyKey);
        }
        return array;
    }

    /// <summary>
    /// Creates configuration for QoS policies
    /// </summary>
    protected String[] PolicyBuilder(String[] parameters) {
        ArrayList<String> lst = new ArrayList<String>();
        if (UserDataQosPolicy != null) {
            for (String value : replacer(UserDataQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (EntityFactoryQosPolicy != null) {
            for (String value : replacer(EntityFactoryQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (PropertyQosPolicy != null) {
            for (String value : replacer(PropertyQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }

    /// <see cref="IConfiguration.Configuration"/>
    public String[] getConfiguration() {
        return PolicyBuilder(super.getConfiguration());
    }
}