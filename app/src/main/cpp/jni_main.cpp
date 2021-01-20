//
// Created by shcha on 2021-01-20.
//

#include "jni_main.h"

static const char *classPathName = "com/shcha/test/NativeApi";

static jint
add(JNIEnv *env, jobject thiz, jint a, jint b) {
    int result = a + b;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "add %d + %d = %d", a, b, result);
    return result;
}

static JNINativeMethod methods[] = {
    { "add", "(II)I", (void*)add },
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
    result = JNI_VERSION_1_6;
    g_jvmPtr = jvm;
    return result;
}