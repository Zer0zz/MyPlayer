#include <jni.h>
#include <string>

// 这个坑不要在踩了
extern "C" {
#include <libavutil/avutil.h>
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_myplayer_MyPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source_) {
    const char *data_source = env->GetStringUTFChars(data_source_, NULL);

    // 释放操作
    env->ReleaseStringUTFChars(data_source_, data_source);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myplayer_MyPlayer_startNative(JNIEnv *env, jobject thiz) {
    // TODO: implement startNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myplayer_MyPlayer_stopNative(JNIEnv *env, jobject thiz) {
    // TODO: implement stopNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myplayer_MyPlayer_releaseNative(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseNative()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myplayer_MyPlayer_getFFmpegVersion(JNIEnv *env, jobject thiz) {
    std::string hello = "FFmpeg version : ";
    hello.append(av_version_info());
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myplayer_MyPlayer_setSurfaceNative(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement setSurfaceNative()
}