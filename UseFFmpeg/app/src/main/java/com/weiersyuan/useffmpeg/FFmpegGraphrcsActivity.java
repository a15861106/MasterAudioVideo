package com.weiersyuan.useffmpeg;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Created by weiersyuan on 2018/5/6.
 */

public class FFmpegGraphrcsActivity extends Activity {
    private static final String TAG = "FFmpegGraphrcsActivity";

    Handler handler = new Handler();
    SurfaceView surfaceView;
    private AtomicBoolean isPlaying = new AtomicBoolean(false);

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        surfaceView = new SurfaceView(this);
        setContentView(surfaceView);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                isPlaying.set(true);
                new Thread(renderRunnable).start();
            }
        }, 3000);
    }

    public native void render(String filePath, Surface surface);
    public native void stop();

    private Runnable renderRunnable = new Runnable() {
        @Override
        public void run() {
            Log.d(TAG, "enter renderRunnable");
            if (isPlaying.get()) {
                render("/sdcard/DCIM/1234.mp4", surfaceView.getHolder().getSurface());
            }
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        stop();
    }
}
