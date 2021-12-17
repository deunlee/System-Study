# System-Study :: Process

## [pid_and_name.c](./pid_and_name.c)
```
$ gcc -o pid_and_name pid_and_name.c && ./pid_and_name
PID=22755 (./pid_and_name)
PPID=26506 (/usr/bin/bash)
```


## [execlp.c](./execlp.c) / [execv.c](./execv.c) / [execve.c](./execve.c)
```
$ gcc -o execlp execlp.c && ./execlp
Before exec function.
.  ..  execlp  execlp.c
```

```
$ gcc -o execv execv.c && ./execv
Before exec function.
.  ..  execv  execv.c
```

```
$ gcc -o execve execve.c && ./execve
Before exec function.
MY_NAME=DEUN_LEE
```


## [fork.c](./fork.c)
```
$ gcc -o fork fork.c && ./fork
[Parent] Forked successfully. (PID=23265)
[Parent] I am your father.
[Child] Forked successfully. (PID=23266, PPID=23265)
[Child] I am a child.
[Parent] Child returned 123.
```


## [fork_memory.c](./fork_memory.c)
부모 프로세스와 자식 프로세스는 동일한 주소의 가상 메모리 공간을 갖지만,
한 쪽에서 내용을 변경할 경우 다른 쪽에 영향을 미치지 않는다.

The parent and child processes have the same virtual memory space at the same address, 
but changing value on one side does not affect the other side.

```
$ gcc -o fork_memory fork_memory.c && ./fork_memory
INIT: Default memory value. (global=DATA, local=STACK, dynamic=HEAP)
[Parent] global=10 (0x0x601060), local=20 (0x0x7ffd916ea268), dynamic=30 (0x0x14c12a0)

FIRST: After fork, child process modifies memory.
[Child]  global=11 (0x0x601060), local=21 (0x0x7ffd916ea268), dynamic=31 (0x0x14c12a0)
[Parent] global=10 (0x0x601060), local=20 (0x0x7ffd916ea268), dynamic=30 (0x0x14c12a0)

SECOND: After fork, parent process modifies memory.
[Parent] global=11 (0x0x601060), local=21 (0x0x7ffd916ea268), dynamic=31 (0x0x14c12a0)
[Child]  global=10 (0x0x601060), local=20 (0x0x7ffd916ea268), dynamic=30 (0x0x14c12a0)
```


## [fork_file.c](./fork_file.c)
부모 프로세스와 자식 프로세스는 파일 기술자를 공유하기 때문에 자식 프로세스에서는 파일을 다시 열지 않아도 접근이 가능하다.
(f)printf 함수는 성능 향상을 목적으로 내부적으로 버퍼를 사용한다.
파일에 기록되지 않고 버퍼에 남아있었던 "Before fork" 문자열이 fork()하는 과정에서
자식 프로세스에게도 복사되어 두 번 출력된 것을 볼 수 있다.

The parent and child processes share a file descriptor.
Therefore, the child can access the file without reopening it.
(f)printf function uses a buffer internally to improve performance.
In the process of fork()ing "Before fork" string that was not written to the file and remained in the buffer 
copied to the child process and printed twice.

```
$ gcc -o fork_file fork_file.c && ./fork_file
$ cat test.txt                               
Before fork
I'm a child.
Before fork
I'm a parent.
```

만약 fflush()를 호출해서 버퍼를 비운 뒤 fork()하면 부모 프로세스의 버퍼가 비워진 채로
자식 프로세스가 생성되어 의도한 대로 작동하는 것을 볼 수 있다.

After calling fflush() to empty the buffer, fork() creates a child with empty buffer 
and you can see the output as intended.

```
$ gcc -o fork_file fork_file.c && ./fork_file --fflush 
$ cat test.txt                                        
Before fork
I'm a child.
I'm a parent.
```
