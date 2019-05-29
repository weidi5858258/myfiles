#ifndef LOG_DUBUG_H
#define LOG_DUBUG_H

// delete this on release.
#define LOCALDEBUG

#include <android/log.h>
#include <utils/Log.h>

#ifdef LOCALDEBUG
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#else
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#endif

#define SERVER_NAME "com.weidi.mydaemon"
#define SERVER_NAME_ "com.weidi.callback"
#define MYDAEMON "com.weidi.daemon.IDaemon"
#define CALLBACK "com.weidi.daemon.ICallback"

#endif // LOG_DEBUG_H
