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
    /// The configuration class for Kafka
    /// </summary>
    public class KafkaConfiguration : CommonConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="KafkaConfiguration"/>
        /// </summary>
        public const string KafkaConfigurationBasePropertyKey = "datadistributionmanager.kafka.";
        /// <summary>
        /// Configuration key of <see cref="ReplicationFactor"/>
        /// </summary>
        public const string ReplicationFactorKey = "datadistributionmanager.kafka.topic.replicationfactor";
        /// <summary>
        /// Configuration key of <see cref="BootstrapBrokers"/>
        /// </summary>
        public const string BootstrapBrokersKey = "datadistributionmanager.kafka.metadata.broker.list";
        /// <summary>
        /// Configuration key of <see cref="Debug"/>
        /// </summary>
        public const string DebugKey = "datadistributionmanager.kafka.debug";
        /// <summary>
        /// Configuration key of <see cref="ClientId"/>
        /// </summary>
        public const string ClientIdKey = "datadistributionmanager.kafka.client.id";
        /// <summary>
        /// Configuration key of <see cref="GroupId"/>
        /// </summary>
        public const string GroupIdKey = "datadistributionmanager.kafka.group.id";
        /// <summary>
        /// Configuration key of <see cref="TopicCreate"/>
        /// </summary>
        public const string TopicCreateKey = "datadistributionmanager.kafka.topic.create";
        /// <summary>
        /// Configuration key of <see cref="DumpMetadata"/>
        /// </summary>
        public const string DumpMetadataKey = "datadistributionmanager.kafka.topic.dumpmetadata";

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
        /// True to request a create topic
        /// </summary>
        public bool TopicCreate
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicCreateKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[TopicCreateKey] = value.ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// True to dump metadata
        /// </summary>
        public bool DumpMetadata
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DumpMetadataKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[DumpMetadataKey] = value.ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// Generic setter/getter for all configuration properties related to librdkafka (see https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
        /// </summary>
        /// <param name="property">The property name</param>
        /// <returns>The property value</returns>
        public string this[string property]
        {
            get
            {
                string value = string.Empty;
                if (property.StartsWith(KafkaConfigurationBasePropertyKey))
                {
                    keyValuePair.TryGetValue(property, out value);
                }
                else
                {
                    keyValuePair.TryGetValue(KafkaConfigurationBasePropertyKey + property, out value);
                }
                return value;
            }
            set
            {
                if (property.StartsWith(KafkaConfigurationBasePropertyKey))
                {
                    keyValuePair[property] = value;
                }
                else
                {
                    keyValuePair[KafkaConfigurationBasePropertyKey + property] = value;
                }
            }
        }

        /// <see cref="GlobalConfiguration.CheckConfiguration"/>
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
