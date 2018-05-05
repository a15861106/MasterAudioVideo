//
// Created by weiersyuan on 2018/5/5.
//
#define __STDC_CONSTANT_MACROS
extern "C" {
    #include "./thirdparty/ffmpeg/include/libavutil/imgutils.h"
    #include "./thirdparty/ffmpeg/include/libswscale/swscale.h"
    #include "./thirdparty/ffmpeg/include/libavutil/dict.h"
    #include "./thirdparty/ffmpeg/include/libswresample/swresample.h"
    #include "./thirdparty/ffmpeg/include/libavutil/samplefmt.h"
    #include "./thirdparty/ffmpeg/include/libavutil/timestamp.h"
    #include "./thirdparty/ffmpeg/include/libavformat/avformat.h"
}

#include "com_weiersyuan_useffmpeg_MainActivity.h"


JNIEXPORT jstring JNICALL Java_com_weiersyuan_useffmpeg_MainActivity_stringFromJNI
        (JNIEnv * env, jobject jobj) {
    av_register_all();
    return env->NewStringUTF("test ffmpeg");
}

