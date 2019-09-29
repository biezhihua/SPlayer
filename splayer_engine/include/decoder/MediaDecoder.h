#ifndef MEDIADECODER_H
#define MEDIADECODER_H

#include <common/Thread.h>
#include <common/Log.h>
#include <player/PlayerState.h>
#include <queue/PacketQueue.h>
#include <queue/FrameQueue.h>

class MediaDecoder : public Runnable {
    const char *const TAG = "MediaDecoder";

public:
    MediaDecoder(AVCodecContext *codecContext, AVStream *stream, int streamIndex, PlayerState *playerState,
                 AVPacket *flushPacket, Condition *readWaitCond);

    virtual ~MediaDecoder();

    virtual void start();

    virtual void stop();

    virtual void flush();

    int getStreamIndex();

    AVCodecContext *getCodecContext();

    AVStream *getStream();

    int getPacketQueueSize();

    int getPacketQueueMemorySize();

    int pushPacket(AVPacket *pkt);

    int hasEnoughPackets();

    void pushFlushPacket();

    void pushNullPacket();

    bool isSamePacketSerial();

    PacketQueue *getPacketQueue();

    virtual void run();

    virtual bool isFinished();

protected:

    /// 播放器状态
    PlayerState *playerState = nullptr;

    /// 数据包队列
    PacketQueue *packetQueue = nullptr;

    /// 解码上下文
    AVCodecContext *codecContext = nullptr;

    /// 码流
    AVStream *stream = nullptr;

    int streamIndex;

    /// 是否已结束
    int finished;

    /// 刷新包
    AVPacket *flushPacket = nullptr;

    /// 流读取通知
    Condition *readWaitCond = nullptr;

    /// 发送解码失败，延迟处理数据包
    bool isPendingPacket;

    /// 包数据
    AVPacket pendingPacket;

    /// 开始的间戳
    int64_t startPts;

    /// 开始的额外参数
    AVRational startPtsTb;

    /// 下一帧时间戳
    int64_t nextPts;

    /// 下一帧额外参数
    AVRational nextPtsTb;

public:
    void setStartPts(int64_t startPts);

    void setStartPtsTb(const AVRational &startPtsTb);
};


#endif //MEDIADECODER_H
