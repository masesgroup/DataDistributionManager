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

using System;
using System.Collections.Generic;

namespace MASES.DataDistributionManager.Bindings.Configuration
{
    #region KafkaConfigurationType
    /// <summary>
    /// Kafka configuration type
    /// </summary>
    public enum KafkaConfigurationType
    {
        /// <summary>
        /// Global configuration paramters
        /// </summary>
        GLOBAL_CONF,
        /// <summary>
        /// Topic configuration paramters
        /// </summary>
        TOPIC_CONF
    }
    #endregion

    #region BuiltinFeaturesType
    /// <summary>
    /// Indicates the builtin features for this build of librdkafka.
    /// </summary>
    [Flags]
    public enum BuiltinFeaturesType
    {
        /// <summary>
        /// gzip
        /// </summary>
        gzip = 0x1,
        /// <summary>
        /// snappy
        /// </summary>
        snappy = 0x2,
        /// <summary>
        /// ssl
        /// </summary>
        ssl = 0x4,
        /// <summary>
        /// sasl
        /// </summary>
        sasl = 0x8,
        /// <summary>
        /// regex
        /// </summary>
        regex = 0x10,
        /// <summary>
        /// lz4
        /// </summary>
        lz4 = 0x20,
        /// <summary>
        /// sasl_gssapi
        /// </summary>
        sasl_gssapi = 0x40,
        /// <summary>
        /// sasl_plain
        /// </summary>
        sasl_plain = 0x80,
        /// <summary>
        /// sasl_scram
        /// </summary>
        sasl_scram = 0x100,
        /// <summary>
        /// plugins
        /// </summary>
        plugins = 0x200,
        /// <summary>
        /// zstd
        /// </summary>
        zstd = 0x400,
        /// <summary>
        /// sasl_oauthbearer
        /// </summary>
        sasl_oauthbearer = 0x800
    }
    #endregion

    #region DebugType
    /// <summary>
    /// Debug levels
    /// </summary>
    [Flags]
    public enum DebugType
    {
        /// <summary>
        /// see docs
        /// </summary>
        generic = 0x1,
        /// <summary>
        /// see docs
        /// </summary>
        broker = 0x2,
        /// <summary>
        /// see docs
        /// </summary>
        topic = 0x4,
        /// <summary>
        /// see docs
        /// </summary>
        metadata = 0x8,
        /// <summary>
        /// see docs
        /// </summary>
        feature = 0x10,
        /// <summary>
        /// see docs
        /// </summary>
        queue = 0x20,
        /// <summary>
        /// see docs
        /// </summary>
        msg = 0x40,
        /// <summary>
        /// see docs
        /// </summary>
        protocol = 0x80,
        /// <summary>
        /// see docs
        /// </summary>
        cgrp = 0x100,
        /// <summary>
        /// see docs
        /// </summary>
        security = 0x200,
        /// <summary>
        /// see docs
        /// </summary>
        fetch = 0x400,
        /// <summary>
        /// see docs
        /// </summary>
        interceptor = 0x800,
        /// <summary>
        /// see docs
        /// </summary>
        plugin = 0x1000,
        /// <summary>
        /// see docs
        /// </summary>
        consumer = 0x2000,
        /// <summary>
        /// see docs
        /// </summary>
        admin = 0x4000,
        /// <summary>
        /// see docs
        /// </summary>
        eos = 0x8000,
        /// <summary>
        /// see docs
        /// </summary>
        mock = 0x10000,
        /// <summary>
        /// see docs
        /// </summary>
        assignor = 0x20000,
        /// <summary>
        /// see docs
        /// </summary>
        conf = 0x40000,
        /// <summary>
        /// see docs
        /// </summary>
        all = 0x80000
    }
    #endregion

    #region BrokerAddressFamilyType
    /// <summary>
    /// Allowed broker IP address families: any, v4, v6
    /// </summary>
    public enum BrokerAddressFamilyType
    {
        /// <summary>
        /// Any
        /// </summary>
        any = 0x1,
        /// <summary>
        /// IPv4
        /// </summary>
        v4 = 0x2,
        /// <summary>
        /// IPv6
        /// </summary>
        v6 = 0x4
    }
    #endregion

    #region SecurityProtocolType
    /// <summary>
    /// Protocol used to communicate with brokers.
    /// </summary>
    public enum SecurityProtocolType
    {
        /// <summary>
        /// PLAINTEXT
        /// </summary>
        plaintext = 0x1,
        /// <summary>
        /// SSL
        /// </summary>
        ssl = 0x2,
        /// <summary>
        /// SASL PLAINTEXT
        /// </summary>
        sasl_plaintext = 0x4,
        /// <summary>
        /// SASL SSL
        /// </summary>
        sasl_ssl = 0x8
    }
    #endregion

    #region SslEndpointIdentificationAlgorithmType
    /// <summary>
    /// Endpoint identification algorithm to validate broker hostname using broker certificate.
    /// </summary>
    public enum SslEndpointIdentificationAlgorithmType
    {
        /// <summary>
        /// No endpoint verification.
        /// </summary>
        none = 0x1,
        /// <summary>
        /// Server (broker) hostname verification as specified in RFC2818.
        /// </summary>
        https = 0x2
    }
    #endregion

    #region SaslMechanismType
    /// <summary>
    /// SASL mechanism to use for authentication.
    /// </summary>
    public enum SaslMechanismType
    {
        /// <summary>
        /// GSSAPI
        /// </summary>
        GSSAPI,
        /// <summary>
        /// GSSAPI
        /// </summary>
        PLAIN,
        /// <summary>
        /// GSSAPI
        /// </summary>
        SCRAM_SHA_256,
        /// <summary>
        /// GSSAPI
        /// </summary>
        SCRAM_SHA_512,
        /// <summary>
        /// GSSAPI
        /// </summary>
        OAUTHBEARER
    }
    #endregion

    #region PartitionAssignmentStrategyType
    /// <summary>
    /// The name of one or more partition assignment strategies.
    /// </summary>
    [Flags]
    public enum PartitionAssignmentStrategyType
    {
        /// <summary>
        /// range
        /// </summary>
        range = 0x1,
        /// <summary>
        /// roundrobin
        /// </summary>
        roundrobin = 0x2,
        /// <summary>
        /// cooperative_sticky
        /// </summary>
        cooperative_sticky = 0x4
    }
    #endregion

    #region IsolationLevelType
    /// <summary>
    /// Controls how to read messages written transactionally
    /// </summary>
    public enum IsolationLevelType
    {
        /// <summary>
        /// only return transactional messages which have been committed
        /// </summary>
        read_committed = 0x1,
        /// <summary>
        /// return all messages, even transactional messages which have been aborted
        /// </summary>
        read_uncommitted = 0x2,
    }
    #endregion

    #region CompressionCodecType
    /// <summary>
    /// compression codec to use for compressing message sets
    /// </summary>
    public enum CompressionCodecType
    {
        /// <summary>
        /// none
        /// </summary>
        none,
        /// <summary>
        /// gzip
        /// </summary>
        gzip,
        /// <summary>
        /// snappy
        /// </summary>
        snappy,
        /// <summary>
        /// lz4
        /// </summary>
        lz4,
        /// <summary>
        /// zstd
        /// </summary>
        zstd
    }
    #endregion

    #region QueueingStrategyType
    /// <summary>
    /// compression codec to use for compressing message sets
    /// </summary>
    public enum QueueingStrategyType
    {
        /// <summary>
        /// fifo
        /// </summary>
        fifo,
        /// <summary>
        /// fifo
        /// </summary>
        lifo,
    }
    #endregion

    #region AutoOffsetResetType
    /// <summary>
    /// Action to take when there is no initial offset in offset store or the desired offset is out of range
    /// </summary>
    public enum AutoOffsetResetType
    {
        /// <summary>
        /// smallest
        /// </summary>
        smallest,
        /// <summary>
        /// earliest
        /// </summary>
        earliest,
        /// <summary>
        /// beginning
        /// </summary>
        beginning,
        /// <summary>
        /// largest
        /// </summary>
        largest,
        /// <summary>
        /// latest
        /// </summary>
        latest,
        /// <summary>
        /// end
        /// </summary>
        end,
        /// <summary>
        /// error
        /// </summary>
        error
    }
    #endregion

    #region PartitionerType
    /// <summary>
    /// Partitioner
    /// </summary>
    public enum PartitionerType
    {
        /// <summary>
        /// random distribution
        /// </summary>
        random,
        /// <summary>
        /// CRC32 hash of key(Empty and NULL keys are mapped to single partition)
        /// </summary>
        consistent,
        /// <summary>
        /// CRC32 hash of key(Empty and NULL keys are randomly partitioned)
        /// </summary>
        consistent_random,
        /// <summary>
        /// Java Producer compatible Murmur2 hash of key(NULL keys are mapped to single partition)
        /// </summary>
        murmur2,
        /// <summary>
        /// Java Producer compatible Murmur2 hash of key(NULL keys are randomly partitioned.This is functionally equivalent to the default partitioner in the Java Producer.)
        /// </summary>
        murmur2_random,
        /// <summary>
        /// FNV-1a hash of key(NULL keys are mapped to single partition)
        /// </summary>
        fnv1a,
        /// <summary>
        /// FNV-1a hash of key(NULL keys are randomly partitioned)
        /// </summary>
        fnv1a_random
    }
    #endregion

    #region InitialOffsetTypes
    /// <summary>
    /// Specific values to use on <see cref="CommonConfiguration.InitialOffset"/> when channel is based on Kafka
    /// </summary>
    public static class InitialOffsetTypes
    {
        /// <summary>
        /// Start consuming from beginning of kafka partition queue: oldest message
        /// </summary>
        public static long Beginning = -2;
        /// <summary>
        /// Start consuming from end of kafka partition queue: next message
        /// </summary>
        public static long End = -1;
        /// <summary>
        /// Start consuming from offset retrieved from offset store
        /// </summary>
        public static long Stored = -1000;
    }
    #endregion

    #region SaslOauthbearerMethodType
    /// <summary>
    /// SaslOauthbearerMethod
    /// </summary>
    public enum SaslOauthbearerMethodType
    {
        /// <summary>
        /// DEFAULT
        /// </summary>
        DEFAULT,
        /// <summary>
        /// OIDC
        /// </summary>
        OIDC,
    }
    #endregion

    #region KafkaChannelConfiguration
    /// <summary>
    /// The configuration class for Kafka channel
    /// </summary>
    public class KafkaChannelConfiguration : KafkaConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="KafkaConfiguration"/> for global configuration
        /// </summary>
        public const string KafkaGlobalConfigurationBasePropertyKey = "datadistributionmanager.kafka.globalconf.";
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="KafkaConfiguration"/> for topic configuration
        /// </summary>
        public const string KafkaTopicConfigurationBasePropertyKey = "datadistributionmanager.kafka.topicconf.";

        #region Kafka module
        /// <summary>
        /// Configuration key of <see cref="ReplicationFactor"/>
        /// </summary>
        public const string ReplicationFactorKey = "datadistributionmanager.kafka.topic_replicationfactor";
        /// <summary>
        /// Configuration key of <see cref="TopicCreate"/>
        /// </summary>
        public const string TopicCreateKey = "datadistributionmanager.kafka.topic_create";
        /// <summary>
        /// Configuration key of <see cref="DumpMetadata"/>
        /// </summary>
        public const string DumpMetadataKey = "datadistributionmanager.kafka.topic_dumpmetadata";
        /// <summary>
        /// Configuration key of <see cref="TransactionsEnabled"/>
        /// </summary>
        public const string TransactionsEnabledKey = "datadistributionmanager.kafka.transactions.enable";
        /// <summary>
        /// Configuration key of <see cref="TransactionsTimeout"/>
        /// </summary>
        public const string TransactionsTimeoutKey = "datadistributionmanager.kafka.transactions.timeout";
        #endregion

