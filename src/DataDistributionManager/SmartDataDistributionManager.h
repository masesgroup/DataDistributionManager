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

#if !defined(SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_)
#define SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_

/**
 * @file SmartDataDistributionManager.h
 * @brief DataDistributionManager library.
 *
 * SmartDataDistributionManager.h contains the public C++ API for Smart management of DataDistributionManager.
 * The API is documented in this file as comments prefixing the class,
 * function, type, enum, define, etc.
 * The C++ interface is STD C++ '03 compliant and adheres to the
 * Google C++ Style Guide.
 *
 * @tableofcontents
 */

/**@cond NO_DOC*/
#include "DataDistributionManager.h"
#include <vector>

/**@endcond*/

/**
 * @brief Smart class to manage ITimeMeasureWrapper
 *
 * A smart manager for timing
 */
class DDM_EXPORT SmartTimeMeasureWrapper : public ITimeMeasureWrapper
{
public:
	/**
	 * @brief Initialize a new SmartTimeMeasureWrapper
	 */
	SmartTimeMeasureWrapper();
	/**
	 * @brief Destructor of SmartTimeMeasureWrapper
	 */
	~SmartTimeMeasureWrapper();
	/**
	 * @brief Reset the timer
	 *
	 * Reset the timer information to zero
	 *
	 */
	void ResetTime();
	/**
	 * @brief Reports the elapsed time in milliseconds
	 *
	 * Use to function to get the elapsed time in milliseconds
	 *
	 * @returns the elapsed time in milliseconds
	 */
	int64_t ElapsedMilliseconds();
	/**
	 * @brief Reports the elapsed time in microseconds
	 *
	 * Use to function to get the elapsed time in microseconds
	 *
	 * @returns the elapsed time in microseconds
	 */
	int64_t ElapsedMicroseconds();
	/**
	 * @brief Reports the elapsed time in nanoseconds
	 *
	 * Use to function to get the elapsed time in nanoseconds
	 *
	 * @returns the elapsed time in nanoseconds
	 */
	int64_t ElapsedNanoseconds();
	/**
	 * @brief Adds nanoseconds time to actual value
	 *
	 * Use to function to add \p time nanoseconds to current value
	 * 
	 * \p time the nanoseconds to be added
	 * 
	 */
	void AddNanoseconds(unsigned int time);

private:
	ITimeMeasureWrapper *m_pITimeMeasureWrapper;
};

/**
 * @brief Smart class to manage IDataDistributionChannelCallback
 *
 * A smart manager for channels
 */
