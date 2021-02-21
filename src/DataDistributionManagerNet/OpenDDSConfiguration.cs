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

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The configuration class for OpenDDS
    /// </summary>
    public class OpenDDSConfiguration : CommonConfiguration
    {
        #region OpenDDSArgsConfiguration
        /// <summary>
        /// Class managing OpenDDSArgs
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
                    keyValuePair[CommandLineKey] = value;
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
                if (!keyValuePair.ContainsKey(CommandLineKey) && commandLineKeyValuePair.Count == 0)
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
        /// Class managing DCPSInfoRepo
        /// </summary>
        public class DCPSInfoRepoConfiguration : BaseConfiguration
        {
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
                    keyValuePair[DCPSInfoRepoCommandLineKey] = value;
                }
            }
        }
        #endregion
/*
        #region DomainParticipantQosConfiguration
        /// <summary>
        /// Class managing DomainParticipantQos
        /// </summary>
        public class DomainParticipantQosConfiguration : BaseConfiguration
        {
            /// <summary>
            /// Configuration key of <see cref="EntityFactoryQosPolicy"/>
            /// </summary>
            public const string EntityFactoryQosPolicyPropertyKey = "datadistributionmanager.opendds.qos.domainparticipant.entityfactoryqospolicy";
            /// <summary>
            /// Initialize a new instance of <see cref="DomainParticipantQosConfiguration"/>
            /// </summary>
            public DomainParticipantQosConfiguration()
            {
                EntityFactoryQosPolicy = true;
            }
            /// <summary>
            /// Initialize a new instance of <see cref="DomainParticipantQosConfiguration"/>
            /// </summary>
            /// <param name="entityFactoryQosPolicy">Value to use for EntityFactoryQosPolicy</param>
            public DomainParticipantQosConfiguration(bool entityFactoryQosPolicy)
            {
                EntityFactoryQosPolicy = entityFactoryQosPolicy;
            }

            /// <summary>
            /// Automatically start DCPSInfoRepo
            /// </summary>
            public bool EntityFactoryQosPolicy
            {
                get
                {
                    string value = string.Empty;
                    keyValuePair.TryGetValue(EntityFactoryQosPolicyPropertyKey, out value);
                    return bool.Parse(value);
                }
                set
                {
                    keyValuePair[EntityFactoryQosPolicyPropertyKey] = value.ToString().ToLowerInvariant();
                }
            }
        }
        #endregion
*/
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="OpenDDSConfiguration"/>
        /// </summary>
        public const string OpenDDSConfigurationBasePropertyKey = "datadistributionmanager.opendds.";
        /// <summary>
        /// Configuration key of <see cref="DomainId"/>
        /// </summary>
        public const string DomainIdKey = "datadistributionmanager.opendds.domain_id";

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

        /// <see cref="IConfiguration.Configuration"/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
}
