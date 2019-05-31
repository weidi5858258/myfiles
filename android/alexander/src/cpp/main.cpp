#include "../include/MyHead.h"

#define LOG_TAG "alexander"

/***
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    ALOGD("-------------------- main start --------------------");

    pid_t pid = getpid();
    ALOGD("Daemon main() PID: %d", pid);
    uid_t uid = getuid();
    ALOGD("Daemon main() UID: %d", uid);

    // sp<ProcessState> proc(ProcessState::self());
    // sp<IServiceManager> sm = defaultServiceManager();

    /***
     在android启动的时候,由系统启动时的init进程启动其他服务,
     这些服务需要在init.rc中进行配置过.
     MyDaemon作为一个服务从android开机启动后一直常驻后台.
     通过binder通信方式跟其他进程进行通信.
     如java代码通过jni方式执行native层代码,
     然后native层代码通过binder通信方式跟MyDaemon进行通信.
     */
    // 先执行BnDaemon的构造方法
    // 再执行MyDaemon的构造方法
    // addService inside in BinderService
    // BnDaemon::instantiate();

    // ProcessState::self()->startThreadPool();
    // IPCThreadState::self()->joinThreadPool();

    ALOGD("-------------------- main end   --------------------");
    return 0;
}
