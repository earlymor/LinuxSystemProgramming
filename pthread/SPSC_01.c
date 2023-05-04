#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include <time.h>
// Single-Producer, Single-Consumer Queue
typedef struct {
    void** buffer;          // 数据缓冲区
    int capacity;           // 容量
    int front;              // 队首
    int rear;               // 队尾
    sem_t empty;            // 信号量表示空缓冲区数量
    sem_t full;             // 信号量表示满缓冲区数量
    pthread_mutex_t mutex;  // 互斥锁
} SPSCQueue;
// Single-producer , single-consumer Queue

SPSCQueue* SPSCQueueInit(int capacity) {  // 队列初始化
    SPSCQueue* queue = (SPSCQueue*)malloc(sizeof(SPSCQueue));

    queue->capacity = capacity;
    queue->buffer = (void**)malloc(sizeof(void*) * capacity);
    queue->front = -1;
    queue->rear = -1;

    pthread_mutex_init(&queue->mutex, NULL);

    sem_init(&queue->empty, 0, capacity);
    sem_init(&queue->full, 0, 0);

    return queue;
}
void SPSCQueuePush(SPSCQueue* queue, void* s) {
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);
    queue->rear++;
    queue->rear %= queue->capacity;
    queue->buffer[queue->rear] = s;
    printf("--------producer:%d \n", *(int*)queue->buffer[queue->rear]);
    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
    // sleep(rand() % 2);
}

void* SPSCQueuePop(SPSCQueue* queue) {
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);
    queue->front++;
    queue->front %= queue->capacity;
    printf("-----------------------consumer:%d\n",*(int*) queue->buffer[queue->front],queue->front);
    queue->buffer[queue->front] = 0;
    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
    // sleep(rand() % 3);

    return NULL;
}

void SPSCQueueDestroy(SPSCQueue* queue) {
    pthread_mutex_destroy(&queue->mutex);

    sem_destroy(&queue->empty);
    sem_destroy(&queue->full);

    free(queue->buffer);
    free(queue);
}

// Multi-producer , Multi-consumer Queue
struct MPMCQueue {
    /* Define Your Data Here */
} typedef MPMCQueue;
/* 
MPMCQueue* MPMCQueueInit(int capacity);
void MPMCQueuePush(MPMCQueue* queue, void* s);
void* MPMCQueuePop(MPMCQueue* queue);
void MPMCQueueDestory(MPMCQueue*); */