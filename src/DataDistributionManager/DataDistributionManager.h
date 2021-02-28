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

#if !defined(DATADISTRIBUTIONMANAGER_H__INCLUDED_)
#define DATADISTRIBUTIONMANAGER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @file DataDistributionManager.h
 * @brief DataDistributionManager library.
 *
 * DataDistributionManager.h contains the public C++ API for DataDistributionManager.
 * The API is documented in this file as comments prefixing the class,
 * function, type, enum, define, etc.
 * The C++ interface is STD C++ '03 compliant and adheres to the
 * Google C++ Style Guide.
 *
 * @tableofcontents
 */

/**@cond NO_DOC*/
#include <stdint.h>
#include <map>

#ifndef _INC_WINDOWS
#include <windows.h>
#endif /* _INC_WINDOWS */

/**@endcond*/

#include "DataDistributionManagerEnums.h"

/**
 * @brief C-style callback invoked for each key-value in the configuration.
 *
 * C-style callback usable to override configuration parameters in some special conditions.
 * \p channelName reports the channel involved (null for global parameters),
 * \p key is the parameter key to be checked and \p value is the actual value.
 * @return \p value if no change is needed, otherwise any new string value according to the expected parameter type
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionCallback::create().
 *
 */
typedef const char *(__cdecl *dataDistributionConfigurationCb)(const void *opaque, const char *channelName, const char *key, const char *value);
/**
 * @brief C-style callback invoked when a log is emitted from subsystem.
 *
 * C-style callback usable to receive log information.
 * \p level is the DDM_LOG_LEVEL of the log reported
 * \p source is the module source name of the log
 * \p function is the function, within the module, reporting the log
 * @return \p logStr is the reported log string
 *
 * @sa IDataDistributionLog::Log
 *
 * The callback's \p opaque argument is the opaque set with DataDistributionCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionLoggingCb)(const void *opaque, const DDM_LOG_LEVEL level, const char *source, const char *function, const char *logStr);
/**
 * @brief C-style callback invoked on a complete disconnection .
 *
 * C-style callback invoked when subsystem detects a complete disconnection from central server or other peers.
 * \p channelName reports the disconnected channel
 * \p reason is the reason of disconnection.
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionCompletelyDisconnectedCb)(const void *opaque, const char *channelName, const char *reason);

/**
 * @brief Contains information needed from the cluster management
 *
 * Contains information needed from the cluster management
 */
struct ClusterHealthElement
{
	int64_t ServerId;		   /*!< The server id of the instance */
	DDM_INSTANCE_STATE Status; /*!< The DDM_INSTANCE_STATE status of the instance */
	int64_t Uptime;			   /*!< The uptime reported from the instance */
	int64_t LastContactTime;   /*!< Last time the server contacted this instance */
};

/* Forward declarations */
class IDataDistributionMastershipCallback;
class IDataDistributionMastershipCommon;
class IDataDistributionSubsystem;

/**
 * @brief Interface to manage time information
 *
 * A manager for timing
 */
class DDM_EXPORT ITimeMeasureWrapper
{
public:
	/**
   * @brief Reset the timer
   *
   * Reset the timer information to zero
   *
   */
	virtual void ResetTime() = 0;
	/**
	 * @brief Reports the elapsed time in milliseconds
	 *
	 * Use to function to get the elapsed time in milliseconds
	 *
	 * @returns the elapsed time in milliseconds
	 */
	virtual int64_t ElapsedMilliseconds() = 0;
	/**
	 * @brief Reports the elapsed time in microseconds
	 *
	 * Use to function to get the elapsed time in microseconds
	 *
	 * @returns the elapsed time in microseconds
	 */
	virtual int64_t ElapsedMicroseconds() = 0;
	/**
	 * @brief Reports the elapsed time in nanoseconds
	 *
	 * Use to function to get the elapsed time in nanoseconds
	 *
	 * @returns the elapsed time in nanoseconds
	 */
	virtual int64_t ElapsedNanoseconds() = 0;
	/**
	 * @brief Adds nanoseconds time to actual value
	 *
	 * Use to function to add \p time nanoseconds to current value
	 * 
	 * \p time the nanoseconds to be added
	 * 
	 */
	virtual void AddNanoseconds(unsigned int time) = 0;
};

