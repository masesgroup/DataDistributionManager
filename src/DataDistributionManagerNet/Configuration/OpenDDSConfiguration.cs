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
using System.Collections.Generic;
using System.Text;

namespace MASES.DataDistributionManager.Bindings.Configuration
{
    /// <summary>
    /// The configuration class for OpenDDS
    /// </summary>
    public class OpenDDSConfiguration : CommonConfiguration
    {
        #region OpenDDSArgsConfiguration
        /// <summary>
        /// Class managing OpenDDSArgs. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public class OpenDDSArgsConfiguration : BaseConfiguration
        {
            Dictionary<string, string> commandLineKeyValuePair = new Dictionary<string, string>();

            /// <summary>
            /// Configuration key of <see cref="DCPSConfigFile"/>
            /// </summary>
            public const string DCPSConfigFileKey = "DCPSConfigFile";
            /// <summary>
            /// Configuration key of <see cref="DCPSTransportDebugLevel"/>
            /// </summary>
            public const string DCPSTransportDebugLevelKey = "DCPSTransportDebugLevel";
            /// <summary>
            /// Configuration key of <see cref="CommandLine"/>
            /// </summary>
            public const string CommandLineKey = "datadistributionmanager.opendds.cmdlineargs";
            /// <summary>
            /// Initialize a new instance of <see cref="OpenDDSArgsConfiguration"/>
            /// </summary>
            public OpenDDSArgsConfiguration()
            {
            }
            /// <summary>
            /// Initialize a new instance of <see cref="OpenDDSArgsConfiguration"/>
            /// </summary>
            /// <param name="dCPSConfigFile">The command line parameters of DCPSConfigFile</param>
            public OpenDDSArgsConfiguration(string dCPSConfigFile)
            {
                DCPSConfigFile = dCPSConfigFile;
            }

            /// <summary>
            /// The command line parameters of DCPSConfigFile
            /// </summary>
            public string DCPSConfigFile
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSConfigFileKey, out value);
                    return value;
                }
                set
                {
                    commandLineKeyValuePair[DCPSConfigFileKey] = value;
                }
            }

            /// <summary>
            /// The command line parameters of DCPSTransportDebugLevel
            /// </summary>
            public uint DCPSTransportDebugLevel
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSTransportDebugLevelKey, out value);
                    return uint.Parse(value);
                }
                set
                {
                    commandLineKeyValuePair[DCPSTransportDebugLevelKey] = value.ToString();
                }
            }

            /// <summary>
            /// The command line to be used on DCPSInfoRepo executable
            /// </summary>
            /// <remarks>Set to null to remove the property</remarks>
            public string CommandLine
            {
                get
                {
                    string value = string.Empty;
                    keyValuePair.TryGetValue(CommandLineKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) keyValuePair.Remove(CommandLineKey);
                    else keyValuePair[CommandLineKey] = value;
                }
            }

            string commandLineBuilder()
            {
                StringBuilder sb = new StringBuilder();
                foreach (var item in commandLineKeyValuePair)
                {
                    sb.AppendFormat("-{0} {1} ", item.Key, item.Value);
                }
                return string.Format("{0}={1}", CommandLineKey, sb.ToString());
            }

            /// <see cref="BaseConfiguration.CheckConfiguration"/>
            protected override void CheckConfiguration()
            {
                base.CheckConfiguration();
                if (!keyValuePair.ContainsKey(CommandLineKey))
                {
                    if (commandLineKeyValuePair.Count == 0)
                    {
                        throw new InvalidOperationException("Missing CommandLine");
                    }
                    else if (!commandLineKeyValuePair.ContainsKey(DCPSConfigFileKey))
                    {
                        throw new InvalidOperationException("Missing DCPSConfigFile");
                    }
                }
            }

            /// <see cref="IConfiguration.Configuration"/>
            public override string[] Configuration
            {
                get
                {
                    CheckConfiguration();
                    if (!keyValuePair.ContainsKey(CommandLineKey))
                    {
                        List<string> lst = new List<string>();
                        lst.AddRange(base.Configuration);
                        lst.Add(commandLineBuilder());
                        return lst.ToArray();
                    }
                    else return base.Configuration;
                }
            }
        }
        #endregion

        #region DCPSInfoRepoConfiguration
        /// <summary>
        /// Class managing DCPSInfoRepo. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public class DCPSInfoRepoConfiguration : BaseConfiguration
        {
            #region DataDistributionManager configuration properties
            /// <summary>
            /// Configuration key of <see cref="Autostart"/>
            /// </summary>
            public const string DCPSInfoRepoAutostartKey = "datadistributionmanager.opendds.dcpsinforepo.autostart";
            /// <summary>
            /// Configuration key of <see cref="LogOnApplication"/>
            /// </summary>
            public const string DCPSInfoRepoLogOnApplicationKey = "datadistributionmanager.opendds.dcpsinforepo.logonapplication";
            /// <summary>
            /// Configuration key of <see cref="CommandLine"/>
            /// </summary>
            public const string DCPSInfoRepoCommandLineKey = "datadistributionmanager.opendds.dcpsinforepo.cmdlineargs";
            #endregion

            #region Command line arguments
            Dictionary<string, string> commandLineKeyValuePair = new Dictionary<string, string>();
            /// <summary>
            /// Configuration key of <see cref="ORBSvcConf"/>
            /// </summary>
            public const string DCPSInfoRepoORBSvcConfKey = "ORBSvcConf";
            /// <summary>
            /// Configuration key of <see cref="ORBEndpoint"/>
            /// </summary>
            public const string DCPSInfoRepoORBEndpointKey = "ORBEndpoint";
            /// <summary>
            /// Configuration key of <see cref="ORBListenEndpoints"/>
            /// </summary>
            public const string DCPSInfoRepoORBListenEndpointsKey = "ORBListenEndpoints";
            /// <summary>
            /// Configuration key of <see cref="IorFile"/>
            /// </summary>
            public const string DCPSInfoRepoIorFileKey = "o";
            /// <summary>
            /// Configuration key of <see cref="NOBITS"/>
            /// </summary>
            public const string DCPSInfoRepoNOBITSKey = "NOBITS";
            /// <summary>
            /// Configuration key of <see cref="ListeningAddress"/>
            /// </summary>
            public const string DCPSInfoRepoListeningAddressKey = "a";
            /// <summary>
            /// Configuration key of <see cref="VerboseLogging"/>
            /// </summary>
            public const string DCPSInfoRepoVerboseLoggingKey = "z";
            /// <summary>
            /// Configuration key of <see cref="PersistenceFile"/>
            /// </summary>
            public const string DCPSInfoRepoPersistenceFileKey = "file";
            /// <summary>
            /// Configuration key of <see cref="Resurrect"/>
            /// </summary>
            public const string DCPSInfoRepoResurrectKey = "r";
            /// <summary>
            /// Configuration key of <see cref="PersistenceReset"/>
            /// </summary>
            public const string DCPSInfoRepoPersistenceResetKey = "reset";
            /// <summary>
            /// Configuration key of <see cref="FederatorConfig"/>
            /// </summary>
            public const string DCPSInfoRepoFederatorConfigKey = "FederatorConfig";
            /// <summary>
            /// Configuration key of <see cref="FederationId"/>
            /// </summary>
            public const string DCPSInfoRepoFederationIdKey = "FederationId";
            /// <summary>
            /// Configuration key of <see cref="FederateWith"/>
            /// </summary>
            public const string DCPSInfoRepoFederateWithKey = "FederateWith";
            /// <summary>
            /// Configuration key of <see cref="ReassociateDelay"/>
            /// </summary>
            public const string DCPSInfoRepoReassociateDelayKey = "ReassociateDelay";
            /// <summary>
            /// Configuration key of <see cref="DispatchingCheckDelay"/>
            /// </summary>
            public const string DCPSInfoRepoDispatchingCheckDelayKey = "DispatchingCheckDelay";
            #endregion

            /// <summary>
            /// Initialize a new instance of <see cref="DCPSInfoRepoConfiguration"/>
            /// </summary>
            public DCPSInfoRepoConfiguration()
            {
            }
            /// <summary>
            /// Initialize a new instance of <see cref="DCPSInfoRepoConfiguration"/>
            /// </summary>
            /// <param name="autostart">Automatically starts DCPSInfoRepo</param>
            /// <param name="commandLine">Command line to use with DCPSInfoRepo</param>
            public DCPSInfoRepoConfiguration(bool autostart, string commandLine)
            {
                Autostart = autostart;
                CommandLine = commandLine;
            }

            /// <summary>
            /// Automatically start DCPSInfoRepo
            /// </summary>
            public bool Autostart
            {
                get
                {
                    string value = string.Empty;
                    keyValuePair.TryGetValue(DCPSInfoRepoAutostartKey, out value);
                    return bool.Parse(value);
                }
                set
                {
                    keyValuePair[DCPSInfoRepoAutostartKey] = value.ToString().ToLowerInvariant();
                }
            }

            /// <summary>
            /// Emits log in the application log
            /// </summary>
            public bool LogOnApplication
            {
                get
                {
                    string value = string.Empty;
                    keyValuePair.TryGetValue(DCPSInfoRepoLogOnApplicationKey, out value);
                    return bool.Parse(value);
                }
                set
                {
                    keyValuePair[DCPSInfoRepoLogOnApplicationKey] = value.ToString().ToLowerInvariant();
                }
            }
            /// <summary>
            /// See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            public string ORBSvcConf
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoORBSvcConfKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoORBSvcConfKey);
                    else commandLineKeyValuePair[DCPSInfoRepoORBSvcConfKey] = value;
                }
            }

            /// <summary>
            /// See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            public string ORBEndpoint
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoORBEndpointKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoORBEndpointKey);
                    else commandLineKeyValuePair[DCPSInfoRepoORBEndpointKey] = value;
                }
            }

            /// <summary>
            /// See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            public string ORBListenEndpoints
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoORBListenEndpointsKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoORBListenEndpointsKey);
                    else commandLineKeyValuePair[DCPSInfoRepoORBListenEndpointsKey] = value;
                }
            }

            /// <summary>
            /// Write the IOR of the DCPSInfo object to the specified file
            /// </summary>
            public string IorFile
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoIorFileKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoIorFileKey);
                    else commandLineKeyValuePair[DCPSInfoRepoIorFileKey] = value;
                }
            }

            /// <summary>
            /// Disable the publication of built-in topics
            /// </summary>
            public bool NOBITS
            {
                get
                {
                    string value = string.Empty;
                    if (commandLineKeyValuePair.TryGetValue(DCPSInfoRepoNOBITSKey, out value))
                    {
                        return true;
                    }
                    return false;
                }
                set
                {
                    if (value == true)
                    {
                        commandLineKeyValuePair[DCPSInfoRepoNOBITSKey] = string.Empty;
                    }
                    else commandLineKeyValuePair.Remove(DCPSInfoRepoNOBITSKey);
                }
            }

            /// <summary>
            /// Listening address for built-in topics (when built-in topics are published).
            /// </summary>
            public string ListeningAddress
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoListeningAddressKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoListeningAddressKey);
                    else commandLineKeyValuePair[DCPSInfoRepoListeningAddressKey] = value;
                }
            }

            /// <summary>
            /// Turn on verbose transport logging
            /// </summary>
            public bool VerboseLogging
            {
                get
                {
                    string value = string.Empty;
                    if (commandLineKeyValuePair.TryGetValue(DCPSInfoRepoVerboseLoggingKey, out value))
                    {
                        return true;
                    }
                    return false;
                }
                set
                {
                    if (value == true)
                    {
                        commandLineKeyValuePair[DCPSInfoRepoVerboseLoggingKey] = string.Empty;
                    }
                    else commandLineKeyValuePair.Remove(DCPSInfoRepoVerboseLoggingKey);
                }
            }

            /// <summary>
            /// Name of the persistent file
            /// </summary>
            public string PersistenceFile
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoPersistenceFileKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoPersistenceFileKey);
                    else commandLineKeyValuePair[DCPSInfoRepoPersistenceFileKey] = value;
                }
            }

            /// <summary>
            /// Resurrect from persistent file
            /// </summary>
            public bool Resurrect
            {
                get
                {
                    string value = string.Empty;
                    if (commandLineKeyValuePair.TryGetValue(DCPSInfoRepoResurrectKey, out value))
                    {
                        if (value == "1") return true;
                    }
                    return false;
                }
                set
                {
                    if (value == true)
                    {
                        commandLineKeyValuePair[DCPSInfoRepoResurrectKey] = "1";
                    }
                    else commandLineKeyValuePair[DCPSInfoRepoResurrectKey] = "0"; ;
                }
            }

            /// <summary>
            /// Wipe out old persistent data.
            /// </summary>
            public bool PersistenceReset
            {
                get
                {
                    string value = string.Empty;
                    if (commandLineKeyValuePair.TryGetValue(DCPSInfoRepoPersistenceResetKey, out value))
                    {
                        if (value == "1") return true;
                    }
                    return false;
                }
                set
                {
                    if (value == true)
                    {
                        commandLineKeyValuePair[DCPSInfoRepoPersistenceResetKey] = "1";
                    }
                    else commandLineKeyValuePair[DCPSInfoRepoPersistenceResetKey] = "0"; ;
                }
            }

            /// <summary>
            /// Configure federation from file
            /// </summary>
            public string FederatorConfig
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoFederatorConfigKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoFederatorConfigKey);
                    else commandLineKeyValuePair[DCPSInfoRepoFederatorConfigKey] = value;
                }
            }

            /// <summary>
            /// Unique identifier for this repository within any federation. This is supplied as a 32 bit decimal numeric value.
            /// </summary>
            public Int32 FederationId
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoFederationIdKey, out value);
                    return Int32.Parse(value);
                }
                set
                {
                    commandLineKeyValuePair[DCPSInfoRepoFederationIdKey] = value.ToString();
                }
            }

            /// <summary>
            /// Repository federation reference at which to join a federation. This is supplied as a valid CORBA object reference in string form: stringified IOR, file: or corbaloc: reference string.
            /// </summary>
            public string FederateWith
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoFederateWithKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) commandLineKeyValuePair.Remove(DCPSInfoRepoFederateWithKey);
                    else commandLineKeyValuePair[DCPSInfoRepoFederateWithKey] = value;
                }
            }

            /// <summary>
            /// delay between reassociations
            /// </summary>
            public Int32 ReassociateDelay
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoReassociateDelayKey, out value);
                    return Int32.Parse(value);
                }
                set
                {
                    commandLineKeyValuePair[DCPSInfoRepoReassociateDelayKey] = value.ToString();
                }
            }

            /// <summary>
            /// Unique identifier for this repository within any federation. This is supplied as a 32 bit decimal numeric value.
            /// </summary>
            public Int32 DispatchingCheckDelay
            {
                get
                {
                    string value = string.Empty;
                    commandLineKeyValuePair.TryGetValue(DCPSInfoRepoDispatchingCheckDelayKey, out value);
                    return Int32.Parse(value);
                }
                set
                {
                    commandLineKeyValuePair[DCPSInfoRepoDispatchingCheckDelayKey] = value.ToString();
                }
            }


            /// <summary>
            /// The command line to be used on DCPSInfoRepo executable
            /// </summary>
            public string CommandLine
            {
                get
                {
                    string value = string.Empty;
                    keyValuePair.TryGetValue(DCPSInfoRepoCommandLineKey, out value);
                    return value;
                }
                set
                {
                    if (value == null) keyValuePair.Remove(DCPSInfoRepoCommandLineKey);
                    else keyValuePair[DCPSInfoRepoCommandLineKey] = value;
                }
            }

            string commandLineBuilder()
            {
                StringBuilder sb = new StringBuilder();
                foreach (var item in commandLineKeyValuePair)
                {
                    sb.AppendFormat("-{0} {1} ", item.Key, item.Value);
                }
                return string.Format("{0}={1}", DCPSInfoRepoCommandLineKey, sb.ToString());
            }

            /// <see cref="BaseConfiguration.CheckConfiguration"/>
            protected override void CheckConfiguration()
            {
                base.CheckConfiguration();
                if (!keyValuePair.ContainsKey(DCPSInfoRepoCommandLineKey) && commandLineKeyValuePair.Count == 0)
                {
                    throw new InvalidOperationException("Missing CommandLine");
                }
            }

            /// <see cref="IConfiguration.Configuration"/>
            public override string[] Configuration
            {
                get
                {
                    CheckConfiguration();
                    if (!keyValuePair.ContainsKey(DCPSInfoRepoCommandLineKey))
                    {
                        List<string> lst = new List<string>();
                        lst.AddRange(base.Configuration);
                        lst.Add(commandLineBuilder());
                        return lst.ToArray();
                    }
                    else return base.Configuration;
                }
            }
        }
        #endregion

        /// <summary>
        /// Base property name of all specific configuration key of <see cref="OpenDDSConfiguration"/>
        /// </summary>
        public const string OpenDDSConfigurationBasePropertyKey = "datadistributionmanager.opendds.";
        /// <summary>
        /// Configuration key of <see cref="DomainId"/>
        /// </summary>
        public const string DomainIdKey = "datadistributionmanager.opendds.domain_id";
        /// <summary>
        /// Configuration key of <see cref="DCPSDebugLevel"/>
        /// </summary>
        public const string DCPSDebugLevelKey = "datadistributionmanager.opendds.dcps.debug_level";
        /// <summary>
        /// Initialize a <see cref="OpenDDSConfiguration"/>
        /// </summary>
        public OpenDDSConfiguration()
