# makefile

## 规则

目标：依赖

例如：

```shell
src = $(wildcard *.c) # add.c hello.c
obj = $(patsubst %.c, %.o, %(src)) # add.o hello.o
ALL:a.out
 
a.out:hello.o add.o 
//table	gcc hello.o add.o -o a.out
hello.o:hello.c
	gcc -c hello.c -o hello.o

add.o:add.c
	gcc -c hello.c -o hello.o

clean:
	-rm -rf $(obj)a.out
```

make clean -n //预览命令不执行

简化后：

```shell
src = $(wildcard *.c) # add.c hello.c
obj = $(patsubst %.c, %.o, %(src)) # add.o hello.o
ALL:a.out

a.out:$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $< -o $@
clean:
	-rm -rf $(obj)a.out
```
