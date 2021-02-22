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

import java.util.HashMap;
import java.util.Map;

public enum DurabilityQosPolicyKind {
    VOLATILE_DURABILITY_QOS("VOLATILE_DURABILITY_QOS", 0X0),
    TRANSIENT_LOCAL_DURABILITY_QOS("TRANSIENT_LOCAL_DURABILITY_QOS", 0X1),
    TRANSIENT_DURABILITY_QOS("TRANSIENT_DURABILITY_QOS", 0X2),
    PERSISTENT_DURABILITY_QOS("PERSISTENT_DURABILITY_QOS", 0X3);

    private static final Map<String, DurabilityQosPolicyKind> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DurabilityQosPolicyKind> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DurabilityQosPolicyKind e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DurabilityQosPolicyKind(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DurabilityQosPolicyKind valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DurabilityQosPolicyKind valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}
