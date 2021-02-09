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

import java.util.ArrayList;
import java.util.HashMap;

/**
 * The general configuration class
 */
public abstract class CommonConfiguration implements IConfiguration {
    final String ProtocolKey = "datadistributionmanager.common.protocol";
    final String ProtocolLibraryKey = "datadistributionmanager.common.protolib";
    final String MaxMessageSizeKey = "datadistributionmanager.maxmessagesize";
    final String CreateChannelTimeoutKey = "datadistributionmanager.timeout.createchannel";
    final String ServerLostTimeoutKey = "datadistributionmanager.timeout.serverlost";
    final String ChannelSeekTimeoutKey = "datadistributionmanager.timeout.channelseek";
    final String ReceiveTimeoutKey = "datadistributionmanager.timeout.receive";
    final String KeepAliveTimeoutKey = "datadistributionmanager.timeout.keepalive";
    final String ConsumerTimeoutKey = "datadistributionmanager.timeout.consumer";
    final String ProducerTimeoutKey = "datadistributionmanager.timeout.producer";
    final String CommitSyncKey = "datadistributionmanager.commit.sync";

    /**
     * The list of key/value pairs
     */
    protected HashMap<String, String> keyValuePair = new HashMap<String, String>();

    CommonConfiguration(String protocol, String protolib) {
        setProtocol(protocol);
        setProtocolLibrary(protolib);
    }

    /**
     * The protocol to use (e.g. kafka, opendds)
     * 
     * @return The protocol
     */
    public String getProtocol() {
        String value = keyValuePair.get(ProtocolKey);
        return (value == null) ? "" : value;
    }

    /**
     * The protocol to use (e.g. kafka, opendds)
     * 
     * @param protocol The protocol
     */
    public void setProtocol(String protocol) {
        keyValuePair.put(ProtocolKey, protocol);
    }

    /**
     * The protocol library to use
     * 
     * @return The protocol library
     */
    public String getProtocolLibrary() {
        String value = keyValuePair.get(ProtocolLibraryKey);
        return (value == null) ? "" : value;
    }

    /**
     * The protocol library to use
     * 
     * @param protolib The protocol library
     */
    public void setProtocolLibrary(String protolib) {
        keyValuePair.put(ProtocolLibraryKey, protolib);
    }

    /**
     * The max message size managed
     * 
     * @return The max message size
     */
    public Integer getMaxMessageSize() {
        String value = keyValuePair.get(MaxMessageSizeKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The max message size managed
     * 
     * @param msgSize The max message size
     */
    public void setMaxMessageSize(Integer msgSize) {
        keyValuePair.put(MaxMessageSizeKey, msgSize.toString());
    }

    /**
     * The timeout on create channel
     * 
     * @return The create channel timeout
     */
    public Integer getCreateChannelTimeout() {
        String value = keyValuePair.get(CreateChannelTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on create channel
     * 
     * @param timeout The create channel timeout
     */
    public void setCreateChannelTimeout(Integer timeout) {
        keyValuePair.put(CreateChannelTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on server lost
     * 
     * @return The server lost timeout
     */
    public Integer getServerLostTimeout() {
        String value = keyValuePair.get(ServerLostTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on server lost
     * 
     * @param timeout The server lost timeout
     */
    public void setServerLostTimeout(Integer timeout) {
        keyValuePair.put(ServerLostTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on seek channel
     * 
     * @return The seek timeout
     */
    public Integer getChannelSeekTimeout() {
        String value = keyValuePair.get(ChannelSeekTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on seek channel
     * 
     * @param timeout The seek timeout
     */
    public void setChannelSeekTimeout(Integer timeout) {
        keyValuePair.put(ChannelSeekTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel receive
     * 
     * @return The receive timeout
     */
    public Integer getReceiveTimeout() {
        String value = keyValuePair.get(ReceiveTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel receive
     * 
     * @param timeout The receive timeout
     */
    public void setReceiveTimeout(Integer timeout) {
        keyValuePair.put(ReceiveTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel keep alive
     * 
     * @return The keep-alive timeout
     */
    public Integer getKeepAliveTimeout() {
        String value = keyValuePair.get(KeepAliveTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel keep alive
     * 
     * @param timeout The keep-alive timeout
     */
    public void setKeepAliveTimeout(Integer timeout) {
        keyValuePair.put(KeepAliveTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel message consume
     * 
     * @return The consumer timeout
     */
    public Integer getConsumerTimeout() {
        String value = keyValuePair.get(ConsumerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel message consume
     * 
     * @param timeout The consumer timeout
     */
    public void setConsumerTimeout(Integer timeout) {
        keyValuePair.put(ConsumerTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel message produce
     * 
     * @return The producer timeout
     */
    public Integer getProducerTimeout() {
        String value = keyValuePair.get(ProducerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel message produce
     * 
     * @param timeout The producer timeout
     */
    public void setProducerTimeout(Integer timeout) {
        keyValuePair.put(ProducerTimeoutKey, timeout.toString());
    }

    /**
     * True to commit message in sync
     * 
     * @return True to commit message in sync
     */
    public boolean getCommitSync() {
        String value = keyValuePair.get(ProducerTimeoutKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to commit message in sync
     * 
     * @param value True to commit message in sync
     */
    public void setCommitSync(Boolean value) {
        keyValuePair.put(ProducerTimeoutKey, value.toString());
    }

    /**
     * Checks the configuration for mandatory information
     */
    protected void CheckConfiguration() throws IllegalArgumentException {
        if (!keyValuePair.containsKey(ProtocolKey) && !keyValuePair.containsKey(ProtocolLibraryKey)) {
            throw new IllegalArgumentException("Missing Protocol or ProtocolLibrary");
        }
    }

    public String[] getConfiguration() throws IllegalArgumentException {
        ArrayList<String> lst = new ArrayList<String>();
        for (String key : keyValuePair.keySet()) {
            lst.add(String.format("%s=%s", key, keyValuePair.get(key)));
        }
        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }
}