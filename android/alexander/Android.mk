#$(call my-dir)调用NDK内部的函数获得当前.mk文件的路径
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#指定头文件的目录
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/src/include \
    audio_utils \
    private/media \
    include/media \
    media \
#     $(TOP)/frameworks/av/media/libstagefright \
#     $(TOP)/frameworks/native/include/media/openmax \
#     $(LOCAL_PATH)/../services/include\
#     $(LOCAL_PATH)/../cmds/include

#源文件(不包括头文件)
#在Android P上编译时，src，src/main/java目录必须要有
LOCAL_SRC_FILES := \
	src/cpp/main.cpp \
    $(call all-subdir-java-files) \
    $(call all-proto-files-under, src/cpp) \
#     $(call all-java-files-under, src/main/java)
# LOCAL_SRC_FILES +:= \
#     $(call all-subdir-java-files)

# LOCAL_JAVA_LIBRARIES := \
#     core-oj \
#     core-libart \

#引用动态库
#     libskia \
LOCAL_SHARED_LIBRARIES := \
    libandroid_runtime \
    libnativehelper \
    libutils \
    libbinder \
    libmedia \
    libmediametrics \
    libmediadrm \
    libmidi \
    liblog \
    libcutils \
    libstagefright \
    libstagefright_foundation \
    libmtp \
    libexif \
    libpiex \
    libmemtrack \
    libandroidfw \
    libappfuse \
    libbase \
    libdebuggerd_client \
    libui \
    libgui \
    libsensor \
    libinput \
    libcamera_client \
    libcamera_metadata \
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
    libaudiohal \
    libaudioutils \
    libnbaio \
    libnblog \
    libsonic \

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
#     -lpthread \
#     -L$(LOCAL_PATH)/libs/ -lABC \

LOCAL_JNI_SHARED_LIBRARIES := \
    libmemoryintarraytest \
    libcutils \
    libc++ \
    libpmtest64 \

#位置很重要，移动上面就编译不成功
LOCAL_LDLIBS := \
    -ldl \
    -llog \
    -landroid \
    -ljnigraphics \

#error: 'LOG_TAG' macro redefined [-Werror,-Wmacro-redefined]
# LOCAL_CFLAGS := -Werror -Wall

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_EXECUTABLES)
LOCAL_MODULE := alexander
LOCAL_MODULE_TAGS := optional

# include $(BUILD_JAVA_LIBRARY)
# include $(BUILD_STATIC_JAVA_LIBRARY)
#编译出静态库
# include $(BUILD_STATIC_LIBRARY)
#编译出动态库
# include $(BUILD_SHARED_LIBRARY)
#编译出Native C可执行程序
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
