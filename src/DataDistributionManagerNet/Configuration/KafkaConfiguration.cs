﻿/*
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

using System;

namespace MASES.DataDistributionManager.Bindings.Configuration
{
    /// <summary>
    /// The configuration class for Kafka
    /// </summary>
    public class KafkaConfiguration : CommonConfiguration
    {
        /// <summary>
        /// Initialize a <see cref="KafkaConfiguration"/>
        /// </summary>
        public KafkaConfiguration()
#if DEBUG
             : base("kafka", "DataDistributionManagerKafkad.dll")
#else
            : base("kafka", "DataDistributionManagerKafka.dll")
#endif
        {
        }
        /// <summary>
        /// Duplicates a configuration
        /// </summary>
        /// <param name="originalConf"><see cref="IConfiguration"/> to duplicate</param>
        public KafkaConfiguration(IConfiguration originalConf)
            : base(originalConf)
        {

        }
    }
}
