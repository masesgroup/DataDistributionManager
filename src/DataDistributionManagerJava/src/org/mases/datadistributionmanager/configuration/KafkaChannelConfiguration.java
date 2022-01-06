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

package org.mases.datadistributionmanager.configuration;

/**
 * The configuration class for Kafka channel
 */
public class KafkaChannelConfiguration extends KafkaConfiguration {
    static final String KafkaGlobalConfigurationBasePropertyKey = "datadistributionmanager.kafka.globalconf.";
    static final String KafkaTopicConfigurationBasePropertyKey = "datadistributionmanager.kafka.topicconf.";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ReplicationFactorKey"
     * target= "_top">ReplicationFactorKey</a>
     */
    public static final String ReplicationFactorKey = "datadistributionmanager.kafka.topic_replicationfactor";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicCreateKey"
     * target= "_top">TopicCreateKey</a>
     */
    public static final String TopicCreateKey = "datadistributionmanager.kafka.topic_create";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_DumpMetadataKey"
     * target= "_top">DumpMetadataKey</a>
     */
    public static final String DumpMetadataKey = "datadistributionmanager.kafka.topic_dumpmetadata";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TransactionsEnabledKey"
     * target= "_top">TransactionsEnabledKey</a>
     */
    public static final String TransactionsEnabledKey = "datadistributionmanager.kafka.transactions.enable";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TransactionsTimeoutKey"
     * target= "_top">TransactionsTimeoutKey</a>
     */
    public static final String TransactionsTimeoutKey = "datadistributionmanager.kafka.transactions.timeout";

    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BootstrapBrokersKey"
     * target= "_top">BootstrapBrokersKey</a>
     */
    public static final String BootstrapBrokersKey = "datadistributionmanager.kafka.globalconf.metadata.broker.list";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_DebugKey"
     * target= "_top">DebugKey</a>
     */
    public static final String DebugKey = "datadistributionmanager.kafka.globalconf.debug";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ClientIdKey"
     * target= "_top">ClientIdKey</a>
     */
    public static final String ClientIdKey = "datadistributionmanager.kafka.globalconf.client.id";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_GroupIdKey"
     * target= "_top">GroupIdKey</a>
     */
    public static final String GroupIdKey = "datadistributionmanager.kafka.globalconf.group.id";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BuiltinFeaturesKey"
     * target= "_top">BuiltinFeaturesKey</a>
     */
    public static final String BuiltinFeaturesKey = "datadistributionmanager.kafka.globalconf.builtin.features";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MessageMaxBytesKey"
     * target= "_top">MessageMaxBytesKey</a>
     */
    public static final String MessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.message.max.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MessageCopyMaxBytesKey"
     * target= "_top">MessageCopyMaxBytesKey</a>
     */
    public static final String MessageCopyMaxBytesKey = "datadistributionmanager.kafka.globalconf.message.copy.max.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ReceiveMessageMaxBytesKey"
     * target= "_top">ReceiveMessageMaxBytesKey</a>
     */
    public static final String ReceiveMessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.receive.message.max.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MaxInFlightRequestsPerConnectionKey"
     * target= "_top">MaxInFlightRequestsPerConnectionKey</a>
     */
    public static final String MaxInFlightRequestsPerConnectionKey = "datadistributionmanager.kafka.globalconf.max.in.flight.requests.per.connection";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MetadataRequestTimeoutKey"
     * target= "_top">MetadataRequestTimeoutKey</a>
     */
    public static final String MetadataRequestTimeoutKey = "datadistributionmanager.kafka.globalconf.metadata.request.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicMetadataRefreshIntervalKey"
     * target= "_top">TopicMetadataRefreshIntervalKey</a>
     */
    public static final String TopicMetadataRefreshIntervalKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MetadataMaxAgeKey"
     * target= "_top">MetadataMaxAgeKey</a>
     */
    public static final String MetadataMaxAgeKey = "datadistributionmanager.kafka.globalconf.metadata.max.age.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicMetadataRefreshFastIntervalKey"
     * target= "_top">TopicMetadataRefreshFastIntervalKey</a>
     */
    public static final String TopicMetadataRefreshFastIntervalKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.fast.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicMetadataRefreshSparseKey"
     * target= "_top">TopicMetadataRefreshSparseKey</a>
     */
    public static final String TopicMetadataRefreshSparseKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.sparse";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicMetadataPropagationMaxKey"
     * target= "_top">TopicMetadataPropagationMaxKey</a>
     */
    public static final String TopicMetadataPropagationMaxKey = "datadistributionmanager.kafka.globalconf.topic.metadata.propagation.max.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicBlackListKey"
     * target= "_top">TopicBlackListKey</a>
     */
    public static final String TopicBlackListKey = "datadistributionmanager.kafka.globalconf.topic.blacklist";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketTimeoutKey"
     * target= "_top">SocketTimeoutKey</a>
     */
    public static final String SocketTimeoutKey = "datadistributionmanager.kafka.globalconf.socket.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketSendBufferBytesKey"
     * target= "_top">SocketSendBufferBytesKey</a>
     */
    public static final String SocketSendBufferBytesKey = "datadistributionmanager.kafka.globalconf.socket.send.buffer.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketReceiveBufferBytesKey"
     * target= "_top">SocketReceiveBufferBytesKey</a>
     */
    public static final String SocketReceiveBufferBytesKey = "datadistributionmanager.kafka.globalconf.socket.receive.buffer.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketKeepaliveEnableKey"
     * target= "_top">SocketKeepaliveEnableKey</a>
     */
    public static final String SocketKeepaliveEnableKey = "datadistributionmanager.kafka.globalconf.socket.keepalive.enable";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketNagleDisableKey"
     * target= "_top">SocketNagleDisableKey</a>
     */
    public static final String SocketNagleDisableKey = "datadistributionmanager.kafka.globalconf.socket.nagle.disable";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SocketMaxFailsKey"
     * target= "_top">SocketMaxFailsKey</a>
     */
    public static final String SocketMaxFailsKey = "datadistributionmanager.kafka.globalconf.socket.max.fails";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BootstrapBrokersKey"
     * target= "_top">BootstrapBrokersKey</a>
     */
    public static final String BrokerAddressTtlKey = "datadistributionmanager.kafka.globalconf.broker.address.ttl";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BrokerAddressFamilyKey"
     * target= "_top">BrokerAddressFamilyKey</a>
     */
    public static final String BrokerAddressFamilyKey = "datadistributionmanager.kafka.globalconf.broker.address.family";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ReconnectBackoffKey"
     * target= "_top">ReconnectBackoffKey</a>
     */
    public static final String ReconnectBackoffKey = "datadistributionmanager.kafka.globalconf.reconnect.backoff.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ReconnectBackoffMaxKey"
     * target= "_top">ReconnectBackoffMaxKey</a>
     */
    public static final String ReconnectBackoffMaxKey = "datadistributionmanager.kafka.globalconf.reconnect.backoff.max.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_StatisticsIntervalKey"
     * target= "_top">StatisticsIntervalKey</a>
     */
    public static final String StatisticsIntervalKey = "datadistributionmanager.kafka.globalconf.statistics.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_LogLevelKey"
     * target= "_top">LogLevelKey</a>
     */
    public static final String LogLevelKey = "datadistributionmanager.kafka.globalconf.log_level";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_LogQueueKey"
     * target= "_top">LogQueueKey</a>
     */
    public static final String LogQueueKey = "datadistributionmanager.kafka.globalconf.log.queue";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_LogThreadNameKey"
     * target= "_top">LogThreadNameKey</a>
     */
    public static final String LogThreadNameKey = "datadistributionmanager.kafka.globalconf.log.thread.name";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableRandomSeedKey"
     * target= "_top">EnableRandomSeedKey</a>
     */
    public static final String EnableRandomSeedKey = "datadistributionmanager.kafka.globalconf.enable.random.seed";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_LogConnectionCloseKey"
     * target= "_top">LogConnectionCloseKey</a>
     */
    public static final String LogConnectionCloseKey = "datadistributionmanager.kafka.globalconf.log.connection.close";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_IntervalTerminationSignalKey"
     * target= "_top">IntervalTerminationSignalKey</a>
     */
    public static final String IntervalTerminationSignalKey = "datadistributionmanager.kafka.globalconf.internal.termination.signal";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ApiVersionRequestKey"
     * target= "_top">ApiVersionRequestKey</a>
     */
    public static final String ApiVersionRequestKey = "datadistributionmanager.kafka.globalconf.api.version.request";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ApiVersionRequestTimeoutKey"
     * target= "_top">ApiVersionRequestTimeoutKey</a>
     */
    public static final String ApiVersionRequestTimeoutKey = "datadistributionmanager.kafka.globalconf.api.version.request.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ApiVersionFallbackKey"
     * target= "_top">ApiVersionFallbackKey</a>
     */
    public static final String ApiVersionFallbackKey = "datadistributionmanager.kafka.globalconf.api.version.fallback.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BrokerVersionFallbackKey"
     * target= "_top">BrokerVersionFallbackKey</a>
     */
    public static final String BrokerVersionFallbackKey = "datadistributionmanager.kafka.globalconf.broker.version.fallback";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SecurityProtocolKey"
     * target= "_top">SecurityProtocolKey</a>
     */
    public static final String SecurityProtocolKey = "datadistributionmanager.kafka.globalconf.security.protocol";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCipherSuitesKey"
     * target= "_top">SslCipherSuitesKey</a>
     */
    public static final String SslCipherSuitesKey = "datadistributionmanager.kafka.globalconf.ssl.cipher.suites";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCurvesListKey"
     * target= "_top">SslCurvesListKey</a>
     */
    public static final String SslCurvesListKey = "datadistributionmanager.kafka.globalconf.ssl.curves.list";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslSisalgsListKey"
     * target= "_top">SslSisalgsListKey</a>
     */
    public static final String SslSisalgsListKey = "datadistributionmanager.kafka.globalconf.ssl.sigalgs.list";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeyLocationKey"
     * target= "_top">SslKeyLocationKey</a>
     */
    public static final String SslKeyLocationKey = "datadistributionmanager.kafka.globalconf.ssl.key.location";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeyPasswordKey"
     * target= "_top">SslKeyPasswordKey</a>
     */
    public static final String SslKeyPasswordKey = "datadistributionmanager.kafka.globalconf.ssl.key.password";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeyPemKey"
     * target= "_top">SslKeyPemKey</a>
     */
    public static final String SslKeyPemKey = "datadistributionmanager.kafka.globalconf.ssl.key.pem";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeyKey"
     * target= "_top">SslKeyKey</a>
     */
    public static final String SslKeyKey = "datadistributionmanager.kafka.globalconf.ssl_key";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCertificateLocationKey"
     * target= "_top">SslCertificateLocationKey</a>
     */
    public static final String SslCertificateLocationKey = "datadistributionmanager.kafka.globalconf.ssl.certificate.location";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCertificatePemKey"
     * target= "_top">SslCertificatePemKey</a>
     */
    public static final String SslCertificatePemKey = "datadistributionmanager.kafka.globalconf.ssl.certificate.pem";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCertificateKey"
     * target= "_top">SslCertificateKey</a>
     */
    public static final String SslCertificateKey = "datadistributionmanager.kafka.globalconf.ssl_certificate";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCALocationKey"
     * target= "_top">SslCALocationKey</a>
     */
    public static final String SslCALocationKey = "datadistributionmanager.kafka.globalconf.ssl.ca.location";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCAKey"
     * target= "_top">SslCAKey</a>
     */
    public static final String SslCAKey = "datadistributionmanager.kafka.globalconf.ssl_ca";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCACertificateStoresKey"
     * target= "_top">SslCACertificateStoresKey</a>
     */
    public static final String SslCACertificateStoresKey = "datadistributionmanager.kafka.globalconf.ssl.ca.certificate.stores";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslCrlLocationKey"
     * target= "_top">SslCrlLocationKey</a>
     */
    public static final String SslCrlLocationKey = "datadistributionmanager.kafka.globalconf.ssl.crl.location";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeystoreLocationKey"
     * target= "_top">SslKeystoreLocationKey</a>
     */
    public static final String SslKeystoreLocationKey = "datadistributionmanager.kafka.globalconf.ssl.keystore.location";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslKeystorePasswordKey"
     * target= "_top">SslKeystorePasswordKey</a>
     */
    public static final String SslKeystorePasswordKey = "datadistributionmanager.kafka.globalconf.ssl.keystore.password";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableSslCertificateVerificationKey"
     * target= "_top">EnableSslCertificateVerificationKey</a>
     */
    public static final String EnableSslCertificateVerificationKey = "datadistributionmanager.kafka.globalconf.enable.ssl.certificate.verification";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SslEndpointIdentificationAlgorithmKey"
     * target= "_top">SslEndpointIdentificationAlgorithmKey</a>
     */
    public static final String SslEndpointIdentificationAlgorithmKey = "datadistributionmanager.kafka.globalconf.ssl.endpoint.identification.algorithm";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslMechanismKey"
     * target= "_top">SaslMechanismKey</a>
     */
    public static final String SaslMechanismKey = "datadistributionmanager.kafka.globalconf.sasl.mechanisms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslKerberosServiceNameKey"
     * target= "_top">SaslKerberosServiceNameKey</a>
     */
    public static final String SaslKerberosServiceNameKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.service.name";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslKerberosPrincipalKey"
     * target= "_top">SaslKerberosPrincipalKey</a>
     */
    public static final String SaslKerberosPrincipalKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.principal";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslKerberosKinitCmdKey"
     * target= "_top">SaslKerberosKinitCmdKey</a>
     */
    public static final String SaslKerberosKinitCmdKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.kinit.cmd";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslKerberosKeytabKey"
     * target= "_top">SaslKerberosKeytabKey</a>
     */
    public static final String SaslKerberosKeytabKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.keytab";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslKerberosMinTimeBeforeReloginKey"
     * target= "_top">SaslKerberosMinTimeBeforeReloginKey</a>
     */
    public static final String SaslKerberosMinTimeBeforeReloginKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.min.time.before.relogin";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslUsernameKey"
     * target= "_top">SaslUsernameKey</a>
     */
    public static final String SaslUsernameKey = "datadistributionmanager.kafka.globalconf.sasl.username";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslPasswordKey"
     * target= "_top">SaslPasswordKey</a>
     */
    public static final String SaslPasswordKey = "datadistributionmanager.kafka.globalconf.sasl.password";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SaslOauthbearerConfigKey"
     * target= "_top">SaslOauthbearerConfigKey</a>
     */
    public static final String SaslOauthbearerConfigKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.config";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_PluginLibraryPathsKey"
     * target= "_top">PluginLibraryPathsKey</a>
     */
    public static final String PluginLibraryPathsKey = "datadistributionmanager.kafka.globalconf.plugin.library.paths";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_GroupInstanceIdKey"
     * target= "_top">GroupInstanceIdKey</a>
     */
    public static final String GroupInstanceIdKey = "datadistributionmanager.kafka.globalconf.group.instance.id";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_PartitionAssignmentStrategyKey"
     * target= "_top">PartitionAssignmentStrategyKey</a>
     */
    public static final String PartitionAssignmentStrategyKey = "datadistributionmanager.kafka.globalconf.partition.assignment.strategy";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_SessionTimeoutKey"
     * target= "_top">SessionTimeoutKey</a>
     */
    public static final String SessionTimeoutKey = "datadistributionmanager.kafka.globalconf.session.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_HeartbeatIntervalKey"
     * target= "_top">HeartbeatIntervalKey</a>
     */
    public static final String HeartbeatIntervalKey = "datadistributionmanager.kafka.globalconf.heartbeat.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_CoordinatorQueryIntervalKey"
     * target= "_top">CoordinatorQueryIntervalKey</a>
     */
    public static final String CoordinatorQueryIntervalKey = "datadistributionmanager.kafka.globalconf.coordinator.query.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MaxPollIntervalKey"
     * target= "_top">MaxPollIntervalKey</a>
     */
    public static final String MaxPollIntervalKey = "datadistributionmanager.kafka.globalconf.max.poll.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableAutoCommitKey"
     * target= "_top">EnableAutoCommitKey</a>
     */
    public static final String EnableAutoCommitKey = "datadistributionmanager.kafka.globalconf.enable.auto.commit";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_AutoCommitIntervalKey"
     * target= "_top">AutoCommitIntervalKey</a>
     */
    public static final String AutoCommitIntervalKey = "datadistributionmanager.kafka.globalconf.auto.commit.interval.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableAutoOffsetStoreKey"
     * target= "_top">EnableAutoOffsetStoreKey</a>
     */
    public static final String EnableAutoOffsetStoreKey = "datadistributionmanager.kafka.globalconf.enable.auto.offset.store";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueuedMinMessagesKey"
     * target= "_top">QueuedMinMessagesKey</a>
     */
    public static final String QueuedMinMessagesKey = "datadistributionmanager.kafka.globalconf.queued.min.messages";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueuedMaxMessagesKbKey"
     * target= "_top">QueuedMaxMessagesKbKey</a>
     */
    public static final String QueuedMaxMessagesKbKey = "datadistributionmanager.kafka.globalconf.queued.max.messages.kbytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_FetchWaitMaxKey"
     * target= "_top">FetchWaitMaxKey</a>
     */
    public static final String FetchWaitMaxKey = "datadistributionmanager.kafka.globalconf.fetch.wait.max.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_FetchMessageMaxBytesKey"
     * target= "_top">FetchMessageMaxBytesKey</a>
     */
    public static final String FetchMessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.fetch.message.max.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_FetchMaxBytesKey"
     * target= "_top">FetchMaxBytesKey</a>
     */
    public static final String FetchMaxBytesKey = "datadistributionmanager.kafka.globalconf.fetch.max.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_FetchMinBytesKey"
     * target= "_top">FetchMinBytesKey</a>
     */
    public static final String FetchMinBytesKey = "datadistributionmanager.kafka.globalconf.fetch.min.bytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_FetchErrorBackoffKey"
     * target= "_top">FetchErrorBackoffKey</a>
     */
    public static final String FetchErrorBackoffKey = "datadistributionmanager.kafka.globalconf.fetch.error.backoff.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_IsolationLevelKey"
     * target= "_top">IsolationLevelKey</a>
     */
    public static final String IsolationLevelKey = "datadistributionmanager.kafka.globalconf.isolation.level";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnablePartitionEofKey"
     * target= "_top">EnablePartitionEofKey</a>
     */
    public static final String EnablePartitionEofKey = "datadistributionmanager.kafka.globalconf.enable.partition.eof";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_CheckCrcsKey"
     * target= "_top">CheckCrcsKey</a>
     */
    public static final String CheckCrcsKey = "datadistributionmanager.kafka.globalconf.check.crcs";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_AllowAutoCreateTopicsKey"
     * target= "_top">AllowAutoCreateTopicsKey</a>
     */
    public static final String AllowAutoCreateTopicsKey = "datadistributionmanager.kafka.globalconf.allow.auto.create.topics";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ClientRackKey"
     * target= "_top">ClientRackKey</a>
     */
    public static final String ClientRackKey = "datadistributionmanager.kafka.globalconf.client.rack";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TransactionalIdKey"
     * target= "_top">TransactionalIdKey</a>
     */
    public static final String TransactionalIdKey = "datadistributionmanager.kafka.globalconf.transactional.id";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TransactionTimeoutKey"
     * target= "_top">TransactionTimeoutKey</a>
     */
    public static final String TransactionTimeoutKey = "datadistributionmanager.kafka.globalconf.transaction.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableIdempotenceKey"
     * target= "_top">EnableIdempotenceKey</a>
     */
    public static final String EnableIdempotenceKey = "datadistributionmanager.kafka.globalconf.enable.idempotence";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_EnableGaplessGuaranteeKey"
     * target= "_top">EnableGaplessGuaranteeKey</a>
     */
    public static final String EnableGaplessGuaranteeKey = "datadistributionmanager.kafka.globalconf.enable.gapless.guarantee";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueueBufferingMaxMessagesKey"
     * target= "_top">QueueBufferingMaxMessagesKey</a>
     */
    public static final String QueueBufferingMaxMessagesKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.messages";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueueBufferingMaxKbKey"
     * target= "_top">QueueBufferingMaxKbKey</a>
     */
    public static final String QueueBufferingMaxKbKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.kbytes";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueueBufferingMaxMsKey"
     * target= "_top">QueueBufferingMaxMsKey</a>
     */
    public static final String QueueBufferingMaxMsKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MessageSendMaxRetriesKey"
     * target= "_top">MessageSendMaxRetriesKey</a>
     */
    public static final String MessageSendMaxRetriesKey = "datadistributionmanager.kafka.globalconf.message.send.max.retries";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_RetryBackoffKey"
     * target= "_top">RetryBackoffKey</a>
     */
    public static final String RetryBackoffKey = "datadistributionmanager.kafka.globalconf.retry.backoff.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueueBufferingBackpressureThresholdKey"
     * target= "_top">QueueBufferingBackpressureThresholdKey</a>
     */
    public static final String QueueBufferingBackpressureThresholdKey = "datadistributionmanager.kafka.globalconf.queue.buffering.backpressure.threshold";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_CompressionCodecKey"
     * target= "_top">CompressionCodecKey</a>
     */
    public static final String CompressionCodecKey = "datadistributionmanager.kafka.globalconf.compression.codec";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BatchNumMessagesKey"
     * target= "_top">BatchNumMessagesKey</a>
     */
    public static final String BatchNumMessagesKey = "datadistributionmanager.kafka.globalconf.batch.num.messages";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_BatchSizeKey"
     * target= "_top">BatchSizeKey</a>
     */
    public static final String BatchSizeKey = "datadistributionmanager.kafka.globalconf.batch.size";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_DeliveryReportOnlyErrorKey"
     * target= "_top">DeliveryReportOnlyErrorKey</a>
     */
    public static final String DeliveryReportOnlyErrorKey = "datadistributionmanager.kafka.globalconf.delivery.report.only.error";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_StickyPartitioningLingerKey"
     * target= "_top">StickyPartitioningLingerKey</a>
     */
    public static final String StickyPartitioningLingerKey = "datadistributionmanager.kafka.globalconf.sticky.partitioning.linger.ms";