#if DEBUG
            : base("opendds", "DataDistributionManagerOpenDDSd.dll")
#else
            : base("opendds", "DataDistributionManagerOpenDDS.dll")
#endif
        {
        }
        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        public OpenDDSConfiguration(IConfiguration originalConf)
            : base(originalConf)
        {
            OpenDDSConfiguration conf = originalConf as OpenDDSConfiguration;
            if (conf != null)
            {
                OpenDDSArgs = conf.OpenDDSArgs;
                DCPSInfoRepo = conf.DCPSInfoRepo;
                DomainParticipantQos = conf.DomainParticipantQos;
            }
        }

        /// <summary>
        /// The domain id parameter to initialize OpenDDS
        /// </summary>
        public uint DomainId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DomainIdKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[DomainIdKey] = value.ToString();
            }
        }

        /// <summary>
        /// Logging verbosity level.
        /// </summary>
        /// <value>
        /// Value guidelines:
        /// 0 - logs that indicate serious errors that are not indicated by return codes (almost none)
        /// 1 - logs that should happen once per process or are warnings
        /// 2 - logs that should happen once per DDS entity
        /// 4 - logs that are related to administrative interfaces
        /// 6 - logs that should happen every Nth sample write/read
        /// 8 - logs that should happen once per sample write/read
        /// 10 - logs that may happen more than once per sample write/read
        /// </value>
        public uint DCPSDebugLevel
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DCPSDebugLevelKey, out value);
                return uint.Parse(value);
            }
            set
            {
                switch (value)
                {
                    case 0: // - logs that indicate serious errors that are not indicated by return codes (almost none)
                    case 1: // - logs that should happen once per process or are warnings
                    case 2: // -logs that should happen once per DDS entity
                    case 4: // -logs that are related to administrative interfaces
                    case 6: // -logs that should happen every Nth sample write/read
                    case 8: // -logs that should happen once per sample write/read
                    case 10: // -logs that may happen more than once per sample write/read
                        break;
                    default: throw new ArgumentException("Value is not in correct range");
                }
                keyValuePair[DCPSDebugLevelKey] = value.ToString();
            }
        }

        /// <summary>
        /// The configuration of <see cref="OpenDDSArgsConfiguration"/>
        /// </summary>
        public OpenDDSArgsConfiguration OpenDDSArgs { get; set; }

        /// <summary>
        /// The configuration of <see cref="DCPSInfoRepoConfiguration"/>
        /// </summary>
        public DCPSInfoRepoConfiguration DCPSInfoRepo { get; set; }

        /// <summary>
        /// The configuration of <see cref="DomainParticipantQosConfiguration"/>
        /// </summary>
        public DomainParticipantQosConfiguration DomainParticipantQos { get; set; }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (OpenDDSArgs != null) lst.AddRange(OpenDDSArgs.Configuration);
            if (DCPSInfoRepo != null) lst.AddRange(DCPSInfoRepo.Configuration);
            if (DomainParticipantQos != null) lst.AddRange(DomainParticipantQos.Configuration);
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
}