/**
 * @brief Class to allocate an ITimeMeasureWrapper
 *
 * Class to allocate an ITimeMeasureWrapper
 */
class DDM_EXPORT TimeMeasureWrapper
{
public:
	/**
   * @brief Creates a new ITimeMeasureWrapper instance.
   *
   * @returns the new ITimeMeasureWrapper
   */
	static ITimeMeasureWrapper *create();
};

/**
 * @brief Interface to reports logging
 *
 * Logging reporting base interface
 */
class DDM_EXPORT IDataDistributionLog
{
public:
	/**
   * @brief Reports log information
   *
   * This interface is implemented in modules to report log information
   * \p level is the DDM_LOG_LEVEL of the log reported
   * \p sourceName is the module source name of the log
   * \p function is the function, within the module, reporting the log
   * \p format is the format string to be filled with parameters
   */
	virtual void Log(const DDM_LOG_LEVEL level, const char *sourceName, const char *function, const char *format, ...) = 0;
	/**
   * @brief Reports log information
   *
   * This interface is implemented in modules to report log information
   * \p level is the DDM_LOG_LEVEL of the log reported
   * \p sourceName is the module source name of the log
   * \p function is the function, within the module, reporting the log
   * \p format is the format string to be filled with parameters
   * \p args are the arguments for the \p format
   */
	virtual void Log(const DDM_LOG_LEVEL level, const char *sourceName, const char *function, const char *format, va_list args) = 0;
};

/**
 * @brief The C++ callback interface to be externally implemented
 *
 * Implement this interface in an external class and pass the pointer to IDataDistribution::Initialize. The external class will be called conseguently.
 *
 * @sa IDataDistribution::Initialize
 * @sa IDataDistributionSubsystem::Initialize
 * @sa IDataDistributionSubsystem::GetCallbacks
 */
class DDM_EXPORT IDataDistributionCallback
{
public:
	/**
	* @brief Invoked during configuration validation
	*
	* The callback is usable to override configuration parameters in some special conditions.
	* \p channelName reports the channel involved (null for global parameters),
	* \p key is the parameter key to be checked and \p value is the actual value.
	* @return \p value if no change is needed, otherwise any new string value according to the expected parameter type
	*/
	virtual const char *OnConfiguration(const char *channelName, const char *key, const char *value) = 0;
	/**
	 * @brief Callback invoked when a log is emitted from subsystem.
	 *
	 * The callback is usable to receive log information.
	 * \p level is the DDM_LOG_LEVEL of the log reported
	 * \p source is the module source name of the log
	 * \p function is the function, within the module, reporting the log
	 * @return \p logStr is the reported log string
	 *
	 * @sa IDataDistributionLog::Log
	 *
	 */
	virtual void OnLogging(DDM_LOG_LEVEL level, const char *source, const char *function, const char *logStr) = 0;
	/**
	 * @brief Callback invoked on a complete disconnection.
	 *
	 * Callback invoked when subsystem detects a complete disconnection from central server or other peers.
	 * \p channelName reports the disconnected channel
	 * \p reason is the reason of disconnection.
	 *
	 */
	virtual void OnCompletelyDisconnected(const char *channelName, const char *reason) = 0;
};

/**
 * @brief Class to convert C style callback into C++ callback
 *
 * This class allocates an IDataDistributionCallback with point to C-style callback
 */
class DDM_EXPORT DataDistributionCallback : public IDataDistributionCallback
{
public:
	/**
   * @brief Creates a new IDataDistributionCallback instance.
   *
   * \p opaque user pointer sent back on C-style callback invocation
   * \p confCb C-style dataDistributionConfigurationCb callback
   * \p logCb C-style dataDistributionLoggingCb callback
   * \p cbCb C-style dataDistributionCompletelyDisconnectedCb callback
   * 
   * @returns the new IDataDistributionCallback
   */
	static IDataDistributionCallback *create(void *opaque, dataDistributionConfigurationCb confCb, dataDistributionLoggingCb logCb, dataDistributionCompletelyDisconnectedCb cbCb);
};

/**
 * @brief Class to reports information from transport subsystem
 *
 * This class allocates an UnderlyingEventData with information from subsystem
 */
