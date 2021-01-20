//
// Created by shcha on 2021-01-20.
//

#include <unistd.h>
#include <pthread.h>
#include "jni_main.h"

#define LOG_TAG "jni_main.cpp"

static const char *classPathName = "com/shcha/test/NativeApi";

int thr_id;
int end_flag;
pthread_t p_thread[2];
JavaVM *g_jvmPtr;
jclass jObject;
jmethodID funccb;

static jint
add_native(JNIEnv *env, jobject thiz, jint a, jint b) {
    int result = a + b;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "add %d + %d = %d", a, b, result);
    return result;
}

jstring * stringFromJni(JNIEnv *env, jobject thiz)
{
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
    #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    return reinterpret_cast<jstring *>(env->NewStringUTF(
            "Hello from JNI !  Compiled with ABI " ABI "."));
}

jint startThread_Native(JNIEnv *env, jobject thiz) {
    end_flag = 1;
    int b = 2;

    LOGD(LOG_TAG, "Call start thread");

    jclass cls;
    cls = env->FindClass(classPathName);
    if (cls == NULL) {
        LOGE(LOG_TAG, "Can't find the class");
    }
    jObject = (jclass)(env->NewGlobalRef(cls));
    funccb = env->GetStaticMethodID(cls, "callback", "(I)V");

    if (funccb == 0) {
        LOGE(LOG_TAG, "Can't find the function");
        env->DeleteGlobalRef(jObject);
    } else {
        LOGD(LOG_TAG, "Method connect success");
        env->CallStaticVoidMethod(cls, funccb, 10);
    }

    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0) {
        LOGE(LOG_TAG, "Create thread fail\n");
        return -1;
    }
    return 0;
}

static JNINativeMethod methods[] = {
    { "addNative", "(II)I", (void*)add_native },
    { "stringFromJni", "()Ljava/lang/String;", (void*)stringFromJni },
    { "startThreadNative", "()I", (void*)startThread_Native },
};

jint JNI_OnLoad(JavaVM *jvm, void *) {
    jint result = -1;
    JNIEnv* env = NULL;
    jclass cls;

    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6)) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
            "ERROR: GetEnv failed");
    }

    cls = env->FindClass(classPathName);
    if (cls == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
            "ERROR: cls failed");
    }

    if (env->RegisterNatives(cls, methods, sizeof(methods)/sizeof(methods[0])) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
            "ERROR: RegisterNatives");
    }

    g_jvmPtr = jvm;
    result = JNI_VERSION_1_6;
    return result;
}

void *t_function(void *data) {
    int id;
    int i = 0;
    id = *((int*)data);

    while (end_flag) {
        Notify(i);
        i++;
        sleep(1);
        if (i > 10) end_flag = false;
    }
    LOGE(LOG_TAG, "end while loop");
}

void Notify(int n) {
    int value = 0;
    value = n;
    if (!g_jvmPtr) {
        LOGE(LOG_TAG, "error (!g_jvmPtr)");
        return;
    }

    if (!funccb) {
        LOGE(LOG_TAG, "error (!funccb)");
        return;
    }
    JNIEnv *env = NULL;
    g_jvmPtr->AttachCurrentThread(&env, NULL);
    if (env == NULL || jObject == NULL) {
        LOGE(LOG_TAG, "error (env == NULL || jObject == NULL)");
        return;
    }
    env->CallStaticVoidMethod(jObject, funccb, value);
    g_jvmPtr->DetachCurrentThread();
}