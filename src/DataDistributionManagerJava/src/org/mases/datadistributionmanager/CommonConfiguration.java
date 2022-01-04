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

package org.mases.datadistributionmanager;

/**
 * The common configuration class
 */
public abstract class CommonConfiguration extends GlobalConfiguration {
    public static final String CreateChannelTimeoutKey = "datadistributionmanager.timeout.createchannel";
    public static final String ChannelSeekTimeoutKey = "datadistributionmanager.timeout.channelseek";
    public static final String ReceiveTimeoutKey = "datadistributionmanager.timeout.receive";
    public static final String KeepAliveTimeoutKey = "datadistributionmanager.timeout.keepalive";
    public static final String ConsumerTimeoutKey = "datadistributionmanager.timeout.consumer";
    public static final String ProducerTimeoutKey = "datadistributionmanager.timeout.producer";
    public static final String CommitTimeoutKey = "datadistributionmanager.timeout.commit";
    public static final String CommitSyncKey = "datadistributionmanager.commit.sync";
    public static final String EventSyncKey = "datadistributionmanager.event.sync";
    public static final String InitialOffsetKey = "datadistributionmanager.initial_offset";

    /**
     * Initialize a new instance of {@link CommonConfiguration}
     * 
     * @param protocol The protocol to use
     * @param protolib The protocol library to use
     */
    public CommonConfiguration(String protocol, String protolib) {
        super(protocol, protolib);
    }

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link IConfiguration} to duplicate
     */
    public CommonConfiguration(IConfiguration originalConf) {
        super(originalConf);
    }

    /**
     * The timeout on create channel
     * 
     * @return The create channel timeout in ms
     */
    public Integer getCreateChannelTimeout() {
        String value = keyValuePair.get(CreateChannelTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on create channel
     * 
     * @param timeout The create channel timeout in ms
     */
    public void setCreateChannelTimeout(Integer timeout) {
        keyValuePair.put(CreateChannelTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on seek channel
     * 
     * @return The seek timeout in ms
     */
    public Integer getChannelSeekTimeout() {
        String value = keyValuePair.get(ChannelSeekTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on seek channel
     * 
     * @param timeout The seek timeout in ms
     */
    public void setChannelSeekTimeout(Integer timeout) {
        keyValuePair.put(ChannelSeekTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel receive
     * 
     * @return The receive timeout in ms
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
     * @return The keep-alive timeout in ms
     */
    public Integer getKeepAliveTimeout() {
        String value = keyValuePair.get(KeepAliveTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel keep alive
     * 
     * @param timeout The keep-alive timeout in ms
     */
    public void setKeepAliveTimeout(Integer timeout) {
        keyValuePair.put(KeepAliveTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel message consume
     * 
     * @return The consumer timeout in ms
     */
    public Integer getConsumerTimeout() {
        String value = keyValuePair.get(ConsumerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel message consume
     * 
     * @param timeout The consumer timeout in ms
     */
    public void setConsumerTimeout(Integer timeout) {
        keyValuePair.put(ConsumerTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel message produce
     * 
     * @return The producer timeout in ms
     */
    public Integer getProducerTimeout() {
        String value = keyValuePair.get(ProducerTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel message produce
     * 
     * @param timeout The producer timeout in ms
     */
    public void setProducerTimeout(Integer timeout) {
        keyValuePair.put(ProducerTimeoutKey, timeout.toString());
    }

    /**
     * The timeout on channel message commit
     * 
     * @return The commit timeout in ms
     */
    public Integer getCommitTimeout() {
        String value = keyValuePair.get(CommitTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout on channel message commit
     * 
     * @param timeout The commit timeout in ms
     */
    public void setCommitTimeout(Integer timeout) {
        keyValuePair.put(CommitTimeoutKey, timeout.toString());
    }

    /**
     * True to commit message in sync
     * 
     * @return True to commit message in sync
     */
    public boolean getCommitSync() {
        String value = keyValuePair.get(CommitSyncKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to commit message in sync
     * 
     * @param value True to commit message in sync
     */
    public void setCommitSync(Boolean value) {
        keyValuePair.put(CommitSyncKey, value.toString());
    }

    /**
     * True to emit message receive events in sync
     * 
     * @return True to emit message receive events in sync
     */
    public boolean getEventSync() {
        String value = keyValuePair.get(EventSyncKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to emit message receive events in sync
     * 
     * @param value True to emit message receive events in sync
     */
    public void setEventSync(Boolean value) {
        keyValuePair.put(EventSyncKey, value.toString());
    }

    /**
     * The initial offset of the channel
     * 
     * @return The initial offset of the channel
     */
    public Long getInitialOffset() {
        String value = keyValuePair.get(InitialOffsetKey);
        return (value == null) ? 0 : Long.parseLong(value);
    }

    /**
     * The initial offset of the channel
     * 
     * @param offset The initial offset of the channel
     */
    public void setInitialOffset(Long offset) {
        keyValuePair.put(InitialOffsetKey, offset.toString());
    }
}