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

namespace MASES.DataDistributionManager.Bindings.Interop
{
    public enum DDM_GENERAL_PARAMETER
    {
        MAX_MESSAGE_SIZE = 0x1,
        SERVER_LOST_TIMEOUT,
        CREATE_TOPIC_TIMEOUT,
        TOPIC_SEEK_TIMEOUT,
        FIRST_CONNECTION_TIMEOUT,
        KEEP_ALIVE_TIMEOUT,
        CONSUMER_TIMEOUT,
        PRODUCER_TIMEOUT,
    };

    public enum DDM_CHANNEL_DIRECTION
    {
        TRANSMITTER = 0x1,
        RECEIVER = 0x2,
        ALL = TRANSMITTER | RECEIVER,
    };

    public enum DDM_LOG_LEVEL
    {
        FATAL_LEVEL = 0x1,
        ERROR_LEVEL = 0x2,
        WARNING_LEVEL = 0x4,
        INFO_LEVEL = 0x8,
        DEBUG_LEVEL = 0x10
    };

    public enum DDM_UNDERLYING_ERROR_CONDITION
    {
        // Error section
        DDM_NO_ERROR_CONDITION = 0,

        DDM_UNMAPPED_ERROR_CONDITION = 0x10000000,
        DDM_FATAL_ERROR,
        DDM_TIMEOUT,
        DDM_INVALID_DATA,

        // Condition section
        DATA_AVAILABLE = 0x20000000,
        DDM_END_OF_STREAM,
        DDM_NO_DATA_RETURNED,
    };

    public enum DDM_INSTANCE_STATE
    {
        UNKNOWN,
        PRIMARY,
        FOLLOWER,
        ALONE,
        STOPPING,
        GOING_ALONE,
        OBSERVER,
        COMMAND
    };
    public enum DDM_CLUSTEREVENT
    {
        NOEVENT = 0x1,
        ADDSERVER = 0x2,
        LOSTSERVER = 0x4,
        REMOVESERVER = 0x8
    };

    public enum CHANNEL_STARTUP_TYPE
    {
        UNDEFINED = 0x1,
        CREATED = 0x2,
        STARTED = 0X4,
        STOPPED = 0X8,
        DISCONNECTED = 0x10
    };
}
