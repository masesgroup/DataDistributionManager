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
    /**
     * Class managing OpenDDSArgs
     */
    public class OpenDDSArgsConfiguration extends BaseConfiguration {
        HashMap<String, String> commandLineKeyValuePair = new HashMap<String, String>();
        public static final String DCPSConfigFileKey = "DCPSConfigFile";
        public static final String DCPSTransportDebugLevelKey = "DCPSTransportDebugLevel";
        public static final String CommandLineKey = "datadistributionmanager.opendds.cmdlineargs";

        /**
         * Initialize a new instance of {@link OpenDDSArgsConfiguration}
         */
        public OpenDDSArgsConfiguration() {
        }

        /**
         * Initialize a new instance of {@link OpenDDSArgsConfiguration}
         * 
         * @param dCPSConfigFile The command line parameters of DCPSConfigFile
         */
        public OpenDDSArgsConfiguration(String dCPSConfigFile) {
            setDCPSConfigFile(dCPSConfigFile);
        }

        /**
         * The command line parameters of DCPSConfigFile
         * 
         * @return The command line parameters of DCPSConfigFile
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
         * @return The command line parameters of DCPSTransportDebugLevel
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
         *         -DCPSConfigFile dds_tcp_conf.ini -DCPSTransportDebugLevel 10)
         */
        public String getCommandLine() {
            String value = keyValuePair.get(CommandLineKey);
            return (value == null) ? "" : value;
        }

        /**
         * The command line parameters to initialize OpenDDS (e.g. -DCPSConfigFile
         * dds_tcp_conf.ini -DCPSTransportDebugLevel 10)
         * 
         * @param commandline The command line parameters to initialize OpenDDS (e.g.
         *                    -DCPSConfigFile dds_tcp_conf.ini -DCPSTransportDebugLevel
         *                    10). Note: Set a value to null remove the property
         */
        public void setCommandLine(String commandline) {
            if (commandline == null)
                keyValuePair.remove(CommandLineKey);
            else
                keyValuePair.put(CommandLineKey, commandline);
        }

        String commandLineBuilder() {
            StringBuilder sb = new StringBuilder();
            for (String key : commandLineKeyValuePair.keySet()) {
                sb.append(String.format("-%s %s ", key, commandLineKeyValuePair.get(key)));
            }
            return String.format("%s=%s", CommandLineKey, sb.toString());
        }

        protected void CheckConfiguration() {
            super.CheckConfiguration();
            if (!keyValuePair.containsKey(CommandLineKey) && commandLineKeyValuePair.size() == 0) {
                throw new IllegalArgumentException("Missing CommandLine");
            }
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

    /**
     * Class managing DCPSInfoRepo
     */
    public class DCPSInfoRepoConfiguration extends BaseConfiguration {
        public static final String DCPSInfoRepoAutostartKey = "datadistributionmanager.opendds.dcpsinforepo.autostart";
        public static final String DCPSInfoRepoLogOnApplicationKey = "datadistributionmanager.opendds.dcpsinforepo.logonapplication";
        public static final String DCPSInfoRepoCommandLineKey = "datadistributionmanager.opendds.dcpsinforepo.cmdlineargs";

        /**
         * Initialize a new instance of {@link DCPSInfoRepoConfiguration}
         */
        public DCPSInfoRepoConfiguration() {
        }

        /**
         * Initialize a new instance of {@link DCPSInfoRepoConfiguration}
         * 
         * @param autostart   Automatically starts DCPSInfoRepo
         * @param commandLine Command line to use with DCPSInfoRepo
         */
        public DCPSInfoRepoConfiguration(Boolean autostart, String commandLine) {
            setAutostart(autostart);
            setCommandLine(commandLine);
        }

        /**
         * Automatically start DCPSInfoRepo
         * 
         * @return Automatically start DCPSInfoRepo
         */
        public boolean getAutostart() {
            String value = keyValuePair.get(DCPSInfoRepoAutostartKey);
            return (value == null) ? false : Boolean.parseBoolean(value);
        }

        /**
         * Automatically start DCPSInfoRepo
         * 
         * @param value Automatically start DCPSInfoRepo
         */
        public void setAutostart(Boolean value) {
            keyValuePair.put(DCPSInfoRepoAutostartKey, value.toString());
        }

        /**
         * Emits log in the application log
         * 
         * @return Emits log in the application log
         */
        public boolean getLogOnApplication() {
            String value = keyValuePair.get(DCPSInfoRepoLogOnApplicationKey);
            return (value == null) ? false : Boolean.parseBoolean(value);
        }

        /**
         * Emits log in the application log
         * 
         * @param value Emits log in the application log
         */
        public void setLogOnApplication(Boolean value) {
            keyValuePair.put(DCPSInfoRepoLogOnApplicationKey, value.toString());
        }

        /**
         * The command line to be used on DCPSInfoRepo executable
         * 
         * @return The command line to be used on DCPSInfoRepo executable
         */
        public String getCommandLine() {
            String value = keyValuePair.get(DCPSInfoRepoCommandLineKey);
            return (value == null) ? "" : value;
        }

        /**
         * The command line to be used on DCPSInfoRepo executable
         * 
         * @param value The command line to be used on DCPSInfoRepo executable
         */
        public void setCommandLine(String value) {
            keyValuePair.put(DCPSInfoRepoCommandLineKey, value);
        }
    }

    public static final String DomainIdKey = "datadistributionmanager.opendds.domain_id";

    /**
     * Initialize a new instance of {@link OpenDDSConfiguration}
     */
    public OpenDDSConfiguration() {
        super("opendds", "DataDistributionManagerOpenDDS.dll");
    }

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link IConfiguration} to duplicate
     */
    public OpenDDSConfiguration(IConfiguration originalConf) {
        super(originalConf);
        OpenDDSConfiguration conf = (OpenDDSConfiguration) originalConf;
        if (conf != null) {
            setOpenDDSArgs(conf.getOpenDDSArgs());
            setDCPSInfoRepo(conf.getDCPSInfoRepo());
            setDomainParticipantQos(conf.getDomainParticipantQos());
        }

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

    OpenDDSArgsConfiguration openDDSArgsConfiguration = null;

    /**
     * The configuration of {@link OpenDDSArgsConfiguration}
     * 
     * @return The configuration of {@link OpenDDSArgsConfiguration}
     */
    public OpenDDSArgsConfiguration getOpenDDSArgs() {
        return openDDSArgsConfiguration;
    }

    /**
     * The configuration of {@link OpenDDSArgsConfiguration}
     * 
     * @param value The configuration of {@link OpenDDSArgsConfiguration}
     */
    public void setOpenDDSArgs(OpenDDSArgsConfiguration value) {
        openDDSArgsConfiguration = value;
    }

    DCPSInfoRepoConfiguration dCPSInfoRepoConfiguration = null;

    /**
     * The configuration of {@link DCPSInfoRepoConfiguration}
     * 
     * @return The configuration of {@link DCPSInfoRepoConfiguration}
     */
    public DCPSInfoRepoConfiguration getDCPSInfoRepo() {
        return dCPSInfoRepoConfiguration;
    }

    /**
     * The configuration of {@link DCPSInfoRepoConfiguration}
     * 
     * @param value The configuration of {@link DCPSInfoRepoConfiguration}
     */
    public void setDCPSInfoRepo(DCPSInfoRepoConfiguration value) {
        dCPSInfoRepoConfiguration = value;
    }

    DomainParticipantQosConfiguration domainParticipantQosConfiguration = null;

    /**
     * The configuration of {@link DomainParticipantQosConfiguration}
     * 
     * @return The configuration of {@link DomainParticipantQosConfiguration}
     */
    public DomainParticipantQosConfiguration getDomainParticipantQos() {
        return domainParticipantQosConfiguration;
    }

    /**
     * The configuration of {@link DomainParticipantQosConfiguration}
     * 
     * @param value The configuration of {@link DomainParticipantQosConfiguration}
     */
    public void setDomainParticipantQos(DomainParticipantQosConfiguration value) {
        domainParticipantQosConfiguration = value;
    }

    /**
     * Creates configuration for QoS policies
     * 
     * @param parameters input parameters
     * @return parameters with policies
     */
    protected String[] PolicyBuilder(String[] parameters) {
        IConfiguration conf = null;
        ArrayList<String> lst = new ArrayList<String>();
        for (String val : parameters) {
            lst.add(val);
        }
        conf = getOpenDDSArgs();
        if (conf != null)
            for (String val : conf.getConfiguration()) {
                lst.add(val);
            }
        conf = getDCPSInfoRepo();
        if (conf != null)
            for (String val : conf.getConfiguration()) {
                lst.add(val);
            }
        conf = getDomainParticipantQos();
        if (conf != null)
            for (String val : conf.getConfiguration()) {
                lst.add(val);
            }

        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }

    @Override
    public String[] getConfiguration() throws IllegalArgumentException {
        return PolicyBuilder(super.getConfiguration());
    }
}
