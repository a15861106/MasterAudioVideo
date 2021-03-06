/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_weiersyuan_useffmpeg_RenderActivity */

#ifndef _Included_com_weiersyuan_useffmpeg_RenderActivity
#define _Included_com_weiersyuan_useffmpeg_RenderActivity
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    open
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_open
  (JNIEnv *, jclass, jstring);

/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    getWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_getWidth
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    getHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_getHeight
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    getFrameRate
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_getFrameRate
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    close
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_close
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_weiersyuan_useffmpeg_RenderActivity
 * Method:    render
 * Signature: (JLjava/lang/Object;)V
 */
JNIEXPORT jboolean JNICALL Java_com_weiersyuan_useffmpeg_AVIRenderActivity_render
  (JNIEnv *, jobject, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif
