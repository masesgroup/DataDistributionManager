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

using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Collections.Generic;

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The global configuration class
    /// </summary>
    public abstract class GlobalConfiguration : IConfiguration
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

        /// <summary>
        /// The list of key/value pairs
        /// </summary>
        protected Dictionary<string, string> keyValuePair = new Dictionary<string, string>();

        internal GlobalConfiguration(string protocol, string protolib)
        {
            Protocol = protocol;
            ProtocolLibrary = protolib;
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
            }
        }

        /// <summary>
        /// The timeout on server lost
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
                return (DDM_LOG_LEVEL)Enum.Parse(typeof(DDM_LOG_LEVEL), value);
            }
            set
            {
                keyValuePair[GlobalLogLevelKey] = ((int)value).ToString();
            }
        }

        /// <summary>
        /// Checks the configuration for mandatory information
        /// </summary>
        protected virtual void CheckConfiguration()
        {
            if (!keyValuePair.ContainsKey(ProtocolKey) && !keyValuePair.ContainsKey(ProtocolLibraryKey))
            {
                throw new InvalidOperationException("Missing Protocol or ProtocolLibrary");
            }
        }

        /// <see cref="IConfiguration.Configuration"/>
        public virtual string[] Configuration
        {
            get
            {
                List<string> lst = new List<string>();
                foreach (var item in keyValuePair)
                {
                    lst.Add(string.Format("{0}={1}", item.Key, item.Value));
                }
                return lst.ToArray();
            }
        }
    }
}
