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

import java.time.Duration;
import java.util.Date;

public interface ISmartDataDistributionChannel extends ISmartDataDistributionChannelInfo {
    /**
     * Starts the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT StartChannel(int timeout);

    /**
     * Stops the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT StopChannel(int timeout);

    /**
     * Set parameter on channel
     * 
     * @param paramName  Parameter name to set
     * @param paramValue Parameter value to set
     */
    void SetParameter(String paramName, String paramValue);

    /**
     * Set parameter on channel
     * 
     * @param paramId    Parameter {@link DDM_GENERAL_PARAMETER} to set
     * @param paramValue Parameter value to set
     */
    void SetParameter(DDM_GENERAL_PARAMETER paramId, String paramValue);

    /**
     * Get parameter from channel
     * 
     * @param paramName Parameter name to get
     * @return Parameter value
     */
    String GetParameter(String paramName);

    /**
     * Get parameter from channel
     * 
     * @param paramId Parameter {@link DDM_GENERAL_PARAMETER} to get
     * @return Parameter value
     */
    String GetParameter(DDM_GENERAL_PARAMETER paramId);

    /**
     * Locks the channel
     * 
     * @param timeout Timeout in ms
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Lock(int timeout);

    /**
     * Unlocks the channel
     * 
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT Unlock();

    /**
     * Seek the channel using absolute offset
     * 
     * @param position Seek offset poisition
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT SeekChannel(long position);

    /**
     * Seek the channel using absolute or relative offset
     * 
     * @param position Seek offset poisition
     * @param kind     {@link DDM_SEEKKIND} to use
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT SeekChannel(long position, DDM_SEEKKIND kind);

    /**
     * Seek the channel to an absolute timestamp
     * 
     * @param position Seek timestamp to an absolute poisition
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT SeekChannel(Date position);

    /**
     * Seek the channel by a relative time expressed as {@link Duration} starting
     * from latest known timestamp
     * 
     * @param position Seek timestamp relative poisition
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT SeekChannel(Duration position);

    /**
     * Seek the channel using absolute or relative offset
     * 
     * @param position Seek offset poisition
     * @param context  {@link DDM_SEEKCONTEXT} to use
     * @param kind     {@link DDM_SEEKKIND} to use
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT SeekChannel(long position, DDM_SEEKCONTEXT context, DDM_SEEKKIND kind);

    /**
     * Writes on the channel
     * 
     * @param value The {@link String} to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String value);

    /**
     * Writes on the channel
     * 
     * @param key   The key to use
     * @param value The {@link String} to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, String value);

    /**
     * Writes on the channel
     * 
     * @param key     The key to use
     * @param value   The {@link String} to write in the channel
     * @param waitAll waits all write in the distributed environment
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll);

    /**
     * Writes on the channel
     * 
     * @param key       The key to use
     * @param value     The {@link String} to write in the channel
     * @param waitAll   waits all write in the distributed environment
     * @param timestamp timestamp to apply
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, String value, boolean waitAll, long timestamp);

    /**
     * Writes on the channel
     * 
     * @param buffer The buffer to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(byte[] buffer);

    /**
     * Writes on the channel
     * 
     * @param key    The key to use
     * @param buffer The buffer to write in the channel
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, byte[] buffer);

    /**
     * Writes on the channel
     * 
     * @param key     The key to use
     * @param buffer  The buffer to write in the channel
     * @param waitAll waits all write in the distributed environment
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll);

    /**
     * Writes on the channel
     * 
     * @param key       The key to use
     * @param buffer    The buffer to write in the channel
     * @param waitAll   waits all write in the distributed environment
     * @param timestamp timestamp to apply
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT WriteOnChannel(String key, byte[] buffer, boolean waitAll, long timestamp);

    /**
     * Reads from the channel
     * 
     * @param offset offset in the stream
     * @param length number of elements
     * @return an array of {@link byte}
     */
    byte[] ReadFromChannel(long offset, long length);

    /**
     * Change direction of the channel
     * 
     * @param direction {@link DDM_CHANNEL_DIRECTION}
     * @return {@link OPERATION_RESULT}
     */
    OPERATION_RESULT ChangeDirectionOnChannel(DDM_CHANNEL_DIRECTION direction);

    /**
     * Adds a {@link IDataAvailableListener} listener
     * 
     * @param listener {@link IDataAvailableListener} listener to add
     */
    void addListener(IDataAvailableListener listener);

    /**
     * Removes a {@link IDataAvailableListener} listener
     * 
     * @param listener {@link IDataAvailableListener} listener to remove
     */
    void removeListener(IDataAvailableListener listener);

    /**
     * Adds a {@link IConditionOrErrorListener} listener
     * 
     * @param listener {@link IConditionOrErrorListener} listener to add
     */
    void addListener(IConditionOrErrorListener listener);

    /**
     * Removes a {@link IConditionOrErrorListener} listener
     * 
     * @param listener {@link IConditionOrErrorListener} listener to remove
     */
    void removeListener(IConditionOrErrorListener listener);
}
