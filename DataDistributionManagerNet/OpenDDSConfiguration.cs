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

        const string CommandLineKey = "datadistributionmanager.opendds.cmdlineargs";
        const string DCPSConfigFileKey = "DCPSConfigFile";
        const string DCPSTransportDebugLevelKey = "DCPSTransportDebugLevel";

        /// <summary>
        /// Initialize a <see cref="OpenDDSConfiguration"/>
        /// </summary>
        public OpenDDSConfiguration()
            : base("opendds", "DataDistributionManagerOpenDDS.dll")
        {
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
                keyValuePair[CommandLineKey] = value;
            }
        }


        /// <see cref="CommonConfiguration.CheckConfiguration"/>
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
