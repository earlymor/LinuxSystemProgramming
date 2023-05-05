#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 5               // 每个线程执行MAX次
#define NUM_PHILOSOPHERS 5  // 哲学家的数量
#define THINKING 0                                   // 哲学家正在思考
#define HUNGRY 1  // 哲学家饿了，但还没有拿到叉子
#define EATING 2  // 哲学家正在吃饭

int state[NUM_PHILOSOPHERS];    // 哲学家的状态数组
pthread_mutex_t mutex;

void sys_error(int ret, const char* string) {
    if (ret != 0) {
        fprintf(stderr, "%s error %s", string, strerror(ret));
        exit(1);
    }
}
void thinking(void* ph_num) {
    printf("----哲学家%d  正在思考问题\n", (int)ph_num);
}
void hungry(void* ph_num) {
    printf("----哲学家%d  饿了\n", (int)ph_num);
}
void eating(void* ph_num) {
    printf("----哲学家%d  吃饭\n", (int)ph_num);
    sleep(1);
}

void* philosopher(void* ph_num) {
    int left = (int)ph_num % NUM_PHILOSOPHERS;
    int right = (int)(ph_num + 1) % NUM_PHILOSOPHERS;
    for (int i = 0; i < MAX; i++) {
        thinking(ph_num);
        hungry(ph_num);
        pthread_mutex_lock(&mutex);
        printf("----哲学家%d  拿起了左筷%d\n", left, left);

        printf("----哲学家%d  拿起了右筷%d\n", left, right);

        eating(ph_num);
        pthread_mutex_unlock(&mutex);

        printf("----哲学家%d  放下了左筷%d\n", left, left);

        printf("----哲学家%d  放下了右筷%d\n", left, right);
    }
        printf("----哲学家%d  is over\n", left, left);

}

int main() {
    pthread_t tid[NUM_PHILOSOPHERS];
    int ret;
    pthread_mutex_init(&mutex,NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {  // 创建哲学家线程
        ret = pthread_create(&tid[i], NULL, philosopher, (void*)i);
        sys_error(ret, "pthread_create");
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {  // 回收线程
        ret = pthread_join(tid[i], NULL);
        sys_error(ret, "pthread_join");
        printf("----哲学家%d  is destroyed\n",  i);
    }
    
}