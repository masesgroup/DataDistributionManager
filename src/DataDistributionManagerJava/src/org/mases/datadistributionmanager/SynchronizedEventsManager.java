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
import java.util.Iterator;

class SynchronizedEventsManager<T> implements Iterable<T> {
    ArrayList<T> m_listeners = new ArrayList<>();

    /**
     * Adds a {@link T} listener
     * 
     * @param listener {@link T} listener to add
     */
    public synchronized void addListener(T listener) {
        m_listeners.add(listener);
    }

    /**
     * Removes a {@link T} listener
     * 
     * @param listener {@link T} listener to remove
     */
    public synchronized void removeListener(T listener) {
        m_listeners.remove(listener);
    }

    /**
     * True if there are listeners
     * 
     * @return True if there are listeners
     */
    public synchronized boolean hasElements() {
        return m_listeners.size() != 0;
    }

    @Override
    public Iterator<T> iterator() {
        synchronized (this) {
            ArrayList<T> local = new ArrayList<T>();
            local.addAll(m_listeners);
            return local.iterator();
        }
    }
}
