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

interface IDataDistributionMastershipCallbackLow {
        void OnClusterStateChange(long opaque, int change, long serverid);

        void OnStateChange(long opaque, int newState, int oldState);

        void OnStateReady(long opaque, byte[] pState);

        void OnRequestedState(long opaque, byte[] pState);

        void OnMultiplePrimary(long opaque, long myId, long otherId);

        void FirstStateChange(long opaque, int newState);

        void ChangingState(long opaque, int oldState, int newState);

        void ChangedState(long opaque, int newState);
}