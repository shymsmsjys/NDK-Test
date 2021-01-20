//
// Created by shcha on 2021-01-20.
//

#include "jni_main.h"

#define LOG_TAG "jni_main.cpp"

static const char *classPathName = "com/shcha/test/NativeApi";


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

static JNINativeMethod methods[] = {
    { "addNative", "(II)I", (void*)add_native },
    { "stringFromJni", "()Ljava/lang/String;", (void*)stringFromJni },
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
    return result;
}