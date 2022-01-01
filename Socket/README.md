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

```
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57788)
[Server] Client says: Hi! I'm client and my pid is 9945!!

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57790)
[Server] Client says: Hi! I'm client and my pid is 9946!!

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57792)
[Server] Client says: Hi! I'm client and my pid is 9947!!

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

```diff
$ gcc -o server server.c && ./server
[Server] Server is running at 0.0.0.0:55555
[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57800)
-Stuck here for 10 seconds because of a slow client.
[Server] Client says: Hi! I'm client and my pid is 10094!!

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57802)
[Server] Client says: Hi! I'm client and my pid is 10095!!

[Server] Waiting for a client...
[Server] Client is connected. (127.0.0.1:57804)
[Server] Client says: Hi! I'm client and my pid is 10096!!

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


## 05. Thread Server

