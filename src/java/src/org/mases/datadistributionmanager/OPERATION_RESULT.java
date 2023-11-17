/*
*  Copyright 2023 MASES s.r.l
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

/**
 * Java mapper of {@link OPERATION_RESULT}
 */
public class OPERATION_RESULT {
    static OPERATION_RESULT fromCondition(int condition) {
        return new OPERATION_RESULT(condition);
    }

    public static final OPERATION_RESULT S_OK = new OPERATION_RESULT(0);
    public static final OPERATION_RESULT E_FAIL = new OPERATION_RESULT(0x80004005);

    final long m_value;

    OPERATION_RESULT(int value) {
        m_value = value;
    }

    OPERATION_RESULT(long value) {
        m_value = value;
    }

    public boolean getFailed() {
        return m_value < 0;
    }

    public boolean getSuccess() {
        return m_value >= 0;
    }

    public long getValue() {
        return m_value;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }

        if (!OPERATION_RESULT.class.isAssignableFrom(obj.getClass())) {
            return false;
        }

        final OPERATION_RESULT other = (OPERATION_RESULT) obj;
        if (this.m_value == other.m_value) {
            return true;
        }

        return false;
    }

    @Override
    public int hashCode() {
        return (int) m_value;
    }
}