typedef struct DDM_EXPORT UnderlyingEventData
{
	const char *ChannelName;				  /*!< Channel name associated to UnderlyingEventData */
	BOOL IsDataAvailable;					  /*!< TRUE if data are available, otherwise FALSE */
	const char *Key;						  /*!< The key of the message associated to UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */
	size_t KeyLen;							  /*!< The length of Key associated to UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */
	const void *Buffer;						  /*!< The buffer of the message associated to UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */
	size_t BufferLength;					  /*!< The length of the Buffer associated to UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */
	DDM_UNDERLYING_ERROR_CONDITION Condition; /*!< DDM_UNDERLYING_ERROR_CONDITION associated to UnderlyingEventData. @remarks Valid only if IsDataAvailable is FALSE */
	int NativeCode;							  /*!< Native code from subsystem associated to Condition of UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */
	const char *SubSystemReason;			  /*!< String reason from subsystem associated to NativeCode of UnderlyingEventData. @remarks Valid only if IsDataAvailable is TRUE */

	/**
	 * @brief Initialize a new UnderlyingEventData
	 *
	 * \p channelName The channel name reporting UnderlyingEventData
	 * 
	 * @remarks it used internally
	 * 
	 * Initialize a new UnderlyingEventData
	 */
	UnderlyingEventData(const char *channelName)
	{
		ChannelName = channelName;
		Condition = DDM_UNDERLYING_ERROR_CONDITION::DDM_NO_ERROR_CONDITION;
		IsDataAvailable = FALSE;
		Key = NULL;
		KeyLen = 0;
		Buffer = NULL;
		BufferLength = 0;
		NativeCode = 0;
		SubSystemReason = NULL;
	}
	/**
	 * @brief Initialize a new UnderlyingEventData to reports data
	 * 
	 * Initialize a new UnderlyingEventData
	 * 
	 * \p channelName The channel name reporting UnderlyingEventData
	 * \p key the key associated to the message received
	 * \p keyLen of the \p key associated to the message received
	 * \p buffer the buffer associated to the message received
	 * \p bufferLength of the \p buffer associated to the message received
	 * 
	 * @remarks it used internally
	 */
	UnderlyingEventData(const char *channelName, const char *key, size_t keyLen, const void *buffer, size_t bufferLength) : UnderlyingEventData(channelName)
	{
		Key = key;
		KeyLen = keyLen;
		Buffer = buffer;
		BufferLength = bufferLength;
		IsDataAvailable = TRUE;
	}
	/**
	 * @brief Initialize a new UnderlyingEventData to reports errors or conditions
	 * 
	 * Initialize a new UnderlyingEventData
	 *
	 * \p channelName The channel name reporting UnderlyingEventData
	 * \p condition the DDM_UNDERLYING_ERROR_CONDITION reported
	 * \p nativeCode Native code from subsystem associated to \p condition 
	 * \p subSystemReason string reason from subsystem associated to \p nativeCode
	 * 
	 * @remarks it used internally
	 *
	 */
	UnderlyingEventData(const char *channelName, DDM_UNDERLYING_ERROR_CONDITION condition, int nativeCode, const char *subSystemReason) : UnderlyingEventData(channelName)
	{
		Condition = condition;
		NativeCode = nativeCode;
		SubSystemReason = subSystemReason;
	}
} UnderlyingEventData, *pUnderlyingEventData;

/**
 * @brief C-style callback invoked to report UnderlyingEventData.
 *
 * C-style callback invoked when subsystem needs to report an UnderlyingEventData.
 * \p channelHandle reports the channel handle source of UnderlyingEventData
 * \p uEvent the UnderlyingEventData event
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionChannelCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionUnderlyingEvent)(const void *opaque, const HANDLE channelHandle, const UnderlyingEventData *uEvent);

/**
 * @brief The C++ callback interface to be externally implemented
 *
 * Implement this interface in an external class and pass the pointer to IDataDistributionChannelBase::CreateChannel. The external class will be called conseguently.
 *
 * @sa IDataDistributionChannelBase::CreateChannel
 */
