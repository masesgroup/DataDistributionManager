/*
*  Copyright 2022 MASES s.r.l.
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

package org.mases.datadistributionmanager.configuration;

import org.mases.datadistributionmanager.CommonConfiguration;
import org.mases.datadistributionmanager.IConfiguration;

/**
 * The configuration class for mastership based on
 * DataDistributionMastershipManager
 */
public class MastershipConfiguration extends CommonConfiguration {
    public static final String MastershipGlobalConfigurationBasePropertyKey = "datadistributionmanager.mastership.";

    /**
     * Duplicates a configuration
     * 
     * @param originalConf {@link IConfiguration} to duplicate
     */
    public MastershipConfiguration(IConfiguration originalConf) {
        super(originalConf);
        setMastershipLibrary("DataDistributionMastershipManager.dll");
    }

    /**
     * Duplicates a configuration
     * @param libraryName The mastership library name (or full path) to use
     * @param originalConf {@link IConfiguration} to duplicate
     */
    public MastershipConfiguration(String libraryName, IConfiguration originalConf) {
        super(originalConf);
        setMastershipLibrary("DataDistributionMastershipManager.dll");
    }

    /**
     * Generic getter for all configuration properties
     * 
     * @param property The property name
     * @return The property value
     */
    public String getProperty(String property) {
        String value = "";
        if (property.startsWith(MastershipGlobalConfigurationBasePropertyKey)) {
            if (keyValuePair.containsKey(property))
                value = keyValuePair.get(property);
        } else {
            if (keyValuePair.containsKey(MastershipGlobalConfigurationBasePropertyKey + property))
                value = keyValuePair.get(MastershipGlobalConfigurationBasePropertyKey + property);
        }
        return value;
    }

    /**
     * Generic setter for all configuration properties
     * 
     * @param property The property name
     * @param value    The property value
     */
    public void setProperty(String property, String value) {
        if (property.startsWith(MastershipGlobalConfigurationBasePropertyKey)) {
            keyValuePair.put(property, value);
        } else {
            keyValuePair.put(MastershipGlobalConfigurationBasePropertyKey + property, value);
        }
    }
}
