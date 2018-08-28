cmake_minimum_required(VERSION 3.9)
project(tcp_ip_client)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

add_executable(tcp_ip_client
        main.cpp
        LinuxSocket.cpp
        LinuxSocket.h)

set(FFMPEG_LIB_PATH
        /root/mydev/tools/ffmpeg/lib)

set(SDL2_LIB_PATH
        /usr/include/SDL2)

target_link_libraries(tcp_ip_client
        -L${FFMPEG_LIB_PATH} -lavformat
        -L${FFMPEG_LIB_PATH} -lavdevice
        -L${FFMPEG_LIB_PATH} -lavcodec
        -L${FFMPEG_LIB_PATH} -lavfilter
        -L${FFMPEG_LIB_PATH} -lavutil
        -L${FFMPEG_LIB_PATH} -lswresample
        -L${FFMPEG_LIB_PATH} -lswscale
        -L${FFMPEG_LIB_PATH} -lpostproc
        -L${SDL2_LIB_PATH} -lSDL2
        -L${SDL2_LIB_PATH} -lSDL2main
        -lmp3lame
        -lx264
        #-lx265
        -llzma
        -lasound
        -lsndio
        -lXext
        -lX11
        -lxcb
        -lsqlite3
        -lz -lrt -lm -lpthread
        -ldl
        #opengl
        -lglut -lGL -lGLU
        #录音
        -lasound
        -lfdk-aac
        -lcrypto
        -lssl
        -lvpx
        )

include_directories(
        include
        /root/mydev/tools/ffmpeg/include
        /usr/include/SDL2
        /usr/local/include/lame
)