class DDM_EXPORT SmartDataDistributionChannel : protected IDataDistributionChannelCallback
{
public:
	/**
	 * @brief Initialize a new SmartDataDistributionChannel
	 */
	SmartDataDistributionChannel();

protected:
	/**
	 * @brief Destructor of SmartDataDistributionChannel
	 */
	~SmartDataDistributionChannel();

public:
	/**
	 * @brief Returns the DDM_CHANNEL_DIRECTION channel direction
	 */
	DDM_CHANNEL_DIRECTION GetDirection();
	/**
	 * @brief Sets information on SmartDataDistributionChannel
	 * 
	 * \p channelName the channel name
	 * \p pIDataDistributionChannelBase pointer to IDataDistributionChannelBase instance
	 * \p channelHandle the CHANNEL_HANDLE to the channel
	 * \p direction DDM_CHANNEL_DIRECTION direction
	 * 
	 * @remarks Used from SmartDataDistribution::CreateSmartChannel
	 * 
	 */
	void SetInformation(const char *channelName, IDataDistributionChannelBase *pIDataDistributionChannelBase, CHANNEL_HANDLE_PARAMETER, DDM_CHANNEL_DIRECTION direction);
	/**
	 * @brief Starts the channel
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT StartChannel(unsigned long timeout);
	/**
	 * @brief Stops the channel
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT StopChannel(unsigned long timeout);
	/**
	 * @brief Locks the channel
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT Lock(unsigned long timeout);
	/**
	 * @brief Unlock the channel
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT Unlock();
	/**
	 * @brief Seeks the channel
	 *
	 * \p position the new channel position
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT SeekChannel(int64_t position);
	/**
	 * @brief Writes data on the channel
	 *
	 * \p key the key of the message to write, it can be NULL
	 * \p keyLen the length of the key
	 * \p buffer the buffer to write on the channel, it cannot be NULL
	 * \p bufferLen the length of the buffer to write
	 * \p waitAll set to TRUE to wait a complete message dispatch, default is FALSE. @remarks it depends on underlying implementation
	 * \p timestamp a timestamp associable to the message, default is DDM_NO_TIMESTAMP and means no timestamp written
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT WriteOnChannel(const char *key, size_t keyLen, void *buffer, size_t bufferLen, const BOOL waitAll = FALSE, const int64_t timestamp = DDM_NO_TIMESTAMP);
	/**
	 * @brief Reads data from the channel
	 *
	 * \p offset offset to read info from
	 * \p dataLen pointer will receive length of data read
	 * \p buffer the buffer to receive data from the channel, it cannot be NULL
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT ReadFromChannel(int64_t offset, size_t *dataLen, void **buffer);
	/**
	 * @brief Change the DDM_CHANNEL_DIRECTION of the channel
	 *
	 * \p direction DDM_CHANNEL_DIRECTION direction
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT ChangeChannelDirection(DDM_CHANNEL_DIRECTION direction);
	/**
	 * @brief Returns the channel name
	 * 
	 * @returns the channel name
	 */
	const char *GetChannelName();
	/**
	 * @brief Function to override to receive data available
	 * 
	 * \p key the key of the message, NULL if the key was omitted in transmission
	 * \p keyLen the length of key
	 * \p buffer the data buffer received
	 * \p len the length of the buffer
	 * 
	 */
	virtual void OnDataAvailable(const char *key, size_t keyLen, const void *buffer, const size_t len);
	/**
	 * @brief Function to override to receive data available
	 * 
	 * \p key the key of the message, NULL if the key was omitted in transmission
	 * \p buffer the data buffer received
	 * \p len the length of the buffer
	 * 
	 */
	virtual void OnDataAvailable(const std::string key, const void *buffer, const size_t len);
	/**
	 * @brief Function to override to receive condition or error
	 * 
	 * \p errorCode the DDM_UNDERLYING_ERROR_CONDITION error or condition
	 * \p nativeCode the native code from transport subsystem
	 * \p subSystemReason reason from transport subsystem
	 * 
	 */
	virtual void OnConditionOrError(const OPERATION_RESULT errorCode, const int nativeCode, const char *subSystemReason);
	/**
	 * @brief Function to override to receive condition or error
	 * 
	 * \p errorCode the DDM_UNDERLYING_ERROR_CONDITION error or condition
	 * \p nativeCode the native code from transport subsystem
	 * \p subSystemReason reason from transport subsystem
	 * 
	 */
	virtual void OnConditionOrError(const OPERATION_RESULT errorCode, const int nativeCode, const std::string subSystemReason);

protected:
	/**
	 * @brief Function to override to receive UnderlyingEventData
	 * 
	 * \p channelHandle the CHANNEL_HANDLE of the channel
	 * \p uEvent the UnderlyingEventData received from transport subsystem
	 * 
	 */
	virtual void OnUnderlyingEvent(const CHANNEL_HANDLE_PARAMETER, const UnderlyingEventData *uEvent);

private:
	const char *m_ChannelName;
	CHANNEL_HANDLE m_channelHandle;
	IDataDistributionChannelBase *m_pIDataDistributionChannelBase;
	DDM_CHANNEL_DIRECTION m_Direction;
};

/**
 * @brief Smart class to manage IDataDistributionCallback and IDataDistributionMastershipCallback
 *
 * The main class to manage Data Distribution
 */
