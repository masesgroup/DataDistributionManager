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
using MASES.DataDistributionManager.Bindings.Interop;
using System;
using System.Diagnostics;
using System.Text;
using System.Threading;

namespace ManagerTestNet
{
    class MySmartDataDistribution : SmartDataDistribution
    {
        public override void OnLogging(IntPtr IDataDistribution_nativePtr, DDM_LOG_LEVEL level, string source, string function, string errStr)
        {
            base.OnLogging(IDataDistribution_nativePtr, level, source, function, errStr);

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
        const int THRESHOLD = 1000;

        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Missing configuration path on command line");
                return;
            }
            byte[] buffer;
            uint counter = 100;

            int pid = Process.GetCurrentProcess().Id;

            var str = string.Format("{0:10}", pid);
            buffer = Encoding.UTF8.GetBytes(str);

            DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.TRANSMITTER;

            MySmartDataDistribution dataDistribution = new MySmartDataDistribution();

            OpenDDSConfiguration conf = new OpenDDSConfiguration();
            conf.DCPSInfoRepoAutostart = true;
            conf.DCPSInfoRepoCommandLine = "-ORBEndpoint iiop://localhost:12345";
            conf.DCPSConfigFile = "dds_tcp_conf.ini";
            conf.DCPSTransportDebugLevel = 10;

            var hRes = dataDistribution.Initialize(conf);

            // var hRes = dataDistribution.Initialize(args[0], str, "KafkaManager");

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

            MySmartDataDistributionChannel testChannel = dataDistribution.CreateSmartChannel<MySmartDataDistributionChannel>("test");

            Console.WriteLine("After StartMasterConsumerAndWait...\n");

            testChannel.StartChannel(uint.MaxValue);

            Thread.Sleep(10000);

            Console.WriteLine("Starting sending...\n");
            while (true)
            {
                if (direction.HasFlag(DDM_CHANNEL_DIRECTION.TRANSMITTER) ? testChannel.WriteOnChannel(null, buffer) : true)
                {
                    str = string.Format("{0:10}", counter++);
                    buffer = Encoding.UTF8.GetBytes(str);
                    if ((counter % THRESHOLD) == 0) Console.WriteLine("SendData Reached {0}", counter);
                }
                Thread.Sleep(1000);
            }
        }
    }
}