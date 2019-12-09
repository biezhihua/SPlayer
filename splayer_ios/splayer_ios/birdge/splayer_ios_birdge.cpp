#include "splayer_ios_birdge.hpp"

const char *TAG = "[MP][BIRDGE][Main]";

bool IOS_DEBUG = false;

void (^SwiftFunc)(void) = nullptr;

void CFuncTest(void) {
    SwiftFunc();
}

void _native_init() {
}

long _getRotate() {
    return 0;
}

long _getVideoWidth() {
    return 0;
}

long _getVideoHeight() {
    return 0;
}

bool _isPlaying() {
    return false;
}

long _getDuration() {
    return 0;
}

long _getCurrentPosition() {
    return 0;
}

void _create(void *) {
    if (IOS_DEBUG) {
        ALOGD(TAG,"[%s]", __func__);
    }
}

void _start() {

}

void _stop() {

}

void _pause() {

}

void _play() {

}

void _release() {

}

void _reset() {

}

void _seekTo(float timeMs) {

}

void _setVolume(float leftVolume, float rightVolume) {

}

void _setMute(bool mute) {

}

void _setRate(float rate) {

}

void _setPitch(float pitch) {

}

void _setLooping(bool looping) {

}

void _setDataSource(const char *path) {

}

bool _isLooping() {
    return false;
}

void _setDataSourceAndHeaders(const char *path, char *keys, void *values) {

}

void _setOptionS(long category, const char *type, const char *option) {

}

void _setOptionL(long category, const char *type, long option) {

}

void _setSurface() {

}
