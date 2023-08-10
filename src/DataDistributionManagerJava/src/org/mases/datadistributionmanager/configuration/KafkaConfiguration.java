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

import org.mases.datadistributionmanager.CommonConfiguration;
import org.mases.datadistributionmanager.IConfiguration;

/**
 * The configuration class for Kafka
 */
public class KafkaConfiguration extends CommonConfiguration {

    /**
     * Initialize a new instance of {@link KafkaConfiguration}
     */
    public KafkaConfiguration() {
        super("kafka", "DataDistributionManagerKafka.dll");
    }

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link IConfiguration} to duplicate
     */
    public KafkaConfiguration(IConfiguration originalConf) {
        super(originalConf);
    }

}
