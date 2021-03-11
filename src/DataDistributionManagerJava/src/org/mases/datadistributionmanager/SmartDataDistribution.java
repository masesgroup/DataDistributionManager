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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.*;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Main class to activate Data Distribution Manager
 */
public class SmartDataDistribution implements IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow {
    SynchronizedEventsManager<IConfigurationListener> m_IConfigurationListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<ILoggingListener> m_ILoggingListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<ICompletelyDisconnectedListener> m_ICompletelyDisconnectedListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IClusterStateChangeListener> m_IClusterStateChangeListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IStateChangeListener> m_IStateChangeListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IStateReadyListener> m_IStateReadyListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IRequestedStateListener> m_IRequestedStateListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IMultiplePrimaryListener> m_IMultiplePrimaryListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IFirstStateChangeListener> m_IFirstStateChangeListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IChangingStateListener> m_IChangingStateListener_listeners = new SynchronizedEventsManager<>();
    SynchronizedEventsManager<IChangedStateListener> m_IChangedStateListener_listeners = new SynchronizedEventsManager<>();

    long m_DataDistributionCallbackLow;
    long m_DataDistributionMastershipCallbackLow;
    long IDataDistribution_ptr;
    long IDataDistributionSubsystemManager_ptr;
    long m_InitializeResult = -1;

    private static void unzip(String zipFilePath, String destDirectory) throws IOException {
        File destDir = new File(destDirectory);
        if (!destDir.exists()) {
            destDir.mkdir();
        }
        try (ZipInputStream zipIn = new ZipInputStream(new FileInputStream(zipFilePath))) {
            ZipEntry entry = zipIn.getNextEntry();
            while (entry != null) {
                String filePath = destDirectory + File.separator + entry.getName();
                if (!entry.isDirectory()) {
                    extractFile(zipIn, filePath);
                    if (!System.getProperty("os.name").contains("Windows")) {
                        try {
                            Runtime.getRuntime().exec(new String[] { "chmod", "755", filePath }).waitFor();
                        } catch (Throwable e) {
                        }
                    }
                } else {
                    File dir = new File(filePath);
                    dir.mkdirs();
                }
                zipIn.closeEntry();
                entry = zipIn.getNextEntry();
            }
        }
    }

