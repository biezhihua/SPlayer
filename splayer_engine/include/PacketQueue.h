#ifndef ENGINE_PACKETQUEUE_H
#define ENGINE_PACKETQUEUE_H

#include <queue>
#include "Mutex.h"
#include "Condition.h"
#include "Log.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

typedef struct PacketData {
    AVPacket pkt;
    struct PacketData *next;
    int serial;
} PacketData;

/**
 * 备注：这里不用std::queue是为了方便计算队列占用内存和队列的时长，在解码的时候要用到
 */
class PacketQueue {

    const char *const TAG = "[MP][NATIVE][PacketQueue]";

public:
    PacketQueue(AVPacket *flushPacket);

    virtual ~PacketQueue();

    // 入队数据包
    int pushPacket(AVPacket *pkt);

    // 入队空数据包
    int putNullPacket(int stream_index);

    // 刷新
    void flush();

    // 终止
    void abort();

    // 开始
    void start();

    // 获取数据包
    int getPacket(AVPacket *pkt);

    // 获取数据包
    int getPacket(AVPacket *pkt, int block);

    int getPacketSize();

    int getSize();

    int64_t getDuration();

    bool isAbort();

    int getLastSeekSerial();

    int *getPointLastSeekSerial();

    int getFirstSeekSerial();

    int signal();

private:
    int put(AVPacket *pkt);

private:

    Mutex mutex;

    Condition condition;

    PacketData *firstPacket;

    PacketData *lastPacket;

    /// 包数量，也就是队列元素数量
    int packetSize;

    /// 占用内存大小
    int memorySize;

    /// 持续时长
    int64_t duration;

    /// 用户退出请求标志
    bool abortRequest;

    /// 序列，seek时使用，向PacketQueue中存储Packet时，被更新为队尾的seekSerial
    int lastSeekSerial;

    /// 序列，seek时使用，从PacketQueue从获取Packet时，被更新为队首的seekSerial
    int firstSeekSerial = -1;

    AVPacket *flushPacket;
};


#endif
