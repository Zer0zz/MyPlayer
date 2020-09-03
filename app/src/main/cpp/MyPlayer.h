//
// Created by Zer0 on 2020/9/2.
//

#ifndef MYPLAYER_MYPLAYER_H
#define MYPLAYER_MYPLAYER_H

#include <pthread.h>
#include <cstring>
#include "JNICallback.h"

extern "C" {
#include <libavformat/avformat.h>
};

class MyPlayer {

public:
    MyPlayer();

    MyPlayer(const char *data_source, JNICallback *jniCallback);

    ~MyPlayer();// 这个类没有子类，所以没有添加虚函数 virtual

    void prepare();

    void start();

    void stop();

    void prepare_();

private:
    char *data_source = 0;
    JNICallback *pCallback = nullptr ;

    //prepare线程ID
    pthread_t pid_prepare;

    //音视频媒体上下文
    AVFormatContext* formatContext = nullptr;

};


#endif //MYPLAYER_MYPLAYER_H
