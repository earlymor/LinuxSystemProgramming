#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Task{

}Task;
// 线程池结构体
typedef struct threadPool {
    // 任务队列
    Task* taskQueue;
    int queueCapacity;  // 容量
    int queueSize;      // 当前任务个数
    int queueFront;     // 队首
    int queueRear;      // 队尾

    pthread_t managerID;  // 管理线程
    pthread_t* threadIDs;   // 工作线程
    int maxNum;            // 最大线程数
    int minNum;            // 最小线程数
    int busyNum;     // 忙线程
    int liveNum;     // 存活线程
    int exitNum;    // 要销毁的线程个数

    pthread_mutex_t mutexPool;// 互斥量锁整个线程池
    pthread_mutex_t mutexBusy;// 锁busyNum变量
    // 条件变量
    pthread_cond_t full; // 任务队列是不是满
    pthread_cond_t empty; // 任务队列是不是空

    int shutdown; // 
} threadPool;

// 创建线程池并初始化
threadPool* threadPoolCreate(int min , int max ,int queueSize){
    threadPool* pool = (threadPool*)malloc(sizeof(threadPool));
    if(pool == NULL){
        printf("线程池初始化失败\n");
        return NULL;
    }

    

} 
// 销毁线程池
// 给线程池添加任务
// 获取线程池中工作的线程个数
// 获取线程池中活着的线程个数