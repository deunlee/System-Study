# System-Study :: Socket

## 01. Basic TCP
```
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] Client is connected. (127.0.0.1:57836) (listen fd=3, accept fd=4)
[Server] Sent a message to the client.
[Server] Received: Hello from the client~
```

```
$ gcc -o client client.c && ./client
[Client] Connected to 127.0.0.1:55555 (sockfd=3)
[Client] Received: Hello~ This is the server!!
[Client] Sent a message to the server.
```

```
$ netstat -anp | grep 55555
tcp        0      0 0.0.0.0:55555      0.0.0.0:*       LISTEN      2569/./server
```


## 02. Basic UDP
```
$ gcc -o server server.c && ./server
[Server] Received: Hello from the client~
[Server] Client Info: 127.0.0.1:48845
[Server] Sent a message to the client.
```

```
$ gcc -o client client.c && ./client
[Client] Sent a message to the server.
[Client] Received: Hello~ This is the server!!
```

```
$ netstat -anp | grep 55555
udp        0      0 0.0.0.0:55555      0.0.0.0:*                   2573/./server
```


## 03. Loop Server
A server that accepts a connection from a client and sends and receives messages in a while loop.
If the client works fine, the server looks fine.

다음은 while 반복문 안에서 클라이언트를 수락하고 메시지를 주고 받는 서버입니다.
클라이언트가 잘 동작한다면, 서버도 잘 동작하는 것 처럼 보입니다.

```
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57788)
[Server] Client says: Hi! I'm client and my pid is 1005.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57790)
[Server] Client says: Hi! I'm client and my pid is 1006.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57792)
[Server] Client says: Hi! I'm client and my pid is 1007.

[Server] Waiting for a client...
```

```
$ gcc -o client client.c
$ ./client & ./client & ./client 
[1] 1005
[Client] (1005) Connected to the server. (127.0.0.1:55555)
[Client] (1005) Server says: Hello~ I'm a server!
[Client] (1005) Sent a message to the server.
[2] 1006
[Client] (1006) Connected to the server. (127.0.0.1:55555)
[Client] (1007) Connected to the server. (127.0.0.1:55555)
[1]  - 1005 done       ./client
[Client] (1006) Server says: Hello~ I'm a server!
[Client] (1006) Sent a message to the server.
[2]  + 1006 done       ./client
[Client] (1007) Server says: Hello~ I'm a server!
[Client] (1007) Sent a message to the server.
```

However, if there is a problem with the first client, the next connection will also have problems.
If the -s option is given to the client, it will intentionally stop for 10 seconds.
In the result below, you can see that the server is stopped because of the slow client and next clients are also stopped.

하지만 첫 번째 클라이언트에 문제가 발생하면, 다음 연결에도 문제가 발생합니다.
이를 테스트하기 위해 클라이언트에 -s 옵션이 주어진 경우 고의적으로 10초간 멈추도록 하였습니다.
아래 결과를 보면 느린 클라이언트 때문에 서버가 멈추고 다음 클라이언트도 멈춘 것을 볼 수 있습니다.

```diff
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57800)
-Stuck here for 10 seconds because of a slow client.
[Server] Client says: Hi! I'm client and my pid is 1004.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57802)
[Server] Client says: Hi! I'm client and my pid is 1005.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57804)
[Server] Client says: Hi! I'm client and my pid is 1006.

[Server] Waiting for a client...
```

```diff
$ gcc -o client client.c
$ ./client -s & ./client & ./client
[Client] (1004) Connected to the server. (127.0.0.1:55555)
[1] 1004
[Client] (1004) Server says: Hello~ I'm a server!
[Client] (1004) I'm going to sleep...
[2] 1005
[Client] (1005) Connected to the server. (127.0.0.1:55555)
[Client] (1006) Connected to the server. (127.0.0.1:55555)
-Stuck here for 10 seconds because of a slow client.
[Client] (1004) Sent a message to the server.
[Client] (1005) Server says: Hello~ I'm a server!
[Client] (1005) Sent a message to the server.
[1]  - 1004 done       ./client -s
[2]  + 1005 done       ./client
[Client] (1006) Server says: Hello~ I'm a server!
[Client] (1006) Sent a message to the server.
```