class DDM_EXPORT SmartDataDistributionBase : protected IDataDistributionCallback, protected IDataDistributionMastershipCallback
{
public:
	/**
	 * @brief Initialize a new SmartDataDistributionBase
	 */
	SmartDataDistributionBase();
	/**
	 * @brief Destructor of SmartDataDistributionBase
	 */
	~SmartDataDistributionBase();
	/**
	 * @brief Initialize SmartDataDistributionBase system
	 *
	 * \p conf_file the external ASCII configuration file with lines written as key=value
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT Initialize(const char *conf_file, const char *hostAddress = 0, const char *channelTrailer = 0);
	/**
	 * @brief Initialize IDataDistribution instance
	 *
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * \p hostAddress optional host address or host name
	 * \p channelTrailer the optional trailer to be appended to channel name when each channel is created
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT Initialize(const char *arrayParams[], int len, const char *hostAddress = 0, const char *channelTrailer = 0);
	/**
	 * @brief Allocate and initialize the mastership manager
	 *
	 * \p hostAddress optional host address or host name
	 * \p arrayParams an array of string in the form key=value
	 * \p len length of \p arrayParams
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	OPERATION_RESULT RequestMastershipManager(const char *hostAddress = NULL, const char *arrayParams[] = NULL, int len = 0);
	/**
	 * @brief Start the common manager
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	BOOL Start(unsigned long timeout);
	/**
	 * @brief Stop the common manager
	 *
	 * \p timeout the operation timeout in milliseconds
	 * 
	 * @returns the OPERATION_RESULT of the operation
	 */
	BOOL Stop(unsigned long timeout);
	/**
	 * @brief Returns the protocol in use from IDataDistribution instance
	 * 
	 * @returns the protocol name
	 */
	std::string GetProtocol();
	/**
	 * @brief Returns the protocol library in use from IDataDistribution instance
	 * 
	 * @returns the protocol library name
	 */
	std::string GetProtocolLib();
	/**
	 * @brief Returns the mastership library in use from IDataDistribution instance
	 * 
	 * @returns the mastership library name
	 */
	std::string GetMastershipLib();

protected:
	// IDataDistributionCallback
	/**
	* @brief Function invoked during configuration validation
	*
	* The callback is usable to override configuration parameters in some special conditions.
	* \p channelName reports the channel involved (null for global parameters),
	* \p key is the parameter key to be checked and \p value is the actual value.
	* @return \p value if no change is needed, otherwise any new string value according to the expected parameter type
	*/
	virtual const char *OnConfiguration(const char *channelName, const char *key, const char *value);
	/**
	 * @brief Function invoked when a log is emitted from subsystem.
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
	virtual void OnLogging(DDM_LOG_LEVEL level, const char *source, const char *function, const char *logStr);
	/**
	 * @brief Callback invoked on a complete disconnection.
	 *
	 * Callback invoked when subsystem detects a complete disconnection from central server or other peers.
	 * \p channelName reports the disconnected channel
	 * \p reason is the reason of disconnection.
	 *
	 */
	virtual void OnCompletelyDisconnected(const char *channelName, const char *reason);
	// IDataDistributionMastershipCallback
	/**
	 * @brief Callback invoked to report cluster state change.
	 *
	 * Callback invoked when subsystem needs to report a cluster state change.
	 * 
	 * \p change reports the DDM_CLUSTEREVENT event
	 * \p serverid the server id reporting the change
	 *
	 */
	virtual void OnClusterStateChange(DDM_CLUSTEREVENT change, int64_t serverid);
	/**
	 * @brief Callback invoked to report a state change.
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p newState reports the new DDM_INSTANCE_STATE state
	 * \p oldState reports the new DDM_INSTANCE_STATE state
	 *
	 */
	virtual void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);
	/**
	 * @brief Callback invoked when a state transfer is completed
	 *
	 * Callback invoked when subsystem needs to report a completed state transfer
	 * 
	 * \p pState pointer to the buffer of the state
	 * \p len the length of the buffer
	 *
	 */
	virtual void OnStateReady(void *pState, int64_t len);
	/**
	 * @brief Callback invoked when a state transfer is requested
	 *
	 * Callback invoked when subsystem needs to request a state transfer. Fill \p pState and \p len with state information
	 * 
	 * \p pState pointer to the buffer of the state
	 * \p len the length of the buffer
	 *
	 */
	virtual void OnRequestedState(void **pState, size_t *len);
	/**
	 * @brief Callback invoked when multiple primary server are detected
	 *
	 * Callback invoked when subsystem needs to report there are multiple primary server are detected
	 * 
	 * \p myId my primary identifier
	 * \p otherId other primary identifier
	 *
	 */
	virtual void OnMultiplePrimary(int64_t myId, int64_t otherId);
	/**
	 * @brief Callback invoked to report a first state change
	 *
	 * Callback invoked when subsystem needs to to report a first state change
	 * 
	 * \p newState DDM_INSTANCE_STATE state
	 *
	 */
	virtual void FirstStateChange(DDM_INSTANCE_STATE newState);
	/**
	 * @brief Callback invoked to report a starting instance state change
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p futureState reports the future DDM_INSTANCE_STATE state
	 * \p actualState reports the actual DDM_INSTANCE_STATE state
	 *
	 */
	virtual void ChangingState(DDM_INSTANCE_STATE actualState, DDM_INSTANCE_STATE futureState);
	/**
	 * @brief Callback invoked to report a finished instance state change
	 *
	 * Callback invoked when subsystem needs to report a state change.
	 * 
	 * \p actualState reports the actual DDM_INSTANCE_STATE state
	 *
	 */
	virtual void ChangedState(DDM_INSTANCE_STATE actualState);

