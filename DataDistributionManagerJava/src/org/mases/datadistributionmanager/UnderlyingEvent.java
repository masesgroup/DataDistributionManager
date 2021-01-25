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

/// <summary>
/// JNI struct for representing the arguments for creating the Java VM.
/// </summary>
public class UnderlyingEvent {
    UnderlyingEvent(String channelName, DDM_UNDERLYING_ERROR_CONDITION condition, boolean isDataAvailable, String key,
            byte[] buffer, int nativeCode, String subSystemReason) {
        this.ChannelName = channelName;
        this.Condition = condition;
        this.IsDataAvailable = isDataAvailable;
        this.Key = key;
        this.Buffer = buffer;
        this.NativeCode = nativeCode;
        this.SubSystemReason = subSystemReason;
    }

    public final String ChannelName;
    public final DDM_UNDERLYING_ERROR_CONDITION Condition;
    public final boolean IsDataAvailable;
    public final String Key;
    public final byte[] Buffer;
    public final int NativeCode;
    public final String SubSystemReason;
}