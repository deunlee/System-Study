# Thread-Study

## [counting.c](./counting.c)
```
$ gcc -pthread -o counting counting.c && ./counting 4
Thread Count: 4

|<TH 0>|<TH 1>|<TH 2>|<TH 3>|
|      |      |      | 1... |
|      |      | 1... |      |
|      | 1... |      |      |
| 1... |      |      |      |
|      |      | 2... |      |
|      | 2... |      |      |
|      |      |      | 2... |
| 2... |      |      |      |
|      |      | 3... |      |
|      | 3... |      |      |
|      |      |      | 3... |
| 3... |      |      |      |
|      |      | 4... |      |
|      | 4... |      |      |
|      |      |      | 4... |
| 4... |      |      |      |
|      |      |FINISH|      |
|      |FINISH|      |      |
|      |      |      |FINISH|
|FINISH|      |      |      |
Thread #0 returned 0
Thread #1 returned 1
Thread #2 returned 2
Thread #3 returned 3
```


## Multi-Threaded Server
See [../Socket/05_Thread_Server](../Socket/05_Thread_Server/server_thread.c)