class DDM_EXPORT IDataDistributionChannelCallback
{
public:
	/**
	 * @brief Callback invoked to report UnderlyingEventData.
	 *
	 * Callback invoked when subsystem needs to report an UnderlyingEventData.
	 * 
	 * \p channelHandle reports the channel handle source of UnderlyingEventData
	 * \p uEvent the UnderlyingEventData event
	 *
	 */
	virtual void OnUnderlyingEvent(const HANDLE channelHandle, const UnderlyingEventData *uEvent) = 0;
};

/**
 * @brief Class to convert C style dataDistributionUnderlyingEvent callback into IDataDistributionChannelCallback C++ callback
 *
 * This class allocates an IDataDistributionChannelCallback which points to C-style callback
 */
class DDM_EXPORT DataDistributionChannelCallback
{
public:
	/**
   * @brief Creates a new IDataDistributionChannelCallback instance.
   *
   * \p opaque user pointer sent back on C-style callback invocation
   * \p uEventCb C-style dataDistributionUnderlyingEvent callback
   * 
   * @returns the new IDataDistributionChannelCallback
   */
	static IDataDistributionChannelCallback *create(void *opaque, dataDistributionUnderlyingEvent uEventCb);
};

/**
 * @brief C-style callback invoked to report cluster state change.
 *
 * C-style callback invoked when subsystem needs to report a cluster state change.
 * 
 * \p change reports the DDM_CLUSTEREVENT event
 * \p serverid the server id reporting the change
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionOnClusterStateChange)(void *opaque, DDM_CLUSTEREVENT change, int64_t serverid);
/**
 * @brief C-style callback invoked to report a state change.
 *
 * C-style callback invoked when subsystem needs to report a state change.
 * 
 * \p newState reports the new DDM_INSTANCE_STATE state
 * \p oldState reports the new DDM_INSTANCE_STATE state
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionOnStateChange)(void *opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
/**
 * @brief C-style callback invoked when a state transfer is completed
 *
 * C-style callback invoked when subsystem needs to report a completed state transfer
 * 
 * \p pState pointer to the buffer of the state
 * \p len the length of the buffer
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionOnStateReady)(void *opaque, void *pState, int64_t len);
/**
 * @brief C-style callback invoked when a state transfer is requested
 *
 * C-style callback invoked when subsystem needs to request a state transfer. Fill \p pState and \p len with state information
 * 
 * \p pState pointer to the buffer of the state
 * \p len the length of the buffer
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionOnRequestedState)(void *opaque, void **pState, size_t *len);
/**
 * @brief C-style callback invoked when multiple primary server are detected
 *
 * C-style callback invoked when subsystem needs to report there are multiple primary server are detected
 * 
 * \p myId my primary identifier
 * \p otherId other primary identifier
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionOnMultiplePrimary)(void *opaque, int64_t myId, int64_t otherId);
/**
 * @brief C-style callback invoked to report a first state change
 *
 * C-style callback invoked when subsystem needs to to report a first state change
 * 
 * \p newState DDM_INSTANCE_STATE state
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionFirstStateChange)(void *opaque, DDM_INSTANCE_STATE newState);
/**
 * @brief C-style callback invoked to report a starting instance state change
 *
 * C-style callback invoked when subsystem needs to report a state change.
 * 
 * \p futureState reports the future DDM_INSTANCE_STATE state
 * \p actualState reports the actual DDM_INSTANCE_STATE state
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionChangingState)(void *opaque, DDM_INSTANCE_STATE actualState, DDM_INSTANCE_STATE futureState);
/**
 * @brief C-style callback invoked to report a finished instance state change
 *
 * C-style callback invoked when subsystem needs to report a state change.
 * 
 * \p actualState reports the actual DDM_INSTANCE_STATE state
 *
 * The callback's \p opaque argument is the opaque set with
 * DataDistributionMastershipCallback::create().
 *
 */
typedef void(__cdecl *dataDistributionChangedState)(void *opaque, DDM_INSTANCE_STATE actualState);

/**
 * @brief The C++ callback interface to be externally implemented
 *
 * Implement this interface in an external class and pass the pointer to IDataDistribution::RequestMastershipManager. The external class will be called conseguently.
 *
 * @sa IDataDistribution::RequestMastershipManager
 * @sa IDataDistributionMastershipCommon::Initialize
 */
