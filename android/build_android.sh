#!/usr/bin/env bash 

# 有以下几种情况
# arm64-v8a
# armeabi
# armeabi-v7a
# x86
# x86_64

# 输出日志
# set -x
# 目标Android版本
API=21
FFMPEG_ANDROID="/Users/alexander/mydev/tools/audio_video/ffmpeg_android"
export TMPDIR="${FFMPEG_ANDROID}/temp_dir"
ANDROID_SDK="/Users/alexander/mydev/tools/android_sdk"
ANDROID_NDK="${ANDROID_SDK}/android-ndk-r20"
# 编译工具链路径
TOOLCHAIN="$ANDROID_NDK/toolchains/llvm/prebuilt/darwin-x86_64"
# 编译环境
SYSROOT="${TOOLCHAIN}/sysroot"

echo ${ANDROID_SDK}
echo ${ANDROID_NDK}
echo ${TOOLCHAIN}
echo ${SYSROOT}
echo

function build_android
{
make clean
make clean
make clean

PREFIX="${FFMPEG_ANDROID}/${1}"
echo ${PREFIX}
echo ${2}
echo

# --target-os=linux
# --arch=arm \
# --cpu=${2}
./configure \
--prefix=$PREFIX \
--target-os=android \
--arch=arm \
--cc=clang --host-cflags= --host-ldflags= \
--enable-shared \
--disable-static \
--enable-ffmpeg \
--enable-asm \
--enable-neon \
--enable-cross-compile \
--enable-pthreads \
--enable-version3 \
--enable-avresample \
--enable-gnutls \
--enable-gpl \
--enable-libaom \
--enable-libbluray \
--enable-libmp3lame \
--enable-libopus \
--enable-librubberband \
--enable-libsnappy \
--enable-libtesseract \
--enable-libtheora \
--enable-libvidstab \
--enable-libvorbis \
--enable-libvpx \
--enable-libwebp \
--enable-libx264 \
--enable-libx265 \
--enable-libxvid \
--enable-lzma \
--enable-libfontconfig \
--enable-libfreetype \
--enable-frei0r \
--enable-libass \
--enable-libopencore-amrnb \
--enable-libopencore-amrwb \
--enable-libopenjpeg \
--enable-libspeex \
--enable-libsoxr \
--enable-videotoolbox \
--disable-libjack \
--disable-indev=jack \
--disable-doc \
--disable-ffplay \
--disable-ffprobe \
--disable-symver \
--sysroot=${SYSROOT} \
--cross-prefix=${TOOLCHAIN}/bin/arm-linux-androideabi- \
--cross-prefix-clang=${TOOLCHAIN}/bin/armv7a-linux-androideabi$API- \
--extra-cflags="-fPIC"
# --extra-ldflags="-L/usr/local/lib" \
# --extra-cflags="-I/usr/local/include"

make clean
make clean
make clean
make clean all
make
make install
}

# start run
build_android "armeabi-v7a" "armv7-a"
# build_android "arm64-v8a" "aarch64"
# build_android "armeabi" "arm"
# build_android "x86" "x86"
# build_android "x86_64" "x86_64"
