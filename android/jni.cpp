#include <stdlib.h>
#include <stdio.h>

/***
用ndk-build命令编译时遇到的问题
Android NDK: WARNING: APP_PLATFORM android-17 is higher than android:minSdkVersion 1 in D:/workspace/jni/app/src/main/AndroidManifest.xml. NDK binaries will *not* be comptible with devices older than android-17. See https://android.googlesource.com/platform/ndk/+/master/docs/user/common_problems.md for more information.
解决方法：
在AndroidManifest.xml中指定<uses-sdk android:minSdkVersion="17"/>
最小版本

assert()
如果断言断的对,那么就不报错;
如果断言断的不对,那么就报错.

System.loadLibrary(“android_runtime”);
要求libandroid_runtime.so是在
/数据/数据​​/应用程序{}/lib目录，
System.load(“/系统/lib目录/libandroid_runtime.so”); 
要求的是完整路径.

*/







#define STACK_NUM 3
char DevName[DEV_NAME_MAX];
sprintf(DevName, "/dev/video0");
sprintf(DevName, "/dev/video%d", i);
int mCameraFd = open(DevName, O_RDWR | O_NONBLOCK);
int ret = close(mCameraFd);
mCameraFd = -1;
if (mCameraFd < 0) {
    LOGI("");
    return;
}
固定操作方式
struct v4l2_capability cap;
int ret = ioctl(mCameraFd, VIDIOC_QUERYCAP, &cap);
if (ret < 0) {
    LOGE("");
    return ret;
}
if (!(cap.capabilities & capabilities)) {
	LOGE("");
    return V4L2_ERROR;
}

bool checkError(int return_value, const char *func, int line) {
    if (return_value < 0) {
        LOGI("%s::%d fail. errno: %s", func, line, strerror(errno));
        return true;
    }
    return false;
}

static const int CAMERA_RESULT_SUCCESS = 0;

remove("/data/local/artifact/image.jpg");
指针需要初始化为NULL,不了安全
ImageInfo *mImageInfo[STACK_NUM];
构造中初始化
for (int i = 0; i < STACK_NUM; ++i) {
    mImageInfo[i] = NULL;
}

const char *path;
FILE *fp;
// 打开文件
if ((fp = fopen(path, "r")) == NULL) {
    LOGI("%s open failed.", path);
    return 0;
}
// 跳到文件末尾
fseek(fp, 0, SEEK_END);
// 得到文件长度
size_t length = ftell(fp);

定义一个enum
typedef enum _ssm_Result {
	SSM_RESULT_OK = 0,					/*!< OK. No problem */
	SSM_RESULT_ERROR,					/*!< NG. general error */
	SSM_RESULT_E_POINTER_NULL,			/*!< NG. null pointer */
	SSM_RESULT_E_INVALID_PARAMS,		/*!< NG. invalid parameter input */
	SSM_RESULT_E_MEM_ALLOC,				/*!< NG. failed allocating memory */
	SSM_RESULT_E_BUSY,					/*!< NG. busy */
	SSM_RESULT_E_THREAD,				/*!< NG. thread access failure */
	SSM_RESULT_E_INSTANCE,				/*!< NG. instance creation / access failure */
	SSM_RESULT_E_ABORT,					/*!< NG. operation aborted */
	SSM_RESULT_E_NOT_INITIALIZED,		/*!< NG. something not initialized */
	SSM_RESULT_OK_FILE_ACCESS,			/*!< OK. file access ended success */
	SSM_RESULT_E_FILE_ACCESS,			/*!< NG. file access ended failure */
	SSM_RESULT_E_UNEXPECTED,			/*!< NG. unexpected error */
	SSM_RESULT_E_NOTIMPL,				/*!< NG. function not implemented */
	SSM_RESULT_FILE_CREATED,			/*!< New File created ack */
	SSM_RESULT_CANCELED,				/*!< Operation canceled ack */
	SSM_RESULT_E_NODATA,				/*!< NG. No data found */
	SSM_RESULT_MAX						/* don't move this */
} ssm_Result;	
ssm_Result SBehaviorAnalyzer_ReleaseResultArray(SBA_Result *pResult, sbap_HANDLE p_handle);

static MyImageAnalyzer *mInstance;
static MyImageAnalyzer *getInstance() {
    return mInstance;
}

