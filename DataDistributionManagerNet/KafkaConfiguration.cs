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

using System;

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The configuration class for OpenDDS
    /// </summary>
    public class KafkaConfiguration : CommonConfiguration
    {
        const string KafkaConfigurationBaseProperty = "datadistributionmanager.kafka.";
        const string ReplicationFactorKey = "datadistributionmanager.kafka.topic.replicationfactor";
        const string BootstrapBrokersKey = "datadistributionmanager.kafka.metadata.broker.list";
        const string DebugKey = "datadistributionmanager.kafka.debug";
        const string ClientIdKey = "datadistributionmanager.kafka.client.id";
        const string GroupIdKey = "datadistributionmanager.kafka.group.id";

        /// <summary>
        /// Initialize a <see cref="KafkaConfiguration"/>
        /// </summary>
        public KafkaConfiguration()
             : base("kafka", "DataDistributionManagerKafka.dll")
        {
        }

        /// <summary>
        /// The client id to be used
        /// </summary>
        public string ClientId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ClientIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[ClientIdKey] = value;
            }
        }

        /// <summary>
        /// The group id to be used
        /// </summary>
        public string GroupId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(GroupIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[GroupIdKey] = value;
            }
        }

        /// <summary>
        /// The debug level (e.g. metadata,topic,msg,broker)
        /// </summary>
        public string Debug
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DebugKey, out value);
                return value;
            }
            set
            {
                keyValuePair[DebugKey] = value;
            }
        }

        /// <summary>
        /// The bootstrap broker(s) to be used as Comma Separated Value(s)
        /// </summary>
        public string BootstrapBrokers
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BootstrapBrokersKey, out value);
                return value;
            }
            set
            {
                keyValuePair[BootstrapBrokersKey] = value;
            }
        }

        /// <summary>
        /// The replication factor to be used
        /// </summary>
        public uint ReplicationFactor
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReplicationFactorKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ReplicationFactorKey] = value.ToString();
            }
        }

        /// <summary>
        /// Generic setter/getter for all configuration properties related to librdkafka (see https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
        /// </summary>
        /// <param name="property">The property name</param>
        /// <returns></returns>
        public string this[string property]
        {
            get
            {
                string value = string.Empty;
                if (property.StartsWith(KafkaConfigurationBaseProperty))
                {
                    keyValuePair.TryGetValue(property, out value);
                }
                else
                {
                    keyValuePair.TryGetValue(KafkaConfigurationBaseProperty + property, out value);
                }
                return value;
            }
            set
            {
                if (property.StartsWith(KafkaConfigurationBaseProperty))
                {
                    keyValuePair[property] = value;
                }
                else
                {
                    keyValuePair[KafkaConfigurationBaseProperty + property] = value;
                }
            }
        }

        /// <see cref="CommonConfiguration.CheckConfiguration"/>
        protected override void CheckConfiguration()
        {
            base.CheckConfiguration();
            if (!keyValuePair.ContainsKey(BootstrapBrokersKey))
            {
                throw new InvalidOperationException("Missing BootstrapBrokers");
            }
            if (!keyValuePair.ContainsKey(ClientIdKey))
            {
                throw new InvalidOperationException("Missing ClientId");
            }
            if (!keyValuePair.ContainsKey(GroupIdKey))
            {
                throw new InvalidOperationException("Missing GroupId");
            }
        }

        /// <see cref="IConfiguration.Configuration"/>
        public override string[] Configuration
        {
            get
            {
                CheckConfiguration();
                return base.Configuration;
            }
        }
    }
}
