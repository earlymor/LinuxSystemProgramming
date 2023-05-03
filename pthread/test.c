#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 10

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *arg) {
    int i;
    for (i = 0; i < MAX; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        put(i);
        printf("-producer:%d\n",i);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *arg) {
    int i;
    for (i = 0; i < MAX; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int tmp = get();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        printf("---consumer:%d\n", tmp);
    }
}

int main(int argc, char *argv[]) {
    pthread_t tid1, tid2;

    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid2, NULL, consumer, NULL);
    pthread_create(&tid1, NULL, producer, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    return 0;
}