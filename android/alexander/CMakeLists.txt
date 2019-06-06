cmake_minimum_required(VERSION 3.13)
project(alexander)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

add_executable(alexander
        src/include/MyHead.h
        src/cpp/main.cpp)

##################################################################

set(ANDROID_INCLUDE_PATH /root/mydev/android_source/hikey970/aosp)
#set(ANDROID_INCLUDE_PATH /media/root/alexander/android_source/aosp)

include_directories(
        ${ANDROID_INCLUDE_PATH}/bionic/libc/include

        ${ANDROID_INCLUDE_PATH}/external/curl/include
        ${ANDROID_INCLUDE_PATH}/external/expat/lib
        ${ANDROID_INCLUDE_PATH}/external/gtest/include
        ${ANDROID_INCLUDE_PATH}/external/libselinux/include
        ${ANDROID_INCLUDE_PATH}/external/libbrillo
        ${ANDROID_INCLUDE_PATH}/external/libchrome
        ${ANDROID_INCLUDE_PATH}/external/kernel-headers/original/uapi
        ${ANDROID_INCLUDE_PATH}/external/skia/include/core
        ${ANDROID_INCLUDE_PATH}/external/skia/src/core

        ${ANDROID_INCLUDE_PATH}/frameworks/av/include
        ${ANDROID_INCLUDE_PATH}/frameworks/base/core/jni
        ${ANDROID_INCLUDE_PATH}/frameworks/base/include
        ${ANDROID_INCLUDE_PATH}/frameworks/native/include

        ${ANDROID_INCLUDE_PATH}/hardware/libhardware/include
        
        ${ANDROID_INCLUDE_PATH}/libnativehelper/include
        ${ANDROID_INCLUDE_PATH}/libnativehelper/include/nativehelper
        ${ANDROID_INCLUDE_PATH}/libnativehelper/include_jni

        ${ANDROID_INCLUDE_PATH}/prebuilts/ndk/current/platforms/android-24/arch-arm/usr/include/asm-generic

        ${ANDROID_INCLUDE_PATH}/system/core/libcutils/include/cutils
        ${ANDROID_INCLUDE_PATH}/system/core/libutils/include/utils
        ${ANDROID_INCLUDE_PATH}/system/extras/perfprofd/quipper
        ${ANDROID_INCLUDE_PATH}/system/bt/include
        ${ANDROID_INCLUDE_PATH}/system/bt/bta/include
        ${ANDROID_INCLUDE_PATH}/system/bt/btcore/include
        ${ANDROID_INCLUDE_PATH}/system/bt/btif/include
        ${ANDROID_INCLUDE_PATH}/system/bt/device/include
        ${ANDROID_INCLUDE_PATH}/system/bt/embdrv/sbc/decoder/include
        ${ANDROID_INCLUDE_PATH}/system/bt/embdrv/sbc/encoder/include
        ${ANDROID_INCLUDE_PATH}/system/bt/hci/include
        ${ANDROID_INCLUDE_PATH}/system/bt/osi/include
        ${ANDROID_INCLUDE_PATH}/system/bt/profile/include
        ${ANDROID_INCLUDE_PATH}/system/bt/stack/include
        ${ANDROID_INCLUDE_PATH}/system/bt/udrv/include
        ${ANDROID_INCLUDE_PATH}/system/bt/utils/include
        ${ANDROID_INCLUDE_PATH}/system/bt/vendor_libs/test_vendor_lib/include
        ${ANDROID_INCLUDE_PATH}/system/bt/vnd/include
        ${ANDROID_INCLUDE_PATH}/system/core/include
        ${ANDROID_INCLUDE_PATH}/system/core/adf/libadf/include
        ${ANDROID_INCLUDE_PATH}/system/core/adf/libadfhwc/include
        ${ANDROID_INCLUDE_PATH}/system/core/base/include
        ${ANDROID_INCLUDE_PATH}/system/core/fs_mgr/include
        ${ANDROID_INCLUDE_PATH}/system/core/healthd/include
        ${ANDROID_INCLUDE_PATH}/system/core/libcrypto_utils/include
        ${ANDROID_INCLUDE_PATH}/system/core/libion/include
        ${ANDROID_INCLUDE_PATH}/system/core/libmemunreachable/include
        ${ANDROID_INCLUDE_PATH}/system/core/libnativeloader/include
        ${ANDROID_INCLUDE_PATH}/system/core/libpackagelistparser/include
        ${ANDROID_INCLUDE_PATH}/system/core/libpixelflinger/include
        ${ANDROID_INCLUDE_PATH}/system/core/libprocessgroup/include
        ${ANDROID_INCLUDE_PATH}/system/core/libsparse/include
        ${ANDROID_INCLUDE_PATH}/system/core/libsuspend/include
        ${ANDROID_INCLUDE_PATH}/system/core/libsync/include
        ${ANDROID_INCLUDE_PATH}/system/core/logwrapper/include
        ${ANDROID_INCLUDE_PATH}/system/core/metricsd/include
        ${ANDROID_INCLUDE_PATH}/system/core/toolbox/upstream-netbsd/include
        ${ANDROID_INCLUDE_PATH}/system/core/trusty/libtrusty/include
        ${ANDROID_INCLUDE_PATH}/system/extras/libfec/include
        ${ANDROID_INCLUDE_PATH}/system/extras/libpagemap/include
        ${ANDROID_INCLUDE_PATH}/system/extras/perfprofd/quipper/kernel-headers/tools/perf/util/include
        ${ANDROID_INCLUDE_PATH}/system/extras/perfprofd/quipper/original-kernel-headers/tools/perf/util/include
        ${ANDROID_INCLUDE_PATH}/system/extras/simpleperf/nonlinux_support/include
        ${ANDROID_INCLUDE_PATH}/system/extras/tests/include
        ${ANDROID_INCLUDE_PATH}/system/gatekeeper/include
        ${ANDROID_INCLUDE_PATH}/system/keymaster/include
        ${ANDROID_INCLUDE_PATH}/system/media/alsa_utils/include
        ${ANDROID_INCLUDE_PATH}/system/media/audio/include
        ${ANDROID_INCLUDE_PATH}/system/media/audio_effects/include
        ${ANDROID_INCLUDE_PATH}/system/media/audio_route/include
        ${ANDROID_INCLUDE_PATH}/system/media/audio_utils/include
        ${ANDROID_INCLUDE_PATH}/system/media/camera/include
        ${ANDROID_INCLUDE_PATH}/system/media/private/camera/include
        ${ANDROID_INCLUDE_PATH}/system/media/private/radio/include
        ${ANDROID_INCLUDE_PATH}/system/media/radio/include
        ${ANDROID_INCLUDE_PATH}/system/nativepower/include
        ${ANDROID_INCLUDE_PATH}/system/netd/include
        ${ANDROID_INCLUDE_PATH}/system/security/keystore/include
        ${ANDROID_INCLUDE_PATH}/system/security/softkeymaster/include
        ${ANDROID_INCLUDE_PATH}/system/update_engine/include
        ${ANDROID_INCLUDE_PATH}/system/update_engine/client_library/include
)