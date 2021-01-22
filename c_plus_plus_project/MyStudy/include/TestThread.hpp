//
// Created by alexander on 2020/1/3.
//

#ifndef MYSTUDY_TESTTHREAD_HPP
#define MYSTUDY_TESTTHREAD_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <zconf.h>

#include <pthread.h>
#include <semaphore.h>

struct WdThread {
    pthread_t thread;
};
struct WdMutex {
    pthread_mutex_t mutex;
};
struct WdSem {
    sem_t sem;
};
typedef struct WdThread WdThread;
typedef struct WdMutex WdMutex;
typedef struct WdSem WdSem;

namespace alexander {

    /***
     同一进程之间不同线程之间的同步用法

     alexander::TestThread testThread;
     testThread.testThread();
     testThread.destroy();
     */
    class TestThread {
    public:
        // 用于同步
        static pthread_mutex_t testMutex;
        // 相当于java中的wait(), notify()一样的用法
        static pthread_cond_t testCond;
    public:
        TestThread();

        virtual ~TestThread();

        void testThread();

        static void *test_thread(void *msg);

        static void destroy();
    };

    pthread_mutex_t TestThread::testMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t TestThread::testCond = PTHREAD_COND_INITIALIZER;

    TestThread::TestThread() {
        // 初始化
        // 第一种初始化:静态初始化锁
        testMutex = PTHREAD_MUTEX_INITIALIZER;
        testCond = PTHREAD_COND_INITIALIZER;
        // 第二种初始化:调用函数
        // pthread_mutexattr_init(pthread_mutexattr_t *mattr);// 设置属性,一般设为NULL就行了
        // int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mattr);
        pthread_mutex_init(&testMutex, NULL);
        pthread_cond_init(&testCond, NULL);

        // 互斥锁的范围
        // 指定是该进程与其他进程的同步还是同一进程内不同线程之间的同步。
        // 可以设置为PTHREAD_PROCESS_SHARE和PTHREAD_PROCESS_PRIVATE。默认是后者，表示进程内使用锁。
        // int pthread_mutexattr_setpshared(pthread_mutexattr_t *mattr, int pshared)
        // int pthread_mutexattr_getpshared(const pthread_mutexattr_t * __restrict, int * __restrict);
    }

    TestThread::~TestThread() {

    }

    void *TestThread::test_thread(void *msg) {
        pthread_mutex_lock(&testMutex);
        for (int i = 0; i < 15; i++) {
            printf("test_thread() %d\n", i);
            usleep(1000);
        }
        pthread_mutex_unlock(&testMutex);

        // wait()
        /*pthread_mutex_lock(&testMutex);
        pthread_cond_wait(&testCond, &testMutex);
        pthread_mutex_unlock(&testMutex);*/

        // notify()
        /*pthread_mutex_lock(&testMutex);
        pthread_cond_signal(&testCond);
        pthread_mutex_unlock(&testMutex);*/
    }

    void TestThread::testThread() {
        pthread_t pthread1;
        pthread_t pthread2;
        pthread_create(&pthread1, NULL, TestThread::test_thread, NULL);
        pthread_create(&pthread2, NULL, TestThread::test_thread, NULL);
        pthread_join(pthread1, NULL);
        pthread_join(pthread2, NULL);
        pthread_detach(pthread1);
        pthread_detach(pthread2);
    }

    void TestThread::destroy() {
        pthread_mutex_destroy(&testMutex);
        pthread_cond_destroy(&testCond);
    }
}

#endif //MYSTUDY_TESTTHREAD_HPP
