#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>

int main() {
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in* sa;
    struct sockaddr_in6* sa6;
    char buffer[INET6_ADDRSTRLEN]; // 46 bytes for IPv6

    if (getifaddrs(&ifap) != 0) {
        printf("Failed to get interface addresses.\n");
        return 1;
    }

    printf("==================== IPv4 ====================\n");
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in*)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), buffer, sizeof(buffer));
            printf("Interface: %-16s Address: %s\n", ifa->ifa_name, buffer);
            // printf("Interface: %-16s Address: %s\n", ifa->ifa_name, inet_ntoa(sa->sin_addr));
        }
    }

    printf("\n==================== IPv6 ====================\n");
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET6) {
            sa6 = (struct sockaddr_in6*)ifa->ifa_addr;
            inet_ntop(AF_INET6, &(sa6->sin6_addr), buffer, sizeof(buffer));
            printf("Interface: %-16s Address: %s\n", ifa->ifa_name, buffer);
        }
    }
 
    freeifaddrs(ifap);
    return 0;
}
