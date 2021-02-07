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
 * Log levels
 */
public enum DDM_LOG_LEVEL {
    /**
     * Fatal
     */
    FATAL_LEVEL("FATAL_LEVEL", 0x1),
    /**
     * Error
     */
    ERROR_LEVEL("ERROR_LEVEL", 0x2),
    /**
     * Warning
     */
    WARNING_LEVEL("WARNING_LEVEL", 0x4),
    /**
     * Info
     */
    INFO_LEVEL("INFO_LEVEL", 0x8),
    /**
     * Debug
     */
    DEBUG_LEVEL("DEBUG_LEVEL", 0x10);

    private static final Map<String, DDM_LOG_LEVEL> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DDM_LOG_LEVEL> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DDM_LOG_LEVEL e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DDM_LOG_LEVEL(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DDM_LOG_LEVEL valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DDM_LOG_LEVEL valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }
}