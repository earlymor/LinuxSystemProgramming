#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
void* worker(void* arg) {}
// 创建线程池并初始化
threadPool* threadPoolCreate(int min, int max, int queueSize) {
    threadPool* pool = (threadPool*)malloc(sizeof(threadPool));
    if (pool == NULL) {
        printf("线程池初始化失败\n");
        return NULL;
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
    pool->taskQueue = (Task*)malloc(sizeof(Task)*queueSize);
    pool->queueCapacity = queueSize;
    pool->queueFront = -1;
    pool->queueRear = -1;
    pool->queueSize = 0;

    // 创建线程
    memset(pool->threadIDs, 0, max * (sizeof(pthread_t)));
    for (int i = 0; i < pool->minNum; i++) {
        pthread_create(pool->threadIDs[i], NULL,
                       pool->taskQueue[pool->queueFront], pool);
    }
}
// 销毁线程池
// 给线程池添加任务
// 获取线程池中工作的线程个数
// 获取线程池中活着的线程个数