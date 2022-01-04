/*
*  Copyright 2022 MASES s.r.l.
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

/**
 * Main interface to activate Data Distribution Manager
 */
public interface ISmartDataDistribution {
    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file Configuration file to use
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String conf_file);

    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file    Configuration file to use
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String conf_file, String topicTrailer);

    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file    Configuration file to use
     * @param szMyAddress  The name of the server hosting the process
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String conf_file, String szMyAddress, String topicTrailer);

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @return {@link OPERATION_RESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    OPERATION_RESULT Initialize(IConfiguration configuration) throws IllegalArgumentException;

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @param topicTrailer  Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    OPERATION_RESULT Initialize(IConfiguration configuration, String topicTrailer) throws IllegalArgumentException;

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @param szMyAddress   The name of the server hosting the process
     * @param topicTrailer  Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    OPERATION_RESULT Initialize(IConfiguration configuration, String szMyAddress, String topicTrailer)
            throws IllegalArgumentException;

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams array of key=value parameters
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String[] arrayParams);

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams  array of key=value parameters
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String[] arrayParams, String topicTrailer);

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams  array of key=value parameters
     * @param szMyAddress  The name of the server hosting the process
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Initialize(String[] arrayParams, String szMyAddress, String topicTrailer);

    /**
     * Request to allocate mastership manager
     * 
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager();

    /**
     * Request to allocate mastership manager
     * 
     * @param parameters Paramaters to send to underlying layer
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager(String[] parameters);

    /**
     * Request to allocate mastership manager
     * 
     * @param parameters {@link IConfiguration} instance of parameters to send to
     *                   underlying layer
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager(IConfiguration parameters);

    /**
     * Request to allocate mastership manager
     * 
     * @param serverName The server name
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager(String serverName);

    /**
     * Request to allocate mastership manager
     * 
     * @param serverName The server name
     * @param parameters {@link IConfiguration} instance of parameters to send to
     *                   underlying layer
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager(String serverName, IConfiguration parameters);

    /**
     * Request to allocate mastership manager
     * 
     * @param serverName The server name
     * @param parameters Paramaters to send to underlying layer
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT RequestMastershipManager(String serverName, String[] parameters);

    /**
     * 
     * @return Returns {@link ISmartDataDistributionMastership}
     */
    ISmartDataDistributionMastership getMastershipManager();

    /**
     * Starts the manager
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Start(int timeout);

    /**
     * Stops the manager
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Stop(int timeout);

    /**
     * 
     * @return Return the protocol in use
     */
    String getProtocol();

    /**
     * 
     * @return Return the protocol library in use
     */
    String getProtocolLib();

    /**
     * 
     * @return Return the mastership library in use
     */
    String getMastershipLib();

    /**
     * Set global parameter
     * 
     * @param paramName  Parameter name to set
     * @param paramValue Parameter value to set
     */
    void SetParameter(String paramName, String paramValue);

    /**
     * Set global parameter
     * 
     * @param paramId    Parameter {@link DDM_GENERAL_PARAMETER} to set
     * @param paramValue Parameter value to set
     */
    void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue);

    /**
     * Get global parameter
     * 
     * @param paramName Parameter name to get
     * @return Parameter value
     */
    String GetParameter(String paramName);

    /**
     * Get global parameter
     * 
     * @param paramId Parameter {@link DDM_GENERAL_PARAMETER} to get
     * @return Parameter value
     */
    String GetParameter(DDM_GENERAL_PARAMETER paramId);

    /**
     * 
     * @param <T>         The class extending {@link SmartDataDistributionChannel}
     * @param clazz       The class to be instantiated
     * @param channelName The channel name
     * @return The allocated instance
     * @throws Throwable generic exception for all errors (if subsystem was unable
     *                   to create a channel the exception is a generic
     *                   {@link Exception})
     */
    <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName) throws Throwable;

    /**
     * 
     * @param <T>           The class extending {@link SmartDataDistributionChannel}
     * @param clazz         The class to be instantiated
     * @param channelName   The channel name
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @return The allocated instance
     * @throws Throwable generic exception for all errors (if subsystem was unable
     *                   to create a channel the exception is a generic
     *                   {@link Exception})
     */
    <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration) throws Throwable;

    /**
     * 
     * @param <T>           The class extending {@link SmartDataDistributionChannel}
     * @param clazz         The class to be instantiated
     * @param channelName   The channel name
     * @param direction     The {@link DDM_CHANNEL_DIRECTION} of the channel
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @return The allocated instance
     * @throws Throwable generic exception for all errors (if subsystem was unable
     *                   to create a channel the exception is a generic
     *                   {@link Exception})
     */
    <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration, DDM_CHANNEL_DIRECTION direction) throws Throwable;

    /**
     * 
     * @param <T>         The class extending {@link SmartDataDistributionChannel}
     * @param clazz       The class to be instantiated
     * @param channelName The channel name
     * @param direction   The {@link DDM_CHANNEL_DIRECTION} of the channel
     * @param arrayParams Specific parameters which override main parameters
     * @return The allocated instance
     * @throws Throwable generic exception for all errors (if subsystem was unable
     *                   to create a channel the exception is a generic
     *                   {@link Exception})
     */
    @SuppressWarnings("unchecked")
    <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            DDM_CHANNEL_DIRECTION direction, String[] arrayParams) throws Throwable;

    /**
     * Adds a {@link IConfigurationListener} listener
     * 
     * @param listener {@link IConfigurationListener} listener to add
     */
    void addListener(IConfigurationListener listener);

    /**
     * Removes a {@link IConfigurationListener} listener
     * 
     * @param listener {@link IConfigurationListener} listener to remove
     */
    void removeListener(IConfigurationListener listener);

    /**
     * Adds a {@link ILoggingListener} listener
     * 
     * @param listener {@link ILoggingListener} listener to add
     */
    void addListener(ILoggingListener listener);

    /**
     * Removes a {@link ILoggingListener} listener
     * 
     * @param listener {@link ILoggingListener} listener to remove
     */
    void removeListener(ILoggingListener listener);

    /**
     * Adds a {@link ICompletelyDisconnectedListener} listener
     * 
     * @param listener {@link ICompletelyDisconnectedListener} listener to add
     */
    void addListener(ICompletelyDisconnectedListener listener);

    /**
     * Removes a {@link ICompletelyDisconnectedListener} listener
     * 
     * @param listener {@link ICompletelyDisconnectedListener} listener to remove
     */
    void removeListener(ICompletelyDisconnectedListener listener);

    // IDataDistributionMastershipCallback

    /**
     * Adds a {@link IClusterStateChangeListener} listener
     * 
     * @param listener {@link IClusterStateChangeListener} listener to add
     */
    void addListener(IClusterStateChangeListener listener);

    /**
     * Removes a {@link IClusterStateChangeListener} listener
     * 
     * @param listener {@link IClusterStateChangeListener} listener to remove
     */
    void removeListener(IClusterStateChangeListener listener);

    /**
     * The cluster has changed its state
     * 
     * @param change   New cluster event
     * @param serverid Server has emitted event
     */
    void OnClusterStateChange(DDM_CLUSTEREVENT change, long serverid);

    /**
     * Adds a {@link IStateChangeListener} listener
     * 
     * @param listener {@link IStateChangeListener} listener to add
     */
    void addListener(IStateChangeListener listener);

    /**
     * Removes a {@link IStateChangeListener} listener
     * 
     * @param listener {@link IStateChangeListener} listener to remove
     */
    void removeListener(IStateChangeListener listener);

    /**
     * My state has changed
     * 
     * @param newState New state
     * @param oldState Old state
     */
    void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState);

    /**
     * Adds a {@link IStateReadyListener} listener
     * 
     * @param listener {@link IStateReadyListener} listener to add
     */
    void addListener(IStateReadyListener listener);

    /**
     * Removes a {@link IStateReadyListener} listener
     * 
     * @param listener {@link IStateReadyListener} listener to remove
     */
    void removeListener(IStateReadyListener listener);

    /**
     * The state is ready to be used
     * 
     * @param buffer The buffer state receiver
     */
    void OnStateReady(byte[] buffer);

    /**
     * Adds a {@link IRequestedStateListener} listener
     * 
     * @param listener {@link IRequestedStateListener} listener to add
     */
    void addListener(IRequestedStateListener listener);

    /**
     * Removes a {@link IRequestedStateListener} listener
     * 
     * @param listener {@link IRequestedStateListener} listener to remove
     */
    void removeListener(IRequestedStateListener listener);

    /**
     * Called when a state transfer was requested
     * 
     * @param buffer Pointer to the buffer
     */
    void OnRequestedState(byte[] buffer);

    /**
     * Adds a {@link IMultiplePrimaryListener} listener
     * 
     * @param listener {@link IMultiplePrimaryListener} listener to add
     */
    void addListener(IMultiplePrimaryListener listener);

    /**
     * Removes a {@link IMultiplePrimaryListener} listener
     * 
     * @param listener {@link IMultiplePrimaryListener} listener to remove
     */
    void removeListener(IMultiplePrimaryListener listener);

    /**
     * There are multiple primary server in the cluster
     * 
     * @param myId    My identifier
     * @param otherId Other identifier which is primary
     */
    void OnMultiplePrimary(long myId, long otherId);

    /**
     * Adds a {@link IFirstStateChangeListener} listener
     * 
     * @param listener {@link IFirstStateChangeListener} listener to add
     */
    void addListener(IFirstStateChangeListener listener);

    /**
     * Removes a {@link IFirstStateChangeListener} listener
     * 
     * @param listener {@link IFirstStateChangeListener} listener to remove
     */
    void removeListener(IFirstStateChangeListener listener);

    /**
     * Called the first time there is state change
     * 
     * @param newState New state
     */
    void OnFirstStateChange(DDM_INSTANCE_STATE newState);

    /**
     * Adds a {@link IChangingStateListener} listener
     * 
     * @param listener {@link IChangingStateListener} listener to add
     */
    void addListener(IChangingStateListener listener);

    /**
     * Removes a {@link IChangingStateListener} listener
     * 
     * @param listener {@link IChangingStateListener} listener to remove
     */
    void removeListener(IChangingStateListener listener);

    /**
     * The state is starting to change
     * 
     * @param oldState Old state
     * @param newState New state
     */
    void OnChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState);

    /**
     * Adds a {@link IChangedStateListener} listener
     * 
     * @param listener {@link IChangedStateListener} listener to add
     */
    void addListener(IChangedStateListener listener);

    /**
     * Removes a {@link IChangedStateListener} listener
     * 
     * @param listener {@link IChangedStateListener} listener to remove
     */
    void removeListener(IChangedStateListener listener);

    /**
     * The state has changed
     * 
     * @param newState New state
     */
    void OnChangedState(DDM_INSTANCE_STATE newState);
}
