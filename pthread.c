#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void* tfn(void*arg){
    printf("thread: pid=%d tid=%lu\n",getpid(),pthread_self());//thread: pid=12912 tid=140685636388544
    return NULL;
}
int main(){
    pthread_t tid;
    printf("pid: %d tid:%lu\n",getpid(),pthread_self());//pid: 12912 tid:140685638399616
    int ret=pthread_create(&tid,NULL,tfn,NULL);//成功返回0,失败返回errno
    if(ret!=0){
        perror("pthread_create:");
    }
    sleep(1);//return 后销毁共用的内存
    return 0;
}