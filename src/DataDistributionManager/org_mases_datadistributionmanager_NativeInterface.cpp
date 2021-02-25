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

#include "DataDistributionManager.h"
#include "org_mases_datadistributionmanager_NativeInterface.h"

JavaVM* jvm = NULL;
jint jvmVersion = 0;

static void StoreJVM(JNIEnv * env)
{
	if (jvm == NULL)
	{
		env->GetJavaVM(&jvm);
		jvmVersion = env->GetVersion();
	}
}

static JNIEnv * NewEnv(char* threadName, BOOL* attached)
{
	*attached = FALSE;
	// in the new thread:
	JNIEnv* newEnv = NULL;
	int getEnvStat = jvm->GetEnv((void **)&newEnv, jvmVersion);

	switch (getEnvStat)
	{
	case JNI_OK: break;
	case JNI_EDETACHED:
	{
		JavaVMAttachArgs args;
		args.version = jvmVersion; // choose your JNI version
		args.name = threadName;    // you might want to give the java thread a name
		args.group = NULL;         // you might want to assign the java thread to a ThreadGroup
		int ret = jvm->AttachCurrentThread((void **)&newEnv, &args);
		if (ret != JNI_OK)
		{
			printf("Failed to attach with error %d", ret);
		}
		else *attached = TRUE;
	}
	break;
	case JNI_ERR:      /* unknown error */
	case JNI_EVERSION: /* JNI version error */
	case JNI_ENOMEM:   /* not enough memory */
	case JNI_EEXIST:   /* VM already created */
	case JNI_EINVAL:   /* invalid arguments */
#ifdef _DEBUG
		printf("Failed jvm->GetEnv with error %d", getEnvStat);
#endif
		break;
	}
	// And now you can use myNewEnv
	return newEnv;
}

static void ReleaseEnv(BOOL attached)
{
	if (attached)
	{
		jint result = jvm->DetachCurrentThread();
		switch (result)
		{
		case JNI_OK: break;
		case JNI_EDETACHED:
		case JNI_ERR:      /* unknown error */
		case JNI_EVERSION: /* JNI version error */
		case JNI_ENOMEM:   /* not enough memory */
		case JNI_EEXIST:   /* VM already created */
		case JNI_EINVAL:   /* invalid arguments */
#ifdef _DEBUG
			printf("Failed jvm->DetachCurrentThread with error %d", result);
#endif
			break;
		};
	}
}