class DDM_EXPORT IDataDistributionMastershipCallback
{
public:
	/**
	 * @brief Callback invoked to report cluster state change.
	 *
	 * Callback invoked when subsystem needs to report a cluster state change.
	 * 
	 * \p change reports the DDM_CLUSTEREVENT event
	 * \p serverid the server id reporting the change
	 *
	 */
	virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid) = 0;
	/**
	 * @brief Callback invoked to report a state change.
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p newState reports the new DDM_INSTANCE_STATE state
	 * \p oldState reports the new DDM_INSTANCE_STATE state
	 *
	 */
	virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) = 0;
	/**
	 * @brief Callback invoked when a state transfer is completed
	 *
	 * Callback invoked when subsystem needs to report a completed state transfer
	 * 
	 * \p pState pointer to the buffer of the state
	 * \p len the length of the buffer
	 *
	 */
	virtual void OnStateReady(void *pState, int64_t len) = 0;
	/**
	 * @brief Callback invoked when a state transfer is requested
	 *
	 * Callback invoked when subsystem needs to request a state transfer. Fill \p pState and \p len with state information
	 * 
	 * \p pState pointer to the buffer of the state
	 * \p len the length of the buffer
	 *
	 */
	virtual void OnRequestedState(void **pState, size_t *len) = 0;
	/**
	 * @brief Callback invoked when multiple primary server are detected
	 *
	 * Callback invoked when subsystem needs to report there are multiple primary server are detected
	 * 
	 * \p myId my primary identifier
	 * \p otherId other primary identifier
	 *
	 */
	virtual void OnMultiplePrimary(int64_t myId, int64_t otherId) = 0;
	/**
	 * @brief Callback invoked to report a first state change
	 *
	 * Callback invoked when subsystem needs to to report a first state change
	 * 
	 * \p newState DDM_INSTANCE_STATE state
	 *
	 */
	virtual void FirstStateChange(DDM_INSTANCE_STATE newState) = 0;
	/**
	 * @brief Callback invoked to report a starting instance state change
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p futureState reports the future DDM_INSTANCE_STATE state
	 * \p actualState reports the actual DDM_INSTANCE_STATE state
	 *
	 */
	virtual void ChangingState(DDM_INSTANCE_STATE actualState, DDM_INSTANCE_STATE futureState) = 0;
	/**
	 * @brief Callback invoked to report a finished instance state change
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p actualState reports the actual DDM_INSTANCE_STATE state
	 *
	 */
	virtual void ChangedState(DDM_INSTANCE_STATE actualState) = 0;
};
/**
 * @brief Class to convert C style callbacks into IDataDistributionMastershipCallback C++ callback
 *
 * This class allocates an IDataDistributionMastershipCallback which points to C-style callbacks
 */
class DDM_EXPORT DataDistributionMastershipCallback
{
public:
	/**
	 * @brief Creates a new IDataDistributionMastershipCallback instance.
	 *
	 * \p opaque user pointer sent back on C-style callback invocation
	 * \p c1 C-style dataDistributionOnClusterStateChange callback
	 * \p c2 C-style dataDistributionOnStateChange callback
	 * \p c3 C-style dataDistributionOnStateReady callback
	 * \p c4 C-style dataDistributionOnRequestedState callback
	 * \p c5 C-style dataDistributionOnMultiplePrimary callback
	 * \p c6 C-style dataDistributionFirstStateChange callback
	 * \p c7 C-style dataDistributionChangingState callback
	 * \p c8 C-style dataDistributionChangedState callback
	 * 
	 * @returns the new IDataDistributionMastershipCallback
	 */
	static IDataDistributionMastershipCallback *create(void *opaque, dataDistributionOnClusterStateChange c1, dataDistributionOnStateChange c2,
													   dataDistributionOnStateReady c3, dataDistributionOnRequestedState c4, dataDistributionOnMultiplePrimary c5,
													   dataDistributionFirstStateChange c6, dataDistributionChangingState c7, dataDistributionChangedState c8);
};
/**
 * @brief Base interface to be implemented from channel manager
 *
 * Base interface to be implemented from channel manager
 */