/*YUV_420_888 format*/
if ((imageLength * 2) / 3 != (width * height)) {
    LOGI("Image size is invalid");
    return false;
}

/*YUYV format*/
if ((imageLength / 2) != (width * height)) {
    LOGI("Image size is invalid");
    return false;
}

if (mGrayData != NULL) {
    free(mGrayData);
    mGrayData = NULL;
}
mGrayData = (unsigned char *) malloc(width * height);
memset(mGrayData, 0, width * height);

open阶段:
只是打开设备,然后检查设备打开是否成功,
再对这个设备的文件描述符作判断是否允许
这个设备操作有关功能.


信号量sem_t的使用:
#include<semaphore.h>
1.
定义变量
sem_t mController;
2.
初始化
sem_init(&mController, 0, 1);
3.
使用
第一种方式
sem_wait(&mController);
// dosomething
sem_post(&mController);
第二种方式
if (sem_trywait(&mController) != 0) {
    if (errno == EAGAIN) {
        LOGI("mController is busy.");
        return RESULT_OK;
    } else {
        LOGI("mController try wait failed.");
        return 0x00030005;
    }
}
// dosomething
sem_post(&mController);
4.
回收资源
sem_destroy(&mController);

java            jni
--------------------------------
Object          jobject
int             jint
long            jlong
int[]           jintArray
--------------------------------



/***
在jni中
JNIEnv *env, jobject thiz
这两个参数是系统给的,在java端不需要传递.
jobject obj的解释:
如果native方法不是static的话，这个obj就代表这个native方法的类实例
如果native方法  是static的话，这个obj就代表这个native方法的类的
class对象实例(static方法不需要类实例的，所以就代表这个类的class对象)
*/

status_t
uint32_t
uid_t
Parcel
sp <VolumeShaper::Configuration>
BnInterface<IPlayer>

// jing -> uint32_t
jint channelMask;
uint32_t localChanMask = (audio_channel_mask_t)channelMask;


// jobject  需要判NULL
// jintArray需要判NULL
jintArray jSession;
if (jSession == NULL) {
}
// jintArray -> jint *
jint *nSession = (jint *) env->GetPrimitiveArrayCritical(jSession, NULL);
if (nSession == NULL) {
}
// audio_session_t is enum
audio_session_t sessionId = (audio_session_t) nSession[0];
env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
nSession = NULL;

AudioRecord::AudioRecord(const String16 &opPackageName){...}
new AudioRecord(String16(opPackageNameStr.c_str()));

// 表示AudioTrack类
android::sp<android::AudioTrack>

