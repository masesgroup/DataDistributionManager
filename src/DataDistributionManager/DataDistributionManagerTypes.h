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

#if !defined(DATADISTRIBUTIONMANAGERTYPES_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGERTYPES_H__INCLUDED_

/**
* @file DataDistributionManagerTypes.h
* @brief DataDistributionManager library.
*
* DataDistributionManagerTypes.h contains the public types for DataDistributionManager.
* The API is documented in this file as comments prefixing the class,
* function, type, enum, define, etc.
* The C++ interface is STD C++ '03 compliant and adheres to the
* Google C++ Style Guide.
*
* @tableofcontents
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataDistributionManagerEnums.h"

/**@cond NO_DOC*/
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#ifdef DDM_EXPORTS
#define DDM_EXPORT __declspec(dllexport)
#else
#define DDM_EXPORT __declspec(dllimport)
#endif

#define DLLEXPORT DDM_EXPORT
#define DLLIMPORT __declspec(dllimport)
#define DLLCALL __cdecl
#define FUNCALL __cdecl

typedef int BOOL;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

/**@endcond*/


/**
* @brief Definition to avoid timestamp on write
*
*/
#define DDM_NO_TIMESTAMP -1

class IDataDistributionChannel;

/**
* @brief Channel handle definition
*
*/
#define CHANNEL_HANDLE IDataDistributionChannel*
/**
* @brief Channel handle parameter definition
*
*/
#define CHANNEL_HANDLE_PARAMETER CHANNEL_HANDLE channelHandle
/**
* @brief Generic handle definition, used for thread, lock, mutex
*
*/
typedef void* GENERIC_HANDLE;

/**
 * @brief OPERATION_RESULT type.
 * 
 * Errors or conditions from underlying layer
 *
 */
typedef long OPERATION_RESULT;
/**
* @brief Check if operation succeeded
*/
#define OPERATION_SUCCEEDED(or) (((OPERATION_RESULT)(or)) >= 0)
/**
* @brief Check if operation failed
*/
#define OPERATION_FAILED(or) (((OPERATION_RESULT)(or)) < 0)

#define DDM_NO_ERROR_CONDITION 0											/**< No error */

// Condition section
#define DDM_DATA_AVAILABLE 0x20000000										/**< Data are available */
#define DDM_END_OF_STREAM DDM_DATA_AVAILABLE + 1							/**< Reached end of stream */
#define DDM_NO_DATA_RETURNED DDM_DATA_AVAILABLE + 2							/**<  No data returned */
#define DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN DDM_DATA_AVAILABLE + 3	/**< Timeout elapsed waiting for messages from the channel */
#define DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END DDM_DATA_AVAILABLE + 4		/**< End timeout condition waiting for messages from the channel */
#define DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT DDM_DATA_AVAILABLE + 5	/**< Timeout on acknowledgment */

// Errors list
#define DDM_UNMAPPED_ERROR_CONDITION 0xF0000000								/**< Unmapped error */
#define DDM_FATAL_ERROR DDM_UNMAPPED_ERROR_CONDITION + 1					/**< Fatal */
#define DDM_TIMEOUT DDM_UNMAPPED_ERROR_CONDITION + 2						/**< Timeout occurred */
#define DDM_INVALID_DATA DDM_UNMAPPED_ERROR_CONDITION + 3					/**< Invalid data */
#define DDM_SUBSYSTEM_NOT_STARTED DDM_UNMAPPED_ERROR_CONDITION + 4			/**< Subsystem not started */
#define DDM_WRITE_FAILED DDM_UNMAPPED_ERROR_CONDITION + 5					/**< Failed on write */
#define DDM_COMMIT_FAILED DDM_UNMAPPED_ERROR_CONDITION + 6					/**< Commit failed */
#define DDM_POINTER_NOT_SET DDM_UNMAPPED_ERROR_CONDITION + 7				/**< Pointer not set within subsystem */
#define DDM_NOT_IMPLEMENTED DDM_UNMAPPED_ERROR_CONDITION + 8				/**< Function not implemented */
#define DDM_PARAMETER_ERROR DDM_UNMAPPED_ERROR_CONDITION + 9				/**< Errors in parameters */

#endif // !defined(DATADISTRIBUTIONMANAGERTYPES_H__INCLUDED_)
