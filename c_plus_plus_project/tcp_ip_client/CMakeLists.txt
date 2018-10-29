cmake_minimum_required(VERSION 3.9)
project(tcp_ip_client)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=c++11)

add_executable(tcp_ip_client
        main.cpp
        LinuxSocket.cpp
        LinuxSocket.h TransferFile.cpp TransferFile.h)

target_link_libraries(tcp_ip_client
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
        )

include_directories(
        include
        /root/mydev/tools/ffmpeg/include
        /usr/include/SDL2
        /usr/local/include/lame
)