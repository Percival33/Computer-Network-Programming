//
// Created by Michał Sobiech on 30.11.2023
//


#ifndef PRINT_ADDRESS_H
#define PRINT_ADDRESS_H

#include <arpa/inet.h>


void print_address(
    struct sockaddr_in *client_address,
    char *client_ip_str,
    int client_ip_str_size
);


#endif