        #region Global conf
        /// <summary>
        /// Configuration key of <see cref="BootstrapBrokers"/>
        /// </summary>
        public const string BootstrapBrokersKey = "datadistributionmanager.kafka.globalconf.metadata.broker.list";
        /// <summary>
        /// Configuration key of <see cref="Debug"/>
        /// </summary>
        public const string DebugKey = "datadistributionmanager.kafka.globalconf.debug";
        /// <summary>
        /// Configuration key of <see cref="ClientId"/>
        /// </summary>
        public const string ClientIdKey = "datadistributionmanager.kafka.globalconf.client.id";
        /// <summary>
        /// Configuration key of <see cref="GroupId"/>
        /// </summary>
        public const string GroupIdKey = "datadistributionmanager.kafka.globalconf.group.id";
        /// <summary>
        /// Configuration key of <see cref="BuiltinFeatures"/>
        /// </summary>
        public const string BuiltinFeaturesKey = "datadistributionmanager.kafka.globalconf.builtin.features";
        /// <summary>
        /// Configuration key of <see cref="MessageMaxBytes"/>
        /// </summary>
        public const string MessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.message.max.bytes";
        /// <summary>
        /// Configuration key of <see cref="MessageCopyMaxBytes"/>
        /// </summary>
        public const string MessageCopyMaxBytesKey = "datadistributionmanager.kafka.globalconf.message.copy.max.bytes";
        /// <summary>
        /// Configuration key of <see cref="ReceiveMessageMaxBytes"/>
        /// </summary>
        public const string ReceiveMessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.receive.message.max.bytes";
        /// <summary>
        /// Configuration key of <see cref="MaxInFlightRequestsPerConnection"/>
        /// </summary>
        public const string MaxInFlightRequestsPerConnectionKey = "datadistributionmanager.kafka.globalconf.max.in.flight.requests.per.connection";
        /// <summary>
        /// Configuration key of <see cref="MetadataRequestTimeout"/>
        /// </summary>
        public const string MetadataRequestTimeoutKey = "datadistributionmanager.kafka.globalconf.metadata.request.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="TopicMetadataRefreshInterval"/>
        /// </summary>
        public const string TopicMetadataRefreshIntervalKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="MetadataMaxAge"/>
        /// </summary>
        public const string MetadataMaxAgeKey = "datadistributionmanager.kafka.globalconf.metadata.max.age.ms";
        /// <summary>
        /// Configuration key of <see cref="TopicMetadataRefreshFastInterval"/>
        /// </summary>
        public const string TopicMetadataRefreshFastIntervalKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.fast.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="TopicMetadataRefreshSparse"/>
        /// </summary>
        public const string TopicMetadataRefreshSparseKey = "datadistributionmanager.kafka.globalconf.topic.metadata.refresh.sparse";
        /// <summary>
        /// Configuration key of <see cref="TopicMetadataPropagationMax"/>
        /// </summary>
        public const string TopicMetadataPropagationMaxKey = "datadistributionmanager.kafka.globalconf.topic.metadata.propagation.max.ms";
        /// <summary>
        /// Configuration key of <see cref="TopicBlackList"/>
        /// </summary>
        public const string TopicBlackListKey = "datadistributionmanager.kafka.globalconf.topic.blacklist";
        /// <summary>
        /// Configuration key of <see cref="SocketTimeout"/>
        /// </summary>
        public const string SocketTimeoutKey = "datadistributionmanager.kafka.globalconf.socket.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="SocketSendBufferBytes"/>
        /// </summary>
        public const string SocketSendBufferBytesKey = "datadistributionmanager.kafka.globalconf.socket.send.buffer.bytes";
        /// <summary>
        /// Configuration key of <see cref="SocketReceiveBufferBytes"/>
        /// </summary>
        public const string SocketReceiveBufferBytesKey = "datadistributionmanager.kafka.globalconf.socket.receive.buffer.bytes";
        /// <summary>
        /// Configuration key of <see cref="SocketKeepaliveEnable"/>
        /// </summary>
        public const string SocketKeepaliveEnableKey = "datadistributionmanager.kafka.globalconf.socket.keepalive.enable";
        /// <summary>
        /// Configuration key of <see cref="SocketNagleDisable"/>
        /// </summary>
        public const string SocketNagleDisableKey = "datadistributionmanager.kafka.globalconf.socket.nagle.disable";
        /// <summary>
        /// Configuration key of <see cref="SocketMaxFails"/>
        /// </summary>
        public const string SocketMaxFailsKey = "datadistributionmanager.kafka.globalconf.socket.max.fails";
        /// <summary>
        /// Configuration key of <see cref="BrokerAddressTtl"/>
        /// </summary>
        public const string BrokerAddressTtlKey = "datadistributionmanager.kafka.globalconf.broker.address.ttl";
        /// <summary>
        /// Configuration key of <see cref="BrokerAddressFamily"/>
        /// </summary>
        public const string BrokerAddressFamilyKey = "datadistributionmanager.kafka.globalconf.broker.address.family";
        /// <summary>
        /// Configuration key of <see cref="SocketConnectionSetupTimeout"/>
        /// </summary>
        public const string SocketConnectionSetupTimeoutKey = "datadistributionmanager.kafka.globalconf.socket.connection.setup.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="ConnectionsMaxIdle"/>
        /// </summary>
        public const string ConnectionsMaxIdleKey = "datadistributionmanager.kafka.globalconf.connections.max.idle.ms";
        /// <summary>
        /// Configuration key of <see cref="ReconnectBackoff"/>
        /// </summary>
        public const string ReconnectBackoffKey = "datadistributionmanager.kafka.globalconf.reconnect.backoff.ms";
        /// <summary>
        /// Configuration key of <see cref="ReconnectBackoffMax"/>
        /// </summary>
        public const string ReconnectBackoffMaxKey = "datadistributionmanager.kafka.globalconf.reconnect.backoff.max.ms";
        /// <summary>
        /// Configuration key of <see cref="StatisticsInterval"/>
        /// </summary>
        public const string StatisticsIntervalKey = "datadistributionmanager.kafka.globalconf.statistics.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="LogLevel"/>
        /// </summary>
        public const string LogLevelKey = "datadistributionmanager.kafka.globalconf.log_level";
        /// <summary>
        /// Configuration key of <see cref="LogQueue"/>
        /// </summary>
        public const string LogQueueKey = "datadistributionmanager.kafka.globalconf.log.queue";
        /// <summary>
        /// Configuration key of <see cref="LogThreadName"/>
        /// </summary>
        public const string LogThreadNameKey = "datadistributionmanager.kafka.globalconf.log.thread.name";
        /// <summary>
        /// Configuration key of <see cref="EnableRandomSeed"/>
        /// </summary>
        public const string EnableRandomSeedKey = "datadistributionmanager.kafka.globalconf.enable.random.seed";
        /// <summary>
        /// Configuration key of <see cref="LogConnectionClose"/>
        /// </summary>
        public const string LogConnectionCloseKey = "datadistributionmanager.kafka.globalconf.log.connection.close";
        /// <summary>
        /// Configuration key of <see cref="IntervalTerminationSignal"/>
        /// </summary>
        public const string IntervalTerminationSignalKey = "datadistributionmanager.kafka.globalconf.internal.termination.signal";
        /// <summary>
        /// Configuration key of <see cref="ApiVersionRequest"/>
        /// </summary>
        public const string ApiVersionRequestKey = "datadistributionmanager.kafka.globalconf.api.version.request";
        /// <summary>
        /// Configuration key of <see cref="ApiVersionRequestTimeout"/>
        /// </summary>
        public const string ApiVersionRequestTimeoutKey = "datadistributionmanager.kafka.globalconf.api.version.request.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="ApiVersionFallback"/>
        /// </summary>
        public const string ApiVersionFallbackKey = "datadistributionmanager.kafka.globalconf.api.version.fallback.ms";
        /// <summary>
        /// Configuration key of <see cref="BrokerVersionFallback"/>
        /// </summary>
        public const string BrokerVersionFallbackKey = "datadistributionmanager.kafka.globalconf.broker.version.fallback";
        /// <summary>
        /// Configuration key of <see cref="SecurityProtocol"/>
        /// </summary>
        public const string SecurityProtocolKey = "datadistributionmanager.kafka.globalconf.security.protocol";
        /// <summary>
        /// Configuration key of <see cref="SslCipherSuites"/>
        /// </summary>
        public const string SslCipherSuitesKey = "datadistributionmanager.kafka.globalconf.ssl.cipher.suites";
        /// <summary>
        /// Configuration key of <see cref="SslCurvesList"/>
        /// </summary>
        public const string SslCurvesListKey = "datadistributionmanager.kafka.globalconf.ssl.curves.list";
        /// <summary>
        /// Configuration key of <see cref="SslSisalgsList"/>
        /// </summary>
        public const string SslSisalgsListKey = "datadistributionmanager.kafka.globalconf.ssl.sigalgs.list";
        /// <summary>
        /// Configuration key of <see cref="SslKeyLocation"/>
        /// </summary>
        public const string SslKeyLocationKey = "datadistributionmanager.kafka.globalconf.ssl.key.location";
        /// <summary>
        /// Configuration key of <see cref="SslKeyPassword"/>
        /// </summary>
        public const string SslKeyPasswordKey = "datadistributionmanager.kafka.globalconf.ssl.key.password";
        /// <summary>
        /// Configuration key of <see cref="SslKeyPem"/>
        /// </summary>
        public const string SslKeyPemKey = "datadistributionmanager.kafka.globalconf.ssl.key.pem";
        /// <summary>
        /// Configuration key of <see cref="SslKey"/>
        /// </summary>
        public const string SslKeyKey = "datadistributionmanager.kafka.globalconf.ssl_key";
        /// <summary>
        /// Configuration key of <see cref="SslCertificateLocation"/>
        /// </summary>
        public const string SslCertificateLocationKey = "datadistributionmanager.kafka.globalconf.ssl.certificate.location";
        /// <summary>
        /// Configuration key of <see cref="SslCertificatePem"/>
        /// </summary>
        public const string SslCertificatePemKey = "datadistributionmanager.kafka.globalconf.ssl.certificate.pem";
        /// <summary>
        /// Configuration key of <see cref="SslCertificate"/>
        /// </summary>
        public const string SslCertificateKey = "datadistributionmanager.kafka.globalconf.ssl_certificate";
        /// <summary>
        /// Configuration key of <see cref="SslCALocation"/>
        /// </summary>
        public const string SslCALocationKey = "datadistributionmanager.kafka.globalconf.ssl.ca.location";
        /// <summary>
        /// Configuration key of <see cref="SslCAPem"/>
        /// </summary>
        public const string SslCAPemKey = "datadistributionmanager.kafka.globalconf.ssl.ca.pem";
        /// <summary>
        /// Configuration key of <see cref="SslCA"/>
        /// </summary>
        public const string SslCAKey = "datadistributionmanager.kafka.globalconf.ssl_ca";
        /// <summary>
        /// Configuration key of <see cref="SslCACertificateStores"/>
        /// </summary>
        public const string SslCACertificateStoresKey = "datadistributionmanager.kafka.globalconf.ssl.ca.certificate.stores";
        /// <summary>
        /// Configuration key of <see cref="SslCrlLocation"/>
        /// </summary>
        public const string SslCrlLocationKey = "datadistributionmanager.kafka.globalconf.ssl.crl.location";
        /// <summary>
        /// Configuration key of <see cref="SslKeystoreLocation"/>
        /// </summary>
        public const string SslKeystoreLocationKey = "datadistributionmanager.kafka.globalconf.ssl.keystore.location";
        /// <summary>
        /// Configuration key of <see cref="SslKeystorePassword"/>
        /// </summary>
        public const string SslKeystorePasswordKey = "datadistributionmanager.kafka.globalconf.ssl.keystore.password";
        /// <summary>
        /// Configuration key of <see cref="SslEngineLocation"/>
        /// </summary>
        public const string SslEngineLocationKey = "datadistributionmanager.kafka.globalconf.ssl.engine.location";
        /// <summary>
        /// Configuration key of <see cref="SslEngineId"/>
        /// </summary>
        public const string SslEngineIdKey = "datadistributionmanager.kafka.globalconf.ssl.engine.id";
        /// <summary>
        /// Configuration key of <see cref="EnableSslCertificateVerification"/>
        /// </summary>
        public const string EnableSslCertificateVerificationKey = "datadistributionmanager.kafka.globalconf.enable.ssl.certificate.verification";
        /// <summary>
        /// Configuration key of <see cref="SslEndpointIdentificationAlgorithm"/>
        /// </summary>
        public const string SslEndpointIdentificationAlgorithmKey = "datadistributionmanager.kafka.globalconf.ssl.endpoint.identification.algorithm";
        /// <summary>
        /// Configuration key of <see cref="SaslMechanism"/>
        /// </summary>
        public const string SaslMechanismKey = "datadistributionmanager.kafka.globalconf.sasl.mechanisms";
        /// <summary>
        /// Configuration key of <see cref="SaslKerberosServiceName"/>
        /// </summary>
        public const string SaslKerberosServiceNameKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.service.name";
        /// <summary>
        /// Configuration key of <see cref="SaslKerberosPrincipal"/>
        /// </summary>
        public const string SaslKerberosPrincipalKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.principal";
        /// <summary>
        /// Configuration key of <see cref="SaslKerberosKinitCmd"/>
        /// </summary>
        public const string SaslKerberosKinitCmdKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.kinit.cmd";
        /// <summary>
        /// Configuration key of <see cref="SaslKerberosKeytab"/>
        /// </summary>
        public const string SaslKerberosKeytabKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.keytab";
        /// <summary>
        /// Configuration key of <see cref="SaslKerberosMinTimeBeforeRelogin"/>
        /// </summary>
        public const string SaslKerberosMinTimeBeforeReloginKey = "datadistributionmanager.kafka.globalconf.sasl.kerberos.min.time.before.relogin";
        /// <summary>
        /// Configuration key of <see cref="SaslUsername"/>
        /// </summary>
        public const string SaslUsernameKey = "datadistributionmanager.kafka.globalconf.sasl.username";
        /// <summary>
        /// Configuration key of <see cref="SaslPassword"/>
        /// </summary>
        public const string SaslPasswordKey = "datadistributionmanager.kafka.globalconf.sasl.password";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerConfig"/>
        /// </summary>
        public const string SaslOauthbearerConfigKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.config";
        /// <summary>
        /// Configuration key of <see cref="EnableSaslOauthbearerUnsecureJwt"/>
        /// </summary>
        public const string EnableSaslOauthbearerUnsecureJwtKey = "datadistributionmanager.kafka.globalconf.enable.sasl.oauthbearer.unsecure.jwt";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerMethod"/>
        /// </summary>
        public const string SaslOauthbearerMethodKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.method";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerClientId"/>
        /// </summary>
        public const string SaslOauthbearerClientIdKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.client.id";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerClientSecret"/>
        /// </summary>
        public const string SaslOauthbearerClientSecretKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.client.secret";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerScope"/>
        /// </summary>
        public const string SaslOauthbearerScopeKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.scope";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerExtensions"/>
        /// </summary>
        public const string SaslOauthbearerExtensionsKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.extensions";
        /// <summary>
        /// Configuration key of <see cref="SaslOauthbearerTokenEndpointUrl"/>
        /// </summary>
        public const string SaslOauthbearerTokenEndpointUrlKey = "datadistributionmanager.kafka.globalconf.sasl.oauthbearer.token.endpoint.url";
        /// <summary>
        /// Configuration key of <see cref="PluginLibraryPaths"/>
        /// </summary>
        public const string PluginLibraryPathsKey = "datadistributionmanager.kafka.globalconf.plugin.library.paths";
        /// <summary>
        /// Configuration key of <see cref="GroupInstanceId"/>
        /// </summary>
        public const string GroupInstanceIdKey = "datadistributionmanager.kafka.globalconf.group.instance.id";
        /// <summary>
        /// Configuration key of <see cref="PartitionAssignmentStrategy"/>
        /// </summary>
        public const string PartitionAssignmentStrategyKey = "datadistributionmanager.kafka.globalconf.partition.assignment.strategy";
        /// <summary>
        /// Configuration key of <see cref="SessionTimeout"/>
        /// </summary>
        public const string SessionTimeoutKey = "datadistributionmanager.kafka.globalconf.session.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="HeartbeatInterval"/>
        /// </summary>
        public const string HeartbeatIntervalKey = "datadistributionmanager.kafka.globalconf.heartbeat.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="GroupProtocolType"/>
        /// </summary>
        public const string GroupProtocolTypeKey = "datadistributionmanager.kafka.globalconf.group.protocol.type";
        /// <summary>
        /// Configuration key of <see cref="CoordinatorQueryInterval"/>
        /// </summary>
        public const string CoordinatorQueryIntervalKey = "datadistributionmanager.kafka.globalconf.coordinator.query.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="MaxPollInterval"/>
        /// </summary>
        public const string MaxPollIntervalKey = "datadistributionmanager.kafka.globalconf.max.poll.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="EnableAutoCommit"/>
        /// </summary>
        public const string EnableAutoCommitKey = "datadistributionmanager.kafka.globalconf.enable.auto.commit";
        /// <summary>
        /// Configuration key of <see cref="AutoCommitInterval"/>
        /// </summary>
        public const string AutoCommitIntervalKey = "datadistributionmanager.kafka.globalconf.auto.commit.interval.ms";
        /// <summary>
        /// Configuration key of <see cref="EnableAutoOffsetStore"/>
        /// </summary>
        public const string EnableAutoOffsetStoreKey = "datadistributionmanager.kafka.globalconf.enable.auto.offset.store";
        /// <summary>
        /// Configuration key of <see cref="QueuedMinMessages"/>
        /// </summary>
        public const string QueuedMinMessagesKey = "datadistributionmanager.kafka.globalconf.queued.min.messages";
        /// <summary>
        /// Configuration key of <see cref="QueuedMaxMessagesKb"/>
        /// </summary>
        public const string QueuedMaxMessagesKbKey = "datadistributionmanager.kafka.globalconf.queued.max.messages.kbytes";
        /// <summary>
        /// Configuration key of <see cref="FetchWaitMax"/>
        /// </summary>
        public const string FetchWaitMaxKey = "datadistributionmanager.kafka.globalconf.fetch.wait.max.ms";
        /// <summary>
        /// Configuration key of <see cref="FetchMessageMaxBytes"/>
        /// </summary>
        public const string FetchMessageMaxBytesKey = "datadistributionmanager.kafka.globalconf.fetch.message.max.bytes";
        /// <summary>
        /// Configuration key of <see cref="FetchMaxBytes"/>
        /// </summary>
        public const string FetchMaxBytesKey = "datadistributionmanager.kafka.globalconf.fetch.max.bytes";
        /// <summary>
        /// Configuration key of <see cref="FetchMinBytes"/>
        /// </summary>
        public const string FetchMinBytesKey = "datadistributionmanager.kafka.globalconf.fetch.min.bytes";
        /// <summary>
        /// Configuration key of <see cref="FetchErrorBackoff"/>
        /// </summary>
        public const string FetchErrorBackoffKey = "datadistributionmanager.kafka.globalconf.fetch.error.backoff.ms";
        /// <summary>
        /// Configuration key of <see cref="IsolationLevel"/>
        /// </summary>
        public const string IsolationLevelKey = "datadistributionmanager.kafka.globalconf.isolation.level";
        /// <summary>
        /// Configuration key of <see cref="EnablePartitionEof"/>
        /// </summary>
        public const string EnablePartitionEofKey = "datadistributionmanager.kafka.globalconf.enable.partition.eof";
        /// <summary>
        /// Configuration key of <see cref="CheckCrcs"/>
        /// </summary>
        public const string CheckCrcsKey = "datadistributionmanager.kafka.globalconf.check.crcs";
        /// <summary>
        /// Configuration key of <see cref="AllowAutoCreateTopics"/>
        /// </summary>
        public const string AllowAutoCreateTopicsKey = "datadistributionmanager.kafka.globalconf.allow.auto.create.topics";
        /// <summary>
        /// Configuration key of <see cref="ClientRack"/>
        /// </summary>
        public const string ClientRackKey = "datadistributionmanager.kafka.globalconf.client.rack";
        /// <summary>
        /// Configuration key of <see cref="TransactionalId"/>
        /// </summary>
        public const string TransactionalIdKey = "datadistributionmanager.kafka.globalconf.transactional.id";
        /// <summary>
        /// Configuration key of <see cref="TransactionTimeout"/>
        /// </summary>
        public const string TransactionTimeoutKey = "datadistributionmanager.kafka.globalconf.transaction.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="EnableIdempotence"/>
        /// </summary>
        public const string EnableIdempotenceKey = "datadistributionmanager.kafka.globalconf.enable.idempotence";
        /// <summary>
        /// Configuration key of <see cref="EnableGaplessGuarantee"/>
        /// </summary>
        public const string EnableGaplessGuaranteeKey = "datadistributionmanager.kafka.globalconf.enable.gapless.guarantee";
        /// <summary>
        /// Configuration key of <see cref="QueueBufferingMaxMessages"/>
        /// </summary>
        public const string QueueBufferingMaxMessagesKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.messages";
        /// <summary>
        /// Configuration key of <see cref="QueueBufferingMaxKb"/>
        /// </summary>
        public const string QueueBufferingMaxKbKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.kbytes";
        /// <summary>
        /// Configuration key of <see cref="QueueBufferingMaxMs"/>
        /// </summary>
        public const string QueueBufferingMaxMsKey = "datadistributionmanager.kafka.globalconf.queue.buffering.max.ms";
        /// <summary>
        /// Configuration key of <see cref="MessageSendMaxRetries"/>
        /// </summary>
        public const string MessageSendMaxRetriesKey = "datadistributionmanager.kafka.globalconf.message.send.max.retries";
        /// <summary>
        /// Configuration key of <see cref="RetryBackoff"/>
        /// </summary>
        public const string RetryBackoffKey = "datadistributionmanager.kafka.globalconf.retry.backoff.ms";
        /// <summary>
        /// Configuration key of <see cref="QueueBufferingBackpressureThreshold"/>
        /// </summary>
        public const string QueueBufferingBackpressureThresholdKey = "datadistributionmanager.kafka.globalconf.queue.buffering.backpressure.threshold";
        /// <summary>
        /// Configuration key of <see cref="CompressionCodec"/>
        /// </summary>
        public const string CompressionCodecKey = "datadistributionmanager.kafka.globalconf.compression.codec";
        /// <summary>
        /// Configuration key of <see cref="BatchNumMessages"/>
        /// </summary>
        public const string BatchNumMessagesKey = "datadistributionmanager.kafka.globalconf.batch.num.messages";
        /// <summary>
        /// Configuration key of <see cref="BatchSizeKey"/>
        /// </summary>
        public const string BatchSizeKey = "datadistributionmanager.kafka.globalconf.batch.size";
        /// <summary>
        /// Configuration key of <see cref="DeliveryReportOnlyError"/>
        /// </summary>
        public const string DeliveryReportOnlyErrorKey = "datadistributionmanager.kafka.globalconf.delivery.report.only.error";
        /// <summary>
        /// Configuration key of <see cref="StickyPartitioningLinger"/>
        /// </summary>
        public const string StickyPartitioningLingerKey = "datadistributionmanager.kafka.globalconf.sticky.partitioning.linger.ms";

