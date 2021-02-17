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
 * Errors from undetlying layer
 */
public enum DDM_UNDERLYING_ERROR_CONDITION {
    /**
     * No error
     */
    DDM_NO_ERROR_CONDITION("DDM_NO_ERROR_CONDITION", 0),
    /**
     * Unmapped error
     */
    DDM_UNMAPPED_ERROR_CONDITION("DDM_UNMAPPED_ERROR_CONDITION", 0x10000000),
    /**
     * Fatal
     */
    DDM_FATAL_ERROR("DDM_FATAL_ERROR", 0x10000001),
    /**
     * Timeout occurred
     */
    DDM_TIMEOUT("DDM_TIMEOUT", 0x10000002),
    /**
     * Invalid data
     */
    DDM_INVALID_DATA("DDM_INVALID_DATA", 0x10000003),
    /**
     * Subsystem not started
     */
    DDM_SUBSYSTEM_NOT_STARTED("DDM_SUBSYSTEM_NOT_STARTED", 0x10000004),
    /**
     * Write failed
     */
    DDM_WRITE_FAILED("DDM_WRITE_FAILED", 0x10000005),
    /**
     * Commit failed
     */
    DDM_COMMIT_FAILED("DDM_COMMIT_FAILED", 0x10000006),
    // Condition section
    /**
     * Data are available
     */
    DATA_AVAILABLE("DATA_AVAILABLE", 0x20000000),
    /**
     * Reached end of stream
     */
    DDM_END_OF_STREAM("DDM_END_OF_STREAM", 0x20000001),
    /**
     * No data returned
     */
    DDM_NO_DATA_RETURNED("DDM_NO_DATA_RETURNED", 0x20000002),
    /**
     * Timeout elapsed waiting for messages from the channel
     */
    DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN("DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN", 0x20000003),
    /**
     * End timeout condition waiting for messages from the channel
     */
    DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END("DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END", 0x20000004),
    /**
     * Timeout on acknowledgment
     */
    DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT("DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT", 0x20000005);

    private static final Map<String, DDM_UNDERLYING_ERROR_CONDITION> BY_LABEL = new HashMap<>();
    private static final Map<Integer, DDM_UNDERLYING_ERROR_CONDITION> BY_ATOMIC_NUMBER = new HashMap<>();

    static {
        for (DDM_UNDERLYING_ERROR_CONDITION e : values()) {
            BY_LABEL.put(e.label, e);
            BY_ATOMIC_NUMBER.put(e.atomicNumber, e);
        }
    }

    public final String label;
    public final int atomicNumber;

    private DDM_UNDERLYING_ERROR_CONDITION(String label, int atomicNumber) {
        this.label = label;
        this.atomicNumber = atomicNumber;
    }

    // ... fields, constructor, methods

    public static DDM_UNDERLYING_ERROR_CONDITION valueOfLabel(String label) {
        return BY_LABEL.get(label);
    }

    public static DDM_UNDERLYING_ERROR_CONDITION valueOfAtomicNumber(int number) {
        return BY_ATOMIC_NUMBER.get(number);
    }

    public String toIntString() {
        return Integer.toString(atomicNumber);
    }
}