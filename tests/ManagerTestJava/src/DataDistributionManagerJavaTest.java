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
		String str = "test";
		HRESULT hRes = dataDistribution.Initialize("OpenDDSManager.conf", str, "Manager");

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
			mytestTopic = dataDistribution.CeateSmartChannel(MySmartDataDistributionTopic.class, "test",
					DDM_CHANNEL_DIRECTION.RECEIVER, null);
		} catch (Throwable e) {
			e.printStackTrace();
			return;
		}

		System.out.println("After StartMasterConsumerAndWait...\n");

		mytestTopic.StartChannel(10000);

		try {
			System.out.println("Starting sending...\n");

			int counter = 100;

			byte[] buffer = str.getBytes(Charset.forName("ASCII"));
			while (true) {
				hRes = HRESULT.S_OK;
				if (direction == DDM_CHANNEL_DIRECTION.TRANSMITTER) {
					hRes = mytestTopic.WriteOnChannel(null, buffer, false, -1);
				}
				if (hRes == HRESULT.S_OK) {
					str = String.format("{0:10}", counter++);
					buffer = str.getBytes(Charset.forName("ASCII"));
					if ((counter % THRESHOLD) == 0)
						System.out.println(String.format("SendData Reached {0}", counter));
				}
				Thread.sleep(1000);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}