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

import java.nio.charset.Charset;

import org.mases.datadistributionmanager.*;

public class DataDistributionManagerJavaTest {
	public static void main(String args[]) {
		final int THRESHOLD = 1000;

		DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.RECEIVER;
		MySmartDataDistribution dataDistribution = new MySmartDataDistribution();
		HRESULT hRes = HRESULT.S_OK;
		if (args.length == 0) {
			OpenDDSConfiguration conf = new OpenDDSConfiguration();
			// set the full command line
			// conf.setCommandLine("-DCPSConfigFile dds_tcp_conf.ini -DCPSTransportDebugLevel 10");
			// set direct values
			conf.setDCPSInfoRepoAutostart(true);
			conf.setDCPSInfoRepoCommandLine("-ORBEndpoint iiop://localhost:12345");
			conf.setDCPSConfigFile("dds_tcp_conf.ini");
			conf.setDCPSTransportDebugLevel(10);
			String[] confRes = conf.getConfiguration();
			hRes = dataDistribution.Initialize(conf);
		} else {
			hRes = dataDistribution.Initialize(args[0]);
		}

		if (hRes.getFailed()) {
			System.out.println("Error in configuration.");
			return;
		}

		hRes = dataDistribution.Start(10000);

		if (hRes.getFailed()) {
			return;
		}

		MySmartDataDistributionTopic mytestTopic;
		try {
			mytestTopic = dataDistribution.CeateSmartChannel(MySmartDataDistributionTopic.class, "test");
		} catch (Throwable e) {
			e.printStackTrace();
			return;
		}

		System.out.println("After CeateSmartChannel...\n");

		mytestTopic.StartChannel(10000);

		try {
			System.out.println("Starting sending...\n");
			int counter = 100;
			String str = "test";
			while (true) {
				hRes = HRESULT.S_OK;
				if (direction == DDM_CHANNEL_DIRECTION.TRANSMITTER) {
					hRes = mytestTopic.WriteOnChannel(null, str);
				}
				if (hRes == HRESULT.S_OK) {
					str = String.format("%d", counter++);
					if ((counter % THRESHOLD) == 0)
						System.out.println(String.format("SendData Reached %d", counter));
				}
				Thread.sleep(1000);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}