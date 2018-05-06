//
// Created by weiersyuan on 2018/5/6.
//

#include <stdlib.h>
extern "C" {
    #include "./thirdparty/ffmpeg/include/libavutil/imgutils.h"
    #include "./thirdparty/ffmpeg/include/libswscale/swscale.h"
    #include "./thirdparty/ffmpeg/include/libavutil/dict.h"
    #include "./thirdparty/ffmpeg/include/libswresample/swresample.h"
    #include "./thirdparty/ffmpeg/include/libavutil/samplefmt.h"
    #include "./thirdparty/ffmpeg/include/libavutil/timestamp.h"
    #include "./thirdparty/ffmpeg/include/libavformat/avformat.h"
}

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <unistd.h>
#include "./common/common.h"
extern "C" {
#include "./libyuv/include/libyuv.h"
}
using namespace libyuv;
#include "com_weiersyuan_useffmpeg_FFmpegGraphrcsActivity.h"

static bool stop = false;

JNIEXPORT void JNICALL Java_com_weiersyuan_useffmpeg_FFmpegGraphrcsActivity_render
        (JNIEnv *env, jobject jobj, jstring jfilepath, jobject surface){
    LOGI("enter Java_com_weiersyuan_useffmpeg_FFmpegGraphrcsActivity_render")
    const char * cFilePath = env->GetStringUTFChars(jfilepath, NULL);
    av_register_all();
    //封装格式上下文
    AVFormatContext * pFormatCtx = avformat_alloc_context();

    // 打开输入视频文件
    if (avformat_open_input(&pFormatCtx, cFilePath, NULL, NULL) < 0){
        LOGE("open file failed")
        return;
    }

    // 获取视频信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("get video info failed")
        return;
    }
    LOGI("bit_rate=%d,faleName=%s,duration=%lld", pFormatCtx->bit_rate, pFormatCtx->filename, pFormatCtx->duration)
    // 视频解码，找到视频对应的ACStream所在pFormatCtx->streams的索引位置
    int video_stream_index = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
//        LOGI("codec type is %p", pFormatCtx->streams[i]->codec->codec_type)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }

    // 获取视频解码器
    AVCodecContext * pCodecCtx = pFormatCtx->streams[video_stream_index]->codec;
   int framerate = pFormatCtx->streams[video_stream_index]->avg_frame_rate.num;
    LOGI("frame rate is %d", framerate)

    AVCodec * pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        LOGE("can not decode")
        return;
    }

    // 打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("decodec can not open")
        return;
    }

    // packet表示编码过的数据
    //编码数据
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    //像素数据（解码数据）
    AVFrame *yuv_frame = av_frame_alloc();
    AVFrame *rgb_frame = av_frame_alloc();

    //native绘制
    //窗体
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env,surface);
    //绘制时的缓冲区
    ANativeWindow_Buffer outBuffer;

    int len ,got_frame, framecount = 0;
    //一帧一帧读取压缩的视频数据AVPacket
    while(!stop && av_read_frame(pFormatCtx,packet) >= 0){
        //解码AVPacket->AVFrame
        len = avcodec_decode_video2(pCodecCtx, yuv_frame, &got_frame, packet);

        //Zero if no frame could be decompressed
        if(got_frame){
            LOGI("解码%d帧",framecount++);
            //lock
            //设置缓冲区的属性（宽、高、像素格式）
            ANativeWindow_setBuffersGeometry(nativeWindow, pCodecCtx->width, pCodecCtx->height,WINDOW_FORMAT_RGBA_8888);
            ANativeWindow_lock(nativeWindow,&outBuffer,NULL);

            //设置rgb_frame的属性（像素格式、宽高）和缓冲区
            //rgb_frame缓冲区与outBuffer.bits是同一块内存
            avpicture_fill((AVPicture *)rgb_frame, (uint8_t *)outBuffer.bits, PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height);

            //YUV->RGBA_8888
            libyuv::I420ToARGB(yuv_frame->data[0],yuv_frame->linesize[0],
                       yuv_frame->data[2],yuv_frame->linesize[2],
                       yuv_frame->data[1],yuv_frame->linesize[1],
                       rgb_frame->data[0], rgb_frame->linesize[0],
                       pCodecCtx->width,pCodecCtx->height);

            //unlock
            ANativeWindow_unlockAndPost(nativeWindow);

            usleep(framerate/30);

        }

        av_free_packet(packet);
    }

    ANativeWindow_release(nativeWindow);
    av_frame_free(&yuv_frame);
    avcodec_close(pCodecCtx);
    avformat_free_context(pFormatCtx);

    env->ReleaseStringUTFChars(jfilepath,cFilePath);

}

JNIEXPORT void JNICALL Java_com_weiersyuan_useffmpeg_FFmpegGraphrcsActivity_stop
        (JNIEnv * env, jobject jobj) {
    stop = true;
}
