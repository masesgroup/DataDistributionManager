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

import org.mases.datadistributionmanager.*;
import org.mases.datadistributionmanager.configuration.OpenDDSConfiguration.DCPSInfoRepoConfiguration;
import org.mases.datadistributionmanager.configuration.OpenDDSConfiguration.OpenDDSArgsConfiguration;
import org.mases.datadistributionmanager.configuration.*;

public class DataDistributionManagerJavaTest {
	public static void main(String args[]) {
		final int THRESHOLD = 10;

		DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION.RECEIVER;
		MySmartDataDistribution dataDistribution = new MySmartDataDistribution();
		HRESULT hRes = HRESULT.S_OK;
		OpenDDSConfiguration conf = new OpenDDSConfiguration();
		if (args.length == 0) {
			OpenDDSArgsConfiguration argsConf = conf.new OpenDDSArgsConfiguration();
			// set the full command line
			// argsConf.setCommandLine("-DCPSConfigFile dds_tcp_conf.ini
			// -DCPSTransportDebugLevel 10");
			argsConf.setDCPSConfigFile("dds_tcp_conf.ini");
			argsConf.setDCPSTransportDebugLevel(10);
			conf.setOpenDDSArgs(argsConf);
			if (direction == DDM_CHANNEL_DIRECTION.RECEIVER) {
				// start info repo on receiver
				DCPSInfoRepoConfiguration infoRepo = conf.new DCPSInfoRepoConfiguration();
				infoRepo.setAutostart(true);
				infoRepo.setORBEndpoint("iiop://localhost:12345");
				infoRepo.setMonitor(true);
				infoRepo.setResurrect(true);
				infoRepo.setPersistenceFile("persistance.file");
				conf.setDCPSInfoRepo(infoRepo);
			}

			DomainParticipantQosConfiguration domainPartQos = new DomainParticipantQosConfiguration();
			domainPartQos.EntityFactoryQosPolicy = new EntityFactoryQosPolicyConfiguration();
			domainPartQos.EntityFactoryQosPolicy.setAutoenableCreatedEntities(true);
			domainPartQos.UserDataQosPolicy = new UserDataQosPolicyConfiguration();
			domainPartQos.UserDataQosPolicy.setValue(new Byte[] { 102, 105 });
			domainPartQos.PropertyQosPolicy = new PropertyQosPolicyConfiguration();
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_IDENTITY_CA = domainPartQos.PropertyQosPolicy.new Property("1",
					false);
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_IDENTITY_CERT = domainPartQos.PropertyQosPolicy.new Property(
					"2", false);
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_IDENTITY_PRIVKEY = domainPartQos.PropertyQosPolicy.new Property(
					"3", false);
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_PERM_CA = domainPartQos.PropertyQosPolicy.new Property("4",
					false);
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_PERM_DOC = domainPartQos.PropertyQosPolicy.new Property("5",
					false);
			domainPartQos.PropertyQosPolicy.DDSSEC_PROP_PERM_GOV_DOC = domainPartQos.PropertyQosPolicy.new Property("6",
					false);
			conf.setDomainParticipantQos(domainPartQos);
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
		OpenDDSChannelConfiguration channelConf = new OpenDDSChannelConfiguration(conf);
		String[] channelConfRes = channelConf.getConfiguration();
		MySmartDataDistributionTopic mytestTopic;
		try {
			mytestTopic = dataDistribution.CreateSmartChannel(MySmartDataDistributionTopic.class, "test");
		} catch (Throwable e) {
			e.printStackTrace();
			return;
		}

		System.out.println("After CeateSmartChannel...\n");

		mytestTopic.StartChannel(10000);

		try {
			if (direction == DDM_CHANNEL_DIRECTION.TRANSMITTER)
				System.out.println("Starting sending...\n");
			else if (direction == DDM_CHANNEL_DIRECTION.RECEIVER)
				System.out.println("Waiting messages...\n");
			int counter = 100;
			String str = "test";
			while (true) {
				hRes = HRESULT.S_OK;
				if (direction == DDM_CHANNEL_DIRECTION.TRANSMITTER) {
					hRes = mytestTopic.WriteOnChannel(str);
					if (hRes.getSuccess()) {
						str = String.format("%d", counter++);
						if ((counter % THRESHOLD) == 0) {
							String key = String.format("SendData Reached %d", counter);
							hRes = mytestTopic.WriteOnChannel(key, str);
							System.out.println(key);
						}
					}
				}
				Thread.sleep(1000);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}