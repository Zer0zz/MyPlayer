//
// Created by Zer0 on 2020/9/2.
//




#include "MyPlayer.h"

MyPlayer::MyPlayer() {

}

MyPlayer::~MyPlayer() {

    if (this->data_source) {
        delete this->data_source;
        this->data_source = nullptr;
    }

}

// TODO 异步 函数指针 - 准备工作prepare
void *customTaskPrepareThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);
    myPlayer->prepare_();
    return nullptr;
}

MyPlayer::MyPlayer(const char *data_source, JNICallback *pCallback) {
    // 这里有坑，这里赋值之后，不能给其他地方用，因为被释放了，变成了悬空指针
    // this->data_source = data_source;

    // 解决上面的坑，自己Copy才行
    // [strlen(data_source)] 这段代码有坑：因为（hello  而在C++中是 hello\n），所以需要加1
    this->data_source = new char[strlen(data_source) + 1];
    strcpy(this->data_source, data_source);

    this->pCallback = pCallback;
}

void MyPlayer::prepare() {
    // 解析媒体流，通过ffmpeg API 来解析 dataSource
    // 思考：可以直接解析吗？
    // 答：1.可能是文件，考虑到io流
    //     2.可能是直播，考虑到网络
    // 所以需要异步

    // 是由MainActivity中方法调用下来的，所以是属于在main线程，不能再main线程操作，所以需要异步
    // 创建异步线程
    pthread_create(&pid_prepare, 0, customTaskPrepareThread, this);
}

void MyPlayer::start() {

}

void MyPlayer::stop() {

}

void MyPlayer::prepare_() {
    // TODO 【大流程是：解封装】
    // TODO 第一步：打开媒体地址（文件路径 、 直播地址）

    // 可以初始为NULL，如果初始为NULL，当执行avformat_open_input函数时，内部会自动申请avformat_alloc_context，这里干脆手动申请
    // 封装了媒体流的格式信息
    formatContext = avformat_alloc_context();
    // 字典：键值对的
//    AVDictionary *dictionary = 0;
//    av_dict_set(&dictionary, "timeout", "5000000", 0); // 单位是微妙

    AVDictionary *dictionary = 0;
    av_dict_set(&dictionary, "timeout", "5000000", 0);
    /**
     * 1.AVFormatContext 二级指针
     * 2.媒体文件路径，或，直播地址   注意：this->data_source; 这样是拿不到的，所以需要把this传递到pVoid
     * 3.输入的封装格式：一般是让ffmpeg自己去检测，所以给了一个0
     * 4.参数
     */
    int ret = avformat_open_input(&formatContext, data_source, 0, &dictionary);

    av_dict_free(&dictionary); // 释放字典

    if (ret != 0) {
        // 你的文件路径，或，你的文件损坏了，需要告诉用户
        // 把错误信息，告诉给Java层去（回调给Java）
        if (pCallback) {
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        }
        return;
    }

    // TODO 第二步：查找媒体中的音视频流的信息
    ret = avformat_find_stream_info(formatContext, 0);
    if (ret < 0) {
        // 把错误信息，告诉给Java层去（回调给Java）
        if (pCallback) {
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        }
        return;
    }

    // TODO 第三步：根据流信息，流的个数，循环查找， 音频流 视频流
    // nb_streams == 流的个数
    for (int stream_index = 0; stream_index < formatContext->nb_streams; ++stream_index) {
        // TODO 第四步：获取媒体流（视频、音频）
        AVStream * stream = formatContext->streams[stream_index];

        // TODO 第五步：从stream流中获取解码这段流的参数信息，区分到底是 音频 还是 视频
        AVCodecParameters * codecParameters = stream->codecpar;

        // TODO 第六步：通过流的编解码参数中编解码ID，来获取当前流的解码器
        AVCodec * codec = avcodec_find_decoder(codecParameters->codec_id);
        // 虽然在第六步，已经拿到当前流的解码器了，但可能不支持解码这种解码方式
        if (!codec) { // 如果为空，就代表：解码器不支持
            // 把错误信息，告诉给Java层去（回调给Java）
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }

        // TODO 第七步：通过 拿到的解码器，获取解码器上下文
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            // 把错误信息，告诉给Java层去（回调给Java）
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }

        // TODO 第八步：给解码器上下文 设置参数
        ret = avcodec_parameters_to_context(codecContext, codecParameters);
        if (ret < 0) {
            // 把错误信息，告诉给Java层去（回调给Java）
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }

        // TODO 第九步：打开解码器
        ret =  avcodec_open2(codecContext, codec, 0);
        if (ret) {
            // 把错误信息，告诉给Java层去（回调给Java）
            pCallback->onErrorAction(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        // TODO 第十步：从编码器参数中获取流类型codec_type
//        if (codecParameters->codec_type == AVMEDIA_TYPE_AUDIO) { // 音频流
//            audioChannel = new AudioChannel(stream_index, codecContext);
//        } else if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO) { // 视频流
//            videoChannel = new VideoChannel(stream_index, codecContext);
//            videoChannel->setRenderCallback(renderCallback);
//        }
    } // end for
}


