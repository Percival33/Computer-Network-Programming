//
// Created by Michał Sobiech on 06/12/2023.
//

#ifndef CHECK_IP_TYPE_H
#define CHECK_IP_TYPE_H

#include <arpa/inet.h>

#define IPV4 0
#define IPV6 1
#define INVALID_IP -1

int check_ip_type(char *ip_address);

#endif /* CHECK_IP_TYPE_H */