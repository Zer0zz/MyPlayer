// TODO 视频解码 和 音频解码 有公用的东西，所以需要抽取父类

#ifndef KEVINPLAYER_BASECHANNEL_H
#define KEVINPLAYER_BASECHANNEL_H

extern "C" {
#include <libavutil/time.h>
#include <libavcodec/avcodec.h>
};
#include "safe_queue.h"


class BaseChannel {
public:
    int stream_index;

    BaseChannel(int stream_index, AVCodecContext *pContext) {
        this->stream_index = stream_index;
        this->pContext = pContext;
        packages.setReleaseCallback(releaseAVPacket);
        frames.setReleaseCallback(releaseAVFrame);
    }

    // 注意：由于是父类，析构函数，必须是虚函数
    virtual ~BaseChannel() {
        packages.clearQueue();
        frames.clearQueue();
    }

    /**
     * 释放AVPacket 队列
     * @param avPacket
     */
    static void releaseAVPacket(AVPacket **avPacket) {
        if (avPacket) {
            av_packet_free(avPacket);
            *avPacket = 0;
        }
    }

    /**
     * 释放AVFrame 队列
     * @param avFrame
     */
    static void releaseAVFrame(AVFrame **avFrame) {
        if (avFrame) {
            av_frame_free(avFrame);
            *avFrame = 0;
        }
    }

    // AVPacket  音频：aac，  视频：h264
    SafeQueue<AVPacket *> packages; // 音频 或者 视频 的压缩数据包 (是编码的数据包)

    // AVFrame 音频：PCM，   视频：YUV
    SafeQueue<AVFrame *> frames; // 音频 或者 视频 的原始数据包（可以直接 渲染 和 播放 的）

    bool isPlaying = 1;

    AVCodecContext *pContext;
};


#endif //KEVINPLAYER_BASECHANNEL_H
