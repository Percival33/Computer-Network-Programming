#include "print_address.h"

#include <arpa/inet.h>
#include <stdio.h>

#include "common.h"


void print_address(
    struct sockaddr_in *client_address,
    char *client_ip_str,
    int client_ip_str_size
) {
    inet_ntop(AF_INET, &(client_address->sin_addr), client_ip_str, client_ip_str_size);
    printf(LOG_INFO"Data received from %s:%d.\n",
        client_ip_str,
        ntohs(client_address->sin_port)
    );
}