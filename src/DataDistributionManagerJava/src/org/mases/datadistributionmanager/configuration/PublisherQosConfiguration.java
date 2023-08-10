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
 * The configuration class for PublisherQos. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class PublisherQosConfiguration extends BaseConfiguration {
    public static final String PublisherQosBasePropertyKey = "datadistributionmanager.opendds.publisherqos";

    /**
     * Initialize a new {@link PublisherQosConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public PublisherQosConfiguration() {
    }

    /**
     * The configuration of {@link PresentationQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public PresentationQosPolicyConfiguration PresentationQosPolicy;

    /**
     * The configuration of {@link PartitionQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public PartitionQosPolicyConfiguration PartitionQosPolicy;
    /**
     * The configuration of {@link GroupDataQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public GroupDataQosPolicyConfiguration GroupDataQosPolicy;

    /**
     * The configuration of {@link EntityFactoryQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy;

    String[] replacer(String[] input) {
        String[] array = new String[input.length];
        for (int index = 0; index < input.length; index++) {
            array[index] = input[index].replace("datadistributionmanager.opendds.qos", PublisherQosBasePropertyKey);
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

    public String[] getConfiguration() {
        return PolicyBuilder(super.getConfiguration());
    }
}