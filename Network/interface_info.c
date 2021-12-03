#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <net/if.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#ifndef IF_NAMESIZE
#define IF_NAMESIZE IFNAMSIZ
#endif

struct addr {
    char addr4 [INET_ADDRSTRLEN];  // 16
    char addr6 [INET6_ADDRSTRLEN]; // 46
};

struct if_info {
    char name [IF_NAMESIZE]; // 16
    unsigned int flags;
    struct addr ip;
    struct addr mask;
    struct addr broad;
};

void set_ip_str(struct sockaddr* src, struct addr* dest) {
    switch (src->sa_family) {
    case AF_INET:
        inet_ntop(AF_INET, &((struct sockaddr_in*)src)->sin_addr, dest->addr4, INET_ADDRSTRLEN);
        break;
    case AF_INET6:
        inet_ntop(AF_INET6, &((struct sockaddr_in6*)src)->sin6_addr, dest->addr6, INET6_ADDRSTRLEN);
        break;
    }
}

void print_flags(unsigned int flags) {
    int i, prev = 0;
    const unsigned int values[] = { 
        IFF_BROADCAST,  IFF_MULTICAST, IFF_LOOPBACK, IFF_POINTOPOINT, 
        IFF_NOTRAILERS, IFF_NOARP,     IFF_PROMISC,  IFF_PORTSEL, 
        IFF_AUTOMEDIA,  IFF_DYNAMIC,   IFF_MASTER,   IFF_SLAVE,
        IFF_UP,         IFF_LOWER_UP,  IFF_DORMANT,  IFF_ECHO, // IFF_RUNNING
        IFF_ALLMULTI,   IFF_DEBUG
    }; // https://man7.org/linux/man-pages/man7/netdevice.7.html
    const char* names[] = {
        "BROADCAST",    "MULTICAST",   "LOOPBACK",   "POINTOPOINT",
        "NOTRAILERS",   "NOARP",       "PROMISC",    "PORTSEL",
        "AUTOMEDIA",    "DYNAMIC",     "MASTER",     "SLAVE",
        "UP",           "LOWER_UP",    "DORMANT",    "ECHO", // "RUNNING"
        "ALLMULTI",     "DEBUG"
    };
    for (i = 0; i < sizeof(values) / sizeof(unsigned int); i++) {
        if (flags & values[i]) {
            if (prev) printf(",");
            printf("%s", names[i]);
            prev = 1;
        }
    }
}

int main() {
    struct if_info info[128] = { 0, };
    int i, cnt = 0;

    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in* sa;
    struct sockaddr_in6* sa6;
    char buffer[INET6_ADDRSTRLEN]; // 46 bytes for IPv6

    if (getifaddrs(&ifap) != 0) {
        printf("Failed to get interface addresses.\n");
        return 1;
    }

    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        for (i = 0; i < cnt; i++) {
            if (strcmp(info[i].name, ifa->ifa_name) == 0) break;
        }
        if (i == cnt) {
            if (cnt == sizeof(info) / sizeof(struct if_info)) {
                printf("Too many interfaces.\n");
                return 1;
            }
            strncpy(info[cnt++].name, ifa->ifa_name, IF_NAMESIZE);
            info[i].flags = ifa->ifa_flags;
        }

        if (ifa->ifa_addr) {
            set_ip_str(ifa->ifa_addr, &info[i].ip);
        }
        if (ifa->ifa_netmask) {
            set_ip_str(ifa->ifa_netmask, &info[i].mask);
        }
        if (ifa->ifa_ifu.ifu_broadaddr) {
            set_ip_str(ifa->ifa_ifu.ifu_broadaddr, &info[i].broad);
        }
    }

    freeifaddrs(ifap);

    for (i = 0; i < cnt; i++) {
        printf("%s: <", info[i].name);
        print_flags(info[i].flags);
        printf(">\n");

        if (info[i].ip.addr4[0]) {
            printf("    Address     %s\n", info[i].ip.addr4);
            if (info[i].mask.addr4[0]) {
                printf("    Netmask     %s\n", info[i].mask.addr4);
            }
            if (info[i].broad.addr4[0]) {
                printf("    %s %s\n", info->flags & IFF_POINTOPOINT ? "Destination" : "Broadcast  ", info[i].broad.addr4);
            }
            printf("\n");
        }

        if (info[i].ip.addr6[0]) {
            printf("    Address     %s\n", info[i].ip.addr6);
            if (info[i].mask.addr6[0]) {
                printf("    Netmask     %s\n", info[i].mask.addr6);
            }
            if (info[i].broad.addr6[0]) {
                printf("    %s %s\n", info->flags & IFF_POINTOPOINT ? "Destination" : "Broadcast  ", info[i].broad.addr6);
            }
            printf("\n");
        }
    }

    return 0;
}
