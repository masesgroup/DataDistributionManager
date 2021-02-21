package org.mases.datadistributionmanager;

import java.util.ArrayList;

/// <summary>
/// The configuration class for DataWriterQos
/// </summary>
public class DataReaderQosConfiguration extends BaseConfiguration {
    /// <summary>
    /// Base property name of all specific configuration key of <see
    /// cref="DataReaderQosConfiguration"/>
    /// </summary>
    public static final String DataReaderQosBasePropertyKey = "datadistributionmanager.opendds.datareaderqos";

    /// <summary>
    /// Initialize a <see cref="DataReaderQosConfiguration"/>
    /// </summary>
    public DataReaderQosConfiguration() {
    }

    /// <summary>
    /// The configuration of <see cref="DurabilityQosPolicyConfiguration"/>
    /// </summary>
    public DurabilityQosPolicyConfiguration DurabilityQosPolicy;

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
    /// The configuration of <see cref="OwnershipQosPolicyConfiguration"/>
    /// </summary>
    public OwnershipQosPolicyConfiguration OwnershipQosPolicy;

    /// <summary>
    /// The configuration of <see cref="TimeBasedFilterQosPolicyConfiguration"/>
    /// </summary>
    public TimeBasedFilterQosPolicyConfiguration TimeBasedFilterQosPolicy;

    /// <summary>
    /// The configuration of <see cref="ReaderDataLifecycleQosPolicyConfiguration"/>
    /// </summary>
    public ReaderDataLifecycleQosPolicyConfiguration ReaderDataLifecycleQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", DataReaderQosBasePropertyKey);
        }
        return array;
    }

    /// <summary>
    /// Creates configuration for QoS policies
    /// </summary>
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

    /// <see cref="IConfiguration.Configuration"/>
    public String[] getConfiguration() {
        return PolicyBuilder(super.getConfiguration());
    }
}