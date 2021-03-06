#ifndef SPLAYER_ANDROID_GLESVIDEODEVICE_H
#define SPLAYER_ANDROID_GLESVIDEODEVICE_H

#include "VideoDevice.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include "EglHelper.h"
#include "InputRenderNode.h"

class AndroidVideoDevice : public VideoDevice {

    const char *const TAG = "[MP][ANDROID][VideoDevice]";

private:

    /// Surface窗口
    ANativeWindow *window = nullptr;

    /// EGL帮助器
    EglHelper *eglHelper = nullptr;

    /// eglSurface
    EGLSurface eglSurface = nullptr;

    /// 重新设置Surface
    bool windowReset;

    /// 是否存在Surface
    bool hasWindow;

    /// 窗口宽度
    int windowWidth;

    /// 窗口高度
    int windowHeight;

    /// EGLSurface
    bool haveEGLSurface;

    /// 释放资源
    bool haveEGlContext;

    /// 视频纹理
    Texture *videoTexture = nullptr;

    // 输入渲染结点
    InputRenderNode *renderNode = nullptr;

    /// 顶点坐标
    float vertices[8];

    /// 纹理坐标
    float textureVertices[8];
public:

    AndroidVideoDevice();

    ~AndroidVideoDevice() override;

    int create() override;

    int destroy() override;

    int onInitTexture(int initTexture, int newWidth, int newHeight, TextureFormat format,
                      BlendMode blendMode, int rotate) override;

    int onUpdateYUV(uint8_t *yData, int yPitch, uint8_t *uData, int uPitch, uint8_t *vData,
                    int vPitch) override;

    int onUpdateARGB(uint8_t *rgba, int pitch) override;

    void onRequestRenderStart(Frame *frame) override;

    int onRequestRenderEnd(Frame *frame, bool flip) override;

    TextureFormat getTextureFormat(int format) override;

    BlendMode getBlendMode(TextureFormat format) override;

    int setNativeWindow(ANativeWindow *nativeWindow);

private:

    void destroy(bool releaseContext);

    void resetVertices();

    void resetTexVertices();
};


#endif
