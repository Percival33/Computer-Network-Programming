//
// Created by Michał Sobiech on 30.11.2023
//

#include "socket.h"
#include <arpa/inet.h>
#include "string.h"
#include "common.h"

int create_binded_socket(int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd  < 0) {
        perror("Failed to create a socket");
        exit(ERROR_FAILED_SOCKET_CREATION);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind a socket");
        exit(ERROR_FAILED_SOCKET_BIND);
    }

    return sockfd;
}
