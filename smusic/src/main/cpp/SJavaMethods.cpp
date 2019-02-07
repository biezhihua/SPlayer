//
// Created by biezhihua on 2019/2/4.
//

#include "SJavaMethods.h"

#include <pthread.h>

SJavaMethods::SJavaMethods(JavaVM *pVm, JNIEnv *pEnv, jobject pInstance) {
    javaVm = pVm;
    mainJniEnv = pEnv;
    javaInstance = mainJniEnv->NewGlobalRef(pInstance);
}

SJavaMethods::~SJavaMethods() {
    javaVm = NULL;
    mainJniEnv = NULL;
    javaInstance = NULL;
}

void SJavaMethods::onCallJavaPrepared() {

    JNIEnv *jniEnv = NULL;

    if (isMainThread()) {
        jniEnv = mainJniEnv;
    } else {
        jint res = javaVm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6);
        if (res != JNI_OK) {
            res = javaVm->AttachCurrentThread(&jniEnv, NULL);
            if (JNI_OK != res) {
                LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
                return;
            }
        }
    }

    if (jniEnv == NULL) {
        return;
    }

    jclass clazz = jniEnv->GetObjectClass(javaInstance);

    if (clazz == NULL) {
        return;
    }

    jmethodID methodId = jniEnv->GetMethodID(clazz, "onPreparedCallFromNative", "()V");

    if (methodId == NULL) {
        return;
    }

    jniEnv->CallVoidMethod(javaInstance, methodId);

    if (!isMainThread()) {
        javaVm->DetachCurrentThread();
    }
}

bool SJavaMethods::isMainThread() {
    JNIEnv *jniEnv = NULL;
    if (javaVm != NULL) {
        jint res = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
        bool result = res != JNI_EDETACHED;
        LOGD("isMainThread %d %d", res, result);
        return result;
    }
    return false;
}
