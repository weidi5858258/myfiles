//
// Created by ex-wangliwei on 2016/2/14.
//

#include "ffmpeg.h"
#include "MediaPlayer.h"

// 这个是自定义的LOG的标识
#define LOG "player_alexander"

int use_mode = USE_MODE_MEDIA;

void createAudioTrack(int sampleRateInHz,
                      int channelCount,
                      int audioFormat) {

}

void write(unsigned char *pcmData,
           int offsetInBytes,
           int sizeInBytes) {

}

void close() {

}

void audioSleep(long ms) {

}

void videoSleep(long ms) {

}

// 回调java端FFMPEG类中的有关方法
void onReady() {

}

void onChangeWindow(int width, int height) {

}

void onPlayed() {

}

void onPaused() {

}

void onFinished() {

}

void onProgressUpdated(long seconds) {

}

void onError(int error, char *errorInfo) {

}

void onInfo(char *info) {

}