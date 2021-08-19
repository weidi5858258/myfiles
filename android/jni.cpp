#include <stdlib.h>
#include <stdio.h>
#include <binder/IPCThreadState.h>
#include "core_jni_helpers.h"

/***
https://source.android.google.cn/setup/build/downloading

# 下面的库是android系统提供的,可以直接使用的
find_library(log-lib
        log)
find_library(android-lib
        android)
find_library(c-lib
        c)
find_library(m-lib
        m)
find_library(z-lib
        z)
find_library(compiler_rt-extras-lib
        compiler_rt-extras)
find_library(dl-lib
        dl)
find_library(EGL-lib
        EGL)
find_library(GLESv1_CM-lib
        GLESv1_CM)
find_library(GLESv2-lib
        GLESv2)
find_library(jnigraphics-lib
        jnigraphics)
find_library(OpenMAXAL-lib
        OpenMAXAL)
find_library(OpenSLES-lib
        OpenSLES)
find_library(stdc++-lib
        stdc++)

# 下面的库是区分sdk版本的
# android-18
find_library(GLESv3-lib
        GLESv3)
# android-21
find_library(mediandk-lib
        mediandk)
# android-24
find_library(camera2ndk-lib
        camera2ndk)
find_library(vulkan-lib
        vulkan)
# android-26
find_library(nativewindow-lib
        nativewindow)
find_library(sync-lib
        sync)
# android-27
find_library(aaudio-lib
        aaudio)
find_library(neuralnetworks-lib
        neuralnetworks)
# android-29
find_library(amidi-lib
        amidi)
find_library(binder_ndk-lib
        binder_ndk)

target_link_libraries(
        socket

        # ${log-lib}是log库的路径
        ${log-lib}
        ${android-lib}
        ${mediandk-lib}
        ${OpenMAXAL-lib}
)

 宏                 ABI版本
arm                armeabi
arm                armeabi-v7
aarch64            arm64-v8a
i386               x86
_x86_64            x86_64
#if defined(__arm64__) || defined(__aarch64__)
// todo
#endif


在jni层能用的类有:
#include <media/stagefright/foundation/AString.h>
AString(const char *name能直接传给参数const AString &name)
#include <utils/KeyedVector.h>
KeyedVector<AString, sp<Capabilities> > mCaps;


在Android底层打印日志时的输出格式有：
打印以0x开关的enum常量时使用%#x，不要使用#%x(可能是写错的)
%s   : String8(mOpPackageName).string()
%d   : enum(这样定义时AUDIO_SOURCE_DEFAULT = 0 1 2 3 4)
       bool
       int
%f   : float
%u   : uint32_t
%p   : void*
%#x  : enum(这样定义时AUDIO_FORMAT_PCM_SUB_16_BIT = 0x1u 0x2u 0x3u 0x4000001u 0x1e000001u)
       uint32_t
#%x  : enum(这样定义时AUDIO_OUTPUT_FLAG_NONE = 0x0 0x10 0x100 0x2000 0x10000)
%zu  : size_t(不能使用%d)
%zd  : ssize_t
0x%x : uint32_t
status_t
nsecs_t
pid_t
uid_t
int64_t
*/

/***
在Android底层中常用的代码有：
#define LOG_TAG "alexander"
pid_t pid_ = getpid();
uid_t uid_ = getuid();
ALOGD("AudioTrack::set() PID: %d UID: %d", pid_, uid_);
pid_t callingPid = IPCThreadState::self()->getCallingPid();
pid_t callingPid = IPCThreadState::self()->getCallingPid();
pid_t pid = getpid();
ALOGD("BpDaemon::BpDaemon()  created   %p PID: %d", this, pid);
pid_t pid = getpid();
ALOGD("BpDaemon::~BpDaemon() destroyed %p PID: %d", this, pid);

// 主要代码
sp<IServiceManager> sm = defaultServiceManager();
sp<IBinder> binder;
do {
    // 主要代码
    binder = sm->getService(String16("media.audio_flinger"));
    if (binder != 0) {
        break;
    }
    ALOGW("AudioFlinger not published, waiting...");
    usleep(500000); // 0.5 s
} while (true);
gAudioFlingerClient = new AudioFlingerClient();
binder->linkToDeath(gAudioFlingerClient);
// 主要代码
sp<IAudioFlinger> gAudioFlinger = interface_cast<IAudioFlinger>(binder);
ProcessState::self()->startThreadPool();

int64_t token = IPCThreadState::self()->clearCallingIdentity();
// ......
IPCThreadState::self()->restoreCallingIdentity(token);

void *user;
const audio_offload_info_t *offloadInfo;
const sp<IMemory> &sharedBuffer;
uint32_t(streamType)

char *av_strdup(const char *s)
{
    char *ptr = NULL;
    if (s) {
        size_t len = strlen(s) + 1;// 关键点
        ptr = av_realloc(NULL, len);
        if (ptr)
            memcpy(ptr, s, len);
    }
    return ptr;
}

char filename[1024];
av_strlcpy(filename, filename ? filename : "", sizeof(filename));

*/

