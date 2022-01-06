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
/**
 * Listener for logs
 */
public interface ILoggingListener {
    /**
     * Emits logs
     * 
     * @param level    {@link DDM_LOG_LEVEL} log level
     * @param source   The log source
     * @param function The function source
     * @param logStr   The log string
     */
    void OnLogging(DDM_LOG_LEVEL level, String source, String function, String logStr);
}
