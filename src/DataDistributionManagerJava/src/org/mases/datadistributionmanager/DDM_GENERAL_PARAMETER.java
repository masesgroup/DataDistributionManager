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
 * Direction of the channel
 */
public enum DDM_GENERAL_PARAMETER {
    /**
     * Max message size
     */
    MAX_MESSAGE_SIZE("MAX_MESSAGE_SIZE", 1),
    /**
     * Server lost timeout
     */
    SERVER_LOST_TIMEOUT("SERVER_LOST_TIMEOUT", 2),
    /**
     * Create channel timeout
     */
    CREATE_CHANNEL_TIMEOUT("CREATE_CHANNEL_TIMEOUT", 3),
    /**
     * Seek channel timeout
     */
    CHANNEL_SEEK_TIMEOUT("CHANNEL_SEEK_TIMEOUT", 4),
    /**
     * Timeout on first connection
     */
    FIRST_CONNECTION_TIMEOUT("FIRST_CONNECTION_TIMEOUT", 5),
    /**
     * Timeout of Keep Alive
     */
    KEEP_ALIVE_TIMEOUT("KEEP_ALIVE_TIMEOUT", 6),
    /**
     * Consumer timeout
     */
    CONSUMER_TIMEOUT("CONSUMER_TIMEOUT", 7),
    /**
     * Producer timeout
     */
    PRODUCER_TIMEOUT("PRODUCER_TIMEOUT", 8);

    private static final Map<String, DDM_GENERAL_PARAMETER> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DDM_GENERAL_PARAMETER> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DDM_GENERAL_PARAMETER e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DDM_GENERAL_PARAMETER(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DDM_GENERAL_PARAMETER valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DDM_GENERAL_PARAMETER valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }
}