/***
下面以android.media.AudioRecord为例
来说明libandroid_runtime.so
call到libmedia.so(名字随着android版本的不同而可能不同)
的过程(主要是得到另一个so库里的类的对象的过程).
libandroid_runtime.so中的代码:
首先宏定义
#define JAVA_POSTEVENT_CALLBACK_NAME                  "postEventFromNative"
#define JAVA_NATIVERECORDERINJAVAOBJ_FIELD_NAME  "mNativeRecorderInJavaObj"
#define JAVA_NATIVECALLBACKINFO_FIELD_NAME          "mNativeCallbackCookie"
#define JAVA_NATIVEDEVICECALLBACK_FIELD_NAME        "mNativeDeviceCallback"
"postEventFromNative"是java代码中定义的类的属性.
再定义一个结构体
struct audio_record_fields_t {
    jmethodID postNativeEventInJava;
    jfieldID  nativeRecorderInJavaObj;
    jfieldID  nativeCallbackCookie;
    jfieldID  nativeDeviceCallback;
};
定义静态变量
static audio_record_fields_t javaAudioRecordFields;
然后把上面的代码联系起来
// 初始化
javaAudioRecordFields.postNativeEventInJava = NULL;
javaAudioRecordFields.nativeRecorderInJavaObj = NULL;
javaAudioRecordFields.nativeCallbackCookie = NULL;
javaAudioRecordFields.nativeDeviceCallback = NULL;
static const char *const kClassPathName = "android/media/AudioRecord";
jclass audioRecordClass = env->FindClass(kClassPathName);
javaAudioRecordFields.postNativeEventInJava = GetStaticMethodIDOrDie(env,
            audioRecordClass, 
            JAVA_POSTEVENT_CALLBACK_NAME,
            "(Ljava/lang/Object;IIILjava/lang/Object;)V");
javaAudioRecordFields.nativeRecorderInJavaObj = GetFieldIDOrDie(env,
            audioRecordClass, 
            JAVA_NATIVERECORDERINJAVAOBJ_FIELD_NAME, 
            "J");
javaAudioRecordFields.nativeCallbackCookie = GetFieldIDOrDie(env,
            audioRecordClass, 
            JAVA_NATIVECALLBACKINFO_FIELD_NAME, 
            "J");
javaAudioRecordFields.nativeDeviceCallback = GetFieldIDOrDie(env,
            audioRecordClass, 
            JAVA_NATIVEDEVICECALLBACK_FIELD_NAME, 
            "J");
// 在libandroid_runtime.so中得到libmedia.so中的AudioRecord对象
#include <ScopedUtfChars.h>
// 包名
jstring opPackageName;
ScopedUtfChars opPackageNameStr(env, opPackageName);
sp<AudioRecord> lpRecorder = NULL;
lpRecorder = new AudioRecord(String16(opPackageNameStr.c_str()));
setAudioRecord(env, thiz, lpRecorder);
// 下面两个方法好像是固定写法
static sp<AudioRecord> setAudioRecord(JNIEnv *env, jobject thiz, const sp<AudioRecord> &ar) {
    Mutex::Autolock l(sLock);
    sp<AudioRecord> old =
            (AudioRecord *) env->GetLongField(thiz, javaAudioRecordFields.nativeRecorderInJavaObj);
    if (ar.get()) {
        ar->incStrong((void *) setAudioRecord);
    }
    if (old != 0) {
        old->decStrong((void *) setAudioRecord);
    }
    env->SetLongField(thiz, javaAudioRecordFields.nativeRecorderInJavaObj, (jlong) ar.get());
    return old;
}
static sp<AudioRecord> getAudioRecord(JNIEnv *env, jobject thiz) {
    Mutex::Autolock l(sLock);
    AudioRecord *const ar =
            (AudioRecord *) env->GetLongField(thiz, javaAudioRecordFields.nativeRecorderInJavaObj);
    return sp<AudioRecord>(ar);
}

可以参考的代码
enum {
    AUDIO_JAVA_SUCCESS            = 0,
    AUDIO_JAVA_ERROR              = -1,
    AUDIO_JAVA_BAD_VALUE          = -2,
    AUDIO_JAVA_INVALID_OPERATION  = -3,
    AUDIO_JAVA_PERMISSION_DENIED  = -4,
    AUDIO_JAVA_NO_INIT            = -5,
    AUDIO_JAVA_DEAD_OBJECT        = -6,
    AUDIO_JAVA_WOULD_BLOCK        = -7,
};
static inline jint nativeToJavaStatus(status_t status) {
    switch (status) {
        case NO_ERROR:
            return AUDIO_JAVA_SUCCESS;
        case BAD_VALUE:
            return AUDIO_JAVA_BAD_VALUE;
        case INVALID_OPERATION:
            return AUDIO_JAVA_INVALID_OPERATION;
        case PERMISSION_DENIED:
            return AUDIO_JAVA_PERMISSION_DENIED;
        case NO_INIT:
            return AUDIO_JAVA_NO_INIT;
        case WOULD_BLOCK:
            return AUDIO_JAVA_WOULD_BLOCK;
        case DEAD_OBJECT:
            return AUDIO_JAVA_DEAD_OBJECT;
        default:
            return AUDIO_JAVA_ERROR;
    }
}
底层一般的方法返回status_t,
从底层返回给java层一般用jini.



*/


struct fields_t {
    jfieldID context;

    jmethodID cryptoInfoSetID;
};
static fields_t gFields;
// FindClass
jclass clazz = env->FindClass("android/media/MediaExtractor");
CHECK(clazz != NULL);
// GetFieldID
gFields.context = env->GetFieldID(clazz, "mNativeContext", "J");
CHECK(gFields.context != NULL); 
clazz = env->FindClass("android/media/MediaCodec$CryptoInfo");
CHECK(clazz != NULL);
// GetMethodID
gFields.cryptoInfoSetID = env->GetMethodID(clazz, "set", "(I[I[I[B[BI)V");

