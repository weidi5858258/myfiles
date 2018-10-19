
/*
 * author : wangbojing
 * email  : 1989wangbojing@163.com 
 * github : https://github.com/wangbojing
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>


#define LL_ADD(item, list) do {    \
    item->prev = NULL;                \
    item->next = list;                \
    list = item;                    \
} while(0)

#define LL_REMOVE(item, list) do {                        \
    if (item->prev != NULL) item->prev->next = item->next;    \
    if (item->next != NULL) item->next->prev = item->prev;    \
    if (list == item) list = item->next;                    \
    item->prev = item->next = NULL;                            \
} while(0)

typedef struct NJOB {
    // 定义函数指针
    // 函数指针的参数是struct NJOB *,那么具体的函数参数也必须是struct NJOB *.
    void (*job_function)(struct NJOB *job);

    void *user_data;

    struct NJOB *prev;
    struct NJOB *next;
} nJob;

// 一个线程就是一个工作者
typedef struct NWORKER {
    pthread_t thread;
    int terminate;
    // 每个线程都包含同一个工作队列
    // 这样工作队列中的同步锁可以使用于当前线程
    struct NWORKQUEUE *workqueue;
    struct NWORKER *prev;
    struct NWORKER *next;
} nWorker;

// 工作队列,全局就一个
typedef struct NWORKQUEUE {
    struct NWORKER *workers;
    struct NJOB *waiting_jobs;
    pthread_mutex_t jobs_mtx;
    pthread_cond_t jobs_cond;
} nWorkQueue;

typedef nWorkQueue nThreadPool;

static void *ntyWorkerThread(void *ptr) {
    nWorker *worker = (nWorker *) ptr;

    while (1) {
        pthread_mutex_lock(&worker->workqueue->jobs_mtx);

        while (worker->workqueue->waiting_jobs == NULL) {
            if (worker->terminate) {
                break;
            }
            pthread_cond_wait(&worker->workqueue->jobs_cond, &worker->workqueue->jobs_mtx);
        }

        if (worker->terminate) {
            pthread_mutex_unlock(&worker->workqueue->jobs_mtx);
            break;
        }

        nJob *job = worker->workqueue->waiting_jobs;
        if (job != NULL) {
            LL_REMOVE(job, worker->workqueue->waiting_jobs);
        }

        pthread_mutex_unlock(&worker->workqueue->jobs_mtx);

        if (job == NULL) {
            continue;
        }

        job->job_function(job);
    }

    free(worker);
    pthread_exit(NULL);
}

int ntyThreadPoolCreate(nThreadPool *workqueue, int numWorkers) {
    if (numWorkers < 1) {
        numWorkers = 1;
    }
    // 先设置好工作队列
    memset(workqueue, 0, sizeof(nThreadPool));

    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&(workqueue->jobs_mtx), &blank_mutex, sizeof(workqueue->jobs_mtx));

    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&(workqueue->jobs_cond), &blank_cond, sizeof(workqueue->jobs_cond));

    int i = 0;
    for (i = 0; i < numWorkers; i++) {
        nWorker *worker = (nWorker *) malloc(sizeof(nWorker));
        if (worker == NULL) {
            perror("malloc");
            return 1;
        }
        // 结构体一般先用memset清零一下
        memset(worker, 0, sizeof(nWorker));

        worker->workqueue = workqueue;

        int ret = pthread_create(&worker->thread, NULL, ntyWorkerThread, (void *) worker);
        if (ret) {
            perror("pthread_create");
            free(worker);
            return 1;
        }

        LL_ADD(worker, worker->workqueue->workers);
    }

    return 0;
}


void ntyThreadPoolShutdown(nThreadPool *workqueue) {
    nWorker *worker = NULL;

    for (worker = workqueue->workers; worker != NULL; worker = worker->next) {
        worker->terminate = 1;
    }

    pthread_mutex_lock(&workqueue->jobs_mtx);

    workqueue->workers = NULL;
    workqueue->waiting_jobs = NULL;

    pthread_cond_broadcast(&workqueue->jobs_cond);

    pthread_mutex_unlock(&workqueue->jobs_mtx);
}

void ntyThreadPoolQueue(nThreadPool *workqueue, nJob *job) {
    pthread_mutex_lock(&workqueue->jobs_mtx);

    LL_ADD(job, workqueue->waiting_jobs);

    pthread_cond_signal(&workqueue->jobs_cond);
    pthread_mutex_unlock(&workqueue->jobs_mtx);
}




/************************** debug thread pool **************************/

#define KING_MAX_THREAD            3
#define KING_COUNTER_SIZE        1000

void king_counter(nJob *job) {
    int index = *(int *) job->user_data;

    printf("index : %d, selfid : %lu\n", index, pthread_self());

    free(job->user_data);
    free(job);
}


int main(int argc, char *argv[]) {
    nThreadPool pool;

    ntyThreadPoolCreate(&pool, KING_MAX_THREAD);

    // 创建一个job
    /*nJob *job = (nJob *) malloc(sizeof(nJob));
    if (job == NULL) {
        perror("malloc");
        exit(1);
    }

    // 通过赋值使得函数指针指向某具体函数
    job->job_function = king_counter;
    job->user_data = malloc(sizeof(int));
    *(int *) job->user_data = 100;

    // 把job放进线程池
    ntyThreadPoolQueue(&pool, job);*/

    int i = 0;
    for (i = 0; i < KING_COUNTER_SIZE; i++) {
        // 创建一个job
        nJob *job = (nJob *) malloc(sizeof(nJob));
        if (job == NULL) {
            perror("malloc");
            exit(1);
        }

        // 通过赋值使得函数指针指向某具体函数
        job->job_function = king_counter;
        job->user_data = malloc(sizeof(int));
        *(int *) job->user_data = i;

        // 把job放进线程池
        ntyThreadPoolQueue(&pool, job);
    }

    getchar();
    printf("\n");
}



