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

public class KafkaChannelConfiguration extends KafkaConfiguration {
    public static final String KafkaConfigurationBaseProperty = "datadistributionmanager.kafka.";
    public static final String ReplicationFactorKey = "datadistributionmanager.kafka.topic.replicationfactor";
    public static final String BootstrapBrokersKey = "datadistributionmanager.kafka.metadata.broker.list";
    public static final String DebugKey = "datadistributionmanager.kafka.debug";
    public static final String ClientIdKey = "datadistributionmanager.kafka.client.id";
    public static final String GroupIdKey = "datadistributionmanager.kafka.group.id";
    public static final String TopicCreateKey = "datadistributionmanager.kafka.topic.create";
    public static final String DumpMetadataKey = "datadistributionmanager.kafka.topic.dumpmetadata";

      /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link KafkaConfiguration} to duplicate
     */
    public KafkaChannelConfiguration(KafkaConfiguration originalConf) {
        super(originalConf);
    }  
    /**
     * 
     * @return The client id to be used
     */
    public String getClientId() {
        String value = keyValuePair.get(ClientIdKey);
        return (value == null) ? "" : value;
    }

    /**
     * The client id to be used
     * 
     * @param clientId The client id to be used
     */
    public void setClientId(String clientId) {
        keyValuePair.put(ClientIdKey, clientId);
    }

    /**
     * 
     * @return The group id to be used
     */
    public String getGroupId() {
        String value = keyValuePair.get(GroupIdKey);
        return (value == null) ? "" : value;
    }

    /**
     * The group id to be used
     * 
     * @param groupId The group id to be used
     */
    public void setGroupId(String groupId) {
        keyValuePair.put(GroupIdKey, groupId);
    }

    /**
     * 
     * @return The debug level (e.g. metadata,topic,msg,broker)
     */
    public String getDebug() {
        String value = keyValuePair.get(DebugKey);
        return (value == null) ? "" : value;
    }

    /**
     * The debug level (e.g. metadata,topic,msg,broker)
     * 
     * @param debug The debug level (e.g. metadata,topic,msg,broker)
     */
    public void setDebug(String debug) {
        keyValuePair.put(DebugKey, debug);
    }

    /**
     * 
     * @return The bootstrap broker(s) to be used as Comma Separated Value(s)
     */
    public String getBootstrapBrokers() {
        String value = keyValuePair.get(BootstrapBrokersKey);
        return (value == null) ? "" : value;
    }

    /**
     * The bootstrap broker(s) to be used as Comma Separated Value(s)
     * 
     * @param brokers The bootstrap broker(s) to be used as Comma Separated Value(s)
     */
    public void setBootstrapBrokers(String brokers) {
        keyValuePair.put(BootstrapBrokersKey, brokers);
    }

    /**
     * 
     * @return The replication factor to be used
     */
    public Integer getReplicationFactor() {
        String value = keyValuePair.get(ReplicationFactorKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The replication factor to be used
     * 
     * @param value The replication factor to be used
     */
    public void setReplicationFactor(Integer value) {
        keyValuePair.put(ReplicationFactorKey, value.toString());
    }

    /**
     * True to request a create topic
     * 
     * @return True to request a create topic
     */
    public boolean getTopicCreate() {
        String value = keyValuePair.get(TopicCreateKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to request a create topic
     * 
     * @param value True to request a create topic
     */
    public void setTopicCreate(Boolean value) {
        keyValuePair.put(TopicCreateKey, value.toString());
    }

    /**
     * True to dump metadata
     * 
     * @return True to dump metadata
     */
    public boolean getDumpMetadata() {
        String value = keyValuePair.get(DumpMetadataKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to dump metadata
     * 
     * @param value True to dump metadata
     */
    public void setDumpMetadata(Boolean value) {
        keyValuePair.put(DumpMetadataKey, value.toString());
    }

    /**
     * Generic getter for all configuration properties related to librdkafka (see
     * https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
     * 
     * @param property The property name
     * @return The property value
     */
    public String getProperty(String property) {
        String value = "";
        if (property.startsWith(KafkaConfigurationBaseProperty)) {
            if (keyValuePair.containsKey(property))
                value = keyValuePair.get(property);
        } else {
            if (keyValuePair.containsKey(KafkaConfigurationBaseProperty + property))
                value = keyValuePair.get(KafkaConfigurationBaseProperty + property);
        }
        return value;
    }

    /**
     * Generic setter for all configuration properties related to librdkafka (see
     * https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
     * 
     * @param property The property name
     * @param value    The property value
     */
    public void setProperty(String property, String value) {
        if (property.startsWith(KafkaConfigurationBaseProperty)) {
            keyValuePair.put(property, value);
        } else {
            keyValuePair.put(KafkaConfigurationBaseProperty + property, value);
        }
    }

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