        #endregion

        #region Topic conf
        /// <summary>
        /// Configuration key of <see cref="RequestRequiredAcks"/>
        /// </summary>
        public const string RequestRequiredAcksKey = "datadistributionmanager.kafka.topicconf.request.required.acks";
        /// <summary>
        /// Configuration key of <see cref="RequestTimeout"/>
        /// </summary>
        public const string RequestTimeoutKey = "datadistributionmanager.kafka.topicconf.request.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="MessageTimeout"/>
        /// </summary>
        public const string MessageTimeoutKey = "datadistributionmanager.kafka.topicconf.message.timeout.ms";
        /// <summary>
        /// Configuration key of <see cref="QueueingStrategy"/>
        /// </summary>
        public const string QueueingStrategyKey = "datadistributionmanager.kafka.topicconf.queuing.strategy";
        /// <summary>
        /// Configuration key of <see cref="Partitioner"/>
        /// </summary>
        public const string PartitionerKey = "datadistributionmanager.kafka.topicconf.partitioner";
        /// <summary>
        /// Configuration key of <see cref="TopicCompressionCodec"/>
        /// </summary>
        public const string TopicCompressionCodecKey = "datadistributionmanager.kafka.topicconf.compression.codec";
        /// <summary>
        /// Configuration key of <see cref="CompressionLevel"/>
        /// </summary>
        public const string CompressionLevelKey = "datadistributionmanager.kafka.topicconf.compression.level";
        /// <summary>
        /// Configuration key of <see cref="AutoOffsetReset"/>
        /// </summary>
        public const string AutoOffsetResetKey = "datadistributionmanager.kafka.topicconf.auto.offset.reset";
        /// <summary>
        /// Configuration key of <see cref="ConsumeCallbackMaxMessages"/>
        /// </summary>
        public const string ConsumeCallbackMaxMessagesKey = "datadistributionmanager.kafka.topicconf.consume.callback.max.messages";