    private static void extractFile(ZipInputStream zipIn, String filePath) throws IOException {
        final int BUFFER_SIZE = 4096;
        try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(filePath))) {
            byte[] bytesIn = new byte[BUFFER_SIZE];
            int read = 0;
            while ((read = zipIn.read(bytesIn)) != -1) {
                bos.write(bytesIn, 0, read);
            }
        }
    }

    private static boolean extractAndLoadLibraryFile(String libFolderForCurrentOS, String targetFolder,
            String entryLibrary) {
        final String containerFileName = "nativepackage.zip";
        String nativeLibraryFilePath = libFolderForCurrentOS + "/" + containerFileName;
        File extractedLibFile = new File(targetFolder, containerFileName);

        try {
            // Extract file into the current directory
            try (InputStream reader = SmartDataDistribution.class.getResourceAsStream(nativeLibraryFilePath);
                    FileOutputStream writer = new FileOutputStream(extractedLibFile)) {
                byte[] buffer = new byte[1024];
                int bytesRead = 0;
                while ((bytesRead = reader.read(buffer)) != -1) {
                    writer.write(buffer, 0, bytesRead);
                }
            }

            unzip(extractedLibFile.toString(), targetFolder);

            return loadNativeLibrary(targetFolder, entryLibrary);
        } catch (IOException e) {
            System.err.println(e.getMessage());
            return false;
        }
    }

    private static synchronized boolean loadNativeLibrary(String path, String name) {
        File libPath = new File(path, name);
        if (libPath.exists()) {
            try {
                System.load(new File(path, name).getAbsolutePath());
                return true;
            } catch (UnsatisfiedLinkError e) {
                System.err.println(e);
                return false;
            }

        } else
            return false;
    }

    static boolean LoadWrapper() {
        String osName = System.getProperty("os.name");
        String osArch = System.getProperty("os.arch");

        String libFolderForCurrentOS = "windows_x64";
        String libName = "DataDistributionManager.dll";

        if (osName.toLowerCase().contains("linux")) {
            if (osArch.toLowerCase().contains("amd64") || osArch.toLowerCase().contains("x86_64")) {
                libFolderForCurrentOS = "linux_x64";
                libName = "DataDistributionManager.so";
            } else if (osArch.toLowerCase().contains("x86")) {
                libFolderForCurrentOS = "linux_x86";
                libName = "DataDistributionManager.so";
            } else if (osArch.toLowerCase().contains("arm")) {
                libFolderForCurrentOS = "linux_arm";
                libName = "DataDistributionManager.so";
            } else if (osArch.toLowerCase().contains("arm64")) {
                libFolderForCurrentOS = "linux_arm64";
                libName = "DataDistributionManager.so";
            }
        } else if (osName.toLowerCase().contains("mac")) {
            if (osArch.toLowerCase().contains("64")) {
                libFolderForCurrentOS = "mac_x64";
                libName = "DataDistributionManager.dylib";
            }
        } else if (osName.toLowerCase().contains("windows")) {
            if (osArch.toLowerCase().contains("amd64") || osArch.toLowerCase().contains("x86_64")) {
                libFolderForCurrentOS = "windows_x64";
                libName = "DataDistributionManager.dll";
            } else if (osArch.toLowerCase().contains("x86")) {
                libFolderForCurrentOS = "windows_x86";
                libName = "DataDistributionManager.dll";
            } else if (osArch.toLowerCase().contains("arm")) {
                libFolderForCurrentOS = "windows_arm";
                libName = "DataDistributionManager.dll";
            } else if (osArch.toLowerCase().contains("arm64")) {
                libFolderForCurrentOS = "windows_arm64";
                libName = "DataDistributionManager.dll";
            }
        }

        try {
            Path tmpPath = Files.createTempDirectory(SmartDataDistribution.class.getName());
            return extractAndLoadLibraryFile(libFolderForCurrentOS, tmpPath.toString(), libName);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    static final boolean loaded = LoadWrapper();

    /**
     * Ctor
     */
    public SmartDataDistribution() {
        IDataDistribution_ptr = NativeInterface.DataDistribution_create();
    }

    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file Configuration file to use
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String conf_file) {
        return Initialize(conf_file, null, null);
    }

    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file    Configuration file to use
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String conf_file, String topicTrailer) {
        return Initialize(conf_file, null, topicTrailer);
    }

    /**
     * Initialize the instance using configuration file
     * 
     * @param conf_file    Configuration file to use
     * @param szMyAddress  The name of the server hosting the process
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String conf_file, String szMyAddress, String topicTrailer) {
        m_DataDistributionCallbackLow = NativeCallbackManager.RegisterCallback((IDataDistributionCallbackLow) this);

        m_InitializeResult = NativeInterface.IDataDistribution_Initialize(IDataDistribution_ptr,
                m_DataDistributionCallbackLow, conf_file, szMyAddress, topicTrailer);

        if (m_InitializeResult == 0) {
            IDataDistributionSubsystemManager_ptr = NativeInterface
                    .IDataDistribution_GetSubsystemManager(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionCallbackLow) this);
        }
        return new HRESULT(m_InitializeResult);
    }

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @return {@link HRESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    public HRESULT Initialize(IConfiguration configuration) throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, null);
    }

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @param topicTrailer  Trailer string to append on channel names
     * @return {@link HRESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    public HRESULT Initialize(IConfiguration configuration, String topicTrailer) throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, topicTrailer);
    }

    /**
     * Initialize the instance using a configuration instance
     * 
     * @param configuration The configuration coming from an instance of
     *                      {@link IConfiguration}
     * @param szMyAddress   The name of the server hosting the process
     * @param topicTrailer  Trailer string to append on channel names
     * @return {@link HRESULT}
     * @throws IllegalArgumentException @see {@link IConfiguration}
     */
    public HRESULT Initialize(IConfiguration configuration, String szMyAddress, String topicTrailer)
            throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), szMyAddress, topicTrailer);
    }

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams array of key=value parameters
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String[] arrayParams) {
        return Initialize(arrayParams, null, null);
    }

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams  array of key=value parameters
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String[] arrayParams, String topicTrailer) {
        return Initialize(arrayParams, null, topicTrailer);
    }

    /**
     * Initialize the instance using a set of key=value pairs
     * 
     * @param arrayParams  array of key=value parameters
     * @param szMyAddress  The name of the server hosting the process
     * @param topicTrailer Trailer string to append on channel names
     * @return {@link HRESULT}
     */
    public HRESULT Initialize(String[] arrayParams, String szMyAddress, String topicTrailer) {
        m_DataDistributionCallbackLow = NativeCallbackManager.RegisterCallback((IDataDistributionCallbackLow) this);

        m_InitializeResult = NativeInterface.IDataDistribution_Initialize2(IDataDistribution_ptr,
                m_DataDistributionCallbackLow, arrayParams, szMyAddress, topicTrailer);

        if (m_InitializeResult == 0) {
            IDataDistributionSubsystemManager_ptr = NativeInterface
                    .IDataDistribution_GetSubsystemManager(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionCallbackLow) this);
        }
        return new HRESULT(m_InitializeResult);
    }

    /**
     * Request to allocate mastership manager
     * 
     * @param serverName The server name
     * @param parameters Paramaters to send to underlying layer
     * @return {@link HRESULT}
     */
    public HRESULT RequestMastershipManager(String serverName, String[] parameters) {
        if (m_SmartDataDistributionMastership != null)
            return HRESULT.S_OK;

        m_DataDistributionMastershipCallbackLow = NativeCallbackManager
                .RegisterCallback((IDataDistributionMastershipCallbackLow) this);

        long res = NativeInterface.IDataDistribution_RequestMastershipManager(IDataDistribution_ptr,
                m_DataDistributionMastershipCallbackLow, serverName, parameters);

        HRESULT hRes = new HRESULT(res);

        if (hRes == HRESULT.S_OK) {
            m_SmartDataDistributionMastership = new SmartDataDistributionMastership(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionMastershipCallbackLow) this);
        }

        return hRes;
    }

    SmartDataDistributionMastership m_SmartDataDistributionMastership;

    /**
     * 
     * @return Returns {@link ISmartDataDistributionMastership}
     */
    public ISmartDataDistributionMastership getMastershipManager() {
        return m_SmartDataDistributionMastership;
    }

    /**
     * Starts the manager
     * 
     * @param timeout Timeout in ms
     * @return {@link HRESULT}
     */
    public HRESULT Start(int timeout) {
        long res = -1;
        if (IDataDistributionSubsystemManager_ptr != 0) {
            res = NativeInterface.IDataDistributionSubsystem_Start(IDataDistributionSubsystemManager_ptr, timeout);
        }
        return new HRESULT(res);
    }

    /**
     * Stops the manager
     * 
     * @param timeout Timeout in ms
     * @return {@link HRESULT}
     */
    public HRESULT Stop(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Stop(IDataDistributionSubsystemManager_ptr, timeout);
        return new HRESULT(res);
    }

    /**
     * 
     * @return Return the protocol in use
     */
    public String getProtocol() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocol(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    /**
     * 
     * @return Return the protocol library in use
     */
    public String getProtocolLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocolLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    /**
     * 
     * @return Return the mastership library in use
     */
    public String getMastershipLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetMastershipLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    /**
     * Set global parameter
     * 
     * @param paramName  Parameter name to set
     * @param paramValue Parameter value to set
     */
    public void SetParameter(String paramName, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, 0, paramName,
                paramValue);
    }

    /**
     * Set global parameter
     * 
     * @param paramId    Parameter {@link DDM_GENERAL_PARAMETER} to set
     * @param paramValue Parameter value to set
     */
    public void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramId.atomicNumber, paramValue);
    }

    /**
     * Get global parameter
     * 
     * @param paramName Parameter name to get
     * @return Parameter value
     */
    public String GetParameter(String paramName) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramName);
    }

    /**
     * Get global parameter
     * 
     * @param paramId Parameter {@link DDM_GENERAL_PARAMETER} to get
     * @return Parameter value
     */
    public String GetParameter(DDM_GENERAL_PARAMETER paramId) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramId.atomicNumber);
    }

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
    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName)
            throws Throwable {
        return CreateSmartChannel(clazz, channelName, (IConfiguration) null, DDM_CHANNEL_DIRECTION.ALL);
    }

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
    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration) throws Throwable {
        return CreateSmartChannel(clazz, channelName, configuration, DDM_CHANNEL_DIRECTION.ALL);
    }

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
    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration, DDM_CHANNEL_DIRECTION direction) throws Throwable {
        return CreateSmartChannel(clazz, channelName, direction,
                (configuration == null) ? null : configuration.getConfiguration());
    }

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
    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            DDM_CHANNEL_DIRECTION direction, String[] arrayParams) throws Throwable {
        if (IDataDistributionSubsystemManager_ptr == 0)
            return null;

        Object newObject = Class.forName(clazz.getName()).getDeclaredConstructor().newInstance();
        SmartDataDistributionChannel inm = (SmartDataDistributionChannel) newObject;

        long handle = NativeInterface.IDataDistributionSubsystem_CreateChannel(IDataDistributionSubsystemManager_ptr,
                channelName, inm.m_DataDistributionChannelCallbackLow, direction.atomicNumber, arrayParams);

        if (handle == 0) {
            throw new Exception("Unable to create channel, see log for the reason.");
        }

        inm.m_direction = direction;
        inm.IDataDistributionSubsystemManager_ptr = IDataDistributionSubsystemManager_ptr;
        inm.channelHandle = handle;

        return (T) inm;
    }

    /**
     * Adds a {@link IConfigurationListener} listener
     * 
     * @param listener {@link IConfigurationListener} listener to add
     */
    public void addListener(IConfigurationListener listener) {
        m_IConfigurationListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IConfigurationListener} listener
     * 
     * @param listener {@link IConfigurationListener} listener to remove
     */
    public void removeListener(IConfigurationListener listener) {
        m_IConfigurationListener_listeners.removeListener(listener);
    }

    public final String OnConfiguration(long IDataDistribution_nativePtr, String channelName, String key,
            String value) {
        if (m_IConfigurationListener_listeners.hasElements()) {
            String retVal = value;
            for (IConfigurationListener iConfigurationListener : m_IConfigurationListener_listeners) {
                retVal = iConfigurationListener.OnConfiguration(channelName, key, value);
            }
            return retVal;
        } else
            return this.OnConfiguration(channelName, key, value);
    }

    /**
     * Request a configuration validation
     * 
     * @param channelName The channel requesting. null for global parameters
     * @param key         The configuration key
     * @param value       The configuration value
     * @return The value or an updated one
     */
    public String OnConfiguration(String channelName, String key, String value) {
        return value;
    }

    /**
     * Adds a {@link ILoggingListener} listener
     * 
     * @param listener {@link ILoggingListener} listener to add
     */
    public void addListener(ILoggingListener listener) {
        m_ILoggingListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link ILoggingListener} listener
     * 
     * @param listener {@link ILoggingListener} listener to remove
     */
    public void removeListener(ILoggingListener listener) {
        m_ILoggingListener_listeners.removeListener(listener);
    }

    public final void OnLogging(long IDataDistribution_nativePtr, int level, String source, String function,
            String errStr) {
        this.OnLogging(DDM_LOG_LEVEL.valueOfAtomicNumber(level), source, function, errStr);
        for (ILoggingListener iLoggingListener : m_ILoggingListener_listeners) {
            iLoggingListener.OnLogging(DDM_LOG_LEVEL.valueOfAtomicNumber(level), source, function, errStr);
        }
    }

    /**
     * Emits logs
     * 
     * @param level    {@link DDM_LOG_LEVEL} log level
     * @param source   The log source
     * @param function The function source
     * @param logStr   The log string
     */
    public void OnLogging(DDM_LOG_LEVEL level, String source, String function, String logStr) {

    }

    /**
     * Adds a {@link ICompletelyDisconnectedListener} listener
     * 
     * @param listener {@link ICompletelyDisconnectedListener} listener to add
     */
    public void addListener(ICompletelyDisconnectedListener listener) {
        m_ICompletelyDisconnectedListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link ICompletelyDisconnectedListener} listener
     * 
     * @param listener {@link ICompletelyDisconnectedListener} listener to remove
     */
    public void removeListener(ICompletelyDisconnectedListener listener) {
        m_ICompletelyDisconnectedListener_listeners.removeListener(listener);
    }

    public void OnCompletelyDisconnected(long IDataDistribution_nativePtr, String channelName, String reason) {
        OnCompletelyDisconnected(channelName, reason);
        for (ICompletelyDisconnectedListener iCompletelyDisconnectedListener : m_ICompletelyDisconnectedListener_listeners) {
            iCompletelyDisconnectedListener.OnCompletelyDisconnected(channelName, reason);
        }
    }

    /**
     * Called when a critical condition disconnects the instance from the server
     * 
     * @param channelName The channel name
     * @param reason      Disconnection reason
     */
    public void OnCompletelyDisconnected(String channelName, String reason) {

    }

    // IDataDistributionMastershipCallback

    /**
     * Adds a {@link IClusterStateChangeListener} listener
     * 
     * @param listener {@link IClusterStateChangeListener} listener to add
     */
    public void addListener(IClusterStateChangeListener listener) {
        m_IClusterStateChangeListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IClusterStateChangeListener} listener
     * 
     * @param listener {@link IClusterStateChangeListener} listener to remove
     */
    public void removeListener(IClusterStateChangeListener listener) {
        m_IClusterStateChangeListener_listeners.removeListener(listener);
    }

    public final void OnClusterStateChange(long IDataDistribution_nativePtr, int change, long serverid) {
        DDM_CLUSTEREVENT changDDM = DDM_CLUSTEREVENT.valueOfAtomicNumber(change);
        OnClusterStateChange(changDDM, serverid);
        for (IClusterStateChangeListener iClusterStateChangeListener : m_IClusterStateChangeListener_listeners) {
            iClusterStateChangeListener.OnClusterStateChange(changDDM, serverid);
        }
    }

    /**
     * The cluster has changed its state
     * 
     * @param change   New cluster event
     * @param serverid Server has emitted event
     */
    public void OnClusterStateChange(DDM_CLUSTEREVENT change, long serverid) {

    }

    /**
     * Adds a {@link IStateChangeListener} listener
     * 
     * @param listener {@link IStateChangeListener} listener to add
     */
    public void addListener(IStateChangeListener listener) {
        m_IStateChangeListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IStateChangeListener} listener
     * 
     * @param listener {@link IStateChangeListener} listener to remove
     */
    public void removeListener(IStateChangeListener listener) {
        m_IStateChangeListener_listeners.removeListener(listener);
    }

    public final void OnStateChange(long IDataDistribution_nativePtr, int newState, int oldState) {
        DDM_INSTANCE_STATE newStateDDM = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        DDM_INSTANCE_STATE oldStateDDM = DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState);
        OnStateChange(newStateDDM, oldStateDDM);
        for (IStateChangeListener iStateChangeListener : m_IStateChangeListener_listeners) {
            iStateChangeListener.OnStateChange(newStateDDM, oldStateDDM);
        }
    }

    /**
     * My state has changed
     * 
     * @param newState New state
     * @param oldState Old state
     */
    public void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) {

    }

    /**
     * Adds a {@link IStateReadyListener} listener
     * 
     * @param listener {@link IStateReadyListener} listener to add
     */
    public void addListener(IStateReadyListener listener) {
        m_IStateReadyListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IStateReadyListener} listener
     * 
     * @param listener {@link IStateReadyListener} listener to remove
     */
    public void removeListener(IStateReadyListener listener) {
        m_IStateReadyListener_listeners.removeListener(listener);
    }

    public final void OnStateReady(long IDataDistribution_nativePtr, byte[] buffer) {
        OnStateReady(buffer);
        for (IStateReadyListener iStateReadyListener : m_IStateReadyListener_listeners) {
            iStateReadyListener.OnStateReady(buffer);
        }
    }

    /**
     * The state is ready to be used
     * 
     * @param buffer The buffer state receiver
     */
    public void OnStateReady(byte[] buffer) {

    }

    /**
     * Adds a {@link IRequestedStateListener} listener
     * 
     * @param listener {@link IRequestedStateListener} listener to add
     */
    public void addListener(IRequestedStateListener listener) {
        m_IRequestedStateListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IRequestedStateListener} listener
     * 
     * @param listener {@link IRequestedStateListener} listener to remove
     */
    public void removeListener(IRequestedStateListener listener) {
        m_IRequestedStateListener_listeners.removeListener(listener);
    }

    public final void OnRequestedState(long IDataDistribution_nativePtr, byte[] buffer) {
        if (m_IRequestedStateListener_listeners.hasElements()) {
            for (IRequestedStateListener iRequestedStateListener : m_IRequestedStateListener_listeners) {
                iRequestedStateListener.OnRequestedState(buffer);
            }
        } else
            OnRequestedState(buffer);
    }

    /**
     * Called when a state transfer was requested
     * 
     * @param buffer Pointer to the buffer
     */
    public void OnRequestedState(byte[] buffer) {

    }

    /**
     * Adds a {@link IMultiplePrimaryListener} listener
     * 
     * @param listener {@link IMultiplePrimaryListener} listener to add
     */
    public void addListener(IMultiplePrimaryListener listener) {
        m_IMultiplePrimaryListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IMultiplePrimaryListener} listener
     * 
     * @param listener {@link IMultiplePrimaryListener} listener to remove
     */
    public void removeListener(IMultiplePrimaryListener listener) {
        m_IMultiplePrimaryListener_listeners.removeListener(listener);
    }

    public final void OnMultiplePrimary(long IDataDistribution_nativePtr, long myId, long otherId) {
        OnMultiplePrimary(myId, otherId);
        for (IMultiplePrimaryListener iMultiplePrimaryListener : m_IMultiplePrimaryListener_listeners) {
            iMultiplePrimaryListener.OnMultiplePrimary(myId, otherId);
        }
    }

    /**
     * There are multiple primary server in the cluster
     * 
     * @param myId    My identifier
     * @param otherId Other identifier which is primary
     */
    public void OnMultiplePrimary(long myId, long otherId) {

    }

    /**
     * Adds a {@link IFirstStateChangeListener} listener
     * 
     * @param listener {@link IFirstStateChangeListener} listener to add
     */
    public void addListener(IFirstStateChangeListener listener) {
        m_IFirstStateChangeListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IFirstStateChangeListener} listener
     * 
     * @param listener {@link IFirstStateChangeListener} listener to remove
     */
    public void removeListener(IFirstStateChangeListener listener) {
        m_IFirstStateChangeListener_listeners.removeListener(listener);
    }

    public final void FirstStateChange(long IDataDistribution_nativePtr, int newState) {
        DDM_INSTANCE_STATE state = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnFirstStateChange(state);
        for (IFirstStateChangeListener iFirstStateChangeListener : m_IFirstStateChangeListener_listeners) {
            iFirstStateChangeListener.OnFirstStateChange(state);
        }
    }

    /**
     * Called the first time there is state change
     * 
     * @param newState New state
     */
    public void OnFirstStateChange(DDM_INSTANCE_STATE newState) {

    }

    /**
     * Adds a {@link IChangingStateListener} listener
     * 
     * @param listener {@link IChangingStateListener} listener to add
     */
    public void addListener(IChangingStateListener listener) {
        m_IChangingStateListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IChangingStateListener} listener
     * 
     * @param listener {@link IChangingStateListener} listener to remove
     */
    public void removeListener(IChangingStateListener listener) {
        m_IChangingStateListener_listeners.removeListener(listener);
    }

    public final void ChangingState(long IDataDistribution_nativePtr, int oldState, int newState) {
        DDM_INSTANCE_STATE oS = DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState);
        DDM_INSTANCE_STATE nS = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnChangingState(oS, nS);
        for (IChangingStateListener iChangingStateListener : m_IChangingStateListener_listeners) {
            iChangingStateListener.OnChangingState(oS, nS);
        }
    }

    /**
     * The state is starting to change
     * 
     * @param oldState Old state
     * @param newState New state
     */
    public void OnChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) {

    }

    /**
     * Adds a {@link IChangedStateListener} listener
     * 
     * @param listener {@link IChangedStateListener} listener to add
     */
    public void addListener(IChangedStateListener listener) {
        m_IChangedStateListener_listeners.addListener(listener);
    }

    /**
     * Removes a {@link IChangedStateListener} listener
     * 
     * @param listener {@link IChangedStateListener} listener to remove
     */
    public void removeListener(IChangedStateListener listener) {
        m_IChangedStateListener_listeners.removeListener(listener);
    }

    public final void ChangedState(long IDataDistribution_nativePtr, int newState) {
        DDM_INSTANCE_STATE s = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnChangedState(s);
        for (IChangedStateListener iChangedStateListener : m_IChangedStateListener_listeners) {
            iChangedStateListener.OnChangedState(s);
        }
    }

    /**
     * The state has changed
     * 
     * @param newState New state
     */
    public void OnChangedState(DDM_INSTANCE_STATE newState) {

    }
}
