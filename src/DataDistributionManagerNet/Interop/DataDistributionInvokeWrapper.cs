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

using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;

namespace MASES.DataDistributionManager.Bindings.Interop
{
    static class DataDistributionManagerInvokeWrapper
    {
        #region native invokes

#if !NET_STANDARD
        [DllImport("kernel32.dll", EntryPoint = "LoadLibraryW")]
        static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPWStr)] string name);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32.dll", EntryPoint = "GetModuleHandleA")]
        static extern IntPtr GetModuleHandle([MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(IntPtr hModule, [MarshalAs(UnmanagedType.LPStr)] string name);
#else
        [DllImport("coredll.dll", EntryPoint="GetModuleHandleW", SetLastError=true)]
        public static extern IntPtr GetModuleHandle(string moduleName);
#endif

        internal static IntPtr WrapperLoadLibrary(string name)
        {
#if !NET_STANDARD
            return LoadLibrary(name);
#else
            return System.Runtime.InteropServices.NativeLibrary.Load(name);
#endif
        }

        internal static bool WrapperFreeLibrary(IntPtr hModule)
        {
#if !NET_STANDARD
            return FreeLibrary(hModule);
#else
            System.Runtime.InteropServices.NativeLibrary.Free(hModule);
            return true;
#endif
        }

        internal static IntPtr WrapperGetModuleHandle(string name)
        {
#if !NET_STANDARD
            return GetModuleHandle(name);
#else
            return GetModuleHandle(name);
#endif
        }

        internal static IntPtr WrapperGetProcAddress(IntPtr hModule, string name)
        {
            IntPtr result = IntPtr.Zero;
#if !NET_STANDARD
            result = GetProcAddress(hModule, name);
#else
            result = System.Runtime.InteropServices.NativeLibrary.GetExport(hModule, name);
#endif
            return result;
        }

        #endregion

        static readonly object _managerDllModuleLock = new object();
        static IntPtr _managerDllModule = IntPtr.Zero;
        static DataDistributionEnv _env;

        internal static DataDistributionEnv DataDistributionEnv
        {
            get
            {
                lock (_managerDllModuleLock)
                {
                    if (_managerDllModule == IntPtr.Zero) LoadInvokeWrapper();
                    return _env;
                }
            }
        }

#if DEBUG
        static string libName = "DataDistributionManagerd.dll";
#else
        static string libName = "DataDistributionManager.dll";
#endif
        internal static string DataDistributionManagerNetPath
        {
            get
            {
                var thisAssemblyPath = typeof(DataDistributionManagerInvokeWrapper).Assembly.Location;
                return Path.GetDirectoryName(thisAssemblyPath);
            }
        }

        internal static string DataDistributionManagerLibrary
        {
            get
            {
                var tmpLib = Path.Combine(DataDistributionManagerNetPath, Environment.Is64BitProcess ? "x64" : "x86", libName);
                return System.IO.Path.GetFullPath(tmpLib);
            }
            set { libName = value; }
        }

        /// <summary>
        /// Create the DataDistributionManager interface and wrap it.
        /// </summary>
        internal static void LoadInvokeWrapper()
        {
            lock (_managerDllModuleLock)
            {
                if (_managerDllModule == IntPtr.Zero)
                {
                    _managerDllModule = WrapperLoadLibrary(DataDistributionManagerLibrary);
                    _env = new DataDistributionEnv(_managerDllModule);
                }

                if (_managerDllModule == IntPtr.Zero)
                {
                    throw new Win32Exception(Marshal.GetLastWin32Error(), "Failed to load library.");
                }
            }
        }

        static IntPtr _DataDistributionManagerEnvLocal = IntPtr.Zero;
        static IntPtr DataDistributionManagerEnvLocal { get { return (_DataDistributionManagerEnvLocal == IntPtr.Zero) ? _managerDllModule : _DataDistributionManagerEnvLocal; } }

        static T GetDelegateFromDataDistributionManager<T>(string procName)
            where T : class
        {
            return Marshal.GetDelegateForFunctionPointer(WrapperGetProcAddress(_managerDllModule, procName), typeof(T)) as T;
        }

        static A ExceptionCheckedDataDistributionManagerCall<A>(Func<IntPtr, A> func)
        {
            return ExceptionCheckedDataDistributionManagerCall(() => func(DataDistributionManagerEnvLocal));
        }

        static B ExceptionCheckedDataDistributionManagerCall<A, B>(Func<IntPtr, A, B> func, A arg1)
        {
            return ExceptionCheckedDataDistributionManagerCall(() => func(DataDistributionManagerEnvLocal, arg1));
        }

        static C ExceptionCheckedDataDistributionManagerCall<A, B, C>(Func<IntPtr, A, B, C> func, A arg1, B arg2)
        {
            return ExceptionCheckedDataDistributionManagerCall(() => func(DataDistributionManagerEnvLocal, arg1, arg2));
        }

        static D ExceptionCheckedDataDistributionManagerCall<A, B, C, D>(Func<IntPtr, A, B, C, D> func, A arg1, B arg2, C arg3)
        {
            return ExceptionCheckedDataDistributionManagerCall(() => func(DataDistributionManagerEnvLocal, arg1, arg2, arg3));
        }

        static E ExceptionCheckedDataDistributionManagerCall<A, B, C, D, E>(Func<IntPtr, A, B, C, D, E> func, A arg1, B arg2, C arg3, D arg4)
        {
            return ExceptionCheckedDataDistributionManagerCall(() => func(DataDistributionManagerEnvLocal, arg1, arg2, arg3, arg4));
        }

        static void ExceptionCheckedDataDistributionManagerCallA(Action<IntPtr> func)
        {
            ExceptionCheckedDataDistributionManagerCall<object>(() => { func(DataDistributionManagerEnvLocal); return null; });
        }

        static void ExceptionCheckedDataDistributionManagerCallA<A>(Action<IntPtr, A> func, A arg1)
        {
            ExceptionCheckedDataDistributionManagerCall<object>(() => { func(DataDistributionManagerEnvLocal, arg1); return null; });
        }

        static void ExceptionCheckedDataDistributionManagerCallA<A, B>(Action<IntPtr, A, B> func, A arg1, B arg2)
        {
            ExceptionCheckedDataDistributionManagerCall<object>(() => { func(DataDistributionManagerEnvLocal, arg1, arg2); return null; });
        }

        static void ExceptionCheckedDataDistributionManagerCallA<A, B, C>(Action<IntPtr, A, B, C> func, A arg1, B arg2, C arg3)
        {
            ExceptionCheckedDataDistributionManagerCall<object>(() => { func(DataDistributionManagerEnvLocal, arg1, arg2, arg3); return null; });
        }

        static void ExceptionCheckedDataDistributionManagerCallA<A, B, C, D>(Action<IntPtr, A, B, C, D> func, A arg1, B arg2, C arg3, D arg4)
        {
            ExceptionCheckedDataDistributionManagerCall<object>(() => { func(DataDistributionManagerEnvLocal, arg1, arg2, arg3, arg4); return null; });
        }

        static T ExceptionCheckedDataDistributionManagerCall<T>(Func<T> func)
        {
            T ret = func();
            return ret;
        }

        [return: MarshalAs(UnmanagedType.LPStr)]
        public delegate string dataDistributionConfigurationCb(IntPtr opaque, [MarshalAs(UnmanagedType.LPStr)] string key, [MarshalAs(UnmanagedType.LPStr)] string value);
        public delegate void dataDistributionLoggingCb(IntPtr opaque, DDM_LOG_LEVEL level, [MarshalAs(UnmanagedType.LPStr)] string source, [MarshalAs(UnmanagedType.LPStr)] string function, [MarshalAs(UnmanagedType.LPStr)] string errorStr);
        public delegate void dataDistributionCompletelyDisconnectedCb(IntPtr opaque, [MarshalAs(UnmanagedType.LPStr)] string source, [MarshalAs(UnmanagedType.LPStr)] string errorStr);

        public delegate void dataDistributionOnDataAvailable(IntPtr opaque, IntPtr channelHandle, IntPtr buffer, IntPtr len);
        public delegate void dataDistributionOnConditionOrError(IntPtr opaque, IntPtr channelHandle, OPERATION_RESULT errorCode, int nativeCode, [MarshalAs(UnmanagedType.LPStr)] string subSystemReason);

        public delegate IntPtr DataDistribution_create();
        public delegate IntPtr DataDistributionCallback_create(IntPtr opaque, dataDistributionConfigurationCb configurationCb, dataDistributionLoggingCb logCb, dataDistributionCompletelyDisconnectedCb disconnectedCb);
        public delegate IntPtr DataDistributionChannelCallback_create(IntPtr opaque, dataDistributionOnDataAvailable dataAvailable, dataDistributionOnConditionOrError errCond);

        // IDataDistribution interface
        public delegate OPERATION_RESULT IDataDistribution_Initialize(IntPtr IDataDistribution_instance, IntPtr iddcb, IntPtr iddmcb,
                                                          [MarshalAs(UnmanagedType.LPStr)] string conf_file,
                                                          [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);


        public delegate OPERATION_RESULT IDataDistribution_Initialize2(IntPtr IDataDistribution_instance, IntPtr iddcb, IntPtr iddmcb,
                                                           [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams, int len,
                                                           [MarshalAs(UnmanagedType.LPStr)] string szMyAddress, [MarshalAs(UnmanagedType.LPStr)] string channelTrailer);

        public delegate IntPtr IDataDistribution_GetSubsystemManager(IntPtr IDataDistribution_instance);
        public delegate IntPtr IDataDistribution_GetMastershipManager(IntPtr IDataDistribution_instance);

        public delegate IntPtr IDataDistribution_GetProtocol(IntPtr IDataDistribution_instance);
        public delegate IntPtr IDataDistribution_GetProtocolLib(IntPtr IDataDistribution_instance);
        public delegate IntPtr IDataDistribution_GetMastershipLib(IntPtr IDataDistribution_instance);

        // IDataDistributionSubsystem interface

        public delegate int IDataDistributionSubsystem_GetServerLostTimeout(IntPtr IDataDistributionSubsystem_instance);

        public delegate long IDataDistributionSubsystem_GetMaxMessageSize(IntPtr IDataDistributionSubsystem_instance);
        public delegate bool IDataDistributionSubsystem_Start(IntPtr IDataDistributionSubsystem_instance, uint dwMilliseconds);
        public delegate bool IDataDistributionSubsystem_Stop(IntPtr IDataDistributionSubsystem_instance, uint dwMilliseconds);
        public delegate OPERATION_RESULT IDataDistributionSubsystem_CreateChannel(IntPtr IDataDistributionSubsystem_instance, [MarshalAs(UnmanagedType.LPStr)] string channelName, IntPtr dataCb,
                                                                       [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] arrayParams = null, int len = 0);

        public delegate bool IDataDistributionSubsystem_StartChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint dwMilliseconds);
        public delegate bool IDataDistributionSubsystem_StopChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint dwMilliseconds);
        public delegate void IDataDistributionSubsystem_Lock(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, uint timeout);
        public delegate void IDataDistributionSubsystem_Unlock(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle);
        public delegate void IDataDistributionSubsystem_SeekChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle, long position);
        public delegate void IDataDistributionSubsystem_DeleteChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle);
        public delegate bool IDataDistributionSubsystem_WriteOnChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle,
                                                                      [MarshalAs(UnmanagedType.LPStr)] string key, IntPtr keyLen,
                                                                      IntPtr param, IntPtr dataLen,
                                                                      bool waitAll = false, Int64 timestamp = SmartDataDistributionChannel.DDM_NO_TIMESTAMP);
        public delegate bool IDataDistributionSubsystem_ReadFromChannel(IntPtr IDataDistributionSubsystem_instance, IntPtr channelHandle,
                                                                       [In, Out] IntPtr param, [In, Out]IntPtr dataLen, long offset);
    }
}