static void CheckAndRaise(JNIEnv * env)
{
	jthrowable exc = env->ExceptionOccurred();
	if (exc) {
		jclass newExcCls;

		env->ExceptionDescribe();
		env->ExceptionClear();

		newExcCls = env->FindClass("java/lang/IllegalArgumentException");
		if (newExcCls == 0) {
			return;
		}
		env->ThrowNew(newExcCls, "thrown from JNI code");
	}
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistribution_create
* Signature: ()J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistribution_1create
(JNIEnv * env, jclass caller)
{
	StoreJVM(env);
	return (jlong)DataDistribution::create();
}

struct DataDistributionCallbackContainer
{
	jobject jthis;
	jclass thisClass;
	IDataDistributionCallback* pcb;
};

const char * dataDistributionConfigurationCbJava(const void* opaque, const char * channelName, const char * key, const char * value)
{
	BOOL attached;
	JNIEnv* env = NewEnv("Configuration", &attached);
	DataDistributionCallbackContainer* p = (DataDistributionCallbackContainer*)opaque;
	jstring jchannelName = env->NewStringUTF(channelName);
	jstring jkey = env->NewStringUTF(key);
	jstring jvalue = env->NewStringUTF(value);

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, "OnConfiguration", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	jobject res = env->CallObjectMethod(p->jthis, callbackMethodId, (jlong)p, jchannelName, jkey, jvalue);

	CheckAndRaise(env);

	env->DeleteLocalRef(jchannelName);
	env->DeleteLocalRef(jkey);
	env->DeleteLocalRef(jvalue);

	ReleaseEnv(attached);

	jboolean isCopy;
	const char* pjStr = env->GetStringUTFChars((jstring)res, &isCopy);
	env->ReleaseStringUTFChars((jstring)res, pjStr);

	return pjStr;
}

void dataDistributionLoggingCbJava(const void* opaque, const DDM_LOG_LEVEL level, const char* source, const char* function, const char* errorStr)
{
	BOOL attached;
	JNIEnv* env = NewEnv("Logger", &attached);
	DataDistributionCallbackContainer* p = (DataDistributionCallbackContainer*)opaque;
	jstring jsource = env->NewStringUTF(source);
	jstring jfunction = env->NewStringUTF(function);
	jstring jerrorStr = env->NewStringUTF(errorStr);

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, "OnLogging", "(JILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, level, jsource, jfunction, jerrorStr);

	CheckAndRaise(env);

	env->DeleteLocalRef(jsource);
	env->DeleteLocalRef(jfunction);
	env->DeleteLocalRef(jerrorStr);

	ReleaseEnv(attached);
}

void dataDistributionCompletelyDisconnectedCbJava(const void* opaque, const char* source, const char* errorStr)
{
	BOOL attached;
	JNIEnv* env = NewEnv("CompletelyDisconnected", &attached);
	DataDistributionCallbackContainer* p = (DataDistributionCallbackContainer*)opaque;
	jstring jsource = env->NewStringUTF(source);
	jstring jerrorStr = env->NewStringUTF(errorStr);

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, "OnCompletelyDisconnected", "(JLjava/lang/String;Ljava/lang/String;)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, jsource, jerrorStr);

	CheckAndRaise(env);

	env->DeleteLocalRef(jsource);
	env->DeleteLocalRef(jerrorStr);
	ReleaseEnv(attached);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionCallback_create
* Signature: (Ljava/lang/Object;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionCallback_1create
(JNIEnv * env, jclass caller, jobject jthis)
{
	DataDistributionCallbackContainer* p = new DataDistributionCallbackContainer();
	jclass thisClass = env->GetObjectClass(jthis);
	// Check for null
	if (thisClass)
	{
		p->thisClass = reinterpret_cast<jclass>(env->NewGlobalRef(thisClass));
		env->DeleteLocalRef(thisClass);
	}
	p->jthis = env->NewGlobalRef(jthis);
	p->pcb = DataDistributionCallback::create(p, dataDistributionConfigurationCbJava, dataDistributionLoggingCbJava, dataDistributionCompletelyDisconnectedCbJava);

	return (jlong)p;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionCallback_delete
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionCallback_1delete
(JNIEnv * env, jclass caller, jlong ref)
{
	DataDistributionCallbackContainer* p = (DataDistributionCallbackContainer*)ref;
	env->DeleteGlobalRef(p->thisClass);
	env->DeleteGlobalRef(p->jthis);
	delete p->pcb;
	delete p;
}

struct DataDistributionChannelCallbackContainer
{
	jobject jthis;
	jclass thisClass;
	IDataDistributionChannelCallback* pcb;
};

void dataDistributionUnderlyingEventCbJava(const void* opaque, const HANDLE channelHandle, const UnderlyingEventData* uEvent)
{
	BOOL attached;
	JNIEnv* env = NewEnv("UnderlyingEvent", &attached);
	DataDistributionChannelCallbackContainer* p = (DataDistributionChannelCallbackContainer*)opaque;
	jstring jChannelName = env->NewStringUTF(uEvent->ChannelName);
	jstring jKey = env->NewStringUTF(uEvent->Key);
	jstring jSubSystemReason = env->NewStringUTF(uEvent->SubSystemReason);
	jbyteArray jbuffer = NULL;
	if (uEvent->IsDataAvailable)
	{
		jbuffer = env->NewByteArray((jsize)uEvent->BufferLength);
		env->SetByteArrayRegion(jbuffer, 0, (jsize)uEvent->BufferLength, (const jbyte*)uEvent->Buffer);
	}

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, "OnUnderlyingEvent", "(JJLjava/lang/String;IZLjava/lang/String;[BILjava/lang/String;)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jlong)channelHandle, jChannelName, (jint)uEvent->Condition, (jboolean)uEvent->IsDataAvailable, jKey, jbuffer, (jint)uEvent->NativeCode, jSubSystemReason);

	CheckAndRaise(env);

	if (jbuffer) env->DeleteLocalRef(jbuffer);
	env->DeleteLocalRef(jChannelName);
	env->DeleteLocalRef(jKey);
	env->DeleteLocalRef(jSubSystemReason);

	ReleaseEnv(attached);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionChannelCallback_create
* Signature: (Ljava/lang/Object;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionChannelCallback_1create
(JNIEnv * env, jclass caller, jobject jthis)
{
	DataDistributionChannelCallbackContainer* p = new DataDistributionChannelCallbackContainer();
	jclass thisClass = env->GetObjectClass(jthis);
	// Check for null
	if (thisClass)
	{
		p->thisClass = reinterpret_cast<jclass>(env->NewGlobalRef(thisClass));
		env->DeleteLocalRef(thisClass);
	}
	p->jthis = env->NewGlobalRef(jthis);
	p->pcb = DataDistributionChannelCallback::create(p, dataDistributionUnderlyingEventCbJava);

	return (jlong)p;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionChannelCallback_delete
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionChannelCallback_1delete
(JNIEnv * env, jclass caller, jlong ref)
{
	DataDistributionChannelCallbackContainer* p = (DataDistributionChannelCallbackContainer*)ref;
	env->DeleteGlobalRef(p->thisClass);
	env->DeleteGlobalRef(p->jthis);
	delete p->pcb;
	delete p;
}

struct DataDistributionMastershipCallbackContainer
{
	jobject jthis;
	jclass thisClass;
	IDataDistributionMastershipCallback* pcb;
};

void dataDistributionOnClusterStateChangeJavaCb(void* opaque, DDM_CLUSTEREVENT change, int64_t serverid)
{
	BOOL attached;
	JNIEnv* env = NewEnv("OnClusterStateChange", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;


	ReleaseEnv(attached);
}

void dataDistributionOnStateChangeJavaCb(void* opaque, DDM_INSTANCE_STATE newState, DDM_INSTANCE_STATE oldState)
{
	BOOL attached;
	JNIEnv* env = NewEnv("OnStateChange", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " OnStateChange", "(JII)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jint)newState, (jint)oldState);

	CheckAndRaise(env);

	ReleaseEnv(attached);
}

void dataDistributionOnStateReadyJavaCb(void* opaque, void* pState, int64_t len)
{
	BOOL attached;
	JNIEnv* env = NewEnv("OnStateReady", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " OnStateReady", "(J[B)V");

	jbyteArray jbuffer = NULL;
	if (pState)
	{
		jbuffer = env->NewByteArray((jsize)len);
		env->SetByteArrayRegion(jbuffer, 0, (jsize)len, (const jbyte*)pState);
	}

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, jbuffer);

	CheckAndRaise(env);

	if (jbuffer) env->DeleteLocalRef(jbuffer);

	ReleaseEnv(attached);
}

void dataDistributionOnRequestedStateJavaCb(void* opaque, void** pState, size_t* len)
{
	BOOL attached;
	JNIEnv* env = NewEnv("OnRequestedState", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	// da rivedere

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " OnRequestedState", "(J[B)V");

	jbyteArray jbuffer = NULL;
	if (pState)
	{
		jbuffer = env->NewByteArray((jsize)*len);
		env->SetByteArrayRegion(jbuffer, 0, (jsize)*len, (const jbyte*)pState);
	}

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, jbuffer);

	CheckAndRaise(env);

	if (jbuffer) env->DeleteLocalRef(jbuffer);

	ReleaseEnv(attached);
}

void dataDistributionOnMultiplePrimaryJavaCb(void* opaque, int64_t myId, int64_t otherId)
{
	BOOL attached;
	JNIEnv* env = NewEnv("OnMultiplePrimary", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " OnMultiplePrimary", "(JJJ)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jlong)myId, (jlong)otherId);

	CheckAndRaise(env);

	ReleaseEnv(attached);
}

void dataDistributionFirstStateChangeJavaCb(void* opaque, DDM_INSTANCE_STATE newState)
{
	BOOL attached;
	JNIEnv* env = NewEnv("FirstStateChange", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " FirstStateChange", "(JI)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jint)newState);

	CheckAndRaise(env);

	ReleaseEnv(attached);
}

void dataDistributionChangingStateJavaCb(void* opaque, DDM_INSTANCE_STATE oldState, DDM_INSTANCE_STATE newState)
{
	BOOL attached;
	JNIEnv* env = NewEnv("ChangingState", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " ChangingState", "(JII)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jint)oldState, (jint)newState);

	CheckAndRaise(env);

	ReleaseEnv(attached);
}

void dataDistributionChangedStateJavaCb(void* opaque, DDM_INSTANCE_STATE newState)
{
	BOOL attached;
	JNIEnv* env = NewEnv("ChangedState", &attached);
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)opaque;

	jmethodID callbackMethodId = env->GetMethodID(p->thisClass, " ChangedState", "(JI)V");

	env->CallVoidMethod(p->jthis, callbackMethodId, (jlong)p, (jint)newState);

	CheckAndRaise(env);

	ReleaseEnv(attached);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionMastershipCallback_create
* Signature: (Ljava/lang/Object;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionMastershipCallback_1create
(JNIEnv * env, jclass, jobject jthis)
{
	DataDistributionMastershipCallbackContainer* p = new DataDistributionMastershipCallbackContainer();

	p->jthis = env->NewGlobalRef(jthis);
	jclass thisClass = env->GetObjectClass(jthis);
	// Check for null
	if (thisClass)
	{
		p->thisClass = reinterpret_cast<jclass>(env->NewGlobalRef(thisClass));
		env->DeleteLocalRef(thisClass);
	}
	p->pcb = DataDistributionMastershipCallback::create(p, dataDistributionOnClusterStateChangeJavaCb, dataDistributionOnStateChangeJavaCb, dataDistributionOnStateReadyJavaCb,
		dataDistributionOnRequestedStateJavaCb, dataDistributionOnMultiplePrimaryJavaCb, dataDistributionFirstStateChangeJavaCb,
		dataDistributionChangingStateJavaCb, dataDistributionChangedStateJavaCb);

	return (jlong)p;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    DataDistributionMastershipCallback_delete
* Signature: (J)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_DataDistributionMastershipCallback_1delete
(JNIEnv * env, jclass caller, jlong ref)
{
	DataDistributionMastershipCallbackContainer* p = (DataDistributionMastershipCallbackContainer*)ref;
	env->DeleteGlobalRef(p->thisClass);
	env->DeleteGlobalRef(p->jthis);
	delete p->pcb;
	delete p;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_Initialize
* Signature: (JJLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1Initialize
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jIDataDistributionCallback, jstring jconf_file, jstring jszMyAddress, jstring jChannelTrailer)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	DataDistributionCallbackContainer* pDataDistributionCallbackContainer = (DataDistributionCallbackContainer*)jIDataDistributionCallback;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	const char* conf_file = NULL;
	const char* szMyAddress = NULL;
	const char* channelTrailer = NULL;

	if (jconf_file != NULL)
	{
		pjStr = env->GetStringUTFChars(jconf_file, &isCopy);
		conf_file = (pjStr != NULL) ? _strdup(pjStr) : NULL;
		env->ReleaseStringUTFChars(jconf_file, pjStr);
	}

	if (jszMyAddress != NULL)
	{
		pjStr = env->GetStringUTFChars(jszMyAddress, &isCopy);
		szMyAddress = (pjStr != NULL) ? _strdup(pjStr) : NULL;
		env->ReleaseStringUTFChars(jszMyAddress, pjStr);
	}

	if (jChannelTrailer != NULL)
	{
		pjStr = env->GetStringUTFChars(jChannelTrailer, &isCopy);
		channelTrailer = (pjStr != NULL) ? _strdup(pjStr) : NULL;
		env->ReleaseStringUTFChars(jChannelTrailer, pjStr);
	}

	HRESULT result = pIDataDistribution->Initialize(pDataDistributionCallbackContainer->pcb,
		(conf_file != NULL) ? _strdup(conf_file) : NULL,
		(szMyAddress != NULL) ? _strdup(szMyAddress) : NULL,
		(channelTrailer != NULL) ? _strdup(channelTrailer) : NULL);

	return (jlong)result;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_Initialize2
* Signature: (JJ[Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1Initialize2
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jIDataDistributionCallback, jobjectArray jarrayParams, jstring jszMyAddress, jstring jChannelTrailer)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	DataDistributionCallbackContainer* pDataDistributionCallbackContainer = (DataDistributionCallbackContainer*)jIDataDistributionCallback;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;
	const char* szMyAddress = NULL;
	const char* channelTrailer = NULL;

	int size = env->GetArrayLength(jarrayParams);

	const char **array = (const char**)malloc(size * sizeof(const char *));

	for (int i = 0; i < size; ++i)
	{
		jstring string = (jstring)env->GetObjectArrayElement(jarrayParams, i);
		const char* myarray = env->GetStringUTFChars(string, 0);

		array[i] = _strdup(myarray);

		env->ReleaseStringUTFChars(string, myarray);
		env->DeleteLocalRef(string);
	}

	if (jszMyAddress != NULL)
	{
		pjStr = env->GetStringUTFChars(jszMyAddress, &isCopy);
		szMyAddress = (pjStr != NULL) ? _strdup(pjStr) : NULL;
		env->ReleaseStringUTFChars(jszMyAddress, pjStr);
	}

	if (jChannelTrailer != NULL)
	{
		pjStr = env->GetStringUTFChars(jChannelTrailer, &isCopy);
		channelTrailer = (pjStr != NULL) ? _strdup(pjStr) : NULL;
		env->ReleaseStringUTFChars(jChannelTrailer, pjStr);
	}

	HRESULT result = pIDataDistribution->Initialize(pDataDistributionCallbackContainer->pcb, array, size,
		(szMyAddress != NULL) ? _strdup(szMyAddress) : NULL,
		(channelTrailer != NULL) ? _strdup(channelTrailer) : NULL);

	return (jlong)result;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_RequestMastershipManager
* Signature: (JJLjava/lang/String;[Ljava/lang/String;)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1RequestMastershipManager
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jIDataDistributionMastershipCallback, jstring jszMyAddress, jobjectArray jarrayParams)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	DataDistributionMastershipCallbackContainer* pDataDistributionMastershipCallbackContainer = (DataDistributionMastershipCallbackContainer*)jIDataDistributionMastershipCallback;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;
	const char* szMyAddress;

	int size = env->GetArrayLength(jarrayParams);

	const char **array = (const char**)malloc(size * sizeof(const char *));

	for (int i = 0; i < size; ++i)
	{
		jstring string = (jstring)env->GetObjectArrayElement(jarrayParams, i);
		const char* myarray = env->GetStringUTFChars(string, 0);

		array[i] = _strdup(myarray);

		env->ReleaseStringUTFChars(string, myarray);
		env->DeleteLocalRef(string);
	}

	pjStr = env->GetStringUTFChars(jszMyAddress, &isCopy);
	szMyAddress = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jszMyAddress, pjStr);

	return (jlong)pIDataDistribution->RequestMastershipManager(pDataDistributionMastershipCallbackContainer->pcb, szMyAddress, array, size);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_GetSubsystemManager
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1GetSubsystemManager
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	return (jlong)pIDataDistribution->GetSubsystemManager();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_GetMastershipManager
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1GetMastershipManager
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	return (jlong)pIDataDistribution->GetMastershipManager();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_GetProtocol
* Signature: (J)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1GetProtocol
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	jstring jresult = env->NewStringUTF(pIDataDistribution->GetProtocol().c_str());
	return jresult;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_GetProtocolLib
* Signature: (J)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1GetProtocolLib
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	jstring jresult = env->NewStringUTF(pIDataDistribution->GetProtocolLib().c_str());
	return jresult;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistribution_GetMastershipLib
* Signature: (J)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistribution_1GetMastershipLib
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	jstring jresult = env->NewStringUTF(pIDataDistribution->GetMastershipLib().c_str());
	return jresult;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_GetServerLostTimeout
* Signature: (J)I
*/
JNIEXPORT jint JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1GetServerLostTimeout
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jint)pIDataDistributionSubsystem->GetServerLostTimeout();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_GetMaxMessageSize
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1GetMaxMessageSize
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jint)pIDataDistributionSubsystem->GetMaxMessageSize();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_Start
* Signature: (JJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1Start
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->Start(jtimeout);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_Stop
* Signature: (JJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1Stop
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->Stop(jtimeout);
}

/*
 * Class:     org_mases_datadistributionmanager_NativeInterface
 * Method:    IDataDistributionSubsystem_CreateChannel
 * Signature: (JLjava/lang/String;JI[Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1CreateChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jstring jChannelName, jlong jIDataDistributionChannelCallback, jint direction, jobjectArray jarrayParams)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	DataDistributionChannelCallbackContainer* pDataDistributionChannelCallbackContainer = (DataDistributionChannelCallbackContainer*)jIDataDistributionChannelCallback;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	const char* channelName;

	pjStr = env->GetStringUTFChars(jChannelName, &isCopy);
	channelName = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jChannelName, pjStr);

	int size = 0;
	const char **array = NULL;

	if (jarrayParams)
	{
		size = env->GetArrayLength(jarrayParams);

		array = (const char**)malloc(size * sizeof(const char *));

		for (int i = 0; i < size; ++i)
		{
			jstring string = (jstring)env->GetObjectArrayElement(jarrayParams, i);
			const char* myarray = env->GetStringUTFChars(string, 0);

			array[i] = _strdup(myarray);

			env->ReleaseStringUTFChars(string, myarray);
			env->DeleteLocalRef(string);
		}
	}

	return (jlong)pIDataDistributionSubsystem->CreateChannel(channelName, pDataDistributionChannelCallbackContainer->pcb, (DDM_CHANNEL_DIRECTION)direction, array, size);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_StartChannel
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1StartChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->StartChannel((HANDLE)jChannelHandle, jtimeout);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_StopChannel
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1StopChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->StopChannel((HANDLE)jChannelHandle, jtimeout);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_SetParameter
* Signature: (JJLjava/lang/String;Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1SetParameter__JJLjava_lang_String_2Ljava_lang_String_2
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jstring jparamName, jstring jparamValue)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	const char* paramName;
	const char* paramValue;

	pjStr = env->GetStringUTFChars(jparamName, &isCopy);
	paramName = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jparamName, pjStr);

	pjStr = env->GetStringUTFChars(jparamValue, &isCopy);
	paramValue = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jparamValue, pjStr);

	pIDataDistributionSubsystem->SetParameter((HANDLE)jChannelHandle, paramName, paramValue);

	if (paramName != NULL) free((void*)paramName);
	if (paramValue != NULL) free((void*)paramValue);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_SetParameter
* Signature: (JJILjava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1SetParameter__JJILjava_lang_String_2
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jint jparamId, jstring jparamValue)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	const char* paramValue;

	pjStr = env->GetStringUTFChars(jparamValue, &isCopy);
	paramValue = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jparamValue, pjStr);

	pIDataDistributionSubsystem->SetParameter((HANDLE)jChannelHandle, (DDM_GENERAL_PARAMETER)jparamId, paramValue);

	if (paramValue != NULL) free((void*)paramValue);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_GetParameter
* Signature: (JJLjava/lang/String;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1GetParameter__JJLjava_lang_String_2
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jstring jparamName)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	const char* paramName;

	pjStr = env->GetStringUTFChars(jparamName, &isCopy);
	paramName = (pjStr != NULL) ? _strdup(pjStr) : NULL;
	env->ReleaseStringUTFChars(jparamName, pjStr);

	const char* paramValue = pIDataDistributionSubsystem->GetParameter((HANDLE)jChannelHandle, paramName);

	if (paramName != NULL) free((void*)paramName);

	return env->NewStringUTF(paramValue);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_GetParameter
* Signature: (JJI)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1GetParameter__JJI
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jint jparamId)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	const char* paramValue = pIDataDistributionSubsystem->GetParameter((HANDLE)jChannelHandle, (DDM_GENERAL_PARAMETER)jparamId);
	return env->NewStringUTF(paramValue);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_Lock
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1Lock
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->Lock((HANDLE)jChannelHandle, jtimeout);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_Unlock
* Signature: (JJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1Unlock
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->Unlock((HANDLE)jChannelHandle);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_SeekChannel
* Signature: (JJJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1SeekChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jlong jposition)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->SeekChannel((HANDLE)jChannelHandle, jposition);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_DeleteChannel
* Signature: (JJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1DeleteChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->DeleteChannel((HANDLE)jChannelHandle);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_WriteOnChannel
* Signature: (JJLjava/lang/String;[BZJ)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1WriteOnChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jstring jkey, jbyteArray jbuffer, jboolean jwriteAll, jlong jtimeout)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;

	const char* pjStr;
	jboolean isCopy = JNI_FALSE;

	int keylen = env->GetStringLength(jkey);
	pjStr = env->GetStringUTFChars(jkey, &isCopy);

	jsize bufferLen = env->GetArrayLength(jbuffer);
	jbyte *buffer = (jbyte *)env->GetByteArrayElements(jbuffer, NULL);

	jlong result = (jlong)pIDataDistributionSubsystem->WriteOnChannel((HANDLE)jChannelHandle, pjStr, keylen, buffer, bufferLen, jwriteAll, jtimeout);

	env->ReleaseStringUTFChars(jkey, pjStr);
	env->ReleaseByteArrayElements(jbuffer, buffer, 0);
	return result;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_ReadFromChannel
* Signature: (JJJJ)[B
*/
JNIEXPORT jbyteArray JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1ReadFromChannel
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jlong jposition, jlong jlength)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;

	size_t dataLen = jlength;
	void* receivingBuffer = NULL;

	jbyteArray jArray = NULL;

	if (pIDataDistributionSubsystem->ReadFromChannel((HANDLE)jChannelHandle, jposition, &dataLen, &receivingBuffer) == TRUE)
	{
		jArray = env->NewByteArray(dataLen);
		env->SetByteArrayRegion(jArray, 0, dataLen, (const jbyte*)receivingBuffer);
	}

	return jArray;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionSubsystem_ChangeChannelDirection
* Signature: (JJI)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionSubsystem_1ChangeChannelDirection
(JNIEnv * env, jclass caller, jlong jIDataDistributionSubSystem, jlong jChannelHandle, jint direction)
{
	IDataDistributionSubsystem* pIDataDistributionSubsystem = (IDataDistributionSubsystem*)jIDataDistributionSubSystem;
	return (jlong)pIDataDistributionSubsystem->ChangeChannelDirection((HANDLE)jChannelHandle, (DDM_CHANNEL_DIRECTION)direction);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_Start
* Signature: (JI)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1Start
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jint dwMilliseconds)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return (jlong)E_FAIL;
	return (jlong)pIDataDistribution->GetMastershipManager()->Start(dwMilliseconds);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_Stop
* Signature: (JI)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1Stop
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jint dwMilliseconds)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return (jlong)E_FAIL;
	return (jlong)pIDataDistribution->GetMastershipManager()->Stop(dwMilliseconds);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetIamNextPrimary
* Signature: (J)Z
*/
JNIEXPORT jboolean JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetIamNextPrimary
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return JNI_FALSE;
	return pIDataDistribution->GetMastershipManager()->GetIamNextPrimary() ? JNI_TRUE : JNI_FALSE;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_RequestIAmNextPrimary
* Signature: (J)Z
*/
JNIEXPORT jboolean JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1RequestIAmNextPrimary
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return JNI_FALSE;
	return pIDataDistribution->GetMastershipManager()->GetIamNextPrimary() ? JNI_TRUE : JNI_FALSE;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetClusterIndexes
* Signature: (J)[J
*/
JNIEXPORT jlongArray JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetClusterIndexes
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;

	size_t length;
	int64_t* arrayElements = pIDataDistribution->GetMastershipManager()->GetClusterIndexes(&length);
	jlongArray jArray = NULL;
	if (length != 0)
	{
		jArray = env->NewLongArray((jsize)length);
		env->SetLongArrayRegion(jArray, 0, length, arrayElements);
		free(arrayElements);
	}

	return jArray;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetStateOf
* Signature: (JJ)I
*/
JNIEXPORT jint JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetStateOf
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jindex)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jint)pIDataDistribution->GetMastershipManager()->GetStateOf(jindex);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetClusterHealth
* Signature: (JJ)Lorg/mases/businesscontinuity/ClusterHealthElement;
*/
JNIEXPORT jobject JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetClusterHealth
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jindex)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	auto health = pIDataDistribution->GetMastershipManager()->GetClusterHealth();

	auto info = health.at(jindex);

	jclass clazz = env->FindClass("org/mases/businesscontinuity/ClusterHealthElement");
	if (!clazz) return NULL;
	jmethodID ctor = env->GetMethodID(clazz, "<init>", "(IJJ)V");
	jobject result = env->NewObject(clazz, ctor, info->ServerId, info->Status, info->Uptime, info->LastContactTime);
	env->DeleteLocalRef(clazz);

	return result;
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetMyState
* Signature: (J)I
*/
JNIEXPORT jint JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetMyState
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jint)pIDataDistribution->GetMastershipManager()->GetMyState();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_ChangeMyState
* Signature: (JI)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1ChangeMyState
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jnewState)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return;
	pIDataDistribution->GetMastershipManager()->ChangeMyState((DDM_INSTANCE_STATE)jnewState);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_ChangeState
* Signature: (JJI)V
*/
JNIEXPORT void JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1ChangeState
(JNIEnv * env, jclass caller, jlong jIDataDistribution, jlong jserverId, jint jnewState)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return;
	pIDataDistribution->GetMastershipManager()->ChangeState(jserverId, (DDM_INSTANCE_STATE)jnewState);
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetLocalServerId
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetLocalServerId
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jlong)pIDataDistribution->GetMastershipManager()->GetLocalServerId();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetPrimaryServerId
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetPrimaryServerId
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jlong)pIDataDistribution->GetMastershipManager()->GetPrimaryServerId();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetMessageDelay
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetMessageDelay
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jlong)pIDataDistribution->GetMastershipManager()->GetMessageDelay();
}

/*
* Class:     org_mases_datadistributionmanager_NativeInterface
* Method:    IDataDistributionMastershipCommon_GetUpTime
* Signature: (J)J
*/
JNIEXPORT jlong JNICALL Java_org_mases_datadistributionmanager_NativeInterface_IDataDistributionMastershipCommon_1GetUpTime
(JNIEnv * env, jclass caller, jlong jIDataDistribution)
{
	IDataDistribution* pIDataDistribution = (IDataDistribution*)jIDataDistribution;
	if (pIDataDistribution->GetMastershipManager() == NULL) return NULL;
	return (jlong)pIDataDistribution->GetMastershipManager()->GetUpTime();
}

