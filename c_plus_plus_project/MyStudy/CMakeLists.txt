cmake_minimum_required(VERSION 3.12)
project(MyStudy)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++11)

add_executable(
        MyStudy
        main.cpp
        include/Test.h
        include/MyHead.h
        include/Method.h
        include/GuessNumber.h

        src/B0.cpp
        src/Test.cpp
        src/Pointer.cpp
        src/Study.cpp
        src/Method.cpp
        src/function_study.cpp
        src/GuessNumber.cpp
        src/platform.h)

if (WIN32)

    include_directories(
            include
            C:/mydev/tools/ffmpeg-dev/include
            C:/mydev/tools/SDL2-2.0.8/x86_64-w64-mingw32/include
            C:/mydev/tools/SDL2-2.0.8/i686-w64-mingw32/include
            C:/mydev/tools/lame3.100-64
            C:/mydev/tools/glutdlls37beta

            C:/MinGW/include
            C:/MinGW/mingw32/include
            C:/MinGW/msys/1.0/lib/gcc/i686-pc-msys/3.4.4/include
            C:/MinGW/msys/1.0/lib/gcc/i686-pc-msys/3.4.4/install-tools/include
            C:/MinGW/share/doc/gcc/include
            C:/MinGW/share/doc/gdb/include
            C:/cygwin64/usr/include
            D:/ubuntu/head_files/usr/local/include
            D:/ubuntu/head_files/usr/usr/lib/gcc/x86_64-linux-gnu/5/include

            "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/Linux/include"
    )

    set(FFMPEG_LIB_PATH
            C:/mydev/tools/ffmpeg-dev/lib)

    set(SDL2_LIB_PATH
            C:/mydev/tools/SDL2-2.0.8/x86_64-w64-mingw32/lib)

    set(GLUTDLLS37BETA_LIB_PATH
            C:/mydev/tools/glutdlls37beta)

    target_link_libraries(
            MyStudy
            -L${FFMPEG_LIB_PATH} -lavformat
            -L${FFMPEG_LIB_PATH} -lavcodec
            -L${FFMPEG_LIB_PATH} -lavdevice
            -L${FFMPEG_LIB_PATH} -lavfilter
            -L${FFMPEG_LIB_PATH} -lavutil
            -L${FFMPEG_LIB_PATH} -lswscale
            -L${FFMPEG_LIB_PATH} -lswresample
            -L${FFMPEG_LIB_PATH} -lpostproc
            -L${SDL2_LIB_PATH} -lSDL2
            -L${SDL2_LIB_PATH} -lSDL2main
            -lz -lm -lpthread
#            -L${GLUTDLLS37BETA_LIB_PATH} -lglut32
#            -L"C:/MinGW/lib" -lmingw32
    )

