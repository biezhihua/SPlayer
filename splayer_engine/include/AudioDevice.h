#ifndef ENGINE_AUDIO_DEVICE_H
#define ENGINE_AUDIO_DEVICE_H

#include "PlayerInfoStatus.h"

// 音频PCM填充回调
typedef void (*AudioPCMCallback)(void *userdata, uint8_t *stream, int len);

typedef struct AudioDeviceSpec {
    /// 采样率
    int sampleRate;

    /// 音频采样格式
    AVSampleFormat format;

    /// 声道
    uint8_t channels;

    /// 采样大小
    uint16_t samples;

    /// 缓冲区大小
    uint32_t size;

    /// 音频回调
    AudioPCMCallback callback;

    /// 音频上下文
    void *userdata;
} AudioDeviceSpec;

class AudioDevice : public Runnable {
    const char *const TAG = "[MP][NATIVE][AudioDevice]";
public:
    AudioDevice();

    virtual ~AudioDevice();

    virtual int create();

    virtual void destroy();

    /// 打开音频设备
    virtual int open(AudioDeviceSpec *desired, AudioDeviceSpec *obtained);

    virtual void start();

    virtual void stop();

    virtual void pause();

    virtual void resume();

    virtual void flush();

    virtual void setStereoVolume(float left_volume, float right_volume);

    virtual void setMute(bool mute);

    virtual void run();
};


#endif