    // end Global conf

    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_RequestRequiredAcksKey"
     * target= "_top">RequestRequiredAcksKey</a>
     */
    public static final String RequestRequiredAcksKey = "datadistributionmanager.kafka.topicconf.request.required.acks";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_RequestTimeoutKey"
     * target= "_top">RequestTimeoutKey</a>
     */
    public static final String RequestTimeoutKey = "datadistributionmanager.kafka.topicconf.request.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_MessageTimeoutKey"
     * target= "_top">MessageTimeoutKey</a>
     */
    public static final String MessageTimeoutKey = "datadistributionmanager.kafka.topicconf.message.timeout.ms";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_QueueingStrategyKey"
     * target= "_top">QueueingStrategyKey</a>
     */
    public static final String QueueingStrategyKey = "datadistributionmanager.kafka.topicconf.queuing.strategy";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_PartitionerKey"
     * target= "_top">PartitionerKey</a>
     */
    public static final String PartitionerKey = "datadistributionmanager.kafka.topicconf.partitioner";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_TopicCompressionCodecKey"
     * target= "_top">TopicCompressionCodecKey</a>
     */
    public static final String TopicCompressionCodecKey = "datadistributionmanager.kafka.topicconf.compression.codec";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_CompressionLevelKey"
     * target= "_top">CompressionLevelKey</a>
     */
    public static final String CompressionLevelKey = "datadistributionmanager.kafka.topicconf.compression.level";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_AutoOffsetResetKey"
     * target= "_top">AutoOffsetResetKey</a>
     */
    public static final String AutoOffsetResetKey = "datadistributionmanager.kafka.topicconf.auto.offset.reset";
    /**
     * See .NET <a href=
     * "../../../../../api/MASES.DataDistributionManager.Bindings.Configuration.KafkaChannelConfiguration.html#MASES_DataDistributionManager_Bindings_Configuration_KafkaChannelConfiguration_ConsumeCallbackMaxMessagesKey"
     * target= "_top">ConsumeCallbackMaxMessagesKey</a>
     */
    public static final String ConsumeCallbackMaxMessagesKey = "datadistributionmanager.kafka.topicconf.consume.callback.max.messages";

