#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void* tfn(void* arg) {
    printf("thread: pid=%d tid=%lu\n", getpid(),
           pthread_self());  // thread: pid=12912 tid=140685636388544
    return NULL;
}
int main() {
    pthread_t tid;
    printf("pid: %d tid:%lu\n", getpid(),
           pthread_self());  // pid: 12912 tid:140685638399616
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_init error:%s\n", strerror(ret));
        exit(1);
    }
    ret = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);//设置线程属性为分离属性
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_setdetachstate error:%s\n", strerror(ret));
        exit(1);
    }
    ret = pthread_create(&tid, &attr, tfn, NULL);  // 成功返回0,失败返回errno
    if (ret != 0) {
        fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
        exit(1);
    }
    ret = pthread_attr_destroy(&attr);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_destroy error:%s\n", strerror(ret));
        exit(1);
    }
    // sleep(1);  // return 后销毁共用的内存
    ret = pthread_join(tid,NULL);
    if (ret != 0) {
        fprintf(stderr, "pthread_join error:%s\n", strerror(ret));
        exit(1);
    }//回收失败说明处于分离态
    printf("main pid:%d tid:%d",getpid(),pthread_self());
    return 0;
}