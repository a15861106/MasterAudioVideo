package com.weiersyuan.useffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * 只播放视频
 * Created by weiersyuan on 2018/5/6.
 */

public class VideoActivity extends Activity implements SurfaceHolder.Callback {
    private static final String TAG = "VideoActivity";

    SurfaceView surfaceView;
    private AtomicBoolean isPlaying = new AtomicBoolean(false);

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);

        surfaceView = findViewById(R.id.sv);
        surfaceView.getHolder().addCallback(this);

        findViewById(R.id.play).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isPlaying.set(true);
                new Thread(renderRunnable).start();
            }
        });
    }

    public native void render(String filePath, Surface surface);
    public native void stop();

    private Runnable renderRunnable = new Runnable() {
        @Override
        public void run() {
            Log.d(TAG, "enter renderRunnable");
            render("/sdcard/DCIM/1234.mp4", surfaceView.getHolder().getSurface());
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stop();
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        stop();
    }
}
