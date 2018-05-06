//
// Created by weiersyuan on 2018/5/6.
//

#ifndef USEFFMPEG_COMMON_H
#define USEFFMPEG_COMMON_H

#include <jni.h>
#include <android/log.h>
#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"weiersyuan",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"weiersyuan",FORMAT,##__VA_ARGS__);

void ThrowException(JNIEnv * env, const char* className, const char* message);


#endif //USEFFMPEG_COMMON_H
