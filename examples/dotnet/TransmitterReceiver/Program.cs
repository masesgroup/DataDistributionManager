/*
*  Copyright 2022 MASES s.r.l.
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
			bool transmitter = false;
			
            SmartDataDistribution dataDistribution = new SmartDataDistribution();
            dataDistribution.LoggingEvent += DataDistribution_LoggingEvent;
            OpenDDSConfiguration conf = null;
            HRESULT hRes = HRESULT.S_OK;

            if (args.Length == 1)
			{
				if (args[0] == "t") transmitter = true;
				else
				{
					Console.WriteLine("Usage TransmitterReceiver [t]");
					Console.WriteLine("Use t for transmitter.");
					return;
				}
			}
            
			conf = new OpenDDSConfiguration()
			{
				OpenDDSArgs = new OpenDDSConfiguration.OpenDDSArgsConfiguration()
				{
					DCPSConfigFile = "dds_tcp_conf.ini",
					DCPSTransportDebugLevel = 10,
				},
				DCPSInfoRepo = new OpenDDSConfiguration.DCPSInfoRepoConfiguration()
				{
					Autostart = !transmitter,
					ORBEndpoint = "iiop://localhost:12345",
				}
			};
			hRes = dataDistribution.Initialize(conf);

            if (hRes.Failed)
            {
                Console.WriteLine("Error in configuration.");
                Console.ReadKey();
                return;
            }
			hRes = dataDistribution.Start(uint.MaxValue);
            if (hRes.Failed)
            {
				Console.WriteLine("Error in Start.");
                Console.ReadKey();
                return;
            }

            SmartDataDistributionChannel testChannel = dataDistribution.CreateSmartChannel<SmartDataDistributionChannel>("testChannel");
            testChannel.DataAvailable += TestChannel_DataAvailable;
            testChannel.ConditionOrError += TestChannel_ConditionOrError;
            testChannel.StartChannel(uint.MaxValue);
			
			int pid = Process.GetCurrentProcess().Id;
            uint counter = 0;
            var str = string.Format("Pid: {0} {1:10}", pid, counter);
            if (transmitter) Console.WriteLine("Starting sending...\n");
			else Console.WriteLine("Waiting messages...\n");
            while (true)
            {
                if (transmitter)
                {
					if (testChannel.WriteOnChannel(str) == HRESULT.S_OK)
					{
						str = string.Format("Pid: {0} {1:10}", pid, counter++);
						if ((counter % THRESHOLD) == 0)
						{
							Console.WriteLine("SendData Reached {0}", counter);
						}
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