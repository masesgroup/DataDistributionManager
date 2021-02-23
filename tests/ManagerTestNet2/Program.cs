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

using MASES.DataDistributionManager.Bindings;
using MASES.DataDistributionManager.Bindings.Configuration;
using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Diagnostics;
using System.Threading;

namespace ManagerTestNet
{
    class Program
    {
        const int THRESHOLD = 10;

        static void Main(string[] args)
        {
            SmartDataDistribution dataDistribution = new SmartDataDistribution();
            dataDistribution.LoggingEvent += DataDistribution_LoggingEvent;
            OpenDDSConfiguration conf = null;
            HRESULT hRes = HRESULT.S_OK;

            if (args.Length == 0)
            {
                conf = new OpenDDSConfiguration()
                {
                    OpenDDSArgs = new OpenDDSConfiguration.OpenDDSArgsConfiguration()
                    {
                        DCPSConfigFile = "dds_tcp_conf.ini",
                        DCPSTransportDebugLevel = 10,
                    },
                    DCPSInfoRepo = new OpenDDSConfiguration.DCPSInfoRepoConfiguration()
                    {
                        Autostart = true,
                        ORBEndpoint = "iiop://localhost:12345",
                    },
                    DomainParticipantQos = new DomainParticipantQosConfiguration()
                    {
                        EntityFactoryQosPolicy = new EntityFactoryQosPolicyConfiguration()
                        {
                            AutoenableCreatedEntities = true
                        },
                        PropertyQosPolicy = new PropertyQosPolicyConfiguration()
                        {
                            DDSSEC_PROP_IDENTITY_CA = new PropertyQosPolicyConfiguration.Property("1"),
                            DDSSEC_PROP_IDENTITY_CERT = new PropertyQosPolicyConfiguration.Property("2"),
                            DDSSEC_PROP_IDENTITY_PRIVKEY = new PropertyQosPolicyConfiguration.Property("3"),
                            DDSSEC_PROP_PERM_CA = new PropertyQosPolicyConfiguration.Property("4"),
                            DDSSEC_PROP_PERM_DOC = new PropertyQosPolicyConfiguration.Property("5"),
                            DDSSEC_PROP_PERM_GOV_DOC = new PropertyQosPolicyConfiguration.Property("6", true),
                        }
                    }
                };
                hRes = dataDistribution.Initialize(conf);
            }
            else
            {
                hRes = dataDistribution.Initialize(args[0]);
            }

            if (hRes.Failed)
            {
                Console.WriteLine("Error in configuration.");
                Console.ReadKey();
                return;
            }

            if (!dataDistribution.Start(uint.MaxValue))
            {
                Console.ReadKey();
                return;
            }

            OpenDDSChannelConfiguration channelConf = new OpenDDSChannelConfiguration(conf)
            {
                TopicQos = new TopicQosConfiguration()
                {
                    TopicDataQosPolicy = new TopicDataQosPolicyConfiguration()
                    {
                        Value = new byte[] { 100, 23 }
                    },
                    DurabilityQosPolicy = new DurabilityQosPolicyConfiguration()
                    {
                        Kind = DurabilityQosPolicyConfiguration.DurabilityQosPolicyKind.TRANSIENT_DURABILITY_QOS
                    }
                },
                SubscriberQos = new SubscriberQosConfiguration()
                {
                    EntityFactoryQosPolicy = new EntityFactoryQosPolicyConfiguration()
                    {
                        AutoenableCreatedEntities = true
                    }
                }
            };

            SmartDataDistributionChannel testChannel = dataDistribution.CreateSmartChannel<SmartDataDistributionChannel>("test", channelConf);
            testChannel.DataAvailable += TestChannel_DataAvailable;
            testChannel.ConditionOrError += TestChannel_ConditionOrError;

            Console.WriteLine("After CreateSmartChannel...\n");

            testChannel.StartChannel(uint.MaxValue);

            uint counter = 100;
            int pid = Process.GetCurrentProcess().Id;
            var str = string.Format("{0:10}", pid);
            DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.TRANSMITTER;
            Console.WriteLine("Starting sending...\n");
            while (true)
            {
                if (direction.HasFlag(DDM_CHANNEL_DIRECTION.TRANSMITTER) ? testChannel.WriteOnChannel(null, str) : true)
                {
                    str = string.Format("{0:10}", counter++);
                    if ((counter % THRESHOLD) == 0)
                    {
                        Console.WriteLine("SendData Reached {0}", counter);
                    }
                }
                Thread.Sleep(1000);
            }
        }

        private static void TestChannel_ConditionOrError(object sender, ConditionOrErrorEventArgs e)
        {
            Console.WriteLine("Received event from {0} with ErrorCode {1} NativeCode {2} SubSystemReason {3}", e.ChannelName, e.ErrorCode, e.NativeCode, e.SubSystemReason);
        }

        private static void TestChannel_DataAvailable(object sender, DataAvailableEventArgs e)
        {
            Console.WriteLine("Received data from {0} with key {1} and buffer {2}", e.ChannelName, e.Key, e.DecodedString);
        }

        private static void DataDistribution_LoggingEvent(object sender, LoggingEventArgs e)
        {
            Console.WriteLine("Timestamp: {0} Source: {1} Function: {2} - {3}", DateTime.Now, e.Source, e.Function, e.LogString);
        }
    }
}