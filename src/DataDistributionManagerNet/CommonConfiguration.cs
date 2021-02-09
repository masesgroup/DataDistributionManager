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

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The general configuration class
    /// </summary>
    public abstract class CommonConfiguration : IConfiguration
    {
        const string ProtocolKey = "datadistributionmanager.common.protocol";
        const string ProtocolLibraryKey = "datadistributionmanager.common.protolib";
        const string MaxMessageSizeKey = "datadistributionmanager.maxmessagesize";
        const string CreateChannelTimeoutKey = "datadistributionmanager.timeout.createchannel";
        const string ServerLostTimeoutKey = "datadistributionmanager.timeout.serverlost";
        const string ChannelSeekTimeoutKey = "datadistributionmanager.timeout.channelseek";
        const string ReceiveTimeoutKey = "datadistributionmanager.timeout.receive";
        const string KeepAliveTimeoutKey = "datadistributionmanager.timeout.keepalive";
        const string ConsumerTimeoutKey = "datadistributionmanager.timeout.consumer";
        const string ProducerTimeoutKey = "datadistributionmanager.timeout.producer";
        const string CommitSyncKey = "datadistributionmanager.commit.sync";

        /// <summary>
        /// The list of key/value pairs
        /// </summary>
        protected Dictionary<string, string> keyValuePair = new Dictionary<string, string>();

        internal CommonConfiguration(string protocol, string protolib)
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
        /// The timeout on create channel
        /// </summary>
        public uint CreateChannelTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CreateChannelTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[CreateChannelTimeoutKey] = value.ToString();
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
        /// The timeout on seek channel
        /// </summary>
        public uint ChannelSeekTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ChannelSeekTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ChannelSeekTimeoutKey] = value.ToString();
            }
        }

        /// <summary>
        /// The timeout on channel receive
        /// </summary>
        public uint ReceiveTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReceiveTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ReceiveTimeoutKey] = value.ToString();
            }
        }

        /// <summary>
        /// The timeout on channel keep alive
        /// </summary>
        public uint KeepAliveTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KeepAliveTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[KeepAliveTimeoutKey] = value.ToString();
            }
        }

        /// <summary>
        /// The timeout on channel message consume
        /// </summary>
        public uint ConsumerTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ConsumerTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ConsumerTimeoutKey] = value.ToString();
            }
        }

        /// <summary>
        /// The timeout on channel message produce
        /// </summary>
        public uint ProducerTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ProducerTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ProducerTimeoutKey] = value.ToString();
            }
        }

        /// <summary>
        /// True to commit message in sync
        /// </summary>
        public bool CommitSync
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CommitSyncKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[CommitSyncKey] = value.ToString().ToLowerInvariant();
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
