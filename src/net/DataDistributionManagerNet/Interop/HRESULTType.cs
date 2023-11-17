/*
*  Copyright 2023 MASES s.r.l.
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

#pragma warning disable 1591, 1570
using System;

namespace MASES.DataDistributionManager.Bindings.Interop
{
    public class OperationResultException : SystemException
    {

    }

    #region OPERATION_RESULT
    [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential, Pack = 4)]
    public struct OPERATION_RESULT : System.IComparable
    {
        private int m_value;

        public OPERATION_RESULT(int value)
        {
            m_value = value;
        }
        public static implicit operator int(OPERATION_RESULT This)
        {
            return This.m_value;
        }
        public static implicit operator OPERATION_RESULT(int This)
        {
            return new OPERATION_RESULT(This);
        }
        public static implicit operator bool(OPERATION_RESULT This)
        {
            if (This.m_value == DDM_NO_ERROR_CONDITION) return true;
            if (This.m_value > 0) return false;
            throw new OperationResultException();
        }

        public static bool operator true(OPERATION_RESULT This)
        {
            return This == true;
        }

        public static bool operator false(OPERATION_RESULT This)
        {
            return This == false;
        }

        #region IEquatable<> Members
        public bool Equals(OPERATION_RESULT that)
        {
            return (this.m_value == that.m_value);
        }
        public bool Equals(int that)
        {
            return (this.m_value == that);
        }
        #endregion

        #region System.Object Members
        public override bool Equals(object obj)
        {
            if (obj == null)
                return false;
            if (obj is OPERATION_RESULT)
                return Equals((OPERATION_RESULT)obj);
            if (obj is int)
                return Equals((int)obj);
            return false;
        }

        public override int GetHashCode()
        {
            return m_value;
        }

        public override string ToString()
        {
            System.Reflection.FieldInfo fi;
            if (!dirCodes.TryGetValue(m_value, out fi))
            {
                return m_value.ToString();
            }
            object[] o = fi.GetCustomAttributes(typeof(DescriptionAttribute), false);
            DescriptionAttribute descr = null;
            if (o != null && o.Length > 0) descr = (DescriptionAttribute)o[0];
            string s = (descr != null) ? fi.Name + ": " + descr.description : fi.Name;
            return s;
        }
        #endregion

        public bool Failed { get { return m_value < 0; } }
        public bool Succeeded { get { return m_value >= 0; } }

        public static bool FAILED(int hr) { return hr < 0; }
        public static bool SUCCEEDED(int hr) { return hr >= 0; }

        #region IComparable<> Members
        public int CompareTo(OPERATION_RESULT that)
        {
            return (m_value < that.m_value) ? -1 : (m_value > that.m_value) ? +1 : 0;
        }
        public int CompareTo(int that)
        {
            return (m_value < that) ? -1 : (m_value > that) ? +1 : 0;
        }
        #endregion

        #region IComparable Members
        public int CompareTo(object obj)
        {
            if (obj == null)
                return +1;
            if (obj is OPERATION_RESULT)
                return CompareTo((OPERATION_RESULT)obj);
            if (obj is int)
                return CompareTo((int)obj);
            throw new System.ArgumentException("Arg_MustBeHRESULT");
        }
        #endregion

        [System.AttributeUsage(System.AttributeTargets.All)]
        class DescriptionAttribute : System.Attribute
        {
            protected string m_description;
            public DescriptionAttribute(string description)
            {
                m_description = description;
            }

            public string description { get { return m_description; } }
        }

        ///<summary>
        /// Success code
        ///</summary>
        [Description("Success code")]
        public const int DDM_NO_ERROR_CONDITION = unchecked((int)0x00000000);

        #region Condition

        ///<summary>
        /// Data are available
        ///</summary>
        [Description("Data are available")]
        public const int DDM_DATA_AVAILABLE = unchecked((int)0x20000000);

        ///<summary>
        /// Reached end of stream
        ///</summary>
        [Description("Reached end of stream")]
        public const int DDM_END_OF_STREAM = unchecked(DDM_DATA_AVAILABLE + 1);

        ///<summary>
        /// No data returned
        ///</summary>
        [Description("No data returned")]
        public const int DDM_NO_DATA_RETURNED = unchecked(DDM_DATA_AVAILABLE + 2);

        ///<summary>
        /// Timeout elapsed waiting for messages from the channel
        ///</summary>
        [Description("Timeout elapsed waiting for messages from the channel")]
        public const int DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_BEGIN = unchecked(DDM_DATA_AVAILABLE + 3);

        ///<summary>
        /// End timeout condition waiting for messages from the channel
        ///</summary>
        [Description("End timeout condition waiting for messages from the channel")]
        public const int DDM_ELAPSED_MESSAGE_RECEIVE_TIMEOUT_END = unchecked(DDM_DATA_AVAILABLE + 4);

        ///<summary>
        /// Timeout on acknowledgment
        ///</summary>
        [Description("Timeout on acknowledgment")]
        public const int DDM_ELAPSED_MESSAGE_ACKNOWLEDGMENT_TIMEOUT = unchecked(DDM_DATA_AVAILABLE + 5);

        #endregion

        #region Error

        ///<summary>
        /// Unmapped error
        ///</summary>
        [Description("Unmapped error")]
        public const int DDM_UNMAPPED_ERROR_CONDITION = unchecked((int)0xF0000000);

        ///<summary>
        /// Fatal
        ///</summary>
        [Description("Fatal")]
        public const int DDM_FATAL_ERROR = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 1);

        ///<summary>
        /// Timeout occurred
        ///</summary>
        [Description("Timeout occurred")]
        public const int DDM_TIMEOUT = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 2);

        ///<summary>
        /// Invalid data
        ///</summary>
        [Description("Invalid data")]
        public const int DDM_INVALID_DATA = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 3);

        ///<summary>
        /// Subsystem not started
        ///</summary>
        [Description("Subsystem not started")]
        public const int DDM_SUBSYSTEM_NOT_STARTED = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 4);

        ///<summary>
        /// Failed on write
        ///</summary>
        [Description("Failed on write")]
        public const int DDM_WRITE_FAILED = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 5);

        ///<summary>
        /// Commit failed
        ///</summary>
        [Description("Commit failed")]
        public const int DDM_COMMIT_FAILED = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 6);

        ///<summary>
        /// Pointer not set within subsystem
        ///</summary>
        [Description("Pointer not set within subsystem")]
        public const int DDM_POINTER_NOT_SET = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 7);

        ///<summary>
        /// Function not implemented
        ///</summary>
        [Description("Function not implemented")]
        public const int DDM_NOT_IMPLEMENTED = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 8);

        ///<summary>
        /// Errors in parameters
        ///</summary>
        [Description("Errors in parameters")]
        public const int DDM_PARAMETER_ERROR = unchecked(DDM_UNMAPPED_ERROR_CONDITION + 9);

        #endregion


        class DirCodes : System.Collections.DictionaryBase
        {
            public DirCodes()
            {
            }
            public DirCodes(int capacity)
            {
            }
            public void Add(int key, System.Reflection.FieldInfo value)
            {
                base.Dictionary.Add(key, value);
            }
            public System.Reflection.FieldInfo this[int key]
            {
                get { return (System.Reflection.FieldInfo)base.Dictionary[key]; }
                set { base.Dictionary[key] = value; }
            }
            public bool TryGetValue(int key, out System.Reflection.FieldInfo value)
            {
                value = this[key];
                return value != null;
            }
            public bool Contains(int key)
            {
                return base.Dictionary.Contains(key);
            }
            public bool ContainsKey(int key)
            {
                return base.InnerHashtable.ContainsKey(key);
            }
            public void CopyTo(System.Reflection.FieldInfo[] values, int index)
            {
                base.Dictionary.Values.CopyTo(values, index);
            }
            public System.Collections.ICollection Keys
            {
                get { return base.Dictionary.Keys; }
            }
            public System.Collections.ICollection Values
            {
                get { return base.Dictionary.Values; }
            }
        }

        static DirCodes dirCodes;

        static OPERATION_RESULT()
        {
            dirCodes = new DirCodes(1280);
            System.Reflection.FieldInfo[] fieldsInfo = typeof(OPERATION_RESULT).GetFields(System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Static);
            foreach (System.Reflection.FieldInfo fi in fieldsInfo)
            {
                if (fi.GetValue(null).GetType() == typeof(int))
                {
                    int hr = (int)fi.GetValue(null);
                    if (!dirCodes.ContainsKey(hr))
                        dirCodes[hr] = fi;
                }
            }
        }
    }
    #endregion OPERATION_RESULT
}

#pragma warning restore 1591, 1570
