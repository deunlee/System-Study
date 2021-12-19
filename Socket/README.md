# System-Study :: Socket

## 01. Basic TCP
```
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] The client is connected. (127.0.0.1:38346) (listen fd=3, accept fd=4)
[Server] Received: hello from client~
```

```
$ gcc -o client client.c && ./client
[Client] Connected to 127.0.0.1:55555 (sockfd=3)
[Client] Received: hello this is server!!
```

```
$ netstat -anp | grep 55555
tcp        0      0 0.0.0.0:55555      0.0.0.0:*       LISTEN      2569/./server
```


## 02. Basic UDP
```
$ gcc -o server server.c && ./server
[Server] Received: hello from client~
[Server] Client Info: 127.0.0.1:56467
[Server] Sent a message to the client.
```

```
$ gcc -o client client.c && ./client
[Client] Sent a message to the server.
[Client] Received: hello this is server!!
```

```
$ netstat -anp | grep 55555
udp        0      0 0.0.0.0:55555      0.0.0.0:*                   2573/./server
```


## 03. Loop Server


## 04. Fork Server


## 05. Thread Server

