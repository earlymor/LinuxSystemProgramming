//Multi-producer , Multi-consumer Queue
#include <stdio.h>
#include <pthread.h>
struct MPMCQueue {
    /* Define Your Data Here */
} typedef MPMCQueue;

MPMCQueue *MPMCQueueInit(int capacity);
void MPMCQueuePush(MPMCQueue *queue, void *s);
void *MPMCQueuePop(MPMCQueue *queue);
void MPMCQueueDestory(MPMCQueue *);