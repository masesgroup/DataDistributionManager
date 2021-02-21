package org.mases.datadistributionmanager;

import java.util.ArrayList;

/// <summary>
/// The configuration class for DataWriterQos
/// </summary>
public class PublisherQosConfiguration extends BaseConfiguration {
    /// <summary>
    /// Base property name of all specific configuration key of <see
    /// cref="PublisherQosConfiguration"/>
    /// </summary>
    public static final String PublisherQosBasePropertyKey = "datadistributionmanager.opendds.publisherqos";

    /// <summary>
    /// Initialize a <see cref="PublisherQosConfiguration"/>
    /// </summary>
    public PublisherQosConfiguration() {
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
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", PublisherQosBasePropertyKey);
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