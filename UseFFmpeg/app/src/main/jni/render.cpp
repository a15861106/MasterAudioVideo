//
// Created by weiersyuan on 2018/5/5.
//
extern "C" {
    #include "avilib.h"

}

#include "./common/common.h"
#include <android/bitmap.h>
#include "com_weiersyuan_useffmpeg_RenderActivity.h"

JNIEXPORT jlong JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_open
        (JNIEnv * env, jclass clazz, jstring jFileName){
    avi_t * avi = 0;
    const char * cFileName = env->GetStringUTFChars(jFileName, 0);
    LOGI("enter Java_com_weiersyuan_useffmpeg_RenderActivity_open jFileName=%s", cFileName);
    if (cFileName == 0) {
        return (jlong)avi;
    }

    avi = AVI_open_input_file(cFileName, -1);
    LOGI("enter Java_com_weiersyuan_useffmpeg_RenderActivity_open avi=%p", avi);
    env->ReleaseStringUTFChars(jFileName, cFileName);
    if (avi == 0) {
        ThrowException(env, "java/io/IOException", AVI_strerror());
    }
    return (jlong)avi;

}

JNIEXPORT jint JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_getWidth
        (JNIEnv * env, jclass clazz, jlong avi) {
    return AVI_video_width((avi_t*)avi);
}


JNIEXPORT jint JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_getHeight
        (JNIEnv * env, jclass clazz, jlong avi) {
    return AVI_video_height((avi_t*)avi);
}


JNIEXPORT jdouble JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_getFrameRate
        (JNIEnv * env, jclass clazz, jlong avi) {
    return AVI_frame_rate((avi_t*)avi);
}


JNIEXPORT void JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_close
        (JNIEnv * env, jclass clazz, jlong avi) {
    AVI_close((avi_t*)avi);
}

JNIEXPORT jboolean JNICALL Java_com_weiersyuan_useffmpeg_RenderActivity_render
        (JNIEnv * env, jobject clazz, jlong avi, jobject jBitmap) {

    char * frameBuffer;
    int keyFrame;
    long frameSize;
    if (AndroidBitmap_lockPixels(env, jBitmap, (void**)&frameBuffer) < 0) {
        ThrowException(env, "java/io/IOException", "unable to lock bitmap");
        return JNI_FALSE;
    }
    frameSize = AVI_read_frame((avi_t*)avi, frameBuffer, &keyFrame);

    if(AndroidBitmap_unlockPixels(env, jBitmap) < 0) {
        ThrowException(env, "java/io/IOException", "unable to lock bitmap");
    }
    LOGI("render");
    if (frameSize > 0) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}