else ()
    #Ubuntu

    include_directories(
            include
            /root/mydev/tools/ffmpeg/include
            /usr/include/SDL2
            /usr/local/include/lame
    )

    set(FFMPEG_LIB_PATH
            /root/mydev/tools/ffmpeg/lib)

    set(SDL2_LIB_PATH
            /usr/include/SDL2)

    target_link_libraries(
            MyStudy
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

endif ()

################################################################

#add_library(libjpeg
#        libturbojpeg
#        STATIC
#        IMPORTED
#        )
#set_target_properties(libjpeg
#        libturbojpeg
#        PROPERTIES IMPORTED_LOCATION
#        /opt/libjpeg-turbo/lib32/libjpeg.a
#        /opt/libjpeg-turbo/lib32/libturbojpeg.a
#        )

#link_libraries(
#        /opt/libjpeg-turbo/lib32/libjpeg.a
#        /opt/libjpeg-turbo/lib32/libturbojpeg.a
#)

#link_directories(
#        /opt/libjpeg-turbo/lib32/libjpeg.a
#        /opt/libjpeg-turbo/lib32/libturbojpeg.a
#)

# 设置c++编译器
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(OpenCV_DIR /usr/local/share/OpenCV)

# 将所有的源文件列为一个集合，集合名字叫做SRC_LISTS
#set(SRC_LISTS
#        ./src/Test.cpp
#        )

# 将集合里的所有的源文件生成一个静态库，该静态库的名字libsugan，
# 注意，在整个CmakeLists里都要用libsugan这个名字来代替之前那个集合生成的库。
#add_library(lib_test
#        ${SRC_LISTS}
#        )

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing
# OpenCVConfig.cmake file via the command line or GUI
# 在整个电脑上找opencv包
#find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
#message(STATUS "OpenCV library status:")
# version: 3.3.0
#message(STATUS "    version: ${OpenCV_VERSION}")

# libraries:
# opencv_calib3d;opencv_core;opencv_dnn;
# opencv_features2d;opencv_flann;opencv_highgui;
# opencv_imgcodecs;opencv_imgproc;opencv_ml;
# opencv_objdetect;opencv_photo;opencv_shape;
# opencv_stitching;opencv_superres;opencv_video;
# opencv_videoio;opencv_videostab
#message(STATUS "    libraries: ${OpenCV_LIBS}")

# include path: /usr/local/include;/usr/local/include/opencv
#message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
# 包含头文件路径
#include_directories(${OpenCV_INCLUDE_DIRS}
#        ./include/
#        )

# Link your application with OpenCV libraries
# 链接静态库需要的依赖库
#target_link_libraries(lib_test
#        ${OpenCV_LIBS}
#        )

######################################################

#set(ANDROID_INCLUDE_PATH
#        /root/mydev/android)
#
#include_directories(${ANDROID_INCLUDE_PATH}/system
#        ${ANDROID_INCLUDE_PATH}/bionic/libc/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/osi/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libmemunreachable
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/frameworks/MediaPlayer/jni/os
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/frameworks/dlna/jni/os
#        ${ANDROID_INCLUDE_PATH}/external/sonivox/arm-wt-22k/include
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/hardware/wfd_client/localtestsrc
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/frameworks/cmpb/include
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/frameworks/mtkstream/include
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/public_inc/android/MtkResourceManager/client
#        ${ANDROID_INCLUDE_PATH}/frameworks/av/include/media
#        ${ANDROID_INCLUDE_PATH}/frameworks/av/media/libmedia/include/media
#        ${ANDROID_INCLUDE_PATH}/frameworks/wilhelm/src/android/
#        ${ANDROID_INCLUDE_PATH}/frameworks/wilhelm/src/autogen/
#        ${ANDROID_INCLUDE_PATH}/vendor/mediatek/open/public_inc/android/system/core/include/cutils
#
#        ${ANDROID_INCLUDE_PATH}/bootable/recovery/bootloader_message/include/
#        ${ANDROID_INCLUDE_PATH}/system/libhidl/base/include/
#        ${ANDROID_INCLUDE_PATH}/system/libhidl/transport/include/
#        ${ANDROID_INCLUDE_PATH}/bionic/libc/include
#        ${ANDROID_INCLUDE_PATH}/bionic/libc/kernel/uapi
#        ${ANDROID_INCLUDE_PATH}/external/avb/
#        ${ANDROID_INCLUDE_PATH}/external/boringssl/include
#        ${ANDROID_INCLUDE_PATH}/external/curl/include
#        ${ANDROID_INCLUDE_PATH}/external/expat/lib
#        ${ANDROID_INCLUDE_PATH}/external/selinux/libselinux/include/
#        ${ANDROID_INCLUDE_PATH}/system/core/include
#        ${ANDROID_INCLUDE_PATH}/frameworks/av/include
#        ${ANDROID_INCLUDE_PATH}/frameworks/av/media/libmedia/include
#        ${ANDROID_INCLUDE_PATH}/frameworks/av/media/libstagefright/include
#        ${ANDROID_INCLUDE_PATH}/frameworks/native/include
#        ${ANDROID_INCLUDE_PATH}/frameworks/base/include
#        ${ANDROID_INCLUDE_PATH}/libnativehelper/include/nativehelper
#        ${ANDROID_INCLUDE_PATH}/system/bt/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/bta/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/btcore/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/btif/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/device/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/embdrv/sbc/decoder/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/embdrv/sbc/encoder/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/hci/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/osi/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/profile/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/stack/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/udrv/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/utils/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/vendor_libs/test_vendor_lib/include
#        ${ANDROID_INCLUDE_PATH}/system/bt/vnd/include
#        ${ANDROID_INCLUDE_PATH}/system/core/include
#        ${ANDROID_INCLUDE_PATH}/system/core/adf/libadf/include
#        ${ANDROID_INCLUDE_PATH}/system/core/adf/libadfhwc/include
#        ${ANDROID_INCLUDE_PATH}/system/core/base/include
#        ${ANDROID_INCLUDE_PATH}/system/core/fs_mgr/include
#        ${ANDROID_INCLUDE_PATH}/system/core/healthd/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libcrypto_utils/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libion/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libmemunreachable/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libnativeloader/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libpackagelistparser/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libpixelflinger/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libprocessgroup/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libsparse/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libsuspend/include
#        ${ANDROID_INCLUDE_PATH}/system/core/libsync/include
#        ${ANDROID_INCLUDE_PATH}/system/core/logwrapper/include
#        ${ANDROID_INCLUDE_PATH}/system/core/metricsd/include
#        ${ANDROID_INCLUDE_PATH}/system/core/mkbootimg/
#        ${ANDROID_INCLUDE_PATH}/system/core/toolbox/upstream-netbsd/include
#        ${ANDROID_INCLUDE_PATH}/system/core/trusty/libtrusty/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/libfec/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/libpagemap/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/ext4_utils/include/
#        ${ANDROID_INCLUDE_PATH}/system/extras/perfprofd/quipper/kernel-headers/tools/perf/util/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/perfprofd/quipper/original-kernel-headers/tools/perf/util/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/simpleperf/nonlinux_support/include
#        ${ANDROID_INCLUDE_PATH}/system/extras/tests/include
#        ${ANDROID_INCLUDE_PATH}/system/gatekeeper/include
#        ${ANDROID_INCLUDE_PATH}/system/keymaster/include
#        ${ANDROID_INCLUDE_PATH}/system/media/alsa_utils/include
#        ${ANDROID_INCLUDE_PATH}/system/media/audio/include
#        ${ANDROID_INCLUDE_PATH}/system/media/audio_effects/include
#        ${ANDROID_INCLUDE_PATH}/system/media/audio_route/include
#        ${ANDROID_INCLUDE_PATH}/system/media/audio_utils/include
#        ${ANDROID_INCLUDE_PATH}/system/media/camera/include
#        ${ANDROID_INCLUDE_PATH}/system/media/private/camera/include
#        ${ANDROID_INCLUDE_PATH}/system/media/private/radio/include
#        ${ANDROID_INCLUDE_PATH}/system/media/radio/include
#        ${ANDROID_INCLUDE_PATH}/system/nativepower/include
#        ${ANDROID_INCLUDE_PATH}/system/netd/include
#        ${ANDROID_INCLUDE_PATH}/system/security/keystore/include
#        ${ANDROID_INCLUDE_PATH}/system/security/softkeymaster/include
#        ${ANDROID_INCLUDE_PATH}/system/update_engine/include
#        ${ANDROID_INCLUDE_PATH}/system/update_engine/client_library/include
#        )
