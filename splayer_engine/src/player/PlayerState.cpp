#include <player/PlayerState.h>

PlayerState::PlayerState() {
    init();
    reset();
}

PlayerState::~PlayerState() {
    reset();
}

void PlayerState::init() {
    swsOpts = (AVDictionary *) malloc(sizeof(AVDictionary));
    memset(swsOpts, 0, sizeof(AVDictionary));
    swrOpts = (AVDictionary *) malloc(sizeof(AVDictionary));
    memset(swrOpts, 0, sizeof(AVDictionary));
    formatOpts = (AVDictionary *) malloc(sizeof(AVDictionary));
    memset(formatOpts, 0, sizeof(AVDictionary));
    codecOpts = (AVDictionary *) malloc(sizeof(AVDictionary));
    memset(codecOpts, 0, sizeof(AVDictionary));

    inputFormat = nullptr;
    url = nullptr;
    headers = nullptr;
    videoTitle = nullptr;

    audioCodecName = nullptr;
    videoCodecName = nullptr;
}

void PlayerState::reset() {

    if (swsOpts) {
        av_dict_free(&swsOpts);
        av_dict_set(&swsOpts, "flags", "bicubic", 0);
    }

    if (swrOpts) {
        av_dict_free(&swrOpts);
    }

    if (formatOpts) {
        av_dict_free(&formatOpts);
    }

    if (codecOpts) {
        av_dict_free(&codecOpts);
    }

    // 文件路径
    if (url) {
        av_freep(&url);
        url = nullptr;
    }

    offset = 0;

    abortRequest = 1;

    pauseRequest = 1;

    seekByBytes = 0;

    syncType = AV_SYNC_AUDIO;

    startTime = AV_NOPTS_VALUE;

    duration = AV_NOPTS_VALUE;

    durationSec = AV_NOPTS_VALUE;

    realTime = 0;

    infiniteBuffer = -1;

    audioDisable = 0;

    videoDisable = 0;

    displayDisable = 0;

    fast = 0;

    generateMissingPts = 0;

    lowResolution = 0;

    playbackRate = 1.0;

    playbackPitch = 1.0;

    seekRequest = 0;

    seekFlags = 0;

    seekPos = 0;

    seekRel = 0;

    autoExit = 0;

    loopTimes = 1;

    audioMute = 0;

    dropFrameWhenSlow = 1;

    decoderReorderPts = -1;

    eof = 0;

    attachmentRequest = 0;

    inputFormat = nullptr;

    formatContext = nullptr;

    videoTitle = nullptr;

    headers = nullptr;

    videoIndex = -1;

    audioIndex = -1;
}

void PlayerState::setOption(int category, const char *type, const char *option) {
    switch (category) {
        case OPT_CATEGORY_FORMAT: {
            av_dict_set(&formatOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_CODEC: {
            av_dict_set(&codecOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_SWS: {
            av_dict_set(&swsOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_PLAYER: {
            parse_string(type, option);
            break;
        }

        case OPT_CATEGORY_SWR: {
            av_dict_set(&swrOpts, type, option, 0);
            break;
        }
    }
}

void PlayerState::setOptionLong(int category, const char *type, int64_t option) {
    switch (category) {
        case OPT_CATEGORY_FORMAT: {
            av_dict_set_int(&formatOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_CODEC: {
            av_dict_set_int(&codecOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_SWS: {
            av_dict_set_int(&swsOpts, type, option, 0);
            break;
        }

        case OPT_CATEGORY_PLAYER: {
            parse_int(type, option);
            break;
        }

        case OPT_CATEGORY_SWR: {
            av_dict_set_int(&swrOpts, type, option, 0);
            break;
        }
    }
}

void PlayerState::parse_string(const char *type, const char *option) {
    if (!strcmp("acodec", type)) { // 指定音频解码器名称
        audioCodecName = av_strdup(option);
    } else if (!strcmp("vcodec", type)) {   // 指定视频解码器名称
        videoCodecName = av_strdup(option);
    } else if (!strcmp("sync", type)) { // 制定同步类型
        if (!strcmp("audio", option)) {
            syncType = AV_SYNC_AUDIO;
        } else if (!strcmp("video", option)) {
            syncType = AV_SYNC_VIDEO;
        } else if (!strcmp("ext", option)) {
            syncType = AV_SYNC_EXTERNAL;
        } else {    // 其他则使用默认的音频同步
            syncType = AV_SYNC_AUDIO;
        }
    } else if (!strcmp("f", type)) { // f 指定输入文件格式
        inputFormat = av_find_input_format(option);
        if (!inputFormat) {
            av_log(nullptr, AV_LOG_FATAL, "Unknown input format: %s\n", option);
        }
    }
}

void PlayerState::parse_int(const char *type, int64_t option) {
    if (!strcmp("an", type)) { // 禁用音频
        audioDisable = (option != 0) ? 1 : 0;
    } else if (!strcmp("vn", type)) { // 禁用视频
        videoDisable = (option != 0) ? 1 : 0;
    } else if (!strcmp("bytes", type)) { // 以字节方式定位
        seekByBytes = (option > 0) ? 1 : ((option < 0) ? -1 : 0);
    } else if (!strcmp("nodisp", type)) { // 不显示
        displayDisable = (option != 0) ? 1 : 0;
    } else if (!strcmp("fast", type)) { // fast标志
        fast = (option != 0) ? 1 : 0;
    } else if (!strcmp("generateMissingPts", type)) { // genpts标志
        generateMissingPts = (option != 0) ? 1 : 0;
    } else if (!strcmp("lowResolution", type)) { // lowres标准字
        lowResolution = (option != 0) ? 1 : 0;
    } else if (!strcmp("drp", type)) { // 重排pts
        decoderReorderPts = (option != 0) ? 1 : 0;
    } else if (!strcmp("autoexit", type)) { // 自动退出标志
        autoExit = (option != 0) ? 1 : 0;
    } else if (!strcmp("framedrop", type)) { // 丢帧标志
        dropFrameWhenSlow = (option != 0) ? 1 : 0;
    } else if (!strcmp("infbuf", type)) { // 无限缓冲区标志
        infiniteBuffer = (option > 0) ? 1 : ((option < 0) ? -1 : 0);
    } else {
//        ALOGE("unknown option - '%s'", type);
    }
}

const char *PlayerState::getSyncType() {
    if (syncType == AV_SYNC_AUDIO) {
        return "AV_SYNC_AUDIO";
    }
    return "NONE";
}

void PlayerState::setFormatContext(AVFormatContext *formatContext) {
    PlayerState::formatContext = formatContext;
}
