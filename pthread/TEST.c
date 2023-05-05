#include "SPSC_01.c"
#define MAX 10
int arr[MAX];
void* producer(void* arg) {
    SPSCQueue* queue = (SPSCQueue*)arg;

    for (int i = 0; i < MAX; i++) {
        arr[i] = i;
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
    SPSCQueue* queue = SPSCQueueInit(MAX);
    pthread_t producer_tid[5], consumer_tid[3];
    int ret;
    for (int i = 0; i < 3; i++) {
        ret = pthread_create(&consumer_tid[i], NULL, consumer, queue);
        if(ret != 0 ){
            fprintf(stderr,"consumer pthread_create error %s",strerror(ret));
            exit(1);
        }
    }
    for (int i = 0; i < 5; i++) {
        ret = pthread_create(&producer_tid[i], NULL, producer, queue);
        if(ret != 0 ){
            fprintf(stderr,"producer pthread_create error %s",strerror(ret));
            exit(1);

        }
    }
    for (int i = 0; i < 3; i++) {
        ret = pthread_join(consumer_tid[i], NULL);
        if(ret != 0 ){
            fprintf(stderr,"consumer pthread_join error %s",strerror(ret));
            exit(1);

        }
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(producer_tid[i], NULL);
        if(ret != 0 ){
            fprintf(stderr,"producer pthread_join error %s",strerror(ret));
        }
    }
    SPSCQueueDestroy(queue);
}