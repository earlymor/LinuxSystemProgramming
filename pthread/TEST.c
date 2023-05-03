#include "SPSC_01.c"
#define MAX 10
int arr[MAX];
void* producer(void* arg) {
    SPSCQueue* queue = (SPSCQueue*)arg;
    
    for (int i = 0; i < MAX; i++) {
        arr[i]=i;
        SPSCQueuePush(queue, &arr[i]);
    }
    return NULL;
}

void* consumer(void* arg) {
    SPSCQueue* queue = (SPSCQueue*)arg;
    for (int i = 0; i < MAX; i++) {
        SPSCQueuePop(queue);
    }
    return NULL;
}
int main() {
// 测试SPSCQueue
SPSCQueue *queue = SPSCQueueInit(MAX);
pthread_t producer_tid, consumer_tid;
pthread_create(&consumer_tid, NULL, consumer, queue);
pthread_create(&producer_tid, NULL, producer, queue);
pthread_join(consumer_tid, NULL);
pthread_join(producer_tid, NULL);
SPSCQueueDestroy(queue);
}