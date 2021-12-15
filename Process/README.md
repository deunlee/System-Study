# System-Study :: Process

## [pid_and_name.c](./pid_and_name.c)
**`gcc -o pid_and_name pid_and_name.c && ./pid_and_name`**
```
PID=22755 (./pid_and_name)
PPID=26506 (/usr/bin/bash)
```


## [simple_fork.c](./simple_fork.c)
**`gcc -o simple_fork simple_fork.c && ./simple_fork`**
```
[Parent] Forked successfully. (PID=23265)
[Parent] I am your father.
[Child] Forked successfully. (PID=23266, PPID=23265)
[Child] I am a child.
[Parent] Child returned 123.
```

