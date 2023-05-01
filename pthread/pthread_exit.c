#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void* tfn(void* arg) {
    int i = (int)arg;
    sleep(i);
    if (i == 2) {
        // exit(0)退出进程
        // return NULL 返回调用函数
        pthread_exit(NULL);//退出当前线程
    }
    printf("%dth thread pid: %d tid: %lu\n", (int)arg + 1, getpid(),
           pthread_self());
}
int main() {
    pthread_t tid;
    int ret;
    int i;
    for (i = 0; i < 5; i++) {
        ret = pthread_create(&tid, NULL, tfn,
                             (void*)i);  // 发生用户态与内核态转变
        if (ret != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    printf("main pid:%d tid:%lu\n", getpid(), pthread_self());
    pthread_exit((void*)0);//退出主线程
    //return 0;退出进程
}