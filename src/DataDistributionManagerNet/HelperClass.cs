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

using MASES.DataDistributionManager.Bindings.Interop;
using System;

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// Class with helping function
    /// </summary>
    public static class HelperClass
    {
        /// <summary>
        /// Converts a numeric string representation into an enumerator
        /// </summary>
        /// <typeparam name="T">Enumerator type</typeparam>
        /// <param name="value">The value to convert</param>
        /// <returns>The enumerator value</returns>
        public static T FromIntString<T>(this string value)
        {
            return (T)Enum.Parse(typeof(T), value);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_GENERAL_PARAMETER"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_GENERAL_PARAMETER level)
        {
            return ToString((int)level);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_CHANNEL_DIRECTION"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_CHANNEL_DIRECTION level)
        {
            return ToString((int)level);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_LOG_LEVEL"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_LOG_LEVEL level)
        {
            return ToString((int)level);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_UNDERLYING_ERROR_CONDITION"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_UNDERLYING_ERROR_CONDITION level)
        {
            return ToString((int)level);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_INSTANCE_STATE"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_INSTANCE_STATE level)
        {
            return ToString((int)level);
        }

        /// <summary>
        /// Converts enum in string
        /// </summary>
        /// <param name="level"><see cref="DDM_CLUSTEREVENT"/></param>
        /// <returns>Numeric string representation</returns>
        public static string ToIntString(this DDM_CLUSTEREVENT level)
        {
            return ToString((int)level);
        }

        static string ToString(int val)
        {
            return val.ToString();
        }
    }
}
