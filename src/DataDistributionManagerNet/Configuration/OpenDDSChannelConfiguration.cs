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

using System;
using System.Collections.Generic;
using System.Text;

namespace MASES.DataDistributionManager.Bindings.Configuration
{
    #region PropertyQosPolicyConfiguration
    /// <summary>
    /// Class managing PropertyQosPolicy
    /// </summary>
    public class PropertyQosPolicyConfiguration : BaseConfiguration
    {
        #region Property
        /// <summary>
        /// A single property
        /// </summary>
        public class Property
        {
            /// <summary>
            /// Initialize a new <see cref="Property"/>
            /// </summary>
            public Property()
            {

            }
            /// <summary>
            /// Initialize a new <see cref="Property"/>
            /// </summary>
            /// <param name="value">Value to use (file: or data:)</param>
            /// <param name="propagate">True to propagate. Default is false</param>
            public Property(string value, bool propagate = false)
            {
                Value = value;
                Propagate = propagate;
            }

            /// <summary>
            /// The value
            /// </summary>
            public string Value { get; set; }
            /// <summary>
            /// True to propagate
            /// </summary>
            public bool Propagate { get; set; }
        }
        #endregion

        /// <summary>
        /// Configuration key of <see cref="PropertyQosPolicyConfiguration"/>
        /// </summary>
        public const string PropertyQosPolicyValuePropertyKey = "datadistributionmanager.opendds.qos.propertyqospolicy.value";
        /// <summary>
        /// Configuration key of <see cref="PropertyQosPolicyConfiguration"/>
        /// </summary>
        public const string PropertyQosPolicyBinaryValuePropertyKey = "datadistributionmanager.opendds.qos.propertyqospolicy.binary_value";
        /// <summary>
        /// Initialize a new instance of <see cref="PropertyQosPolicyConfiguration"/>
        /// </summary>
        public PropertyQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_IDENTITY_CA { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_IDENTITY_CERT { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_IDENTITY_PRIVKEY { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_IDENTITY_PRIVKEY_PWD { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_PERM_CA { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_PERM_GOV_DOC { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public Property DDSSEC_PROP_PERM_DOC { get; set; }
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public override string[] Configuration
        {
            get
            {
                if (DDSSEC_PROP_IDENTITY_CA == null || DDSSEC_PROP_IDENTITY_CERT == null || DDSSEC_PROP_IDENTITY_PRIVKEY == null 
                    || DDSSEC_PROP_PERM_CA == null || DDSSEC_PROP_PERM_GOV_DOC == null || DDSSEC_PROP_PERM_DOC == null)
                {
                    throw new InvalidOperationException("Only DDSSEC_PROP_IDENTITY_PRIVKEY_PWD can be null in PropertyQosPolicyConfiguration");
                }

                StringBuilder sb = new StringBuilder();
                sb.AppendFormat("{0};{1};{2}", "dds.sec.auth.identity_ca", DDSSEC_PROP_IDENTITY_CA.Value, DDSSEC_PROP_IDENTITY_CA.Propagate.ToString().ToLowerInvariant());sb.Append("|");
                sb.AppendFormat("{0};{1};{2}", "dds.sec.auth.identity_certificate", DDSSEC_PROP_IDENTITY_CERT.Value, DDSSEC_PROP_IDENTITY_CERT.Propagate.ToString().ToLowerInvariant()); sb.Append("|");
                sb.AppendFormat("{0};{1};{2}", "dds.sec.auth.private_key", DDSSEC_PROP_IDENTITY_PRIVKEY.Value, DDSSEC_PROP_IDENTITY_PRIVKEY.Propagate.ToString().ToLowerInvariant()); sb.Append("|");
                if (DDSSEC_PROP_IDENTITY_PRIVKEY_PWD != null) sb.AppendFormat("{0};{1};{2}", "dds.sec.auth.password", DDSSEC_PROP_IDENTITY_PRIVKEY_PWD.Value, DDSSEC_PROP_IDENTITY_PRIVKEY_PWD.Propagate.ToString().ToLowerInvariant()); sb.Append("|");
                sb.AppendFormat("{0};{1};{2}", "dds.sec.access.permissions_ca", DDSSEC_PROP_PERM_CA.Value, DDSSEC_PROP_PERM_CA.Propagate.ToString().ToLowerInvariant()); sb.Append("|");
                sb.AppendFormat("{0};{1};{2}", "dds.sec.access.governance", DDSSEC_PROP_PERM_GOV_DOC.Value, DDSSEC_PROP_PERM_GOV_DOC.Propagate.ToString().ToLowerInvariant()); sb.Append("|");
                sb.AppendFormat("{0};{1};{2}", "dds.sec.access.permissions", DDSSEC_PROP_PERM_DOC.Value, DDSSEC_PROP_PERM_DOC.Propagate.ToString().ToLowerInvariant()); sb.Append("|");

                List<string> lst = new List<string>(base.Configuration);
                lst.Add(string.Format("{0}={1}", PropertyQosPolicyValuePropertyKey, sb.ToString()));
                return lst.ToArray();
            }
        }
    }
    #endregion

    #region DurabilityQosPolicyConfiguration
    /// <summary>
    /// Class managing DurabilityQosPolicy
    /// </summary>
    public class DurabilityQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum DurabilityQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            VOLATILE_DURABILITY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            TRANSIENT_LOCAL_DURABILITY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            TRANSIENT_DURABILITY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            PERSISTENT_DURABILITY_QOS
        }

        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.durabilityqospolicy.kind";
        /// <summary>
        /// Initialize a new instance of <see cref="DurabilityQosPolicyConfiguration"/>
        /// </summary>
        public DurabilityQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="DurabilityQosPolicyConfiguration"/>
        /// </summary>
        /// <param name="kind">Value to use for <see name="DurabilityQosPolicy"/></param>
        public DurabilityQosPolicyConfiguration(DurabilityQosPolicyKind kind)
        {
            Kind = kind;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (DurabilityQosPolicyKind)Enum.Parse(typeof(DurabilityQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region DurabilityServiceQosPolicyConfiguration
    /// <summary>
    /// Class managing DurabilityServiceQosPolicy
    /// </summary>
    public class DurabilityServiceQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum HistoryQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            KEEP_LAST_HISTORY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            KEEP_ALL_HISTORY_QOS
        }

        /// <summary>
        /// Configuration key of <see cref="ServiceCleanupDelay"/>
        /// </summary>
        public const string ServiceCleanupDelayPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.service_cleanup_delay";
        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.kind";
        /// <summary>
        /// Configuration key of <see cref="HistoryDepth"/>
        /// </summary>
        public const string HistoryDepthPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.history_depth";
        /// <summary>
        /// Configuration key of <see cref="MaxSamples"/>
        /// </summary>
        public const string MaxSamplesPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_samples";
        /// <summary>
        /// Configuration key of <see cref="MaxInstances"/>
        /// </summary>
        public const string MaxInstancesPropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_instances";
        /// <summary>
        /// Configuration key of <see cref="MaxSamplesPerInstance"/>
        /// </summary>
        public const string MaxSamplesPerInstancePropertyKey = "datadistributionmanager.opendds.qos.durabilityserviceqospolicy.max_samples_per_instance";
        /// <summary>
        /// Initialize a new instance of <see cref="DurabilityServiceQosPolicyConfiguration"/>
        /// </summary>
        public DurabilityServiceQosPolicyConfiguration()
        {

        }
        /// <summary>
        /// Initialize a new instance of <see cref="DurabilityServiceQosPolicyConfiguration"/>
        /// </summary>
        /// <param name="kind">Value to use for <see name="HistoryQosPolicyKind"/></param>
        public DurabilityServiceQosPolicyConfiguration(HistoryQosPolicyKind kind)
        {
            Kind = kind;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int ServiceCleanupDelay
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ServiceCleanupDelayPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ServiceCleanupDelayPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public HistoryQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (HistoryQosPolicyKind)Enum.Parse(typeof(HistoryQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int HistoryDepth
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(HistoryDepthPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[HistoryDepthPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxSamples
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxSamplesPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxSamplesPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxInstances
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxInstancesPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxInstancesPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxSamplesPerInstance
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxSamplesPerInstancePropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxSamplesPerInstancePropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region DeadlineQosPolicyConfiguration
    /// <summary>
    /// Class managing DeadlineQosPolicy
    /// </summary>
    public class DeadlineQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Period"/>
        /// </summary>
        public const string PeriodPropertyKey = "datadistributionmanager.opendds.qos.deadlineqospolicy.period";
        /// <summary>
        /// Initialize a new instance of <see cref="DeadlineQosPolicyConfiguration"/>
        /// </summary>
        public DeadlineQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="DeadlineQosPolicyConfiguration"/>
        /// </summary>
        public DeadlineQosPolicyConfiguration(int period)
        {
            Period = period;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Period
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(PeriodPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[PeriodPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region LatencyBudgetQosPolicyConfiguration
    /// <summary>
    /// Class managing LatencyBudgetQosPolicy
    /// </summary>
    public class LatencyBudgetQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Period"/>
        /// </summary>
        public const string PeriodPropertyKey = "datadistributionmanager.opendds.qos.latencybudgetqospolicy.period";
        /// <summary>
        /// Initialize a new instance of <see cref="LatencyBudgetQosPolicyConfiguration"/>
        /// </summary>
        public LatencyBudgetQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="LatencyBudgetQosPolicyConfiguration"/>
        /// </summary>
        public LatencyBudgetQosPolicyConfiguration(int period)
        {
            Period = period;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Period
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(PeriodPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[PeriodPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region LivelinessQosPolicyConfiguration
    /// <summary>
    /// Class managing LivelinessQosPolicy
    /// </summary>
    public class LivelinessQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum LivelinessQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            AUTOMATIC_LIVELINESS_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            MANUAL_BY_TOPIC_LIVELINESS_QOS
        }
        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.livelinessqospolicy.kind";
        /// <summary>
        /// Configuration key of <see cref="LeaseDuration"/>
        /// </summary>
        public const string LeaseDurationPropertyKey = "datadistributionmanager.opendds.qos.livelinessqospolicy.lease_duration";
        /// <summary>
        /// Initialize a new instance of <see cref="LivelinessQosPolicyConfiguration"/>
        /// </summary>
        public LivelinessQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="LivelinessQosPolicyConfiguration"/>
        /// </summary>
        public LivelinessQosPolicyConfiguration(LivelinessQosPolicyKind kind, int period)
        {
            Kind = kind;
            LeaseDuration = period;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LivelinessQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (LivelinessQosPolicyKind)Enum.Parse(typeof(LivelinessQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int LeaseDuration
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(LeaseDurationPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[LeaseDurationPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region ReliabilityQosPolicyConfiguration
    /// <summary>
    /// Class managing ReliabilityQosPolicy
    /// </summary>
    public class ReliabilityQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum ReliabilityQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            BEST_EFFORT_RELIABILITY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            RELIABLE_RELIABILITY_QOS
        }
        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.reliabilityqospolicy.kind";
        /// <summary>
        /// Configuration key of <see cref="MaxBlockingTime"/>
        /// </summary>
        public const string MaxBlockingTimePropertyKey = "datadistributionmanager.opendds.qos.reliabilityqospolicy.max_blocking_time";
        /// <summary>
        /// Initialize a new instance of <see cref="ReliabilityQosPolicyConfiguration"/>
        /// </summary>
        public ReliabilityQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="ReliabilityQosPolicyConfiguration"/>
        /// </summary>
        public ReliabilityQosPolicyConfiguration(ReliabilityQosPolicyKind kind, int period)
        {
            Kind = kind;
            MaxBlockingTime = period;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ReliabilityQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (ReliabilityQosPolicyKind)Enum.Parse(typeof(ReliabilityQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxBlockingTime
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxBlockingTimePropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxBlockingTimePropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region DestinationOrderQosPolicyConfiguration
    /// <summary>
    /// Class managing DestinationOrderQosPolicy
    /// </summary>
    public class DestinationOrderQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum DestinationOrderQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
        }
        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.destinationorderqospolicy.kind";
        /// <summary>
        /// Initialize a new instance of <see cref="DestinationOrderQosPolicyConfiguration"/>
        /// </summary>
        public DestinationOrderQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="DestinationOrderQosPolicyConfiguration"/>
        /// </summary>
        public DestinationOrderQosPolicyConfiguration(DestinationOrderQosPolicyKind kind)
        {
            Kind = kind;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DestinationOrderQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (DestinationOrderQosPolicyKind)Enum.Parse(typeof(DestinationOrderQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region HistoryQosPolicyConfiguration
    /// <summary>
    /// Class managing HistoryQosPolicy
    /// </summary>
    public class HistoryQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum HistoryQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            KEEP_LAST_HISTORY_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            KEEP_ALL_HISTORY_QOS
        }
        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.historyqospolicy.kind";
        /// <summary>
        /// Configuration key of <see cref="Depth"/>
        /// </summary>
        public const string DepthPropertyKey = "datadistributionmanager.opendds.qos.historyqospolicy.depth";
        /// <summary>
        /// Initialize a new instance of <see cref="HistoryQosPolicyConfiguration"/>
        /// </summary>
        public HistoryQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="HistoryQosPolicyConfiguration"/>
        /// </summary>
        public HistoryQosPolicyConfiguration(HistoryQosPolicyKind kind, int depth)
        {
            Kind = kind;
            Depth = depth;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public HistoryQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (HistoryQosPolicyKind)Enum.Parse(typeof(HistoryQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Depth
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DepthPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[DepthPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region ResourceLimitsQosPolicyConfiguration
    /// <summary>
    /// Class managing ResourceLimitsQosPolicy
    /// </summary>
    public class ResourceLimitsQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="MaxSamples"/>
        /// </summary>
        public const string MaxSamplesPropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_samples";
        /// <summary>
        /// Configuration key of <see cref="MaxInstances"/>
        /// </summary>
        public const string MaxInstancesPropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_instances";
        /// <summary>
        /// Configuration key of <see cref="MaxSamplesPerInstance"/>
        /// </summary>
        public const string MaxSamplesPerInstancePropertyKey = "datadistributionmanager.opendds.qos.resourcelimitsqospolicy.max_samples_per_instance";
        /// <summary>
        /// Initialize a new instance of <see cref="ResourceLimitsQosPolicyConfiguration"/>
        /// </summary>
        public ResourceLimitsQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxSamples
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxSamplesPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxSamplesPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxInstances
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxInstancesPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxInstancesPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MaxSamplesPerInstance
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MaxSamplesPerInstancePropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MaxSamplesPerInstancePropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region TransportPriorityQosPolicyConfiguration
    /// <summary>
    /// Class managing TransportPriorityQosPolicy
    /// </summary>
    public class TransportPriorityQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Value"/>
        /// </summary>
        public const string ValuePropertyKey = "datadistributionmanager.opendds.qos.transportpriorityqospolicy.value";
        /// <summary>
        /// Initialize a new instance of <see cref="TransportPriorityQosPolicyConfiguration"/>
        /// </summary>
        public TransportPriorityQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Value
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ValuePropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ValuePropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region LifespanQosPolicyConfiguration
    /// <summary>
    /// Class managing LifespanQosPolicy
    /// </summary>
    public class LifespanQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Duration"/>
        /// </summary>
        public const string DurationPropertyKey = "datadistributionmanager.opendds.qos.lifespanqospolicy.duration";
        /// <summary>
        /// Initialize a new instance of <see cref="LifespanQosPolicyConfiguration"/>
        /// </summary>
        public LifespanQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Duration
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(DurationPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[DurationPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region OwnershipQosPolicyConfiguration
    /// <summary>
    /// Class managing OwnershipQosPolicy
    /// </summary>
    public class OwnershipQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum OwnershipQosPolicyKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            SHARED_OWNERSHIP_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            EXCLUSIVE_OWNERSHIP_QOS
        }

        /// <summary>
        /// Configuration key of <see cref="Kind"/>
        /// </summary>
        public const string KindPropertyKey = "datadistributionmanager.opendds.qos.ownershipqospolicy.kind";
        /// <summary>
        /// Initialize a new instance of <see cref="OwnershipQosPolicyConfiguration"/>
        /// </summary>
        public OwnershipQosPolicyConfiguration()
        {
        }
        /// <summary>
        /// Initialize a new instance of <see cref="OwnershipQosPolicyConfiguration"/>
        /// </summary>
        /// <param name="ownershipQosPolicy">Value to use for <see name="OwnershipQosPolicy"/></param>
        public OwnershipQosPolicyConfiguration(OwnershipQosPolicyKind ownershipQosPolicy)
        {
            Kind = ownershipQosPolicy;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public OwnershipQosPolicyKind Kind
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(KindPropertyKey, out value);
                return (OwnershipQosPolicyKind)Enum.Parse(typeof(OwnershipQosPolicyKind), value);
            }
            set
            {
                keyValuePair[KindPropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region OwnershipStrengthQosPolicyConfiguration
    /// <summary>
    /// Class managing OwnershipStrengthQosPolicy
    /// </summary>
    public class OwnershipStrengthQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Value"/>
        /// </summary>
        public const string ValuePropertyKey = "datadistributionmanager.opendds.qos.ownershipstrengthqospolicy.value";
        /// <summary>
        /// Initialize a new instance of <see cref="OwnershipStrengthQosPolicyConfiguration"/>
        /// </summary>
        public OwnershipStrengthQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int Value
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ValuePropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[ValuePropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region WriterDataLifecycleQosPolicyConfiguration
    /// <summary>
    /// Class managing WriterDataLifecycleQosPolicy
    /// </summary>
    public class WriterDataLifecycleQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="AutodisposeUnregisteredInstances"/>
        /// </summary>
        public const string AutodisposeUnregisteredInstancesPropertyKey = "datadistributionmanager.opendds.qos.writerdatalifecycleqospolicy.autodispose_unregistered_instances";
        /// <summary>
        /// Initialize a new instance of <see cref="WriterDataLifecycleQosPolicyConfiguration"/>
        /// </summary>
        public WriterDataLifecycleQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public bool AutodisposeUnregisteredInstances
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutodisposeUnregisteredInstancesPropertyKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[AutodisposeUnregisteredInstancesPropertyKey] = value.ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region PresentationQosPolicyConfiguration
    /// <summary>
    /// Class managing PresentationQosPolicy
    /// </summary>
    public class PresentationQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public enum PresentationQosPolicyAccessScopeKind
        {
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            INSTANCE_PRESENTATION_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            TOPIC_PRESENTATION_QOS,
            /// <summary>
            /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
            /// </summary>
            GROUP_PRESENTATION_QOS
        }
        /// <summary>
        /// Configuration key of <see cref="AccessScope"/>
        /// </summary>
        public const string AccessScopePropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.access_scope";
        /// <summary>
        /// Configuration key of <see cref="CoherentAccess"/>
        /// </summary>
        public const string CoherentAccessPropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.coherent_access";
        /// <summary>
        /// Configuration key of <see cref="OrderedAccess"/>
        /// </summary>
        public const string OrderedAccessPropertyKey = "datadistributionmanager.opendds.qos.presentationqospolicy.ordered_access";
        /// <summary>
        /// Initialize a new instance of <see cref="OwnershipStrengthQosPolicyConfiguration"/>
        /// </summary>
        public PresentationQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PresentationQosPolicyAccessScopeKind AccessScope
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AccessScopePropertyKey, out value);
                return (PresentationQosPolicyAccessScopeKind)Enum.Parse(typeof(PresentationQosPolicyAccessScopeKind), value);
            }
            set
            {
                keyValuePair[AccessScopePropertyKey] = ((int)value).ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public bool CoherentAccess
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(CoherentAccessPropertyKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[CoherentAccessPropertyKey] = value.ToString().ToLowerInvariant();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public bool OrderedAccess
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(OrderedAccessPropertyKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[OrderedAccessPropertyKey] = value.ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region PartitionQosPolicyConfiguration
    /// <summary>
    /// Class managing PartitionQosPolicy
    /// </summary>
    public class PartitionQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Name"/>
        /// </summary>
        public const string NamePropertyKey = "datadistributionmanager.opendds.qos.partitionqospolicy.name";
        /// <summary>
        /// Initialize a new instance of <see cref="PresentationQosPolicyConfiguration"/>
        /// </summary>
        public PartitionQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public string Name
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(NamePropertyKey, out value);
                return value;
            }
            set
            {
                keyValuePair[NamePropertyKey] = value;
            }
        }

    }
    #endregion

    #region TimeBasedFilterQosPolicyConfiguration
    /// <summary>
    /// Class managing TimeBasedFilterQosPolicy
    /// </summary>
    public class TimeBasedFilterQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="MinimumSeparation"/>
        /// </summary>
        public const string MinimumSeparationPropertyKey = "datadistributionmanager.opendds.qos.timebasedfilterqospolicy.minimum_separation";
        /// <summary>
        /// Initialize a new instance of <see cref="TimeBasedFilterQosPolicyConfiguration"/>
        /// </summary>
        public TimeBasedFilterQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int MinimumSeparation
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(MinimumSeparationPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[MinimumSeparationPropertyKey] = value.ToString();
            }
        }

    }
    #endregion

    #region ReaderDataLifecycleQosPolicyConfiguration
    /// <summary>
    /// Class managing ReaderDataLifecycleQosPolicy
    /// </summary>
    public class ReaderDataLifecycleQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="AutopurgeNowriterSamplesDelay"/>
        /// </summary>
        public const string AutopurgeNowriterSamplesDelayPropertyKey = "datadistributionmanager.opendds.qos.readerdatalifecycleqospolicy.autopurge_nowriter_samples_delay";
        /// <summary>
        /// Configuration key of <see cref="AutopurgeDisposedSamplesDelay"/>
        /// </summary>
        public const string AutopurgeDisposedSamplesDelayPropertyKey = "datadistributionmanager.opendds.qos.readerdatalifecycleqospolicy.autopurge_disposed_samples_delay";
        /// <summary>
        /// Initialize a new instance of <see cref="ReaderDataLifecycleQosPolicyConfiguration"/>
        /// </summary>
        public ReaderDataLifecycleQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int AutopurgeNowriterSamplesDelay
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutopurgeNowriterSamplesDelayPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[AutopurgeNowriterSamplesDelayPropertyKey] = value.ToString();
            }
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public int AutopurgeDisposedSamplesDelay
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutopurgeDisposedSamplesDelayPropertyKey, out value);
                return int.Parse(value);
            }
            set
            {
                keyValuePair[AutopurgeDisposedSamplesDelayPropertyKey] = value.ToString();
            }
        }
    }
    #endregion

    #region EntityFactoryQosPolicyConfiguration
    /// <summary>
    /// Class managing EntityFactoryQosPolicy
    /// </summary>
    public class EntityFactoryQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="AutoenableCreatedEntities"/>
        /// </summary>
        public const string AutoenableCreatedEntitiesPropertyKey = "datadistributionmanager.opendds.qos.entityfactoryqospolicy.autoenable_created_entities";
        /// <summary>
        /// Initialize a new instance of <see cref="EntityFactoryQosPolicyConfiguration"/>
        /// </summary>
        public EntityFactoryQosPolicyConfiguration()
        {
            AutoenableCreatedEntities = true;
        }
        /// <summary>
        /// Initialize a new instance of <see cref="EntityFactoryQosPolicyConfiguration"/>
        /// </summary>
        /// <param name="autoenableCreatedEntities">Value to use for EntityFactoryQosPolicy</param>
        public EntityFactoryQosPolicyConfiguration(bool autoenableCreatedEntities)
        {
            AutoenableCreatedEntities = autoenableCreatedEntities;
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public bool AutoenableCreatedEntities
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(AutoenableCreatedEntitiesPropertyKey, out value);
                return bool.Parse(value);
            }
            set
            {
                keyValuePair[AutoenableCreatedEntitiesPropertyKey] = value.ToString().ToLowerInvariant();
            }
        }
    }
    #endregion

    #region UserDataQosPolicyConfiguration
    /// <summary>
    /// Class managing UserDataQosPolicy
    /// </summary>
    public class UserDataQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Value"/>
        /// </summary>
        public const string ValuePropertyKey = "datadistributionmanager.opendds.qos.userdataqospolicy.value";
        /// <summary>
        /// Initialize a new instance of <see cref="UserDataQosPolicyConfiguration"/>
        /// </summary>
        public UserDataQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public byte[] Value
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ValuePropertyKey, out value);
                if (value == null) return null;
                if (value == string.Empty) return new byte[0];
                List<byte> lst = new List<byte>();
                for (int i = 0; i < value.Length / 2; i++)
                {
                    byte val = Convert.ToByte(value.Substring(i * 2, 2), 16);
                    lst.Add(val);
                }
                return lst.ToArray();
            }
            set
            {
                if (value == null) keyValuePair.Remove(ValuePropertyKey);
                System.Text.StringBuilder builder = new System.Text.StringBuilder();
                for (int i = 0; i < value.Length; i++)
                {
                    builder.Append(value[i].ToString("x2"));
                }
                keyValuePair[ValuePropertyKey] = builder.ToString();
            }
        }
    }
    #endregion

    #region TopicDataQosPolicyConfiguration
    /// <summary>
    /// Class managing TopicDataQosPolicy
    /// </summary>
    public class TopicDataQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Value"/>
        /// </summary>
        public const string ValuePropertyKey = "datadistributionmanager.opendds.qos.topicdataqospolicy.value";
        /// <summary>
        /// Initialize a new instance of <see cref="TopicDataQosPolicyConfiguration"/>
        /// </summary>
        public TopicDataQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public byte[] Value
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ValuePropertyKey, out value);
                if (string.IsNullOrEmpty(value)) return null;
                List<byte> lst = new List<byte>();
                for (int i = 0; i < value.Length / 2; i++)
                {
                    byte val = Convert.ToByte(value.Substring(i * 2, 2), 16);
                    lst.Add(val);
                }
                return lst.ToArray();
            }
            set
            {
                if (value == null) keyValuePair.Remove(ValuePropertyKey);
                System.Text.StringBuilder builder = new System.Text.StringBuilder();
                for (int i = 0; i < value.Length; i++)
                {
                    builder.Append(value[i].ToString("x2"));
                }
                keyValuePair[ValuePropertyKey] = builder.ToString();
            }
        }
    }
    #endregion

    #region GroupDataQosPolicyConfiguration
    /// <summary>
    /// Class managing GroupDataQosPolicy
    /// </summary>
    public class GroupDataQosPolicyConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Configuration key of <see cref="Value"/>
        /// </summary>
        public const string ValuePropertyKey = "datadistributionmanager.opendds.qos.groupdataqospolicy.value";
        /// <summary>
        /// Initialize a new instance of <see cref="GroupDataQosPolicyConfiguration"/>
        /// </summary>
        public GroupDataQosPolicyConfiguration()
        {
        }

        /// <summary>
        /// See OpenDDS documentation at http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public byte[] Value
        {
            get
            {
                string value = string.Empty;
                keyValuePair.TryGetValue(ValuePropertyKey, out value);
                if (string.IsNullOrEmpty(value)) return null;
                List<byte> lst = new List<byte>();
                for (int i = 0; i < value.Length / 2; i++)
                {
                    byte val = Convert.ToByte(value.Substring(i * 2, 2), 16);
                    lst.Add(val);
                }
                return lst.ToArray();
            }
            set
            {
                if (value == null) keyValuePair.Remove(ValuePropertyKey);
                System.Text.StringBuilder builder = new System.Text.StringBuilder();
                for (int i = 0; i < value.Length; i++)
                {
                    builder.Append(value[i].ToString("x2"));
                }
                keyValuePair[ValuePropertyKey] = builder.ToString();
            }
        }
    }
    #endregion

    #region DataWriterQosConfiguration 
    /// <summary>
    /// The configuration class for DataWriterQos
    /// </summary>
    public class DataWriterQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="DataWriterQosConfiguration"/>
        /// </summary>
        public const string DataWriterQosBasePropertyKey = "datadistributionmanager.opendds.datawriterqos";

        /// <summary>
        /// Initialize a <see cref="DataWriterQosConfiguration"/>
        /// </summary>
        public DataWriterQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="DurabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityQosPolicyConfiguration DurabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DurabilityServiceQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityServiceQosPolicyConfiguration DurabilityServiceQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DeadlineQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DeadlineQosPolicyConfiguration DeadlineQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LatencyBudgetQosPolicyConfiguration LatencyBudgetQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LivelinessQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LivelinessQosPolicyConfiguration LivelinessQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ReliabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ReliabilityQosPolicyConfiguration ReliabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DestinationOrderQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DestinationOrderQosPolicyConfiguration DestinationOrderQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="HistoryQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public HistoryQosPolicyConfiguration HistoryQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ResourceLimitsQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ResourceLimitsQosPolicyConfiguration ResourceLimitsQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="TransportPriorityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public TransportPriorityQosPolicyConfiguration TransportPriorityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LifespanQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LifespanQosPolicyConfiguration LifespanQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="UserDataQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public UserDataQosPolicyConfiguration UserDataQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="OwnershipQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public OwnershipQosPolicyConfiguration OwnershipQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="OwnershipStrengthQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public OwnershipStrengthQosPolicyConfiguration OwnershipStrengthQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="WriterDataLifecycleQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public WriterDataLifecycleQosPolicyConfiguration WriterDataLifecycleQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", DataWriterQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (DurabilityQosPolicy != null) lst.AddRange(replacer(DurabilityQosPolicy.Configuration));
            if (DurabilityServiceQosPolicy != null) lst.AddRange(replacer(DurabilityServiceQosPolicy.Configuration));
            if (DeadlineQosPolicy != null) lst.AddRange(replacer(DeadlineQosPolicy.Configuration));
            if (LatencyBudgetQosPolicy != null) lst.AddRange(replacer(LatencyBudgetQosPolicy.Configuration));
            if (LivelinessQosPolicy != null) lst.AddRange(replacer(LivelinessQosPolicy.Configuration));
            if (ReliabilityQosPolicy != null) lst.AddRange(replacer(ReliabilityQosPolicy.Configuration));
            if (DestinationOrderQosPolicy != null) lst.AddRange(replacer(DestinationOrderQosPolicy.Configuration));
            if (HistoryQosPolicy != null) lst.AddRange(replacer(HistoryQosPolicy.Configuration));
            if (ResourceLimitsQosPolicy != null) lst.AddRange(replacer(ResourceLimitsQosPolicy.Configuration));
            if (TransportPriorityQosPolicy != null) lst.AddRange(replacer(TransportPriorityQosPolicy.Configuration));
            if (LifespanQosPolicy != null) lst.AddRange(replacer(LifespanQosPolicy.Configuration));
            if (UserDataQosPolicy != null) lst.AddRange(replacer(UserDataQosPolicy.Configuration));
            if (OwnershipQosPolicy != null) lst.AddRange(replacer(OwnershipQosPolicy.Configuration));
            if (OwnershipStrengthQosPolicy != null) lst.AddRange(replacer(OwnershipStrengthQosPolicy.Configuration));
            if (WriterDataLifecycleQosPolicy != null) lst.AddRange(replacer(WriterDataLifecycleQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region DataReaderQosConfiguration 
    /// <summary>
    /// The configuration class for DataReaderQos
    /// </summary>
    public class DataReaderQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="DataReaderQosConfiguration"/>
        /// </summary>
        public const string DataReaderQosBasePropertyKey = "datadistributionmanager.opendds.datareaderqos";

        /// <summary>
        /// Initialize a <see cref="DataReaderQosConfiguration"/>
        /// </summary>
        public DataReaderQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="DurabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityQosPolicyConfiguration DurabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DeadlineQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DeadlineQosPolicyConfiguration DeadlineQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LatencyBudgetQosPolicyConfiguration LatencyBudgetQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LivelinessQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LivelinessQosPolicyConfiguration LivelinessQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ReliabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ReliabilityQosPolicyConfiguration ReliabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DestinationOrderQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DestinationOrderQosPolicyConfiguration DestinationOrderQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="HistoryQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public HistoryQosPolicyConfiguration HistoryQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ResourceLimitsQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ResourceLimitsQosPolicyConfiguration ResourceLimitsQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="OwnershipQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public OwnershipQosPolicyConfiguration OwnershipQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="TimeBasedFilterQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public TimeBasedFilterQosPolicyConfiguration TimeBasedFilterQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ReaderDataLifecycleQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ReaderDataLifecycleQosPolicyConfiguration ReaderDataLifecycleQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", DataReaderQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (DurabilityQosPolicy != null) lst.AddRange(replacer(DurabilityQosPolicy.Configuration));
            if (DeadlineQosPolicy != null) lst.AddRange(replacer(DeadlineQosPolicy.Configuration));
            if (LatencyBudgetQosPolicy != null) lst.AddRange(replacer(LatencyBudgetQosPolicy.Configuration));
            if (LivelinessQosPolicy != null) lst.AddRange(replacer(LivelinessQosPolicy.Configuration));
            if (ReliabilityQosPolicy != null) lst.AddRange(replacer(ReliabilityQosPolicy.Configuration));
            if (DestinationOrderQosPolicy != null) lst.AddRange(replacer(DestinationOrderQosPolicy.Configuration));
            if (HistoryQosPolicy != null) lst.AddRange(replacer(HistoryQosPolicy.Configuration));
            if (ResourceLimitsQosPolicy != null) lst.AddRange(replacer(ResourceLimitsQosPolicy.Configuration));
            if (OwnershipQosPolicy != null) lst.AddRange(replacer(OwnershipQosPolicy.Configuration));
            if (TimeBasedFilterQosPolicy != null) lst.AddRange(replacer(TimeBasedFilterQosPolicy.Configuration));
            if (ReaderDataLifecycleQosPolicy != null) lst.AddRange(replacer(ReaderDataLifecycleQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region TopicQosConfiguration 
    /// <summary>
    /// The configuration class for TopicQos
    /// </summary>
    public class TopicQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="TopicQosConfiguration"/>
        /// </summary>
        public const string TopicQosBasePropertyKey = "datadistributionmanager.opendds.topicqos";

        /// <summary>
        /// Initialize a <see cref="TopicQosConfiguration"/>
        /// </summary>
        public TopicQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="TopicDataQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public TopicDataQosPolicyConfiguration TopicDataQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DurabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityQosPolicyConfiguration DurabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DurabilityServiceQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DurabilityServiceQosPolicyConfiguration DurabilityServiceQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DeadlineQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DeadlineQosPolicyConfiguration DeadlineQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LatencyBudgetQosPolicyConfiguration LatencyBudgetQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LivelinessQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LivelinessQosPolicyConfiguration LivelinessQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ReliabilityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ReliabilityQosPolicyConfiguration ReliabilityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="DestinationOrderQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DestinationOrderQosPolicyConfiguration DestinationOrderQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="HistoryQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public HistoryQosPolicyConfiguration HistoryQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="ResourceLimitsQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public ResourceLimitsQosPolicyConfiguration ResourceLimitsQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="TransportPriorityQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public TransportPriorityQosPolicyConfiguration TransportPriorityQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LifespanQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public LifespanQosPolicyConfiguration LifespanQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="OwnershipQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public OwnershipQosPolicyConfiguration OwnershipQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", TopicQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (TopicDataQosPolicy != null) lst.AddRange(replacer(TopicDataQosPolicy.Configuration));
            if (DurabilityQosPolicy != null) lst.AddRange(replacer(DurabilityQosPolicy.Configuration));
            if (DurabilityServiceQosPolicy != null) lst.AddRange(replacer(DurabilityServiceQosPolicy.Configuration));
            if (DeadlineQosPolicy != null) lst.AddRange(replacer(DeadlineQosPolicy.Configuration));
            if (LatencyBudgetQosPolicy != null) lst.AddRange(replacer(LatencyBudgetQosPolicy.Configuration));
            if (LivelinessQosPolicy != null) lst.AddRange(replacer(LivelinessQosPolicy.Configuration));
            if (ReliabilityQosPolicy != null) lst.AddRange(replacer(ReliabilityQosPolicy.Configuration));
            if (DestinationOrderQosPolicy != null) lst.AddRange(replacer(DestinationOrderQosPolicy.Configuration));
            if (HistoryQosPolicy != null) lst.AddRange(replacer(HistoryQosPolicy.Configuration));
            if (ResourceLimitsQosPolicy != null) lst.AddRange(replacer(ResourceLimitsQosPolicy.Configuration));
            if (TransportPriorityQosPolicy != null) lst.AddRange(replacer(TransportPriorityQosPolicy.Configuration));
            if (LifespanQosPolicy != null) lst.AddRange(replacer(LifespanQosPolicy.Configuration));
            if (OwnershipQosPolicy != null) lst.AddRange(replacer(OwnershipQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region PublisherQosConfiguration  
    /// <summary>
    /// The configuration class for PublisherQos
    /// </summary>
    public class PublisherQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="PublisherQosConfiguration"/>
        /// </summary>
        public const string PublisherQosBasePropertyKey = "datadistributionmanager.opendds.publisherqos";

        /// <summary>
        /// Initialize a <see cref="PublisherQosConfiguration"/>
        /// </summary>
        public PublisherQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="PresentationQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PresentationQosPolicyConfiguration PresentationQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="PartitionQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PartitionQosPolicyConfiguration PartitionQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="GroupDataQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public GroupDataQosPolicyConfiguration GroupDataQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", PublisherQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (PresentationQosPolicy != null) lst.AddRange(replacer(PresentationQosPolicy.Configuration));
            if (PartitionQosPolicy != null) lst.AddRange(replacer(PartitionQosPolicy.Configuration));
            if (GroupDataQosPolicy != null) lst.AddRange(replacer(GroupDataQosPolicy.Configuration));
            if (EntityFactoryQosPolicy != null) lst.AddRange(replacer(EntityFactoryQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region SubscriberQosConfiguration   
    /// <summary>
    /// The configuration class for SubscriberQos
    /// </summary>
    public class SubscriberQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="SubscriberQosConfiguration"/>
        /// </summary>
        public const string SubscriberQosBasePropertyKey = "datadistributionmanager.opendds.subscriberqos";

        /// <summary>
        /// Initialize a <see cref="SubscriberQosConfiguration"/>
        /// </summary>
        public SubscriberQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="PresentationQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PresentationQosPolicyConfiguration PresentationQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="PartitionQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PartitionQosPolicyConfiguration PartitionQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="GroupDataQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public GroupDataQosPolicyConfiguration GroupDataQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="LatencyBudgetQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", SubscriberQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (PresentationQosPolicy != null) lst.AddRange(replacer(PresentationQosPolicy.Configuration));
            if (PartitionQosPolicy != null) lst.AddRange(replacer(PartitionQosPolicy.Configuration));
            if (GroupDataQosPolicy != null) lst.AddRange(replacer(GroupDataQosPolicy.Configuration));
            if (EntityFactoryQosPolicy != null) lst.AddRange(replacer(EntityFactoryQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region DomainParticipantQosConfiguration   
    /// <summary>
    /// The configuration class for DomainParticipantQos
    /// </summary>
    public class DomainParticipantQosConfiguration : BaseConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="SubscriberQosConfiguration"/>
        /// </summary>
        public const string DomainParticipantQosBasePropertyKey = "datadistributionmanager.opendds.domainparticipantqos";

        /// <summary>
        /// Initialize a <see cref="DomainParticipantQosConfiguration"/>
        /// </summary>
        public DomainParticipantQosConfiguration() { }

        /// <summary>
        /// The configuration of <see cref="UserDataQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public UserDataQosPolicyConfiguration UserDataQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="EntityFactoryQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public EntityFactoryQosPolicyConfiguration EntityFactoryQosPolicy { get; set; }

        /// <summary>
        /// The configuration of <see cref="PropertyQosPolicyConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PropertyQosPolicyConfiguration PropertyQosPolicy { get; set; }

        IEnumerable<string> replacer(IEnumerable<string> input)
        {
            List<string> lst = new List<string>();
            foreach (var item in input)
            {
                lst.Add(item.Replace("datadistributionmanager.opendds.qos", DomainParticipantQosBasePropertyKey));
            }
            return lst;
        }

        /// <summary>
        /// Creates configuration for QoS policies
        /// </summary>
        protected virtual string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(parameters);
            if (UserDataQosPolicy != null) lst.AddRange(replacer(UserDataQosPolicy.Configuration));
            if (EntityFactoryQosPolicy != null) lst.AddRange(replacer(EntityFactoryQosPolicy.Configuration));
            if (PropertyQosPolicy != null) lst.AddRange(replacer(PropertyQosPolicy.Configuration));
            return lst.ToArray();
        }

        /// <inheritdoc/>
        public override string[] Configuration
        {
            get
            {
                return PolicyBuilder(base.Configuration);
            }
        }
    }
    #endregion

    #region OpenDDSChannelConfiguration
    /// <summary>
    /// The configuration class for OpenDDS
    /// </summary>
    public class OpenDDSChannelConfiguration : OpenDDSConfiguration
    {
        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="rootConf"><see cref="IConfiguration"/> to duplicate</param>
        public OpenDDSChannelConfiguration(OpenDDSConfiguration rootConf)
            : base(rootConf)
        {

        }

        /// <summary>
        /// The configuration of <see cref="TopicQosConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public TopicQosConfiguration TopicQos { get; set; }

        /// <summary>
        /// The configuration of <see cref="PublisherQosConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public PublisherQosConfiguration PublisherQos { get; set; }

        /// <summary>
        /// The configuration of <see cref="SubscriberQosConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public SubscriberQosConfiguration SubscriberQos { get; set; }

        /// <summary>
        /// The configuration of <see cref="DataReaderQosConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DataReaderQosConfiguration DataReaderQos { get; set; }

        /// <summary>
        /// The configuration of <see cref="SubscriberQosConfiguration"/>. See http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf
        /// </summary>
        public DataWriterQosConfiguration DataWriterQos { get; set; }

        /// <inheritdoc/>
        protected override string[] PolicyBuilder(string[] parameters)
        {
            List<string> lst = new List<string>(base.PolicyBuilder(parameters));
            if (TopicQos != null) lst.AddRange(TopicQos.Configuration);
            if (PublisherQos != null) lst.AddRange(PublisherQos.Configuration);
            if (SubscriberQos != null) lst.AddRange(SubscriberQos.Configuration);
            if (DataReaderQos != null) lst.AddRange(DataReaderQos.Configuration);
            if (DataWriterQos != null) lst.AddRange(DataWriterQos.Configuration);
            return lst.ToArray();
        }
    }
    #endregion
}
