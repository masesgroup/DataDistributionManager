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

import org.mases.datadistributionmanager.BaseConfiguration;

import java.util.ArrayList;

/**
 * The configuration class for PropertyQosPolicy. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class PropertyQosPolicyConfiguration extends BaseConfiguration {
        /**
         * The configuration class for {@link Property}. See
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         */
        public class Property {
                /**
                 * Initialize a new {@link Property}. See
                 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 * target=
                 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 */
                public Property() {
                }

                /**
                 * Initialize a new {@link Property}. See
                 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 * target=
                 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 * 
                 * @param value See OpenDDS documentation at <a href=
                 *              "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 *              target=
                 *              "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 */
                public Property(String value) {
                        Value = value;
                        Propagate = false;
                }

                /**
                 * Initialize a new {@link Property}. See
                 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 * target=
                 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 * 
                 * @param value     See OpenDDS documentation at <a href=
                 *                  "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 *                  target=
                 *                  "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 * @param propagate See OpenDDS documentation at <a href=
                 *                  "http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 *                  target=
                 *                  "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 */
                public Property(String value, Boolean propagate) {
                        Value = value;
                        Propagate = propagate;
                }

                /**
                 * See OpenDDS documentation at
                 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 * target=
                 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 * 
                 */
                public String Value;
                /**
                 * See OpenDDS documentation at
                 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
                 * target=
                 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
                 * 
                 */
                public Boolean Propagate;
        }

        public static final String PropertyQosPolicyValuePropertyKey = "datadistributionmanager.opendds.qos.propertyqospolicy.value";
        public static final String PropertyQosPolicyBinaryValuePropertyKey = "datadistributionmanager.opendds.qos.propertyqospolicy.binary_value";

        /**
         * Initialize a new {@link PropertyQosPolicyConfiguration}. See
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         */
        public PropertyQosPolicyConfiguration() {
        }

        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_IDENTITY_CA;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_IDENTITY_CERT;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_IDENTITY_PRIVKEY;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_IDENTITY_PRIVKEY_PWD;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_PERM_CA;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_PERM_GOV_DOC;
        /**
         * See OpenDDS documentation at
         * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
         * target=
         * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
         * 
         */
        public Property DDSSEC_PROP_PERM_DOC;

        public String[] getConfiguration() {
                if (DDSSEC_PROP_IDENTITY_CA == null || DDSSEC_PROP_IDENTITY_CERT == null
                                || DDSSEC_PROP_IDENTITY_PRIVKEY == null || DDSSEC_PROP_PERM_CA == null
                                || DDSSEC_PROP_PERM_GOV_DOC == null || DDSSEC_PROP_PERM_DOC == null) {
                        throw new IllegalArgumentException(
                                        "Only DDSSEC_PROP_IDENTITY_PRIVKEY_PWD can be null in PropertyQosPolicyConfiguration");
                }

                StringBuilder sb = new StringBuilder();
                sb.append(String.format("%s;%s;%s", "dds.sec.auth.identity_ca", DDSSEC_PROP_IDENTITY_CA.Value,
                                DDSSEC_PROP_IDENTITY_CA.Propagate.toString().toLowerCase()));
                sb.append("|");
                sb.append(String.format("%s;%s;%s", "dds.sec.auth.identity_certificate",
                                DDSSEC_PROP_IDENTITY_CERT.Value,
                                DDSSEC_PROP_IDENTITY_CERT.Propagate.toString().toLowerCase()));
                sb.append("|");
                sb.append(String.format("%s;%s;%s", "dds.sec.auth.private_key", DDSSEC_PROP_IDENTITY_PRIVKEY.Value,
                                DDSSEC_PROP_IDENTITY_PRIVKEY.Propagate.toString().toLowerCase()));
                sb.append("|");
                if (DDSSEC_PROP_IDENTITY_PRIVKEY_PWD != null) {
                        sb.append(String.format("%s;%s;%s", "dds.sec.auth.password",
                                        DDSSEC_PROP_IDENTITY_PRIVKEY_PWD.Value,
                                        DDSSEC_PROP_IDENTITY_PRIVKEY_PWD.Propagate.toString().toLowerCase()));
                        sb.append("|");
                }
                sb.append(String.format("%s;%s;%s", "dds.sec.access.permissions_ca", DDSSEC_PROP_PERM_CA.Value,
                                DDSSEC_PROP_PERM_CA.Propagate.toString().toLowerCase()));
                sb.append("|");
                sb.append(String.format("%s;%s;%s", "dds.sec.access.governance", DDSSEC_PROP_PERM_GOV_DOC.Value,
                                DDSSEC_PROP_PERM_GOV_DOC.Propagate.toString().toLowerCase()));
                sb.append("|");
                sb.append(String.format("%s;%s;%s", "dds.sec.access.permissions", DDSSEC_PROP_PERM_DOC.Value,
                                DDSSEC_PROP_PERM_DOC.Propagate.toString().toLowerCase()));
                sb.append("|");

                ArrayList<String> lst = new ArrayList<String>();
                lst.add(String.format("%s=%s", PropertyQosPolicyValuePropertyKey, sb.toString()));
                String[] array = new String[lst.size()];
                return lst.toArray(array);
        }
}
