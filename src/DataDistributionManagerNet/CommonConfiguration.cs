﻿/*
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
    /// The common configuration class
    /// </summary>
    public abstract class CommonConfiguration : GlobalConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="CreateChannelTimeout"/>
        /// </summary>
        public const string CreateChannelTimeoutKey = "datadistributionmanager.timeout.createchannel";
        /// <summary>
        /// Configuration key of <see cref="ChannelSeekTimeout"/>
        /// </summary>
        public const string ChannelSeekTimeoutKey = "datadistributionmanager.timeout.channelseek";
        /// <summary>
        /// Configuration key of <see cref="ReceiveTimeout"/>
        /// </summary>
        public const string ReceiveTimeoutKey = "datadistributionmanager.timeout.receive";
        /// <summary>
        /// Configuration key of <see cref="KeepAliveTimeout"/>
        /// </summary>
        public const string KeepAliveTimeoutKey = "datadistributionmanager.timeout.keepalive";
        /// <summary>
        /// Configuration key of <see cref="ConsumerTimeout"/>
        /// </summary>
        public const string ConsumerTimeoutKey = "datadistributionmanager.timeout.consumer";
        /// <summary>
        /// Configuration key of <see cref="ProducerTimeout"/>
        /// </summary>
        public const string ProducerTimeoutKey = "datadistributionmanager.timeout.producer";
        /// <summary>
        /// Configuration key of <see cref="CommitTimeout"/>
        /// </summary>
        public const string CommitTimeoutKey = "datadistributionmanager.timeout.commit";
        /// <summary>
        /// Configuration key of <see cref="CommitSync"/>
        /// </summary>
        public const string CommitSyncKey = "datadistributionmanager.commit.sync";

        /// <summary>
        /// Initialize a new <see cref="CommonConfiguration"/>
        /// </summary>
        /// <param name="protocol">The protocol to use</param>
        /// <param name="protolib">The protocol library to use</param>
        public CommonConfiguration(string protocol, string protolib):
            base(protocol, protolib)
        {
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
        /// The timeout on channel message commit
        /// </summary>
        public uint CommitTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CommitTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[CommitTimeoutKey] = value.ToString();
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
    }
}
