#$(call my-dir)调用NDK内部的函数获得当前.mk文件的路径
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#指定头文件的目录
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
#     $(TOP)/frameworks/av/media/libstagefright \
#     $(TOP)/frameworks/native/include/media/openmax \
#     $(LOCAL_PATH)/../services/include\
#     $(LOCAL_PATH)/../cmds/include

#源文件(不包括头文件)
LOCAL_SRC_FILES := \
	main.cpp \
    $(call all-subdir-java-files) \
    $(call all-proto-files-under, src) \
    $(call all-java-files-under, src/main/java)
# LOCAL_SRC_FILES +:= \
#     $(call all-subdir-java-files)

# LOCAL_JAVA_LIBRARIES := \
#     core-oj \
#     core-libart \

#引用动态库
LOCAL_SHARED_LIBRARIES := \
    libandroid_runtime \
    libnativehelper \
    libutils \
    libbinder \
    libmedia \
    libmediametrics \
    libmediadrm \
    libmidi \
    libskia \
    libui \
    liblog \
    libcutils \
    libgui \
    libstagefright \
    libstagefright_foundation \
    libcamera_client \
    libmtp \
    libexif \
    libpiex \
    libandroidfw \
    libmemtrack \
    libandroidfw \
    libappfuse \
    libbase \
    libnativehelper \
    liblog \
    libcutils \
    libdebuggerd_client \
    libutils \
    libbinder \
    libui \
    libgui \
    libsensor \
    libinput \
    libcamera_client \
    libcamera_metadata \
    libskia \
    libsqlite \
    libEGL \
    libGLESv1_CM \
    libGLESv2 \
    libvulkan \
    libziparchive \
    libETC1 \
    libhardware \
    libhardware_legacy \
    libselinux \
    libicuuc \
    libmedia \
    libaudioclient \
    libjpeg \
    libusbhost \
    libharfbuzz_ng \
    libz \
    libpdfium \
    libimg_utils \
    libnetd_client \
    libradio \
    libsoundtrigger \
    libminikin \
    libprocessgroup \
    libnativebridge \
    libradio_metadata \
    libnativeloader \
    libmemunreachable \
    libhidlbase \
    libhidltransport \
    libhwbinder \
    libvintf \
    libnativewindow \
    libhwui \
    libdl \

#引用静态库
# LOCAL_STATIC_LIBRARIES := \
#     libgrallocusage \
#     libgif \
#     libseccomp_policy \
#     libselinux \
#     libcrypto \
#     libgrallocusage \
#     libinput \
#     libutils \
#     libcutils \
#     liblog \

#libs/libABC.a
# LOCAL_LDLIBS := \
#     -ldl \
#     -lpthread \
#     -L$(LOCAL_PATH)/libs/ -lABC \

LOCAL_JNI_SHARED_LIBRARIES := \
    libmemoryintarraytest \
    libcutils \
    libc++ \
    libpmtest64 \

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_EXECUTABLES)
LOCAL_MODULE := alexander

# include $(BUILD_JAVA_LIBRARY)
# include $(BUILD_STATIC_JAVA_LIBRARY)
#编译出静态库
# include $(BUILD_STATIC_LIBRARY)
#编译出动态库
# include $(BUILD_SHARED_LIBRARY)
#编译出Native C可执行程序
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
