# Network-Study

## [interface_addr.c](./interface_addr.c)
네트워크 인터페이스별 IP 주소 출력

**`gcc -o interface_addr interface_addr.c && ./interface_addr`**
```
==================== IPv4 ====================
Interface: lo               Address: 127.0.0.1
Interface: ens192           Address: 10.10.0.80
Interface: docker0          Address: 172.17.0.1
Interface: br-62649fd23ba1  Address: 172.27.0.1

==================== IPv6 ====================
Interface: lo               Address: ::1
Interface: ens192           Address: fe80::xxxx:xxxx:xxxx:xxxx
Interface: docker0          Address: fe80::42:b4ff:fe4a:4717
Interface: br-62649fd23ba1  Address: fe80::42:77ff:fefc:d789
```

**`ifconfig`**
```
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
...

ens192: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.10.0.80  netmask 255.255.0.0  broadcast 10.10.255.255
        inet6 fe80::xxxx:xxxx:xxxx:xxxx  prefixlen 64  scopeid 0x20<link>
        ether xx:xx:xx:xx:xx:xx  txqueuelen 1000  (Ethernet)
...

docker0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.17.0.1  netmask 255.255.0.0  broadcast 172.17.255.255
        inet6 fe80::42:b4ff:fe4a:4717  prefixlen 64  scopeid 0x20<link>
        ether 02:42:b4:4a:47:17  txqueuelen 0  (Ethernet)
...

br-62649fd23ba1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.27.0.1  netmask 255.255.0.0  broadcast 172.27.255.255
        inet6 fe80::42:77ff:fefc:d789  prefixlen 64  scopeid 0x20<link>
        ether 02:42:77:fc:d7:89  txqueuelen 0  (Ethernet)
...
```

<br>

## [interface_info.c](./interface_info.c)
네트워크 인터페이스별 자세한 정보 출력

**`gcc -o interface_info interface_info.c && ./interface_info`**
```
lo: <LOOPBACK,UP,LOWER_UP>
    Address     127.0.0.1
    Netmask     255.0.0.0
    Broadcast   127.0.0.1

    Address     ::1
    Netmask     ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff

ens192: <BROADCAST,MULTICAST,UP,LOWER_UP>
    Address     10.10.0.80
    Netmask     255.255.0.0
    Broadcast   10.10.255.255

    Address     fe80::xxxx:xxxx:xxxx:xxxx
    Netmask     ffff:ffff:ffff:ffff::

docker0: <BROADCAST,MULTICAST,UP>
    Address     172.17.0.1
    Netmask     255.255.0.0
    Broadcast   172.17.255.255

    Address     fe80::42:b4ff:fe4a:4717
    Netmask     ffff:ffff:ffff:ffff::

br-62649fd23ba1: <BROADCAST,MULTICAST,UP,LOWER_UP>
    Address     172.27.0.1
    Netmask     255.255.0.0
    Broadcast   172.27.255.255

    Address     fe80::42:77ff:fefc:d789
    Netmask     ffff:ffff:ffff:ffff::
```

**`ip addr`**
```                                               
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: ens192: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP group default qlen 1000
    link/ether xx:xx:xx:xx:xx:xx brd ff:ff:ff:ff:ff:ff
    inet 10.10.0.80/16 brd 10.10.255.255 scope global dynamic noprefixroute ens192
       valid_lft 80239sec preferred_lft 80239sec
    inet6 fe80::xxxx:xxxx:xxxx:xxxx/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
3: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:b4:4a:47:17 brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
       valid_lft forever preferred_lft forever
    inet6 fe80::42:b4ff:fe4a:4717/64 scope link 
       valid_lft forever preferred_lft forever
237: br-62649fd23ba1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default 
    link/ether 02:42:77:fc:d7:89 brd ff:ff:ff:ff:ff:ff
    inet 172.27.0.1/16 brd 172.27.255.255 scope global br-62649fd23ba1
       valid_lft forever preferred_lft forever
    inet6 fe80::42:77ff:fefc:d789/64 scope link 
       valid_lft forever preferred_lft forever
```
