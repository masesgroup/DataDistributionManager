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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _INC_WINDOWS
#include <windows.h>
#endif /* _INC_WINDOWS */

typedef enum class _DDM_GENERAL_PARAMETER
{
	MAX_MESSAGE_SIZE = 0x1,
	SERVER_LOST_TIMEOUT ,
	CREATE_CHANNEL_TIMEOUT ,
	CHANNEL_SEEK_TIMEOUT ,
	FIRST_CONNECTION_TIMEOUT ,
	KEEP_ALIVE_TIMEOUT ,
	CONSUMER_TIMEOUT ,
	PRODUCER_TIMEOUT ,
} DDM_GENERAL_PARAMETER;

typedef enum class _DDM_DDM_KEEPALIVE_TYPE
{
	ALIVE = 0x1,
	HELLO = 0x2,
	WELCOME = 0x4,
	GOODBYE = 0x8,
	STATECHANGEREQUEST = 0x10,
	STATECHANGERESPONSE = 0x20,
} DDM_KEEPALIVE_TYPE;

typedef enum class _DDM_CHANNEL_DIRECTION
{
	TRANSMITTER = 0x1,
	RECEIVER = 0x2,
	ALL = TRANSMITTER | RECEIVER,
} DDM_CHANNEL_DIRECTION;

typedef enum class _DDM_LOG_LEVEL
{ 
	FATAL_LEVEL = 0x1,
	ERROR_LEVEL = 0x2, 
	WARNING_LEVEL = 0x4, 
	INFO_LEVEL = 0x8, 
	DEBUG_LEVEL = 0x10,
} DDM_LOG_LEVEL;

typedef enum class _DDM_INSTANCE_STATE
{ 
	UNKNOWN, 
	PRIMARY, 
	FOLLOWER, 
	ALONE, 
	STOPPING, 
	GOING_ALONE, 
	OBSERVER, 
	COMMAND 
} DDM_INSTANCE_STATE;

typedef enum class _DDM_CLUSTEREVENT
{ 
	NOEVENT = 0x1, 
	ADDSERVER = 0x2,
	LOSTSERVER = 0x4, 
	REMOVESERVER = 0x8 
} DDM_CLUSTEREVENT;

typedef enum class _CHANNEL_STARTUP_TYPE
{
	UNDEFINED = 0x1,
	CREATED = 0x2,
	STARTED = 0X4,
	STOPPED = 0X8,
	DISCONNECTED = 0x10
} CHANNEL_STARTUP_TYPE;

typedef enum class _DDM_UNDERLYING_ERROR_CONDITION
{
	// Error section
	DDM_NO_ERROR_CONDITION = 0,

	DDM_UNMAPPED_ERROR_CONDITION = 0x10000000,
	DDM_FATAL_ERROR,
	DDM_TIMEOUT,
	DDM_INVALID_DATA,
	DDM_SUBSYSTEM_NOT_STARTED,

	// Condition section
	DATA_AVAILABLE = 0x20000000,
	DDM_END_OF_STREAM,
	DDM_NO_DATA_RETURNED,
	DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN,
	DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END,
} DDM_UNDERLYING_ERROR_CONDITION;

#endif // !defined(DATADISTRIBUTIONMANAGERENUMS_H__INCLUDED_)
