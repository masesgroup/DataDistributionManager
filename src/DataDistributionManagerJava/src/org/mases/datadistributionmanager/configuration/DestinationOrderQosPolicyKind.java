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

import java.util.HashMap;
import java.util.Map;

/**
 * The configuration class for DestinationOrderQosPolicyKind. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public enum DestinationOrderQosPolicyKind {
    /**
     * See <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS("BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS", 0X0),
    /**
     * See <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS("BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS", 0X1);

    private static final Map<String, DestinationOrderQosPolicyKind> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DestinationOrderQosPolicyKind> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DestinationOrderQosPolicyKind e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DestinationOrderQosPolicyKind(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DestinationOrderQosPolicyKind valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DestinationOrderQosPolicyKind valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}
