#ifndef SPLAYER_CORE_FRAME_H
#define SPLAYER_CORE_FRAME_H

extern "C" {
#include <libavutil/rational.h>
#include <libavutil/frame.h>
#include <libavformat/avformat.h>
};

/**
 * Common struct for handling all types of decoded data and allocated render buffers.
 */
class Frame {
public:
    AVFrame *frame;
    AVSubtitle subtitle;
    AVRational sampleAspectRatio;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
    int width;
    int height;
    int format;
    int uploaded; // 是否更新到Texture
    int flipVertical;
};


#endif //SPLAYER_CORE_FRAME_H
