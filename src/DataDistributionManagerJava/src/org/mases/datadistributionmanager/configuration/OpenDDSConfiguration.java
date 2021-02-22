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

package org.mases.datadistributionmanager.configuration;

import org.mases.datadistributionmanager.BaseConfiguration;
import org.mases.datadistributionmanager.CommonConfiguration;
import org.mases.datadistributionmanager.IConfiguration;

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
            if (!keyValuePair.containsKey(CommandLineKey)) {
                if (commandLineKeyValuePair.size() == 0) {
                    throw new IllegalArgumentException("Missing CommandLine");
                } else if (!commandLineKeyValuePair.containsKey(DCPSConfigFileKey)) {
                    throw new IllegalArgumentException("Missing DCPSConfigFile");
                }
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

        HashMap<String, String> commandLineKeyValuePair = new HashMap<String, String>();
        /// <summary>
        /// Configuration key of <see cref="ORBSvcConf"/>
        /// </summary>
        public static final String DCPSInfoRepoORBSvcConfKey = "ORBSvcConf";
        /// <summary>
        /// Configuration key of <see cref="ORBEndpoint"/>
        /// </summary>
        public static final String DCPSInfoRepoORBEndpointKey = "ORBEndpoint";
        /// <summary>
        /// Configuration key of <see cref="ORBListenEndpoints"/>
        /// </summary>
        public static final String DCPSInfoRepoORBListenEndpointsKey = "ORBListenEndpoints";
        /// <summary>
        /// Configuration key of <see cref="IorFile"/>
        /// </summary>
        public static final String DCPSInfoRepoIorFileKey = "o";
        /// <summary>
        /// Configuration key of <see cref="NOBITS"/>
        /// </summary>
        public static final String DCPSInfoRepoNOBITSKey = "NOBITS";
        /// <summary>
        /// Configuration key of <see cref="ListeningAddress"/>
        /// </summary>
        public static final String DCPSInfoRepoListeningAddressKey = "a";
        /// <summary>
        /// Configuration key of <see cref="VerboseLogging"/>
        /// </summary>
        public static final String DCPSInfoRepoVerboseLoggingKey = "z";
        /// <summary>
        /// Configuration key of <see cref="PersistenceFile"/>
        /// </summary>
        public static final String DCPSInfoRepoPersistenceFileKey = "file";
        /// <summary>
        /// Configuration key of <see cref="Resurrect"/>
        /// </summary>
        public static final String DCPSInfoRepoResurrectKey = "r";
        /// <summary>
        /// Configuration key of <see cref="PersistenceReset"/>
        /// </summary>
        public static final String DCPSInfoRepoPersistenceResetKey = "reset";
        /// <summary>
        /// Configuration key of <see cref="FederatorConfig"/>
        /// </summary>
        public static final String DCPSInfoRepoFederatorConfigKey = "FederatorConfig";
        /// <summary>
        /// Configuration key of <see cref="FederationId"/>
        /// </summary>
        public static final String DCPSInfoRepoFederationIdKey = "FederationId";
        /// <summary>
        /// Configuration key of <see cref="FederateWith"/>
        /// </summary>
        public static final String DCPSInfoRepoFederateWithKey = "FederateWith";
        /// <summary>
        /// Configuration key of <see cref="ReassociateDelay"/>
        /// </summary>
        public static final String DCPSInfoRepoReassociateDelayKey = "ReassociateDelay";
        /// <summary>
        /// Configuration key of <see cref="DispatchingCheckDelay"/>
        /// </summary>
        public static final String DCPSInfoRepoDispatchingCheckDelayKey = "DispatchingCheckDelay";

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

        public String getORBSvcConf() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoORBSvcConfKey);
            return value;
        }

        public void setORBSvcConf(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoORBSvcConfKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoORBSvcConfKey, value);
        }

        public String getORBEndpoint() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoORBEndpointKey);
            return value;
        }

        public void setORBEndpoint(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoORBEndpointKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoORBEndpointKey, value);
        }

        public String getORBListenEndpoints() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoORBListenEndpointsKey);
            return value;
        }

        public void setORBListenEndpoints(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoORBListenEndpointsKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoORBListenEndpointsKey, value);
        }

        public String getIorFile() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoIorFileKey);
            return value;
        }

        public void setIorFile(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoIorFileKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoIorFileKey, value);
        }

        public boolean getNOBITS() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoNOBITSKey);
            if (value != null && value == "1")
                return true;
            return false;
        }

        public void setNOBITS(boolean value) {
            if (value == true)
                commandLineKeyValuePair.put(DCPSInfoRepoNOBITSKey, "1");
            else
                commandLineKeyValuePair.put(DCPSInfoRepoNOBITSKey, "0");
        }

        public String getListeningAddress() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoListeningAddressKey);
            return value;
        }

        public void setListeningAddress(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoListeningAddressKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoListeningAddressKey, value);
        }

        public boolean getVerboseLogging() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoVerboseLoggingKey);
            if (value != null && value == "1")
                return true;
            return false;
        }

        public void setVerboseLogging(boolean value) {
            if (value == true)
                commandLineKeyValuePair.put(DCPSInfoRepoVerboseLoggingKey, "1");
            else
                commandLineKeyValuePair.put(DCPSInfoRepoVerboseLoggingKey, "0");
        }

        public String getPersistenceFile() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoPersistenceFileKey);
            return value;
        }

        public void setPersistenceFile(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoPersistenceFileKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoPersistenceFileKey, value);
        }

        public boolean getResurrect() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoResurrectKey);
            if (value != null && value == "1")
                return true;
            return false;
        }

        public void setResurrect(boolean value) {
            if (value == true)
                commandLineKeyValuePair.put(DCPSInfoRepoResurrectKey, "1");
            else
                commandLineKeyValuePair.put(DCPSInfoRepoResurrectKey, "0");
        }

        public boolean getPersistenceReset() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoPersistenceResetKey);
            if (value != null && value == "1")
                return true;
            return false;
        }

        public void setPersistenceReset(boolean value) {
            if (value == true)
                commandLineKeyValuePair.put(DCPSInfoRepoPersistenceResetKey, "1");
            else
                commandLineKeyValuePair.put(DCPSInfoRepoPersistenceResetKey, "0");
        }

        public String getFederatorConfig() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoFederatorConfigKey);
            return value;
        }

        public void setFederatorConfig(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoFederatorConfigKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoFederatorConfigKey, value);
        }

        public Integer getFederationId() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoFederationIdKey);
            return (value == null) ? 0 : Integer.parseInt(value);
        }

        public void setFederationId(Integer value) {
            commandLineKeyValuePair.put(DCPSInfoRepoFederationIdKey, value.toString());
        }

        public String getFederateWith() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoFederateWithKey);
            return value;
        }

        public void setFederateWith(String value) {
            if (value == null)
                commandLineKeyValuePair.remove(DCPSInfoRepoFederateWithKey);
            else
                commandLineKeyValuePair.put(DCPSInfoRepoFederateWithKey, value);
        }

        public Integer getReassociateDelay() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoReassociateDelayKey);
            return (value == null) ? 0 : Integer.parseInt(value);
        }

        public void setReassociateDelay(Integer value) {
            commandLineKeyValuePair.put(DCPSInfoRepoReassociateDelayKey, value.toString());
        }

        public Integer getDispatchingCheckDelay() {
            String value = commandLineKeyValuePair.get(DCPSInfoRepoDispatchingCheckDelayKey);
            return (value == null) ? 0 : Integer.parseInt(value);
        }

        public void setDispatchingCheckDelay(Integer value) {
            commandLineKeyValuePair.put(DCPSInfoRepoDispatchingCheckDelayKey, value.toString());
        }

        String commandLineBuilder() {
            StringBuilder sb = new StringBuilder();
            for (String key : commandLineKeyValuePair.keySet()) {
                sb.append(String.format("-%s %s ", key, commandLineKeyValuePair.get(key)));
            }
            return String.format("%s=%s", DCPSInfoRepoCommandLineKey, sb.toString());
        }

        protected void CheckConfiguration() {
            super.CheckConfiguration();
            if (!keyValuePair.containsKey(DCPSInfoRepoCommandLineKey)) {
                if (commandLineKeyValuePair.size() == 0) {
                    throw new IllegalArgumentException("Missing CommandLine");
                }
            }
        }

        @Override
        public String[] getConfiguration() throws IllegalArgumentException {
            CheckConfiguration();
            if (!keyValuePair.containsKey(DCPSInfoRepoCommandLineKey)) {
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

    public static final String DomainIdKey = "datadistributionmanager.opendds.domain_id";
    public static final String DCPSDebugLevelKey = "datadistributionmanager.opendds.dcps.debug_level";

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
     * The domain id parameter to initialize OpenDDS
     * 
     * @return The domain id parameter to initialize OpenDDS
     */
    public Integer getDomainId() {
        String value = keyValuePair.get(DomainIdKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The domain id parameter to initialize OpenDDS
     * 
     * @param value The domain id parameter to initialize OpenDDS
     */
    public void setDomainId(Integer value) {
        keyValuePair.put(DomainIdKey, value.toString());
    }

    /**
     * Logging verbosity level.
     * 
     * @return Logging verbosity level.
     */
    public Integer getDCPSDebugLevel() {
        String value = keyValuePair.get(DCPSDebugLevelKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * Logging verbosity level.
     * 
     * @param value Logging verbosity level.
     */
    public void setDCPSDebugLevel(Integer value) {
        switch (value) {
            case 0: // - logs that indicate serious errors that are not indicated by return codes
                    // (almost none)
            case 1: // - logs that should happen once per process or are warnings
            case 2: // -logs that should happen once per DDS entity
            case 4: // -logs that are related to administrative interfaces
            case 6: // -logs that should happen every Nth sample write/read
            case 8: // -logs that should happen once per sample write/read
            case 10: // -logs that may happen more than once per sample write/read
                break;
            default:
                throw new IllegalArgumentException("Value is not in correct range");
        }
        keyValuePair.put(DCPSDebugLevelKey, value.toString());
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
