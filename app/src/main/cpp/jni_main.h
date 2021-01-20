//
// Created by shcha on 2021-01-20.
//

#ifndef NDK_TEST_JNI_MAIN_H
#define NDK_TEST_JNI_MAIN_H
#include <jni.h>
#include <android/log.h>

#define LOGD(tag, fmt, ...) __android_log_print(\
                                ANDROID_LOG_DEBUG, tag, fmt \
                                "\n--> %s\n----> %s:%d", \
                                ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#define LOGE(tag, fmt, ...) __android_log_print(\
                                ANDROID_LOG_ERROR, tag, fmt \
                                "\n--> %s\n----> %s:%d", \
                                ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)

void *t_function(void *data);
void Notify(int n);

#endif //NDK_TEST_JNI_MAIN_H
