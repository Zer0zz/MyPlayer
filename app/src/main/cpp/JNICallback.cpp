//
// Created by Zer0 on 2020/9/2.
//

#include "JNICallback.h"

JNICallback::JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance) {
    this->javaVm = javaVm;
    this->env = env;
    this->instance = env->NewGlobalRef(instance); // 坑，需要是全局（jobject一旦涉及到跨函数，跨线程，必须是全局引用）

    jclass mKevinPlayerClass = env->GetObjectClass(this->instance);
    const char * sig = "()V";
    jmd_repared = env->GetMethodID(mKevinPlayerClass, "onPrepared", sig);
    // env->CallVoidMethod(instance, onPrepared); 这里就可以调用 Java层的方法

    sig = "(I)V";
    jmd_error = env->GetMethodID(mKevinPlayerClass, "onError", sig);
}
/**
 * 析构函数：专门完成释放的工作
 */
JNICallback::~JNICallback() {
    this->javaVm = 0;
    env->DeleteGlobalRef(this->instance);
    this->instance = 0;
    env = 0;
}
/**
 * 坑，此onPrepared函数，是由子线程调用的，所以onPrepared函数时子线程
 * 子线程不能用这种传递过来的env，因为env不能跨线程
 */
void JNICallback::onPrepared(int thread_mode) {
    if (thread_mode == THREAD_MAIN) {
        // 主线程
        env->CallVoidMethod(this->instance, jmd_repared); // 这里就可以调用 Java层的方法
    } else {
        // 子线程
        // 用附加native线程到JVM的方式，来获得权限的env
        // 是子线程的env
        JNIEnv * jniEnv = nullptr;
        jint ret = javaVm->AttachCurrentThread(/*reinterpret_cast<JNIEnv **>(jniEnv)*/ &jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(this->instance, jmd_repared); // 这里就可以调用 Java层的方法
        javaVm->DetachCurrentThread(); // 解除附加
    }
}

void JNICallback::onErrorAction(int thread_mode, int error_code) {
    if (thread_mode == THREAD_MAIN) {
        // 主线程
        env->CallVoidMethod(this->instance, jmd_error,error_code); // 这里就可以调用 Java层的方法
    } else {
        // 子线程
        // 用附加native线程到JVM的方式，来获得权限的env
        // 是子线程的env
        JNIEnv * jniEnv = nullptr;
        jint ret = javaVm->AttachCurrentThread(/*reinterpret_cast<JNIEnv **>(jniEnv)*/ &jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(this->instance, jmd_error,error_code); // 这里就可以调用 Java层的方法
        javaVm->DetachCurrentThread(); // 解除附加
    }
}
