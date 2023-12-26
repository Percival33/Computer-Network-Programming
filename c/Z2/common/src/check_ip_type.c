//
// Created by Michał Sobiech on 06/12/2023.
//

#include "check_ip_type.h"

int check_ip_type(char *ip_address) {
    struct sockaddr_in sa; // IPv4
    struct sockaddr_in6 sa6; // IPv6

    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 1) {
        return IPV4;
    }

    if (inet_pton(AF_INET6, ip_address, &(sa6.sin6_addr)) == 1) {
        return IPV6;
    }

    return INVALID_IP;
}
