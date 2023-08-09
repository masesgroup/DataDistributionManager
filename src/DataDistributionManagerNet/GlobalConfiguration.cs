/*
*  Copyright 2023 MASES s.r.l.
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

using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Collections.Generic;

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The global configuration class
    /// </summary>
    public abstract class GlobalConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Protocol"/>
        /// </summary>
        public const string ProtocolKey = "datadistributionmanager.common.protocol";
        /// <summary>
        /// Configuration key of <see cref="ProtocolLibrary"/>
        /// </summary>
        public const string ProtocolLibraryKey = "datadistributionmanager.common.protolib";
        /// <summary>
        /// Configuration key of <see cref="MastershipLibrary"/>
        /// </summary>
        public const string MastershipLibraryKey = "datadistributionmanager.common.mastershiplib";
        /// <summary>
        /// Configuration key of <see cref="MaxMessageSize"/>
        /// </summary>
        public const string MaxMessageSizeKey = "datadistributionmanager.maxmessagesize";
        /// <summary>
        /// Configuration key of <see cref="ServerLostTimeout"/>
        /// </summary>
        public const string ServerLostTimeoutKey = "datadistributionmanager.timeout.serverlost";
        /// <summary>
        /// Configuration key of <see cref="GlobalLogLevel"/>
        /// </summary>
        public const string GlobalLogLevelKey = "datadistributionmanager.loglevel.global";

        internal GlobalConfiguration(string protocol, string protolib)
        {
            Protocol = protocol;
            ProtocolLibrary = protolib;
        }
        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        internal GlobalConfiguration(IConfiguration originalConf)
        {
            GlobalConfiguration conf = originalConf as GlobalConfiguration;
            if (conf != null)
            {
                keyValuePair = new Dictionary<string, string>(conf.keyValuePair);
            }
        }

        /// <summary>
        /// The protocol to use (e.g. kafka, opendds)
        /// </summary>
        public string Protocol
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ProtocolKey, out value);
                return value;
            }
            set
            {
                keyValuePair[ProtocolKey] = value;
                EmitPropertyChanged("Protocol");
            }
        }

        /// <summary>
        /// The protocol library to use
        /// </summary>
        public string ProtocolLibrary
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ProtocolLibraryKey, out value);
                return value;
            }
            set
            {
                keyValuePair[ProtocolLibraryKey] = value;
                EmitPropertyChanged("ProtocolLibrary");
            }
        }

        /// <summary>
        /// The mastership library to use
        /// </summary>
        public string MastershipLibrary
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MastershipLibraryKey, out value);
                return value;
            }
            set
            {
                keyValuePair[MastershipLibraryKey] = value;
                EmitPropertyChanged("MastershipLibrary");
            }
        }

        /// <summary>
        /// The max message size managed
        /// </summary>
        public uint MaxMessageSize
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxMessageSizeKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[MaxMessageSizeKey] = value.ToString();
                EmitPropertyChanged("MaxMessageSize");
            }
        }

        /// <summary>
        /// The timeout on server lost in ms
        /// </summary>
        public uint ServerLostTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ServerLostTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ServerLostTimeoutKey] = value.ToString();
                EmitPropertyChanged("ServerLostTimeout");
            }
        }

        /// <summary>
        /// The global log value
        /// </summary>
        public DDM_LOG_LEVEL GlobalLogLevel
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(GlobalLogLevelKey, out value);
                return value.FromIntString<DDM_LOG_LEVEL>();
            }
            set
            {
                keyValuePair[GlobalLogLevelKey] = value.ToIntString();
                EmitPropertyChanged("GlobalLogLevel");
            }
        }

        /// <inheritdoc/>
        protected override void CheckConfiguration()
        {
            if (!keyValuePair.ContainsKey(ProtocolKey) && !keyValuePair.ContainsKey(ProtocolLibraryKey))
            {
                throw new InvalidOperationException("Missing Protocol or ProtocolLibrary");
            }
        }
    }
}
