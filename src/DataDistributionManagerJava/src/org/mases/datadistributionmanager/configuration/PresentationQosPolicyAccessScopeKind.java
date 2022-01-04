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
 * The configuration class for PresentationQosPolicyAccessScopeKind. See
 * <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
 * target=
 * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
 */
public enum PresentationQosPolicyAccessScopeKind {
    /**
     * See <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    INSTANCE_PRESENTATION_QOS("INSTANCE_PRESENTATION_QOS", 0X0),
    /**
     * See <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    TOPIC_PRESENTATION_QOS("TOPIC_PRESENTATION_QOS", 0X1),
    /**
     * See <a href="http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf"
     * target=
     * "_top">http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf</a>
     */
    GROUP_PRESENTATION_QOS("GROUP_PRESENTATION_QOS", 0X2);

    private static final Map<String, PresentationQosPolicyAccessScopeKind> BY_LABEL = new HashMap<>();
    private static final Map<Integer, PresentationQosPolicyAccessScopeKind> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (PresentationQosPolicyAccessScopeKind e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private PresentationQosPolicyAccessScopeKind(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static PresentationQosPolicyAccessScopeKind valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static PresentationQosPolicyAccessScopeKind valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}
