package com.example.myplayer;

import android.os.Bundle;
import android.view.SurfaceView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    //更多直播流地址参见：https://blog.csdn.net/XiaoYuWen1242466468/article/details/90287886
    private final static String PATH = "rtmp://58.200.131.2:1935/livetv/hunantv";

    private SurfaceView surfaceView;
    private MyPlayer myPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        surfaceView = findViewById(R.id.surface_view);
        myPlayer = new MyPlayer(PATH);
        myPlayer.setSurfaceHolder(surfaceView);

        myPlayer.setOnPreparedListener(new MyPlayer.OnPreparedListener() {
            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        new AlertDialog.Builder(MainActivity.this)
                                .setTitle("UI")
                                .setMessage("准备好了，开始播放 ...")
                                .setPositiveButton("老夫知道了", null)
                                .show();
                    }
                });
                myPlayer.start();
            }

            @Override
            public void onError(final String errorText) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        new AlertDialog.Builder(MainActivity.this)
                                .setTitle("Error")
                                .setMessage("已经发生错误，请查阅:" + errorText)
                                .setPositiveButton("我来个去，什么情况", null)
                                .show();
                    }
                });
            }
        });

    }
    @Override
    protected void onResume() {
        super.onResume();
        myPlayer.prepare();
    }

    @Override
    protected void onStop() {
        super.onStop();
        myPlayer.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        myPlayer.release();
    }
}
