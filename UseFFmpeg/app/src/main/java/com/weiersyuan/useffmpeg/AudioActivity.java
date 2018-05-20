package com.weiersyuan.useffmpeg;

import android.app.Activity;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;

/**
 * Created by weiersyuan on 2018/5/20.
 */

public class AudioActivity extends Activity{
    public static final String TAG = "AudioActivity";
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio);
        findViewById(R.id.play).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(renderRunnable).start();
            }
        });
    }

    private Runnable renderRunnable = new Runnable() {
        @Override
        public void run() {
            Log.d(TAG, "enter renderRunnable");
            render("/sdcard/DCIM/1234.mp4");
        }
    };

    public AudioTrack createAudioTrack(int sampleRateInHz, int nb_channels){
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        Log.i(TAG, "nb_channels:"+nb_channels);
        int channelConfig;
        if(nb_channels == 1){
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_MONO;
        }else if(nb_channels == 2){
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        }else{
            channelConfig = android.media.AudioFormat.CHANNEL_OUT_STEREO;
        }

        int bufferSizeInBytes = AudioTrack.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat);

        AudioTrack audioTrack = new AudioTrack(
                AudioManager.STREAM_MUSIC,
                sampleRateInHz, channelConfig,
                audioFormat,
                bufferSizeInBytes, AudioTrack.MODE_STREAM);
        return audioTrack;
    }

    public native void render(String input);

}
