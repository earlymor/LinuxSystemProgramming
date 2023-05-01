# LinuxSystemProgramming

## **线程**

### 一、线程是什么

LWP（轻量级线程)

### 二、线程创建

> pthread_create - create a new thread

```c
 #include <pthread.h>

       int pthread_create(pthread_t *restrict thread,//传出参数
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);

       Compile and link with -pthread.
```

### 三、线程终止、回收

> pthread_exit - terminate calling thread

<pre><b>#include < pthread.h > </b>

       <b>void pthread_exit(void *</b><i>retval</i><b>);</b>

       Compile and link with <i>-pthread</i>.</pre>

> pthread_cancel - send a cancellation request to a thread


<pre><b>#include <pthread.h></b>

       <b>int pthread_cancel(pthread_t </b><i>thread</i><b>);</b>

       Compile and link with <i>-pthread</i>.</pre>

pthread_testcancel()/* *手动设置取消点* */

> pthread_join - join with a terminated thread// 回收


<pre><b>#include <pthread.h></b>

       <b>int pthread_join(pthread_t </b><i>thread</i><b>, void **</b><i>retval</i><b>);</b>

       Compile and link with <i>-pthread</i>.
</pre>

返回-1表示线程异常退出

### 四、线程分离

### 五、注意事项

### 六、线程同步

### 七、生产者消费者模型
