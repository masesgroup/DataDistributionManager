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

package org.mases.datadistributionmanager;

import java.util.*;

/**
 * Cluster events
 */
public enum DDM_CLUSTEREVENT {
    /**
     * No event
     */
    NOEVENT("NOEVENT", 0x1),
    /**
     * A server was added in the cluster
     */
    ADDSERVER("ADDSERVER", 0x2),
    /**
     * A server was lost in the cluster
     */
    LOSTSERVER("LOSTSERVER", 0x4),
    /**
     * A server was removed from the cluster
     */
    REMOVESERVER("REMOVESERVER", 0x8);

    private static final Map<String, DDM_CLUSTEREVENT> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DDM_CLUSTEREVENT> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DDM_CLUSTEREVENT e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DDM_CLUSTEREVENT(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DDM_CLUSTEREVENT valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DDM_CLUSTEREVENT valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}