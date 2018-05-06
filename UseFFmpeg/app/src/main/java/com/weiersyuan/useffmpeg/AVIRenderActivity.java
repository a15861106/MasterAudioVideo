package com.weiersyuan.useffmpeg;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Created by weiersyuan on 2018/5/5.
 */

public class AVIRenderActivity extends Activity implements SurfaceHolder.Callback {
    private static final String TAG = "AVIRenderActivity";
    Handler handler = new Handler();

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private ImageView iv;


    static {
        System.loadLibrary("avlib");
    }

    private AtomicBoolean isPlaying = new AtomicBoolean(false);

    long avi;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_render);
        surfaceView = findViewById(R.id.sv);
        iv = findViewById(R.id.iv);
        try {
            avi = open("/sdcard/DCIM/1234.avi");
            Log.d(TAG, "open file success, avi=" + avi);
        } catch (IOException e) {
            e.printStackTrace();
        }
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                isPlaying.set(true);
                new Thread(renderRunnable).start();
            }
        }, 3000);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        close(avi);
    }

    public native static long open(String fileName) throws IOException;

    public native static int getWidth(long fileDecs);

    public native static int getHeight(long fileDecs);

    public native static double getFrameRate(long fileDecs);

    public native static void close(long fileDecs);

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        isPlaying.set(false);
    }

    Bitmap bitmap;
    // 渲染线程，通过bitmap将avi中的视频帧渲染到surface上
    private Runnable renderRunnable = new Runnable() {
        @Override
        public void run() {
            boolean enter = false;
            Log.d(TAG, "enter renderRunnable");
            bitmap = Bitmap.createBitmap(getWidth(avi), getHeight(avi), Bitmap.Config.ARGB_8888);long frameDelay = (long) (1000/getFrameRate(avi));

            while (isPlaying.get()) {
                render(avi, bitmap);
                if (!enter) {
                    handler.post(new Runnable() {
                        @Override
                        public void run() {
                            iv.setImageBitmap(bitmap);
                        }
                    });
                    enter = true;
                }

                Canvas canvas = surfaceHolder.lockCanvas();
                canvas.drawBitmap(bitmap, 0, 0, null);
                surfaceHolder.unlockCanvasAndPost(canvas);

                try{
                    Thread.sleep(frameDelay);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };

    public native boolean render(long avi, Bitmap bitmap);
}
