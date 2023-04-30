#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void* tfn(void* arg)
{
    int i = (int)arg;
    printf("%dth thread pid: %d tid: %lu\n", (int)arg + 1, getpid(),
           pthread_self());
    if (i == 2) {
        pthread_exit(NULL);
    }
}
int main()
{
    pthread_t tid;
    int       ret;
    int       i;
    for (i = 0; i < 5; i++) {
        ret = pthread_create(&tid, NULL, tfn,
                             (void*)i);  // 发生用户态与内核态转变
        if (ret != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    sleep(i);
    printf("main pid:%d tid:%lu\n", getpid(), pthread_self());
    return 0;
}