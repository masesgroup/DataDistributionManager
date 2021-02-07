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
 * The configuration class for OpenDDS
 */
public class OpenDDSConfiguration extends CommonConfiguration {
    HashMap<String, String> commandLineKeyValuePair = new HashMap<String, String>();

    final String CommandLineKey = "datadistributionmanager.opendds.cmdlineargs";
    final String DomainIdKey = "datadistributionmanager.opendds.domain_id";
    final String DCPSConfigFileKey = "DCPSConfigFile";
    final String DCPSTransportDebugLevelKey = "DCPSTransportDebugLevel";

    /**
     * Ctor
     */
    public OpenDDSConfiguration() {
        super("opendds", "DataDistributionManagerOpenDDS.dll");
    }

    /**
     * The command line parameters of DCPSConfigFile
     * 
     * @return
     */
    public String getDCPSConfigFile() {
        String value = commandLineKeyValuePair.get(DCPSConfigFileKey);
        return (value == null) ? "" : value;
    }

    /**
     * The command line parameters of DCPSConfigFile
     * 
     * @param value The command line parameters of DCPSConfigFile
     */
    public void setDCPSConfigFile(String value) {
        commandLineKeyValuePair.put(DCPSConfigFileKey, value);
    }

    /**
     * The command line parameters of DCPSTransportDebugLevel
     * 
     * @return
     */
    public Integer getDCPSTransportDebugLevel() {
        String value = commandLineKeyValuePair.get(DCPSTransportDebugLevelKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The command line parameters of DCPSTransportDebugLevel
     * 
     * @param value The command line parameters of DCPSTransportDebugLevel
     */
    public void setDCPSTransportDebugLevel(Integer value) {
        commandLineKeyValuePair.put(DCPSTransportDebugLevelKey, value.toString());
    }

    /**
     * 
     * @return The command line parameters to initialize OpenDDS (e.g.
     *         -DCPSConfigFile ../../Configuration/dds_tcp_conf.ini
     *         -DCPSTransportDebugLevel 10)
     */
    public String getCommandLine() {
        String value = keyValuePair.get(CommandLineKey);
        return (value == null) ? "" : value;
    }

    /**
     * The command line parameters to initialize OpenDDS (e.g. -DCPSConfigFile
     * ../../Configuration/dds_tcp_conf.ini -DCPSTransportDebugLevel 10)
     * 
     * @param commandline The command line parameters to initialize OpenDDS (e.g.
     *                    -DCPSConfigFile ../../Configuration/dds_tcp_conf.ini
     *                    -DCPSTransportDebugLevel 10). Note: Set a value to null
     *                    remove the property
     */
    public void setCommandLine(String commandline) {
        if (commandline == null)
            keyValuePair.remove(CommandLineKey);
        else
            keyValuePair.put(CommandLineKey, commandline);
    }

    /**
     * 
     * @return The domain id parameter to initialize OpenDDS
     */
    public String getDomainId() {
        String value = keyValuePair.get(DomainIdKey);
        return (value == null) ? "" : value;
    }

    /**
     * The domain id parameter to initialize OpenDDS
     * 
     * @param value The domain id parameter to initialize OpenDDS
     */
    public void setDomainId(Integer value) {
        keyValuePair.put(DomainIdKey, value.toString());
    }

    @Override
    protected void CheckConfiguration() throws IllegalArgumentException {
        super.CheckConfiguration();
        if (!keyValuePair.containsKey(CommandLineKey) && commandLineKeyValuePair.size() == 0) {
            throw new IllegalArgumentException("Missing CommandLine");
        }
    }

    String commandLineBuilder() {
        StringBuilder sb = new StringBuilder();
        for (String key : commandLineKeyValuePair.keySet()) {
            sb.append(String.format("-%s %s ", key, commandLineKeyValuePair.get(key)));
        }
        return String.format("%s=%s", CommandLineKey, sb.toString());
    }

    @Override
    public String[] getConfiguration() throws IllegalArgumentException {
        CheckConfiguration();
        if (!keyValuePair.containsKey(CommandLineKey)) {
            ArrayList<String> lst = new ArrayList<String>();
            for (String val : super.getConfiguration()) {
                lst.add(val);
            }
            lst.add(commandLineBuilder());
            String[] array = new String[lst.size()];
            return lst.toArray(array);
        } else
            return super.getConfiguration();
    }
}
