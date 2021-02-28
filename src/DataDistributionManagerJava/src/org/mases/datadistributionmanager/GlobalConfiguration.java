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

import java.util.HashMap;

/**
 * The global configuration class
 */
public abstract class GlobalConfiguration extends BaseConfiguration {
    public static final String ProtocolKey = "datadistributionmanager.common.protocol";
    public static final String ProtocolLibraryKey = "datadistributionmanager.common.protolib";
    public static final String MaxMessageSizeKey = "datadistributionmanager.maxmessagesize";
    public static final String ServerLostTimeoutKey = "datadistributionmanager.timeout.serverlost";
    public static final String GlobalLogLevelKey = "datadistributionmanager.loglevel.global";

    GlobalConfiguration(String protocol, String protolib) {
        setProtocol(protocol);
        setProtocolLibrary(protolib);
    }

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link IConfiguration} to duplicate
     */
    GlobalConfiguration(IConfiguration originalConf) {
        GlobalConfiguration conf = (GlobalConfiguration) originalConf;
        if (conf != null) {
            keyValuePair = new HashMap<String, String>(conf.keyValuePair);
        }
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
     * The global log value
     * 
     * @return The global log value
     */
    public DDM_LOG_LEVEL getGlobalLogLevel() {
        String value = keyValuePair.get(GlobalLogLevelKey);
        return DDM_LOG_LEVEL.valueOfLabel(value);
    }

    /**
     * The global log value
     * 
     * @param level The global log value
     */
    public void setGlobalLogLevel(DDM_LOG_LEVEL level) {
        keyValuePair.put(GlobalLogLevelKey, level.toIntString());
    }

    /**
     * Checks the configuration for mandatory information
     */
    protected void CheckConfiguration() throws IllegalArgumentException {
        if (!keyValuePair.containsKey(ProtocolKey) && !keyValuePair.containsKey(ProtocolLibraryKey)) {
            throw new IllegalArgumentException("Missing Protocol or ProtocolLibrary");
        }
    }
}