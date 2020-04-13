//
// Created by root on 19-8-8.
//

#ifndef USEFRAGMENTS_MEDIAPLAYER_H
#define USEFRAGMENTS_MEDIAPLAYER_H

#include "MediaWrapper.hpp"
#include "ffmpeg.h"

namespace alexander_media {

    void *readData(void *opaque);

    void *handleData(void *opaque);

    void initAV();

    void initAudio();

    void initVideo();

    int openAndFindAVFormatContext();

    int findStreamIndex();

    int findAndOpenAVCodecForAudio();

    int findAndOpenAVCodecForVideo();

    int createSwrContent();

    int createSwsContext();

    void closeAudio();

    void closeVideo();

    int initPlayer();

    // void setJniParameters(JNIEnv *env, const char *filePath, jobject surfaceJavaObject);

    int play();

    int pause();

    int stop();

    int release();

    bool isRunning();

    bool isPlaying();

    int seekTo(int64_t timestamp);

    long getDuration();

    void stepAdd(int64_t addStep);

    void stepSubtract(int64_t subtractStep);

    void startPlayer();
}

#endif //USEFRAGMENTS_MEDIAPLAYER_H
