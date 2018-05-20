package com.weiersyuan.useffmpeg;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "MainActivity";

    static {
        System.loadLibrary("avlib");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG, stringFromJNI());

        findViewById(R.id.play_video).setOnClickListener(this);
        findViewById(R.id.play_audio).setOnClickListener(this);
    }

    public native String stringFromJNI();

    @Override
    public void onClick(View view) {
        Intent intent = new Intent();

        switch (view.getId()) {
            case R.id.play_video:
                intent.setClass(MainActivity.this, VideoActivity.class);
                break;
            case R.id.play_audio:
                intent.setClass(MainActivity.this, AudioActivity.class);
                break;
        }
        startActivity(intent);
    }
}
