#!/bin/bash

SOURCE=`pwd`
ANDROID_NDK="/root/mydev/tools/android_sdk/ndk-bundle-15"

PREFIX=$SOURCE/android/x264
SYSROOT=$ANDROID_NDK/platforms/android-26/arch-arm
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

#EXTRA_CFLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=neon -D__ARM_ARCH_7__ -D__ARM_ARCH_7A__"
#EXTRA_LDFLAGS="-nostdlib"

./configure \
    --prefix=$PREFIX \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --enable-pic \
    --enable-shared \
    --enable-strip \
    --disable-cli \
    --disable-asm \
    --host=arm-linux \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $ADDI_CFLAGS $EXTRA_CFLAGS" \
    --extra-ldflags="$ADDI_LDFLAGS $EXTRA_LDFLAGS" \
    # --enable-static \
    # $ADDITIONAL_CONFIGURE_FLAG

# --host=arm-linux-androideabi \

make clean
make STRIP= -j4 install

# 找到x264 repo 的根目录下的 configure 文件，
# 找到 echo "SONAME=libx264.so.$API" >> config.mak 
# 改为 echo "SONAME=libx264-$API.so" >> config.mak