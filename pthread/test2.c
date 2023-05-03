#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define QUEUE_SIZE 10

typedef struct SPSCQueue {
    int front, rear;
    int capacity;
    void **array;
    pthread_mutex_t mutex;
    pthread_cond_t cond_full, cond_empty;
} SPSCQueue;

SPSCQueue *SPSCQueueInit(int capacity) {
    SPSCQueue *queue = malloc(sizeof(SPSCQueue));
    queue->capacity = capacity;
    queue->front = -1;
    queue->rear = -1;
    queue->array = malloc(capacity * sizeof(void *));
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond_full, NULL);
    pthread_cond_init(&queue->cond_empty, NULL);
    return queue;
}

void SPSCQueuePush(SPSCQueue *queue, void *s) {
    pthread_mutex_lock(&queue->mutex);
    while ((queue->front == 0 && queue->rear == queue->capacity - 1) || queue->rear == (queue->front - 1) % (queue->capacity - 1)) {
        pthread_cond_wait(&queue->cond_full, &queue->mutex);
    }
    if (queue->front == -1) {
        queue->front = queue->rear = 0;
    } else if (queue->rear == queue->capacity - 1 && queue->front != 0) {
        queue->rear = 0;
    } else {
        queue->rear++;
    }
    queue->array[queue->rear] = s;
    pthread_cond_signal(&queue->cond_empty);
    pthread_mutex_unlock(&queue->mutex);
}

void *SPSCQueuePop(SPSCQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->front == -1) {
        pthread_cond_wait(&queue->cond_empty, &queue->mutex);
    }
    void *data = queue->array[queue->front];
    queue->array[queue->front] = NULL;
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else if (queue->front == queue->capacity - 1) {
        queue->front = 0;
    } else {
        queue->front++;
    }
    pthread_cond_signal(&queue->cond_full);
    pthread_mutex_unlock(&queue->mutex);
    return data;
}

void SPSCQueueDestory(SPSCQueue *queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond_full);
    pthread_cond_destroy(&queue->cond_empty);
    free(queue->array);
    free(queue);
}
int main(){
    
}