
#include <SJavaMethods.h>

#define TAG "Native_JavaMethods"

SJavaMethods::SJavaMethods(JavaVM *pVm, JNIEnv *pEnv, jobject pInstance) {
    javaVm = pVm;
    mainJniEnv = pEnv;
    javaInstance = mainJniEnv->NewGlobalRef(pInstance);
    jclass jClazz = mainJniEnv->GetObjectClass(javaInstance);
    if (jClazz != NULL) {
        idCreate = mainJniEnv->GetMethodID(jClazz, "onPlayerCreateFromNative", "()V");
        idStart = mainJniEnv->GetMethodID(jClazz, "onPlayerStartFromNative", "()V");
        idPlay = mainJniEnv->GetMethodID(jClazz, "onPlayerPlayFromNative", "()V");
        idPause = mainJniEnv->GetMethodID(jClazz, "onPlayerPauseFromNative", "()V");
        idStop = mainJniEnv->GetMethodID(jClazz, "onPlayerStopFromNative", "()V");
        idDestroy = mainJniEnv->GetMethodID(jClazz, "onPlayerDestroyFromNative", "()V");
        idTime = mainJniEnv->GetMethodID(jClazz, "onPlayerTimeFromNative", "(II)V");
        idError = mainJniEnv->GetMethodID(jClazz, "onPlayerErrorFromNative", "(ILjava/lang/String;)V");
        idComplete = mainJniEnv->GetMethodID(jClazz, "onPlayerCompleteFromNative", "()V");
        idLoad = mainJniEnv->GetMethodID(jClazz, "onPlayerLoadStateFromNative", "(Z)V");
        idRender = mainJniEnv->GetMethodID(jClazz, "onPlayerRenderYUVFromNative", "(II[B[B[B)V");
    }
}

SJavaMethods::~SJavaMethods() {
    mainJniEnv->DeleteGlobalRef(javaInstance);
    javaInstance = NULL;
    mainJniEnv = NULL;
    javaVm = NULL;
}

bool SJavaMethods::isMainThread() {
    JNIEnv *jniEnv = NULL;
    if (javaVm != NULL) {
        jint res = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
        bool result = res != JNI_EDETACHED;
        // LOGD(TAG,"isMainThread %d %d", res, result);
        return result;
    }
    return false;
}

void SJavaMethods::onCallJavaCreate() {
    LOGD(TAG, "SJavaMethods:onCallJavaCreate");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idCreate);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaStart() {
    LOGD(TAG, "SJavaMethods:onCallJavaStart");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idStart);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaPlay() {
    LOGD(TAG, "SJavaMethods:onCallJavaPlay");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idPlay);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaPause() {
    LOGD(TAG, "SJavaMethods:onCallJavaPause");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idPause);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaStop() {
    LOGD(TAG, "SJavaMethods:onCallJavaStop");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idStop);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaDestroy() {
    LOGD(TAG, "SJavaMethods:onCallJavaDestroy");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idDestroy);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaTimeFromThread(int totalTimeMillis, int currentTimeMillis) {
    LOGD(TAG, "SJavaMethods:onCallJavaTime: %d %d", totalTimeMillis, currentTimeMillis);
    JNIEnv *jniEnv;
    if (javaVm->AttachCurrentThread(&jniEnv, 0) == JNI_OK) {
        jniEnv->CallVoidMethod(javaInstance, idTime, (jint) totalTimeMillis, (jint) currentTimeMillis);
        javaVm->DetachCurrentThread();
    }
}

void SJavaMethods::tryUnLoadEnv() {
    if (!isMainThread()) {
        javaVm->DetachCurrentThread();
    }
}

JNIEnv *SJavaMethods::tryLoadEnv() {
    JNIEnv *jniEnv = NULL;
    if (isMainThread()) {
        jniEnv = mainJniEnv;
    } else {
        int res = javaVm->AttachCurrentThread(&jniEnv, NULL);
        if (JNI_OK != res) {
            LOGE(TAG, "Failed to AttachCurrentThread, ErrorCode = %d", res);
            jniEnv = NULL;
        }
    }
    return jniEnv;
}

void SJavaMethods::onCallJavaError(int code, const char *message) {
    LOGD(TAG, "SJavaMethods:onCallJavaError: %s", message);
    JNIEnv *jniEnv;
    if (javaVm->AttachCurrentThread(&jniEnv, 0) == JNI_OK) {
        jstring jMessage = jniEnv->NewStringUTF(message);
        jniEnv->CallVoidMethod(javaInstance, idError, code, jMessage);
        jniEnv->DeleteLocalRef(jMessage);
        javaVm->DetachCurrentThread();
    }
}

void SJavaMethods::onCallJavaComplete() {
    LOGD(TAG, "SJavaMethods:onCallJavaComplete");
    JNIEnv *jniEnv = tryLoadEnv();
    if (jniEnv != NULL) {
        jniEnv->CallVoidMethod(javaInstance, idComplete);
        tryUnLoadEnv();
    }
}

void SJavaMethods::onCallJavaLoadState(bool loadState) {
    JNIEnv *jniEnv;
    LOGD(TAG, "SJavaMethods:onCallJavaLoadState %d", loadState);
    if (javaVm->AttachCurrentThread(&jniEnv, 0) == JNI_OK) {
        jniEnv->CallVoidMethod(javaInstance, idLoad, loadState);
        javaVm->DetachCurrentThread();
    }
}

void SJavaMethods::onCallJavaRenderYUVFromThread(int width, int height, uint8_t *y, uint8_t *u, uint8_t *v) {
    LOGD(TAG, "SJavaMethods:onCallJavaRenderYUVFromThread: %d %d", width, height);
    JNIEnv *jniEnv;
    if (javaVm->AttachCurrentThread(&jniEnv, 0) == JNI_OK) {

        jbyteArray yByte = jniEnv->NewByteArray(width * height);
        jniEnv->SetByteArrayRegion(yByte, 0, width * height, (const jbyte *) (y));

        jbyteArray uByte = jniEnv->NewByteArray(width * height / 4);
        jniEnv->SetByteArrayRegion(uByte, 0, width * height / 4, (const jbyte *) (u));

        jbyteArray vByte = jniEnv->NewByteArray(width * height / 4);
        jniEnv->SetByteArrayRegion(vByte, 0, width * height / 4, (const jbyte *) (v));

        jniEnv->CallVoidMethod(javaInstance, idRender, (jint) width, (jint) height, yByte, uByte, vByte);

        jniEnv->DeleteLocalRef(yByte);
        jniEnv->DeleteLocalRef(uByte);
        jniEnv->DeleteLocalRef(vByte);

        javaVm->DetachCurrentThread();
    }
}