// 使用"jobject thiz"这个参数得到java端的Class对象
jclass clazz = env->GetObjectClass(thiz);
CHECK(clazz != NULL);
jclass mClass = (jclass) env->NewGlobalRef(clazz);
jweak mObject = env->NewWeakGlobalRef(thiz);
// 析构
env->DeleteWeakGlobalRef(mObject);
mObject = NULL;
env->DeleteGlobalRef(mClass);
mClass = NULL;
// 创建一个强引用对象
sp<JMediaExtractor> extractor = new JMediaExtractor(env, thiz);
if (extractor != NULL) {
    extractor->incStrong(thiz);
}

/***
# our headers include libnativewindow's public headers
LOCAL_EXPORT_SHARED_LIBRARY_HEADERS := \
    libnativewindow \

mmm -B frameworks/base/core/jni
LOCAL_MODULE:= libandroid_runtime
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/camera
LOCAL_MODULE:= libcamera_client
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/cmds/screenrecord
LOCAL_MODULE:= screenrecord
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/cmds/stagefright
LOCAL_MODULE:= stagefright record recordvideo audioloop
               stream codec mediafilter muxer
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/drm/common
LOCAL_MODULE:= libdrmframeworkcommon
include $(BUILD_STATIC_LIBRARY)

mmm -B frameworks/av/drm/drmserver
LOCAL_MODULE:= drmserver
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/drm/libdrmframework
LOCAL_MODULE:= libdrmframework
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/drm/libmediadrm
LOCAL_MODULE:= libmediadrm
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/audioserver
LOCAL_MODULE := audioserver
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/media/common_time
LOCAL_MODULE := libcommon_time_client
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/img_utils
LOCAL_MODULE := libimg_utils
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libaaudio
LOCAL_MODULE := libaaudio
include $(BUILD_STATIC_LIBRARY)
LOCAL_MODULE := libaaudio
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libaudiohal
LOCAL_MODULE := libaudiohal
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libaudioprocessing
LOCAL_MODULE := libaudioprocessing
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libaudioprocessing/audio-resampler
LOCAL_MODULE := libaudio-resampler
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libcpustats
LOCAL_MODULE := libcpustats
include $(BUILD_STATIC_LIBRARY)

mmm -B frameworks/av/media/libmedia
LOCAL_MODULE:= libmedia
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libmediametrics
LOCAL_MODULE:= libmediametrics
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libmediaplayerservice
LOCAL_MODULE:= libmediaplayerservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/libmediaplayerservice/nuplayer
LOCAL_MODULE:= libstagefright_nuplayer
include $(BUILD_STATIC_LIBRARY)

mmm -B frameworks/av/media/libstagefright
LOCAL_MODULE:= libstagefright
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/mediaserver
LOCAL_MODULE:= mediaserver
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/media/mtp
LOCAL_MODULE:= libmtp
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/ndk
LOCAL_MODULE:= libmediandk
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/media/utils
frameworks/av/media/utils/Android.bp
cc_library_shared {
    name: "libmediautils",
    srcs: [
        "BatteryNotifier.cpp",
        "ISchedulingPolicyService.cpp",
        "MemoryLeakTrackUtil.cpp",
        "ProcessInfo.cpp",
        "SchedulingPolicyService.cpp",
    ],
    shared_libs: [
        "libbinder",
        "libcutils",
        "liblog",
        "libutils",
        "libmemunreachable",
    ],
    cflags: [
        "-Wall",
        "-Wextra",
        "-Werror",
    ],
    local_include_dirs: ["include"],
    export_include_dirs: ["include"],
}

mmm -B frameworks/av/radio
LOCAL_MODULE:= libradio
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/audioflinger
LOCAL_MODULE := libserviceutility
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE:= libaudioflinger
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/audiopolicy
LOCAL_MODULE:= libaudiopolicyservice
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE:= libaudiopolicymanagerdefault
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE:= libaudiopolicymanager
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/camera/libcameraservice
LOCAL_MODULE:= libcameraservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/mediaanalytics
LOCAL_MODULE:= mediametrics
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/services/mediacodec
LOCAL_MODULE:= libmediacodecservice
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE := android.hardware.media.omx@1.0-service
include $(BUILD_EXECUTABLE)
LOCAL_MODULE := mediacodec.policy
include $(BUILD_PREBUILT)

mmm -B frameworks/av/services/mediadrm
LOCAL_MODULE:= mediadrmserver
include $(BUILD_EXECUTABLE)

mmm -B frameworks/av/services/mediaextractor
LOCAL_MODULE:= libmediaextractorservice
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE:= mediaextractor
include $(BUILD_EXECUTABLE)
LOCAL_MODULE := mediaextractor.policy
include $(BUILD_PREBUILT)

mmm -B frameworks/av/services/medialog
LOCAL_MODULE:= libmedialogservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/mediaresourcemanager
LOCAL_MODULE:= libresourcemanagerservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/minijail
LOCAL_MODULE := libavservices_minijail
include $(BUILD_SHARED_LIBRARY)
LOCAL_MODULE := libavservices_minijail_unittest
include $(BUILD_NATIVE_TEST)

mmm -B frameworks/av/services/oboeservice
LOCAL_MODULE := libaaudioservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/radio
LOCAL_MODULE:= libradioservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/services/soundtrigger
LOCAL_MODULE:= libsoundtriggerservice
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/soundtrigger
LOCAL_MODULE:= libsoundtrigger
include $(BUILD_SHARED_LIBRARY)

mmm -B frameworks/av/tools/resampler_tools
LOCAL_MODULE := fir
include $(BUILD_HOST_EXECUTABLE)

////////////////////////////////////////////////////////////////

Android.mk语法:

#$(call my-dir)调用NDK内部的函数获得当前.mk文件的路径
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_REQUIRED_MODULES_arm := mediacodec.policy

#指定头文件的目录
LOCAL_C_INCLUDES := \
    $(TOP)/frameworks/av/media/libstagefright \
    $(TOP)/frameworks/native/include/media/openmax \
    $(LOCAL_PATH)/../services/include\
    $(LOCAL_PATH)/../cmds/include

#源文件(不包括头文件)
LOCAL_SRC_FILES := \
    MediaCodecService.cpp
LOCAL_SRC_FILES := \
    $(call all-subdir-java-files)
LOCAL_SRC_FILES := \
    $(call all-proto-files-under, src)
LOCAL_SRC_FILES := \
    $(call all-java-files-under, src/main/java)

LOCAL_JAVA_LIBRARIES := \
    core-oj \
    core-libart \
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
LOCAL_STATIC_LIBRARIES := \
    libgrallocusage \
    libgif \
    libseccomp_policy \
    libselinux \
    libcrypto \
    libgrallocusage \
    libinput \
    libutils \
    libcutils \
    liblog \

#libs/libABC.a
LOCAL_LDLIBS += \
    -ldl \
    -lpthread
    -L$(LOCAL_PATH)/libs/ -lABC \

LOCAL_JNI_SHARED_LIBRARIES := \
    libmemoryintarraytest \
    libcutils \
    libc++ \
    libpmtest64 \

LOCAL_MODULE := app_process
LOCAL_MODULE := libmediacodecservice
LOCAL_MODULE := android.hardware.media.omx@1.0-service
LOCAL_MODULE := liblog
LOCAL_REQUIRED_MODULES := libRS libRSDriver
LOCAL_WHOLE_STATIC_LIBRARIES := liblog
#指定编译之后放置的位置
#一般情况下不需要指定,当使用include $(BUILD_PREBUILT)时需要指定
#标识了所编译模块最后放置的位置,如果不指定,不会放到系统中,之后放在最后的obj目录下的对应目录中
#表示放在system/etc目录下
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_CLASS := APP
#表示放在/system/lib目录下
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
#表示放在/system/bin目录下
LOCAL_MODULE_CLASS := EXECUTABLES
#标识在什么情况下去编译该模块
LOCAL_MODULE_TAGS := user
#在eng模式下编译该模块
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE_TAGS := debug
LOCAL_MODULE_TAGS := tests
#在所有模式下都编译该模块
LOCAL_MODULE_TAGS := optional
#$(TARGET_ROOT_OUT)根文件系统路径
#out/target/product/generic/root
#$(TARGET_OUT)system文件系统路径
#out/target/product/generic/system
#$(TARGET_OUT_DATA)data文件系统路径
#out/target/product/generic/data
#指定最后生成的模块的目标地址,默认是$(TARGET_OUT)
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc/seccomp_policy
LOCAL_MODULE_PATH_64 := $(TARGET_OUT)/etc/seccomp_policy
#指定动态库的相对路径,以各自编译对象放置的目录为参考
#表示把编译出的产物放到/system/lib/hw(而非/system/lib)中
LOCAL_MODULE_RELATIVE_PATH := hw
#对于Android系统应用，LOCAL_PRIVILEGED_MODULE决定了其编译后的在ROM中的安装位置：
#如果不设置或者设置为false，安装位置为system/app；
#如果设置为true，安装位置为system/priv-app。
LOCAL_PROPRIETARY_MODULE := true
LOCAL_32_BIT_ONLY := true
LOCAL_CLANG := true
LOCAL_CFLAGS := \
    -D_SPIRV_LLVM_API \
    -DFAKE_LOG_DEVICE=1 \
    -std=c99 \
    -std=c++11 \
    -Werror \
    -Wall \
    -Wunused \
    -Weverything \
    -Wno-undef \
    -Wno-padded \
    -Wno-switch-enum \
    -fstrict-aliasing \
    -Wunreachable-code \
    -Wno-mismatched-tags \
    -Wno-unused-parameter \
    -fvisibility=hidden $(liblog_cflags) \
    -Wno-error=deprecated-declarations \
    -Wno-error=pessimizing-move \
    -Wno-error=unused-variable \
    -Wno-error=unused-private-field \
    -Wno-error=unused-function \
    -Wno-error=dangling-else \
    -Wno-error=ignored-qualifiers \
    -Wno-error=non-virtual-dtor
LOCAL_CPPFLAGS := \
    -std=c++1y \
    -Wno-c++98-compat-pedantic \
    -Wno-c99-extensions \
    -Wno-old-style-cast
LOCAL_LDFLAGS_arm := -Wl,--hash-style=both

LOCAL_REQUIRED_MODULES_arm := mediaextractor.policy
LOCAL_REQUIRED_MODULES_arm64 := mediaextractor.policy
LOCAL_REQUIRED_MODULES_x86 := mediaextractor.policy

LOCAL_MULTILIB := 32
LOCAL_MULTILIB := 64
LOCAL_MULTILIB := both
LOCAL_MULTILIB := first

LOCAL_MODULE_STEM_32 := app_process32
LOCAL_MODULE_STEM_64 := app_process64

LOCAL_SDK_VERSION := 17
LOCAL_SDK_VERSION := current

LOCAL_PACKAGE_NAME := PMTest_Java64

LOCAL_PROGUARD_ENABLED := disabled

LOCAL_MANIFEST_FILE := dual/AndroidManifest.xml

LOCAL_SANITIZE := never

LOCAL_CXX_STL := none

LOCAL_MODULE_HOST_OS := darwin linux windows

LOCAL_LDLIBS_linux := -lrt

LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_INIT_RC := mediametrics.rc
LOCAL_INIT_RC := android.hardware.media.omx@1.0-service.rc

include $(BUILD_JAVA_LIBRARY)
include $(BUILD_STATIC_JAVA_LIBRARY)
#编译出静态库
include $(BUILD_STATIC_LIBRARY)
#编译出动态库
include $(BUILD_SHARED_LIBRARY)
#编译出Native C可执行程序
include $(BUILD_EXECUTABLE)
#该模块已经预先编译
include $(BUILD_PREBUILT)
include $(BUILD_PACKAGE)
include $(BUILD_HOST_STATIC_LIBRARY)
include $(BUILD_HOST_SHARED_LIBRARY)
include $(BUILD_HOST_EXECUTABLE)

#编译当前目录下的Android.mk文件
include $(call all-makefiles-under, $(LOCAL_PATH))
#编译libvulkan nulldrv tools这三个文件夹下面的Android.mk文件
include $(call all-named-subdir-makefiles, libvulkan nulldrv tools)
*/

