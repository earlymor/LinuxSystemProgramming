#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define NUM 5
typedef struct Task {
    void (*func)(void* arg);
    void* arg;
} Task;
// 线程池结构体
typedef struct threadPool {
    // 任务队列
    Task* taskQueue;
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务个数
    int queueFront;     // 队首
    int queueRear;      // 队尾

    pthread_t managerID;   // 管理线程
    pthread_t* threadIDs;  // 工作线程
    int maxNum;            // 最大线程数
    int minNum;            // 最小线程数
    int busyNum;           // 忙线程
    int liveNum;           // 存活线程
    int exitNum;           // 要销毁的线程个数

    pthread_mutex_t mutexPool;  // 互斥量锁整个线程池
    pthread_mutex_t mutexBusy;  // 锁busyNum变量
    // 条件变量
    pthread_cond_t full;   // 任务队列是不是满
    pthread_cond_t empty;  // 任务队列是不是空

    int shutdown;  //
} threadPool;
// 工作线程函数
void* worker(void* arg) {
    threadPool* pool = (threadPool*)arg;
    while (1) {
        pthread_mutex_lock(&pool->mutexPool);
        // 任务队列为空且线程池不关闭
        while (pool->queueSize == 0 && !pool->shutdown) {
            // 阻塞线程
            pthread_cond_wait(&pool->empty, &pool->mutexPool);
            // 判断是否销毁线程(exitNum>0)
            if (pool->exitNum > 0) {
                pool->liveNum--;
                pool->exitNum--;
                pthread_mutex_unlock(&pool->mutexPool);
                threadExit(&pool);
            }
        }
        // 判断线程池是否关闭
        if (pool->shutdown) {
            threadExit(&pool);
        }
        // 取出任务
        pool->queueFront++;
        pool->queueFront %= pool->queueCapacity;
        void (*func)(void*) = pool->taskQueue[pool->queueFront].func;
        void* arg = pool->taskQueue[pool->queueFront].arg;
        pool->queueSize--;
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);

        // 执行任务
        printf("thread %ld start working...\n", pthread_self());
        func(arg);
        free(arg);
        printf("thread %ld end working...\n", pthread_self());
        // 忙线数量减一
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
}
// 管理者线程函数
void* manager(void* arg) {
    threadPool* pool = (threadPool*)arg;
    while (!pool->shutdown) {
        // 每隔三秒检查一次
        sleep(3);

        // ??取出线程池中的线程数量,忙线数量,任务数量
        pthread_mutex_lock(&pool->mutexPool);

        // 根据数量来进行添加线程
        // 忙线数量小于存活数量的两倍,存活数量小于等于最大线程数
        // 存活线程小于任务数量+NUM
        if (pool->busyNum * 2 < pool->liveNum &&
            pool->liveNum < pool->queueSize + NUM) {
            // 每次添加两个线程
            int count = 0;
            for (int i = 0; i < pool->maxNum && count < NUM &&
                            pool->liveNum < pool->maxNum;
                 i++) {
                if (pool->threadIDs[i] == 0) {  //  找到空线程
                    count++;
                    pool->liveNum++;
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                }
            }
        }
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexPool);
        // 销毁线程
        if (pool->liveNum > pool->queueSize) {
            // 让工作的线程自杀
            pool->exitNum = 2;  // 每次退出两个
            // 唤醒线程
            for (int i = 0; i < pool->exitNum; i++) {
                pthread_cond_signal(&pool->empty);
            }
        }
        pthread_mutex_unlock(&pool->mutexPool);
    }
}
/* 创建线程池并初始化 */
threadPool* threadPoolCreate(int min, int max, int queueSize) {
    threadPool* pool = (threadPool*)malloc(sizeof(threadPool));
    do {
        if (pool == NULL) {
            printf("线程池初始化失败\n");
            break;
        }
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
        if (pool->threadIDs == NULL) {
            printf("malloc threadIDs fail...\n");
            break;
        }

        // 线程池变量初始化
        pool->minNum = min;
        pool->maxNum = max;
        pool->busyNum = 0;
        pool->liveNum = min;
        pool->exitNum = 0;
        if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
            pthread_cond_init(&pool->full, NULL) != 0 ||
            pthread_cond_init(&pool->empty, NULL) != 0) {
            printf("mutex init error\n");
            exit(1);
        }
        // 任务队列
        pool->taskQueue = (Task*)malloc(sizeof(Task) * queueSize);
        pool->queueCapacity = queueSize;
        pool->queueFront = -1;
        pool->queueRear = -1;
        pool->queueSize = 0;

        // 创建线程
        pthread_create(pool->managerID, NULL, manager, pool);  // 管理者线程
        memset(pool->threadIDs, 0, max * (sizeof(pthread_t)));  // 工作线程
        for (int i = 0; i < pool->minNum; i++) {
            pthread_create(pool->threadIDs[i], NULL, worker, pool);
        }
        return pool;
    } while (0);

    // 释放资源
    if (pool && pool->threadIDs)
        free(pool->threadIDs);
    if (pool && pool->taskQueue)
        free(pool->taskQueue);
    if (pool)
        free(pool);
    return NULL;
}
// 销毁线程池
int threadPoolDestroy(threadPool* pool) {}
// 给线程池添加任务
void threadPoolAdd(threadPool* pool, void (*func)(void*), void* arg) {
    pthread_mutex_lock(&pool->mutexPool);
    if (pool->shutdown) {
        return;
    }
    pool->queueRear++;
    pool->queueRear %= pool->queueCapacity;
    pool->taskQueue[pool->queueRear].func = func;
    pool->taskQueue[pool->queueRear].arg = arg;
}
// 获取线程池中工作的线程个数
int threadPoolBusyNum(threadPool* pool) {}
// 获取线程池中活着的线程个数
int threadPoolAliveNum(threadPool* pool) {}
// 线程退出
int threadExit(threadPool* pool) {
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->maxNum; i++) {
        if (tid == pool->threadIDs[i]) {
            pool->threadIDs[i] = 0;
            printf("thread is exiting");
            break;
        }
    }
    pthread_exit(NULL);
}