class DDM_EXPORT IDataDistributionChannelBase
{
public:
	/**
	 * @brief Creates a new channel
	 *
	 * \p channelName the name of the channel
	 * \p dataCb IDataDistributionChannelCallback pointer to receive data, condition or errors
	 * \p direction DDM_CHANNEL_DIRECTION direction, default is DDM_CHANNEL_DIRECTION::ALL
	 * \p arrayParams an array of string in the form key=value to override parameters passed into IDataDistribution::Initialize
	 * \p len length of arrayParams
	 * 
	 * @returns the new HANDLE or NULL if there was an error
	 */
	virtual HANDLE CreateChannel(const char *channelName, IDataDistributionChannelCallback *dataCb, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char *arrayParams[] = NULL, int len = 0) = 0;
	/**
	 * @brief Starts the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT StartChannel(HANDLE channelHandle, DWORD timeout) = 0;
	/**
	 * @brief Stops the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT StopChannel(HANDLE channelHandle, DWORD timeout) = 0;
	/**
	 * @brief Sets a parameter at run-time
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p paramName the parameter name
	 * \p paramValue the parameter value
	 * 
	 */
	virtual void SetParameter(HANDLE channelHandle, const char *paramName, const char *paramValue) = 0;
	/**
	 * @brief Sets a parameter at run-time
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p paramId the DDM_GENERAL_PARAMETER parameter id
	 * \p paramValue the parameter value
	 * 
	 */
	virtual void SetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId, const char *paramValue) = 0;
	/**
	 * @brief Reads a parameter at run-time
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p paramName the parameter name
	 * 
	 * @return parameter value
	 */
	virtual const char *GetParameter(HANDLE channelHandle, const char *paramName) = 0;
	/**
	 * @brief Reads a parameter at run-time
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p paramId the DDM_GENERAL_PARAMETER parameter id
	 * 
	 * @return parameter value
	 */
	virtual const char *GetParameter(HANDLE channelHandle, DDM_GENERAL_PARAMETER paramId) = 0;
	/**
	 * @brief Locks the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Lock(HANDLE channelHandle, DWORD timeout) = 0;
	/**
	 * @brief Unlock the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Unlock(HANDLE channelHandle) = 0;
	/**
	 * @brief Seeks the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p position the new channel position
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT SeekChannel(HANDLE channelHandle, int64_t position) = 0;
	/**
	 * @brief Deletes the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT DeleteChannel(HANDLE channelHandle) = 0;
	/**
	 * @brief Writes data on the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p key the key of the message to write, it can be NULL
	 * \p keyLen the length of the key
	 * \p buffer the buffer to write on the channel, it cannot be NULL
	 * \p bufferLen the length of the buffer to write
	 * \p waitAll set to TRUE to wait a complete message dispatch, default is FALSE. @remarks it depends on underlying implementation
	 * \p timestamp a timestamp associable to the message, default is DDM_NO_TIMESTAMP and means no timestamp written
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT WriteOnChannel(HANDLE channelHandle, const char *key, size_t keyLen, void *buffer, size_t bufferLen, const BOOL waitAll = FALSE, const int64_t timestamp = DDM_NO_TIMESTAMP) = 0;
	/**
	 * @brief Reads data from the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p offset offset to read info from
	 * \p dataLen pointer will receive length of data read
	 * \p buffer the buffer to receive data from the channel, it cannot be NULL
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT ReadFromChannel(HANDLE channelHandle, int64_t offset, size_t *bufferLen, void **buffer) = 0;
	/**
	 * @brief Change the DDM_CHANNEL_DIRECTION of the channel
	 *
	 * \p channelHandle the HANDLE of the channel return from IDataDistributionChannelBase::CreateChannel
	 * \p direction DDM_CHANNEL_DIRECTION direction
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT ChangeChannelDirection(HANDLE channelHandle, DDM_CHANNEL_DIRECTION direction) = 0;
};
/**
 * @brief Interface to be implemented from transport subsystem
 *
 * Interface to be implemented from transport subsystem. Extends IDataDistributionChannelBase
 */
