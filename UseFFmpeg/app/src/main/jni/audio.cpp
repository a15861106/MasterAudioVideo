//
// Created by weiersyuan on 2018/5/20.
//

#include <common/common.h>
#include "com_weiersyuan_useffmpeg_AudioActivity.h"
#include <stdlib.h>
#define MAX_AUDIO_FRME_SIZE 44100 * 4

extern "C"{
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
};

JNIEXPORT void JNICALL Java_com_weiersyuan_useffmpeg_AudioActivity_render
        (JNIEnv * env, jobject jobj, jstring pathName) {
    const char* input_cstr = env->GetStringUTFChars(pathName, NULL);
    av_register_all();
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx,input_cstr,NULL,NULL) != 0){
        LOGI("%s","can not open audio");
        return;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL) < 0){
        LOGI("%s","cannot find stream info");
        return;
    }
    int i = 0;
    int audio_stream_idx = -1;
    for(; i < pFormatCtx->nb_streams;i++){
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            audio_stream_idx = i;
            break;
        }
    }
    AVCodecContext *codecCtx = pFormatCtx->streams[audio_stream_idx]->codec;
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if(codec == NULL){
        LOGI("%s","can not get decoder");
        return;
    }
    if(avcodec_open2(codecCtx,codec,NULL) < 0){
        LOGI("%s","can not open decoder");
        return;
    }
    // 重采样,frame->16bit 44100 PCM
    SwrContext *swrCtx = swr_alloc();
    enum AVSampleFormat in_sample_fmt = codecCtx->sample_fmt;
    //输出采样格式16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int in_sample_rate = codecCtx->sample_rate;
    int out_sample_rate = in_sample_rate;
    //输入声道布局 av_get_default_channel_layout(codecCtx->channels);
    uint64_t in_ch_layout = codecCtx->channel_layout;
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;

    swr_alloc_set_opts(swrCtx, out_ch_layout,out_sample_fmt,out_sample_rate,
                       in_ch_layout,in_sample_fmt,in_sample_rate, 0, NULL);
    swr_init(swrCtx);
    //输出的声道个数
    int out_channel_nb = av_get_channel_layout_nb_channels(out_ch_layout);

    jclass jclazz = env->GetObjectClass(jobj);
    jmethodID create_audio_track_methodid = env->GetMethodID(jclazz, "createAudioTrack", "(II)Landroid/media/AudioTrack;");
    jobject audio_track = env->CallObjectMethod(jobj, create_audio_track_methodid, out_sample_rate, out_channel_nb);

    jclass audio_track_class = env->GetObjectClass(audio_track);
    jmethodID audio_track_play_method = env->GetMethodID(audio_track_class,"play","()V");
    env->CallVoidMethod(audio_track, audio_track_play_method);

    //AudioTrack.write
    jmethodID audio_track_write_method = env->GetMethodID(audio_track_class,"write","([BII)I");

    //16bit 44100 PCM 数据
    uint8_t *out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRME_SIZE);

    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    AVFrame *frame = av_frame_alloc();
    int got_frame = 0,index = 0, ret;
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == audio_stream_idx) {
            ret = avcodec_decode_audio4(codecCtx, frame, &got_frame, packet);
            if (ret < 0) {
                LOGI("decode finish")
            }
            if (got_frame > 0) {
                LOGI("解码：%d",index++);
                swr_convert(swrCtx, &out_buffer, MAX_AUDIO_FRME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
                int out_buffer_size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples, out_sample_fmt, 1);

                //out_buffer缓冲区数据，转成byte数组
                jbyteArray audio_sample_array = env->NewByteArray(out_buffer_size);
                jbyte* sample_bytep = env->GetByteArrayElements(audio_sample_array,NULL);
                //out_buffer的数据复制到sampe_bytep
                memcpy(sample_bytep,out_buffer,out_buffer_size);
                //同步
                env->ReleaseByteArrayElements(audio_sample_array,sample_bytep,0);
                //AudioTrack.write PCM数据
                env->CallIntMethod(audio_track,audio_track_write_method, audio_sample_array,0,out_buffer_size);
                //释放局部引用
                env->DeleteLocalRef(audio_sample_array);
            }
        }
    }
    av_free_packet(packet);
    av_frame_free(&frame);
    av_free(out_buffer);
    swr_free(&swrCtx);
    avcodec_close(codecCtx);
    avformat_close_input(&pFormatCtx);
    env->ReleaseStringUTFChars(pathName,input_cstr);
}