    // end Topic conf

    /**
     * Kafka configuration type
     */
    public enum KafkaConfigurationType {
        /**
         * Global configuration paramters
         */
        GLOBAL_CONF,
        /**
         * Topic configuration paramters
         */
        TOPIC_CONF
    }

    /**
     * Specific values to use within
     * {@link org.mases.datadistributionmanager.CommonConfiguration#setInitialOffset(Long)}
     * when channel is based on Kafka
     */
    public static class InitialOffsetTypes {
        /**
         * Start consuming from beginning of kafka partition queue: oldest message
         */
        public static long Beginning = -2;
        /**
         * Start consuming from end of kafka partition queue: next message
         */
        public static long End = -1;
        /**
         * Start consuming from offset retrieved from offset store
         */
        public static long Stored = -1000;
    }

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link KafkaConfiguration} to duplicate
     */
    public KafkaChannelConfiguration(KafkaConfiguration originalConf) {
        super(originalConf);
    }

    /**
     * 
     * @return The client id to be used
     */
    public String getClientId() {
        String value = keyValuePair.get(ClientIdKey);
        return (value == null) ? "" : value;
    }

    /**
     * The client id to be used
     * 
     * @param clientId The client id to be used
     */
    public void setClientId(String clientId) {
        keyValuePair.put(ClientIdKey, clientId);
    }

    /**
     * 
     * @return The group id to be used
     */
    public String getGroupId() {
        String value = keyValuePair.get(GroupIdKey);
        return (value == null) ? "" : value;
    }

    /**
     * The group id to be used
     * 
     * @param groupId The group id to be used
     */
    public void setGroupId(String groupId) {
        keyValuePair.put(GroupIdKey, groupId);
    }

    /**
     * 
     * @return The debug level (e.g. metadata,topic,msg,broker)
     */
    public String getDebug() {
        String value = keyValuePair.get(DebugKey);
        return (value == null) ? "" : value;
    }

    /**
     * The debug level (e.g. metadata,topic,msg,broker)
     * 
     * @param debug The debug level (e.g. metadata,topic,msg,broker)
     */
    public void setDebug(String debug) {
        keyValuePair.put(DebugKey, debug);
    }

    /**
     * 
     * @return The bootstrap broker(s) to be used as Comma Separated Value(s)
     */
    public String getBootstrapBrokers() {
        String value = keyValuePair.get(BootstrapBrokersKey);
        return (value == null) ? "" : value;
    }

    /**
     * The bootstrap broker(s) to be used as Comma Separated Value(s)
     * 
     * @param brokers The bootstrap broker(s) to be used as Comma Separated Value(s)
     */
    public void setBootstrapBrokers(String brokers) {
        keyValuePair.put(BootstrapBrokersKey, brokers);
    }

    /**
     * 
     * @return The replication factor to be used
     */
    public Integer getReplicationFactor() {
        String value = keyValuePair.get(ReplicationFactorKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The replication factor to be used
     * 
     * @param value The replication factor to be used
     */
    public void setReplicationFactor(Integer value) {
        keyValuePair.put(ReplicationFactorKey, value.toString());
    }

    /**
     * True to request a create topic
     * 
     * @return True to request a create topic
     */
    public boolean getTopicCreate() {
        String value = keyValuePair.get(TopicCreateKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to request a create topic
     * 
     * @param value True to request a create topic
     */
    public void setTopicCreate(Boolean value) {
        keyValuePair.put(TopicCreateKey, value.toString());
    }

    /**
     * True to dump metadata
     * 
     * @return True to dump metadata
     */
    public boolean getDumpMetadata() {
        String value = keyValuePair.get(DumpMetadataKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to dump metadata
     * 
     * @param value True to dump metadata
     */
    public void setDumpMetadata(Boolean value) {
        keyValuePair.put(DumpMetadataKey, value.toString());
    }

    /**
     * True to enable transactions
     * 
     * @return True to enable transactions
     */
    public boolean getTransactionsEnabled() {
        String value = keyValuePair.get(TransactionsEnabledKey);
        return (value == null) ? false : Boolean.parseBoolean(value);
    }

    /**
     * True to enable transactions
     * 
     * @param value True to enable transactions
     */
    public void setTransactionsEnabled(Boolean value) {
        keyValuePair.put(TransactionsEnabledKey, value.toString());
    }

    /**
     * The timeout to be used in transaction operations when TransactionsEnabled is
     * true
     * 
     * @return The timeout in ms
     */
    public Integer getTransactionsTimeout() {
        String value = keyValuePair.get(TransactionsTimeoutKey);
        return (value == null) ? 0 : Integer.parseInt(value);
    }

    /**
     * The timeout to be used in transaction operations when TransactionsEnabled is
     * true
     * 
     * @param value The timeout in ms
     */
    public void setTransactionsTimeout(Integer value) {
        keyValuePair.put(TransactionsTimeoutKey, value.toString());
    }

    /**
     * Generic getter for all configuration properties related to librdkafka (see
     * https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
     * 
     * @param type     {@link KafkaConfigurationType} type
     * @param property The property name
     * @return The property value
     */
    public String getProperty(KafkaConfigurationType type, String property) {
        String propKey = null;
        switch (type) {
            case GLOBAL_CONF:
                propKey = KafkaGlobalConfigurationBasePropertyKey;
                break;
            case TOPIC_CONF:
                propKey = KafkaTopicConfigurationBasePropertyKey;
                break;
            default:
                throw new IllegalArgumentException(String.format("Type %s is invalid", type.toString()));
        }

        String value = "";
        if (property.startsWith(propKey)) {
            if (keyValuePair.containsKey(property))
                value = keyValuePair.get(property);
        } else {
            if (keyValuePair.containsKey(propKey + property))
                value = keyValuePair.get(propKey + property);
        }
        return value;
    }

    /**
     * Generic setter for all configuration properties related to librdkafka (see
     * https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
     * 
     * @param type     {@link KafkaConfigurationType} type
     * @param property The property name
     * @param value    The property value
     */
    public void setProperty(KafkaConfigurationType type, String property, String value) {
        String propKey = null;
        switch (type) {
            case GLOBAL_CONF:
                propKey = KafkaGlobalConfigurationBasePropertyKey;
                break;
            case TOPIC_CONF:
                propKey = KafkaTopicConfigurationBasePropertyKey;
                break;
            default:
                throw new IllegalArgumentException(String.format("Type %s is invalid", type.toString()));
        }

        if (property.startsWith(propKey)) {
            keyValuePair.put(property, value);
        } else {
            keyValuePair.put(propKey + property, value);
        }
    }

    @Override
    protected void CheckConfiguration() throws IllegalArgumentException {
        super.CheckConfiguration();
        if (!keyValuePair.containsKey(BootstrapBrokersKey)) {
            throw new IllegalArgumentException("Missing BootstrapBrokers");
        }
        if (!keyValuePair.containsKey(ClientIdKey)) {
            throw new IllegalArgumentException("Missing ClientId");
        }
        if (!keyValuePair.containsKey(GroupIdKey)) {
            throw new IllegalArgumentException("Missing GroupId");
        }
    }

    @Override
    public String[] getConfiguration() throws IllegalArgumentException {
        CheckConfiguration();
        return super.getConfiguration();
    }
}
