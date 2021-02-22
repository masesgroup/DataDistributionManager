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
public class TopicQosConfiguration extends BaseConfiguration {
    /// <summary>
    /// Base property name of all specific configuration key of <see
    /// cref="TopicQosConfiguration"/>
    /// </summary>
    public static final String TopicQosBasePropertyKey = "datadistributionmanager.opendds.topicqos";

    /// <summary>
    /// Initialize a <see cref="TopicQosConfiguration"/>
    /// </summary>
    public TopicQosConfiguration() {
    }

    /// <summary>
    /// The configuration of <see cref="TopicDataQosPolicyConfiguration"/>
    /// </summary>
    public TopicDataQosPolicyConfiguration TopicDataQosPolicy;

    /// <summary>
    /// The configuration of <see cref="DurabilityQosPolicyConfiguration"/>
    /// </summary>
    public DurabilityQosPolicyConfiguration DurabilityQosPolicy;

    /// <summary>
    /// The configuration of <see cref="DurabilityServiceQosPolicyConfiguration"/>
    /// </summary>
    public DurabilityServiceQosPolicyConfiguration DurabilityServiceQosPolicy;

    /// <summary>
    /// The configuration of <see cref="DeadlineQosPolicyConfiguration"/>
    /// </summary>
    public DeadlineQosPolicyConfiguration DeadlineQosPolicy;

    /// <summary>
    /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>
    /// </summary>
    public LatencyBudgetQosPolicyConfiguration LatencyBudgetQosPolicy;

    /// <summary>
    /// The configuration of <see cref="LivelinessQosPolicyConfiguration"/>
    /// </summary>
    public LivelinessQosPolicyConfiguration LivelinessQosPolicy;

    /// <summary>
    /// The configuration of <see cref="ReliabilityQosPolicyConfiguration"/>
    /// </summary>
    public ReliabilityQosPolicyConfiguration ReliabilityQosPolicy;

    /// <summary>
    /// The configuration of <see cref="DestinationOrderQosPolicyConfiguration"/>
    /// </summary>
    public DestinationOrderQosPolicyConfiguration DestinationOrderQosPolicy;

    /// <summary>
    /// The configuration of <see cref="HistoryQosPolicyConfiguration"/>
    /// </summary>
    public HistoryQosPolicyConfiguration HistoryQosPolicy;

    /// <summary>
    /// The configuration of <see cref="ResourceLimitsQosPolicyConfiguration"/>
    /// </summary>
    public ResourceLimitsQosPolicyConfiguration ResourceLimitsQosPolicy;

    /// <summary>
    /// The configuration of <see cref="TransportPriorityQosPolicyConfiguration"/>
    /// </summary>
    public TransportPriorityQosPolicyConfiguration TransportPriorityQosPolicy;

    /// <summary>
    /// The configuration of <see cref="LifespanQosPolicyConfiguration"/>
    /// </summary>
    public LifespanQosPolicyConfiguration LifespanQosPolicy;

    /// <summary>
    /// The configuration of <see cref="OwnershipQosPolicyConfiguration"/>
    /// </summary>
    public OwnershipQosPolicyConfiguration OwnershipQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", TopicQosBasePropertyKey);
        }
        return array;
    }

    /// <summary>
    /// Creates configuration for QoS policies
    /// </summary>
    protected String[] PolicyBuilder(String[] parameters) {
        ArrayList<String> lst = new ArrayList<String>();
        if (TopicDataQosPolicy != null) {
            for (String value : replacer(TopicDataQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (DurabilityQosPolicy != null) {
            for (String value : replacer(DurabilityQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (DurabilityServiceQosPolicy != null) {
            for (String value : replacer(DurabilityServiceQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (DeadlineQosPolicy != null) {
            for (String value : replacer(DeadlineQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (LatencyBudgetQosPolicy != null) {
            for (String value : replacer(LatencyBudgetQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (LivelinessQosPolicy != null) {
            for (String value : replacer(LivelinessQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (ReliabilityQosPolicy != null) {
            for (String value : replacer(ReliabilityQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (DestinationOrderQosPolicy != null) {
            for (String value : replacer(DestinationOrderQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (HistoryQosPolicy != null) {
            for (String value : replacer(HistoryQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (ResourceLimitsQosPolicy != null) {
            for (String value : replacer(ResourceLimitsQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (TransportPriorityQosPolicy != null) {
            for (String value : replacer(TransportPriorityQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (LifespanQosPolicy != null) {
            for (String value : replacer(LifespanQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (OwnershipQosPolicy != null) {
            for (String value : replacer(OwnershipQosPolicy.getConfiguration())) {
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