        #endregion

        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        public KafkaChannelConfiguration(KafkaConfiguration originalConf)
            : base(originalConf)
        {

        }

        #region Module conf

        /// <summary>
        /// The replication factor to be used
        /// </summary>
        public uint ReplicationFactor
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReplicationFactorKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ReplicationFactorKey] = value.ToString();
                EmitPropertyChanged("ReplicationFactor");
            }
        }

        /// <summary>
        /// True to request a create topic
        /// </summary>
        public bool TopicCreate
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicCreateKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[TopicCreateKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("TopicCreate");
            }
        }

        /// <summary>
        /// True to dump metadata
        /// </summary>
        public bool DumpMetadata
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DumpMetadataKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[DumpMetadataKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("DumpMetadata");
            }
        }

        /// <summary>
        /// True to enable transactions
        /// </summary>
        public bool TransactionsEnabled
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TransactionsEnabledKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[TransactionsEnabledKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("TransactionsEnabled");
            }
        }

        /// <summary>
        /// The timeout to be used in transaction operations when <see cref="TransactionsEnabled"/> is true
        /// </summary>
        public uint TransactionsTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TransactionsTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[TransactionsTimeoutKey] = value.ToString();
                EmitPropertyChanged("TransactionsTimeout");
            }
        }

        #endregion

        #region Global conf
        /// <summary>
        /// The client id to be used
        /// </summary>
        public string ClientId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ClientIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[ClientIdKey] = value;
                EmitPropertyChanged("ClientId");
            }
        }

        /// <summary>
        /// The group id to be used
        /// </summary>
        public string GroupId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(GroupIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[GroupIdKey] = value;
                EmitPropertyChanged("GroupId");
            }
        }

        /// <summary>
        /// A comma-separated list of debug contexts to enable. Detailed Producer debugging: broker,topic,msg. Consumer: consumer,cgrp,topic,fetch
        /// </summary>
        public DebugType Debug
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DebugKey, out value);
                return (DebugType)Enum.Parse(typeof(DebugType), value);
            }
            set
            {
                keyValuePair[DebugKey] = Enum.Format(typeof(BuiltinFeaturesType), value, "g");
                EmitPropertyChanged("Debug");
            }
        }

        /// <summary>
        /// The bootstrap broker(s) to be used as Comma Separated Value(s)
        /// </summary>
        public string BootstrapBrokers
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BootstrapBrokersKey, out value);
                return value;
            }
            set
            {
                keyValuePair[BootstrapBrokersKey] = value;
                EmitPropertyChanged("BootstrapBrokers");
            }
        }



        /// <summary>
        /// Indicates the builtin features for this build of librdkafka. An application can either query this value or attempt to set it with its list of required features to check for library support.
        /// </summary>
        public BuiltinFeaturesType BuiltinFeatures
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BuiltinFeaturesKey, out value);
                return (BuiltinFeaturesType)Enum.Parse(typeof(BuiltinFeaturesType), value);
            }
            set
            {
                keyValuePair[BuiltinFeaturesKey] = Enum.Format(typeof(BuiltinFeaturesType), value, "g");
                EmitPropertyChanged("BuiltinFeatures");
            }
        }

        /// <summary>
        /// Maximum Kafka protocol request message size. Due to differing framing overhead between protocol versions the producer is unable to reliably enforce a strict max message limit at produce time and may exceed the maximum size by one message in protocol ProduceRequests, the broker will enforce the the topic's `max.message.bytes` limit (see Apache Kafka documentation).
        /// </summary>
        /// <value>1000 .. 1000000000</value>
        public uint MessageMaxBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MessageMaxBytesKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[MessageMaxBytesKey] = value.ToString();
                EmitPropertyChanged("MessageMaxBytes");
            }
        }

        /// <summary>
        /// Maximum size for message to be copied to buffer. Messages larger than this will be passed by reference (zero-copy) at the expense of larger iovecs. 
        /// </summary>
        /// <value>0 .. 1000000000</value>
        public uint MessageCopyMaxBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MessageCopyMaxBytesKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[MessageCopyMaxBytesKey] = value.ToString();
                EmitPropertyChanged("MessageCopyMaxBytes");
            }
        }

        /// <summary>
        /// Maximum Kafka protocol response message size. This serves as a safety precaution to avoid memory exhaustion in case of protocol hickups. This value must be at least <see cref="FetchMaxBytes"/>  + 512 to allow for protocol overhead; the value is adjusted automatically unless the configuration property is explicitly set.
        /// </summary>
        /// <value>1000 .. 2147483647</value>
        public uint ReceiveMessageMaxBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReceiveMessageMaxBytesKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[ReceiveMessageMaxBytesKey] = value.ToString();
                EmitPropertyChanged("ReceiveMessageMaxBytes");
            }
        }

        /// <summary>
        /// Maximum number of in-flight requests per broker connection. This is a generic property applied to all broker communication, however it is primarily relevant to produce requests. In particular, note that other mechanisms limit the number of outstanding consumer fetch request per broker to one.
        /// </summary>
        /// <value>1 .. 1000000</value>
        public uint MaxInFlightRequestsPerConnection
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxInFlightRequestsPerConnectionKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[MaxInFlightRequestsPerConnectionKey] = value.ToString();
                EmitPropertyChanged("MaxInFlightRequestsPerConnection");
            }
        }

        /// <summary>
        /// Non-topic request timeout in milliseconds. This is for metadata requests, etc.
        /// </summary>
        /// <value>10 .. 900000</value>
        public uint MetadataRequestTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MetadataRequestTimeoutKey, out value);
                return uint.Parse(value);
            }
            set
            {
                keyValuePair[MetadataRequestTimeoutKey] = value.ToString();
                EmitPropertyChanged("MetadataRequestTimeout");
            }
        }

        /// <summary>
        /// Period of time in milliseconds at which topic and broker metadata is refreshed in order to proactively discover any new brokers, topics, partitions or partition leader changes. Use -1 to disable the intervalled refresh (not recommended). If there are no locally referenced topics (no topic objects created, no messages produced, no subscription or no assignment) then only the broker list will be refreshed every interval but no more often than every 10s.
        /// </summary>
        /// <value>-1 .. 3600000</value>
        public int TopicMetadataRefreshInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicMetadataRefreshIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[TopicMetadataRefreshIntervalKey] = value.ToString();
                EmitPropertyChanged("TopicMetadataRefreshInterval");
            }
        }

        /// <summary>
        /// Metadata cache max age. Defaults to <see cref="TopicMetadataRefreshInterval"/> * 3
        /// </summary>
        /// <value>1 .. 86400000</value>
        public int MetadataMaxAge
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MetadataMaxAgeKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MetadataMaxAgeKey] = value.ToString();
                EmitPropertyChanged("MetadataMaxAge");
            }
        }

        /// <summary>
        /// When a topic loses its leader a new metadata request will be enqueued with this initial interval, exponentially increasing until the topic metadata has been refreshed. This is used to recover quickly from transitioning leader brokers.
        /// </summary>
        /// <value>1 .. 60000</value>
        public int TopicMetadataRefreshFastInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicMetadataRefreshFastIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[TopicMetadataRefreshFastIntervalKey] = value.ToString();
                EmitPropertyChanged("TopicMetadataRefreshFastInterval");
            }
        }

        /// <summary>
        /// Sparse metadata requests (consumes less network bandwidth)
        /// </summary>
        public bool TopicMetadataRefreshSparse
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicMetadataRefreshSparseKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[TopicMetadataRefreshFastIntervalKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("TopicMetadataRefreshSparse");
            }
        }

        /// <summary>
        /// Apache Kafka topic creation is asynchronous and it takes some time for a new topic to propagate throughout the cluster to all brokers. If a client requests topic metadata after manual topic creation but before the topic has been fully propagated to the broker the client is requesting metadata from, the topic will seem to be non-existent and the client will mark the topic as such, failing queued produced messages with `ERR__UNKNOWN_TOPIC`. This setting delays marking a topic as non-existent until the configured propagation max time has passed. The maximum propagation time is calculated from the time the topic is first referenced in the client, e.g., on produce().
        /// </summary>
        /// <value>0 .. 3600000</value>
        public int TopicMetadataPropagationMax
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicMetadataPropagationMaxKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[TopicMetadataPropagationMaxKey] = value.ToString();
                EmitPropertyChanged("TopicMetadataPropagationMax");
            }
        }

        /// <summary>
        /// Topic blacklist, a comma-separated list of regular expressions for matching topic names that should be ignored in broker metadata information as if the topics did not exist.
        /// </summary>
        public IEnumerable<string> TopicBlackList
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicBlackListKey, out value);
                return value.Split(',');
            }
            set
            {
                keyValuePair[TopicBlackListKey] = string.Join(",", value);
                EmitPropertyChanged("TopicBlackList");
            }
        }

        /// <summary>
        /// Default timeout for network requests. Producer: ProduceRequests will use the lesser value of <see cref="SocketTimeout"/> and remaining <see cref="MessageTimeout"/> for the first message in the batch. Consumer: FetchRequests will use <see cref="FetchWaitMax"/> + <see cref="SocketTimeout"/>. Admin: Admin requests will use <see cref="SocketTimeout"/> or explicitly set `rd_kafka_AdminOptions_set_operation_timeout()` value.
        /// </summary>
        /// <value>10 .. 300000</value>
        public int SocketTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SocketTimeoutKey] = value.ToString();
                EmitPropertyChanged("SocketTimeout");
            }
        }

        /// <summary>
        /// Broker socket send buffer size. System default is used if 0.
        /// </summary>
        /// <value>0 .. 100000000</value>
        public int SocketSendBufferBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketSendBufferBytesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SocketSendBufferBytesKey] = value.ToString();
                EmitPropertyChanged("SocketSendBufferBytes");
            }
        }

        /// <summary>
        /// Broker socket receive buffer size. System default is used if 0.
        /// </summary>
        /// <value>0 .. 100000000</value>
        public int SocketReceiveBufferBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketReceiveBufferBytesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SocketReceiveBufferBytesKey] = value.ToString();
                EmitPropertyChanged("SocketReceiveBufferBytes");
            }
        }

        /// <summary>
        /// Enable TCP keep-alives (SO_KEEPALIVE) on broker sockets
        /// </summary>
        public bool SocketKeepaliveEnable
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketKeepaliveEnableKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[SocketKeepaliveEnableKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("SocketKeepaliveEnable");
            }
        }

        /// <summary>
        /// Disable the Nagle algorithm (TCP_NODELAY) on broker sockets.
        /// </summary>
        public bool SocketNagleDisable
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketNagleDisableKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[SocketNagleDisableKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("SocketNagleDisable");
            }
        }

        /// <summary>
        /// Disconnect from broker when this number of send failures (e.g., timed out requests) is reached. Disable with 0. WARNING: It is highly recommended to leave this setting at its default value of 1 to avoid the client and broker to become desynchronized in case of request timeouts. NOTE: The connection is automatically re-established.
        /// </summary>
        /// <value>0 .. 100000000</value>
        public int SocketMaxFails
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketMaxFailsKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SocketMaxFailsKey] = value.ToString();
                EmitPropertyChanged("SocketMaxFails");
            }
        }

        /// <summary>
        /// How long to cache the broker address resolving results (milliseconds).
        /// </summary>
        /// <value>0 .. 86400000</value>
        public int BrokerAddressTtl
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BrokerAddressTtlKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[BrokerAddressTtlKey] = value.ToString();
                EmitPropertyChanged("BrokerAddressTtl");
            }
        }

        /// <summary>
        /// Maximum time allowed for broker connection setup (TCP connection setup as well SSL and SASL handshake). If the connection to the broker is not fully functional after this the connection will be closed and retried.
        /// </summary>
        /// <value>1000 .. 2147483647</value>
        public int SocketConnectionSetupTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SocketConnectionSetupTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SocketConnectionSetupTimeoutKey] = value.ToString();
                EmitPropertyChanged("SocketConnectionSetupTimeout");
            }
        }

        /// <summary>
        /// Close broker connections after the specified time of inactivity. Disable with 0. If this property is left at its default value some heuristics are performed to determine a suitable default value, this is currently limited to identifying brokers on Azure (see librdkafka issue #3109 for more info).
        /// </summary>
        /// <value>0 .. 2147483647</value>
        public int ConnectionsMaxIdle
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ConnectionsMaxIdleKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ConnectionsMaxIdleKey] = value.ToString();
                EmitPropertyChanged("ConnectionsMaxIdle");
            }
        }

        /// <summary>
        /// Allowed broker IP address families: any, v4, v6
        /// </summary>
        public BrokerAddressFamilyType BrokerAddressFamily
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BrokerAddressFamilyKey, out value);
                return (BrokerAddressFamilyType)Enum.Parse(typeof(BrokerAddressFamilyType), value);
            }
            set
            {
                keyValuePair[BrokerAddressFamilyKey] = Enum.Format(typeof(BrokerAddressFamilyType), value, "g");
                EmitPropertyChanged("BrokerAddressFamily");
            }
        }

        /// <summary>
        /// The initial time to wait before reconnecting to a broker after the connection has been closed. The time is increased exponentially until <see cref="ReconnectBackoffMax"/> is reached. -25% to +50% jitter is applied to each reconnect backoff. A value of 0 disables the backoff and reconnects immediately.
        /// </summary>
        /// <value>0 .. 3600000</value>
        public int ReconnectBackoff
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReconnectBackoffKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ReconnectBackoffKey] = value.ToString();
                EmitPropertyChanged("ReconnectBackoff");
            }
        }

        /// <summary>
        /// The maximum time to wait before reconnecting to a broker after the connection has been closed.
        /// </summary>
        /// <value>0 .. 3600000</value>
        public int ReconnectBackoffMax
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ReconnectBackoffMaxKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ReconnectBackoffMaxKey] = value.ToString();
                EmitPropertyChanged("ReconnectBackoffMax");
            }
        }

        /// <summary>
        /// librdkafka statistics emit interval. The application also needs to register a stats callback using `rd_kafka_conf_set_stats_cb()`. The granularity is 1000ms. A value of 0 disables statistics.
        /// </summary>
        /// <value>0 .. 86400000</value>
        public int StatisticsInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(StatisticsIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[StatisticsIntervalKey] = value.ToString();
                EmitPropertyChanged("StatisticsInterval");
            }
        }

        /// <summary>
        /// Logging level (syslog(3) levels)
        /// </summary>
        /// <value>0 .. 7</value>
        public int LogLevel
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(LogLevelKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[LogLevelKey] = value.ToString();
                EmitPropertyChanged("LogLevel");
            }
        }

        /// <summary>
        /// Disable spontaneous log_cb from internal librdkafka threads, instead enqueue log messages on queue set with `rd_kafka_set_log_queue()` and serve log callbacks or events through the standard poll APIs. **NOTE**: Log messages will linger in a temporary queue until the log queue has been set.
        /// </summary>
        public bool LogQueue
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(LogQueueKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[LogQueueKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("LogQueue");
            }
        }

        /// <summary>
        /// Print internal thread name in log messages (useful for debugging librdkafka internals)
        /// </summary>
        public bool LogThreadName
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(LogThreadNameKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[LogThreadNameKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("LogThreadName");
            }
        }

        /// <summary>
        /// If enabled librdkafka will initialize the PRNG with srand(current_time.milliseconds) on the first invocation of rd_kafka_new() (required only if rand_r() is not available on your platform). If disabled the application must call srand() prior to calling rd_kafka_new().
        /// </summary>
        public bool EnableRandomSeed
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableRandomSeedKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableRandomSeedKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableRandomSeed");
            }
        }

        /// <summary>
        /// Log broker disconnects. It might be useful to turn this off when interacting with 0.9 brokers with an aggressive `connection.max.idle.ms` value.
        /// </summary>
        public bool LogConnectionClose
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(LogConnectionCloseKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[LogConnectionCloseKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("LogConnectionClose");
            }
        }

        /// <summary>
        /// Signal that librdkafka will use to quickly terminate on rd_kafka_destroy(). If this signal is not set then there will be a delay before rd_kafka_wait_destroyed() returns true as internal threads are timing out their system calls. If this signal is set however the delay will be minimal. The application should mask this signal as an internal signal handler is installed.
        /// </summary>
        /// <value>0 .. 128</value>
        public int IntervalTerminationSignal
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(IntervalTerminationSignalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[IntervalTerminationSignalKey] = value.ToString();
                EmitPropertyChanged("IntervalTerminationSignal");
            }
        }

        /// <summary>
        /// Request broker's supported API versions to adjust functionality to available protocol features. If set to false, or the ApiVersionRequest fails, the fallback version `broker.version.fallback` will be used. **NOTE**: Depends on broker version >=0.10.0. If the request is not supported by (an older) broker the `broker.version.fallback` fallback is used.
        /// </summary>
        public bool ApiVersionRequest
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ApiVersionRequestKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[ApiVersionRequestKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("ApiVersionRequest");
            }
        }

        /// <summary>
        /// Timeout for broker API version requests.
        /// </summary>
        /// <value>1 .. 300000</value>
        public int ApiVersionRequestTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ApiVersionRequestTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ApiVersionRequestTimeoutKey] = value.ToString();
                EmitPropertyChanged("ApiVersionRequestTimeout");
            }
        }

        /// <summary>
        /// Dictates how long the `broker.version.fallback` fallback is used in the case the ApiVersionRequest fails. **NOTE**: The ApiVersionRequest is only issued when a new connection to the broker is made (such as after an upgrade).
        /// </summary>
        /// <value>0 .. 604800000</value>
        public int ApiVersionFallback
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ApiVersionFallbackKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ApiVersionFallbackKey] = value.ToString();
                EmitPropertyChanged("ApiVersionFallback");
            }
        }

        /// <summary>
        /// Older broker versions (before 0.10.0) provide no way for a client to query for supported protocol features (ApiVersionRequest, see `api.version.request`) making it impossible for the client to know what features it may use. As a workaround a user may set this property to the expected broker version and the client will automatically adjust its feature set accordingly if the ApiVersionRequest fails (or is disabled). The fallback broker version will be used for `api.version.fallback.ms`. Valid values are: 0.9.0, 0.8.2, 0.8.1, 0.8.0. Any other value >= 0.10, such as 0.10.2.1, enables ApiVersionRequests.
        /// </summary>
        public string BrokerVersionFallback
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BrokerVersionFallbackKey, out value);
                return value;
            }
            set
            {
                keyValuePair[BrokerVersionFallbackKey] = value;
                EmitPropertyChanged("BrokerVersionFallback");
            }
        }

        /// <summary>
        /// Protocol used to communicate with brokers.
        /// </summary>
        public SecurityProtocolType SecurityProtocol
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SecurityProtocolKey, out value);
                return (SecurityProtocolType)Enum.Parse(typeof(SecurityProtocolType), value);
            }
            set
            {
                keyValuePair[SecurityProtocolKey] = Enum.Format(typeof(SecurityProtocolType), value, "g");
                EmitPropertyChanged("SecurityProtocol");
            }
        }

        /// <summary>
        /// A cipher suite is a named combination of authentication, encryption, MAC and key exchange algorithm used to negotiate the security settings for a network connection using TLS or SSL network protocol. See manual page for `ciphers(1)` and `SSL_CTX_set_cipher_list(3).
        /// </summary>
        public string SslCipherSuites
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCipherSuitesKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCipherSuitesKey] = value;
                EmitPropertyChanged("SslCipherSuites");
            }
        }

        /// <summary>
        /// The supported-curves extension in the TLS ClientHello message specifies the curves (standard/named, or 'explicit' GF(2^k) or GF(p)) the client is willing to have the server use. See manual page for `SSL_CTX_set1_curves_list(3)`. OpenSSL >= 1.0.2 required.
        /// </summary>
        public string SslCurvesList
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCurvesListKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCurvesListKey] = value;
                EmitPropertyChanged("SslCurvesList");
            }
        }

        /// <summary>
        /// The client uses the TLS ClientHello signature_algorithms extension to indicate to the server which signature/hash algorithm pairs may be used in digital signatures. See manual page for `SSL_CTX_set1_sigalgs_list(3)`. OpenSSL >= 1.0.2 required.
        /// </summary>
        public string SslSisalgsList
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslSisalgsListKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslSisalgsListKey] = value;
                EmitPropertyChanged("SslSisalgsList");
            }
        }

        /// <summary>
        /// Path to client's private key (PEM) used for authentication.
        /// </summary>
        public string SslKeyLocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeyLocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeyLocationKey] = value;
                EmitPropertyChanged("SslKeyLocation");
            }
        }

        /// <summary>
        /// Private key passphrase (for use with `ssl.key.location` and `set_ssl_cert()`)
        /// </summary>
        public string SslKeyPassword
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeyPasswordKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeyPasswordKey] = value;
                EmitPropertyChanged("SslKeyPassword");
            }
        }

        /// <summary>
        /// Client's private key string (PEM format) used for authentication.
        /// </summary>
        public string SslKeyPem
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeyPemKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeyPemKey] = value;
                EmitPropertyChanged("SslKeyPem");
            }
        }

        /// <summary>
        /// Client's private key as set by rd_kafka_conf_set_ssl_cert()
        /// </summary>
        public string SslKey
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeyKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeyKey] = value;
                EmitPropertyChanged("SslKey");
            }
        }

        /// <summary>
        /// Path to client's public key (PEM) used for authentication.
        /// </summary>
        public string SslCertificateLocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCertificateLocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCertificateLocationKey] = value;
                EmitPropertyChanged("SslCertificateLocation");
            }
        }

        /// <summary>
        /// Client's public key string (PEM format) used for authentication.
        /// </summary>
        public string SslCertificatePem
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCertificatePemKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCertificatePemKey] = value;
                EmitPropertyChanged("SslCertificatePem");
            }
        }

        /// <summary>
        /// Client's public key as set by rd_kafka_conf_set_ssl_cert()
        /// </summary>
        public string SslCertificate
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCertificateKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCertificateKey] = value;
                EmitPropertyChanged("SslCertificate");
            }
        }

        /// <summary>
        /// File or directory path to CA certificate(s) for verifying the broker's key. Defaults: On Windows the system's CA certificates are automatically looked up in the Windows Root certificate store. On Mac OSX this configuration defaults to `probe`. It is recommended to install openssl using Homebrew, to provide CA certificates. On Linux install the distribution's ca-certificates package. If OpenSSL is statically linked or `ssl.ca.location` is set to `probe` a list of standard paths will be probed and the first one found will be used as the default CA certificate location path. If OpenSSL is dynamically linked the OpenSSL library's default path will be used (see `OPENSSLDIR` in `openssl version -a`).
        /// </summary>
        public string SslCALocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCALocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCALocationKey] = value;
                EmitPropertyChanged("SslCALocation");
            }
        }

        /// <summary>
        /// CA certificate string (PEM format) for verifying the broker's key.
        /// </summary>
        public string SslCAPem
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCAPemKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCAPemKey] = value;
                EmitPropertyChanged("SslCAPem");
            }
        }

        /// <summary>
        /// CA certificate as set by rd_kafka_conf_set_ssl_cert()
        /// </summary>
        public string SslCA
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCAKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCAKey] = value;
                EmitPropertyChanged("SslCA");
            }
        }

        /// <summary>
        /// Comma-separated list of Windows Certificate stores to load CA certificates from. Certificates will be loaded in the same order as stores are specified. If no certificates can be loaded from any of the specified stores an error is logged and the OpenSSL library's default CA location is used instead. Store names are typically one or more of: MY, Root, Trust, CA.
        /// </summary>
        public IEnumerable<string> SslCACertificateStores
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCACertificateStoresKey, out value);
                return value.Split(',');
            }
            set
            {
                keyValuePair[SslCACertificateStoresKey] = string.Join(",", value);
                EmitPropertyChanged("SslCACertificateStores");
            }
        }

        /// <summary>
        /// Path to CRL for verifying broker's certificate validity.
        /// </summary>
        public string SslCrlLocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslCrlLocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslCrlLocationKey] = value;
                EmitPropertyChanged("SslCrlLocation");
            }
        }

        /// <summary>
        /// Path to client's keystore (PKCS#12) used for authentication.
        /// </summary>
        public string SslKeystoreLocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeystoreLocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeystoreLocationKey] = value;
                EmitPropertyChanged("SslKeystoreLocation");
            }
        }

        /// <summary>
        /// Client's keystore (PKCS#12) password.
        /// </summary>
        public string SslKeystorePassword
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslKeystorePasswordKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslKeystorePasswordKey] = value;
                EmitPropertyChanged("SslKeystorePassword");
            }
        }

        /// <summary>
        /// Path to OpenSSL engine library. OpenSSL >= 1.1.0 required.
        /// </summary>
        public string SslEngineLocation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslEngineLocationKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslEngineLocationKey] = value;
                EmitPropertyChanged("SslEngineLocation");
            }
        }

        /// <summary>
        /// OpenSSL engine id is the name used for loading engine.
        /// </summary>
        public string SslEngineId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslEngineIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SslEngineIdKey] = value;
                EmitPropertyChanged("SslEngineId");
            }
        }

        /// <summary>
        /// Enable OpenSSL's builtin broker (server) certificate verification. This verification can be extended by the application by implementing a certificate_verify_cb.
        /// </summary>
        public bool EnableSslCertificateVerification
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableSslCertificateVerificationKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableSslCertificateVerificationKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableSslCertificateVerification");
            }
        }

        /// <summary>
        /// Endpoint identification algorithm to validate broker hostname using broker certificate. <see cref="SslEndpointIdentificationAlgorithmType.https"/> - Server (broker) hostname verification as specified in RFC2818. <see cref="SslEndpointIdentificationAlgorithmType.none"/> - No endpoint verification. OpenSSL >= 1.0.2 required.
        /// </summary>
        public SslEndpointIdentificationAlgorithmType SslEndpointIdentificationAlgorithm
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SslEndpointIdentificationAlgorithmKey, out value);
                return (SslEndpointIdentificationAlgorithmType)Enum.Parse(typeof(SslEndpointIdentificationAlgorithmType), value);
            }
            set
            {
                keyValuePair[SslEndpointIdentificationAlgorithmKey] = Enum.Format(typeof(SslEndpointIdentificationAlgorithmType), value, "g");
                EmitPropertyChanged("SslEndpointIdentificationAlgorithm");
            }
        }

        /// <summary>
        /// SASL mechanism to use for authentication. Supported <see cref="SaslMechanismType"/>
        /// </summary>
        public SaslMechanismType SaslMechanism
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslMechanismKey, out value);
                value = value.Replace('-', '_');
                return (SaslMechanismType)Enum.Parse(typeof(SaslMechanismType), value);
            }
            set
            {
                keyValuePair[SaslMechanismKey] = Enum.Format(typeof(SaslMechanismType), value, "g").Replace('_', '-'); ;
                EmitPropertyChanged("SaslMechanism");
            }
        }

        /// <summary>
        /// Kerberos principal name that Kafka runs as, not including /hostname@REALM
        /// </summary>
        public string SaslKerberosServiceName
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslKerberosServiceNameKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslKerberosServiceNameKey] = value;
                EmitPropertyChanged("SaslKerberosServiceName");
            }
        }

        /// <summary>
        /// This client's Kerberos principal name. (Not supported on Windows, will use the logon user's principal).
        /// </summary>
        public string SaslKerberosPrincipal
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslKerberosPrincipalKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslKerberosPrincipalKey] = value;
                EmitPropertyChanged("SaslKerberosPrincipal");
            }
        }

        /// <summary>
        /// Shell command to refresh or acquire the client's Kerberos ticket. This command is executed on client creation and every sasl.kerberos.min.time.before.relogin (0=disable). %{config.prop.name} is replaced by corresponding config object value.
        /// </summary>
        public string SaslKerberosKinitCmd
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslKerberosKinitCmdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslKerberosKinitCmdKey] = value;
                EmitPropertyChanged("SaslKerberosKinitCmd");
            }
        }

        /// <summary>
        /// Path to Kerberos keytab file. This configuration property is only used as a variable in `sasl.kerberos.kinit.cmd` as ` ... -t "%{sasl.kerberos.keytab}"`.
        /// </summary>
        public string SaslKerberosKeytab
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslKerberosKeytabKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslKerberosKeytabKey] = value;
                EmitPropertyChanged("SaslKerberosKeytab");
            }
        }

        /// <summary>
        /// Minimum time in milliseconds between key refresh attempts. Disable automatic key refresh by setting this property to 0.
        /// </summary>
        /// <value>0 .. 86400000</value>
        public int SaslKerberosMinTimeBeforeRelogin
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslKerberosMinTimeBeforeReloginKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SaslKerberosMinTimeBeforeReloginKey] = value.ToString();
                EmitPropertyChanged("SaslKerberosMinTimeBeforeRelogin");
            }
        }

        /// <summary>
        /// SASL username for use with the PLAIN and SASL-SCRAM-.. mechanisms 
        /// </summary>
        public string SaslUsername
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslUsernameKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslUsernameKey] = value;
                EmitPropertyChanged("SaslUsername");
            }
        }

        /// <summary>
        /// SASL password for use with the PLAIN and SASL-SCRAM-.. mechanism
        /// </summary>
        public string SaslPassword
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslPasswordKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslPasswordKey] = value;
                EmitPropertyChanged("SaslPassword");
            }
        }

        /// <summary>
        /// SASL/OAUTHBEARER configuration. The format is implementation-dependent and must be parsed accordingly. The default unsecured token implementation (see https://tools.ietf.org/html/rfc7515#appendix-A.5) recognizes space-separated name=value pairs with valid names including principalClaimName, principal, scopeClaimName, scope, and lifeSeconds. The default value for principalClaimName is "sub", the default value for scopeClaimName is "scope", and the default value for lifeSeconds is 3600. The scope value is CSV format with the default value being no/empty scope. For example: `principalClaimName=azp principal=admin scopeClaimName=roles scope=role1,role2 lifeSeconds=600`. In addition, SASL extensions can be communicated to the broker via `extension_NAME=value`. For example: `principal=admin extension_traceId=123`
        /// </summary>
        public string SaslOauthbearerConfig
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerConfigKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerConfigKey] = value;
                EmitPropertyChanged("SaslOauthbearerConfig");
            }
        }

        /// <summary>
        /// Enable the builtin unsecure JWT OAUTHBEARER token handler if no oauthbearer_refresh_cb has been set. This builtin handler should only be used for development or testing, and not in production.
        /// </summary>
        public bool EnableSaslOauthbearerUnsecureJwt
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableSaslOauthbearerUnsecureJwtKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableSaslOauthbearerUnsecureJwtKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableSaslOauthbearerUnsecureJwt");
            }
        }

        /// <summary>
        /// Set to <see cref="SaslOauthbearerMethodType.DEFAULT"/> or <see cref="SaslOauthbearerMethodType.OIDC"/> to control which login method to be used. If set to <see cref="SaslOauthbearerMethodType.OIDC"/>, the following properties must also be be specified: `sasl.oauthbearer.client.id`, `sasl.oauthbearer.client.secret`, and `sasl.oauthbearer.token.endpoint.url`.
        /// </summary>
        public SaslOauthbearerMethodType SaslOauthbearerMethod
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerMethodKey, out value);
                value = value.ToUpperInvariant();
                return (SaslOauthbearerMethodType)Enum.Parse(typeof(SaslOauthbearerMethodType), value);
            }
            set
            {
                keyValuePair[SaslOauthbearerMethodKey] = Enum.Format(typeof(SaslOauthbearerMethodType), value, "g").ToLowerInvariant();
                EmitPropertyChanged("SaslOauthbearerMethod");
            }
        }

        /// <summary>
        /// Public identifier for the application. Must be unique across all clients that the authorization server handles. Only used when <see cref="SaslOauthbearerMethod"/> is set to <see cref="SaslOauthbearerMethodType.OIDC"/>.
        /// </summary>
        public string SaslOauthbearerClientId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerClientIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerClientIdKey] = value;
                EmitPropertyChanged("SaslOauthbearerClientId");
            }
        }

        /// <summary>
        /// Client secret only known to the application and the authorization server. This should be a sufficiently random string that is not guessable. Only used when <see cref="SaslOauthbearerMethod"/> is set to <see cref="SaslOauthbearerMethodType.OIDC"/>.
        /// </summary>
        public string SaslOauthbearerClientSecret
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerClientSecretKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerClientSecretKey] = value;
                EmitPropertyChanged("SaslOauthbearerClientSecret");
            }
        }

        /// <summary>
        /// Client use this to specify the scope of the access request to the broker. Only used when <see cref="SaslOauthbearerMethod"/> is set to <see cref="SaslOauthbearerMethodType.OIDC"/>.
        /// </summary>
        public string SaslOauthbearerScope
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerScopeKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerScopeKey] = value;
                EmitPropertyChanged("SaslOauthbearerScope");
            }
        }

        /// <summary>
        /// Allow additional information to be provided to the broker. Comma-separated list of key=value pairs. E.g., "supportFeatureX=true,organizationId=sales-emea". Only used when <see cref="SaslOauthbearerMethod"/> is set to <see cref="SaslOauthbearerMethodType.OIDC"/>.
        /// </summary>
        public string SaslOauthbearerExtensions
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerExtensionsKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerExtensionsKey] = value;
                EmitPropertyChanged("SaslOauthbearerExtensions");
            }
        }

        /// <summary>
        /// OAuth/OIDC issuer token endpoint HTTP(S) URI used to retrieve token. Only used when <see cref="SaslOauthbearerMethod"/> is set to <see cref="SaslOauthbearerMethodType.OIDC"/>.
        /// </summary>
        public string SaslOauthbearerTokenEndpointUrl
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SaslOauthbearerTokenEndpointUrlKey, out value);
                return value;
            }
            set
            {
                keyValuePair[SaslOauthbearerTokenEndpointUrlKey] = value;
                EmitPropertyChanged("SaslOauthbearerTokenEndpointUrl");
            }
        }

        /// <summary>
        /// List of plugin libraries to load (; separated). The library search path is platform dependent (see dlopen(3) for Unix and LoadLibrary() for Windows). If no filename extension is specified the platform-specific extension (such as .dll or .so) will be appended automatically.
        /// </summary>
        public string PluginLibraryPaths
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(PluginLibraryPathsKey, out value);
                return value;
            }
            set
            {
                keyValuePair[PluginLibraryPathsKey] = value;
                EmitPropertyChanged("PluginLibraryPaths");
            }
        }

        /// <summary>
        /// Enable static group membership. Static group members are able to leave and rejoin a group within the configured <see cref="SessionTimeout"/> without prompting a group rebalance. This should be used in combination with a larger <see cref="SessionTimeout"/> to avoid group rebalances caused by transient unavailability (e.g. process restarts). Requires broker version >= 2.3.0.
        /// </summary>
        public string GroupInstanceId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(GroupInstanceIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[GroupInstanceIdKey] = value;
                EmitPropertyChanged("GroupInstanceId");
            }
        }

        /// <summary>
        /// The name of one or more partition assignment strategies. The elected group leader will use a strategy supported by all members of the group to assign partitions to group members. If there is more than one eligible strategy, preference is determined by the order of this list (strategies earlier in the list have higher priority). Cooperative and non-cooperative (eager) strategies must not be mixed. Available strategies: range, roundrobin, cooperative-sticky. 
        /// </summary>
        public PartitionAssignmentStrategyType PartitionAssignmentStrategy
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(PartitionAssignmentStrategyKey, out value);
                value = value.Replace('_', '-');
                return (PartitionAssignmentStrategyType)Enum.Parse(typeof(PartitionAssignmentStrategyType), value);
            }
            set
            {
                keyValuePair[PartitionAssignmentStrategyKey] = Enum.Format(typeof(PartitionAssignmentStrategyType), value, "g").Replace('_', '-');
                EmitPropertyChanged("PartitionAssignmentStrategy");
            }
        }

        /// <summary>
        /// Client group session and failure detection timeout. The consumer sends periodic heartbeats (<see cref="HeartbeatInterval"/>) to indicate its liveness to the broker. If no hearts are received by the broker for a group member within the session timeout, the broker will remove the consumer from the group and trigger a rebalance. The allowed range is configured with the **broker** configuration properties `group.min.session.timeout.ms` and `group.max.session.timeout.ms`. Also see <see cref="MaxPollInterval"/>.
        /// </summary>
        /// <value>1 .. 3600000</value>
        public int SessionTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(SessionTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[SessionTimeoutKey] = value.ToString();
                EmitPropertyChanged("SessionTimeout");
            }
        }

        /// <summary>
        /// Group session keepalive heartbeat interval.
        /// </summary>
        /// <value>1 .. 3600000</value>
        public int HeartbeatInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(HeartbeatIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[HeartbeatIntervalKey] = value.ToString();
                EmitPropertyChanged("HeartbeatInterval");
            }
        }

        /// <summary>
        /// Group protocol type. NOTE: Currently, the only supported group protocol type is 'consumer'.
        /// </summary>
        public string GroupProtocolType
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(GroupProtocolTypeKey, out value);
                return value;
            }
            set
            {
                keyValuePair[GroupProtocolTypeKey] = value;
                EmitPropertyChanged("GroupProtocolType");
            }
        }

        /// <summary>
        /// How often to query for the current client group coordinator. If the currently assigned coordinator is down the configured query interval will be divided by ten to more quickly recover in case of coordinator reassignment.
        /// </summary>
        /// <value>1 .. 3600000</value>
        public int CoordinatorQueryInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CoordinatorQueryIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[CoordinatorQueryIntervalKey] = value.ToString();
                EmitPropertyChanged("CoordinatorQueryInterval");
            }
        }

        /// <summary>
        /// Maximum allowed time between calls to consume messages (e.g., rd_kafka_consumer_poll()) for high-level consumers. If this interval is exceeded the consumer is considered failed and the group will rebalance in order to reassign the partitions to another consumer group member. Warning: Offset commits may be not possible at this point. Note: It is recommended to set <see cref="EnableAutoOffsetStore"/> to false for long-time processing applications and then explicitly store offsets (using offsets_store()) *after* message processing, to make sure offsets are not auto-committed prior to processing has finished. The interval is checked two times per second. See KIP-62 for more information.
        /// </summary>
        /// <value>1 .. 86400000</value>
        public int MaxPollInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxPollIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxPollIntervalKey] = value.ToString();
                EmitPropertyChanged("MaxPollInterval");
            }
        }

        /// <summary>
        /// Automatically and periodically commit offsets in the background. Note: setting this to false does not prevent the consumer from fetching previously committed start offsets. To circumvent this behaviour set specific start offsets per partition in the call to assign().
        /// </summary>
        /// <value>1 .. 86400000</value>
        public bool EnableAutoCommit
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableAutoCommitKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableAutoCommitKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableAutoCommit");
            }
        }

        /// <summary>
        /// The frequency in milliseconds that the consumer offsets are committed (written) to offset storage. (0 = disable). This setting is used by the high-level consumer.
        /// </summary>
        /// <value>0 .. 86400000</value>
        public int AutoCommitInterval
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutoCommitIntervalKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[AutoCommitIntervalKey] = value.ToString();
                EmitPropertyChanged("AutoCommitInterval");
            }
        }

        /// <summary>
        /// Automatically store offset of last message provided to application. The offset store is an in-memory store of the next offset to (auto-)commit for each partition.
        /// </summary>
        public bool EnableAutoOffsetStore
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableAutoOffsetStoreKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableAutoOffsetStoreKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableAutoOffsetStore");
            }
        }

        /// <summary>
        /// Minimum number of messages per topic+partition librdkafka tries to maintain in the local consumer queue.
        /// </summary>
        /// <value>1 .. 10000000</value>
        public int QueuedMinMessages
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueuedMinMessagesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueuedMinMessagesKey] = value.ToString();
                EmitPropertyChanged("QueuedMinMessages");
            }
        }

        /// <summary>
        /// Maximum number of kilobytes of queued pre-fetched messages in the local consumer queue. If using the high-level consumer this setting applies to the single consumer queue, regardless of the number of partitions. When using the legacy simple consumer or when separate partition queues are used this setting applies per partition. This value may be overshot by fetch.message.max.bytes. This property has higher priority than <see cref="QueuedMinMessages"/>.
        /// </summary>
        /// <value>1 .. 2097151</value>
        public int QueuedMaxMessagesKb
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueuedMaxMessagesKbKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueuedMaxMessagesKbKey] = value.ToString();
                EmitPropertyChanged("QueuedMaxMessagesKb");
            }
        }

        /// <summary>
        /// Maximum time the broker may wait to fill the Fetch response with fetch.min.bytes of messages.
        /// </summary>
        /// <value>0 .. 300000</value>
        public int FetchWaitMax
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(FetchWaitMaxKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[FetchWaitMaxKey] = value.ToString();
                EmitPropertyChanged("FetchWaitMax");
            }
        }

        /// <summary>
        /// Initial maximum number of bytes per topic+partition to request when fetching messages from the broker. If the client encounters a message larger than this value it will gradually try to increase it until the entire message can be fetched.
        /// </summary>
        /// <value>1 .. 1000000000</value>
        public int FetchMessageMaxBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(FetchMessageMaxBytesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[FetchMessageMaxBytesKey] = value.ToString();
                EmitPropertyChanged("FetchMessageMaxBytes");
            }
        }

        /// <summary>
        /// Maximum amount of data the broker shall return for a Fetch request. Messages are fetched in batches by the consumer and if the first message batch in the first non-empty partition of the Fetch request is larger than this value, then the message batch will still be returned to ensure the consumer can make progress. The maximum message batch size accepted by the broker is defined via <see cref="MessageMaxBytes"/> (broker config) or <see cref="MessageMaxBytes"/> (broker topic config). <see cref="FetchMaxBytes"/> is automatically adjusted upwards to be at least <see cref="MessageMaxBytes"/> (consumer config).
        /// </summary>
        /// <value>0 .. 2147483135</value>
        public int FetchMaxBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(FetchMaxBytesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[FetchMaxBytesKey] = value.ToString();
                EmitPropertyChanged("FetchMaxBytes");
            }
        }

        /// <summary>
        /// Minimum number of bytes the broker responds with. If <see cref="FetchWaitMax"/> expires the accumulated data will be sent to the client regardless of this setting.
        /// </summary>
        /// <value>1 .. 100000000</value>
        public int FetchMinBytes
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(FetchMinBytesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[FetchMinBytesKey] = value.ToString();
                EmitPropertyChanged("FetchMinBytes");
            }
        }

        /// <summary>
        /// How long to postpone the next fetch request for a topic+partition in case of a fetch error.
        /// </summary>
        /// <value>0 .. 300000</value>
        public int FetchErrorBackoff
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(FetchErrorBackoffKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[FetchErrorBackoffKey] = value.ToString();
                EmitPropertyChanged("FetchErrorBackoff");
            }
        }

        /// <summary>
        /// Controls how to read messages written transactionally: <see cref="IsolationLevelType.read_committed"/> - only return transactional messages which have been committed. <see cref="IsolationLevelType.read_uncommitted"/> - return all messages, even transactional messages which have been aborted.
        /// </summary>
        public IsolationLevelType IsolationLevel
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(IsolationLevelKey, out value);
                return (IsolationLevelType)Enum.Parse(typeof(IsolationLevelType), value);
            }
            set
            {
                keyValuePair[IsolationLevelKey] = Enum.Format(typeof(IsolationLevelType), value, "g");
                EmitPropertyChanged("IsolationLevel");
            }
        }

        /// <summary>
        /// Emit RD_KAFKA_RESP_ERR__PARTITION_EOF event whenever the consumer reaches the end of a partition.
        /// </summary>
        public bool EnablePartitionEof
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnablePartitionEofKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnablePartitionEofKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnablePartitionEof");
            }
        }

        /// <summary>
        /// Verify CRC32 of consumed messages, ensuring no on-the-wire or on-disk corruption to the messages occurred. This check comes at slightly increased CPU usage.
        /// </summary>
        public bool CheckCrcs
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CheckCrcsKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[CheckCrcsKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("CheckCrcs");
            }
        }

        /// <summary>
        /// Allow automatic topic creation on the broker when subscribing to or assigning non-existent topics. The broker must also be configured with `auto.create.topics.enable=true` for this configuraiton to take effect. Note: The default value (false) is different from the Java consumer (true). Requires broker version >= 0.11.0.0, for older broker versions only the broker configuration applies.
        /// </summary>
        public bool AllowAutoCreateTopics
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AllowAutoCreateTopicsKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[AllowAutoCreateTopicsKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("AllowAutoCreateTopics");
            }
        }

        /// <summary>
        /// A rack identifier for this client. This can be any string value which indicates where this client is physically located. It corresponds with the broker config `broker.rack`.
        /// </summary>
        public string ClientRack
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ClientRackKey, out value);
                return value;
            }
            set
            {
                keyValuePair[ClientRackKey] = value;
                EmitPropertyChanged("ClientRack");
            }
        }

        /// <summary>
        /// Enables the transactional producer. The <see cref="TransactionalId"/> is used to identify the same transactional producer instance across process restarts. It allows the producer to guarantee that transactions corresponding to earlier instances of the same producer have been finalized prior to starting any new transactions, and that any zombie instances are fenced off. If no transactional.id is provided, then the producer is limited to idempotent delivery (if <see cref="EnableIdempotence"/> is set). Requires broker version >= 0.11.0.
        /// </summary>
        public string TransactionalId
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TransactionalIdKey, out value);
                return value;
            }
            set
            {
                keyValuePair[TransactionalIdKey] = value;
                EmitPropertyChanged("TransactionalId");
            }
        }

        /// <summary>
        /// The maximum amount of time in milliseconds that the transaction coordinator will wait for a transaction status update from the producer before proactively aborting the ongoing transaction. If this value is larger than the `transaction.max.timeout.ms` setting in the broker, the init_transactions() call will fail with ERR_INVALID_TRANSACTION_TIMEOUT. The transaction timeout automatically adjusts <see cref="MessageTimeout"/> and <see cref="SocketTimeout"/>, unless explicitly configured in which case they must not exceed the transaction timeout (<see cref="SocketTimeout"/> must be at least 100ms lower than <see cref="TransactionTimeout"/>). This is also the default timeout value if no timeout (-1) is supplied to the transactional API methods.
        /// </summary>
        /// <value>0 .. 300000</value>
        public int TransactionTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TransactionTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[TransactionTimeoutKey] = value.ToString();
                EmitPropertyChanged("TransactionTimeout");
            }
        }

        /// <summary>
        /// When set to `true`, the producer will ensure that messages are successfully produced exactly once and in the original produce order. The following configuration properties are adjusted automatically (if not modified by the user) when idempotence is enabled: <see cref="MaxInFlightRequestsPerConnection"/> to 5 (must be less than or equal to 5), <see cref="MessageSendMaxRetries"/> to <see cref="Int32.MaxValue"/> (must be greater than 0), <see cref="RequestRequiredAcks"/> to all, <see cref="QueueingStrategy"/> `queuing.strategy=fifo`. Producer instantation will fail if user-supplied configuration is incompatible.
        /// </summary>
        public bool EnableIdempotence
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableIdempotenceKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableIdempotenceKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableIdempotence");
            }
        }

        /// <summary>
        /// When set to `true`, any error that could result in a gap in the produced message series when a batch of messages fails, will raise a fatal error (ERR__GAPLESS_GUARANTEE) and stop the producer. Messages failing due to <see cref="MessageTimeout"/> are not covered by this guarantee. Requires <see cref="EnableIdempotence"/> to true.
        /// </summary>
        /// <remarks>**EXPERIMENTAL**: subject to change or removal.</remarks>
        public bool EnableGaplessGuarantee
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(EnableGaplessGuaranteeKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[EnableGaplessGuaranteeKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("EnableGaplessGuarantee");
            }
        }

        /// <summary>
        /// Maximum number of messages allowed on the producer queue. This queue is shared by all topics and partitions.
        /// </summary>
        /// <value>1 .. 10000000</value>
        public int QueueBufferingMaxMessages
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueueBufferingMaxMessagesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueueBufferingMaxMessagesKey] = value.ToString();
                EmitPropertyChanged("QueueBufferingMaxMessages");
            }
        }

        /// <summary>
        /// Maximum total message size sum allowed on the producer queue. This queue is shared by all topics and partitions. This property has higher priority than<see cref="QueueBufferingMaxMessages"/>.
        /// </summary>
        /// <value>1 .. 2147483647</value>
        public int QueueBufferingMaxKb
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueueBufferingMaxKbKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueueBufferingMaxKbKey] = value.ToString();
                EmitPropertyChanged("QueueBufferingMaxKb");
            }
        }

        /// <summary>
        /// Delay in milliseconds to wait for messages in the producer queue to accumulate before constructing message batches (MessageSets) to transmit to brokers. A higher value allows larger and more effective (less overhead, improved compression) batches of messages to accumulate at the expense of increased message delivery latency.
        /// </summary>
        /// <value>0 .. 900000</value>
        public int QueueBufferingMaxMs
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueueBufferingMaxMsKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueueBufferingMaxMsKey] = value.ToString();
                EmitPropertyChanged("QueueBufferingMaxMs");
            }
        }

        /// <summary>
        /// How many times to retry sending a failing Message. **Note:** retrying may cause reordering unless <see cref="EnableIdempotence"/> is set to true.
        /// </summary>
        /// <value>0 .. 2147483647</value>
        public int MessageSendMaxRetries
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MessageSendMaxRetriesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MessageSendMaxRetriesKey] = value.ToString();
                EmitPropertyChanged("MessageSendMaxRetries");
            }
        }

        /// <summary>
        /// The backoff time in milliseconds before retrying a protocol request.
        /// </summary>
        /// <value>1 .. 300000</value>
        public int RetryBackoff
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(RetryBackoffKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[RetryBackoffKey] = value.ToString();
                EmitPropertyChanged("RetryBackoff");
            }
        }

        /// <summary>
        /// The threshold of outstanding not yet transmitted broker requests needed to backpressure the producer's message accumulator. If the number of not yet transmitted requests equals or exceeds this number, produce request creation that would have otherwise been triggered (for example, in accordance with <see cref="QueueBufferingMaxMs"/>) will be delayed. A lower number yields larger and more effective batches. A higher value can improve latency when using compression on slow machines.
        /// </summary>
        /// <value>1 .. 1000000</value>
        public int QueueBufferingBackpressureThreshold
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueueBufferingBackpressureThresholdKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[QueueBufferingBackpressureThresholdKey] = value.ToString();
                EmitPropertyChanged("QueueBufferingBackpressureThreshold");
            }
        }

        /// <summary>
        /// Compression codec to use for compressing message sets. This is the default value for all topics, may be overridden by the topic configuration property `compression.codec`.
        /// </summary>
        public CompressionCodecType CompressionCodec
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CompressionCodecKey, out value);
                return (CompressionCodecType)Enum.Parse(typeof(CompressionCodecType), value);
            }
            set
            {
                keyValuePair[CompressionCodecKey] = Enum.Format(typeof(CompressionCodecType), value, "g");
                EmitPropertyChanged("CompressionCodec");
            }
        }

        /// <summary>
        /// Maximum number of messages batched in one MessageSet. The total MessageSet size is also limited by <see cref="BatchSize"/> and <see cref="MessageMaxBytes"/>.
        /// </summary>
        /// <value>1 .. 1000000</value>
        public int BatchNumMessages
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BatchNumMessagesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[BatchNumMessagesKey] = value.ToString();
                EmitPropertyChanged("BatchNumMessages");
            }
        }

        /// <summary>
        /// Maximum size (in bytes) of all messages batched in one MessageSet, including protocol framing overhead. This limit is applied after the first message has been added to the batch, regardless of the first message's size, this is to ensure that messages that exceed batch.size are produced. The total MessageSet size is also limited by <see cref="BatchNumMessages"/> and <see cref="MessageMaxBytes"/>.
        /// </summary>
        /// <value>1 .. 2147483647</value>
        public int BatchSize
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(BatchSizeKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[BatchSizeKey] = value.ToString();
                EmitPropertyChanged("BatchSize");
            }
        }

        /// <summary>
        /// Only provide delivery reports for failed messages.
        /// </summary>
        public bool DeliveryReportOnlyError
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DeliveryReportOnlyErrorKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[DeliveryReportOnlyErrorKey] = value.ToString().ToLowerInvariant();
                EmitPropertyChanged("DeliveryReportOnlyError");
            }
        }

        /// <summary>
        /// Delay in milliseconds to wait to assign new sticky partitions for each topic. By default, set to double the time of linger.ms. To disable sticky behavior, set to 0. This behavior affects messages with the key NULL in all cases, and messages with key lengths of zero when the consistent_random partitioner is in use. These messages would otherwise be assigned randomly. A higher value allows for more effective batching of these messages.
        /// </summary>
        /// <value>0 .. 900000</value>
        public int StickyPartitioningLinger
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(StickyPartitioningLingerKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[StickyPartitioningLingerKey] = value.ToString();
                EmitPropertyChanged("StickyPartitioningLinger");
            }
        }

        #endregion

        #region Topic conf
        /// <summary>
        /// This field indicates the number of acknowledgements the leader broker must receive from ISR brokers before responding to the request: *0*=Broker does not send any response/ack to client, *-1* or *all*=Broker will block until message is committed by all in sync replicas (ISRs). If there are less than `min.insync.replicas` (broker configuration) in the ISR set the produce request will fail.
        /// </summary>
        /// <value>-1 .. 1000</value>
        public int RequestRequiredAcks
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(RequestRequiredAcksKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[RequestRequiredAcksKey] = value.ToString();
                EmitPropertyChanged("RequestRequiredAcks");
            }
        }

        /// <summary>
        /// The ack timeout of the producer request in milliseconds. This value is only enforced by the broker and relies on <see cref="RequestRequiredAcks"/> being != 0
        /// </summary>
        /// <value>-1 .. 1000</value>
        public int RequestTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(RequestTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[RequestTimeoutKey] = value.ToString();
                EmitPropertyChanged("RequestTimeout");
            }
        }

        /// <summary>
        /// Local message timeout. This value is only enforced locally and limits the time a produced message waits for successful delivery. A time of 0 is infinite. This is the maximum time librdkafka may use to deliver a message (including retries). Delivery error occurs when either the retry count or the message timeout are exceeded. The message timeout is automatically adjusted to <see cref="TransactionTimeout"/> if <see cref="TransactionalId"/> is configured.
        /// </summary>
        /// <value>0 .. 2147483647</value>
        public int MessageTimeout
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MessageTimeoutKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MessageTimeoutKey] = value.ToString();
                EmitPropertyChanged("MessageTimeout");
            }
        }

        /// <summary>
        /// Partitioner: <see cref="PartitionerType"/>
        /// </summary>
        public PartitionerType Partitioner
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(PartitionerKey, out value);
                return (PartitionerType)Enum.Parse(typeof(PartitionerType), value);
            }
            set
            {
                keyValuePair[PartitionerKey] = Enum.Format(typeof(PartitionerType), value, "g");
                EmitPropertyChanged("Partitioner");
            }
        }

        /// <summary>
        /// <see cref="QueueingStrategyType.fifo"/> preserves produce ordering, while <see cref="QueueingStrategyType.lifo"/> prioritizes new messages.
        /// </summary>
        /// <remarks>**EXPERIMENTAL**: subject to change or removal. **DEPRECATED** Producer queuing strategy.</remarks>
        public QueueingStrategyType QueueingStrategy
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(QueueingStrategyKey, out value);
                return (QueueingStrategyType)Enum.Parse(typeof(QueueingStrategyType), value);
            }
            set
            {
                keyValuePair[QueueingStrategyKey] = Enum.Format(typeof(QueueingStrategyType), value, "g");
                EmitPropertyChanged("QueueingStrategy");
            }
        }

        /// <summary>
        /// Compression codec to use for compressing message sets. inherit = inherit global compression.codec configuration.
        /// </summary>
        /// <remarks>inherit is default: don't set this preperty to use default</remarks>
        public CompressionCodecType TopicCompressionCodec
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(TopicCompressionCodecKey, out value);
                return (CompressionCodecType)Enum.Parse(typeof(CompressionCodecType), value);
            }
            set
            {
                keyValuePair[TopicCompressionCodecKey] = Enum.Format(typeof(CompressionCodecType), value, "g");
                EmitPropertyChanged("TopicCompressionCodec");
            }
        }

        /// <summary>
        /// Compression level parameter for algorithm selected by configuration property <see cref="CompressionCodec"/>. Higher values will result in better compression at the cost of more CPU usage. Usable range is algorithm-dependent: [0-9] for gzip; [0-12] for lz4; only 0 for snappy; -1 = codec-dependent default compression level.
        /// </summary>
        /// <value>0 .. 2147483647</value>
        public int CompressionLevel
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CompressionLevelKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[CompressionLevelKey] = value.ToString();
                EmitPropertyChanged("CompressionLevel");
            }
        }

        /// <summary>
        /// Action to take when there is no initial offset in offset store or the desired offset is out of range: <see cref="AutoOffsetResetType.smallest"/>,<see cref="AutoOffsetResetType.earliest"/> - automatically reset the offset to the smallest offset, <see cref="AutoOffsetResetType.largest"/>,<see cref="AutoOffsetResetType.latest"/> - automatically reset the offset to the largest offset, <see cref="AutoOffsetResetType.error"/> - trigger an error (ERR__AUTO_OFFSET_RESET) which is retrieved by consuming messages and checking 'message->err'.
        /// </summary>
        public AutoOffsetResetType AutoOffsetReset
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutoOffsetResetKey, out value);
                return (AutoOffsetResetType)Enum.Parse(typeof(AutoOffsetResetType), value);
            }
            set
            {
                keyValuePair[AutoOffsetResetKey] = Enum.Format(typeof(AutoOffsetResetType), value, "g");
                EmitPropertyChanged("AutoOffsetReset");
            }
        }

        /// <summary>
        /// Maximum number of messages to dispatch in one `rd_kafka_consume_callback*()` call (0 = unlimited)
        /// </summary>
        /// <value>0 .. 1000000</value>
        public int ConsumeCallbackMaxMessages
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ConsumeCallbackMaxMessagesKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ConsumeCallbackMaxMessagesKey] = value.ToString();
                EmitPropertyChanged("ConsumeCallbackMaxMessages");
            }
        }

        #endregion

        /// <summary>
        /// Generic setter/getter for all configuration properties related to librdkafka (see https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md)
        /// </summary>
        /// <param name="type"><see cref="KafkaConfigurationType"/> to set</param>
        /// <param name="property">The property name</param>
        /// <returns>The property value</returns>
        public string this[KafkaConfigurationType type, string property]
        {
            get
            {
                string propKey = null;
                switch (type)
                {
                    case KafkaConfigurationType.GLOBAL_CONF:
                        propKey = KafkaGlobalConfigurationBasePropertyKey;
                        break;
                    case KafkaConfigurationType.TOPIC_CONF:
                        propKey = KafkaTopicConfigurationBasePropertyKey;
                        break;
                    default: throw new ArgumentException(string.Format("Type {0} is invalid", type));
                }

                string value = string.Empty;
                if (property.StartsWith(propKey))
                {
                    keyValuePair.TryGetValue(property, out value);
                }
                else
                {
                    keyValuePair.TryGetValue(propKey + property, out value);
                }
                return value;
            }
            set
            {
                string propKey = null;
                switch (type)
                {
                    case KafkaConfigurationType.GLOBAL_CONF:
                        propKey = KafkaGlobalConfigurationBasePropertyKey;
                        break;
                    case KafkaConfigurationType.TOPIC_CONF:
                        propKey = KafkaTopicConfigurationBasePropertyKey;
                        break;
                    default: throw new ArgumentException(string.Format("Type {0} is invalid", type));
                }

                if (property.StartsWith(propKey))
                {
                    keyValuePair[property] = value;
                }
                else
                {
                    keyValuePair[propKey + property] = value;
                }
                EmitPropertyChanged(property);
            }
        }

        /// <inheritdoc/>
        protected override void CheckConfiguration()
        {
            base.CheckConfiguration();
            if (!keyValuePair.ContainsKey(BootstrapBrokersKey))
            {
                throw new InvalidOperationException("Missing BootstrapBrokers");
            }
            if (!keyValuePair.ContainsKey(ClientIdKey))
            {
                throw new InvalidOperationException("Missing ClientId");
            }
            if (!keyValuePair.ContainsKey(GroupIdKey))
            {
                throw new InvalidOperationException("Missing GroupId");
            }
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                CheckConfiguration();
                return base.Configuration;
            }
        }
    }
    #endregion
}
