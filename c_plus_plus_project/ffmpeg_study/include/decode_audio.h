//
// Created by root on 18-9-19.
//

#ifndef FFMPEG_STUDY_DECODE_AUDIO_H
#define FFMPEG_STUDY_DECODE_AUDIO_H

#include "MyHead.h"

class decode_audio {
public:
    void decode(AVCodecContext *dec_ctx, AVPacket *pkt,
                AVFrame *frame, FILE *outfile);

    int main_(int argc, char **argv);
};


#endif //FFMPEG_STUDY_DECODE_AUDIO_H