////////////////////////////////////////////////////////////////





/***
android 第一次启动时，会读取apk的信息， 
保存到/data/system/packages.xml, 
从此以后每次启动就通过读这个xml来判断走32bit加载还是64bit加载；
如果不想重烧image，就只能手动的"mkdir /system/app/lib/arm/"目录， 
然后删除/data/system/packages.xml再重启.
*/

/***
底层进程间通信方式:
1.
先定义一个Bp和Bn公同的基类
#include <binder/IInterface.h>
class IPowerManager : public IInterface {...}
2.
//PowerManager不能随便定义,接口是IPowerManager,那么定义时去掉I,为PowerManager
DECLARE_META_INTERFACE(PowerManager)
这个PowerManager需要跟Bp的实现类中的
IMPLEMENT_META_INTERFACE(PowerManager, "android.os.IPowerManager");
这个对应起来
3.
在基类中定义枚举,用于操作不同的动作
enum {
    ACQUIRE_WAKE_LOCK = IBinder::FIRST_CALL_TRANSACTION
};
4.
在基类中定义各种需要用到的纯虚函数
virtual status_t crash(const String16 &message) = 0;
5.
实现Bp类
#include <utils/Log.h>
#include <utils/Errors.h>
#include <binder/Parcel.h>//必须要用到的
class BpPowerManager : public BpInterface<IPowerManager> {...}
5-1.
//固定写法
IMPLEMENT_META_INTERFACE(PowerManager, "android.os.IPowerManager");
5-2.
//构造函数需要这么写(固定写法)
explicit BpPowerManager(const sp<IBinder> &impl)
                : BpInterface<IPowerManager>(impl) {
}
virtual ~BpPowerManager();
5-3.
实现基类中定义的纯虚函数

Bp(客户端)一般过程是这样的:
Parcel data, reply;
data.writeInterfaceToken(IPowerManager::getInterfaceDescriptor());
IPowerManager是基类,因此使用IPowerManager::getInterfaceDescriptor().
上面也是固定写法.

然后把要传递的数据写到data中去.
//int flags, size_t len
data.writeInt32(flags);
data.writeInt32(len);
//player_type_t playerType(player_type_t是enum)
data.writeInt32((int32_t) playerType);
//int64_t event_time_ms
data.writeInt64(event_time_ms);
//bool wait
data.writeInt32(wait);
//float vol
data.writeFloat(vol);
//const sp<IBinder> &lock
data.writeStrongBinder(lock);
//const String16 &tag
data.writeString16(tag);
//const char *path
data.writeInt32(strlen(path));
data.writeCString(path);
//int len, const int *uids
data.writeInt32Array(len, uids);
//const android::sp<ICmrdCallback> &callback
data.writeStrongBinder(IInterface::asBinder(callback));

return remote()->transact(REBOOT, data, &reply, 0);
status_t status = remote()->transact(IDaemon::OPEN, data, &reply);
if (status != OK) {
    return RESULT_ERR_BINDER_ERROR;
}
int ret = reply.readInt32();
return ret;


Bn(服务端)一般过程是这样的:
status_t BnPowerManager::onTransact(uint32_t code,
    const Parcel &data, Parcel *reply, uint32_t flags) {...}
//固定写法
CHECK_INTERFACE(IPowerManager, data, reply);

int width = data.readInt32();
const char *path = NULL;
path = data.readCString();
sp<ICmrdCallback> callback =
    interface_cast<ICmrdCallback>(data.readStrongBinder());

reply->writeInt32(ret);

default:
    return BBinder::onTransact(code, data, reply, flags);

6.
在其他地方要能够通过Bp端发送消息给Bn端,那么接下去的工作首先就是
怎样调用到Bp端的代码.
6-1.
首先有三种方法(只需选一种就行了)可以选择做第一步.
1)
在IPowerManager基类的头文件中需要有这样的代码.
static const char *getServiceName() {
    return SERVER_NAME;
}
2)
在BnPowerManager的头文件中需要有这样的代码.
static const char *getServiceName() {
    return SERVER_NAME;
}
3)在BnPowerManager的子类的头文件中需要有这样的代码.
static const char *getServiceName() {
    return SERVER_NAME;
}
6-2.
取得Bp端的对象调用其方法.
sp<IServiceManager> serviceManager = defaultServiceManager();
//此处的SERVER_NAME必须跟上面三种方法中返回的字符串一样
sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME));
sp<IDaemon> daemon = interface_cast<IDaemon>(binder);
//然后就可以使用daemon对象调用Bp端定义的方法了
daemon->open(1);

7.
要想Bp端能够连接到Bn端,那么需要把Bn端先启动起来.
7-1.
首先需要定义好一个启动类.
class Daemon :
		//固定写法,必须要这样继承
        public BinderService<Daemon>,
        public BnDaemon {
	//固定写法(好像不写也能正常工作,不过还是写了吧)
    friend class BinderService<Daemon>;

}
7-2.
然后在main函数中启动起来.
sp<ProcessState> proc(ProcessState::self());
sp<IServiceManager> sm = defaultServiceManager();

//addService inside in BinderService
//先执行BnDaemon的构造方法
//再执行Daemon的构造方法
Daemon::instantiate();

ProcessState::self()->startThreadPool();
IPCThreadState::self()->joinThreadPool();

8.
编译的时候Bp,Bn端的代码都要加入进去.
LOCAL_SRC_FILES:= \
    main.cpp \
    ../part_a/BpDaemon.cpp \
    ../part_b/BnDaemon.cpp \
    ../part_a/BnCallback.cpp \
    ../part_b/BpCallback.cpp \

通过上面的步骤就能够在其他地方先调用到Bp的代码,然后通过Bp中的代码调用
到Bn中的代码.这样就实现了跨进程调用了.
*/





