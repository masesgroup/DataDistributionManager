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
import java.io.IOException;
import java.net.URL;
import java.nio.file.*;

public class SmartDataDistribution implements IDataDistributionCallbackLow, IDataDistributionMastershipCallbackLow {
    long m_DataDistributionCallbackLow;
    long m_DataDistributionMastershipCallbackLow;
    long IDataDistribution_ptr;
    long IDataDistributionSubsystemManager_ptr;
    long m_InitializeResult = -1;

    static boolean LoadWrapper() {
        String libName = "DataDistributionManager.dll";
        String bridgePath = null;
        if (bridgePath == null || bridgePath.isEmpty()) {
            ClassLoader loader = SmartDataDistribution.class.getClassLoader();
            URL url = loader.getResource("org/mases/datadistributionmanager/SmartDataDistribution.class");

            bridgePath = url.getFile();
            int index = bridgePath.indexOf("/datadistributionmanager.jar");
            bridgePath = bridgePath.substring(0, index);
            bridgePath = bridgePath.substring("file:/".length());

            bridgePath = bridgePath.replace("%20", " ");
        }

        if (System.getProperty("sun.arch.data.model").equals("64")) {
            bridgePath += "/x64";
        } else {
            bridgePath += "/x86";
        }

        Path rootPath = Paths.get(bridgePath);
        Path partialPath = Paths.get(libName);
        Path resolvedPath = rootPath.resolve(partialPath).normalize();

        String fileStr = resolvedPath.toString();

        String b = null;
        try {
            b = new File(fileStr).getCanonicalPath();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println(b);
        System.load(b);
        return true;
    }

    static final boolean loaded = LoadWrapper();

    public SmartDataDistribution() {
        IDataDistribution_ptr = NativeInterface.DataDistribution_create();
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
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

    @SuppressWarnings("unchecked")
    public <T extends SmartDataDistributionChannel> T CeateSmartChannel(Class<T> clazz, String topicName,
            DDM_CHANNEL_DIRECTION direction, String[] arrayParams)
            throws InstantiationException, IllegalAccessException, ClassNotFoundException {

        if (IDataDistributionSubsystemManager_ptr == 0)
            return null;

        Object newObject = Class.forName(clazz.getName()).newInstance();
        SmartDataDistributionChannel inm = (SmartDataDistributionChannel) newObject;

        long handle = NativeInterface.IDataDistributionSubsystem_CreateChannel(IDataDistributionSubsystemManager_ptr,
                topicName, inm.m_DataDistributionChannelCallbackLow, direction.atomicNumber, arrayParams);

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