/***
什么样的进程能够调用libandroid_runtime.so库

typedef union jvalue {
    jboolean    z;
    jbyte       b;
    jchar       c;
    jshort      s;
    jint        i;
    jlong       j;
    jfloat      f;
    jdouble     d;
    jobject     l;
} jvalue;

// 下面这些类型因为是指针,所以需要判NULL
typedef _jthrowable*    jthrowable;
typedef _jobject*       jobject;
typedef _jobject*       jweak;
typedef _jclass*        jclass;
typedef _jstring*       jstring;
typedef _jarray*        jarray;
typedef _jobjectArray*  jobjectArray;
typedef _jbooleanArray* jbooleanArray;
typedef _jbyteArray*    jbyteArray;
typedef _jcharArray*    jcharArray;
typedef _jshortArray*   jshortArray;
typedef _jintArray*     jintArray;
typedef _jlongArray*    jlongArray;
typedef _jfloatArray*   jfloatArray;
typedef _jdoubleArray*  jdoubleArray;

jintArray jSession;
if (jSession == NULL) {
    ALOGE("Error creating AudioRecord: invalid session ID pointer");
    return (jint) AUDIO_JAVA_ERROR;
}
jint *nSession = (jint *) env->GetPrimitiveArrayCritical(jSession, NULL);
if (nSession == NULL) {
    ALOGE("Error creating AudioRecord: Error retrieving session id pointer");
    return (jint) AUDIO_JAVA_ERROR;
}
audio_session_t sessionId = (audio_session_t) nSession[0];
ALOGI("android_media_AudioRecord_setup() sessionId: %d", sessionId);
env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
nSession = NULL;

jintArray jSampleRate;
if (jSampleRate == NULL) {
    ALOGE("Error creating AudioRecord: invalid sample rates");
    return (jint) AUDIO_JAVA_ERROR;
}
jint elements[1];
env->GetIntArrayRegion(jSampleRate, 0, 1, elements);
int sampleRateInHertz = elements[0];

jstring opPackageName;
ScopedUtfChars opPackageNameStr(env, opPackageName);
new AudioRecord(String16(opPackageNameStr.c_str()));

// 系统给的参数, native方法在java层的哪个类中,thiz就指向这个类
JNIEnv *env, jobject thiz;
jclass clazz = env->GetObjectClass(thiz);
if (clazz == NULL) {
    ALOGE("Can't find %s when setting up callback.", kClassPathName);
    return (jint) AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;
}
// audioRecord_class这个有什么用呢?
// 在native层调用java层的static方法时需要用到
jclass audioRecord_class = (jclass) env->NewGlobalRef(clazz);
env->DeleteGlobalRef(lpCookie->audioRecord_class);

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

frameworks/base/core/jni
frameworks/base/media/jni
frameworks/av/media/libmedia
frameworks/av/services/audioflinger

打印String16的字符串
const String16 &mOpPackageName
ALOGD("mOpPackageName: %s", String8(mOpPackageName).string());

// 定义类型的同时定义了一个变量
static struct audio_record_fields_t {
    jmethodID postNativeEventInJava;
    jfieldID nativeRecorderInJavaObj;
    jfieldID nativeCallbackCookie;
    jfieldID nativeDeviceCallback;
} javaAudioRecordFields;
static struct {
    jfieldID fieldFramePosition;
    jfieldID fieldNanoTime;
} javaAudioTimestampFields;

struct audiorecord_callback_cookie {
    jclass audioRecord_class;
    jobject audioRecord_ref;
    bool busy;
    Condition cond;
};
audiorecord_callback_cookie *lpCallbackData = NULL;
// new结构体对象时不要有括号
lpCallbackData = new audiorecord_callback_cookie;
delete lpCallbackData;

static Mutex sLock;
// 哪段代码需要lock就用大括号括起来
{
    Mutex::Autolock l(sLock);
    if (sAudioRecordCallBackCookies.indexOf(callbackInfo) < 0) {
        return;
    }
    callbackInfo->busy = true;
}

static jint
android_media_AudioRecord_setup(JNIEnv *env, jobject thiz,
                                jobject weak_this, jobject jaa, jintArray jSampleRate,
                                jint channelMask, jint channelIndexMask, jint audioFormat,
                                jint buffSizeInBytes, jintArray jSession, jstring opPackageName,
                                jlong nativeRecordInJavaObj)
JNIEnv *env和jobject thiz这两个参数是不能传的,系统会给的.


*/

