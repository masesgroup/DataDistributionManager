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
 * UnderlyingEvent struct representing the information from low system.
 */
public class UnderlyingEvent {
    UnderlyingEvent(String channelName, OPERATION_RESULT condition, boolean isDataAvailable, String key,
            byte[] buffer, int nativeCode, String subSystemReason) {
        this.ChannelName = channelName;
        this.Condition = condition;
        this.IsDataAvailable = isDataAvailable;
        this.Key = key;
        this.Buffer = buffer;
        this.NativeCode = nativeCode;
        this.SubSystemReason = subSystemReason;
    }

    final String ChannelName;
    final OPERATION_RESULT Condition;
    final boolean IsDataAvailable;
    final String Key;
    final byte[] Buffer;
    final int NativeCode;
    final String SubSystemReason;
}