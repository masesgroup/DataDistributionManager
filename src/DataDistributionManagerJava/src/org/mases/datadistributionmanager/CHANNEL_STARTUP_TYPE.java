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
 * Startup state
 */
public enum CHANNEL_STARTUP_TYPE {
    /**
     * Undefined
     */
    UNDEFINED("UNDEFINED", 0X1),
    /**
     * Created
     */
    CREATED("CREATED", 0X2),
    /**
     * Started
     */
    STARTED("STARTED", 0X4),
    /**
     * Stopped
     */
    STOPPED("STOPPED", 0X8),
    /**
     * Disconnected
     */
    DISCONNECTED("DISCONNECTED", 0X10);

    private static final Map<String, CHANNEL_STARTUP_TYPE> BY_LABEL = new HashMap<>();
    private static final Map<Integer, CHANNEL_STARTUP_TYPE> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (CHANNEL_STARTUP_TYPE e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private CHANNEL_STARTUP_TYPE(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static CHANNEL_STARTUP_TYPE valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static CHANNEL_STARTUP_TYPE valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}