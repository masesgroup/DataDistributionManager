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

using System.Collections.Generic;

namespace MASES.DataDistributionManager.Bindings
{
    /// <summary>
    /// The base configuration class
    /// </summary>
    public abstract class BaseConfiguration : IConfiguration
    {
        internal Dictionary<string, string> keyValuePair = new Dictionary<string, string>();

        /// <summary>
        /// Checks the configuration for mandatory information
        /// </summary>
        protected virtual void CheckConfiguration()
        {

        }

        /// <inheritdoc/>
        public virtual string[] Configuration
        {
            get
            {
                List<string> lst = new List<string>();
                foreach (var item in keyValuePair)
                {
                    lst.Add(string.Format("{0}={1}", item.Key, item.Value));
                }
                return lst.ToArray();
            }
        }
    }
}
