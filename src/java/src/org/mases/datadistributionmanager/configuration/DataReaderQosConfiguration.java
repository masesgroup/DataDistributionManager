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

import java.util.ArrayList;

/**
 * The configuration class for DataWriterQos. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class DataReaderQosConfiguration extends BaseConfiguration {
    public static final String DataReaderQosBasePropertyKey = "datadistributionmanager.opendds.datareaderqos";

    /**
     * Initialize a {@link DataReaderQosConfiguration}
     */
    public DataReaderQosConfiguration() {
    }

    /**
     * The configuration of {@link DurabilityQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public DurabilityQosPolicyConfiguration DurabilityQosPolicy;

    /**
     * The configuration of {@link DeadlineQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public DeadlineQosPolicyConfiguration DeadlineQosPolicy;

    /**
     * The configuration of {@link LatencyBudgetQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public LatencyBudgetQosPolicyConfiguration LatencyBudgetQosPolicy;

    /**
     * The configuration of {@link LivelinessQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public LivelinessQosPolicyConfiguration LivelinessQosPolicy;

    /**
     * The configuration of {@link ReliabilityQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public ReliabilityQosPolicyConfiguration ReliabilityQosPolicy;

    /**
     * The configuration of {@link DestinationOrderQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public DestinationOrderQosPolicyConfiguration DestinationOrderQosPolicy;

    /**
     * The configuration of {@link HistoryQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public HistoryQosPolicyConfiguration HistoryQosPolicy;

    /**
     * The configuration of {@link ResourceLimitsQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public ResourceLimitsQosPolicyConfiguration ResourceLimitsQosPolicy;

    /**
     * The configuration of {@link OwnershipQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public OwnershipQosPolicyConfiguration OwnershipQosPolicy;

    /**
     * The configuration of {@link TimeBasedFilterQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public TimeBasedFilterQosPolicyConfiguration TimeBasedFilterQosPolicy;

    /**
     * The configuration of {@link ReaderDataLifecycleQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public ReaderDataLifecycleQosPolicyConfiguration ReaderDataLifecycleQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", DataReaderQosBasePropertyKey);
        }
        return array;
    }

    /**
     * Creates configuration for QoS policies
     * 
     * @param parameters Input parameters
     * @return updated parameters
     */
    protected String[] PolicyBuilder(String[] parameters) {
        ArrayList<String> lst = new ArrayList<String>();
        if (DurabilityQosPolicy != null) {
            for (String value : replacer(DurabilityQosPolicy.getConfiguration())) {
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
        if (OwnershipQosPolicy != null) {
            for (String value : replacer(OwnershipQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (TimeBasedFilterQosPolicy != null) {
            for (String value : replacer(TimeBasedFilterQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        if (ReaderDataLifecycleQosPolicy != null) {
            for (String value : replacer(ReaderDataLifecycleQosPolicy.getConfiguration())) {
                lst.add(value);
            }
        }
        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }

    public String[] getConfiguration() {
        return PolicyBuilder(super.getConfiguration());
    }
}