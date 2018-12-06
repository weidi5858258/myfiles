cmake_minimum_required(VERSION 3.12)
project(mytest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS  -std=c++11)

include_directories(src)

add_executable(
        mytest
        main.cpp
        src/my_struct.h
        src/my_head.h
        src/my_define.h
        src/http.hpp
        src/utils.hpp
)

target_link_libraries(
        mytest
        -lssl
        -lcrypto
        -lboost_system
        -lboost_filesystem
        -lboost_thread
)
