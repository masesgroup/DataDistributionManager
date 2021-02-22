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
public class SubscriberQosConfiguration extends BaseConfiguration {
    /// <summary>
    /// Base property name of all specific configuration key of <see
    /// cref="SubscriberQosConfiguration"/>
    /// </summary>
    public static final String SubscriberQosBasePropertyKey = "datadistributionmanager.opendds.subscriberqos";

    /// <summary>
    /// Initialize a <see cref="SubscriberQosConfiguration"/>
    /// </summary>
    public SubscriberQosConfiguration() {
    }

    /// <summary>
    /// The configuration of <see cref="PresentationQosPolicyConfiguration"/>
    /// </summary>
    public PresentationQosPolicyConfiguration PresentationQosPolicy;

    /// <summary>
    /// The configuration of <see cref="PartitionQosPolicyConfiguration"/>
    /// </summary>
    public PartitionQosPolicyConfiguration PartitionQosPolicy;

    /// <summary>
    /// The configuration of <see cref="GroupDataQosPolicyConfiguration"/>
    /// </summary>
    public GroupDataQosPolicyConfiguration GroupDataQosPolicy;

    /// <summary>
    /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>
    /// </summary>
    public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", SubscriberQosBasePropertyKey);
        }
        return array;
    }

    /// <summary>
    /// Creates configuration for QoS policies
    /// </summary>
    protected String[] PolicyBuilder(String[] parameters) {
        ArrayList<String> lst = new ArrayList<String>();
        if (PresentationQosPolicy != null) {
            for (String value : replacer(PresentationQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (PartitionQosPolicy != null) {
            for (String value : replacer(PartitionQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (GroupDataQosPolicy != null) {
            for (String value : replacer(GroupDataQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (EntityFactoryQosPolicy != null) {
            for (String value : replacer(EntityFactoryQosPolicy.getConfiguration())) {
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