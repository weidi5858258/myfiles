#!/bin/bash

SOURCE=`pwd`
ANDROID_NDK=/root/mydev/tools/android_sdk/ndk-bundle-14

PREFIX=$SOURCE/android/ffmpeg
SYSROOT=$ANDROID_NDK/platforms/android-24/arch-arm
TOOLCHAIN=$ANDROID_NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
ADDI_CFLAGS="-marm"

OS=`uname`
HOST_ARCH=`uname -m`
export CCACHE=; type ccache >/dev/null 2>&1 && export CCACHE=ccache
if [ $OS == 'Linux' ]; then
    export HOST_SYSTEM=linux-$HOST_ARCH
elif [ $OS == 'Darwin' ]; then
    export HOST_SYSTEM=darwin-$HOST_ARCH
fi

# 加入x264编译库
EXTRA_DIR=$SOURCE/android/x264
EXTRA_CFLAGS="-I${EXTRA_DIR}/include"
EXTRA_LDFLAGS="-L${EXTRA_DIR}/lib"

function start_build {
	./configure \
	--prefix=$PREFIX \
	--enable-gpl \
	--enable-shared \
	--enable-asm \
	--enable-nonfree \
	--enable-libx264 \
	--enable-small \
	--enable-cross-compile \
	\
	--enable-filter=crop \
	--enable-filter=rotate \
	--enable-filter=scale \
	\
	--disable-encoders \
	--disable-decoders \
	--disable-static \
	--disable-doc \
	--disable-ffmpeg \
	--disable-ffplay \
	--disable-ffprobe \
	--disable-doc \
	--disable-symver \
	--disable-x86asm \
	\
	--enable-encoder=mpeg4 \
	--enable-encoder=aac \
	--enable-encoder=png \
	--enable-encoder=gif \
	--enable-encoder=libx264 \
	\
	--enable-decoder=mpeg4 \
	--enable-decoder=aac \
	--enable-decoder=gif \
	--enable-decoder=h264 \
	\
	--enable-parser=h264 \
	\
	--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
	--arch=arm \
	--target-os=linux \
	--sysroot=$SYSROOT \
	--extra-cflags="-Os -fpic $ADDI_CFLAGS $EXTRA_CFLAGS" \
	--extra-ldflags="$ADDI_LDFLAGS $EXTRA_LDFLAGS" \
	# CFLAGS=-I/usr/local/arm/2.95.3/arm-linux/include \
    # LDFLAGS=-L/usr/local/arm/2.95.3/arm-linux/lib \

	# $ADDITIONAL_CONFIGURE_FLAG
	# make clean && 
	# make && 
	# make install
}

start_build