/***
Android 7.1.1源码下载
1、下载安装repo 工具
mkdir ~/bin
PATH=~/bin:$PATH
curl https://storage-googleapis.proxy.ustclug.org/git-repo-downloads/repo > ~/bin/repo
chmod a+x ~/bin/repo
2、建立一个放android源码的目录，名字随意
mkdir android_sourcecode
cd android_sourcecode
3、初始化repo库
repo init -u git://mirrors.ustc.edu.cn/aosp/platform/manifest
4、如果需要下载某个特定的android版本，只需通过 －b指定版本号就行
repo init -u git://mirrors.ustc.edu.cn/aosp/platform/manifest -b android-7.1.1_r22
5、repo库初始化后，就可以直接下载指定版本的android源码，也可以通过指定下载路径，来下载源码中某一个模块
#下载repo库里所有的android源码
repo sync 
＃指定下载路径，下载某个模块代码
repo sync packages/providers/ContactsProvider
repo sync命令后接的path，在执行repo init目录下（此处即android _sourcecode）有个.repo文件夹，可以通过
.repo/manifest.xml 查看各个模块的路径，然后通过repo sycn path来下载学习最新的android源码，查看googel大神们写的代码
如果以后android源码更新了，只需改变步骤4中－b后指定的路径，Android所有版本列表

============================================
PLATFORM_VERSION_CODENAME=REL
PLATFORM_VERSION=8.1.0
TARGET_PRODUCT=aosp_arm
TARGET_BUILD_VARIANT=eng
TARGET_BUILD_TYPE=release
TARGET_PLATFORM_VERSION=OPM1
TARGET_BUILD_APPS=
TARGET_ARCH=arm
TARGET_ARCH_VARIANT=armv7-a
TARGET_CPU_VARIANT=generic
TARGET_2ND_ARCH=
TARGET_2ND_ARCH_VARIANT=
TARGET_2ND_CPU_VARIANT=
HOST_ARCH=x86_64
HOST_2ND_ARCH=x86
HOST_OS=linux
HOST_OS_EXTRA=Linux-4.4.0-17758-Microsoft-x86_64-with-Ubuntu-16.04-xenial
HOST_CROSS_OS=windows
HOST_CROSS_ARCH=x86
HOST_CROSS_2ND_ARCH=x86_64
HOST_BUILD_TYPE=release
BUILD_ID=OPM6.171019.030.E1
OUT_DIR=out
AUX_OS_VARIANT_LIST=
============================================
root@WEIDI5858258:/mnt/d/android_source/android_n# make -j4
21:21:50 ************************************************************
21:21:50 You are building on a case-insensitive filesystem.
21:21:50 Please move your source tree to a case-sensitive filesystem.
21:21:50 ************************************************************
21:21:50 Case-insensitive filesystems not supported
*/