/***
frameworks/av/media/libaudioclient
frameworks/av/media/libaudioclient/include/media/IAudioRecord.h
class IAudioRecord : public IInterface {
public:
    DECLARE_META_INTERFACE(AudioRecord);
    virtual status_t start(int  event, audio_session_t triggerSession) = 0;
    virtual void stop() = 0;
};
class BnAudioRecord : public BnInterface<IAudioRecord> {
public:
    virtual status_t onTransact(uint32_t code,
                                const Parcel &data,
                                Parcel *reply,
                                uint32_t flags = 0);
};
frameworks/av/media/libaudioclient/IAudioRecord.cpp
IMPLEMENT_META_INTERFACE(AudioRecord, "android.media.IAudioRecord");
class BpAudioRecord : public BpInterface<IAudioRecord> {
    explicit BpAudioRecord(const sp<IBinder> &impl)
            : BpInterface<IAudioRecord>(impl) {
    }
    virtual status_t start(int event, audio_session_t triggerSession) {
        
    }
    virtual void stop() {
        
    }
};







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
在native层得到java层的类,属性,方法
#include "core_jni_helpers.h"
jclass    FindClassOrDie(        JNIEnv* env,               const char* class_name)
jfieldID  GetFieldIDOrDie(       JNIEnv* env, jclass clazz, const char* field_name,  const char* field_signature)
jmethodID GetMethodIDOrDie(      JNIEnv* env, jclass clazz, const char* method_name, const char* method_signature)
jfieldID  GetStaticFieldIDOrDie( JNIEnv* env, jclass clazz, const char* field_name,  const char* field_signature)
jmethodID GetStaticMethodIDOrDie(JNIEnv* env, jclass clazz, const char* method_name, const char* method_signature)
T         MakeGlobalRefOrDie(    JNIEnv* env, T in)
int       RegisterMethodsOrDie(  JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods)
field_name和method_name是java层类的属性名称和方法名称.
难点是写field_signature和method_signature.

native层调用java层的静态方法
env->CallStaticVoidMethod(
                // 通过FindClassOrDie(...)得到
                callbackInfo->audioRecord_class,
                // 通过GetStaticMethodIDOrDie(...)得到
                javaAudioRecordFields.postNativeEventInJava,
                // 其他的参数跟java层调用时一样
                callbackInfo->audioRecord_ref, event, 0,0, NULL);

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
// 先对结构体成员进行初始化
javaAudioRecordFields.postNativeEventInJava = NULL;
javaAudioRecordFields.nativeRecorderInJavaObj = NULL;
javaAudioRecordFields.nativeCallbackCookie = NULL;
javaAudioRecordFields.nativeDeviceCallback = NULL;
static const char *const kClassPathName = "android/media/AudioRecord";
// 找到java类
jclass audioRecordClass = FindClassOrDie(env, kClassPathName);
jclass audioRecordClass = env->FindClass(kClassPathName);
// 实际是这样调用的
FindClassOrDie(...) -> env->FindClass(...)
javaAudioRecordFields.postNativeEventInJava = GetStaticMethodIDOrDie(env,
            audioRecordClass, 
            // java中的方法名
            JAVA_POSTEVENT_CALLBACK_NAME,
            // java中的方法参数签名
            "(Ljava/lang/Object;IIILjava/lang/Object;)V");
// 实际是这样调用的
GetStaticMethodIDOrDie(...) -> env->GetStaticMethodID(...)
javaAudioRecordFields.nativeRecorderInJavaObj = GetFieldIDOrDie(env,
            audioRecordClass, 
            // java中的属性名
            JAVA_NATIVERECORDERINJAVAOBJ_FIELD_NAME, 
            "J");
// 实际是这样调用的
GetFieldIDOrDie(...) -> env->GetFieldID(...)
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
    AUDIO_JAVA_SUCCESS            =  0,
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
在IPowerManager.h头文件中先定义一个Bp和Bn公同的基类,
Bp和Bn类的定义也可以放在这个头文件中
#include <binder/IInterface.h>
class IPowerManager : public IInterface {...}
class IAudioTrack   : public IInterface {...}
class BpAudioTrack  : public BpInterface<IAudioTrack> {...}
class BnAudioTrack  : public BnInterface<IAudioTrack> {...}
2.
//PowerManager不能随便定义,接口是IPowerManager,那么定义时去掉I,为PowerManager
DECLARE_META_INTERFACE(PowerManager)
这个PowerManager需要跟Bp的实现类中的这个
IMPLEMENT_META_INTERFACE(PowerManager, "android.os.IPowerManager");
对应起来
3.
在基类中定义枚举,用于操作不同的动作
enum {
    ACQUIRE_WAKE_LOCK = IBinder::FIRST_CALL_TRANSACTION
};
4.
在基类中定义各种需要用到的纯虚函数
virtual status_t crash(const String16 &message) = 0;
virtual sp<media::VolumeShaper::State> getVolumeShaperState(int id) = 0;
virtual media::VolumeShaper::Status applyVolumeShaper(
                const sp<media::VolumeShaper::Configuration> &configuration,
                const sp<media::VolumeShaper::Operation> &operation) = 0;
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
//固定写法
Parcel data, reply;
data.writeInterfaceToken(IPowerManager::getInterfaceDescriptor());
IPowerManager是基类,因此使用IPowerManager::getInterfaceDescriptor().

然后把要传递的数据写到data中去.
const android::sp<ICmrdCallback> &callback
//写
data.writeStrongBinder(IInterface::asBinder(callback));
//读
sp<IBinder> binder = reply.readStrongBinder();

const android::sp<IBinder> &lock
data.writeStrongBinder(lock);

player_type_t playerType(player_type_t是enum)
data.writeInt32((int32_t) playerType);

bool wait
data.writeInt32(wait);

int flags, size_t len
data.writeInt32(flags);
data.writeInt32(len);

int64_t event_time_ms
data.writeInt64(event_time_ms);

float vol
data.writeFloat(vol);

const String16 &tag
data.writeString16(tag);

const char *path
data.writeInt32(strlen(path));
data.writeCString(path);

int len, const int *uids
data.writeInt32Array(len, uids);

//把data数据发送给Bn端,操作Bn端相关的动作.然后就去看Bn的实现过程.
return remote()->transact(IDaemon::REBOOT, data, &reply, 0);

status_t status = remote()->transact(IDaemon::OPEN, data, &reply);
if (status != OK) {
    return RESULT_ERR_BINDER_ERROR;
}
//Bn端如果在返回时带了一个值过来,那么可以使用这样读出来
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
取得Bp端的对象调用其方法(得到Bp端对象都是使用的下面方法).
sp<IServiceManager> serviceManager = defaultServiceManager();
//此处的SERVER_NAME必须跟上面三种方法中返回的字符串一样
sp<IBinder> binder = serviceManager->getService(String16(SERVER_NAME));
sp<IDaemon> daemon = interface_cast<IDaemon>(binder);
//然后就可以使用daemon对象调用Bp端定义的方法了
daemon->open(1);

扩展:
有哪些服务(实际是Bp端对象指针)可以通过
serviceManager->getService(String16(SERVER_NAME));
得到?
sp<IAudioTrack>
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

audio流程:
/root/mydev/android_source/hikey970/aosp/frameworks/base/core/jni/android_media_AudioTrack.cpp
假如是进程A
/root/mydev/android_source/hikey970/aosp/frameworks/av/media/libaudioclient/AudioTrack.cpp
// 得到IAudioFlinger的Bp端(客户端)对象
const sp<IAudioFlinger> &audioFlinger = AudioSystem::get_audio_flinger();
if (audioFlinger == 0) {
    ALOGE("Could not get audioflinger");
    status = NO_INIT;
    goto exit;
}
// 由IAudioFlinger的Bp端(客户端)对象调用到IAudioFlinger的Bn端(假如是进程B)
// IAudioTrack的Bp端(客户端)对象
sp<IAudioTrack> mBpAudioTrack = audioFlinger->createTrack(input, output, &status);

进程B
/root/mydev/android_source/hikey970/aosp/frameworks/av/media/libaudioclient/IAudioFlinger.cpp(Bn端)
/root/mydev/android_source/hikey970/aosp/frameworks/av/services/audioflinger/AudioFlinger.cpp
class AudioFlinger : public BinderService<AudioFlinger>, public BnAudioFlinger {...}
IAudioFlinger的Bn端调用到子类AudioFlinger的createTrack(...)方法
在这个方法中,创建TrackHandle对象(class TrackHandle : public android::BnAudioTrack)然后返回该对象,
并使用IBinder传递给进程A.



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

/***
 Android C++开发
 .h文件只存放命名空间(如果需要的话),和跟类定义有关的头文件,其他东西一律不放在这里.
 .cpp文件存放类实现时需要用到的头文件.然后是需要用到的常量,如
 #define WAIT_STREAM_END_TIMEOUT_SEC 120
 static const int kMaxLoopCountNotifications = 32;

 NDK so库文件位置:
 /Users/alexander/mydev/tools/android_sdk/android-ndk-r16b/platforms/android-27/arch-arm/usr/lib

jclass,jmethodID,jfieldID变量都是可以初始化为NULL.

java端native方法参数 ---> 对应jni标识
Object ---> Ljava/lang/Object;
String ---> Ljava/lang/String;
FileDescriptor ---> Ljava/io/FileDescriptor;
int    ---> I
long ---> J
byte ---> B
boolean[] ---> [Z

java端native方法返回值 ---> 对应jni标识
void ---> V
boolean ---> Z

////////////////////////////////////////////注册jni方法(一般App开发中不需要这样子去做)
文件: android_media_JetPlayer.cpp
#include <jni.h>
#include <JNIHelp.h>
#include "core_jni_helpers.h"
// java
private native final boolean native_setup(Object Jet_this, int maxTracks, int trackBufferSize);
// jni
static jboolean android_media_JetPlayer_setup(JNIEnv *env, jobject thiz, jobject weak_this,
                                                jint maxTracks, jint trackBufferSize)
static const JNINativeMethod gMethods[] = {
        // name,         signature,                 funcPtr
        {"native_setup", "(Ljava/lang/Object;II)Z", (void *) android_media_JetPlayer_setup},
};
int register_android_media_JetPlayer(JNIEnv *env) {
    // RegisterMethodsOrDie在core_jni_helpers.h中定义(实际上是AndroidRuntime::registerNativeMethods(...))
    return RegisterMethodsOrDie(env, "android/media/JetPlayer", gMethods, NELEM(gMethods));
}
java端调用native_setup(...)方法时,就会调用到jni层的android_media_JetPlayer_setup方法

// 为java端的long属性(mNativePlayerInJavaObj)设置jni层的一个对象
// java端定义
private long mNativePlayerInJavaObj;
// jni端定义
static const char *const kClassPathName = "android/media/JetPlayer";
#define JAVA_NATIVEJETPLAYERINJAVAOBJ_FIELD_NAME "mNativePlayerInJavaObj"
jclass jetPlayerClass = FindClassOrDie(env, kClassPathName);
jclass jetClass = MakeGlobalRefOrDie(env, jetPlayerClass);
jfieldID nativePlayerInJavaObj = GetFieldIDOrDie(env, jetPlayerClass, JAVA_NATIVEJETPLAYERINJAVAOBJ_FIELD_NAME, "J");
JetPlayer *lpJet = new JetPlayer(env->NewGlobalRef(weak_this), maxTracks, trackBufferSize);
// set(为什么要像下面这样子set一下,然后get再使用,而不直接使用lpJet对象呢?)
env->SetLongField(thiz, nativePlayerInJavaObj, (jlong) lpJet);
env->SetLongField(weak_this, nativePlayerInJavaObj, 0);
// get
JetPlayer *lpJet = (JetPlayer *) env->GetLongField(thiz, nativePlayerInJavaObj);
if (lpJet == NULL) {
    jniThrowException(env, "java/lang/IllegalStateException", "Unable to retrieve JetPlayer pointer for openFile()");
    jniThrowException(env, "java/lang/IllegalArgumentException",
                String8::format("Failed to initialize %s, error %#x", tmp, err));
    return JNI_FALSE;
} else {
    lpJet->release();
    delete lpJet;
}

文件: android_media_MediaPlayer.cpp
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
extern int register_android_media_JetPlayer(JNIEnv *env);
// java端调用System.loadLibrary()加载一个库的时候会执行JNI_OnLoad(...)方法
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    javaVM = vm;
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
        goto bail;
    }
    assert(env != NULL);

    if (register_android_media_JetPlayer(env) < 0) {
        ALOGE("ERROR: JetPlayer native registration failed");
        goto bail;
    }

    // success -- return valid version number
    result = JNI_VERSION_1_4;

    bail:
    return result;
}
////////////////////////////////////////////

JavaVM* AndroidRuntime::getJavaVM() {
    return javaVM;
}

JNIEnv* AndroidRuntime::getJNIEnv() {
    JNIEnv* env = nullptr;
    if (javaVM->GetEnv((void**)(&env), JNI_VERSION_1_6) != JNI_OK) {
        return nullptr;
    }
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    return env;
}

jclass system = FindClassOrDie(env, "java/lang/System");

static vector<string> parseCsv(JNIEnv* env, jstring csvJString) {
    // jstring ---> const char*
    const char* charArray = env->GetStringUTFChars(csvJString, 0);
    const char* charArray = env->GetStringUTFChars(csvJString, NULL);
    if (charArray == NULL) {
        // Out of memory
    }
    // const char* ---> string
    string csvString(charArray);
    // ...
    env->ReleaseStringUTFChars(csvJString, charArray);
    return result;
}

JNIEnv *env = AndroidRuntime::getJNIEnv();
if (env) {
    env->CallStaticVoidMethod(...);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
} else {
    ALOGE("JET jetPlayerEventCallback(): No JNI env for JET event callback, can't post event.");
    return;
}

在java端调用native方法的对象被传递到jni层时,
最好这样使用new WeakReference<FFMPEG>(this).
参数: jobject weak_this;
jobject ffmpegJavaObject = weak_this;// error 直接赋值是不OK的
jobject ffmpegJavaObject = reinterpret_cast<jobject>(env->NewGlobalRef(weak_this));

// jintArray
参数: jintArray jSampleRate;
if (jSampleRate == 0) {
    ALOGE("Error creating AudioTrack: invalid sample rates");
    return (jint) AUDIO_JAVA_ERROR;
}
int* sampleRates = env->GetIntArrayElements(jSampleRate, NULL);
if (sampleRates == NULL) {
    ALOGE("...");
    return (jint) AUDIO_JAVA_ERROR;
}
int sampleRateInHertz = sampleRates[0];
env->ReleaseIntArrayElements(jSampleRate, sampleRates, JNI_ABORT);
参数: jintArray jSession;
if (jSession == NULL) {
    ALOGE("Error creating AudioTrack: invalid session ID pointer");
    return (jint) AUDIO_JAVA_ERROR;
}
jint* nSession = (jint *) env->GetPrimitiveArrayCritical(jSession, NULL);
if (nSession == NULL) {
    ALOGE("Error creating AudioTrack: Error retrieving session id pointer");
    return (jint) AUDIO_JAVA_ERROR;
}
audio_session_t sessionId = (audio_session_t) nSession[0];
env->ReleasePrimitiveArrayCritical(jSession, nSession, 0);
nSession = NULL;

// jbooleanArray
参数: jbooleanArray muteArray;
if (muteArray == NULL) {
    ALOGE("...");
    return (jint) AUDIO_JAVA_ERROR;
}
jboolean *muteTracks = NULL;
muteTracks = env->GetBooleanArrayElements(muteArray, NULL);
if (muteTracks == NULL) {
    ALOGE("android_media_JetPlayer_queueSegment(): failed to read track mute mask.");
    return JNI_FALSE;
}
muteTracks[0] = JNI_TRUE;
muteTracks[1] = JNI_FALSE;
...
env->ReleaseBooleanArrayElements(muteArray, muteTracks, 0);

// jstring
参数: jstring name;
if (name == NULL) {
    jniThrowException(env, "java/lang/NullPointerException", NULL);
    return;
}
const char *tmp = env->GetStringUTFChars(name, NULL);// nullptr
if (tmp == NULL) {
    return;
}
...
env->ReleaseStringUTFChars(name, tmp);


typedef struct {
    EAS_U32     libVersion;
    EAS_BOOL    checkedVersion;
} S_EAS_LIB_CONFIG;// 写在这里可以直接使用
static const S_EAS_LIB_CONFIG* pLibConfig = NULL;

video/raw
video/hevc
    OMX.MTK.VIDEO.DECODER.HEVC
    OMX.MTK.VIDEO.DECODER.HEVC.secure
    OMX.qcom.video.decoder.hevc.secure
    OMX.qcom.video.decoder.hevc
    OMX.google.hevc.decoder
    c2.android.hevc.decoder
video/avc
    OMX.MTK.VIDEO.DECODER.AVC
    OMX.MTK.VIDEO.DECODER.AVC.secure
    OMX.qcom.video.decoder.avc
    OMX.qcom.video.decoder.avc.secure
    OMX.google.h264.decoder
    c2.android.avc.decoder
video/3gpp
    OMX.MTK.VIDEO.DECODER.H263
    OMX.qcom.video.decoder.h263
    OMX.google.h263.decoder
    c2.android.h263.decoder
video/mp4v-es
    OMX.MTK.VIDEO.DECODER.MPEG4
    OMX.qcom.video.decoder.mpeg4
    OMX.google.mpeg4.decoder
    c2.android.mpeg4.decoder
video/mpeg2
    OMX.MTK.VIDEO.DECODER.MPEG2
    OMX.MTK.VIDEO.DECODER.MPEG2.secure
    OMX.qcom.video.decoder.mpeg2
    OMX.qcom.video.decoder.mpeg2.secure
    OMX.google.mpeg2.decoder
video/x-vnd.on2.vp8
    OMX.MTK.VIDEO.DECODER.VP8
    OMX.qcom.video.decoder.vp8
    OMX.google.vp8.decoder
    c2.android.vp8.decoder
video/x-vnd.on2.vp9
    OMX.MTK.VIDEO.DECODER.VP9
    OMX.MTK.VIDEO.DECODER.VP9.secure
    OMX.qcom.video.decoder.vp9
    OMX.qcom.video.decoder.vp9.secure
    OMX.google.vp9.decoder
    c2.android.vp9.decoder

audio/raw
    OMX.google.raw.decoder
    c2.android.raw.decoder
audio/flac
    OMX.qti.audio.decoder.flac
    OMX.google.flac.decoder
    c2.android.flac.decoder
audio/mpeg
    OMX.google.mp3.decoder
    c2.android.mp3.decoder
audio/ac3
    OMX.MTK.AUDIO.DECODER.DSPAC3
audio/eac3
    OMX.MTK.AUDIO.DECODER.DSPEAC3
audio/ac4
audio/mpeg-L2
    OMX.MTK.AUDIO.DECODER.DSPMP2
audio/mp4a-latm
    OMX.google.aac.decoder
    c2.android.aac.decoder
audio/x-ms-wma
    OMX.MTK.AUDIO.DECODER.DSPWMA
audio/vorbis
    OMX.google.vorbis.decoder
    c2.android.vorbis.decoder


Mutex mMutex;
Condition mCondition;

frameworks/av/media/libaaudio
frameworks/av/media/libaudioclient
frameworks/av/media/libaudiofoundation
frameworks/av/media/libaudiohal
frameworks/av/media/libaudioprocessing
frameworks/av/media/libmedia
frameworks/av/media/libmediahelper
frameworks/av/media/libmediametrics
frameworks/av/media/libmediaplayerservice
frameworks/av/media/libmediatranscoding

// 虚拟继承
struct AHandler          : public         RefBase {}
struct MediaCodec        : public         AHandler{}
class MediaCodecBuffer   : public         RefBase { 有些类也不一定是用virtual继承RefBase }
class IMediaDeathNotifier: public virtual RefBase {//
class IMediaDeathNotifier: virtual public RefBase {
    public:
        IMediaDeathNotifier() {}
        virtual ~IMediaDeathNotifier() {}
        virtual ~MediaCodecBuffer() = default;
    private:
        MediaCodecBuffer() = delete;
}
class MediaPlayer : public BnMediaPlayerClient,
                    public virtual IMediaDeathNotifier {
    public:
        MediaPlayer();
        ~MediaPlayer();
}
// 使用
#include <utils/StrongPointer.h>
sp<MediaPlayer> mp = new MediaPlayer();
if (mp == NULL) {
    jniThrowException(env, "java/lang/RuntimeException", "Out of memory");
    return;
}
setMediaPlayer(env, thiz, mp);

// 函数定义
status_t MediaCodec::getBufferAndFormat(
            size_t portIndex, size_t index, sp<MediaCodecBuffer> *buffer, sp<AMessage> *format) {...}
sp<AMessage> format;// 此时format != NULL
getBufferAndFormat(kPortIndexInput, index, buffer, &format);
// 在android中的基本用法(在底层差不多是这种模式)
static Mutex sLock;
static sp<MediaPlayer> setMediaPlayer(JNIEnv* env, jobject thiz, const sp<MediaPlayer>& player) {
    Mutex::Autolock l(sLock);
    sp<MediaPlayer> old = (MediaPlayer*)env->GetLongField(thiz, fields.context);
    if (player.get()) {
        player->incStrong((void*)setMediaPlayer);
    }
    if (old != 0) {
        old->decStrong((void*)setMediaPlayer);
    }
    env->SetLongField(thiz, fields.context, (jlong)player.get());
    return old;
}
static sp<MediaPlayer> getMediaPlayer(JNIEnv* env, jobject thiz) {
    Mutex::Autolock l(sLock);
    MediaPlayer* const p = (MediaPlayer*)env->GetLongField(thiz, fields.context);
    return sp<MediaPlayer>(p);
}
static void android_media_MediaPlayer_start(JNIEnv *env, jobject thiz) {
    ALOGV("start");
    sp<MediaPlayer> mp = getMediaPlayer(env, thiz);
    if (mp == NULL ) {
        jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }
    process_media_player_call( env, thiz, mp->start(), NULL, NULL );
}

static sp<JMediaCodec> setMediaCodec(
        JNIEnv *env, jobject thiz, const sp<JMediaCodec> &codec, bool release = true) {
    sp<JMediaCodec> old = (JMediaCodec *)env->CallLongMethod(thiz, gFields.lockAndGetContextID);
    if (codec != NULL) {
        codec->incStrong(thiz);
    }
    if (old != NULL) {
        if (release) {
            old->release();
        }
        old->decStrong(thiz);
    }
    env->CallVoidMethod(thiz, gFields.setAndUnlockContextID, (jlong)codec.get());
    return old;
}
static sp<JMediaCodec> getMediaCodec(JNIEnv *env, jobject thiz) {
    sp<JMediaCodec> codec = (JMediaCodec *)env->CallLongMethod(thiz, gFields.lockAndGetContextID);
    env->CallVoidMethod(thiz, gFields.setAndUnlockContextID, (jlong)codec.get());
    return codec;
}


class IInterface : public virtual RefBase
class BpRefBase : public virtual RefBase
class BpHwRefBase : public virtual RefBase
class [[clang::lto_visibility_public]] IBinder : public virtual RefBase

// const char *message, status_t opStatus
char msg[256];
sprintf(msg, "%s: status=0x%X", message, opStatus);

怎样输出jlong类型的值?
#include <stdint.h>
#include <inttypes.h>
jlong nativeAudioTrack
ALOGV("nativeAudioTrack=0x%" PRIX64 ", %d, %p", nativeAudioTrack, ...);

frameworks/base/core/jni/android_media_AudioTrack.cpp
frameworks/av/media/libaudioclient/AudioTrack.cpp

write(@NonNull byte[] audioData, int offsetInBytes, int sizeInBytes)
write(audioData, offsetInBytes, sizeInBytes, WRITE_BLOCKING)
native_write_byte(audioData, offsetInBytes, sizeInBytes, mAudioFormat, writeMode == WRITE_BLOCKING)
(void *)android_media_AudioTrack_writeArray<jbyteArray>
(void *)android_media_AudioTrack_writeArray<jshortArray>
(void *)android_media_AudioTrack_writeArray<jfloatArray>
// 用了模板(byte[], short[], float[])
android_media_AudioTrack_writeArray(JNIEnv *env, jobject thiz,
                                                T javaAudioData,
                                                jint offsetInSamples,
                                                jint sizeInSamples,
                                                jint javaAudioFormat,
                                                jboolean isWriteBlocking)
auto cAudioData = env->GetFloatArrayElements(javaAudioData, NULL);
if (cAudioData == NULL) {
    ALOGE("Error retrieving source of audio data to play");
    return (jint)AUDIO_JAVA_BAD_VALUE; // out of memory or no data to load
}
jint samplesWritten = writeToTrack(const sp<AudioTrack>& track, jint javaAudioFormat, const T *cAudioData,
                         jint offsetInSamples, jint sizeInSamples, bool blocking)
env->ReleaseFloatArrayElements(javaAudioData, cAudioData, 0);

static constexpr const char *stateToString(State state) {
    switch (state) {
        case STATE_ACTIVE:          return "STATE_ACTIVE";
        case STATE_STOPPED:         return "STATE_STOPPED";
        case STATE_PAUSED:          return "STATE_PAUSED";
        case STATE_PAUSED_STOPPING: return "STATE_PAUSED_STOPPING";
        case STATE_FLUSHED:         return "STATE_FLUSHED";
        case STATE_STOPPING:        return "STATE_STOPPING";
        default:                    return "UNKNOWN";
    }
}

std::vector<std::string> names;
bool hasSecure = false;
bool hasNonSecure = false;
for (const std::string &name : names) {
    if (name.length() >= 7 && name.substr(name.length() - 7) == ".secure") {
        hasSecure = true;
    } else {
        hasNonSecure = true;
    }
}

jni层抛异常
#include <nativehelper/ScopedLocalRef.h>
static void throwCryptoException(JNIEnv *env, status_t err, const char *msg) {
    ScopedLocalRef<jclass> clazz(env, env->FindClass("android/media/MediaCodec$CryptoException"));
    CHECK(clazz.get() != NULL);
    jmethodID constructID = env->GetMethodID(clazz.get(), "<init>", "(ILjava/lang/String;)V");
    CHECK(constructID != NULL);
    const char *defaultMsg = "Unknown Error";
    switch (err) {
        case ERROR_DRM_NO_LICENSE:
            err = gCryptoErrorCodes.cryptoErrorNoKey;
            defaultMsg = "Crypto key not available";
            break;
        default:
            break;
    }
    jstring msgObj = env->NewStringUTF(msg != NULL ? msg : defaultMsg);
    jthrowable exception = (jthrowable)env->NewObject(clazz.get(), constructID, err, msgObj);
    env->Throw(exception);
}

static jthrowable createCodecException(JNIEnv *env, status_t err, int32_t actionCode, const char *msg = NULL) {
    ScopedLocalRef<jclass> clazz(env, env->FindClass("android/media/MediaCodec$CodecException"));
    CHECK(clazz.get() != NULL);
    const jmethodID ctor = env->GetMethodID(clazz.get(), "<init>", "(IILjava/lang/String;)V");
    CHECK(ctor != NULL);
    ScopedLocalRef<jstring> msgObj(env, env->NewStringUTF(msg != NULL ? msg : String8::format("Error %#x", err)));
    // translate action code to Java equivalent
    switch (actionCode) {
    case ACTION_CODE_TRANSIENT:
        actionCode = gCodecActionCodes.codecActionTransient;
        break;
    case ACTION_CODE_RECOVERABLE:
        actionCode = gCodecActionCodes.codecActionRecoverable;
        break;
    default:
        actionCode = 0;  // everything else is fatal
        break;
    }
    switch (err) {
        case NO_MEMORY:
            err = gCodecErrorCodes.errorInsufficientResource;
            break;
        case DEAD_OBJECT:
            err = gCodecErrorCodes.errorReclaimed;
            break;
        default:
            break;
    }
    return (jthrowable)env->NewObject(clazz.get(), ctor, err, actionCode, msgObj.get());
}

android_media_MediaCodec.cpp
    JMediaCodec
        sp<MediaCodec> mCodec
            sp<MediaCodecPlugin> mPlugin = new MediaCodecPlugin();
java:
MediaCodec
    public static MediaCodec createByCodecName(@NonNull String name)
        new MediaCodec(name, false, false);
            native_setup(name, nameIsType, encoder);
C++:
static void android_media_MediaCodec_native_setup(JNIEnv *env, jobject thiz,
        jstring name, jboolean nameIsType, jboolean encoder)
    sp<JMediaCodec> codec = new JMediaCodec(env, thiz, tmp, nameIsType, encoder);
        sp<MediaCodec> mCodec = MediaCodec::CreateByComponentName(mLooper, name, &mInitStatus);
            sp<MediaCodec> codec = new MediaCodec(looper, pid, uid);


gFields.postEventFromNativeID ---> "postEventFromNative"
private void postEventFromNative(int what, int arg1, int arg2, Object obj)
what = EVENT_CALLBACK
private void handleCallback(@NonNull Message msg)

#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/ALooper.h>
struct AMessage;
struct AHandler;
struct ALooper;

std::vector<BufferInfo> mPortBuffers[2];
// 使用一
uint64_t size = 0;
size_t portNum = sizeof(mPortBuffers) / sizeof((mPortBuffers)[0]);// 重点
for (size_t i = 0; i < portNum; ++i) {
    size += mPortBuffers[i].size() * mVideoWidth * mVideoHeight * 3 / 2;
}
// 使用二
std::vector<BufferInfo> &buffers = mPortBuffers[portIndex];

// 类中属性,可以这样初始化
bool mGraphicOutput{false};
// 类中属性
std::shared_ptr<const std::vector<const BufferInfo>> mInputBuffers;
std::shared_ptr<const std::vector<const BufferInfo>> mOutputBuffers;
// 使用
std::shared_ptr<const std::vector<const BufferInfo>> inputBuffers(std::atomic_load(&mInputBuffers));
for (const BufferInfo &elem : *inputBuffers) {
    array->push_back(elem.mClientBuffer);
}

long ---> jlong ---> int64_t

int64_t mLastActivityTimeUs;
// 赋值
mLastActivityTimeUs = -1ll;
if (mLastActivityTimeUs < 0ll) {
}

sp<ICrypto> crypto;此时crypto为NULL
const char *name(可以直接赋值给AString变量,即AString codecName = name)
AString componentName;
AString tmp;
for (const AString &codecName : {componentName, tmp}) {}

#include <stdio.h>
#include <string.h>
int strncmp(const char *str1, const char *str2, size_t n)
如果返回值 < 0，则表示 str1 小于 str2。
如果返回值 > 0，则表示 str2 小于 str1。
如果返回值 = 0，则表示 str1 等于 str2。

参数中的参数是定义成指针还是定义成引用?
如果方法体中只用到这个参数的值,那么定义成引用,如果要对这个参数重新赋值,那么定义成指针.
status_t MediaCodec::PostAndAwaitResponse(const sp<AMessage> &msg, sp<AMessage> *response){}

struct MediaCodec : public AHandler
sp<AMessage> msg      = new AMessage(kWhatInit, this);
sp<AMessage> response = new AMessage;
sp<AMessage> response;
msg->setSize("index", bufferIndex);                 // size_t
msg->setInt64("timeUs", 0LL);                       // int64_t
msg->setInt32("flags", BUFFER_FLAG_CODECCONFIG);    // int32_t
msg->setDouble("timeUs", );                         // double
msg->setFloat("timeUs", );                          // float
msg->setString("componentName", name);              // const AString &
                            AString errorDetailMsg;
msg->setPointer("errorDetailMsg", &errorDetailMsg); // void *
msg->setPointer("key", (void *) key);
msg->setObject("c2buffer", obj);                    // const sp<RefBase> &
msg->setMessage("tunings", new AMessage);           // const sp<AMessage> &
status_t status = msg->postAndAwaitResponse(&response);

sp<ALooper> mLooper;
mLooper = new ALooper;
mLooper->setName("MediaCodec_looper");
mLooper->start(
        false,      // runOnCallingThread
        true,       // canCallJava
        ANDROID_PRIORITY_VIDEO);

MediaCodec异步解码过程
------------------------------------------------
native_init();
第一步
String codecName = "OMX.qcom.video.decoder.avc";
第二步
MediaCodec codec = MediaCodec.createByCodecName(codecName)
    new MediaCodec(codecName, false[nameIsType], false[encoder]);
        native_setup(codecName, nameIsType, encoder);
            sp<JMediaCodec> codec = new JMediaCodec(env, thiz, codecName, nameIsType, encoder);
                mCodec = MediaCodec::CreateByComponentName(mLooper, codecName, &mInitStatus);
                    const status_t ret = codec->init(name);
第三步
codec.setCallback(Callback cb, Handler handler);
    // 底层并没有保存或者使用这个Callback对象,只是用它判断是否为null
    native_setCallback(cb);
        status_t err = codec->setCallback(cb);
            if (cb != NULL) {
                mCallbackNotification = new AMessage(kWhatCallbackNotify, this);
            }
            mCodec->setCallback(mCallbackNotification);
                sp<AMessage> callback;
                CHECK(msg->findMessage("callback", &callback));
                sp<AMessage> mCallback = callback;
第四步
codec.configure(MediaFormat format, Surface surface, MediaCrypto crypto, int flags);
    configure(format, surface, crypto, null, 0[decoder]);
        native_configure(keys, values, surface, crypto, descramblerBinder, flags);
            err = codec->configure(format, bufferProducer, crypto, descrambler, flags);
                mSurfaceTextureClient = new Surface(bufferProducer, true[controlledByApp]);
                mCodec->configure(format, mSurfaceTextureClient, crypto, descrambler, flags);
第五步
codec.start();
    native_start();
        status_t err = codec->start();
    ByteBuffer[] buffers = getBuffers(input);// true and false
        jobjectArray buffers;
        status_t err = codec->getBuffers(env, input, &buffers);
            sp<MediaCodecBuffer> buffer;
            status_t err = mCodec->getInputBuffer(index, &buffer)
            status_t err = mCodec->getOutputBuffer(index, &buffer)
    if (input) {
        mCachedInputBuffers = buffers;
    } else {
        mCachedOutputBuffers = buffers;
    }

public ByteBuffer getInputBuffer(int index)
public ByteBuffer getOutputBuffer(int index)
    ByteBuffer newBuffer = getBuffer(true [input], index);
    ByteBuffer newBuffer = getBuffer(false[input], index);
        jobject buffer;
        status_t err = codec->getBuffer(env, input, index, &buffer);
            sp<MediaCodecBuffer> buffer;
            status_t err = mCodec->getInputBuffer(index, &buffer);
            status_t err = mCodec->getOutputBuffer(index, &buffer);

然后关注以下几个接口就行了(都是public void)
第一个接口
onInputBufferAvailable(MediaCodec codec, int roomIndex)
    ByteBuffer room = codec.getInputBuffer(roomIndex);
    room.put(data, 0, size);
    codec.queueInputBuffer(roomIndex, 0, size, presentationTimeUs, 0[flags]);
第二个接口
onOutputBufferAvailable(MediaCodec codec, int roomIndex, MediaCodec.BufferInfo roomInfo)
    ByteBuffer room = codec.getOutputBuffer(roomIndex);
    ......
    codec.releaseOutputBuffer(roomIndex, true[video]/false[audio]);
第三个接口
onError(MediaCodec mediaCodec, MediaCodec.CodecException e)
第四个接口
onOutputFormatChanged(MediaCodec mediaCodec, MediaFormat mediaFormat)

异步回调
由MediaCodec类的mCallback(sp<AMessage>)传递过来的(见上面第三步的过程)
发送消息:
sp<AMessage> msg = mCallback->dup();
msg->setInt32("callbackID", CB_INPUT_AVAILABLE);
msg->setInt32("index", index);
msg->post();
接收消息:
void JMediaCodec::onMessageReceived(const sp<AMessage> &msg)
    case kWhatCallbackNotify: {
        handleCallback(msg);
    }

void JMediaCodec::handleCallback(const sp<AMessage> &msg) ---> postEventFromNative(...)
底层回调接口
private void postEventFromNative(int what, int arg1, int arg2, Object obj)
    handleCallback(msg);
        case CB_INPUT_AVAILABLE:
            mCallback.onInputBufferAvailable(mCodec, index);
        case CB_OUTPUT_AVAILABLE:
            mCallback.onOutputBufferAvailable(mCodec, index, info);
        case CB_ERROR:
            mCallback.onError(mCodec, (MediaCodec.CodecException) msg.obj);
        case CB_OUTPUT_FORMAT_CHANGE:
            mCallback.onOutputFormatChanged(mCodec, new MediaFormat((Map<String, Object>) msg.obj));
------------------------------------------------

// input
dequeueInputBuffer
getInputBuffer
queueInputBuffer
// output
dequeueOutputBuffer
getOutputBuffer
releaseOutputBuffer

frameworks/av/include/media/stagefright/media/stagefright/MediaCodec.h

frameworks/av/media/libstagefright/MediaCodec.cpp分析
------------------------------------------------
frameworks/base/media/jni/android_media_MediaCodec.cpp
    JMediaCodec::JMediaCodec(...)
        mCodec = MediaCodec::CreateByComponentName(mLooper, codecName, &mInitStatus);
frameworks/av/media/libstagefright/MediaCodec.cpp
    sp<MediaCodec> MediaCodec::CreateByComponentName(...)
        sp<MediaCodec> codec = new MediaCodec(looper, pid, uid);
        const status_t ret = codec->init(name);
            sp<CodecBase> mCodec = GetCodecBase(componentName, owner);
            mCodec->setCallback(
                std::unique_ptr<CodecBase::CodecCallback>(new CodecCallback(new AMessage(kWhatCodecNotify, this))));
            std::shared_ptr<BufferChannelBase> mBufferChannel = mCodec->getBufferChannel();

            mCodec->initiateStart();
            mCodec->signalResume();
            mCodec->setSurface(mReleaseSurface->getSurface());
            mCodec->initiateShutdown(false);
            mCodec->signalEndOfInputStream();
            mCodec->signalFlush();
            mCodec->signalRequestIDRFrame();
            mCodec->id()
            mCodec->signalSetParameters(params);
            mCodec->initiateAllocateComponent(format);
            mCodec->initiateConfigureComponent(format);
            mCodec->initiateCreateInputSurface();
            mCodec->initiateSetInputSurface(static_cast<PersistentSurface *>(obj.get()));
------------------------------------------------




const char* pName;
size_t nKeyLen = strlen(pName) + 1;// 关键点
char*        pKey;
pKey = new char[nKeyLen];
strncpy(pKey, pName, nKeyLen);
......
delete       pKey;// 关键点
pKey = NULL;

int         nSize;
void*       pData;
pData = new char[nSize];
delete (char *)pData;// 关键点
pData = NULL;

typedef struct PplShm {
    pplList* pList;
    bool     bAttached;
} PplShm;
// 定义函数
PplShmCreate(const char* pName, int nSize, PplShm** ppShm) {
    PplAssert(ppShm != NULL);
    PplAssert(*ppShm == NULL);
    *ppShm = new PplShm;// 关键点
    PplAssert(*ppShm != NULL);
    if (*ppShm == NULL) {
        break;
    }
    (*ppShm)->pList = pList->pNext;
    (*ppShm)->bAttached = false;

    PPL_LOG((__FILE__, __LINE__, PPL_LOG_INFO,
                "PplShmCreate Count = %d, ShmAddress = %p\n", ++count, &(*ppShm)));
    
    ......
    delete *ppShm;// 关键点
    *ppShm = NULL;
}
// 使用函数
PplShm* m_pShmID = NULL;
PplShmCreate(m_pShmPathName, sizeof(vnac_shm), &m_pShmID);

Android的底层库libutils






































*/





















