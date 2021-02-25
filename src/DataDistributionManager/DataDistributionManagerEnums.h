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

#if !defined(DATADISTRIBUTIONMANAGERENUMS_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERENUMS_H__INCLUDED_

/**
 * @file DataDistributionManagerEnums.h
 * @brief DataDistributionManager library.
 *
 * DataDistributionManagerEnums.h contains the public enums for DataDistributionManager.
 * The API is documented in this file as comments prefixing the class,
 * function, type, enum, define, etc.
 * The C++ interface is STD C++ '03 compliant and adheres to the
 * Google C++ Style Guide.

 * @sa For the C interface see rdkafka.h
 *
 * @tableofcontents
 */

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**@cond NO_DOC*/

#ifndef _INC_WINDOWS
#include <windows.h>
#endif /* _INC_WINDOWS */

/**@endcond*/

#ifdef DDM_EXPORTS
#define DDM_EXPORT __declspec(dllexport)
#else
#define DDM_EXPORT __declspec(dllimport)
#endif

#define DDM_NO_TIMESTAMP -1

/**
 * @enum DDM_GENERAL_PARAMETER
 *
 * @brief DDM_GENERAL_PARAMETER type.
 * 
 * General configuration parameters
 *
 * @sa IDataDistributionChannelBase::SetParameter()
 * @sa IDataDistributionChannelBase::GetParameter()
 */
typedef enum class DDM_GENERAL_PARAMETER
{
	MAX_MESSAGE_SIZE = 0x1, /**< Max message size */
	SERVER_LOST_TIMEOUT , /**< Server lost timeout */
	CREATE_CHANNEL_TIMEOUT , /**< Create channel timeout */
	CHANNEL_SEEK_TIMEOUT , /**< Seek channel timeout */
	FIRST_CONNECTION_TIMEOUT , /**< Timeout on first connection */
	KEEP_ALIVE_TIMEOUT , /**< Timeout of Keep Alive */
	CONSUMER_TIMEOUT , /**< Consumer timeout */
	PRODUCER_TIMEOUT , /**< Producer timeout */
} DDM_GENERAL_PARAMETER;
/**
 * @enum DDM_CHANNEL_DIRECTION
 *
 * @brief DDM_CHANNEL_DIRECTION type.
 * 
 * Direction of the channel
 *
 * @sa IDataDistributionChannelBase::CreateChannel()
 */
typedef enum class DDM_CHANNEL_DIRECTION
{
	TRANSMITTER = 0x1, /**< Transmitter */
	RECEIVER = 0x2, /**< Receiver */
	ALL = TRANSMITTER | RECEIVER, /**< Both */
} DDM_CHANNEL_DIRECTION;
/**
 * @enum DDM_LOG_LEVEL
 *
 * @brief DDM_LOG_LEVEL type.
 * 
 * Log levels
 *
 * @sa dataDistributionLoggingCb
 * @sa IDataDistributionLog::Log()
 * @sa IDataDistributionCallback:OnLogging
 */
typedef enum class DDM_LOG_LEVEL
{ 
	FATAL_LEVEL = 0x1, /**< Fatal */
	ERROR_LEVEL = 0x2, /**< Error */
	WARNING_LEVEL = 0x4, /**< Warning */
	INFO_LEVEL = 0x8, /**< Info */
	DEBUG_LEVEL = 0x10, /**< Debug */
} DDM_LOG_LEVEL;
/**
 * @enum DDM_INSTANCE_STATE
 *
 * @brief DDM_INSTANCE_STATE type.
 * 
 * State of the instance
 *
 * @sa ClusterHealthElement::Status
 * @sa dataDistributionOnStateChange
 * @sa dataDistributionFirstStateChange
 * @sa dataDistributionChangingState
 * @sa dataDistributionChangedState
 * @sa IDataDistributionMastershipCallback::OnStateChange()
 * @sa IDataDistributionMastershipCallback::FirstStateChange()
 * @sa IDataDistributionMastershipCallback::ChangingState()
 * @sa IDataDistributionMastershipCallback::ChangedState()
 */
typedef enum class DDM_INSTANCE_STATE
{ 
	UNKNOWN, /**< Unknown */
	PRIMARY, /**< Primary */
	FOLLOWER, /**< Follower */
	ALONE, /**< Alone */
	STOPPING, /**< Stopping */
	GOING_ALONE, /**< Going alone */
	OBSERVER, /**< Observer */
	COMMAND /**< Command sender */
} DDM_INSTANCE_STATE;
/**
 * @enum DDM_CLUSTEREVENT
 *
 * @brief DDM_CLUSTEREVENT type.
 * 
 * Cluster events
 *
 * @sa dataDistributionOnClusterStateChange
 * @sa IDataDistributionMastershipCallback::OnClusterStateChange()
 */
typedef enum class DDM_CLUSTEREVENT
{ 
	NOEVENT = 0x1, /**< No event */
	ADDSERVER = 0x2, /**< A server was added in the cluster */
	LOSTSERVER = 0x4, /**< A server was lost in the cluster */
	REMOVESERVER = 0x8 /**< A server was removed from the cluster */
} DDM_CLUSTEREVENT;
/**
 * @enum DDM_UNDERLYING_ERROR_CONDITION
 *
 * @brief DDM_UNDERLYING_ERROR_CONDITION type.
 * 
 * Errors or conditions from underlying layer
 *
 * @sa dataDistributionOnClusterStateChange
 * @sa IDataDistributionMastershipCallback::OnClusterStateChange()
 */
typedef enum class DDM_UNDERLYING_ERROR_CONDITION
{
	// Error section
	DDM_NO_ERROR_CONDITION = 0, /**< No error */

	DDM_UNMAPPED_ERROR_CONDITION = 0x10000000, /**< Unmapped error */
	DDM_FATAL_ERROR, /**< Fatal */
	DDM_TIMEOUT, /**< Timeout occurred */
	DDM_INVALID_DATA, /**< Invalid data */
	DDM_SUBSYSTEM_NOT_STARTED, /**< Subsystem not started */
	DDM_WRITE_FAILED, /**< Failed on write */
	DDM_COMMIT_FAILED, /**< Commit failed */

	// Condition section
	DATA_AVAILABLE = 0x20000000, /**< Data are available */
	DDM_END_OF_STREAM, /**< Reached end of stream */
	DDM_NO_DATA_RETURNED, /**<  No data returned */
	DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN, /**< Timeout elapsed waiting for messages from the channel */
	DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END, /**< End timeout condition waiting for messages from the channel */
	DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT, /**< Timeout on acknowledgment */
} DDM_UNDERLYING_ERROR_CONDITION;

#endif // !defined(DATADISTRIBUTIONMANAGERENUMS_H__INCLUDED_)
