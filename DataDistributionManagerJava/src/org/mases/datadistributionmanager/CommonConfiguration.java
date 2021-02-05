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

public class CommonConfiguration implements IConfiguration {
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

    /// <summary>
    /// The list of key/value pairs
    /// </summary>
    protected HashMap<String, String> keyValuePair = new HashMap<String, String>();

    /// <summary>
    /// Initialize a new <see cref="CommonConfiguration"/> with <paramref
    /// name="protocol"/> and <paramref name="protolib"/>
    /// </summary>
    /// <param name="protocol">The protocol to use</param>
    /// <param name="protolib">Protocol library</param>
    public CommonConfiguration(String protocol, String protolib) {
        setProtocol(protocol);
        setProtocolLibrary(protolib);
    }

    /// <summary>
    /// The protocol to use (e.g. kafka, opendds)
    /// </summary>
    public String getProtocol() {
        String value = keyValuePair.get(ProtocolKey);
        return (value == null) ? "" : value;
    }

    public void setProtocol(String protocol) {
        keyValuePair.put(ProtocolKey, protocol);
    }

    /// <summary>
    /// The protocol library to use
    /// </summary>
    public String getProtocolLibrary() {
        String value = keyValuePair.get(ProtocolLibraryKey);
        return (value == null) ? "" : value;
    }

    /// <summary>
    /// The protocol library to use
    /// </summary>
    public void setProtocolLibrary(String protolib) {
        keyValuePair.put(ProtocolLibraryKey, protolib);
    }

    /// <summary>
    /// The max message size managed
    /// </summary>
    public Integer getMaxMessageSize() {
        String value = keyValuePair.get(MaxMessageSizeKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The max message size managed
    /// </summary>
    public void setMaxMessageSize(Integer msgSize) {
        keyValuePair.put(MaxMessageSizeKey, msgSize.toString());
    }

    /// <summary>
    /// The timeout on create channel
    /// </summary>
    public Integer getCreateChannelTimeout() {
        String value = keyValuePair.get(CreateChannelTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on create channel
    /// </summary>
    public void setCreateChannelTimeout(Integer timeout) {
        keyValuePair.put(CreateChannelTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on server lost
    /// </summary>
    public Integer getServerLostTimeout() {
        String value = keyValuePair.get(ServerLostTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on server lost
    /// </summary>
    public void setServerLostTimeout(Integer timeout) {
        keyValuePair.put(ServerLostTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on seek channel
    /// </summary>
    public Integer getChannelSeekTimeout() {
        String value = keyValuePair.get(ChannelSeekTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on seek channel
    /// </summary>
    public void setChannelSeekTimeout(Integer timeout) {
        keyValuePair.put(ChannelSeekTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on channel receive
    /// </summary>
    public Integer getReceiveTimeout() {
        String value = keyValuePair.get(ReceiveTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on channel receive
    /// </summary>
    public void setReceiveSeekTimeout(Integer timeout) {
        keyValuePair.put(ReceiveTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on channel keep alive
    /// </summary>
    public Integer getKeepAliveTimeout() {
        String value = keyValuePair.get(KeepAliveTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on channel keep alive
    /// </summary>
    public void setKeepAliveTimeout(Integer timeout) {
        keyValuePair.put(KeepAliveTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on channel message consume
    /// </summary>
    public Integer getConsumerTimeout() {
        String value = keyValuePair.get(ConsumerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on channel message consume
    /// </summary>
    public void setConsumerTimeout(Integer timeout) {
        keyValuePair.put(ConsumerTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on channel message produce
    /// </summary>
    public Integer getProducerTimeout() {
        String value = keyValuePair.get(ProducerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /// <summary>
    /// The timeout on channel message produce
    /// </summary>
    public void ProducerTimeout(Integer timeout) {
        keyValuePair.put(ProducerTimeoutKey, timeout.toString());
    }

    /// <summary>
    /// The timeout on channel message produce
    /// </summary>
    public boolean getCommitSync() {
        String value = keyValuePair.get(ProducerTimeoutKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /// <summary>
    /// The timeout on channel message produce
    /// </summary>
    public void setCommitSync(Boolean value) {
        keyValuePair.put(ProducerTimeoutKey, value.toString());
    }

    /// <summary>
    /// Checks the configuration for mandatory information
    /// </summary>
    protected void CheckConfiguration() throws IllegalArgumentException {
        if (!keyValuePair.containsKey(ProtocolKey) && !keyValuePair.containsKey(ProtocolLibraryKey)) {
            throw new IllegalArgumentException("Missing Protocol or ProtocolLibrary");
        }
    }

    /// <see cref="IConfiguration.Configuration"/>
    public String[] getConfiguration() throws IllegalArgumentException {
        ArrayList<String> lst = new ArrayList<String>();
        for (String key : keyValuePair.keySet()) {
            lst.add(String.format("%s=%s", key, keyValuePair.get(key)));
        }
        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }
}