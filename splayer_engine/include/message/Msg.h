#ifndef SPLAYER_CORE_MESSAGE_H

class Msg {
public:
    Msg();

    ~Msg();

    int what = -1;
    int arg1 = -1;
    int arg2 = -1;
//    MessageObject *obj = nullptr;

    void free();

    /////////////////////////////////////////////
    /////////////////////////////////////////////
    ///  消息类型范围 0~9999

    /// 默认
    static const int MSG_FLUSH = 0;

    /// 出错回调
    static const int MSG_ERROR = 1;

    /// 准备完成回调
    static const int MSG_PREPARED = 2;

    /// 已经开始
    static const int MSG_STARTED = 3;

    /// 播放完成回调
    static const int MSG_COMPLETED = 4;

    /// 打开文件
    static const int MSG_OPEN_INPUT = 5;

    /// 查找媒体流信息
    static const int MSG_FIND_STREAM_INFO = 6;

    /// 准备解码器
    static const int MSG_PREPARE_DECODER = 7;

    /// 视频大小变化
    static const int MSG_VIDEO_SIZE_CHANGED = 8;

    /// 长宽比变化
    static const int MSG_SAR_CHANGED = 9;

    /// 开始音频解码
    static const int MSG_AUDIO_START = 10;

    /// 音频渲染开始(播放开始)
    static const int MSG_AUDIO_RENDERING_START = 11;

    /// 开始视频解码
    static const int MSG_VIDEO_START = 12;

    /// 视频渲染开始(渲染开始)
    static const int MSG_VIDEO_RENDERING_START = 13;

    /// 旋转角度变化
    static const int MSG_VIDEO_ROTATION_CHANGED = 14;

    /// 缓冲开始
    static const int MSG_BUFFERING_START = 15;

    /// 缓冲完成
    static const int MSG_BUFFERING_END = 16;

    /// 缓冲更新
    static const int MSG_BUFFERING_UPDATE = 17;

    /// 缓冲时间更新
    static const int MSG_BUFFERING_TIME_UPDATE = 18;

    /// 定位完成
    static const int MSG_SEEK_COMPLETE = 19;

    /// 播放状态变更
    static const int MSG_PLAYBACK_STATE_CHANGED = 20;

    /// 字幕
    static const int MSG_TIMED_TEXT = 21;


    /////////////////////////////////////////////
    /////////////////////////////////////////////
    ///  请求消息范围 20000 ~ 29999

    /// 异步请求准备
    static const int MSG_REQUEST_PREPARE = 20000;

    /// 异步请求开始
    static const int MSG_REQUEST_START = 20001;

    /// 请求暂停
    static const int MSG_REQUEST_PAUSE = 20002;

    /// 请求定位
    static const int MSG_REQUEST_SEEK = 20004;

    /// 请求退出
    static const int MSG_REQUEST_QUIT = 20005;

    /////////////////////////////////////////////
    /////////////////////////////////////////////
    ///  扩展消息范围 30000 ~ 39999

    /// 当前时钟
    static const int MSG_CURRENT_POSITON = 30000;

    static const char *getMsgSimpleName(int what);
};

#endif //SPLAYER_CORE_MESSAGE_H
