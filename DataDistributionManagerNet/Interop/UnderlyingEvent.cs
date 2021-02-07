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
using System.Runtime.InteropServices;

namespace MASES.DataDistributionManager.Bindings.Interop
{
    /// <summary>
    /// UnderlyingEvent struct representing the information from low system.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct UnderlyingEvent
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string ChannelName;
        public DDM_UNDERLYING_ERROR_CONDITION Condition;
        [MarshalAs(UnmanagedType.U1)]
        public bool IsDataAvailable;
        public IntPtr Key;
        public IntPtr KeyLen;
        public IntPtr Buffer;
        public IntPtr BufferLength;
        [MarshalAs(UnmanagedType.I4)]
        public int NativeCode;
        [MarshalAs(UnmanagedType.LPStr)]
        public string SubSystemReason;
    }
}
