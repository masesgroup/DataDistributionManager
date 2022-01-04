/*
*  Copyright 2022 MASES s.r.l.
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

import java.util.ArrayList;

public class OpenDDSChannelConfiguration extends OpenDDSConfiguration {

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link OpenDDSConfiguration} to duplicate
     */
    public OpenDDSChannelConfiguration(OpenDDSConfiguration originalConf) {
        super(originalConf);
    }

    /**
     * The configuration class for TopicQos. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public TopicQosConfiguration TopicQos;

    /**
     * The configuration class for PublisherQos. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public PublisherQosConfiguration PublisherQos;

    /**
     * The configuration class for SubscriberQos. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public SubscriberQosConfiguration SubscriberQos;

    /**
     * The configuration class for DataReaderQos. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public DataReaderQosConfiguration DataReaderQos;

    /**
     * The configuration class for DataWriterQos. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public DataWriterQosConfiguration DataWriterQos;

    /**
     * Creates configuration for QoS policies
     * 
     * @param parameters Input parameters
     * @return updated parameters
     */
    protected String[] PolicyBuilder(String[] parameters) {
        ArrayList<String> lst = new ArrayList<String>();
        if (TopicQos != null) {
            for (String value : TopicQos.getConfiguration()) {
                lst.add(value);
            }
        }
        if (PublisherQos != null) {
            for (String value : PublisherQos.getConfiguration()) {
                lst.add(value);
            }
        }
        if (SubscriberQos != null) {
            for (String value : SubscriberQos.getConfiguration()) {
                lst.add(value);
            }
        }
        if (DataReaderQos != null) {
            for (String value : DataReaderQos.getConfiguration()) {
                lst.add(value);
            }
        }
        if (DataWriterQos != null) {
            for (String value : DataWriterQos.getConfiguration()) {
                lst.add(value);
            }
        }

        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }
}
