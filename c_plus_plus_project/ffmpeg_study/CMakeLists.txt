cmake_minimum_required(VERSION 3.9)
project(ffmpeg_study)

#set(CMAKE_CXX_STANDARD 11)
#add_definitions(-std=c++11)

add_executable(
        ffmpeg_study
#        main.cpp

#        ffmpeg.cpp
#        include/config.h
#        include/cmdutils.h
#        include/MyHead.h
#        include/decode_audio.h

        src/business/ffplay/ffplay.h
        src/business/ffplay/ffplay_media.c
        src/business/ffplay/cmdutils.c

#        src/business/ffplay/ffplay.c
#        src/business/ffplay/ffplay_audio.c
#        src/alexander_ffplay.c
#        src/StudyFFmpeg.cpp
#        src/ffmpeg_sample.cpp
#        src/leixiaohua1020.cpp
#        src/player.cpp
#        src/music_player.cpp
#        src/music_player2.cpp
#        src/decode_audio.cpp
#        src/StudyAgain.cpp
        src/business/ffplay/FFplayer.h
        src/business/ffplay/FFplayer.cpp
)
# 要包含的头文件
include_directories(
        include
        src/business/ffplay
        /usr/local/include
        /usr/local/include/lame
        /usr/local/Cellar/sdl2/2.0.10/include/SDL2
        /Users/alexander/mydev/tools/audio_video/ffmpeg_macos/include
)
# 要包含的ffmpeg lib库
set(FFMPEG_LIB_PATH
        /Users/alexander/mydev/tools/audio_video/ffmpeg_macos/lib)
# 要包含的sdl2 lib库
set(SDL2_LIB_PATH
        /usr/local/Cellar/sdl2/2.0.10/lib)

target_link_libraries(
        ffmpeg_study

        -L${FFMPEG_LIB_PATH} -lavformat
        -L${FFMPEG_LIB_PATH} -lavcodec
        -L${FFMPEG_LIB_PATH} -lavfilter
        -L${FFMPEG_LIB_PATH} -lavutil
        -L${FFMPEG_LIB_PATH} -lavresample
        -L${FFMPEG_LIB_PATH} -lswresample
        -L${FFMPEG_LIB_PATH} -lswscale
        -L${FFMPEG_LIB_PATH} -lavdevice
        -L${FFMPEG_LIB_PATH} -lpostproc
        -L${SDL2_LIB_PATH} -lSDL2
        -L${SDL2_LIB_PATH} -lSDL2main

        -L/usr/local/Cellar/x264/r2917_1/lib -lx264
        -L/usr/local/Cellar/x265/3.3/lib -lx265
)
