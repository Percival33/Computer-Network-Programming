//
// Created by Michał Sobiech on 06/12/2023.
//

#include "check_ip_type.h"

int check_ip_type(char *ip_address) {
    int *ipv4_address;
    int *ipv6_address;

    if (inet_pton(AF_INET, ip_address, &ipv4_address) == 1) {
        return IPV4;
    }

    if (inet_pton(AF_INET6, ip_address, &ipv6_address) == 1) {
        return IPV6;
    }

    return INVALID_IP;
}