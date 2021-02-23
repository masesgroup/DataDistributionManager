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

package org.mases.datadistributionmanager.configuration;

import org.mases.datadistributionmanager.BaseConfiguration;
import java.util.ArrayList;

/**
 * The configuration class for GroupDataQosPolicy. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public class GroupDataQosPolicyConfiguration extends BaseConfiguration {
    public static final String ValuePropertyKey = "datadistributionmanager.opendds.qos.groupdataqospolicy.value";

    /**
     * Initialize a new {@link GroupDataQosPolicyConfiguration}. See
     * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    public GroupDataQosPolicyConfiguration() {
    }

    /**
     * See OpenDDS documentation
     * 
     * @return See OpenDDS documentation
     */
    public Byte[] getValue() {
        String value = keyValuePair.get(ValuePropertyKey);
        if (value == null)
            return null;
        if (value == "")
            return new Byte[0];
        ArrayList<Byte> lst = new ArrayList<Byte>();
        for (int i = 0; i < value.length() / 2; i++) {
            Byte val = Byte.valueOf(value.substring(i * 2, 2), 16);
            lst.add(val);
        }
        Byte[] bytes = new Byte[lst.size()];
        return lst.toArray(bytes);
    }

    /**
     * See OpenDDS documentation
     * 
     * @param value See OpenDDS documentation
     */
    public void setValue(Byte[] value) {
        if (value == null)
            keyValuePair.remove(ValuePropertyKey);
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i < value.length; i++) {
            builder.append(String.format("%2x", value[i]));
        }
        keyValuePair.put(ValuePropertyKey, builder.toString());
    }
}
