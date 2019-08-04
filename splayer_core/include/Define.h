#ifndef SPLAYER_MAC_DEFINE_H
#define SPLAYER_MAC_DEFINE_H

#define VERSION_MODULE_FILE_NAME_LENGTH  13

#define VIDEO_QUEUE_SIZE  3
#define AUDIO_QUEUE_SIZE  9
#define SUBTITLE_QUEUE_SIZE  16
#define FRAME_QUEUE_SIZE FFMAX(AUDIO_QUEUE_SIZE, FFMAX(VIDEO_QUEUE_SIZE, SUBTITLE_QUEUE_SIZE))

#endif //SPLAYER_MAC_DEFINE_H