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
/**
 * Listener for condition or error
 */
public interface IConditionOrErrorListener {
    /**
     * Called when an event condition is raised from subsystem
     * 
     * @param channelName     The channel name
     * @param errorCode       The error code reported
     * @param nativeCode      The native code associated to the error if available
     * @param subSystemReason A String with a reason from subsystem
     */
    void OnConditionOrError(String channelName, DDM_UNDERLYING_ERROR_CONDITION errorCode, int nativeCode,
            String subSystemReason);
}