class DDM_EXPORT IDataDistributionTransport : public IDataDistributionChannelBase
{
public:
	/**
	 * @brief Returns the server lost timeout
	 * 
	 * @returns The server lost timeout
	 */
	virtual int GetServerLostTimeout() = 0;
	/**
	 * @brief Returns the max message size managed from the subsystem
	 * 
	 * @returns Returns the max message size managed from the subsystem
	 */
	virtual size_t GetMaxMessageSize() = 0;
	/**
	 * @brief Start the transport subsystem
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Start(DWORD timeout) = 0;
	/**
	 * @brief Start the transport subsystem
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Stop(DWORD timeout) = 0;
};
/**
 * @brief Interface to be implemented from subsystem
 *
 * Interface to be implemented from subsystem. Extends IDataDistributionTransport and IDataDistributionLog
 */
class DDM_EXPORT IDataDistributionSubsystem : public IDataDistributionTransport, public IDataDistributionLog
{
public:
	/**
	 * @brief Initialize the subsystem
	 *
	 * \p cb IDataDistributionCallback received from IDataDistribution::Initialize
	 * \p conf_file the external ASCII configuration file with lines written as key=value
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Initialize(IDataDistributionCallback *cb, const char *conf_file, const char *hostAddress = NULL, const char *channelTrailer = NULL) = 0;
	/**
	 * @brief Initialize the subsystem
	 *
	 * \p cb IDataDistributionCallback received from IDataDistribution::Initialize
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Initialize(IDataDistributionCallback *cb, const char *arrayParams[], int len, const char *hostAddress = NULL, const char *channelTrailer = NULL) = 0;
	/**
	 * @brief Returns IDataDistributionCallback passed during IDataDistributionSubsystem::Initialize
	 *
	 * @returns IDataDistributionCallback
	 */
	virtual IDataDistributionCallback *GetCallbacks() = 0;
};
/**
 * @brief Association map between server id and ClusterHealthElement
 *
 */
typedef std::map<int64_t, ClusterHealthElement *> ClusterHealth;
/**
 * @brief Iterator of ClusterHealth
 *
 */
typedef std::map<int64_t, ClusterHealthElement *>::const_iterator ClusterHealthIterator;

/**
 * @brief Interface to be implemented from mastership subsystem
 *
 * Interface to be implemented from mastership subsystem
 */
class DDM_EXPORT IDataDistributionMastershipCommon
{
public:
	/**
	 * @brief Initialize the subsystem
	 *
	 * \p transportManager IDataDistributionSubsystem to use
	 * \p cbs IDataDistributionMastershipCallback to communicate information
	 * \p hostAddress optional host address or host name
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Initialize(IDataDistributionSubsystem *transportManager, IDataDistributionMastershipCallback *cbs, const char *hostAddress = NULL, const char *arrayParams[] = NULL, int len = 0) = 0;
	/**
	 * @brief Start the mastership subsystem
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Start(DWORD timeout) = 0;
	/**
	 * @brief Stop the mastership subsystem
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Stop(DWORD timeout) = 0;
	/**
	 * @brief Verify if this server will be elected as next primary
	 * 
	 * @returns TRUE if this server is the next primary
	 */
	virtual BOOL GetIamNextPrimary() = 0;
	/**
	 * @brief Request to be the next primary server
	 * 
	 * @returns TRUE in case of success
	 */
	virtual BOOL RequestIAmNextPrimary() = 0;
	/**
	 * @brief Returns an array of all known indexes of the servers within the cluster
	 * 
	 * \p length the parameter receiving the length of returned buffer
	 * 
	 * @returns the buffer of known indexes
	 * @remarks the returned buffer shall be freed
	 */
	virtual int64_t *GetClusterIndexes(size_t *length) = 0;
	/**
	 * @brief Returns the DDM_INSTANCE_STATE of a specific server
	 * 
	 * \p serverId server index to get DDM_INSTANCE_STATE
	 * 
	 * @returns the DDM_INSTANCE_STATE
	 */
	virtual DDM_INSTANCE_STATE GetStateOf(int64_t serverId) = 0;
	/**
	 * @brief Returns the health of the cluster
	 * 
	 * @returns the ClusterHealth
	 */
	virtual ClusterHealth GetClusterHealth() = 0;
	/**
	 * @brief Returns DDM_INSTANCE_STATE of this instance
	 * 
	 * @returns the DDM_INSTANCE_STATE
	 */
	virtual DDM_INSTANCE_STATE GetMyState() = 0;
	/**
	 * @brief Change DDM_INSTANCE_STATE of this instance
	 * 
	 * \p state DDM_INSTANCE_STATE expected
	 */
	virtual void ChangeMyState(DDM_INSTANCE_STATE state) = 0;
	/**
	 * @brief Change DDM_INSTANCE_STATE of \p instanceId
	 * 
	 * \p instanceId server id to change state
	 * \p state DDM_INSTANCE_STATE expected
	 */
	virtual void ChangeState(int64_t instanceId, DDM_INSTANCE_STATE state) = 0;
	/**
	 * @brief Returns the server id of the local instance
	 * 
	 * @returns the server id
	 */
	virtual int64_t GetLocalServerId() = 0;
	/**
	 * @brief Returns the server id of the primary instance
	 * 
	 * @returns the server id
	 */
	virtual int64_t GetPrimaryServerId() = 0;
	/**
	 * @brief Returns the message delay within the cluster
	 * 
	 * @returns the message delay in milliseconds
	 */
	virtual int64_t GetMessageDelay() = 0;
	/**
	 * @brief Returns the current uptime
	 * 
	 * @returns the uptime in milliseconds
	 */
	virtual int64_t GetUpTime() = 0;
};

/**
 * @brief Interface to be implemented from common manager
 *
 * Interface to be implemented from common manager
 */
class DDM_EXPORT IDataDistribution
{
public:
	/**
	 * @brief Initialize IDataDistribution instance
	 *
	 * \p iddcb IDataDistributionCallback from external user application
	 * \p conf_file the external ASCII configuration file with lines written as key=value
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Initialize(IDataDistributionCallback *iddcb, const char *conf_file = NULL, const char *hostAddress = NULL, const char *channelTrailer = NULL) = 0;
	/**
	 * @brief Initialize IDataDistribution instance
	 *
	 * \p iddcb IDataDistributionCallback received from external user application
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT Initialize(IDataDistributionCallback *iddcb, const char *arrayParams[], int len, const char *hostAddress = NULL, const char *channelTrailer = NULL) = 0;
	/**
	 * @brief Allocate and initialize the mastership manager
	 *
	 * \p cbs IDataDistributionMastershipCallback received from external user application
	 * \p hostAddress optional host address or host name
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual HRESULT RequestMastershipManager(IDataDistributionMastershipCallback *cbs, const char *szMyAddress = NULL, const char *arrayParams[] = NULL, int len = 0) = 0;
	/**
	 * @brief Start the common manager
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual BOOL Start(DWORD timeout) = 0;
	/**
	 * @brief Stop the common manager
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the HRESULT of the operation
	 */
	virtual BOOL Stop(DWORD timeout) = 0;
	/**
	 * @brief Returns the allocated communication subsystem
	 * 
	 * @returns IDataDistributionSubsystem of communication subsystem
	 */
	virtual IDataDistributionSubsystem *GetSubsystemManager() = 0;
	/**
	 * @brief Returns the allocated mastership subsystem
	 * 
	 * @returns IDataDistributionMastershipCommon of mastership subsystem
	 */
	virtual IDataDistributionMastershipCommon *GetMastershipManager() = 0;
	/**
	 * @brief Returns the protocol in use from IDataDistribution instance
	 * 
	 * @returns the protocol name
	 */
	virtual std::string GetProtocol() = 0;
	/**
	 * @brief Returns the protocol library in use from IDataDistribution instance
	 * 
	 * @returns the protocol library name
	 */
	virtual std::string GetProtocolLib() = 0;
	/**
	 * @brief Returns the mastership library in use from IDataDistribution instance
	 * 
	 * @returns the mastership library name
	 */
	virtual std::string GetMastershipLib() = 0;
};

/**
 * @brief Main entry point to activate an instance of IDataDistribution
 *
 * Main entry point to activate an instance of IDataDistribution
 */
class DDM_EXPORT DataDistribution : public IDataDistribution
{
public:
	/**
	 * @brief Creates a new IDataDistribution instance.
	 * 
	 * @returns the IDataDistribution istance created
	 */
	static IDataDistribution *create();
};

#endif // !defined(DATADISTRIBUTIONMANAGER_H__INCLUDED_)
