cmake_minimum_required(VERSION 3.12)
project(mytest)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(
        mytest
        main.cpp
        src/my_struct.h src/my_head.h src/my_define.h src/http.hpp src/utils.hpp)
