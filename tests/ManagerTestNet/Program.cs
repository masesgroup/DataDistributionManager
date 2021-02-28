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

using MASES.DataDistributionManager.Bindings;
using MASES.DataDistributionManager.Bindings.Configuration;
using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Diagnostics;
using System.Text;
using System.Threading;

namespace ManagerTestNet
{
    class MySmartDataDistribution : SmartDataDistribution
    {
        public override string OnConfiguration(string channelName, string key, string value)
        {
            return base.OnConfiguration(channelName, key, value);
        }

        public override void OnLogging(DDM_LOG_LEVEL level, string source, string function, string errStr)
        {
            base.OnLogging(level, source, function, errStr);

            Console.WriteLine("Timestamp: {0} Source: {1} Function: {2} - {3}", DateTime.Now, source, function, errStr);
        }

    };

    class MySmartDataDistributionChannel : SmartDataDistributionChannel
    {
        public override void OnDataAvailable(string channelName, string key, byte[] buffer)
        {
            var str = Encoding.UTF8.GetString(buffer);
            Console.WriteLine("Received data from {0} with key {1} and buffer {2}", channelName, key, str);
        }

        public override void OnConditionOrError(string channelName, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode, string subSystemReason)
        {
            base.OnConditionOrError(channelName, errorCode, nativeCode, subSystemReason);
        }
    };


    class Program
    {
        const int THRESHOLD = 10;

        static void Main(string[] args)
        {
            DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.TRANSMITTER;

            MySmartDataDistribution dataDistribution = new MySmartDataDistribution();
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
                        Autostart = direction.HasFlag(DDM_CHANNEL_DIRECTION.RECEIVER),
                        Monitor = true,
                        Resurrect = true,
                        PersistenceFile = "persistance.file",
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

            MySmartDataDistributionChannel testChannel = dataDistribution.CreateSmartChannel<MySmartDataDistributionChannel>("test", channelConf);

            Console.WriteLine("After CreateSmartChannel...\n");

            testChannel.StartChannel(uint.MaxValue);

            uint counter = 100;
            int pid = Process.GetCurrentProcess().Id;
            var str = string.Format("{0:10}", pid);

            Console.WriteLine("Starting operations...\n");
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

        private static void DataDistribution_Logging(object sender, LoggingEventArgs e)
        {
            throw new NotImplementedException();
        }
    }
}