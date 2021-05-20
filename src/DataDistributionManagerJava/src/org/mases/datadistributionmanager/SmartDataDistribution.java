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
public class SmartDataDistribution
        implements ISmartDataDistribution, IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow {
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

    public OPERATION_RESULT Initialize(String conf_file) {
        return Initialize(conf_file, null, null);
    }

    public OPERATION_RESULT Initialize(String conf_file, String topicTrailer) {
        return Initialize(conf_file, null, topicTrailer);
    }

    public OPERATION_RESULT Initialize(String conf_file, String szMyAddress, String topicTrailer) {
        m_DataDistributionCallbackLow = NativeCallbackManager.RegisterCallback((IDataDistributionCallbackLow) this);

        m_InitializeResult = NativeInterface.IDataDistribution_Initialize(IDataDistribution_ptr,
                m_DataDistributionCallbackLow, conf_file, szMyAddress, topicTrailer);

        if (m_InitializeResult == 0) {
            IDataDistributionSubsystemManager_ptr = NativeInterface
                    .IDataDistribution_GetSubsystemManager(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionCallbackLow) this);
        }
        return new OPERATION_RESULT(m_InitializeResult);
    }

    public OPERATION_RESULT Initialize(IConfiguration configuration) throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, null);
    }

    public OPERATION_RESULT Initialize(IConfiguration configuration, String topicTrailer)
            throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, topicTrailer);
    }

    public OPERATION_RESULT Initialize(IConfiguration configuration, String szMyAddress, String topicTrailer)
            throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), szMyAddress, topicTrailer);
    }

    public OPERATION_RESULT Initialize(String[] arrayParams) {
        return Initialize(arrayParams, null, null);
    }

    public OPERATION_RESULT Initialize(String[] arrayParams, String topicTrailer) {
        return Initialize(arrayParams, null, topicTrailer);
    }

    public OPERATION_RESULT Initialize(String[] arrayParams, String szMyAddress, String topicTrailer) {
        m_DataDistributionCallbackLow = NativeCallbackManager.RegisterCallback((IDataDistributionCallbackLow) this);

        m_InitializeResult = NativeInterface.IDataDistribution_Initialize2(IDataDistribution_ptr,
                m_DataDistributionCallbackLow, arrayParams, szMyAddress, topicTrailer);

        if (m_InitializeResult == 0) {
            IDataDistributionSubsystemManager_ptr = NativeInterface
                    .IDataDistribution_GetSubsystemManager(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionCallbackLow) this);
        }
        return new OPERATION_RESULT(m_InitializeResult);
    }

    public OPERATION_RESULT RequestMastershipManager() {
        return RequestMastershipManager(null, (IConfiguration) null);
    }

    public OPERATION_RESULT RequestMastershipManager(String[] parameters) {
        return RequestMastershipManager(null, parameters);
    }

    public OPERATION_RESULT RequestMastershipManager(IConfiguration parameters) {
        return RequestMastershipManager(null, parameters != null ? parameters.getConfiguration() : null);
    }

    public OPERATION_RESULT RequestMastershipManager(String serverName) {
        return RequestMastershipManager(serverName, (IConfiguration) null);
    }

    public OPERATION_RESULT RequestMastershipManager(String serverName, IConfiguration parameters) {
        return RequestMastershipManager(serverName, parameters != null ? parameters.getConfiguration() : null);
    }

    public OPERATION_RESULT RequestMastershipManager(String serverName, String[] parameters) {
        if (m_SmartDataDistributionMastership != null)
            return OPERATION_RESULT.S_OK;

        m_DataDistributionMastershipCallbackLow = NativeCallbackManager
                .RegisterCallback((IDataDistributionMastershipCallbackLow) this);

        long res = NativeInterface.IDataDistribution_RequestMastershipManager(IDataDistribution_ptr,
                m_DataDistributionMastershipCallbackLow, serverName, parameters);

        OPERATION_RESULT hRes = new OPERATION_RESULT(res);

        if (hRes == OPERATION_RESULT.S_OK) {
            m_SmartDataDistributionMastership = new SmartDataDistributionMastership(IDataDistribution_ptr);
        } else {
            NativeCallbackManager.UnregisterCallback((IDataDistributionMastershipCallbackLow) this);
        }

        return hRes;
    }

    SmartDataDistributionMastership m_SmartDataDistributionMastership;

    public ISmartDataDistributionMastership getMastershipManager() {
        return m_SmartDataDistributionMastership;
    }

    public OPERATION_RESULT Start(int timeout) {
        long res = -1;
        if (IDataDistributionSubsystemManager_ptr != 0) {
            res = NativeInterface.IDataDistributionSubsystem_Start(IDataDistributionSubsystemManager_ptr, timeout);
        }
        return new OPERATION_RESULT(res);
    }

    public OPERATION_RESULT Stop(int timeout) {
        long res = NativeInterface.IDataDistributionSubsystem_Stop(IDataDistributionSubsystemManager_ptr, timeout);
        return new OPERATION_RESULT(res);
    }

    public String getProtocol() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocol(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public String getProtocolLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocolLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public String getMastershipLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetMastershipLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public void SetParameter(String paramName, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, 0, paramName,
                paramValue);
    }

    public void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue) {
        NativeInterface.IDataDistributionSubsystem_SetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramId.atomicNumber, paramValue);
    }

    public String GetParameter(String paramName) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramName);
    }

    public String GetParameter(DDM_GENERAL_PARAMETER paramId) {
        return NativeInterface.IDataDistributionSubsystem_GetParameter(IDataDistributionSubsystemManager_ptr, 0,
                paramId.atomicNumber);
    }

    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName)
            throws Throwable {
        return CreateSmartChannel(clazz, channelName, (IConfiguration) null, DDM_CHANNEL_DIRECTION.ALL);
    }

    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration) throws Throwable {
        return CreateSmartChannel(clazz, channelName, configuration, DDM_CHANNEL_DIRECTION.ALL);
    }

    public <T extends SmartDataDistributionChannel> T CreateSmartChannel(Class<T> clazz, String channelName,
            IConfiguration configuration, DDM_CHANNEL_DIRECTION direction) throws Throwable {
        return CreateSmartChannel(clazz, channelName, direction,
                (configuration == null) ? null : configuration.getConfiguration());
    }

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
        inm.m_channelHandle = handle;
        inm.m_channelName = channelName;

        return (T) inm;
    }

    public void addListener(IConfigurationListener listener) {
        m_IConfigurationListener_listeners.addListener(listener);
    }

    public void removeListener(IConfigurationListener listener) {
        m_IConfigurationListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
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

    public String OnConfiguration(String channelName, String key, String value) {
        return value;
    }

    public void addListener(ILoggingListener listener) {
        m_ILoggingListener_listeners.addListener(listener);
    }

    public void removeListener(ILoggingListener listener) {
        m_ILoggingListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnLogging(long IDataDistribution_nativePtr, int level, String source, String function,
            String errStr) {
        this.OnLogging(DDM_LOG_LEVEL.valueOfAtomicNumber(level), source, function, errStr);
        for (ILoggingListener iLoggingListener : m_ILoggingListener_listeners) {
            iLoggingListener.OnLogging(DDM_LOG_LEVEL.valueOfAtomicNumber(level), source, function, errStr);
        }
    }

    public void OnLogging(DDM_LOG_LEVEL level, String source, String function, String logStr) {

    }

    public void addListener(ICompletelyDisconnectedListener listener) {
        m_ICompletelyDisconnectedListener_listeners.addListener(listener);
    }

    public void removeListener(ICompletelyDisconnectedListener listener) {
        m_ICompletelyDisconnectedListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnCompletelyDisconnected(long IDataDistribution_nativePtr, String channelName, String reason) {
        OnCompletelyDisconnected(channelName, reason);
        for (ICompletelyDisconnectedListener iCompletelyDisconnectedListener : m_ICompletelyDisconnectedListener_listeners) {
            iCompletelyDisconnectedListener.OnCompletelyDisconnected(channelName, reason);
        }
    }

    public void OnCompletelyDisconnected(String channelName, String reason) {

    }

    // IDataDistributionMastershipCallback

    public void addListener(IClusterStateChangeListener listener) {
        m_IClusterStateChangeListener_listeners.addListener(listener);
    }

    public void removeListener(IClusterStateChangeListener listener) {
        m_IClusterStateChangeListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnClusterStateChange(long IDataDistribution_nativePtr, int change, long serverid) {
        DDM_CLUSTEREVENT changDDM = DDM_CLUSTEREVENT.valueOfAtomicNumber(change);
        OnClusterStateChange(changDDM, serverid);
        for (IClusterStateChangeListener iClusterStateChangeListener : m_IClusterStateChangeListener_listeners) {
            iClusterStateChangeListener.OnClusterStateChange(changDDM, serverid);
        }
    }

    public void OnClusterStateChange(DDM_CLUSTEREVENT change, long serverid) {

    }

    public void addListener(IStateChangeListener listener) {
        m_IStateChangeListener_listeners.addListener(listener);
    }

    public void removeListener(IStateChangeListener listener) {
        m_IStateChangeListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnStateChange(long IDataDistribution_nativePtr, int newState, int oldState) {
        DDM_INSTANCE_STATE newStateDDM = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        DDM_INSTANCE_STATE oldStateDDM = DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState);
        OnStateChange(newStateDDM, oldStateDDM);
        for (IStateChangeListener iStateChangeListener : m_IStateChangeListener_listeners) {
            iStateChangeListener.OnStateChange(newStateDDM, oldStateDDM);
        }
    }

    public void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) {

    }

    public void addListener(IStateReadyListener listener) {
        m_IStateReadyListener_listeners.addListener(listener);
    }

    public void removeListener(IStateReadyListener listener) {
        m_IStateReadyListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnStateReady(long IDataDistribution_nativePtr, byte[] buffer) {
        OnStateReady(buffer);
        for (IStateReadyListener iStateReadyListener : m_IStateReadyListener_listeners) {
            iStateReadyListener.OnStateReady(buffer);
        }
    }

    public void OnStateReady(byte[] buffer) {

    }

    public void addListener(IRequestedStateListener listener) {
        m_IRequestedStateListener_listeners.addListener(listener);
    }

    public void removeListener(IRequestedStateListener listener) {
        m_IRequestedStateListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnRequestedState(long IDataDistribution_nativePtr, byte[] buffer) {
        if (m_IRequestedStateListener_listeners.hasElements()) {
            for (IRequestedStateListener iRequestedStateListener : m_IRequestedStateListener_listeners) {
                iRequestedStateListener.OnRequestedState(buffer);
            }
        } else
            OnRequestedState(buffer);
    }

    public void OnRequestedState(byte[] buffer) {

    }

    public void addListener(IMultiplePrimaryListener listener) {
        m_IMultiplePrimaryListener_listeners.addListener(listener);
    }

    public void removeListener(IMultiplePrimaryListener listener) {
        m_IMultiplePrimaryListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void OnMultiplePrimary(long IDataDistribution_nativePtr, long myId, long otherId) {
        OnMultiplePrimary(myId, otherId);
        for (IMultiplePrimaryListener iMultiplePrimaryListener : m_IMultiplePrimaryListener_listeners) {
            iMultiplePrimaryListener.OnMultiplePrimary(myId, otherId);
        }
    }

    public void OnMultiplePrimary(long myId, long otherId) {

    }

    public void addListener(IFirstStateChangeListener listener) {
        m_IFirstStateChangeListener_listeners.addListener(listener);
    }

    public void removeListener(IFirstStateChangeListener listener) {
        m_IFirstStateChangeListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void FirstStateChange(long IDataDistribution_nativePtr, int newState) {
        DDM_INSTANCE_STATE state = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnFirstStateChange(state);
        for (IFirstStateChangeListener iFirstStateChangeListener : m_IFirstStateChangeListener_listeners) {
            iFirstStateChangeListener.OnFirstStateChange(state);
        }
    }

    public void OnFirstStateChange(DDM_INSTANCE_STATE newState) {

    }

    public void addListener(IChangingStateListener listener) {
        m_IChangingStateListener_listeners.addListener(listener);
    }

    public void removeListener(IChangingStateListener listener) {
        m_IChangingStateListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void ChangingState(long IDataDistribution_nativePtr, int oldState, int newState) {
        DDM_INSTANCE_STATE oS = DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState);
        DDM_INSTANCE_STATE nS = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnChangingState(oS, nS);
        for (IChangingStateListener iChangingStateListener : m_IChangingStateListener_listeners) {
            iChangingStateListener.OnChangingState(oS, nS);
        }
    }

    public void OnChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) {

    }

    public void addListener(IChangedStateListener listener) {
        m_IChangedStateListener_listeners.addListener(listener);
    }

    public void removeListener(IChangedStateListener listener) {
        m_IChangedStateListener_listeners.removeListener(listener);
    }

    /**
     * Internal method
     */
    public final void ChangedState(long IDataDistribution_nativePtr, int newState) {
        DDM_INSTANCE_STATE s = DDM_INSTANCE_STATE.valueOfAtomicNumber(newState);
        OnChangedState(s);
        for (IChangedStateListener iChangedStateListener : m_IChangedStateListener_listeners) {
            iChangedStateListener.OnChangedState(s);
        }
    }

    public void OnChangedState(DDM_INSTANCE_STATE newState) {

    }
}
