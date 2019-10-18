#ifndef ENGINE_AUDIO_DECODER_H
#define ENGINE_AUDIO_DECODER_H

#include <decoder/MediaDecoder.h>
#include <player/PlayerState.h>

class AudioDecoder : public MediaDecoder {
    const char *const TAG = "AudioDecoder";

public:
    AudioDecoder(AVFormatContext *formatCtx,
                 AVCodecContext *avctx,
                 AVStream *stream,
                 int streamIndex,
                 PlayerState *playerState,
                 AVPacket *flushPacket,
                 Condition *pCondition,
                 AVDictionary *opts,
                 MessageCenter *messageCenter);

    virtual ~AudioDecoder();

    int getAudioFrame(Frame **frame);

    void start() override;

    void stop() override;

    void flush() override;

    int getFrameSize();

    FrameQueue *getFrameQueue();

    void run() override;

    bool isFinished() override;

    int64_t getFrameQueueLastPos();

    int getAudioFrame(AVFrame *frame);

private:

    /// 解复用上下文
    AVFormatContext *formatContext;

    /// 帧队列
    FrameQueue *frameQueue;

    /// 解码线程
    Thread *decodeThread;

private:

    int decodeAudio();

    int decodeFrameFromPacketQueue(AVFrame *pFrame);

    int decodeFrame(AVFrame *frame);

};


#endif
