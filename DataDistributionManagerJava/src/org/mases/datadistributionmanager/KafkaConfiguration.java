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

package org.mases.datadistributionmanager;

public class KafkaConfiguration extends CommonConfiguration {
    final String KafkaConfigurationBaseProperty = "datadistributionmanager.kafka.";
    final String ReplicationFactorKey = "datadistributionmanager.kafka.topic.replicationfactor";
    final String BootstrapBrokersKey = "datadistributionmanager.kafka.metadata.broker.list";
    final String DebugKey = "datadistributionmanager.kafka.debug";
    final String ClientIdKey = "datadistributionmanager.kafka.client.id";
    final String GroupIdKey = "datadistributionmanager.kafka.group.id";

    public KafkaConfiguration() {
        super("kafka", "DataDistributionManagerKafka.dll");
    }

    /// <summary>
    /// The client id to be used
    /// </summary>
    public String getClientId() {
        String value = keyValuePair.get(ClientIdKey);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// The client id to be used
    /// </summary>
    public void ClientId(String clientId) {
        keyValuePair.put(ClientIdKey, clientId);
    }

    /// <summary>
    /// The group id to be used
    /// </summary>
    public String getGroupId() {
        String value = keyValuePair.get(GroupIdKey);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// The group id to be used
    /// </summary>
    public void setGroupId(String groupId) {
        keyValuePair.put(GroupIdKey, groupId);
    }

    /// <summary>
    /// The debug level (e.g. metadata,topic,msg,broker)
    /// </summary>
    public String getDebug() {
        String value = keyValuePair.get(DebugKey);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// The debug level (e.g. metadata,topic,msg,broker)
    /// </summary>
    public void setDebug(String debug) {
        keyValuePair.put(DebugKey, debug);
    }

    /// <summary>
    /// The bootstrap broker(s) to be used as Comma Separated Value(s)
    /// </summary>
    public String getBootstrapBrokers() {
        String value = keyValuePair.get(BootstrapBrokersKey);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// The bootstrap broker(s) to be used as Comma Separated Value(s)
    /// </summary>
    public void setBootstrapBrokers(String brokers) {
        keyValuePair.put(BootstrapBrokersKey, brokers);
    }

    /// <summary>
    /// The replication factor to be used
    /// </summary>
    public Integer getReplicationFactor() {
        String value = keyValuePair.get(ReplicationFactorKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The replication factor to be used
    /// </summary>
    public void setReplicationFactor(Integer value) {
        keyValuePair.put(ProducerTimeoutKey, value.toString());
    }

    /// <summary>
    /// Generic setter/getter for all configuration properties related to librdkafka
    /// (see https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
    /// </summary>
    /// <param name="property">The property name</param>
    /// <returns></returns>
    public String getProperty(String property) {
        String value = keyValuePair.get(property);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// Generic setter/getter for all configuration properties related to librdkafka
    /// (see https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
    /// </summary>
    /// <param name="property">The property name</param>
    /// <returns></returns>
    public void setProperty(String property, String value) {
        keyValuePair.put(property, value.toString());
    }

    /// <see cref="CommonConfiguration.CheckConfiguration"/>
    @Override
    protected void CheckConfiguration() throws IllegalArgumentException {
        super.CheckConfiguration();
        if (!keyValuePair.containsKey(BootstrapBrokersKey)) {
            throw new IllegalArgumentException("Missing BootstrapBrokers");
        }
        if (!keyValuePair.containsKey(ClientIdKey)) {
            throw new IllegalArgumentException("Missing ClientId");
        }
        if (!keyValuePair.containsKey(GroupIdKey)) {
            throw new IllegalArgumentException("Missing GroupId");
        }
    }

    @Override
    public String[] getConfiguration() throws IllegalArgumentException {
        CheckConfiguration();
        return super.getConfiguration();
    }
}
