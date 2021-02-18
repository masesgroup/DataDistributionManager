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
        Dictionary<string, string> commandLineKeyValuePair = new Dictionary<string, string>();
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="KafkaConfiguration"/>
        /// </summary>
        public const string OpenDDSConfigurationBasePropertyKey = "datadistributionmanager.opendds.";
        /// <summary>
        /// Configuration key of <see cref="DCPSInfoRepoAutostart"/>
        /// </summary>
        public const string DCPSInfoRepoAutostartKey = "datadistributionmanager.opendds.dcpsinforepo.autostart";
        /// <summary>
        /// Configuration key of <see cref="DCPSInfoRepoLogOnApplication"/>
        /// </summary>
        public const string DCPSInfoRepoLogOnApplicationKey = "datadistributionmanager.opendds.dcpsinforepo.logonapplication";
        /// <summary>
        /// Configuration key of <see cref="DCPSInfoRepoCommandLine"/>
        /// </summary>
        public const string DCPSInfoRepoCommandLineKey = "datadistributionmanager.opendds.dcpsinforepo.cmdlineargs";
        /// <summary>
        /// Configuration key of <see cref="CommandLine"/>
        /// </summary>
        public const string CommandLineKey = "datadistributionmanager.opendds.cmdlineargs";
        /// <summary>
        /// Configuration key of <see cref="DomainId"/>
        /// </summary>
        public const string DomainIdKey = "datadistributionmanager.opendds.domain_id";
        /// <summary>
        /// Configuration key of <see cref="DCPSConfigFile"/>
        /// </summary>
        public const string DCPSConfigFileKey = "DCPSConfigFile";
        /// <summary>
        /// Configuration key of <see cref="DCPSTransportDebugLevel"/>
        /// </summary>
        public const string DCPSTransportDebugLevelKey = "DCPSTransportDebugLevel";

        /// <summary>
        /// Initialize a <see cref="OpenDDSConfiguration"/>
        /// </summary>
        public OpenDDSConfiguration()
            : base("opendds", "DataDistributionManagerOpenDDS.dll")
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
            if (conf !=null)
            {
                commandLineKeyValuePair = new Dictionary<string, string>(conf.commandLineKeyValuePair);
            }
        }
        /// <summary>
        /// Automatically start DCPSInfoRepo
        /// </summary>
        public bool DCPSInfoRepoAutostart
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
        public bool DCPSInfoRepoLogOnApplication
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
        public string DCPSInfoRepoCommandLine
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
        /// The command line parameters to initialize OpenDDS (e.g. -DCPSConfigFile ../../Configuration/dds_tcp_conf.ini -DCPSTransportDebugLevel 10)
        /// </summary>
        /// <remarks>Set a value to null remove the property</remarks>
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

        /// <see cref="GlobalConfiguration.CheckConfiguration"/>
        protected override void CheckConfiguration()
        {
            base.CheckConfiguration();
            if (!keyValuePair.ContainsKey(CommandLineKey) && commandLineKeyValuePair.Count == 0)
            {
                throw new InvalidOperationException("Missing CommandLine");
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
}