protected:
	IDataDistribution *m_pIDataDistribution;
	IDataDistributionMastershipCommon *m_pIDataDistributionMastershipCommon;
	OPERATION_RESULT m_pInitializeHRESULT;
};

/**
 * @brief Smart class to manage DataDistribution
 * 
 * @tparam T The class inherited from SmartDataDistributionChannel
 */
template <typename T>
class SmartDataDistribution : public SmartDataDistributionBase
{
public:
	/**
	 * @brief Initialize a new SmartDataDistribution
	 */
	SmartDataDistribution() : SmartDataDistributionBase() {}
	/**
	 * @brief Destructor of SmartDataDistribution
	 */
	~SmartDataDistribution() {}
	/**
	 * @brief Creates a new smart channel
	 *
	 * \p channelName the name of the channel
	 * \p direction DDM_CHANNEL_DIRECTION direction, default is DDM_CHANNEL_DIRECTION::ALL
	 * \p arrayParams an array of string in the form key=value to override parameters passed into IDataDistribution::Initialize
	 * \p len length of arrayParams
	 * 
	 * @return T* the allocated instance
	 */
	T *CreateSmartChannel(const char *channelName, DDM_CHANNEL_DIRECTION direction = DDM_CHANNEL_DIRECTION::ALL, const char *arrayParams[] = NULL, int len = 0)
	{
		static_assert(std::is_base_of<SmartDataDistributionChannel, T>::value, "type parameter of this class must derive from SmartDataDistributionChannel");
		if (OPERATION_FAILED(m_pInitializeHRESULT))
			return NULL;
		T *pSmartChannelT = new T();
		SmartDataDistributionChannel *pSmartChannel = (SmartDataDistributionChannel *)pSmartChannelT;
		IDataDistributionChannelBase *pChannelBase = m_pIDataDistribution->GetSubsystemManager();
		CHANNEL_HANDLE_PARAMETER = pChannelBase->CreateChannel(channelName, (IDataDistributionChannelCallback *)pSmartChannel, direction, arrayParams, len);
		pSmartChannel->SetInformation(channelName, pChannelBase, channelHandle, direction);

		return pSmartChannelT;
	}
};

#endif // End SMARTDATADISTRIBUTIONMANAGER_H__INCLUDED_
