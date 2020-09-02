package com.example.myplayer;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MyPlayer {
    static {
        System.loadLibrary("myplayer");
    }

    // 媒体流的源 (文件路径 、 直播地址（要集成X264才行）)
    private String dataSource;
    private OnPreparedListener onPreparedListener;
    private SurfaceHolder surfaceHolder;

    public MyPlayer(String path) {
        dataSource = path;
    }

    public void setSurfaceHolder(SurfaceView surfaceView) {
        if (null != this.surfaceHolder) {
//            this.surfaceHolder.removeCallback(this);
        }
        this.surfaceHolder = surfaceView.getHolder();
//        this.surfaceHolder.addCallback(this);
    }

    /**
     * MainActivity设置监听，就可以回调到MainActivity的方法，进行UI的操作了
     * @param onPreparedListener
     */
    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    /**
     * 给JNI 方式调用的方法
     */
    public void onPrepared() {
        if (null != onPreparedListener) {
            this.onPreparedListener.onPrepared();
        }
    }
    /**
     * 给JNI 方式回调的方法（进行错误的回调）
     */
    public void onError(int errorCode) {
        if (null != onPreparedListener) {
            String errorText = null;

            switch (errorCode) {
                case Flags.FFMPEG_ALLOC_CODEC_CONTEXT_FAIL:
                    errorText = "无法根据解码器创建上下文";
                    break;
                case Flags.FFMPEG_CAN_NOT_FIND_STREAMS:
                    errorText = "找不到媒体流信息";
                    break;
                case Flags.FFMPEG_CAN_NOT_OPEN_URL:
                    errorText = "打不开媒体数据源";
                    break;
                case Flags.FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL:
                    errorText = "根据流信息 配置上下文参数失败";
                    break;
                case Flags.FFMPEG_FIND_DECODER_FAIL:
                    errorText = "找不到解码器";
                    break;
                case Flags.FFMPEG_NOMEDIA:
                    errorText = "没有音视频";
                    break;
                case Flags.FFMPEG_READ_PACKETS_FAIL:
                    errorText = "读取媒体数据包失败";
                    break;
                default:
                    errorText = "未知错误，自己去检测你的垃圾代码...";
                    break;
            }
            this.onPreparedListener.onError(errorText);
        }
    }


    interface OnPreparedListener {
        void onPrepared();

        void onError(String errorText);
    }

    public native void prepareNative(String dataSource);
    public native void startNative();
    public native void stopNative();
    public native void releaseNative();
    public native String getFFmpegVersion();
    private native void setSurfaceNative(Surface surface);

}