## 04. Fork Server
In a multiprocess-based server, mentioned problem does not occur. 
For example, there is a problem with a client, 
only the child process of the server that handles it is delayed.
As the parent process operates normally, the server can continue to accept other clients. 

멀티 프로세스 기반 서버에서는 위에서 언급한 문제가 발생하지 않습니다.
예를 들어, 어떤 클라이언트에 문제가 생겨도 그것을 처리하는 서버의 자식 프로세스 하나만 지연됩니다.
부모 프로세스가 정상 동작하므로 서버는 다른 클라이언트를 계속 받을 수 있습니다.

```
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Child] (PID=1004) Forked successfully.
[Child] (PID=1004) Client is connected. (127.0.0.1:46988)
[Child] (PID=1005) Forked successfully.
[Child] (PID=1005) Client is connected. (127.0.0.1:46990)
[Child] (PID=1005) Client says: Hi! I'm client and my pid is 1003.
[Server] Received signal: Child exited
[Server] (PID=1005) Child returned: 0
[Child] (PID=1007) Forked successfully.
[Child] (PID=1007) Client is connected. (127.0.0.1:46992)
[Child] (PID=1007) Client says: Hi! I'm client and my pid is 1006.
[Server] Received signal: Child exited
[Server] (PID=1007) Child returned: 0
[Child] (PID=1004) Client says: Hi! I'm client and my pid is 1002.
[Server] Received signal: Child exited
[Server] (PID=1004) Child returned: 0
```

```
$ gcc -o client client.c
$ ./client -s & ./client & ./client
[Client] (1002) Connected to the server. (127.0.0.1:55555)
[1] 1002
[Client] (1002) Server says: Hello~ I'm a server!
[Client] (1002) I'm going to sleep...
[2] 1003
[Client] (1003) Connected to the server. (127.0.0.1:55555)
[Client] (1003) Server says: Hello~ I'm a server!
[Client] (1003) Sent a message to the server.
[2]  + 1003 done       ./client
[Client] (1006) Connected to the server. (127.0.0.1:55555)
[Client] (1006) Server says: Hello~ I'm a server!
[Client] (1006) Sent a message to the server.
[Client] (1002) Sent a message to the server.
[1]  + 1002 done       ./client -s
```


## 05. Thread Server
A thread-based server works like a multiprocess-based server.
Using threads is efficient because of less context switching, but if one thread fails, the entire program can be affected.

스레드 기반 서버도 위의 멀티프로세스 기반 서버와 같이 동작합니다.
자식 프로세스 대신 스레드를 사용하면 컨텍스트 스위칭이 적게 발생하므로 효율적이지만, 
하나의 스레드에 문제가 생기면 전체 프로그램에도 영향을 줄 수 있다는 단점이 있습니다.

```
$ gcc -pthread -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Thread] (TID=139807916951296) Client is connected. (127.0.0.1:46994)
[Thread] (TID=139807908558592) Client is connected. (127.0.0.1:46996)
[Thread] (TID=139807908558592) Client says: Hi! I'm client and my pid is 1006.
[Thread] (TID=139807908558592) Client is connected. (127.0.0.1:46998)
[Thread] (TID=139807908558592) Client says: Hi! I'm client and my pid is 1009.
[Thread] (TID=139807916951296) Client says: Hi! I'm client and my pid is 1005.
```

```
$ gcc -o client client.c
$ ./client -s & ./client & ./client
[1] 1005
[Client] (1005) Connected to the server. (127.0.0.1:55555)
[Client] (1005) Server says: Hello~ I'm a server!
[Client] (1005) I'm going to sleep...
[2] 1006
[Client] (1006) Connected to the server. (127.0.0.1:55555)
[Client] (1006) Server says: Hello~ I'm a server!
[Client] (1006) Sent a message to the server.
[2]  + 1006 done       ./client
[Client] (1009) Connected to the server. (127.0.0.1:55555)
[Client] (1009) Server says: Hello~ I'm a server!
[Client] (1009) Sent a message to the server.
[Client] (1005) Sent a message to the server.
[1]  + 1005 done       ./client -s
```


## 06. File Transfer

