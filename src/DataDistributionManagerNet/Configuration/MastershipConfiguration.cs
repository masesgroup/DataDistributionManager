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

namespace MASES.DataDistributionManager.Bindings.Configuration
{
    /// <summary>
    /// The configuration class for mastership based on DataDistributionMastershipManager
    /// </summary>
    public class MastershipConfiguration : CommonConfiguration
    {
        /// <summary>
        /// Base property name of all specific configuration key of <see cref="MastershipConfiguration"/>
        /// </summary>
        public const string MastershipGlobalConfigurationBasePropertyKey = "datadistributionmanager.mastership.";

        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        public MastershipConfiguration(IConfiguration originalConf)
            : base(originalConf)
        {
#if DEBUG
            MastershipLibrary = "DataDistributionMastershipManagerd.dll";
#else
            MastershipLibrary = "DataDistributionMastershipManager.dll";
#endif
        }

        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="libraryName">The mastership library name (or full path) to use</param>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        public MastershipConfiguration(string libraryName, IConfiguration originalConf)
            : base(originalConf)
        {
            MastershipLibrary = libraryName;
        }

        /// <summary>
        /// Generic setter/getter for all configuration properties
        /// </summary>
        /// <param name="property">The property name</param>
        /// <returns>The property value</returns>
        public string this[string property]
        {
            get
            {
                string value = string.Empty;
                if (property.StartsWith(MastershipGlobalConfigurationBasePropertyKey))
                {
                    keyValuePair.TryGetValue(property, out value);
                }
                else
                {
                    keyValuePair.TryGetValue(MastershipGlobalConfigurationBasePropertyKey + property, out value);
                }
                return value;
            }
            set
            {
                if (property.StartsWith(MastershipGlobalConfigurationBasePropertyKey))
                {
                    keyValuePair[property] = value;
                }
                else
                {
                    keyValuePair[MastershipGlobalConfigurationBasePropertyKey + property] = value;
                }
                EmitPropertyChanged(property);
            }
        }
    }
}
