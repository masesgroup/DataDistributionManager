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

import org.mases.datadistributionmanager.*;

public class MySmartDataDistributionTopic extends SmartDataDistributionChannel {
    public void OnDataAvailable(String key, byte[] buffer) {
        String s = new String(buffer);
        System.out.println(String.format("Channel %s with key %s is saying %s", getChannelName(), key, s));
    }

    public void OnConditionOrError(OPERATION_RESULT errorCode, int nativeCode,
            String subSystemReason) {
        System.out.println(String.format("Channel %s with errorCode %d nativeCode %d subSystemReason %s", getChannelName(),
                (errorCode != null) ? errorCode.getValue() : "", nativeCode, subSystemReason));
    }
}