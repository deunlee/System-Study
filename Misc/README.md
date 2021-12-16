# System-Study :: Misc

## [errno_and_perror.c](./errno_and_perror.c)
```
$ gcc -o errno_and_perror errno_and_perror.c && ./errno_and_perror  
errno = 2
error message: No such file or directory
```


## [syscall_pid.c](./syscall_pid.c)
```
$ gcc -o syscall_pid syscall_pid.c && ./syscall_pid
PID: 25152 by getpid()
PID: 25152 by syscall(39)
```
