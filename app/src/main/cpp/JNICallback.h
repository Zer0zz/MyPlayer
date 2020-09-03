//
// Created by Zer0 on 2020/9/2.
//

#ifndef MYPLAYER_JNICALLBACK_H
#define MYPLAYER_JNICALLBACK_H

#include <jni.h>
#include "macro.h"

class JNICallback {
public:

    JNICallback(JavaVM *javaVm,JNIEnv *env,jobject instance);
    ~JNICallback();

    void onPrepared(int thread_mode);
    void onErrorAction(int thread_mode, int error_code);
private:
    JavaVM *javaVm = 0;
    JNIEnv *env = 0; //理解为主线程的env， 因为env无法跨线程，所以在子线程时需要以上jvm来创建env。
    jobject instance; // 是回调的对象。

    jmethodID jmd_repared;
    jmethodID jmd_error;

};


#endif //MYPLAYER_JNICALLBACK_H
