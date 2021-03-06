cmake_minimum_required(VERSION 3.9)
project(grammar)

set(CMAKE_CXX_STANDARD 11)
set(ANDROID_INCLUDE_PATH
        /root/mydev/songo/src/android/o-song)
set(ANDROID_KEYDL_HAL_HEADER_PATH
        ${ANDROID_INCLUDE_PATH}/out/soong/.intermediates/vendor/song/hardware/interfaces/eepserver/1.0/com.song.dtv.hardware.eepserver@1.0_genc++_headers/gen)
set(ANDROID_HIDL_HAL_HEADER_PATH
        ${ANDROID_INCLUDE_PATH}/out/soong/.intermediates/system/libhidl/transport/base/1.0/android.hidl.base@1.0_genc++_headers/gen)

include_directories(
        #        ${ANDROID_KEYDL_HAL_HEADER_PATH}/
        #        ${ANDROID_HIDL_HAL_HEADER_PATH}/
        #        ${ANDROID_INCLUDE_PATH}/external/curl/include
        #        ${ANDROID_INCLUDE_PATH}/external/expat/lib
        #        ${ANDROID_INCLUDE_PATH}/system/core/include
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
        #        ${ANDROID_INCLUDE_PATH}/system/core/toolbox/upstream-netbsd/include
        #        ${ANDROID_INCLUDE_PATH}/system/core/trusty/libtrusty/include
        #        ${ANDROID_INCLUDE_PATH}/system/extras/libfec/include
        #        ${ANDROID_INCLUDE_PATH}/system/extras/libpagemap/include
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
        #        include
        #        binder

        /usr/local/include
        /usr/usr/lib/gcc/x86_64-linux-gnu/5/include
)

#set(SOURCE_KEYDL_DAEMON_FILES
#        server/main.cpp
#        Binder/IKeyDLDaemon.cpp
#        server/KeyDLDaemon.cpp)
#set(SOURCE_HIDL_SERVER_FILES
#        hidl_server/service.cpp
#        hidl_server/KeyDL.cpp)
#set(SOURCE_JNI_FILES
#        JNI/RecogsitionJNI.cpp)

#add_library(libkeydl ${SOURCE_JNI_FILES})

add_executable(grammar
        main.cpp
        wd_grammar.cpp
        wd_grammar.h
        linuxhttp/LinuxSocket.cpp
        linuxhttp/LinuxSocket.h
        mylog/MyLog.cpp
        mylog/MyLog.h
        method/Method.cpp
        method/Method.h)

# 用到线程时需要的设置
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(grammar Threads::Threads)

############################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

#set(OPENGL_FILE E:/project/3rdparty/freeglut)
#INCLUDE_DIRECTORIES(${OPENGL_FILE}/Include)
#link_directories(${OPENGL_FILE}/lib D:/Program/MinGW/lib)
#set(SOURCE_FILES main.cpp)
#add_executable(ClionTest ${SOURCE_FILES})
#set(PROJECT_BINARY_DIR E:/project/ClionTest)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#target_link_libraries(ClionTest freeglut glu32 opengl32)