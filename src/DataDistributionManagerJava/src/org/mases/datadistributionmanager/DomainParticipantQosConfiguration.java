package org.mases.datadistributionmanager;

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