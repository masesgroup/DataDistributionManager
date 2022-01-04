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

package org.mases.datadistributionmanager;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * The global configuration class
 */
public abstract class BaseConfiguration implements IConfiguration {
    /**
     * The list of key/value pairs
     */
    protected HashMap<String, String> keyValuePair = new HashMap<String, String>();

    /**
     * Checks the configuration for mandatory information
     */
    protected void CheckConfiguration() throws IllegalArgumentException {

    }

    public String[] getConfiguration() throws IllegalArgumentException {
        ArrayList<String> lst = new ArrayList<String>();
        for (String key : keyValuePair.keySet()) {
            lst.add(String.format("%s=%s", key, keyValuePair.get(key)));
        }
        String[] array = new String[lst.size()];
        return lst.toArray(array);
    }
}