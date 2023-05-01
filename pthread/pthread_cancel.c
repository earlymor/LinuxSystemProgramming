#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void* tfn(void* arg) {
    int i = (int)arg;
    while (1) {
        // sleep(1);
        // printf("%dth thread pid: %d tid: %lu\n", (int)arg + 1, getpid(),
        //        pthread_self());
        pthread_testcancel();/* 手动设置取消点 */
    }
    return (void*)666;
}
int main() {
    pthread_t tid;
    int ret;
    void* tret=NULL;
    int i;
    ret = pthread_create(&tid, NULL, tfn,
                         (void*)i);  // 发生用户态与内核态转变
    if (ret != 0) {
        perror("pthread_create");
        exit(1);
    }
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid,&tret);//回收线程，传出返回值tret
    printf("pthread ret:%d\n",(int)tret);
    printf("main pid:%d tid:%lu\n", getpid(), pthread_self());
    pthread_exit((void*)0);  // 退出主线程
    // return 0;退出进程
}