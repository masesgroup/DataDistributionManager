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

package org.mases.datadistributionmanager;

import java.util.*;

/**
 * State of the instance
 */
public enum DDM_INSTANCE_STATE {
    /**
     * Unknown
     */
    UNKNOWN("UNKNOWN", 0x1),
    /**
     * Primary
     */
    PRIMARY("PRIMARY", 0x2),
    /**
     * Follower
     */
    FOLLOWER("FOLLOWER", 0x3),
    /**
     * Alone
     */
    ALONE("ALONE", 0x4),
    /**
     * Stopping
     */
    STOPPING("STOPPING", 0x5),
    /**
     * Going alone
     */
    GOING_ALONE("GOING_ALONE", 0x6),
    /**
     * Observer
     */
    OBSERVER("OBSERVER", 0x7),
    /**
     * Command sender
     */
    COMMAND("COMMAND", 0x8);

    private static final Map<String, DDM_INSTANCE_STATE> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DDM_INSTANCE_STATE> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DDM_INSTANCE_STATE e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DDM_INSTANCE_STATE(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DDM_INSTANCE_STATE valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DDM_INSTANCE_STATE valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}