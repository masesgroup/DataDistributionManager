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

public class SmartDataDistribution implements IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow {
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

        String extractedLibFileName = containerFileName;
        File extractedLibFile = new File(targetFolder, extractedLibFileName);

        try {
            // Extract file into the current directory
            InputStream reader = SmartDataDistribution.class.getResourceAsStream(nativeLibraryFilePath);
            FileOutputStream writer = new FileOutputStream(extractedLibFile);
            byte[] buffer = new byte[1024];
            int bytesRead = 0;
            while ((bytesRead = reader.read(buffer)) != -1) {
                writer.write(buffer, 0, bytesRead);
            }

            writer.close();
            reader.close();

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

    public SmartDataDistribution() {
        IDataDistribution_ptr = NativeInterface.DataDistribution_create();
    }

    public HRESULT Initialize(String conf_file) throws IllegalArgumentException {
        return Initialize(conf_file, null, null);
    }

    public HRESULT Initialize(String conf_file, String topicTrailer) throws IllegalArgumentException {
        return Initialize(conf_file, null, topicTrailer);
    }

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

    public HRESULT Initialize(IConfiguration configuration) throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, null);
    }

    public HRESULT Initialize(IConfiguration configuration, String topicTrailer) throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), null, topicTrailer);
    }

    public HRESULT Initialize(IConfiguration configuration, String szMyAddress, String topicTrailer)
            throws IllegalArgumentException {
        return Initialize(configuration.getConfiguration(), szMyAddress, topicTrailer);
    }

    public HRESULT Initialize(String[] arrayParams) throws IllegalArgumentException {
        return Initialize(arrayParams, null, null);
    }

    public HRESULT Initialize(String[] arrayParams, String topicTrailer) throws IllegalArgumentException {
        return Initialize(arrayParams, null, topicTrailer);
    }

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

    public ISmartDataDistributionMastership getMastershipManager() {
        return m_SmartDataDistributionMastership;
    }

    public HRESULT Start(int dwMilliseconds) {
        long res = -1;
        if (IDataDistributionSubsystemManager_ptr != 0) {
            res = NativeInterface.IDataDistributionSubsystem_Start(IDataDistributionSubsystemManager_ptr,
                    dwMilliseconds);
        }
        return new HRESULT(res);
    }

    public HRESULT Stop(int dwMilliseconds) {
        long res = NativeInterface.IDataDistributionSubsystem_Stop(IDataDistributionSubsystemManager_ptr,
                dwMilliseconds);
        return new HRESULT(res);
    }

    public String GetProtocol() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocol(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public String GetProtocolLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetProtocolLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public String GetMastershipLib() {
        if (IDataDistributionSubsystemManager_ptr != 0) {
            return NativeInterface.IDataDistribution_GetMastershipLib(IDataDistributionSubsystemManager_ptr);
        }
        return "";
    }

    public <T extends SmartDataDistributionChannel> T CeateSmartChannel(Class<T> clazz, String topicName)
            throws Throwable {
        return CeateSmartChannel(clazz, topicName, DDM_CHANNEL_DIRECTION.ALL, (IConfiguration) null);
    }

    public <T extends SmartDataDistributionChannel> T CeateSmartChannel(Class<T> clazz, String topicName,
            DDM_CHANNEL_DIRECTION direction) throws Throwable {
        return CeateSmartChannel(clazz, topicName, direction, (IConfiguration) null);
    }

    public <T extends SmartDataDistributionChannel> T CeateSmartChannel(Class<T> clazz, String topicName,
            DDM_CHANNEL_DIRECTION direction, IConfiguration configuration) throws Throwable {
        return CeateSmartChannel(clazz, topicName, direction,
                (configuration == null) ? null : configuration.getConfiguration());
    }

    @SuppressWarnings("unchecked")
    public <T extends SmartDataDistributionChannel> T CeateSmartChannel(Class<T> clazz, String topicName,
            DDM_CHANNEL_DIRECTION direction, String[] arrayParams) throws Throwable {
        if (IDataDistributionSubsystemManager_ptr == 0)
            return null;

        Object newObject = Class.forName(clazz.getName()).getDeclaredConstructor().newInstance();
        SmartDataDistributionChannel inm = (SmartDataDistributionChannel) newObject;

        long handle = NativeInterface.IDataDistributionSubsystem_CreateChannel(IDataDistributionSubsystemManager_ptr,
                topicName, inm.m_DataDistributionChannelCallbackLow, direction.atomicNumber, arrayParams);

        if (handle == 0) {
            throw new Exception("Unable to create channel, see log for the reason.");
        }

        inm.m_direction = direction;
        inm.IDataDistributionSubsystemManager_ptr = IDataDistributionSubsystemManager_ptr;
        inm.topicHandle = handle;

        return (T) inm;
    }

    public final String OnConfiguration(long IDataDistribution_nativePtr, String key, String value) {
        return this.OnConfiguration(key, value);
    }

    public String OnConfiguration(String key, String value) {
        return value;
    }

    public final void OnLogging(long IDataDistribution_nativePtr, int level, String source, String function,
            String errStr) {
        this.OnLogging(DDM_LOG_LEVEL.valueOfAtomicNumber(level), source, function, errStr);
    }

    public void OnLogging(DDM_LOG_LEVEL level, String source, String function, String errStr) {

    }

    public void OnCompletelyDisconnected(long IDataDistribution_nativePtr, String topicName, String reason) {
        OnCompletelyDisconnected(topicName, reason);
    }

    public void OnCompletelyDisconnected(String topicName, String reason) {

    }

    // IDataDistributionMastershipCallback
    public final void OnClusterStateChange(long IDataDistribution_nativePtr, int change, long serverid) {
        OnClusterStateChange(DDM_CLUSTEREVENT.valueOfAtomicNumber(change), serverid);
    }

    public void OnClusterStateChange(DDM_CLUSTEREVENT change, long serverid) {

    }

    public final void OnStateChange(long IDataDistribution_nativePtr, int newState, int oldState) {
        OnStateChange(DDM_INSTANCE_STATE.valueOfAtomicNumber(newState),
                DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState));
    }

    public void OnStateChange(DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState) {

    }

    public final void OnStateReady(long IDataDistribution_nativePtr, byte[] buffer) {
        OnStateReady(buffer);
    }

    public void OnStateReady(byte[] buffer) {

    }

    public final void OnRequestedState(long IDataDistribution_nativePtr, byte[] buffer) {
        OnRequestedState(buffer);
    }

    public void OnRequestedState(byte[] buffer) {

    }

    public final void OnMultiplePrimary(long IDataDistribution_nativePtr, long myId, long otherId) {
        OnMultiplePrimary(myId, otherId);
    }

    public void OnMultiplePrimary(long myId, long otherId) {

    }

    public final void FirstStateChange(long IDataDistribution_nativePtr, int newState) {
        FirstStateChange(DDM_INSTANCE_STATE.valueOfAtomicNumber(newState));
    }

    public void FirstStateChange(DDM_INSTANCE_STATE newState) {

    }

    public final void ChangingState(long IDataDistribution_nativePtr, int oldState, int newState) {
        ChangingState(DDM_INSTANCE_STATE.valueOfAtomicNumber(oldState),
                DDM_INSTANCE_STATE.valueOfAtomicNumber(newState));
    }

    public void ChangingState(DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState) {

    }

    public final void ChangedState(long IDataDistribution_nativePtr, int newState) {
        ChangedState(DDM_INSTANCE_STATE.valueOfAtomicNumber(newState));
    }

    public void ChangedState(DDM_INSTANCE_STATE newState) {

    }
}
