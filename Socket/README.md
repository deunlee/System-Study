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

다음은 while 반복문 안에서 클라이언트를 수락하고, 메시지를 주고 받는 서버이다.
클라이언트가 잘 동작한다면, 서버도 잘 동작하는 것 처럼 보인다.

```
$ gcc -o server_loop server_loop.c && ./server_loop
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57788)
[Server] Client says: Hi! I'm client and my pid is 9945.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57790)
[Server] Client says: Hi! I'm client and my pid is 9946.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57792)
[Server] Client says: Hi! I'm client and my pid is 9947.

[Server] Waiting for a client...
```

```
$ gcc -o client client.c
$ ./client & ./client & ./client 
[1] 9945
[Client] (9945) Connected to the server. (127.0.0.1:55555)
[Client] (9945) Server says: Hello~ I'm a server!
[Client] (9945) Sent a message to the server.
[2] 9946
[Client] (9946) Connected to the server. (127.0.0.1:55555)
[Client] (9947) Connected to the server. (127.0.0.1:55555)
[1]  - 9945 done       ./client
[Client] (9946) Server says: Hello~ I'm a server!
[Client] (9946) Sent a message to the server.
[2]  + 9946 done       ./client
[Client] (9947) Server says: Hello~ I'm a server!
[Client] (9947) Sent a message to the server.
```

However, if there is a problem with the first client, the next connection will also have problems.
If the -s option is given to the client, it will intentionally stop for 10 seconds.
In the result below, you can see that the server is stopped because of the slow client and next clients are also stopped.

하지만 첫 번째 클라이언트에 문제가 발생하면, 다음 연결에도 문제가 발생한다.
이를 테스트하기 위해 클라이언트에 -s 옵션이 주어진 경우 고의적으로 10초간 멈추도록 하였다.
아래 결과를 보면 느린 클라이언트 때문에 서버가 멈추고, 다음 클라이언트도 멈춘 것을 볼 수 있다.

```diff
$ gcc -o server_loop server_loop.c && ./server_loop
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57800)
-Stuck here for 10 seconds because of a slow client.
[Server] Client says: Hi! I'm client and my pid is 10094.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57802)
[Server] Client says: Hi! I'm client and my pid is 10095.

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57804)
[Server] Client says: Hi! I'm client and my pid is 10096.

[Server] Waiting for a client...
```

```diff
$ gcc -o client client.c
$ ./client -s & ./client & ./client
[Client] (10094) Connected to the server. (127.0.0.1:55555)
[1] 10094
[Client] (10094) Server says: Hello~ I'm a server!
[Client] (10094) I'm going to sleep...
[2] 10095
[Client] (10095) Connected to the server. (127.0.0.1:55555)
[Client] (10096) Connected to the server. (127.0.0.1:55555)
-Stuck here for 10 seconds because of a slow client.
[Client] (10094) Sent a message to the server.
[Client] (10095) Server says: Hello~ I'm a server!
[Client] (10095) Sent a message to the server.
[1]  - 10094 done       ./client -s
[2]  + 10095 done       ./client
[Client] (10096) Server says: Hello~ I'm a server!
[Client] (10096) Sent a message to the server.
```


## 04. Fork Server
In a multiprocess-based server, the mentioned problem does not occur. 
Even if there is a problem with the client with PID 12000, only the child process of the server with PID 12002 that handles it is delayed.
As the parent process operates normally, the server can continue to accept other clients. 

멀티 프로세스 기반 서버에서는 위에서 언급한 문제가 발생하지 않는다.
PID가 12000인 클라이언트에 문제가 생겨도 그것을 처리하는 PID가 12002인 서버의 자식 프로세스만 지연된다.
부모 프로세스가 정상 동작하므로 서버는 다른 클라이언트를 계속 받을 수 있다.

```
$ gcc -o server_fork server_fork.c && ./server_fork
[Server] Server is running at 0.0.0.0:55555
[Child] (PID=12002) Forked successfully.
[Child] (PID=12002) Client is connected. (127.0.0.1:57870)
[Child] (PID=12004) Forked successfully.
[Child] (PID=12004) Client is connected. (127.0.0.1:57872)
[Server] Client says: Hi! I'm client and my pid is 12001.
[Server] Received a signal: User defined signal 1
[Server] Child (PID=12004) returned: 0
[Child] (PID=12005) Forked successfully.
[Child] (PID=12005) Client is connected. (127.0.0.1:57874)
[Server] Client says: Hi! I'm client and my pid is 12003.
[Server] Received a signal: User defined signal 1
[Server] Child (PID=12005) returned: 0
[Server] Client says: Hi! I'm client and my pid is 12000.
[Server] Received a signal: User defined signal 1
[Server] Child (PID=12002) returned: 0
```

```
$ gcc -o client client.c
$ ./client -s & ./client & ./client
[Client] (12000) Connected to the server. (127.0.0.1:55555)
[1] 12000
[Client] (12001) Connected to the server. (127.0.0.1:55555)
[2] 12001
[Client] (12000) Server says: Hello~ I'm a server!
[Client] (12000) I'm going to sleep...
[Client] (12001) Server says: Hello~ I'm a server!
[Client] (12001) Sent a message to the server.
[2]  + 12001 done       ./client
[Client] (12003) Connected to the server. (127.0.0.1:55555)
[Client] (12003) Server says: Hello~ I'm a server!
[Client] (12003) Sent a message to the server.
[Client] (12000) Sent a message to the server.
[1]  + 12000 done       ./client -s
```


## 05. Thread Server

