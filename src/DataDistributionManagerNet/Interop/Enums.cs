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
    /// <summary>
    /// General configuration parameters
    /// </summary>
    public enum DDM_GENERAL_PARAMETER
    {
        /// <summary>
        /// Max message size
        /// </summary>
        MAX_MESSAGE_SIZE = 0x1,
        /// <summary>
        /// Server lost timeout
        /// </summary>
        SERVER_LOST_TIMEOUT,
        /// <summary>
        /// Create channel timeout
        /// </summary>
        CREATE_CHANNEL_TIMEOUT,
        /// <summary>
        /// Seek channel timeout
        /// </summary>
        CHANNEL_SEEK_TIMEOUT,
        /// <summary>
        /// Timeout on first connection
        /// </summary>
        FIRST_CONNECTION_TIMEOUT,
        /// <summary>
        /// Timeout of Keep Alive
        /// </summary>
        KEEP_ALIVE_TIMEOUT,
        /// <summary>
        /// Consumer timeout
        /// </summary>
        CONSUMER_TIMEOUT,
        /// <summary>
        /// Producer timeout
        /// </summary>
        PRODUCER_TIMEOUT,
    };
    /// <summary>
    /// Direction of the channel
    /// </summary>
    public enum DDM_CHANNEL_DIRECTION
    {
        /// <summary>
        /// Transmitter
        /// </summary>
        TRANSMITTER = 0x1,
        /// <summary>
        /// Receiver
        /// </summary>
        RECEIVER = 0x2,
        /// <summary>
        /// Both
        /// </summary>
        ALL = TRANSMITTER | RECEIVER,
    };
    /// <summary>
    /// Log levels
    /// </summary>
    public enum DDM_LOG_LEVEL
    {
        /// <summary>
        /// Fatal
        /// </summary>
        FATAL_LEVEL = 0x1,
        /// <summary>
        /// Error
        /// </summary>
        ERROR_LEVEL = 0x2,
        /// <summary>
        /// Warning
        /// </summary>
        WARNING_LEVEL = 0x4,
        /// <summary>
        /// Info
        /// </summary>
        INFO_LEVEL = 0x8,
        /// <summary>
        /// Debug
        /// </summary>
        DEBUG_LEVEL = 0x10
    };
    /// <summary>
    /// Errors from undetlying layer
    /// </summary>
    public enum DDM_UNDERLYING_ERROR_CONDITION
    {
        // Error section
        /// <summary>
        /// No error
        /// </summary>
        DDM_NO_ERROR_CONDITION = 0,
        /// <summary>
        /// Unmapped error
        /// </summary>
        DDM_UNMAPPED_ERROR_CONDITION = 0x10000000,
        /// <summary>
        /// Fatal
        /// </summary>
        DDM_FATAL_ERROR,
        /// <summary>
        /// Timeout occurred
        /// </summary>
        DDM_TIMEOUT,
        /// <summary>
        /// Invalid data
        /// </summary>
        DDM_INVALID_DATA,
        /// <summary>
        /// Subsystem not started
        /// </summary>
        DDM_SUBSYSTEM_NOT_STARTED,
        /// <summary>
        /// Failed on write
        /// </summary>
        DDM_WRITE_FAILED,
        /// <summary>
        /// Commit failed
        /// </summary>
        DDM_COMMIT_FAILED,

        // Condition section
        /// <summary>
        /// Data are available
        /// </summary>
        DATA_AVAILABLE = 0x20000000,
        /// <summary>
        /// Reached end of stream
        /// </summary>
        DDM_END_OF_STREAM,
        /// <summary>
        /// No data returned
        /// </summary>
        DDM_NO_DATA_RETURNED,
        /// <summary>
        /// Timeout elapsed waiting for messages from the channel
        /// </summary>
        DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN,
        /// <summary>
        /// End timeout condition waiting for messages from the channel
        /// </summary>
        DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END,
        /// <summary>
        /// Timeout on acknowledgment
        /// </summary>
        DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT,
    };
    /// <summary>
    /// State of the instance
    /// </summary>
    public enum DDM_INSTANCE_STATE
    {
        /// <summary>
        /// Unknown
        /// </summary>
        UNKNOWN,
        /// <summary>
        /// Primary
        /// </summary>
        PRIMARY,
        /// <summary>
        /// Follower
        /// </summary>
        FOLLOWER,
        /// <summary>
        /// Alone
        /// </summary>
        ALONE,
        /// <summary>
        /// Stopping
        /// </summary>
        STOPPING,
        /// <summary>
        /// Going alone
        /// </summary>
        GOING_ALONE,
        /// <summary>
        /// Observer
        /// </summary>
        OBSERVER,
        /// <summary>
        /// Command sender
        /// </summary>
        COMMAND
    };
    /// <summary>
    /// Cluster events
    /// </summary>
    public enum DDM_CLUSTEREVENT
    {
        /// <summary>
        /// No event
        /// </summary>
        NOEVENT = 0x1,
        /// <summary>
        /// A server was added in the cluster
        /// </summary>
        ADDSERVER = 0x2,
        /// <summary>
        /// A server was lost in the cluster
        /// </summary>
        LOSTSERVER = 0x4,
        /// <summary>
        /// A server was removed from the cluster
        /// </summary>
        REMOVESERVER = 0x8
    };
    /// <summary>
    /// Startup state
    /// </summary>
    public enum CHANNEL_STARTUP_TYPE
    {
        /// <summary>
        /// Undefined
        /// </summary>
        UNDEFINED = 0x1,
        /// <summary>
        /// Created
        /// </summary>
        CREATED = 0x2,
        /// <summary>
        /// Started
        /// </summary>
        STARTED = 0X4,
        /// <summary>
        /// Stopped
        /// </summary>
        STOPPED = 0X8,
        /// <summary>
        /// Disconnected
        /// </summary>
        DISCONNECTED = 0x10
    };
}
