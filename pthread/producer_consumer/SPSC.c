// Single-producer , single-consumer Queue
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define MAX 10

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

struct SPSCQueue {
    /* Define Your Data Here */
    int* data;
    int front;
    int rear;
} typedef SPSCQueue;

struct thread_args {
    SPSCQueue* arg1;
    void* arg2;
};

SPSCQueue* SPSCQueueInit(int capacity);
void* SPSCQueuePush(void* queue);
void* SPSCQueuePop(void* queue);
void SPSCQueueDestroy(SPSCQueue*);

int main() {
    pthread_t tid1, tid2;
    srand(time(NULL));
    SPSCQueue* queue = SPSCQueueInit(MAX);
    pthread_mutex_init(&mutex, NULL);
    struct thread_args args = {queue, NULL};

    pthread_create(&tid2, NULL, SPSCQueuePop, queue);
    pthread_create(&tid1, NULL, SPSCQueuePush, queue);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    SPSCQueueDestroy(queue);
    return 0;
}

SPSCQueue* SPSCQueueInit(int capacity) {
    SPSCQueue* queue = (SPSCQueue*)malloc(sizeof(SPSCQueue));
    queue->data = (int*)malloc(sizeof(int) * capacity);
    queue->front = -1;
    queue->rear = -1;
    sem_init(&empty, 0, capacity);
    sem_init(&full, 0, 0);
    return queue;
}

void* SPSCQueuePush(void* que) {
    SPSCQueue* queue = (SPSCQueue*)que;
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        queue->rear++;
        queue->rear %= MAX;
        queue->data[queue->rear] = rand() % 100;
        printf("--------producer:%d\n", queue->data[queue->rear]);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(rand() % 3);
    }
}
void* SPSCQueuePop(void* que) {
    SPSCQueue* queue = (SPSCQueue*)que;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        queue->front++;
        queue->front %= MAX;
        printf("--consumer:%d\n", queue->data[queue->front]);
        queue->data[queue->front] = 0;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(rand() % 3);
    }
    return NULL;
}
void SPSCQueueDestroy(SPSCQueue* queue) {
    free(queue->data);
    free(queue);
}
